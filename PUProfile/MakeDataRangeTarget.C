////////////////////////////////////////////////////////////////////////////
//
// MakeDataRangeTarget.C
//
// PU reweighting target for MC restricted to the mu range that the ZeroBias
// data actually populates. The recorded ZeroBias mu distribution
// (DataMu_<sample>.root) is empty above mu ~ 58-61, so the data cannot be
// reweighted into the official profile there. Using the same truncated profile
// as the MC target gives data and MC the same mu distribution.
//
// For each era E: v1_Run<E>_150bin_dataRange.root ("pileup") =
//   official v1_Run<E>_150bin.root with every bin set to 0 where
//   DataMu_<sample>.root is empty, renormalised to unit area.
// Full 2025: DATAPileUp_Run2025All_150bin_dataRange.root ("pileup") =
//   lumi-weighted sum of the five truncated era profiles (lumi from
//   CalNoise/configs/lumi_2025.txt), unit area.
// The official fraction that is dropped is printed per era.
//
// Usage (run from the pileuInfo directory):
//   cd pileuInfo
//   root -l -b -q '../PUProfile/MakeDataRangeTarget.C'
//
////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <map>
#include <sstream>
#include <string>

#include "TFile.h"
#include "TH1D.h"

namespace mdrt {

const char* kEras[5]    = {"C", "D", "E", "F", "G"};
const char* kSamples[5] = {"Data_ZeroBias_Run2025C", "Data_ZeroBias_Run2025Dv1", "Data_ZeroBias_Run2025Ev1",
                           "Data_ZeroBias_Run2025F", "Data_ZeroBias_Run2025Gv1"};

TH1D* Get(const std::string& path, const char* name) {
  TFile* f = TFile::Open(path.c_str());
  if (!f || f->IsZombie()) { printf("MakeDataRangeTarget: cannot open %s\n", path.c_str()); return 0; }
  TH1D* h = (TH1D*)f->Get("pileup");
  if (!h) { printf("MakeDataRangeTarget: no pileup in %s\n", path.c_str()); f->Close(); return 0; }
  TH1D* c = (TH1D*)h->Clone(name);
  c->SetDirectory(0);
  f->Close();
  return c;
}

void Write(const std::string& path, TH1D* h) {
  TFile* f = TFile::Open(path.c_str(), "RECREATE");
  TH1D* c = (TH1D*)h->Clone("pileup");
  c->SetTitle("pileup");
  c->Write();
  f->Close();
  printf("MakeDataRangeTarget: wrote %s\n", path.c_str());
}

}  // namespace mdrt

void MakeDataRangeTarget(const char* dir = "For2025", const char* lumiFile = "../CalNoise/configs/lumi_2025.txt") {
  std::map<std::string, double> lumi;
  std::ifstream in(lumiFile);
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty() || line[0] == '#') continue;
    std::istringstream ss(line);
    std::string era;
    double l;
    if (ss >> era >> l) lumi[era] = l;
  }
  if (lumi.size() < 5) { printf("MakeDataRangeTarget: %s not read\n", lumiFile); return; }

  TH1D* all = 0;
  for (int e = 0; e < 5; ++e) {
    const char* E = mdrt::kEras[e];
    TH1D* off = mdrt::Get(Form("%s/v1_Run%s_150bin.root", dir, E), Form("off_%s", E));
    TH1D* rec = mdrt::Get(Form("%s/DataMu_%s.root", dir, mdrt::kSamples[e]), Form("rec_%s", E));
    if (!off || !rec) return;
    if (off->GetNbinsX() != rec->GetNbinsX()) { printf("MakeDataRangeTarget: binning differs for %s\n", E); return; }
    const double tot = off->Integral();
    double dropped = 0.;
    for (int b = 0; b <= off->GetNbinsX() + 1; ++b) {
      if (rec->GetBinContent(b) > 0.) continue;
      if (b >= 1 && b <= off->GetNbinsX()) dropped += off->GetBinContent(b);
      off->SetBinContent(b, 0.);
      off->SetBinError(b, 0.);
    }
    off->Scale(1. / off->Integral());
    printf("  Run%s: official fraction dropped (no recorded data) = %.4f, max recorded mu bin edge = %.0f\n", E,
           tot > 0. ? dropped / tot : 0., rec->GetBinLowEdge(rec->FindLastBinAbove(0.) + 1));
    mdrt::Write(Form("%s/v1_Run%s_150bin_dataRange.root", dir, E), off);
    TH1D* w = (TH1D*)off->Clone(Form("w_%s", E));
    w->Scale(lumi.at(E));
    if (!all) { all = w; all->SetName("all"); }
    else all->Add(w);
  }
  all->Scale(1. / all->Integral());
  mdrt::Write(Form("%s/DATAPileUp_Run2025All_150bin_dataRange.root", dir), all);
}
