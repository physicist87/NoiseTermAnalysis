// ExtendPileupTo150.C
//
// Copy an official pileupCalc histogram ("pileup", 100 bins, 0-100) into a
// 150-bin (0-150) histogram, so that it matches the binning of the mu
// histograms made by PUProfile (150 bins, 0-150). Bins above the original
// range stay empty. The input file is not modified.
//
// edm::LumiReWeighting divides the two histograms bin by bin, so both must
// have the same number of bins.
//
// Usage (from pileuInfo/For2025):
//   root -l -b -q '../../PUProfile/ExtendPileupTo150.C("v1_RunC.root","v1_RunC_150bin.root")'

#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include <iostream>

void ExtendPileupTo150(const char* inName, const char* outName, const char* histName = "pileup")
{
   const int nNew = 150;

   TFile* fin = TFile::Open(inName, "READ");
   if (!fin || fin->IsZombie()) {
      std::cout << "ERROR: cannot open " << inName << std::endl;
      return;
   }

   TH1* hin = dynamic_cast<TH1*>(fin->Get(histName));
   if (!hin) {
      std::cout << "ERROR: histogram " << histName << " not found in " << inName << std::endl;
      return;
   }

   const int nOld = hin->GetNbinsX();
   const double lo = hin->GetXaxis()->GetXmin();
   const double hi = hin->GetXaxis()->GetXmax();
   if (lo != 0. || hi != (double)nOld || nOld > nNew) {
      std::cout << "ERROR: expected unit-width bins starting at 0 and at most " << nNew
                << " bins, got " << nOld << " bins in [" << lo << ", " << hi << "]" << std::endl;
      return;
   }

   TFile* fout = TFile::Open(outName, "RECREATE");
   TH1D* hout = new TH1D(histName, hin->GetTitle(), nNew, 0., (double)nNew);
   hout->SetDirectory(fout);

   for (int i = 1; i <= nOld; i++) {
      hout->SetBinContent(i, hin->GetBinContent(i));
      hout->SetBinError(i, hin->GetBinError(i));
   }

   std::cout << inName << " -> " << outName
             << " : bins " << nOld << " -> " << nNew
             << ", integral " << hin->Integral() << " -> " << hout->Integral()
             << ", input overflow " << hin->GetBinContent(nOld + 1) << std::endl;

   hout->Write();
   fout->Close();
   fin->Close();
}
