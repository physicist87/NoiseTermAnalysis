////////////////////////////////////////////////////////////////////////////
//
// PUProfile
//
// Standalone, minimal companion to NoiseTermAnalysis. It reads only the
// "mu" branch (true number of pileup interactions) from the Run3 2025
// PUPPI noise-study ntuples and fills a single histogram with it - nothing
// else (no correctionlib, no JEC, no random cone search). It exists so
// pileup-profile production can run as a much lighter, faster Condor job
// than the full NoiseTerm analysis.
//
// Works for both data and MC input lists - the "mu" branch is the same
// OffsetTreeMaker branch used by NoiseTerm.cpp (analysis/NoiseTree.h,
// Float_t mu), so no per-sample branching is needed here.
//
// The output histogram is named "pileup" - matching the official
// pileupCalc.py convention (confirmed against v1_RunC.root) - so it can be
// used interchangeably with an official dataPileupHistogram-*.root file
// without any code needing to branch on which kind of file it is.
//
// Usage:
//   ./PUProfile <input_list> <output_root_file>
//
//   <input_list>       one ROOT file path (local or root://) per line
//   <output_root_file> output file containing a single histogram "pileup"
//
////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>

#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>

using namespace std;

int main(int argc, char **argv)
{
   if (argc < 3)
   {
      cout << "Usage: " << argv[0] << " <input_list> <output_root_file>" << endl;
      return 1;
   }

   string inputList  = argv[1];
   string outputFile = argv[2];

   cout << "Input list  = " << inputList  << endl;
   cout << "Output file = " << outputFile << endl;

   ifstream listFile(inputList.c_str());
   if (!listFile.is_open())
   {
      cout << "ERROR: could not open input list: " << inputList << endl;
      return 1;
   }

   // Same tree name as NoiseTerm.cpp / main_noiseterm.cpp (analysis/NoiseTree.h).
   TChain *ch = new TChain("pf/T");

   string line;
   int nFiles = 0;
   while (getline(listFile, line))
   {
      if (line.empty()) { continue; }
      ch->Add(line.c_str());
      nFiles++;
      cout << "Added: " << line << endl;
   }
   listFile.close();

   cout << "Total files added: " << nFiles << endl;

   if (nFiles == 0)
   {
      cout << "ERROR: input list had no usable file entries." << endl;
      return 1;
   }

   Long64_t nEntries = ch->GetEntries();
   cout << "Total events in chain: " << nEntries << endl;

   // Only "mu" is needed - disable every other branch so xrootd reads stay
   // cheap. This is the whole point of PUProfile being separate from
   // NoiseTerm (which binds the full branch set via analysis/NoiseTree.h).
   ch->SetBranchStatus("*", 0);
   ch->SetBranchStatus("mu", 1);

   Float_t mu = 0;
   ch->SetBranchAddress("mu", &mu);

   // Official pileupCalc convention is 0-100 with 1 unit per bin. Extended
   // to 0-150 here since SingleNeutrino_Flat2025 is a flat-PU0to120 MC
   // sample - both the data-side and MC-side profiles use the same 0-150
   // binning so LumiReWeighting's bin-by-bin Divide() lines up cleanly;
   // the empty 100-150 tail on the data side just floors the weight there
   // to ~0, which is the desired behavior.
   //
   // Histogram name "pileup" matches the official convention (see comment
   // at the top of this file) - not "h_mu".
   TH1D *h_pileup = new TH1D("pileup", "Pileup profile;true interactions (mu);Events", 150, 0, 150);
   h_pileup->Sumw2();

   for (Long64_t i = 0; i < nEntries; ++i)
   {
      ch->GetEntry(i);
      h_pileup->Fill(mu);
   }

   cout << "Processed " << nEntries << " events." << endl;

   TFile *fout = new TFile(outputFile.c_str(), "RECREATE");
   h_pileup->Write();
   fout->Close();

   cout << "Wrote output: " << outputFile << endl;

   delete ch;
   return 0;
}
