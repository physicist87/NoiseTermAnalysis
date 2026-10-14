#include "NoiseTermCorrections.hpp"

#include <iostream>
#include <sys/stat.h>

namespace {

bool DirExists(const std::string& path) {
   struct stat info;
   if (stat(path.c_str(), &info) != 0) return false;
   return (info.st_mode & S_IFDIR) != 0;
}

// Build the evaluate() argument vector by matching each input's NAME
// (JetA/JetEta/JetPt/Rho/JetPhi/run) rather than a fixed position or count.
template <class Inputs>
std::vector<correction::Variable::Type> BuildArgs(const Inputs& inputs, double pt, double eta, double phi,
                                                  double area, double rho, unsigned int run_number) {
   std::vector<correction::Variable::Type> args;
   args.reserve(inputs.size());
   for (const auto& var : inputs) {
      const std::string& n = var.name();
      if      (n == "JetA")   args.push_back(area);
      else if (n == "JetEta") args.push_back(eta);
      else if (n == "JetPt")  args.push_back(pt);
      else if (n == "Rho")    args.push_back(rho);
      else if (n == "JetPhi") args.push_back(phi);
      else if (n == "run")    args.push_back(static_cast<double>(run_number));
      else {
         std::cout << "[NoiseTermCorrections] WARNING: unrecognized JEC input \"" << n
                   << "\", passing 0.0 - check against the actual json schema" << std::endl;
         args.push_back(0.0);
      }
   }
   return args;
}

} // namespace

NoiseTermCorrections::NoiseTermCorrections(TextReader* reader)
{
   jsonDir_ = ResolveJsonDir();
   std::cout << "[NoiseTermCorrections] jsonDir: " << jsonDir_ << std::endl;

   std::string jecPath  = reader->GetText("JECPath");
   std::string jecName  = reader->GetText("JECName");
   std::string jvetoPath = reader->GetText("JetVetoPath");
   std::string jvetoName = reader->GetText("JetVetoName");
   jvetoKey_ = reader->GetText("JetVetoKey");
   jecLevel_ = reader->GetText("JECLevel");

   jecSet_ = correction::CorrectionSet::from_file(jsonDir_ + jecPath);

   // --- Per-level JEC ---
   // JECLevel (config): "L2L3" (default) or "L1L2L3". A trailing "Res"
   // ("L2L3Res", "L1L2L3Res") additionally applies L2L3Residual.
   // L1FastJet subtracts the pileup offset (rho*area), which is the very
   // quantity the noise term measures, so the default leaves it out.
   // Level names are derived from the compound name by replacing
   // "L1L2L3Res": <prefix>_L1L2L3Res_<suffix> -> <prefix>_L2Relative_<suffix>, ...
   bool useL1 = false, useRes = false;
   if (jecLevel_.empty()) {
      jecLevel_ = "L2L3";
   } else if (jecLevel_ == "L2L3") {
   } else if (jecLevel_ == "L1L2L3") {
      useL1 = true;
   } else if (jecLevel_ == "L2L3Res") {
      useRes = true;
   } else if (jecLevel_ == "L1L2L3Res") {
      useL1 = true; useRes = true;
   } else {
      std::cout << "[NoiseTermCorrections] WARNING: unknown JECLevel \"" << jecLevel_
                << "\", using L2L3" << std::endl;
      jecLevel_ = "L2L3";
   }

   const std::string tok = "_L1L2L3Res_";
   const size_t pos = jecName.find(tok);
   if (pos == std::string::npos) {
      std::cout << "[NoiseTermCorrections] ERROR: JECName \"" << jecName
                << "\" does not contain \"" << tok << "\", cannot derive per-level names" << std::endl;
   } else {
      const std::string prefix = jecName.substr(0, pos);
      const std::string suffix = jecName.substr(pos + tok.size());
      std::vector<std::string> levels;
      if (useL1) levels.push_back("L1FastJet");
      levels.push_back("L2Relative");
      levels.push_back("L3Absolute");
      if (useRes) levels.push_back("L2L3Residual");
      for (const auto& lv : levels) {
         const std::string cname = prefix + "_" + lv + "_" + suffix;
         bool found = false;
         for (auto it = jecSet_->begin(); it != jecSet_->end(); ++it) {
            if (it->first == cname) { jecSteps_.push_back(it->second); found = true; break; }
         }
         if (!found) {
            std::cout << "[NoiseTermCorrections] ERROR: correction \"" << cname << "\" not found" << std::endl;
         } else {
            std::cout << "[NoiseTermCorrections] JEC step: " << cname << " - inputs: ";
            for (const auto& var : jecSteps_.back()->inputs()) std::cout << var.name() << " ";
            std::cout << std::endl;
         }
      }
   }
   std::cout << "[NoiseTermCorrections] JECLevel " << jecLevel_ << ": " << jecSteps_.size()
             << " step(s), L1FastJet " << (useL1 ? "APPLIED" : "NOT applied") << std::endl;

   // --- Jet veto map ---
   jvetoSet_ = correction::CorrectionSet::from_file(jsonDir_ + jvetoPath);
   auto jvetoIt = jvetoSet_->begin();
   bool jvetoFound = false;
   for (auto it = jvetoSet_->begin(); it != jvetoSet_->end(); ++it) {
      if (it->first == jvetoName) { jvetoIt = it; jvetoFound = true; break; }
   }
   if (!jvetoFound) {
      std::cout << "[NoiseTermCorrections] ERROR: correction \"" << jvetoName
                << "\" not found in " << (jsonDir_ + jvetoPath)
                << " - available correction(s):" << std::endl;
      for (auto it = jvetoSet_->begin(); it != jvetoSet_->end(); ++it) {
         std::cout << "    " << it->first << std::endl;
      }
   } else {
      jetVetoMap_ = jvetoIt->second;
   }

   std::cout << "[NoiseTermCorrections] JetVeto: " << jvetoName
             << " (key=" << jvetoKey_ << ")" << std::endl;
}

NoiseTermCorrections::~NoiseTermCorrections() {
}

std::string NoiseTermCorrections::ResolveJsonDir() const {
   // Same 3-step fallback as SSBCorrections (see
   // noiseterm-correctionlib-pattern-reference.md): new CAT cvmfs path,
   // then the legacy jsonpog-integration cvmfs path, then a local
   // directory next to the executable - useful when cvmfs isn't mounted
   // (e.g. running on a laptop instead of lxplus), as long as the same
   // JME/<campaign>/latest/*.json.gz layout is copied there by hand.
   std::vector<std::string> candidates = {
      "/cvmfs/cms-griddata.cern.ch/cat/metadata/",
      "/cvmfs/cms.cern.ch/rsync/cms-nanoAOD/jsonpog-integration/POG/",
      "./jsonpog-integration/POG/"
   };
   for (const auto& c : candidates) {
      if (DirExists(c)) return c;
   }
   std::cout << "[NoiseTermCorrections] WARNING: none of the jsonDir candidates exist, "
                "falling back to \"" << candidates.back() << "\" (file opens will likely fail). "
                "If cvmfs isn't mounted here, copy the needed JME/*/latest/*.json.gz files under "
             << candidates.back() << " relative to the working directory." << std::endl;
   return candidates.back();
}

double NoiseTermCorrections::GetCorrectedJetPt(double raw_pt, double eta, double phi, double area, double rho,
                                                unsigned int run_number) const {
   // Sequential, each level evaluated at the pt after the previous levels,
   // like a compound correction.
   if (!jecSteps_.empty()) {
      double pt = raw_pt;
      for (const auto& step : jecSteps_) {
         pt *= step->evaluate(BuildArgs(step->inputs(), pt, eta, phi, area, rho, run_number));
      }
      return pt;
   }

   return raw_pt; // no JEC step loaded - no-op rather than a hard crash
}

bool NoiseTermCorrections::ShouldVetoJet(double eta, double phi) const {
   if (!jetVetoMap_) return false; // not loaded - never veto rather than a hard crash
   double val = jetVetoMap_->evaluate({jvetoKey_, eta, phi});
   return val > 0.0;
}
