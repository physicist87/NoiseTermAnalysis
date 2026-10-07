#include "NoiseTermCorrections.hpp"

#include <iostream>
#include <sys/stat.h>

namespace {

bool DirExists(const std::string& path) {
   struct stat info;
   if (stat(path.c_str(), &info) != 0) return false;
   return (info.st_mode & S_IFDIR) != 0;
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

   // --- JEC (compound correction: L1FastJet+L2Relative+L3Absolute, +
   // L2L3Residual for DATA - already combined in the json, see
   // configs/Run3_2025_PUPPI_v1/) ---
   jecSet_ = correction::CorrectionSet::from_file(jsonDir_ + jecPath);
   const auto& compoundMap = jecSet_->compound();
   auto jecIt = compoundMap.find(jecName);
   if (jecIt == compoundMap.end()) {
      std::cout << "[NoiseTermCorrections] ERROR: compound correction \"" << jecName
                << "\" not found in " << (jsonDir_ + jecPath) << std::endl;
   } else {
      jec_ = jecIt->second;
   }

   if (jec_) {
      std::cout << "[NoiseTermCorrections] JEC: " << jecName << " - inputs (" << jec_->inputs().size() << "): ";
      for (const auto& var : jec_->inputs()) std::cout << var.name() << " ";
      std::cout << std::endl;
   }

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
   if (!jec_) return raw_pt; // not loaded - no-op rather than a hard crash

   // Build the evaluate() argument vector by matching each input's NAME
   // (JetA/JetEta/JetPt/Rho/JetPhi/run) rather than assuming a fixed
   // position or count. Verified necessary against the real
   // Summer24Prompt25_V3 jet_jerc.json.gz: DATA needs 6 inputs including
   // JetPhi and run, MC needs 5 (no run) but still needs JetPhi - neither
   // matches the older 4/5-input {area,eta,pt,rho[,run]} pattern used by
   // some earlier (e.g. Run2 UL18) campaigns, so this must not be
   // hardcoded positionally.
   std::vector<correction::Variable::Type> args;
   args.reserve(jec_->inputs().size());
   for (const auto& var : jec_->inputs()) {
      const std::string& n = var.name();
      if      (n == "JetA")   args.push_back(area);
      else if (n == "JetEta") args.push_back(eta);
      else if (n == "JetPt")  args.push_back(raw_pt);
      else if (n == "Rho")    args.push_back(rho);
      else if (n == "JetPhi") args.push_back(phi);
      else if (n == "run")    args.push_back(static_cast<double>(run_number));
      else {
         std::cout << "[NoiseTermCorrections] WARNING: unrecognized JEC input \"" << n
                   << "\", passing 0.0 - check GetCorrectedJetPt() against the actual json schema"
                   << std::endl;
         args.push_back(0.0);
      }
   }
   double sf = jec_->evaluate(args);
   return raw_pt * sf;
}

bool NoiseTermCorrections::ShouldVetoJet(double eta, double phi) const {
   if (!jetVetoMap_) return false; // not loaded - never veto rather than a hard crash
   double val = jetVetoMap_->evaluate({jvetoKey_, eta, phi});
   return val > 0.0;
}
