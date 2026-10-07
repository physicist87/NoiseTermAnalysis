//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep  9 14:35:06 2026 by ROOT version 6.32.13
// from TTree T/Offset Tree
// found on file: /pnfs/knu.ac.kr/data/cms/store/user/sha/JERNoiseStudy/Run3/2025/ZB/ZeroBias/PUPPINoiseStudy_Run2025F_v1_v2/260807_154935/0001/Offset_Data_Run2025F_1001.root
//////////////////////////////////////////////////////////

#ifndef NoiseTree_h
#define NoiseTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"
#include "vector"

// MakeClass generates bare "vector<T>" below (no std:: prefix), relying on
// something already having pulled "using namespace std" into scope. On
// lxplus (gcc + libstdc++) some ROOT header leaks that globally, so it
// happened to compile; on macOS (clang + libc++) it does not, so this is
// added explicitly to keep both platforms working.
using namespace std;

class NoiseTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           run;
   Int_t           lumi;
   Int_t           bx;
   ULong64_t       event;
   vector<int>     *pf_type;
   vector<int>     *pf_fromPV;
   vector<bool>    *pf_fromPVs;
   vector<float>   *pf_pt;
   vector<float>   *pf_eta;
   vector<float>   *pf_phi;
   vector<float>   *pf_et;
   vector<float>   *pf_energy;
   vector<float>   *pf_m;
   vector<float>   *pf_puppiW;
   vector<float>   *pf_hcalDepth1;
   vector<float>   *pf_hcalDepth2;
   vector<float>   *pf_hcalDepth3;
   vector<float>   *pf_hcalDepth4;
   vector<float>   *pf_hcalDepth5;
   vector<float>   *pf_hcalDepth6;
   vector<float>   *pf_hcalDepth7;
   Float_t         mu;
   Float_t         muWeight;
   Float_t         rho;
   Float_t         rhoC0;
   Float_t         rhoCC;
   Float_t         rhoCentral;
   Float_t         rhoCentralCalo;
   Int_t           nPVall;
   Int_t           nPV;
   Float_t         pv_ndof[256];   //[nPVall]  // NOTE: bumped from MakeClass's file-specific 63/150 - see comment below
   Float_t         pv_z[256];   //[nPVall]
   Float_t         pv_rho[256];   //[nPVall]
   Int_t           nEta;
   Float_t         energy[82];   //[nEta]
   Float_t         et[82];   //[nEta]
   Float_t         eRMS[82];   //[nEta]
   Float_t         etMED[82];   //[nEta]
   Float_t         etMEAN[82];   //[nEta]
   Float_t         et_gme[18][11];
   Float_t         et_twopi[18][11];
   Float_t         rho_gme;
   Float_t         et_twopi_gen[18][11];   // MC only - 0 if the open file is Data
   Float_t         rho_gme_gen;             // MC only - 0 if the open file is Data
   Float_t         et_twopi_det[18][11];    // MC only - 0 if the open file is Data
   Float_t         rho_gme_det;             // MC only - 0 if the open file is Data
   Float_t         et_twopi_scaled[18][11]; // MC only - 0 if the open file is Data
   Float_t         rho_gme_scaled;          // MC only - 0 if the open file is Data
   Float_t         weight;                  // MC only - 1.0 if the open file is Data
   Int_t           nGenParticles;           // MC only - -1 if the open file is Data
   Int_t           idHardScat1;             // MC only - -999 if the open file is Data
   Int_t           idHardScat2;             // MC only - -999 if the open file is Data
   Int_t           idxHardScat;             // MC only - -1 if the open file is Data
   UChar_t         fchm[82];   //[nEta]
   UChar_t         fchu[82];   //[nEta]
   UChar_t         fnh[82];   //[nEta]
   UChar_t         fne[82];   //[nEta]
   UChar_t         fhfh[82];   //[nEta]
   UChar_t         fhfe[82];   //[nEta]
   UChar_t         flep[82];   //[nEta]
   UChar_t         funtrk[82];   //[nEta]
   Float_t         ht;
   Int_t           nJets;
   vector<float>   *jet_eta;
   vector<float>   *jet_phi;
   vector<float>   *jet_pt;
   vector<float>   *jet_area;
   vector<float>   *jet_ch;
   vector<float>   *jet_nh;
   vector<float>   *jet_ne;
   vector<float>   *jet_hfh;
   vector<float>   *jet_hfe;
   vector<int>     *rc1jet_numpf;
   vector<float>   *rc1jet_eta;
   vector<float>   *rc1jet_phi;
   vector<float>   *rc1jet_pt;
   vector<float>   *rc1jet_energy;
   vector<float>   *rc1jet_cor;
   vector<float>   *rc1jet_coruncUp;
   vector<float>   *rc1jet_coruncDown;
   vector<float>   *rc1jet_corL1;
   vector<float>   *rc1jet_corL1RC;
   vector<float>   *rc1jet_corL1Nega;
   vector<float>   *rc1jet_corL1RCNega;
   vector<float>   *rc1jet_corAll;
   vector<float>   *rc1jet_corAllRC;
   vector<float>   *rc1jet_corAllNega;
   vector<float>   *rc1jet_corAllRCNega;
   vector<int>     *rc2jet_numpf;
   vector<float>   *rc2jet_eta;
   vector<float>   *rc2jet_phi;
   vector<float>   *rc2jet_pt;
   vector<float>   *rc2jet_energy;
   vector<float>   *rc2jet_cor;
   vector<float>   *rc2jet_coruncUp;
   vector<float>   *rc2jet_coruncDown;
   vector<float>   *rc2jet_corL1;
   vector<float>   *rc2jet_corL1RC;
   vector<float>   *rc2jet_corL1Nega;
   vector<float>   *rc2jet_corL1RCNega;
   vector<float>   *rc2jet_corAll;
   vector<float>   *rc2jet_corAllRC;
   vector<float>   *rc2jet_corAllNega;
   vector<float>   *rc2jet_corAllRCNega;
   vector<float>   *rc_diff;
   vector<bool>    *rc_ispair;
   vector<bool>    *rc_ispair_puppi;
   vector<int>     *rc1jet_puppi_numpf;
   vector<int>     *rc2jet_puppi_numpf;
   vector<float>   *rc1jet_puppi_eta;
   vector<float>   *rc2jet_puppi_eta;
   vector<float>   *rc1jet_puppi_phi;
   vector<float>   *rc2jet_puppi_phi;
   vector<float>   *rc1jet_puppi_pt;
   vector<float>   *rc2jet_puppi_pt;
   vector<float>   *rc1jet_puppi_energy;
   vector<float>   *rc2jet_puppi_energy;
   vector<float>   *rc1jet_puppi_cor;
   vector<float>   *rc2jet_puppi_cor;
   vector<float>   *rc1jet_puppi_coruncUp;
   vector<float>   *rc2jet_puppi_coruncUp;
   vector<float>   *rc1jet_puppi_coruncDown;
   vector<float>   *rc2jet_puppi_coruncDown;
   vector<float>   *rc1jet_puppi_corL1;
   vector<float>   *rc2jet_puppi_corL1;
   vector<float>   *rc1jet_puppi_corL1RC;
   vector<float>   *rc2jet_puppi_corL1RC;
   vector<float>   *rc1jet_puppi_corL1Nega;
   vector<float>   *rc2jet_puppi_corL1Nega;
   vector<float>   *rc1jet_puppi_corL1RCNega;
   vector<float>   *rc2jet_puppi_corL1RCNega;
   vector<float>   *rc1jet_puppi_corAll;
   vector<float>   *rc2jet_puppi_corAll;
   vector<float>   *rc1jet_puppi_corAllRC;
   vector<float>   *rc2jet_puppi_corAllRC;
   vector<float>   *rc1jet_puppi_corAllNega;
   vector<float>   *rc2jet_puppi_corAllNega;
   vector<float>   *rc1jet_puppi_corAllRCNega;
   vector<float>   *rc2jet_puppi_corAllRCNega;
   vector<float>   *rc1jet_puppiET;
   vector<float>   *rc2jet_puppiET;
   vector<float>   *rc_puppi_diff;
   vector<float>   *rc1jet_puppiWsum;
   vector<float>   *rc2jet_puppiWsum;
   vector<float>   *rc1jet_et_nh;
   vector<float>   *rc2jet_et_nh;
   vector<float>   *rc1jet_et_ne;
   vector<float>   *rc2jet_et_ne;
   vector<float>   *rc1jet_et_chm;
   vector<float>   *rc2jet_et_chm;
   vector<float>   *rc1jet_et_chu;
   vector<float>   *rc2jet_et_chu;
   vector<float>   *rc1jet_et_hfh;
   vector<float>   *rc2jet_et_hfh;
   vector<float>   *rc1jet_et_hfe;
   vector<float>   *rc2jet_et_hfe;
   vector<float>   *rc1jet_puppiET_nh;
   vector<float>   *rc2jet_puppiET_nh;
   vector<float>   *rc1jet_puppiET_ne;
   vector<float>   *rc2jet_puppiET_ne;
   vector<float>   *rc1jet_puppiET_chm;
   vector<float>   *rc2jet_puppiET_chm;
   vector<float>   *rc1jet_puppiET_hfh;
   vector<float>   *rc2jet_puppiET_hfh;
   vector<float>   *rc1jet_puppiET_hfe;
   vector<float>   *rc2jet_puppiET_hfe;
   vector<float>   *rc1jet_hcalDepth1;
   vector<float>   *rc2jet_hcalDepth1;
   vector<float>   *rc1jet_hcalDepth2;
   vector<float>   *rc2jet_hcalDepth2;
   vector<float>   *rc1jet_hcalDepth3;
   vector<float>   *rc2jet_hcalDepth3;
   vector<float>   *rc1jet_hcalDepth4;
   vector<float>   *rc2jet_hcalDepth4;
   vector<float>   *rc1jet_hcalDepth5;
   vector<float>   *rc2jet_hcalDepth5;
   vector<float>   *rc1jet_hcalDepth6;
   vector<float>   *rc2jet_hcalDepth6;
   vector<float>   *rc1jet_hcalDepth7;
   vector<float>   *rc2jet_hcalDepth7;
   vector<bool>    *trigger_pass;
   vector<bool>    *trigger_error;
   vector<bool>    *trigger_run;
   vector<double>  *trigger_prescale;
   vector<string>  *trigger_name;
   Float_t         genht;
   Int_t           nGenJets;
   vector<float>   *genjet_eta;
   vector<float>   *genjet_phi;
   vector<float>   *genjet_pt;
   vector<float>   *genjet_area;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_event;   //!
   TBranch        *b_pf_type;   //!
   TBranch        *b_pf_fromPV;   //!
   TBranch        *b_pf_fromPVs;   //!
   TBranch        *b_pf_pt;   //!
   TBranch        *b_pf_eta;   //!
   TBranch        *b_pf_phi;   //!
   TBranch        *b_pf_et;   //!
   TBranch        *b_pf_energy;   //!
   TBranch        *b_pf_m;   //!
   TBranch        *b_pf_puppiW;   //!
   TBranch        *b_pf_hcalDepth1;   //!
   TBranch        *b_pf_hcalDepth2;   //!
   TBranch        *b_pf_hcalDepth3;   //!
   TBranch        *b_pf_hcalDepth4;   //!
   TBranch        *b_pf_hcalDepth5;   //!
   TBranch        *b_pf_hcalDepth6;   //!
   TBranch        *b_pf_hcalDepth7;   //!
   TBranch        *b_mu;   //!
   TBranch        *b_muWeight;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_rhoC0;   //!
   TBranch        *b_rhoCC;   //!
   TBranch        *b_rhoCentral;   //!
   TBranch        *b_rhoCentralCalo;   //!
   TBranch        *b_nPVall;   //!
   TBranch        *b_nPV;   //!
   TBranch        *b_pv_ndof;   //!
   TBranch        *b_pv_z;   //!
   TBranch        *b_pv_rho;   //!
   TBranch        *b_nEta;   //!
   TBranch        *b_energy;   //!
   TBranch        *b_et;   //!
   TBranch        *b_eRMS;   //!
   TBranch        *b_etMED;   //!
   TBranch        *b_etMEAN;   //!
   TBranch        *b_et_gme;   //!
   TBranch        *b_et_twopi;   //!
   TBranch        *b_rho_gme;   //!
   TBranch        *b_et_twopi_gen;   //! nullptr if the open file is Data
   TBranch        *b_rho_gme_gen;   //! nullptr if the open file is Data
   TBranch        *b_et_twopi_det;   //! nullptr if the open file is Data
   TBranch        *b_rho_gme_det;   //! nullptr if the open file is Data
   TBranch        *b_et_twopi_scaled;   //! nullptr if the open file is Data
   TBranch        *b_rho_gme_scaled;   //! nullptr if the open file is Data
   TBranch        *b_weight;   //! nullptr if the open file is Data
   TBranch        *b_nGenParticles;   //! nullptr if the open file is Data
   TBranch        *b_idHardScat1;   //! nullptr if the open file is Data
   TBranch        *b_idHardScat2;   //! nullptr if the open file is Data
   TBranch        *b_idxHardScat;   //! nullptr if the open file is Data
   TBranch        *b_fchm;   //!
   TBranch        *b_fchu;   //!
   TBranch        *b_fnh;   //!
   TBranch        *b_fne;   //!
   TBranch        *b_fhfh;   //!
   TBranch        *b_fhfe;   //!
   TBranch        *b_flep;   //!
   TBranch        *b_funtrk;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_area;   //!
   TBranch        *b_jet_ch;   //!
   TBranch        *b_jet_nh;   //!
   TBranch        *b_jet_ne;   //!
   TBranch        *b_jet_hfh;   //!
   TBranch        *b_jet_hfe;   //!
   TBranch        *b_rc1jet_numpf;   //!
   TBranch        *b_rc1jet_eta;   //!
   TBranch        *b_rc1jet_phi;   //!
   TBranch        *b_rc1jet_pt;   //!
   TBranch        *b_rc1jet_energy;   //!
   TBranch        *b_rc1jet_cor;   //!
   TBranch        *b_rc1jet_coruncUp;   //!
   TBranch        *b_rc1jet_coruncDown;   //!
   TBranch        *b_rc1jet_corL1;   //!
   TBranch        *b_rc1jet_corL1RC;   //!
   TBranch        *b_rc1jet_corL1Nega;   //!
   TBranch        *b_rc1jet_corL1RCNega;   //!
   TBranch        *b_rc1jet_corAll;   //!
   TBranch        *b_rc1jet_corAllRC;   //!
   TBranch        *b_rc1jet_corAllNega;   //!
   TBranch        *b_rc1jet_corAllRCNega;   //!
   TBranch        *b_rc2jet_numpf;   //!
   TBranch        *b_rc2jet_eta;   //!
   TBranch        *b_rc2jet_phi;   //!
   TBranch        *b_rc2jet_pt;   //!
   TBranch        *b_rc2jet_energy;   //!
   TBranch        *b_rc2jet_cor;   //!
   TBranch        *b_rc2jet_coruncUp;   //!
   TBranch        *b_rc2jet_coruncDown;   //!
   TBranch        *b_rc2jet_corL1;   //!
   TBranch        *b_rc2jet_corL1RC;   //!
   TBranch        *b_rc2jet_corL1Nega;   //!
   TBranch        *b_rc2jet_corL1RCNega;   //!
   TBranch        *b_rc2jet_corAll;   //!
   TBranch        *b_rc2jet_corAllRC;   //!
   TBranch        *b_rc2jet_corAllNega;   //!
   TBranch        *b_rc2jet_corAllRCNega;   //!
   TBranch        *b_rc_diff;   //!
   TBranch        *b_rc_ispair;   //!
   TBranch        *b_rc_ispair_puppi;   //!
   TBranch        *b_rc1jet_puppi_numpf;   //!
   TBranch        *b_rc2jet_puppi_numpf;   //!
   TBranch        *b_rc1jet_puppi_eta;   //!
   TBranch        *b_rc2jet_puppi_eta;   //!
   TBranch        *b_rc1jet_puppi_phi;   //!
   TBranch        *b_rc2jet_puppi_phi;   //!
   TBranch        *b_rc1jet_puppi_pt;   //!
   TBranch        *b_rc2jet_puppi_pt;   //!
   TBranch        *b_rc1jet_puppi_energy;   //!
   TBranch        *b_rc2jet_puppi_energy;   //!
   TBranch        *b_rc1jet_puppi_cor;   //!
   TBranch        *b_rc2jet_puppi_cor;   //!
   TBranch        *b_rc1jet_puppi_coruncUp;   //!
   TBranch        *b_rc2jet_puppi_coruncUp;   //!
   TBranch        *b_rc1jet_puppi_coruncDown;   //!
   TBranch        *b_rc2jet_puppi_coruncDown;   //!
   TBranch        *b_rc1jet_puppi_corL1;   //!
   TBranch        *b_rc2jet_puppi_corL1;   //!
   TBranch        *b_rc1jet_puppi_corL1RC;   //!
   TBranch        *b_rc2jet_puppi_corL1RC;   //!
   TBranch        *b_rc1jet_puppi_corL1Nega;   //!
   TBranch        *b_rc2jet_puppi_corL1Nega;   //!
   TBranch        *b_rc1jet_puppi_corL1RCNega;   //!
   TBranch        *b_rc2jet_puppi_corL1RCNega;   //!
   TBranch        *b_rc1jet_puppi_corAll;   //!
   TBranch        *b_rc2jet_puppi_corAll;   //!
   TBranch        *b_rc1jet_puppi_corAllRC;   //!
   TBranch        *b_rc2jet_puppi_corAllRC;   //!
   TBranch        *b_rc1jet_puppi_corAllNega;   //!
   TBranch        *b_rc2jet_puppi_corAllNega;   //!
   TBranch        *b_rc1jet_puppi_corAllRCNega;   //!
   TBranch        *b_rc2jet_puppi_corAllRCNega;   //!
   TBranch        *b_rc1jet_puppiET;   //!
   TBranch        *b_rc2jet_puppiET;   //!
   TBranch        *b_rc_puppi_diff;   //!
   TBranch        *b_rc1jet_puppiWsum;   //!
   TBranch        *b_rc2jet_puppiWsum;   //!
   TBranch        *b_rc1jet_et_nh;   //!
   TBranch        *b_rc2jet_et_nh;   //!
   TBranch        *b_rc1jet_et_ne;   //!
   TBranch        *b_rc2jet_et_ne;   //!
   TBranch        *b_rc1jet_et_chm;   //!
   TBranch        *b_rc2jet_et_chm;   //!
   TBranch        *b_rc1jet_et_chu;   //!
   TBranch        *b_rc2jet_et_chu;   //!
   TBranch        *b_rc1jet_et_hfh;   //!
   TBranch        *b_rc2jet_et_hfh;   //!
   TBranch        *b_rc1jet_et_hfe;   //!
   TBranch        *b_rc2jet_et_hfe;   //!
   TBranch        *b_rc1jet_puppiET_nh;   //!
   TBranch        *b_rc2jet_puppiET_nh;   //!
   TBranch        *b_rc1jet_puppiET_ne;   //!
   TBranch        *b_rc2jet_puppiET_ne;   //!
   TBranch        *b_rc1jet_puppiET_chm;   //!
   TBranch        *b_rc2jet_puppiET_chm;   //!
   TBranch        *b_rc1jet_puppiET_hfh;   //!
   TBranch        *b_rc2jet_puppiET_hfh;   //!
   TBranch        *b_rc1jet_puppiET_hfe;   //!
   TBranch        *b_rc2jet_puppiET_hfe;   //!
   TBranch        *b_rc1jet_hcalDepth1;   //!
   TBranch        *b_rc2jet_hcalDepth1;   //!
   TBranch        *b_rc1jet_hcalDepth2;   //!
   TBranch        *b_rc2jet_hcalDepth2;   //!
   TBranch        *b_rc1jet_hcalDepth3;   //!
   TBranch        *b_rc2jet_hcalDepth3;   //!
   TBranch        *b_rc1jet_hcalDepth4;   //!
   TBranch        *b_rc2jet_hcalDepth4;   //!
   TBranch        *b_rc1jet_hcalDepth5;   //!
   TBranch        *b_rc2jet_hcalDepth5;   //!
   TBranch        *b_rc1jet_hcalDepth6;   //!
   TBranch        *b_rc2jet_hcalDepth6;   //!
   TBranch        *b_rc1jet_hcalDepth7;   //!
   TBranch        *b_rc2jet_hcalDepth7;   //!
   TBranch        *b_trigger_pass;   //!
   TBranch        *b_trigger_error;   //!
   TBranch        *b_trigger_run;   //!
   TBranch        *b_trigger_prescale;   //!
   TBranch        *b_trigger_name;   //!
   TBranch        *b_genht;   //!
   TBranch        *b_nGenJets;   //!
   TBranch        *b_genjet_eta;   //!
   TBranch        *b_genjet_phi;   //!
   TBranch        *b_genjet_pt;   //!
   TBranch        *b_genjet_area;   //!

   NoiseTree(TTree *tree=0);
   virtual ~NoiseTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef NoiseTree_cxx
NoiseTree::NoiseTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/pnfs/knu.ac.kr/data/cms/store/user/sha/JERNoiseStudy/Run3/2025/ZB/ZeroBias/PUPPINoiseStudy_Run2025F_v1_v2/260807_154935/0001/Offset_Data_Run2025F_1001.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/pnfs/knu.ac.kr/data/cms/store/user/sha/JERNoiseStudy/Run3/2025/ZB/ZeroBias/PUPPINoiseStudy_Run2025F_v1_v2/260807_154935/0001/Offset_Data_Run2025F_1001.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/pnfs/knu.ac.kr/data/cms/store/user/sha/JERNoiseStudy/Run3/2025/ZB/ZeroBias/PUPPINoiseStudy_Run2025F_v1_v2/260807_154935/0001/Offset_Data_Run2025F_1001.root:/pf");
      dir->GetObject("T",tree);

   }
   Init(tree);
}

NoiseTree::~NoiseTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NoiseTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NoiseTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void NoiseTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   pf_type = 0;
   pf_fromPV = 0;
   pf_fromPVs = 0;
   pf_pt = 0;
   pf_eta = 0;
   pf_phi = 0;
   pf_et = 0;
   pf_energy = 0;
   pf_m = 0;
   pf_puppiW = 0;
   pf_hcalDepth1 = 0;
   pf_hcalDepth2 = 0;
   pf_hcalDepth3 = 0;
   pf_hcalDepth4 = 0;
   pf_hcalDepth5 = 0;
   pf_hcalDepth6 = 0;
   pf_hcalDepth7 = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_pt = 0;
   jet_area = 0;
   jet_ch = 0;
   jet_nh = 0;
   jet_ne = 0;
   jet_hfh = 0;
   jet_hfe = 0;
   rc1jet_numpf = 0;
   rc1jet_eta = 0;
   rc1jet_phi = 0;
   rc1jet_pt = 0;
   rc1jet_energy = 0;
   rc1jet_cor = 0;
   rc1jet_coruncUp = 0;
   rc1jet_coruncDown = 0;
   rc1jet_corL1 = 0;
   rc1jet_corL1RC = 0;
   rc1jet_corL1Nega = 0;
   rc1jet_corL1RCNega = 0;
   rc1jet_corAll = 0;
   rc1jet_corAllRC = 0;
   rc1jet_corAllNega = 0;
   rc1jet_corAllRCNega = 0;
   rc2jet_numpf = 0;
   rc2jet_eta = 0;
   rc2jet_phi = 0;
   rc2jet_pt = 0;
   rc2jet_energy = 0;
   rc2jet_cor = 0;
   rc2jet_coruncUp = 0;
   rc2jet_coruncDown = 0;
   rc2jet_corL1 = 0;
   rc2jet_corL1RC = 0;
   rc2jet_corL1Nega = 0;
   rc2jet_corL1RCNega = 0;
   rc2jet_corAll = 0;
   rc2jet_corAllRC = 0;
   rc2jet_corAllNega = 0;
   rc2jet_corAllRCNega = 0;
   rc_diff = 0;
   rc_ispair = 0;
   rc_ispair_puppi = 0;
   rc1jet_puppi_numpf = 0;
   rc2jet_puppi_numpf = 0;
   rc1jet_puppi_eta = 0;
   rc2jet_puppi_eta = 0;
   rc1jet_puppi_phi = 0;
   rc2jet_puppi_phi = 0;
   rc1jet_puppi_pt = 0;
   rc2jet_puppi_pt = 0;
   rc1jet_puppi_energy = 0;
   rc2jet_puppi_energy = 0;
   rc1jet_puppi_cor = 0;
   rc2jet_puppi_cor = 0;
   rc1jet_puppi_coruncUp = 0;
   rc2jet_puppi_coruncUp = 0;
   rc1jet_puppi_coruncDown = 0;
   rc2jet_puppi_coruncDown = 0;
   rc1jet_puppi_corL1 = 0;
   rc2jet_puppi_corL1 = 0;
   rc1jet_puppi_corL1RC = 0;
   rc2jet_puppi_corL1RC = 0;
   rc1jet_puppi_corL1Nega = 0;
   rc2jet_puppi_corL1Nega = 0;
   rc1jet_puppi_corL1RCNega = 0;
   rc2jet_puppi_corL1RCNega = 0;
   rc1jet_puppi_corAll = 0;
   rc2jet_puppi_corAll = 0;
   rc1jet_puppi_corAllRC = 0;
   rc2jet_puppi_corAllRC = 0;
   rc1jet_puppi_corAllNega = 0;
   rc2jet_puppi_corAllNega = 0;
   rc1jet_puppi_corAllRCNega = 0;
   rc2jet_puppi_corAllRCNega = 0;
   rc1jet_puppiET = 0;
   rc2jet_puppiET = 0;
   rc_puppi_diff = 0;
   rc1jet_puppiWsum = 0;
   rc2jet_puppiWsum = 0;
   rc1jet_et_nh = 0;
   rc2jet_et_nh = 0;
   rc1jet_et_ne = 0;
   rc2jet_et_ne = 0;
   rc1jet_et_chm = 0;
   rc2jet_et_chm = 0;
   rc1jet_et_chu = 0;
   rc2jet_et_chu = 0;
   rc1jet_et_hfh = 0;
   rc2jet_et_hfh = 0;
   rc1jet_et_hfe = 0;
   rc2jet_et_hfe = 0;
   rc1jet_puppiET_nh = 0;
   rc2jet_puppiET_nh = 0;
   rc1jet_puppiET_ne = 0;
   rc2jet_puppiET_ne = 0;
   rc1jet_puppiET_chm = 0;
   rc2jet_puppiET_chm = 0;
   rc1jet_puppiET_hfh = 0;
   rc2jet_puppiET_hfh = 0;
   rc1jet_puppiET_hfe = 0;
   rc2jet_puppiET_hfe = 0;
   rc1jet_hcalDepth1 = 0;
   rc2jet_hcalDepth1 = 0;
   rc1jet_hcalDepth2 = 0;
   rc2jet_hcalDepth2 = 0;
   rc1jet_hcalDepth3 = 0;
   rc2jet_hcalDepth3 = 0;
   rc1jet_hcalDepth4 = 0;
   rc2jet_hcalDepth4 = 0;
   rc1jet_hcalDepth5 = 0;
   rc2jet_hcalDepth5 = 0;
   rc1jet_hcalDepth6 = 0;
   rc2jet_hcalDepth6 = 0;
   rc1jet_hcalDepth7 = 0;
   rc2jet_hcalDepth7 = 0;
   trigger_pass = 0;
   trigger_error = 0;
   trigger_run = 0;
   trigger_prescale = 0;
   trigger_name = 0;
   genjet_eta = 0;
   genjet_phi = 0;
   genjet_pt = 0;
   genjet_area = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   // run/lumi/bx/event: present in Data ntuples, absent in MC ntuples (MC has no
   // real run/lumi/event numbers). Guard on branch existence so one NoiseTree class
   // reads both without ROOT's "unknown branch" warnings.
   if (tree->GetBranch("run")) {
      fChain->SetBranchAddress("run", &run, &b_run);
      fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
      fChain->SetBranchAddress("bx", &bx, &b_bx);
      fChain->SetBranchAddress("event", &event, &b_event);
   } else {
      run = -1; lumi = -1; bx = -1; event = 0;
      b_run = b_lumi = b_bx = nullptr; b_event = nullptr;
   }
   fChain->SetBranchAddress("pf_type", &pf_type, &b_pf_type);
   fChain->SetBranchAddress("pf_fromPV", &pf_fromPV, &b_pf_fromPV);
   fChain->SetBranchAddress("pf_fromPVs", &pf_fromPVs, &b_pf_fromPVs);
   fChain->SetBranchAddress("pf_pt", &pf_pt, &b_pf_pt);
   fChain->SetBranchAddress("pf_eta", &pf_eta, &b_pf_eta);
   fChain->SetBranchAddress("pf_phi", &pf_phi, &b_pf_phi);
   fChain->SetBranchAddress("pf_et", &pf_et, &b_pf_et);
   fChain->SetBranchAddress("pf_energy", &pf_energy, &b_pf_energy);
   fChain->SetBranchAddress("pf_m", &pf_m, &b_pf_m);
   fChain->SetBranchAddress("pf_puppiW", &pf_puppiW, &b_pf_puppiW);
   fChain->SetBranchAddress("pf_hcalDepth1", &pf_hcalDepth1, &b_pf_hcalDepth1);
   fChain->SetBranchAddress("pf_hcalDepth2", &pf_hcalDepth2, &b_pf_hcalDepth2);
   fChain->SetBranchAddress("pf_hcalDepth3", &pf_hcalDepth3, &b_pf_hcalDepth3);
   fChain->SetBranchAddress("pf_hcalDepth4", &pf_hcalDepth4, &b_pf_hcalDepth4);
   fChain->SetBranchAddress("pf_hcalDepth5", &pf_hcalDepth5, &b_pf_hcalDepth5);
   fChain->SetBranchAddress("pf_hcalDepth6", &pf_hcalDepth6, &b_pf_hcalDepth6);
   fChain->SetBranchAddress("pf_hcalDepth7", &pf_hcalDepth7, &b_pf_hcalDepth7);
   fChain->SetBranchAddress("mu", &mu, &b_mu);
   fChain->SetBranchAddress("muWeight", &muWeight, &b_muWeight);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("rhoC0", &rhoC0, &b_rhoC0);
   fChain->SetBranchAddress("rhoCC", &rhoCC, &b_rhoCC);
   fChain->SetBranchAddress("rhoCentral", &rhoCentral, &b_rhoCentral);
   fChain->SetBranchAddress("rhoCentralCalo", &rhoCentralCalo, &b_rhoCentralCalo);
   fChain->SetBranchAddress("nPVall", &nPVall, &b_nPVall);
   fChain->SetBranchAddress("nPV", &nPV, &b_nPV);
   fChain->SetBranchAddress("pv_ndof", pv_ndof, &b_pv_ndof);
   fChain->SetBranchAddress("pv_z", pv_z, &b_pv_z);
   fChain->SetBranchAddress("pv_rho", pv_rho, &b_pv_rho);
   fChain->SetBranchAddress("nEta", &nEta, &b_nEta);
   fChain->SetBranchAddress("energy", energy, &b_energy);
   fChain->SetBranchAddress("et", et, &b_et);
   fChain->SetBranchAddress("eRMS", eRMS, &b_eRMS);
   fChain->SetBranchAddress("etMED", etMED, &b_etMED);
   fChain->SetBranchAddress("etMEAN", etMEAN, &b_etMEAN);
   fChain->SetBranchAddress("et_gme", et_gme, &b_et_gme);
   fChain->SetBranchAddress("et_twopi", et_twopi, &b_et_twopi);
   fChain->SetBranchAddress("rho_gme", &rho_gme, &b_rho_gme);
   // et_twopi_gen/det/scaled, rho_gme_gen/det/scaled, weight, nGenParticles,
   // idHardScat1/2, idxHardScat: present in MC ntuples, absent in Data ntuples (no
   // generator-level truth for real data). Guard on branch existence, same reasoning
   // as the run/lumi/bx/event guard above.
   if (tree->GetBranch("weight")) {
      fChain->SetBranchAddress("et_twopi_gen", et_twopi_gen, &b_et_twopi_gen);
      fChain->SetBranchAddress("rho_gme_gen", &rho_gme_gen, &b_rho_gme_gen);
      fChain->SetBranchAddress("et_twopi_det", et_twopi_det, &b_et_twopi_det);
      fChain->SetBranchAddress("rho_gme_det", &rho_gme_det, &b_rho_gme_det);
      fChain->SetBranchAddress("et_twopi_scaled", et_twopi_scaled, &b_et_twopi_scaled);
      fChain->SetBranchAddress("rho_gme_scaled", &rho_gme_scaled, &b_rho_gme_scaled);
      fChain->SetBranchAddress("weight", &weight, &b_weight);
      fChain->SetBranchAddress("nGenParticles", &nGenParticles, &b_nGenParticles);
      fChain->SetBranchAddress("idHardScat1", &idHardScat1, &b_idHardScat1);
      fChain->SetBranchAddress("idHardScat2", &idHardScat2, &b_idHardScat2);
      fChain->SetBranchAddress("idxHardScat", &idxHardScat, &b_idxHardScat);
   } else {
      rho_gme_gen = 0; rho_gme_det = 0; rho_gme_scaled = 0;
      weight = 1.0; nGenParticles = -1; idHardScat1 = -999; idHardScat2 = -999; idxHardScat = -1;
      b_et_twopi_gen = b_et_twopi_det = b_et_twopi_scaled = nullptr;
      b_rho_gme_gen = b_rho_gme_det = b_rho_gme_scaled = nullptr;
      b_weight = b_nGenParticles = b_idHardScat1 = b_idHardScat2 = b_idxHardScat = nullptr;
   }
   fChain->SetBranchAddress("fchm", fchm, &b_fchm);
   fChain->SetBranchAddress("fchu", fchu, &b_fchu);
   fChain->SetBranchAddress("fnh", fnh, &b_fnh);
   fChain->SetBranchAddress("fne", fne, &b_fne);
   fChain->SetBranchAddress("fhfh", fhfh, &b_fhfh);
   fChain->SetBranchAddress("fhfe", fhfe, &b_fhfe);
   fChain->SetBranchAddress("flep", flep, &b_flep);
   fChain->SetBranchAddress("funtrk", funtrk, &b_funtrk);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_area", &jet_area, &b_jet_area);
   fChain->SetBranchAddress("jet_ch", &jet_ch, &b_jet_ch);
   fChain->SetBranchAddress("jet_nh", &jet_nh, &b_jet_nh);
   fChain->SetBranchAddress("jet_ne", &jet_ne, &b_jet_ne);
   fChain->SetBranchAddress("jet_hfh", &jet_hfh, &b_jet_hfh);
   fChain->SetBranchAddress("jet_hfe", &jet_hfe, &b_jet_hfe);
   fChain->SetBranchAddress("rc1jet_numpf", &rc1jet_numpf, &b_rc1jet_numpf);
   fChain->SetBranchAddress("rc1jet_eta", &rc1jet_eta, &b_rc1jet_eta);
   fChain->SetBranchAddress("rc1jet_phi", &rc1jet_phi, &b_rc1jet_phi);
   fChain->SetBranchAddress("rc1jet_pt", &rc1jet_pt, &b_rc1jet_pt);
   fChain->SetBranchAddress("rc1jet_energy", &rc1jet_energy, &b_rc1jet_energy);
   fChain->SetBranchAddress("rc1jet_cor", &rc1jet_cor, &b_rc1jet_cor);
   fChain->SetBranchAddress("rc1jet_coruncUp", &rc1jet_coruncUp, &b_rc1jet_coruncUp);
   fChain->SetBranchAddress("rc1jet_coruncDown", &rc1jet_coruncDown, &b_rc1jet_coruncDown);
   fChain->SetBranchAddress("rc1jet_corL1", &rc1jet_corL1, &b_rc1jet_corL1);
   fChain->SetBranchAddress("rc1jet_corL1RC", &rc1jet_corL1RC, &b_rc1jet_corL1RC);
   fChain->SetBranchAddress("rc1jet_corL1Nega", &rc1jet_corL1Nega, &b_rc1jet_corL1Nega);
   fChain->SetBranchAddress("rc1jet_corL1RCNega", &rc1jet_corL1RCNega, &b_rc1jet_corL1RCNega);
   fChain->SetBranchAddress("rc1jet_corAll", &rc1jet_corAll, &b_rc1jet_corAll);
   fChain->SetBranchAddress("rc1jet_corAllRC", &rc1jet_corAllRC, &b_rc1jet_corAllRC);
   fChain->SetBranchAddress("rc1jet_corAllNega", &rc1jet_corAllNega, &b_rc1jet_corAllNega);
   fChain->SetBranchAddress("rc1jet_corAllRCNega", &rc1jet_corAllRCNega, &b_rc1jet_corAllRCNega);
   fChain->SetBranchAddress("rc2jet_numpf", &rc2jet_numpf, &b_rc2jet_numpf);
   fChain->SetBranchAddress("rc2jet_eta", &rc2jet_eta, &b_rc2jet_eta);
   fChain->SetBranchAddress("rc2jet_phi", &rc2jet_phi, &b_rc2jet_phi);
   fChain->SetBranchAddress("rc2jet_pt", &rc2jet_pt, &b_rc2jet_pt);
   fChain->SetBranchAddress("rc2jet_energy", &rc2jet_energy, &b_rc2jet_energy);
   fChain->SetBranchAddress("rc2jet_cor", &rc2jet_cor, &b_rc2jet_cor);
   fChain->SetBranchAddress("rc2jet_coruncUp", &rc2jet_coruncUp, &b_rc2jet_coruncUp);
   fChain->SetBranchAddress("rc2jet_coruncDown", &rc2jet_coruncDown, &b_rc2jet_coruncDown);
   fChain->SetBranchAddress("rc2jet_corL1", &rc2jet_corL1, &b_rc2jet_corL1);
   fChain->SetBranchAddress("rc2jet_corL1RC", &rc2jet_corL1RC, &b_rc2jet_corL1RC);
   fChain->SetBranchAddress("rc2jet_corL1Nega", &rc2jet_corL1Nega, &b_rc2jet_corL1Nega);
   fChain->SetBranchAddress("rc2jet_corL1RCNega", &rc2jet_corL1RCNega, &b_rc2jet_corL1RCNega);
   fChain->SetBranchAddress("rc2jet_corAll", &rc2jet_corAll, &b_rc2jet_corAll);
   fChain->SetBranchAddress("rc2jet_corAllRC", &rc2jet_corAllRC, &b_rc2jet_corAllRC);
   fChain->SetBranchAddress("rc2jet_corAllNega", &rc2jet_corAllNega, &b_rc2jet_corAllNega);
   fChain->SetBranchAddress("rc2jet_corAllRCNega", &rc2jet_corAllRCNega, &b_rc2jet_corAllRCNega);
   fChain->SetBranchAddress("rc_diff", &rc_diff, &b_rc_diff);
   fChain->SetBranchAddress("rc_ispair", &rc_ispair, &b_rc_ispair);
   fChain->SetBranchAddress("rc_ispair_puppi", &rc_ispair_puppi, &b_rc_ispair_puppi);
   fChain->SetBranchAddress("rc1jet_puppi_numpf", &rc1jet_puppi_numpf, &b_rc1jet_puppi_numpf);
   fChain->SetBranchAddress("rc2jet_puppi_numpf", &rc2jet_puppi_numpf, &b_rc2jet_puppi_numpf);
   fChain->SetBranchAddress("rc1jet_puppi_eta", &rc1jet_puppi_eta, &b_rc1jet_puppi_eta);
   fChain->SetBranchAddress("rc2jet_puppi_eta", &rc2jet_puppi_eta, &b_rc2jet_puppi_eta);
   fChain->SetBranchAddress("rc1jet_puppi_phi", &rc1jet_puppi_phi, &b_rc1jet_puppi_phi);
   fChain->SetBranchAddress("rc2jet_puppi_phi", &rc2jet_puppi_phi, &b_rc2jet_puppi_phi);
   fChain->SetBranchAddress("rc1jet_puppi_pt", &rc1jet_puppi_pt, &b_rc1jet_puppi_pt);
   fChain->SetBranchAddress("rc2jet_puppi_pt", &rc2jet_puppi_pt, &b_rc2jet_puppi_pt);
   fChain->SetBranchAddress("rc1jet_puppi_energy", &rc1jet_puppi_energy, &b_rc1jet_puppi_energy);
   fChain->SetBranchAddress("rc2jet_puppi_energy", &rc2jet_puppi_energy, &b_rc2jet_puppi_energy);
   fChain->SetBranchAddress("rc1jet_puppi_cor", &rc1jet_puppi_cor, &b_rc1jet_puppi_cor);
   fChain->SetBranchAddress("rc2jet_puppi_cor", &rc2jet_puppi_cor, &b_rc2jet_puppi_cor);
   fChain->SetBranchAddress("rc1jet_puppi_coruncUp", &rc1jet_puppi_coruncUp, &b_rc1jet_puppi_coruncUp);
   fChain->SetBranchAddress("rc2jet_puppi_coruncUp", &rc2jet_puppi_coruncUp, &b_rc2jet_puppi_coruncUp);
   fChain->SetBranchAddress("rc1jet_puppi_coruncDown", &rc1jet_puppi_coruncDown, &b_rc1jet_puppi_coruncDown);
   fChain->SetBranchAddress("rc2jet_puppi_coruncDown", &rc2jet_puppi_coruncDown, &b_rc2jet_puppi_coruncDown);
   fChain->SetBranchAddress("rc1jet_puppi_corL1", &rc1jet_puppi_corL1, &b_rc1jet_puppi_corL1);
   fChain->SetBranchAddress("rc2jet_puppi_corL1", &rc2jet_puppi_corL1, &b_rc2jet_puppi_corL1);
   fChain->SetBranchAddress("rc1jet_puppi_corL1RC", &rc1jet_puppi_corL1RC, &b_rc1jet_puppi_corL1RC);
   fChain->SetBranchAddress("rc2jet_puppi_corL1RC", &rc2jet_puppi_corL1RC, &b_rc2jet_puppi_corL1RC);
   fChain->SetBranchAddress("rc1jet_puppi_corL1Nega", &rc1jet_puppi_corL1Nega, &b_rc1jet_puppi_corL1Nega);
   fChain->SetBranchAddress("rc2jet_puppi_corL1Nega", &rc2jet_puppi_corL1Nega, &b_rc2jet_puppi_corL1Nega);
   fChain->SetBranchAddress("rc1jet_puppi_corL1RCNega", &rc1jet_puppi_corL1RCNega, &b_rc1jet_puppi_corL1RCNega);
   fChain->SetBranchAddress("rc2jet_puppi_corL1RCNega", &rc2jet_puppi_corL1RCNega, &b_rc2jet_puppi_corL1RCNega);
   fChain->SetBranchAddress("rc1jet_puppi_corAll", &rc1jet_puppi_corAll, &b_rc1jet_puppi_corAll);
   fChain->SetBranchAddress("rc2jet_puppi_corAll", &rc2jet_puppi_corAll, &b_rc2jet_puppi_corAll);
   fChain->SetBranchAddress("rc1jet_puppi_corAllRC", &rc1jet_puppi_corAllRC, &b_rc1jet_puppi_corAllRC);
   fChain->SetBranchAddress("rc2jet_puppi_corAllRC", &rc2jet_puppi_corAllRC, &b_rc2jet_puppi_corAllRC);
   fChain->SetBranchAddress("rc1jet_puppi_corAllNega", &rc1jet_puppi_corAllNega, &b_rc1jet_puppi_corAllNega);
   fChain->SetBranchAddress("rc2jet_puppi_corAllNega", &rc2jet_puppi_corAllNega, &b_rc2jet_puppi_corAllNega);
   fChain->SetBranchAddress("rc1jet_puppi_corAllRCNega", &rc1jet_puppi_corAllRCNega, &b_rc1jet_puppi_corAllRCNega);
   fChain->SetBranchAddress("rc2jet_puppi_corAllRCNega", &rc2jet_puppi_corAllRCNega, &b_rc2jet_puppi_corAllRCNega);
   fChain->SetBranchAddress("rc1jet_puppiET", &rc1jet_puppiET, &b_rc1jet_puppiET);
   fChain->SetBranchAddress("rc2jet_puppiET", &rc2jet_puppiET, &b_rc2jet_puppiET);
   fChain->SetBranchAddress("rc_puppi_diff", &rc_puppi_diff, &b_rc_puppi_diff);
   fChain->SetBranchAddress("rc1jet_puppiWsum", &rc1jet_puppiWsum, &b_rc1jet_puppiWsum);
   fChain->SetBranchAddress("rc2jet_puppiWsum", &rc2jet_puppiWsum, &b_rc2jet_puppiWsum);
   fChain->SetBranchAddress("rc1jet_et_nh", &rc1jet_et_nh, &b_rc1jet_et_nh);
   fChain->SetBranchAddress("rc2jet_et_nh", &rc2jet_et_nh, &b_rc2jet_et_nh);
   fChain->SetBranchAddress("rc1jet_et_ne", &rc1jet_et_ne, &b_rc1jet_et_ne);
   fChain->SetBranchAddress("rc2jet_et_ne", &rc2jet_et_ne, &b_rc2jet_et_ne);
   fChain->SetBranchAddress("rc1jet_et_chm", &rc1jet_et_chm, &b_rc1jet_et_chm);
   fChain->SetBranchAddress("rc2jet_et_chm", &rc2jet_et_chm, &b_rc2jet_et_chm);
   fChain->SetBranchAddress("rc1jet_et_chu", &rc1jet_et_chu, &b_rc1jet_et_chu);
   fChain->SetBranchAddress("rc2jet_et_chu", &rc2jet_et_chu, &b_rc2jet_et_chu);
   fChain->SetBranchAddress("rc1jet_et_hfh", &rc1jet_et_hfh, &b_rc1jet_et_hfh);
   fChain->SetBranchAddress("rc2jet_et_hfh", &rc2jet_et_hfh, &b_rc2jet_et_hfh);
   fChain->SetBranchAddress("rc1jet_et_hfe", &rc1jet_et_hfe, &b_rc1jet_et_hfe);
   fChain->SetBranchAddress("rc2jet_et_hfe", &rc2jet_et_hfe, &b_rc2jet_et_hfe);
   fChain->SetBranchAddress("rc1jet_puppiET_nh", &rc1jet_puppiET_nh, &b_rc1jet_puppiET_nh);
   fChain->SetBranchAddress("rc2jet_puppiET_nh", &rc2jet_puppiET_nh, &b_rc2jet_puppiET_nh);
   fChain->SetBranchAddress("rc1jet_puppiET_ne", &rc1jet_puppiET_ne, &b_rc1jet_puppiET_ne);
   fChain->SetBranchAddress("rc2jet_puppiET_ne", &rc2jet_puppiET_ne, &b_rc2jet_puppiET_ne);
   fChain->SetBranchAddress("rc1jet_puppiET_chm", &rc1jet_puppiET_chm, &b_rc1jet_puppiET_chm);
   fChain->SetBranchAddress("rc2jet_puppiET_chm", &rc2jet_puppiET_chm, &b_rc2jet_puppiET_chm);
   fChain->SetBranchAddress("rc1jet_puppiET_hfh", &rc1jet_puppiET_hfh, &b_rc1jet_puppiET_hfh);
   fChain->SetBranchAddress("rc2jet_puppiET_hfh", &rc2jet_puppiET_hfh, &b_rc2jet_puppiET_hfh);
   fChain->SetBranchAddress("rc1jet_puppiET_hfe", &rc1jet_puppiET_hfe, &b_rc1jet_puppiET_hfe);
   fChain->SetBranchAddress("rc2jet_puppiET_hfe", &rc2jet_puppiET_hfe, &b_rc2jet_puppiET_hfe);
   fChain->SetBranchAddress("rc1jet_hcalDepth1", &rc1jet_hcalDepth1, &b_rc1jet_hcalDepth1);
   fChain->SetBranchAddress("rc2jet_hcalDepth1", &rc2jet_hcalDepth1, &b_rc2jet_hcalDepth1);
   fChain->SetBranchAddress("rc1jet_hcalDepth2", &rc1jet_hcalDepth2, &b_rc1jet_hcalDepth2);
   fChain->SetBranchAddress("rc2jet_hcalDepth2", &rc2jet_hcalDepth2, &b_rc2jet_hcalDepth2);
   fChain->SetBranchAddress("rc1jet_hcalDepth3", &rc1jet_hcalDepth3, &b_rc1jet_hcalDepth3);
   fChain->SetBranchAddress("rc2jet_hcalDepth3", &rc2jet_hcalDepth3, &b_rc2jet_hcalDepth3);
   fChain->SetBranchAddress("rc1jet_hcalDepth4", &rc1jet_hcalDepth4, &b_rc1jet_hcalDepth4);
   fChain->SetBranchAddress("rc2jet_hcalDepth4", &rc2jet_hcalDepth4, &b_rc2jet_hcalDepth4);
   fChain->SetBranchAddress("rc1jet_hcalDepth5", &rc1jet_hcalDepth5, &b_rc1jet_hcalDepth5);
   fChain->SetBranchAddress("rc2jet_hcalDepth5", &rc2jet_hcalDepth5, &b_rc2jet_hcalDepth5);
   fChain->SetBranchAddress("rc1jet_hcalDepth6", &rc1jet_hcalDepth6, &b_rc1jet_hcalDepth6);
   fChain->SetBranchAddress("rc2jet_hcalDepth6", &rc2jet_hcalDepth6, &b_rc2jet_hcalDepth6);
   fChain->SetBranchAddress("rc1jet_hcalDepth7", &rc1jet_hcalDepth7, &b_rc1jet_hcalDepth7);
   fChain->SetBranchAddress("rc2jet_hcalDepth7", &rc2jet_hcalDepth7, &b_rc2jet_hcalDepth7);
   fChain->SetBranchAddress("trigger_pass", &trigger_pass, &b_trigger_pass);
   fChain->SetBranchAddress("trigger_error", &trigger_error, &b_trigger_error);
   fChain->SetBranchAddress("trigger_run", &trigger_run, &b_trigger_run);
   fChain->SetBranchAddress("trigger_prescale", &trigger_prescale, &b_trigger_prescale);
   fChain->SetBranchAddress("trigger_name", &trigger_name, &b_trigger_name);
   fChain->SetBranchAddress("genht", &genht, &b_genht);
   fChain->SetBranchAddress("nGenJets", &nGenJets, &b_nGenJets);
   fChain->SetBranchAddress("genjet_eta", &genjet_eta, &b_genjet_eta);
   fChain->SetBranchAddress("genjet_phi", &genjet_phi, &b_genjet_phi);
   fChain->SetBranchAddress("genjet_pt", &genjet_pt, &b_genjet_pt);
   fChain->SetBranchAddress("genjet_area", &genjet_area, &b_genjet_area);
   Notify();
}

bool NoiseTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void NoiseTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NoiseTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef NoiseTree_cxx
