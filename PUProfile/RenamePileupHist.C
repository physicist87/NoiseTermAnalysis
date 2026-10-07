////////////////////////////////////////////////////////////////////////////
//
// RenamePileupHist.C
//
// One-shot fixup macro. PUProfile.cpp originally wrote its output histogram
// as "h_mu"; it now writes "pileup" to match the official pileupCalc.py
// convention (confirmed against v1_RunC.root: KEY: TH1D pileup;1 pileup).
// This macro renames the histogram in an already-produced PUProfile output
// file IN PLACE, so the 6 files made before that switch don't need to be
// regenerated from xrootd.
//
// It is a no-op (with a message) if the file already has "pileup" and no
// "h_mu", so it is safe to run again by accident.
//
// Usage (one file):
//   root -l -b -q 'RenamePileupHist.C("For2025/PileupMC.root")'
//
// Usage (all 6 known files, from a shell, run from the pileuInfo directory):
//   for f in PileupMC.root \
//            DataMu_Data_ZeroBias_Run2025C.root \
//            DataMu_Data_ZeroBias_Run2025Dv1.root \
//            DataMu_Data_ZeroBias_Run2025Ev1.root \
//            DataMu_Data_ZeroBias_Run2025F.root \
//            DataMu_Data_ZeroBias_Run2025Gv1.root; do
//     root -l -b -q 'RenamePileupHist.C("For2025/'"$f"'")'
//   done
//
////////////////////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TH1.h"

void RenamePileupHist(const char *fname)
{
   TFile *f = TFile::Open(fname, "UPDATE");
   if (!f || f->IsZombie())
   {
      printf("ERROR: could not open %s\n", fname);
      return;
   }

   TH1 *h_old = (TH1*)f->Get("h_mu");
   if (!h_old)
   {
      TH1 *h_check = (TH1*)f->Get("pileup");
      if (h_check)
      {
         printf("%s: already has 'pileup', nothing to do.\n", fname);
      }
      else
      {
         printf("WARNING: %s has neither 'h_mu' nor 'pileup' - check by hand.\n", fname);
      }
      f->Close();
      return;
   }

   TH1 *h_new = (TH1*)h_old->Clone("pileup");
   h_new->SetDirectory(0);

   f->Delete("h_mu;*");   // remove every cycle of the old key
   f->cd();
   h_new->Write("pileup");
   f->Close();

   printf("%s: renamed h_mu -> pileup\n", fname);
}
