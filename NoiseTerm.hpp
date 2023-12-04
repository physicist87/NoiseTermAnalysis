#ifndef _NoiseTerm_

#define _NoiseTerm_
  
#include <set>
#include <string>
#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <TF1.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include "TLorentzVector.h"
#include "TRandom.h"
#include "TRandom1.h"
#include "TRandom3.h"
  
#include "analysis/NoiseTree.h"
#include "./TextReader/TextReader.hpp"
#include "LumiReWeighting.h"
#include "./jetveto/JetVeto.hpp"

                                                                                                                                        
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "CondFormats/JetMETObjects/interface/JetResolutionObject.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"

using namespace std;

Double_t fJEC(Double_t *x, Double_t *p);

struct RandCone {   
    double diff_;
    int num_pfcan_rc1;
    int num_pfcan_rc2;
    bool findRC;
    TLorentzVector rc1;
    TLorentzVector rc2; 
    TLorentzVector vecrc1;
    TLorentzVector vecrc2; 
};


class NoiseTerm : public NoiseTree 
{
   public:
      //declare functions
      NoiseTerm(TTree *tree=0, int rndseed=0, string ConfName="");
      //NoiseTerm(TTree *tree=0,  string ConfName="");
      virtual ~NoiseTerm();

      //basic frame
      virtual void Loop();
      void Start();
      void End();

      //user define functions
      void SetOutputFileName(char* studyname, char *outname);
      void DeclareDefHistos();
      void DeclareHistos(int index);
      //void SetRandomCone();      
      int FindIndexNPU(float npu_);
      //double SetRandomCone(int idx);      
      //double SetRandomCone();      
      RandCone SetRandomCone(double upperEta, double lowerEta);
      float getdR(float eta1, float eta2, float phi1, float phi2);
      bool Trigger();      
      double GetPreScaleFactor();
      bool IsPFCHS(int itype); 
//      Double_t fJEC(Double_t *x, Double_t *p);
 
   private:
      //put variables that you want
      char *studydir;
      char *outfile;
      TFile *fout;
      vector<TFile *>v_fout;
      //TFile *v_fout[15];
      TextReader *SSBConfReader;
      JetVeto* jveto;
      bool isMC;
      bool doPUCor;
      bool doPreSC;
      bool doJEC;
      TString JECType;
      TString JECEra_;
      TString JetType_;
      int num_trig;

      double evtWeight;

      // Declare user variables
      std::vector<std::string> trigName;
      std::vector<int> v_idx; //Channel_Idx
      std::vector<int> v_pf_RC1_idx; //Channel_Idx
      std::vector<int> v_pf_RC2_idx; //Channel_Idx
      TRandom* rand; // To obtain random variable in phi direction// 
      //TRandom3* r; // To obtain random variable in phi direction// 
      TRandom3 r;
      TRandom *r1;//=new TRandom1();
      edm::LumiReWeighting *puweight;
      double puWieght;
      double upperEta;
      double lowerEta;
      double RCone;
      string PUMCFile;//=new TRandom1();
      string PUDATAFile;//=new TRandom1();
      enum Flavor{ chm = 0, chu, nh, ne, hfh, hfe, lep, untrk, numFlavors};
      vector<TString> v_EtaBins;

   public:
      //declare histograms

      TH1D *h_puWeight;
      TH1D *h_Mu[9];
      TH1D *h_NPV[9];

      TH1D *h_PF_pt[9];
      TH1D *h_PF_eta[9];
      TH1D *h_PF_phi[9];
      TH1D *h_PF_energy[9];


      TH1D *h_mu[20][9];
      TH1D *h_pvx[20][9];

      //TH2D *h_pvx_vs_mu[20][9];
      //TH2D *h_mu_vs_pvx[20][9];
      TH1D *h_rho[20][9];
      TH1D *h_Num_pf_RC1[20][9];
      TH1D *h_Num_pf_RC2[20][9];
      TH1D *hConeEta[20][9];
      TH1D *h_Noise_Diff[20][9];
      TH1D *h_Noise_Rho_Diff[20][9];
      TH1D *h_Noise_Avg[20][9];

      TH1D *h_Debug_Phi1[20][9];
      TH1D *h_Debug_Phi2[20][9];
      TH1D *h_Debug_Eta1[20][9];
      TH1D *h_Debug_Eta2[20][9];

      TH2D *h_Debug_Eta1vsPhi1[20][9];
      TH2D *h_Debug_Eta2vsPhi2[20][9];
      //TH2D *h_Debug_Phi1vsPhi2[20][9];
      //TH2D *h_Debug_Eta1vsEta2[20][9];

      //TH2D *h_RC1_ptvsRC2_pt[20][9];
      //TH2D *h_RC1_ptvsDiff[20][9];
      //TH2D *h_RC2_ptvsDiff[20][9];

      TH1D *h_RC_Del_phi[20][9];
      TH1D *h_RC_Del_eta[20][9];
      TH2D *h_RC_Phi1vsPhi2[20][9];
      TH2D *h_RC_Eta1vsEta2[20][9];
      TH2D *h_Noise_DiffvsRCpt1[20][9];
      TH2D *h_Noise_DiffvsRCpt2[20][9];

      TH1D *h_RC1_pt[20][9];
      TH1D *h_RC2_pt[20][9];
      TH1D *h_RC1_eta[20][9];
      TH1D *h_RC2_eta[20][9];
      TH1D *h_RC1_phi[20][9];
      TH1D *h_RC2_phi[20][9];
      TH1D *h_RC1_energy[20][9];
      TH1D *h_RC2_energy[20][9];
      TH1D *h_RC1_Cor[20][9];
      TH1D *h_RC2_Cor[20][9];

      TH2D *h_JetVeto;

};

#endif

#ifdef NoiseTerm_cxx

//NoiseTerm::NoiseTerm(TTree *tree, string ConfName)
NoiseTerm::NoiseTerm(TTree *tree, int rndseed, string ConfName)
{
   if (tree == 0)
   {
      printf("ERROR: Can't find any input tree.\n");
   }
   Init(tree);
   rand = new TRandom();
   rand->SetSeed(8728*rndseed);
   r1 =new TRandom1();
   r1->SetSeed(8729*rndseed); 
   /// Read Text Configure file ///
   
   SSBConfReader = new TextReader();
   SSBConfReader->ReadFile(ConfName);
   SSBConfReader->ReadVariables();
   SSBConfReader->PrintoutVariables(); 

   PUMCFile = SSBConfReader->GetText( "PileUpMCFileName"     );
   PUDATAFile = SSBConfReader->GetText( "PileUpDATAFileName" );


   PUMCFile = "./pileuInfo/" + PUMCFile;
   PUDATAFile = "./pileuInfo/" + PUDATAFile;
   cout << "PUMCFile : " << PUMCFile << endl;
   cout << "PUDATAFile : " << PUDATAFile << endl;
   /// PileUp ///
   puweight   = new edm::LumiReWeighting(PUMCFile,
                                         PUDATAFile,
                                            "pileup",
                                            "pileup");
                                            //"jt0");

/*   puweight = new edm::LumiReWeighting("./pileuInfo/PileupMC_v2.root",// + PileUpMCFile,
                                            "./pileuInfo/pileup_DT_UL18.root",// + PileUpDATAFile, // mu Data
                                            //"./pileuInfo/DATAPileUp.root",// + PileUpDATAFile, // central provided 
                                            "pileup",
                                            "jt0" );*/
   cout << "ConfName !!" << ConfName << endl;
   num_trig = SSBConfReader->Size( "trigger" );
   for(int i =0; i < num_trig; ++i)                                                                                                     
   {                                                                                                                                    
      cout << SSBConfReader->GetText("trigger",i+1) << endl;                                                                            
      trigName.push_back( SSBConfReader->GetText("trigger",i+1) );                                                                      
   }    
   num_trig = SSBConfReader->Size( "trigger" );
   doPUCor = SSBConfReader->GetBool("DoPUCor"); // This is to do jer 

/*   lowerEta = SSBConfReader->GetNumber("EtabinLower");
   upperEta = SSBConfReader->GetNumber("EtabinUpper");
   cout << "lowerEta : " << lowerEta << endl;
   cout << "uppperEta : " << upperEta << endl;*/
   if (SSBConfReader->GetBool("DoJetVeto"))jveto = new JetVeto(SSBConfReader->GetText("JetVetoFName"),SSBConfReader->GetText("JetVetoHName"));
   doPreSC =  SSBConfReader->GetBool("DoPrescale"); 
   cout << "SSBConfReader->SizeEtaBins "  << SSBConfReader->Size("EtaBins") << endl;
   for (int i = 0; i < SSBConfReader->Size("EtaBins"); ++i)
   {
      if (i != SSBConfReader->Size("EtaBins")-1) 
      {
      TString lower = SSBConfReader->GetText("EtaBins",i+1);
      TString upper =  SSBConfReader->GetText("EtaBins",i+2);
         v_EtaBins.push_back(Form("EtaBin_%s_%s",lower.Data(),upper.Data()) );//EtaBin_0_0p5
      }
   }
  for (int i = 0; i < v_EtaBins.size(); ++i)
       cout << "v_EtaBins : " << v_EtaBins.at(i) << endl;

  doJEC = SSBConfReader->GetBool("JECApply");
  JECType = SSBConfReader->GetText("JECType");
  JECEra_ = SSBConfReader->GetText("JECEra");
  JetType_ = SSBConfReader->GetText("JetType");
  RCone = SSBConfReader->GetNumber("JetCone");
}

NoiseTerm::~NoiseTerm()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   delete puweight;
   delete SSBConfReader;
   delete jveto;
}

#endif
   

