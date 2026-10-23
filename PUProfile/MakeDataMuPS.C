////////////////////////////////////////////////////////////////////////////
//
// MakeDataMuPS.C
//
// Denominator for the data PU reweighting with the ZeroBias prescale kept as
// an event weight. The data event weight is w_PU(mu) x prescale, so w_PU must
// map the PRESCALE-WEIGHTED recorded mu distribution onto the official
// profile. The first DataMu_<sample>.root files were filled without the
// prescale, so the reweighted data did not close (Run2025C, D, F, where the
// prescale changes between runs).
//
// The prescale-weighted distribution is recovered exactly from an existing
// production: h_mu_9 (top-level file of each data sample) was filled with
// w_PU,old(mu) x prescale, and w_PU,old is constant inside each unit mu bin, so
//   sum of prescales in bin i = h_mu_9[i] / w_PU,old[i],
//   w_PU,old[i] = (official[i]/sum official) / (DataMu[i]/sum DataMu)   (as edm::LumiReWeighting).
// Output: For2025/DataMuPS_<sample>.root, histogram "pileup", same binning
// as DataMu_<sample>.root (150 unit bins, 0-150).
//
// Usage (run from the pileuInfo directory):
//   cd pileuInfo
//   root -l -b -q '../PUProfile/MakeDataMuPS.C("/Users/seungkyuha/SK_Analysis_v2/ServiceWork/JERC/ForRun3/AnalysisCode/CalNoise_ForRun3/Run3_2025_PUPPI_v5_Merged")'
//
////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "TFile.h"
#include "TH1D.h"

namespace mdps {

const char* kEras[5]    = {"C", "D", "E", "F", "G"};
const char* kSamples[5] = {"Data_ZeroBias_Run2025C", "Data_ZeroBias_Run2025Dv1", "Data_ZeroBias_Run2025Ev1",
                           "Data_ZeroBias_Run2025F", "Data_ZeroBias_Run2025Gv1"};

TH1D* Get(const char* path, const char* hname, const char* newname) {
  TFile* f = TFile::Open(path);
  if (!f || f->IsZombie()) { printf("MakeDataMuPS: cannot open %s\n", path); return 0; }
  TH1D* h = (TH1D*)f->Get(hname);
  if (!h) { printf("MakeDataMuPS: %s not in %s\n", hname, path); f->Close(); return 0; }
  TH1D* c = (TH1D*)h->Clone(newname);
  c->SetDirectory(0);
  f->Close();
  return c;
}

}  // namespace mdps

void MakeDataMuPS(const char* mergedBase, const char* dir = "For2025") {
  for (int e = 0; e < 5; ++e) {
    const char* E = mdps::kEras[e];
    const char* S = mdps::kSamples[e];
    TH1D* off = mdps::Get(Form("%s/v1_Run%s_150bin.root", dir, E), "pileup", Form("off_%s", E));
    TH1D* rec = mdps::Get(Form("%s/DataMu_%s.root", dir, S), "pileup", Form("rec_%s", E));
    TH1D* hmu = mdps::Get(Form("%s/Central/%s/%s.root", mergedBase, S, S), "h_mu_9", Form("hmu_%s", E));
    if (!off || !rec || !hmu) return;
    const double so = off->Integral(), sr = rec->Integral();
    TH1D* ps = (TH1D*)rec->Clone("pileup");
    ps->Reset();
    double nZeroW = 0.;
    for (int b = 1; b <= rec->GetNbinsX(); ++b) {
      const double lo = rec->GetBinLowEdge(b), hi = lo + rec->GetBinWidth(b);
      // h_mu_9 has finer bins (0.1) over 0-100: sum the sub-bins of this unit bin
      double sum = 0.;
      for (int k = 1; k <= hmu->GetNbinsX(); ++k) {
        const double x = hmu->GetBinCenter(k);
        if (x >= lo && x < hi) sum += hmu->GetBinContent(k);
      }
      if (rec->GetBinContent(b) <= 0.) continue;
      const double wold = (off->GetBinContent(b) / so) / (rec->GetBinContent(b) / sr);
      if (wold <= 0.) { nZeroW += rec->GetBinContent(b); continue; }  // official empty: weight 0, prescale sum unknown
      ps->SetBinContent(b, sum / wold);
      ps->SetBinError(b, 0.);
    }
    TFile* f = TFile::Open(Form("%s/DataMuPS_%s.root", dir, S), "RECREATE");
    ps->Write();
    f->Close();
    printf("MakeDataMuPS: Run%s  <prescale> = %.0f, recorded events in bins with empty official profile = %.0f -> %s/DataMuPS_%s.root\n",
           E, ps->Integral() / sr, nZeroW, dir, S);
  }
}
