// NoiseTermCorrections - JEC + jet veto map via correctionlib.
//
// Scoped-down port of SSBCorrections (from SSBNanoAODANCode, ttbar CPV):
// only what NoiseTerm's random-cone noise-term measurement needs. No JER
// (random-cone jets are pseudo-jets, not reconstructed jets - JER
// smearing/SF has no physical target here, per Seungkyu's decision), no
// lepton/b-tag SFs.
//
// Config keys read from the TextReader (see configs/Run3_2025_PUPPI_v1/):
//   JECPath, JECName, JetAlgoTag, JECLevel
//   JetVetoPath, JetVetoName, JetVetoKey, JetVetoType

#ifndef NoiseTermCorrections_h
#define NoiseTermCorrections_h

#include <string>
#include <memory>

#include "correction.h"
#include "./TextReader/TextReader.hpp"

class NoiseTermCorrections {

public:
   NoiseTermCorrections(TextReader* reader);
   ~NoiseTermCorrections();

   // raw_pt: uncorrected jet pt (GeV). area: jet area (pi*R^2 for a cone).
   // rho: event energy density. run_number: only used when the loaded JEC
   // compound correction's schema includes a "run" input (2025 V3 DATA
   // does, MC doesn't) - matched by input NAME at evaluate() time (see
   // .cpp), not by a hardcoded position/count, since that already proved
   // wrong once: verified directly against the real jet_jerc.json.gz that
   // the 2025 V3 compound needs JetA, JetEta, JetPt, Rho, JetPhi, and
   // (DATA only) run - five inputs for MC, six for DATA, JetPhi included
   // for both (a genuine physics input here, not optional - unlike some
   // older/Run2 campaigns' compounds which didn't take phi at all).
   double GetCorrectedJetPt(double raw_pt, double eta, double phi, double area, double rho,
                             unsigned int run_number = 0) const;

   // Direct eta/phi lookup against the jet veto map, no jetID/EM-fraction
   // preselection - random-cone jets are pseudo-jets, those quantities are
   // not meaningful for them (Seungkyu's design decision, 2026-09-09).
   bool ShouldVetoJet(double eta, double phi) const;

   const std::string& JsonDir() const { return jsonDir_; }

private:
   std::string ResolveJsonDir() const;

   std::string jsonDir_;

   std::unique_ptr<correction::CorrectionSet> jecSet_;
   correction::CompoundCorrection::Ref jec_;

   std::unique_ptr<correction::CorrectionSet> jvetoSet_;
   correction::Correction::Ref jetVetoMap_;
   std::string jvetoKey_;
};

#endif // NoiseTermCorrections_h
