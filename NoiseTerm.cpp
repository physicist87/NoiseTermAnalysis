#define NoiseTerm_cxx

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "TMath.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"

#include "NoiseTerm.hpp"
#include "CommonTools.hpp"

using namespace std;

void NoiseTerm::Loop()
{
   //////////
   if (fChain == 0) return;
   //////////

   //////////
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   //////////

   ///My variables
   Long64_t __tot_evt = 0;

   int passedEvt = 0;
   ////////////////////////
   /// start event loop ///
   ////////////////////////
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
      {
         printf("ERROR: Could not load tree!!!\n");
         break;
      }

      nb = fChain->GetEntry(jentry);   nbytes += nb;
       
      if (jentry % 10000 == 0) printf("Event %lld\n", jentry); //%lld supports Long64_t

      __tot_evt++;

      //////////////////////////////////////
      /// start any calculation and cuts ///
      //////////////////////////////////////
      ///////////////////////////////////
      /// Pile Up Correction Factor ! ///
      ///////////////////////////////////
      //puWieght =-999;
      puWieght =1.0;
      //if (nPV <=0) {continue;}
      if (Trigger()==false) {continue;}
      double evt_trigpre =1.;
      evt_trigpre = GetPreScaleFactor();
      
      //puWieght = puweight->weight( mu );
      //if (doPUCor){if (isMC)puWieght = puweight->weight( mu );}
      if (doPUCor){puWieght = puweight->weight( mu );}
      ///FillHisto(h_puWeight,puWieght);
      evtWeight = 1;
      evtWeight = puWieght*evt_trigpre;
      //cout << "puWieght : " << puWieght<< endl;
      //cout << "muWieht : " << muWeight << endl;
      //if (mu < 30 || mu > 40) {continue;}
      //cout << "nPV : " << nPV << endl;

      //cout << "evtWeight : " << evtWeight << endl;
      //cout << "puWieght : " << puWieght<< endl;
      //cout << "NmuWieht : " << NmuWeight << endl;
      int nidx = FindIndexNPU(mu);
      int ridx = FindIndexNPU(rho);
      int NPV_ = 0;
      NPV_ =nPV;
      FillHisto(h_Mu[nidx], mu, evtWeight);
      FillHisto(h_NPV[nidx],NPV_, evtWeight);
      FillHisto(h_Mu[8],mu, evtWeight);
      FillHisto(h_NPV[8],NPV_, evtWeight);
      vector<JetCorrectorParameters> jetParsL23; // 
    
      FactorizedJetCorrector* jetCorrectorsL23;
      string jer_sf_file = "CondFormats/JetMETObjects/data/";
      string era_ = Form("%s", Form(JECEra_.Data()));
      //cout << "era_ : " << era_ << endl;
      string jet_type_ = Form("%s", JetType_.Data());

/*      JetCorrectorParameters* L1JetPars  = new JetCorrectorParameters(jer_sf_file + era_ + "/" + era_ + "_L1FastJet_"    +  jet_type_ + ".txt");

      JetCorrectorParameters* L2JetPars  = new JetCorrectorParameters(jer_sf_file + era_ + "/" + era_ + "_L2Relative_"   +  jet_type_ + ".txt");
      JetCorrectorParameters* L3JetPars  = new JetCorrectorParameters(jer_sf_file + era_ + "/" + era_ + "_L3Absolute_"   +  jet_type_ + ".txt");
      JetCorrectorParameters* ResJetPars = new JetCorrectorParameters(jer_sf_file + era_ + "/" + era_ + "_L2L3Residual_" +  jet_type_ + ".txt");

      JetCorrectionUncertainty* JecUncert = new JetCorrectionUncertainty(jer_sf_file + era_ + "/" + era_ + "_Uncertainty_" +  jet_type_ + ".txt");
*/



      JetCorrectorParameters* L1JetPars;
      JetCorrectorParameters* L2JetPars;
      JetCorrectorParameters* L3JetPars;
      JetCorrectorParameters* ResJetPars;
      JetCorrectionUncertainty* JecUncert;




      if (isUseUserJEC){
          //cout << "check out : L1JetParsTxtName : " << Form("%s/%s",jer_sf_file.c_str(), L1JetParsTxtName.Data()) << endl; 
          L1JetPars  = new JetCorrectorParameters(Form("%s/%s",jer_sf_file.c_str(), L1JetParsTxtName.Data()));
          L2JetPars  = new JetCorrectorParameters(Form("%s/%s",jer_sf_file.c_str(), L2JetParsTxtName.Data()));
          L3JetPars  = new JetCorrectorParameters(Form("%s/%s",jer_sf_file.c_str(), L3JetParsTxtName.Data()));
          ResJetPars  = new JetCorrectorParameters(Form("%s/%s",jer_sf_file.c_str(), ResJetParsTxtName.Data()));
          JecUncert  = new JetCorrectionUncertainty(Form("%s/%s",jer_sf_file.c_str(), JecUncertTxtName.Data()));
      }


      else {
         L1JetPars  = new JetCorrectorParameters(jer_sf_file + era_ + "/" + era_ + "_L1FastJet_"    +  jet_type_ + ".txt");
         L2JetPars  = new JetCorrectorParameters(jer_sf_file + era_ + "/" + era_ + "_L2Relative_"   +  jet_type_ + ".txt");
         L3JetPars  = new JetCorrectorParameters(jer_sf_file + era_ + "/" + era_ + "_L3Absolute_"   +  jet_type_ + ".txt");
         ResJetPars = new JetCorrectorParameters(jer_sf_file + era_ + "/" + era_ + "_L2L3Residual_" +  jet_type_ + ".txt");
         JecUncert = new JetCorrectionUncertainty(jer_sf_file + era_ + "/" + era_ + "_Uncertainty_" +  jet_type_ + ".txt");
      }

      bool doL1L2L3Res_ = true;
      if (doL1L2L3Res_){
        //jetPars.push_back( *L1JetPars );
        jetParsL23.push_back( *L2JetPars );
        jetParsL23.push_back( *L3JetPars );
        if (!isMC)jetParsL23.push_back( *ResJetPars );
      }
      jetCorrectorsL23 = new FactorizedJetCorrector( jetParsL23 );
     
      vector<int> v_rc1jet_numpf, v_rc2jet_numpf;
      vector<float> v_rc1jet_eta, v_rc1jet_phi, v_rc1jet_pt, v_rc1jet_energy;
      vector<float> v_rc2jet_eta, v_rc2jet_phi, v_rc2jet_pt, v_rc2jet_energy;
      vector<float> v_rc_diff;
      std::vector<bool> v_isRC;
 
      v_rc1jet_numpf.clear();
      v_rc1jet_eta.clear();
      v_rc1jet_phi.clear();
      v_rc1jet_pt.clear();
      v_rc1jet_energy.clear();
      v_rc2jet_numpf.clear();
      v_rc2jet_eta.clear();
      v_rc2jet_phi.clear();
      v_rc2jet_pt.clear();
      v_rc2jet_energy.clear();
      v_isRC.clear();
      v_rc_diff.clear();
      vector<double> vEtaBins = {0,0.261, 0.522, 0.783, 1.044, 1.305, 1.566, 1.740, 1.930, 2.043, 2.172, 2.322, 2.500, 2.650, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
      //cout << "vEtaBins " << vEtaBins.size() << endl;
      for (vector<double>::iterator ibins = vEtaBins.begin() ; ibins !=vEtaBins.end(); ibins++) {
         //cout << "ibins: " << *ibins << endl;
         unsigned int etabinIndex = distance(vEtaBins.begin(), ibins);
         if (etabinIndex!=vEtaBins.size()-1){
            RandCone tmp; 
            //cout << etabinIndex << " " << vEtaBins.size() << endl; 
            //cout << vEtaBins[etabinIndex+1] << " " << vEtaBins[etabinIndex] << endl; 
            tmp = SetRandomCone(vEtaBins[etabinIndex+1],vEtaBins[etabinIndex]);
            /// RC information ///
            v_isRC.push_back(tmp.findRC);
            v_rc_diff.push_back(tmp.diff_);
            v_rc1jet_numpf.push_back(tmp.num_pfcan_rc1);
            v_rc1jet_eta.push_back(tmp.rc1.Eta());
            v_rc1jet_phi.push_back(tmp.rc1.Phi());
            v_rc1jet_pt.push_back(tmp.rc1.Pt());
            v_rc1jet_energy.push_back(tmp.rc1.Energy());
            v_rc2jet_numpf.push_back(tmp.num_pfcan_rc2);
            v_rc2jet_eta.push_back(tmp.rc2.Eta());
            v_rc2jet_phi.push_back(tmp.rc2.Phi());
            v_rc2jet_pt.push_back(tmp.rc2.Pt());
            v_rc2jet_energy.push_back(tmp.rc2.Energy());
             
         }
      }

 
      //double jetArea_ = TMath::Pi()*pow(0.4,2);
      double jetArea_ = TMath::Pi()*pow(RCone,2);
      int idx_ = 0;
      for(int i = 0; i < v_isRC.size(); ++i )
      {
         if (v_isRC.at(i)==false){continue;}
         
         if (!jveto->VetoCheck(v_rc1jet_eta.at(i),v_rc1jet_phi.at(i))) {continue;}
         if (!jveto->VetoCheck(v_rc2jet_eta.at(i),v_rc2jet_phi.at(i))) {continue;}

//         double diff_ = rc_diff->at(i);
         double jec_rc1=1.;
         double jec_rc2=1.;
         double jetrc1_pt= v_rc1jet_pt.at(i);
         double jetrc2_pt= v_rc2jet_pt.at(i);
         if (jetrc1_pt < 15.) {jetrc1_pt = 15.;}
         if (jetrc2_pt < 15.) {jetrc2_pt = 15.;}
         //cout << "rc1jet_cor : " << rc1jet_cor->at(i) << " rc1jet_coruncUp : " << rc1jet_coruncUp->at(i) << " rc1jet_coruncDown : " << rc1jet_coruncDown->at(i) << endl;
         if (doJEC) {
            double l2l3jec_ = -999;
           
            jetCorrectorsL23->setJetEta( v_rc1jet_eta.at(i) );
            //jetCorrectorsL23->setJetPt( 15.0 ); // To avoid wrong JEC with low energy region... //
            jetCorrectorsL23->setJetPt( 30.0 ); // To avoid wrong JEC with low energy region... //
      //      jetCorrectorsL23->setJetPt( jetrc1_pt ); // To avoid wrong JEC with low energy region... //
            jetCorrectorsL23->setJetA( jetArea_ );
            jetCorrectorsL23->setRho(rho);
            jec_rc1 = jetCorrectorsL23->getCorrection();

            jetCorrectorsL23->setJetEta( v_rc2jet_eta.at(i) );
            //jetCorrectorsL23->setJetPt( 15.0 ); // To avoid wrong JEC with low energy region... //
            jetCorrectorsL23->setJetPt( 30.0 ); // To avoid wrong JEC with low energy region... //
      //      jetCorrectorsL23->setJetPt( jetrc2_pt ); // To avoid wrong JEC with low energy region... //
            jetCorrectorsL23->setJetA( jetArea_ );
            jetCorrectorsL23->setRho(rho);
            jec_rc2 = jetCorrectorsL23->getCorrection();

         } 
         //if (v_rc1jet_numpf.at(i) == 0 ) {cout << "WRONG!!" << endl;}
         double diff_ = (v_rc1jet_pt.at(i)*jec_rc1)-(v_rc2jet_pt.at(i)*jec_rc2);
         double avg_ = ((v_rc1jet_pt.at(i)*jec_rc1)+(v_rc2jet_pt.at(i)*jec_rc2))/2;
         FillHisto(h_mu[i][nidx],mu, evtWeight);
         FillHisto(h_pvx[i][nidx],NPV_, evtWeight);
         FillHisto(h_Num_pf_RC1[i][nidx], v_rc1jet_numpf.at(i), evtWeight);
         FillHisto(h_Num_pf_RC2[i][nidx], v_rc2jet_numpf.at(i), evtWeight);
         FillHisto(h_Noise_Diff[i][nidx], diff_, evtWeight);
         FillHisto(h_Noise_Rho_Diff[i][ridx], diff_, evtWeight);
         FillHisto(h_Noise_Avg[i][nidx], avg_, evtWeight);
         FillHisto(h_Debug_Phi1[i][nidx],v_rc1jet_phi.at(i), evtWeight);
         FillHisto(h_Debug_Phi2[i][nidx],v_rc2jet_phi.at(i), evtWeight);
         FillHisto(h_Debug_Eta1[i][nidx],v_rc1jet_eta.at(i), evtWeight);
         FillHisto(h_Debug_Eta2[i][nidx],v_rc2jet_eta.at(i), evtWeight);
         
         FillHisto(h_Debug_Eta1vsPhi1[i][nidx],v_rc1jet_eta.at(i),v_rc1jet_phi.at(i), evtWeight);
         FillHisto(h_Debug_Eta2vsPhi2[i][nidx],v_rc2jet_eta.at(i),v_rc2jet_phi.at(i), evtWeight);
         //FillHisto(h_Debug_Phi1vsPhi2[i][nidx],rc1jet_phi->at(i),rc2jet_phi->at(i), evtWeight);
         //FillHisto(h_Debug_Eta1vsEta2[i][nidx],rc1jet_eta->at(i),rc2jet_eta->at(i), evtWeight);
    
         /////FillHisto(h_RC1_pt,SumRCone1.Pt());
         /////FillHisto(h_RC2_pt,SumRCone2.Pt());
   
         /////FillHisto(h_RC1_energy[nidx],SumRCone1.Energy(), evtWeight);
         /////FillHisto(h_RC2_energy[nidx],SumRCone1.Energy(), evtWeight);
   
         /////FillHisto(h_RC1_ptvsDiff[nidx],SumRCone1.Pt(),diff_, evtWeight);
         /////FillHisto(h_RC2_ptvsDiff[nidx],SumRCone2.Pt(),diff_, evtWeight);
         /////FillHisto(h_RC1_ptvsRC2_pt[nidx],SumRCone1.Pt(),SumRCone2.Pt(), evtWeight);
         FillHisto(h_RC_Del_phi[i][nidx],fabs(v_rc1jet_phi.at(i)-v_rc2jet_phi.at(i)), evtWeight);
         FillHisto(h_RC_Del_eta[i][nidx],(v_rc1jet_eta.at(i)-v_rc2jet_eta.at(i)), evtWeight);
         FillHisto(h_RC1_pt[i][nidx],v_rc1jet_pt.at(i)*jec_rc1, evtWeight);
         FillHisto(h_RC2_pt[i][nidx],v_rc2jet_pt.at(i)*jec_rc2, evtWeight);
         FillHisto(h_RC1_Cor[i][nidx],jec_rc1, evtWeight);
         FillHisto(h_RC2_Cor[i][nidx],jec_rc2, evtWeight);
         /////FillHisto(h_RC_Eta1vsEta2[nidx],SumRCone1.Eta(),SumRCone2.Eta(), evtWeight);
         /////FillHisto(h_RC_Phi1vsPhi2[nidx],SumRCone1.Phi(),SumRCone2.Phi(), evtWeight);
         //
         FillHisto(h_Noise_DiffvsRCpt1[i][nidx], diff_, v_rc1jet_pt.at(i)*jec_rc1,evtWeight);
         FillHisto(h_Noise_DiffvsRCpt2[i][nidx], diff_, v_rc2jet_pt.at(i)*jec_rc2,evtWeight);

         FillHisto(h_mu[i][8],mu, evtWeight);
         FillHisto(h_pvx[i][8],NPV_, evtWeight);
         FillHisto(h_Num_pf_RC1[i][8], v_rc1jet_numpf.at(i), evtWeight);
         FillHisto(h_Num_pf_RC2[i][8], v_rc2jet_numpf.at(i), evtWeight);
         FillHisto(h_RC1_pt[i][8],v_rc1jet_pt.at(i)*jec_rc1, evtWeight);
         FillHisto(h_RC2_pt[i][8],v_rc2jet_pt.at(i)*jec_rc2, evtWeight);
         FillHisto(h_RC1_Cor[i][8],jec_rc1, evtWeight);
         FillHisto(h_RC2_Cor[i][8],jec_rc2, evtWeight);

         FillHisto(h_Noise_Diff[i][8], diff_, evtWeight);
         FillHisto(h_Noise_Rho_Diff[i][8], diff_, evtWeight);
         FillHisto(h_Noise_Avg[i][8], avg_, evtWeight);
         FillHisto(h_Debug_Phi1[i][8],v_rc1jet_phi.at(i), evtWeight);
         FillHisto(h_Debug_Phi2[i][8],v_rc2jet_phi.at(i), evtWeight);
         FillHisto(h_Debug_Eta1[i][8],v_rc1jet_eta.at(i), evtWeight);
         FillHisto(h_Debug_Eta2[i][8],v_rc2jet_eta.at(i), evtWeight);
         
         FillHisto(h_Debug_Eta1vsPhi1[i][8],v_rc1jet_eta.at(i),v_rc1jet_phi.at(i), evtWeight);
         FillHisto(h_Debug_Eta2vsPhi2[i][8],v_rc2jet_eta.at(i),v_rc2jet_phi.at(i), evtWeight);
         //FillHisto(h_Debug_Phi1vsPhi2[i][8],rc1jet_phi->at(i),rc2jet_phi->at(i), evtWeight);
         //FillHisto(h_Debug_Eta1vsEta2[i][8],rc1jet_eta->at(i),rc2jet_eta->at(i), evtWeight);
         FillHisto(h_RC_Del_phi[i][8],fabs(v_rc1jet_phi.at(i)-v_rc2jet_phi.at(i)), evtWeight);
         FillHisto(h_RC_Del_eta[i][8],(v_rc1jet_eta.at(i)-v_rc2jet_eta.at(i)), evtWeight);

         FillHisto(h_Noise_DiffvsRCpt1[i][8], diff_, v_rc1jet_pt.at(i)*jec_rc1,evtWeight);
         FillHisto(h_Noise_DiffvsRCpt2[i][8], diff_, v_rc2jet_pt.at(i)*jec_rc2,evtWeight);
      }
      delete L1JetPars;

      delete L2JetPars;
      delete L3JetPars;
      delete ResJetPars;
      delete JecUncert;
      delete jetCorrectorsL23;
      
      //cout << "genjet_pt->size() " << genjet_pt->size() << endl;
       
   }//event loop
  
   printf("Total processed number of events: %lld\n", __tot_evt);
   cout << " passedEvt : " << passedEvt << endl;


}//end Loop function

void NoiseTerm::Start()
{
   fout = new TFile(Form("output/%s/%s",studydir,outfile),"RECREATE");
   fout->cd("");

   TDirectory *dir = gDirectory;
   dir->cd();

   DeclareDefHistos();

   for (int i = 0; i < v_EtaBins.size(); i++ ){
      gSystem->mkdir(Form("output/%s",studydir));
      gSystem->mkdir(Form("output/%s/%s",studydir,v_EtaBins[i].Data()));
      //TFile *tmp = new TFile(Form("output/%s/%s/%s",studydir,v_EtaBins[i].Data(),outfile),"RECREATE");
      v_fout.push_back(new TFile(Form("output/%s/%s/%s",studydir,v_EtaBins[i].Data(),outfile),"RECREATE"));
      //v_fout.push_back(tmp);
      //v_fout[i] = new TFile(Form("output/%s/%s/%s",studydir,v_EtaBins[i].Data(),outfile),"RECREATE");
      cout << "v_fout[i] getname : " << v_fout[i]->GetName() << endl;
      v_fout[i]->cd();
      DeclareHistos(i);
   }
   TString outName = Form("%s",outfile);
   isMC = false;
   //if (outName.Contains("Run")) {isMC = false;}
   if (outName.Contains("Data")) {isMC = false;}
   else {isMC = true;} 

}
void NoiseTerm::DeclareDefHistos()
{
   double pi = TMath::Pi();
   TString npustr = "";
   for (int i =0; i < 9; ++i) 
   {
      if ( i == 0) {npustr = Form("%d <= nPU <= %d", i*10, (i+1)*10);}
      else if ( i < 7) {npustr = Form("%d < nPU <= %d", i*10, (i+1)*10);}
      else if  (i == 7){ npustr = Form("70 < nPU ");}  
      else {npustr = "all";}
 
      cout << " i " << i << " npustr : " << npustr << endl;
      h_Mu[i] = new TH1D(Form("h_mu_%d",i+1), Form("Dist. mu (%s); pile up", npustr.Data()), 1000, 0, 100);   h_Mu[i]->Sumw2();
      h_NPV[i] = new TH1D(Form("h_NPV_%d",i+1), Form("Dist. mu (%s) ; Primary Vertex", npustr.Data()), 1000, 0, 100);   h_NPV[i]->Sumw2();
      h_PF_eta[i] = new TH1D(Form("h_PF_eta_%d",i+1), Form("Eta of PF (%s); #eta", npustr.Data()), 140, -5.2, 5.2);   h_PF_eta[i]->Sumw2();
      h_PF_pt[i] = new TH1D(Form("h_PF_pt_%d",i+1), Form("pT of PF (%s); pT (GeV)", npustr.Data()), 1000, 0, 100);   h_PF_pt[i]->Sumw2();
      h_PF_phi[i] = new TH1D(Form("h_PF_phi_%d",i+1), Form("phi of PF (%s); #phi", npustr.Data()), 640, -3.2, 3.2);   h_PF_phi[i]->Sumw2();
      h_PF_energy[i] = new TH1D(Form("h_PF_energy_%d",i+1), Form("Energy of PF (%s); Energy (GeV)", npustr.Data()), 10000, 0, 100);   h_PF_energy[i]->Sumw2();

   }
}
void NoiseTerm::DeclareHistos(int index)
{
   double pi = TMath::Pi();
   TString npustr = "";
   TString rhostr = "";
   //h_puWeight = new TH1D(Form("h_puWeight"), Form("Dist. PU weight Value; PU weight factor", npustr.Data()), 2000, -10, 10);   h_puWeight->Sumw2();
   for (int i =0; i < 9; ++i) 
   {
      if ( i < 7) {
         npustr = Form("%d < nPU <= %d", i*10, (i+1)*10);
         rhostr = Form("%d < rho <= %d", i*10, (i+1)*10);
      }
      else if  (i == 7){ 
         npustr = Form("70 < nPU ");
         rhostr = Form("70 < rho ");
      }  
      else {npustr = "all";}
     
      cout << "index : " << index << " i " << i << " npustr : " << npustr << endl;
      h_mu[index][i] = new TH1D(Form("h_mu_%d",i+1), Form("Dist. mu (%s); pile up", npustr.Data()), 1000, 0, 100);   h_mu[index][i]->Sumw2();
      h_pvx[index][i] = new TH1D(Form("h_pvx_%d",i+1), Form("Dist. mu (%s) ; Primary Vertex", npustr.Data()), 1000, 0, 100);   h_pvx[index][i]->Sumw2();
      //h_pvx_vs_mu[index][i] = new TH2D(Form("h_pvx_vs_mu_%d",i+1), Form("Dist. Num. PV vs Num. #mu (%s) ; Num. PV; pile up(#mu)", npustr.Data()), 1000, 0, 100, 1000, 0,100);   h_pvx_vs_mu[index][i]->Sumw2();
      //h_mu_vs_pvx[index][i] = new TH2D(Form("h_mu_vs_pvx_%d",i+1), Form("Dist. Num. PV vs Num. #mu (%s) ; pile up(#mu); NPV(#mu)", npustr.Data()), 1000, 0, 100, 1000, 0,100);   h_mu_vs_pvx[index][i]->Sumw2();
      h_rho[index][i] = new TH1D(Form("h_rho_%d",i+1), Form("Dist. rho (%s) ; rho", npustr.Data()), 1000, 0, 100);   h_rho[index][i]->Sumw2();
      h_Num_pf_RC1[index][i] = new TH1D(Form("h_Num_pf_RC1_%d",i+1), Form("Number of PF particles (%s); Number of PF particles", npustr.Data()), 200, 0, 200);   h_Num_pf_RC1[index][i]->Sumw2();
      h_Num_pf_RC2[index][i] = new TH1D(Form("h_Num_pf_RC2_%d",i+1), Form("Number of PF particles (%s); Number of PF particles", npustr.Data()), 200, 0, 200);   h_Num_pf_RC2[index][i]->Sumw2();
      
      //h_PF_eta[index][i] = new TH1D(Form("h_PF_eta_%d",i+1), Form("Eta of PF (%s); #eta", npustr.Data()), 100, -5, 5);   h_PF_eta[index][i]->Sumw2();
      //h_PF_pt[index][i] = new TH1D(Form("h_PF_pt_%d",i+1), Form("pT of PF (%s); pT (GeV)", npustr.Data()), 1000, 0, 100);   h_PF_pt[index][i]->Sumw2();
      //h_PF_phi[index][i] = new TH1D(Form("h_PF_phi_%d",i+1), Form("phi of PF (%s); #phi", npustr.Data()), 100, -1*pi, pi);   h_PF_phi[index][i]->Sumw2();
      //h_PF_phi[index][i] = new TH1D(Form("h_PF_phi_%d",i+1), Form("phi of PF (%s); #phi", npustr.Data()), 640, -3.2, 3.2);   h_PF_phi[index][i]->Sumw2();
      //h_PF_energy[index][i] = new TH1D(Form("h_PF_energy_%d",i+1), Form("Energy of PF (%s); Energy (GeV)", npustr.Data()), 10000, 0, 100);   h_PF_energy[index][i]->Sumw2();
      h_Noise_Diff[index][i] = new TH1D(Form("h_Noise_Diff_%d",i+1),Form("h_Nmise_Diff (%s);pT RC1 - pT RC2", npustr.Data()),10000,-50,50); h_Noise_Diff[index][i]->Sumw2();
      h_Noise_Rho_Diff[index][i] = new TH1D(Form("h_Noise_Rho_Diff_%d",i+1),Form("h_Nmise_Diff (%s);pT RC1 - pT RC2", rhostr.Data()),10000,-50,50); h_Noise_Rho_Diff[index][i]->Sumw2();
      h_Noise_Avg[index][i] = new TH1D(Form("h_Noise_Avg_%d",i+1),Form("h_Nmise_Avg (%s);pT (RC1 + pT RC2)/2", npustr.Data()),10000,-50,50); h_Noise_Avg[index][i]->Sumw2();
      //cout << "In DeclareHistos " << h_Noise_Avg[index][i] ->GetName() << endl;
      //h_Debug_Phi1[index][i] = new TH1D(Form("h_Debug_Phi1_%d",i+1),Form("Dist. #phi of RC1 (%s)", npustr.Data()),100,-1*pi,pi); h_Debug_Phi1[index][i]->Sumw2();
      h_Debug_Phi1[index][i] = new TH1D(Form("h_Debug_Phi1_%d",i+1),Form("Dist. #phi of RC1 (%s)", npustr.Data()),640,-3.2,3.2); h_Debug_Phi1[index][i]->Sumw2();
      //h_Debug_Phi2[index][i] = new TH1D(Form("h_Debug_Phi2_%d",i+1),Form("Dist. #phi of RC2 (%s)", npustr.Data()),100,-1*pi,pi); h_Debug_Phi2[index][i]->Sumw2();
      h_Debug_Phi2[index][i] = new TH1D(Form("h_Debug_Phi2_%d",i+1),Form("Dist. #phi of RC2 (%s)", npustr.Data()),100,-3.2,3.2); h_Debug_Phi2[index][i]->Sumw2();
      h_Debug_Eta1[index][i] = new TH1D(Form("h_Debug_Eta1_%d",i+1),Form("Dist. #eta of RC1 (%s)", npustr.Data()),1400,-5.2,5.2); h_Debug_Eta1[index][i]->Sumw2();
      h_Debug_Eta2[index][i] = new TH1D(Form("h_Debug_Eta2_%d",i+1),Form("Dist. #eta of RC2 (%s)", npustr.Data()),1400,-5.2,5.2); h_Debug_Eta2[index][i]->Sumw2();
      
      //h_Debug_Eta1vsPhi1[index][i] = new TH2D(Form("h_Debug_Eta1vsPhi1_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),1000,-5,5,100,-1*pi,pi); h_Debug_Eta1vsPhi1[index][i]->Sumw2();
      h_Debug_Eta1vsPhi1[index][i] = new TH2D(Form("h_Debug_Eta1vsPhi1_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),1000,-5,5,640,-3.2,3.2); h_Debug_Eta1vsPhi1[index][i]->Sumw2();
      //h_Debug_Eta2vsPhi2[index][i] = new TH2D(Form("h_Debug_Eta2vsPhi2_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),1000,-5,5,100,-1*pi,pi); h_Debug_Eta2vsPhi2[index][i]->Sumw2();
      h_Debug_Eta2vsPhi2[index][i] = new TH2D(Form("h_Debug_Eta2vsPhi2_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),1000,-5,5,640,-3.2,3.2); h_Debug_Eta2vsPhi2[index][i]->Sumw2();
      
      //h_Debug_Phi1vsPhi2[index][i] = new TH2D(Form("h_Debug_Phi1vsPhi2_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),100,-1*pi,pi,100,-1*pi,pi); h_Debug_Phi1vsPhi2[index][i]->Sumw2();
      //h_Debug_Phi1vsPhi2[index][i] = new TH2D(Form("h_Debug_Phi1vsPhi2_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),640,-3.2,3.2,640, -3.2,3.2); h_Debug_Phi1vsPhi2[index][i]->Sumw2();
      //h_Debug_Eta1vsEta2[index][i] = new TH2D(Form("h_Debug_Eta1vsEta2_%d",i+1),Form("Dist. #eta of RC1 vs RC2 (%s)", npustr.Data()),1000,-5,5,1000,-5,5); h_Debug_Eta1vsEta2[index][i]->Sumw2();
      
      //h_RC1_ptvsRC2_pt[index][i] = new TH2D(Form("h_RC1_ptvsRC2_pt_%d",i+1),Form("Dist. p_{t} of RC1 vs Diff (%s)", npustr.Data()),1000,0,100,1000,0,100); h_RC1_ptvsRC2_pt[index][i]->Sumw2();
      //h_RC1_ptvsDiff[index][i] = new TH2D(Form("h_RC1_ptvsDiff_%d",i+1),Form("Dist. p_{t} of RC1 vs Diff (%s)", npustr.Data()),1000,0,100,1000,-50,50); h_RC1_ptvsDiff[index][i]->Sumw2();
      //h_RC2_ptvsDiff[index][i] = new TH2D(Form("h_RC2_ptvsDiff_%d",i+1),Form("Dist. p_{t} of RC2 vs Diff (%s)", npustr.Data()),1000,0,100,1000,-50,50); h_RC2_ptvsDiff[index][i]->Sumw2();
      
      h_RC1_pt[index][i] = new TH1D(Form("h_RC1_pt_%d",i+1), Form("pT of RC1 (%s); pT", npustr.Data()), 40000, -200, 200);   h_RC1_pt[index][i]->Sumw2();
      h_RC2_pt[index][i] = new TH1D(Form("h_RC2_pt_%d",i+1), Form("pT of RC2 (%s); pT", npustr.Data()), 40000, -200, 200);   h_RC2_pt[index][i]->Sumw2();
      h_RC1_eta[index][i] = new TH1D(Form("h_RC1_eta_%d", i+1), Form("Eta of PF (%s); #eta", npustr.Data()), 140, -5.2, 5.2);   h_RC1_eta[index][i]->Sumw2();
      h_RC2_eta[index][i] = new TH1D(Form("h_RC2_eta_%d",i+1), Form("Eta of PF (%s); #eta", npustr.Data()), 140, -5.2, 5.2);   h_RC2_eta[index][i]->Sumw2();
      //h_RC1_phi[index][i] = new TH1D(Form("h_RC1_phi_%d",i+1), Form("phi of RC1 (%s); #phi", npustr.Data()), 100, -1*pi, pi);   h_RC1_phi[index][i]->Sumw2();
      h_RC1_phi[index][i] = new TH1D(Form("h_RC1_phi_%d",i+1), Form("phi of RC1 (%s); #phi", npustr.Data()), 640, -3.2, 3.2);   h_RC1_phi[index][i]->Sumw2();
      //h_RC2_phi[index][i] = new TH1D(Form("h_RC2_phi_%d",i+1), Form("phi of RC2 (%s); #phi", npustr.Data()), 100, -1*pi, pi);   h_RC2_phi[index][i]->Sumw2();
      h_RC2_phi[index][i] = new TH1D(Form("h_RC2_phi_%d",i+1), Form("phi of RC2 (%s); #phi", npustr.Data()), 640, -3.2, 3.2);   h_RC2_phi[index][i]->Sumw2();
      h_RC1_energy[index][i] = new TH1D(Form("h_RC1_energy_%d",i+1), Form("Energy of RC1 (%s); pT", npustr.Data()), 40000, 200, 200);   h_RC1_energy[index][i]->Sumw2();
      h_RC2_energy[index][i] = new TH1D(Form("h_RC2_energy_%d",i+1), Form("Energy of RC2 (%s); pT", npustr.Data()), 40000, -200, 200);   h_RC2_energy[index][i]->Sumw2();
      
      h_RC1_Cor[index][i] = new TH1D(Form("h_RC1_Cor_%d",i+1), Form("JEC factor of RC1 (%s); pT", npustr.Data()), 1000, -5, 5);   h_RC1_Cor[index][i]->Sumw2();
      h_RC2_Cor[index][i] = new TH1D(Form("h_RC2_Cor_%d",i+1), Form("JEC factor of RC2 (%s); pT", npustr.Data()), 1000, -5, 5);   h_RC2_Cor[index][i]->Sumw2();

      h_RC_Del_phi[index][i] = new TH1D(Form("h_RC_Del_phi_%d",i+1), Form("#Delta #phi between RC1 and RC2 (%s); |#Delta #phi|", npustr.Data()), 315, 0, 3.15);   h_RC_Del_phi[index][i]->Sumw2();
      h_RC_Del_eta[index][i] = new TH1D(Form("h_RC_Del_eta_%d",i+1), Form("#Delta #eta between RC1 and RC2 (%s); #Delta #eta", npustr.Data()), 140, -5.2, 5.2);   h_RC_Del_eta[index][i]->Sumw2();
      //h_RC_Phi1vsPhi2[index][i] = new TH2D(Form("h_RC_Phi1vsPhi2_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),100,-1*pi,pi,100,-1*pi,pi); h_RC_Phi1vsPhi2[index][i]->Sumw2();
      h_RC_Phi1vsPhi2[index][i] = new TH2D(Form("h_RC_Phi1vsPhi2_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),640,-3.2,3.2,640,-3.2,3.2); h_RC_Phi1vsPhi2[index][i]->Sumw2();
      h_RC_Eta1vsEta2[index][i] = new TH2D(Form("h_RC_Eta1vsEta2_%d",i+1),Form("Dist. #eta of RC1 vs RC2 (%s)", npustr.Data()),1400,-5.2,5.2,1400,-5.2,5.2); h_RC_Eta1vsEta2[index][i]->Sumw2();
      h_Noise_DiffvsRCpt1[index][i] = new TH2D(Form("h_Noise_DiffvsRCpt1_%d",i+1),Form("Noise Term vs  (%s); p_{T} RC1 - p_{T} RC2; p_{T} RC1", npustr.Data()),1000,-50,50,400, -200, 200); h_Noise_DiffvsRCpt1[index][i]->Sumw2();
      h_Noise_DiffvsRCpt2[index][i] = new TH2D(Form("h_Noise_DiffvsRCpt2_%d",i+1),Form("Noise Term vs  (%s); p_{T} RC1 - p_{T} RC2; p_{T} RC2", npustr.Data()),1000,-50,50,400, -200, 200); h_Noise_DiffvsRCpt2[index][i]->Sumw2();
   }
}

void NoiseTerm::End()
{
   fout->Write();
   fout->Close();
   //for(int i = 0; i < v_fout.size(); ++i){
   //for(int i = 0; i < 13; ++i){
   for (int i = 0; i < v_EtaBins.size(); i++ ){
      v_fout[i]->Write();
      v_fout[i]->Close();
   }
}

void NoiseTerm::SetOutputFileName(char* studyname, char *outname)
{   
   studydir = studyname;
   outfile = outname;
}

float NoiseTerm::getdR(float eta1, float eta2, float phi1, float phi2)
{
  float detasq = (eta1-eta2)*(eta1-eta2);
  float dphi = fabs(phi1 - phi2);
  if(dphi > TMath::Pi())
    dphi = 2*TMath::Pi() - dphi;
  float dphisq = dphi*dphi;
  return sqrt( dphisq + detasq );
}
int NoiseTerm::FindIndexNPU(float npu_)
{
   int indx_npu =  -1;
   if (npu_ <= 10) {indx_npu =0; }
   else if ( npu_ <= 20) {indx_npu =1; }
   else if ( npu_ <= 30) {indx_npu =2; }
   else if ( npu_ <= 40) {indx_npu =3; }
   else if ( npu_ <= 50) {indx_npu =4; }
   else if ( npu_ <= 60) {indx_npu =5; }
   else if ( npu_ <= 70) {indx_npu =6; }
   else {indx_npu =7; }  
   //cout << "Check !! " << " npu_ " << npu_ << " indx " << indx_npu << endl;
   return indx_npu;   
}


bool NoiseTerm::IsPFCHS(int itype)
{
   bool ispfchs = false;
   if (itype == 1) {ispfchs=true;}
   if (itype == 2) {ispfchs=true;}
   if (itype == 3) {ispfchs=true;}
   if (itype == 4) {ispfchs=true;}
   if (itype == 5) {ispfchs=true;}
//   if (itype == 6) {ispfchs=true;}
   return ispfchs;
/*  chm : 0
chu : 1
nh : 2
ne : 3
hfh : 4
hfe : 5
lep : 6
untrk : 7
numFlavors : 8 
*/
}
Double_t fJEC(Double_t *x, Double_t *p) {
double ptuncorr = *x;
return (ptuncorr*p[0]);
}
bool NoiseTerm::Trigger()
{

   int ptrigindex;
   bool trigpass;
   
   ptrigindex =0;
   trigpass = false; 
   TString trgName = ""; 
   for (int i =0; i < trigger_name->size(); i++)
   {
     for (int j = 0; j < trigName.size(); j++)
      {  
         trgName = trigName[j];
         if ( TString( trigger_name->at(i) ).Contains( trigName.at(j) ) )//TString clone 
         {
            //cout << "trigger_name->at(i) " << trigger_name->at(i)  << " pass : " << trigger_pass->at(i) << endl;
            if ( ( trigger_pass->at(i)  ) &&
                !( trigger_error->at(i) ) && 
                 ( trigger_run->at(i) )      )
            { ptrigindex = ptrigindex+1; }
         }
      }
   }
   if ( ptrigindex > 0 ) { trigpass = true; }
//   if (trigpass==false)cout << "fail  " << endl;
   return trigpass;
}
double NoiseTerm::GetPreScaleFactor()
{
   int ptrigindex;
   
   ptrigindex =0;
   TString trgName = "";
   double prescale_ = 1.;
   if (!doPreSC) {return prescale_;}
   if (!isMC) {
       for (int i =0; i < trigger_name->size(); i++)
       {
         for (int j = 0; j < trigName.size(); j++)
          {  
             trgName = trigName[j];
             if ( TString( trigger_name->at(i) ).Contains( trigName.at(j) ) )//TString clone 
             {
                //cout << "trigger_name->at(i) " << trigger_name->at(i)  << " pass : " << trigger_pass->at(i) << endl;
                if ( ( trigger_pass->at(i)  ) &&
                    !( trigger_error->at(i) ) && 
                     ( trigger_run->at(i) )      )
                { ptrigindex = ptrigindex+1; 
                  //cout << "trigger_name->at(i) " << trigger_name->at(i)  << " prescale : " << trigger_prescale->at(i) << endl;
                  if (trigger_prescale->at(i) >prescale_ ) prescale_= trigger_prescale->at(i);
                }
             }
          }
       }
   }
   //if ( ptrigindex > 0 ) { trigpass = true; }
   
   return prescale_;
}

RandCone NoiseTerm::SetRandomCone(double upperEta, double lowerEta)
{   

   bool isPFInCone1=false;
   bool isPFInCone2=false;
   
   double pt1=0;
   double pt2=0;
   
   TLorentzVector clusterVectorE;
   
   TLorentzVector SumRCone1;
   TLorentzVector SumRCone2;
   
   
   TLorentzVector rndmCone1;
   TLorentzVector rndmCone2;

   TLorentzVector JetCone1;
   TLorentzVector JetCone2;
   
   double eta1 = -999;
   double eta2 = -999;
   double phi1 = -999;
   double phi2 = -999;
   
   ////////////////////////////////////////////
   /// How many choose random cone in event ///
   ////////////////////////////////////////////
   
   int NumOfSelct=0;
   int maxSelct = 1;
   
   double RangePhi=TMath::Pi()*2.;
   //double upperEta = 0.5;
   //double lowerEta = 0;
   //upperEta = 0.5;
   //lowerEta = 0;
   double RangeEta=(upperEta-lowerEta)*2;


   double depositERC1 = 0;
   double depositERC2 = 0;

   double depositETRC1 = 0;
   double depositETRC2 = 0;
   int num_rc1_pf =0;
   int num_rc2_pf =0;
   /// RC ///
   while ((isPFInCone1==false||isPFInCone2==false)&&NumOfSelct<maxSelct) 
   {
      NumOfSelct++;
      phi1 = rand->Uniform(-1*TMath::Pi(),TMath::Pi());
      phi2 = rand->Uniform(-1*TMath::Pi(),TMath::Pi());
      
      double rndValue=rand->Rndm();
      
      if (rndValue<.5) { eta1=-upperEta+rndValue*RangeEta; } 
      else { eta1=lowerEta+(rndValue-.5)*RangeEta; }
      
      rndValue=rand->Rndm();
      if (rndValue<.5) { eta2=-upperEta+rndValue*RangeEta; }
      else { eta2=lowerEta+(rndValue-.5)*RangeEta; }
    
      if (eta1*eta2 > 0.) {eta2= -1*eta2;}
  
      // For debuging the codes //
      
      // Set Direction of RCs 
      rndmCone1.SetPtEtaPhiE(500,eta1,phi1,500); 
      rndmCone2.SetPtEtaPhiE(2000,eta2,phi2,2000);
      
      SumRCone1.SetXYZT(0,0,0,0);
      SumRCone2.SetXYZT(0,0,0,0);
      
      isPFInCone1=false;
      isPFInCone2=false;
      
      bool inBothCones=false;
      bool tempInCone1=false;
      bool tempInCone2=false;
      
      //double RCone = 0.4;
      
      if (fabs(rndmCone2.DeltaR(rndmCone1))<2*RCone) {continue;}
      if (!jveto->VetoCheck(eta1,phi1)) {continue;}
      if (!jveto->VetoCheck(eta2,phi2)) {continue;}
      bool overlayed_ = false;
      for (unsigned int i=0;i<pf_pt->size();i++) 
      {
         if (IsPFCHS(pf_type->at(i))==false) { /*cout <<  "pf type ? " << pf_type.at(i) << endl; */continue;}
         clusterVectorE.SetPtEtaPhiE(pf_pt->at(i),pf_eta->at(i),pf_phi->at(i),pf_energy->at(i));
         tempInCone1=false;
         tempInCone2=false;
         if (fabs(rndmCone1.DeltaR(clusterVectorE))<RCone) 
         {
            
            SumRCone1=SumRCone1+clusterVectorE;
            isPFInCone1=true;
            tempInCone1=true;
            depositERC1 += pf_energy->at(i);
            num_rc1_pf++;
            depositETRC1 += pf_et->at(i);
         }
         if (fabs(rndmCone2.DeltaR(clusterVectorE))<RCone) 
         {
            SumRCone2=SumRCone2+clusterVectorE;
            isPFInCone2=true;
            tempInCone2=true;
            depositERC2 += pf_energy->at(i);
            depositETRC2 += pf_et->at(i);
            num_rc2_pf++;
         }
         if (tempInCone1==true&&tempInCone2==true) {
            overlayed_ = true;             
            cout <<"In both cones"<<endl;// inBothCones = true;
            break;

         }
      }
      if (overlayed_) {continue;}
      
   }/// End of while

   /// Obtain difference of pt beteween RC1 and RC2
   double diff_ = -99999999;

   RandCone tmp;
   bool isRC = false;
   if ((NumOfSelct<=maxSelct)&&(isPFInCone1==true)&&(isPFInCone2==true)) 
   {
      isRC=true;

      diff_ =  depositETRC1-depositETRC2;
      JetCone1.SetPtEtaPhiE(depositETRC1,eta1,phi1,depositETRC1);
      JetCone2.SetPtEtaPhiE(depositETRC2,eta2,phi2,depositETRC2);
   }
   tmp.diff_ = diff_;
   tmp.findRC = isRC;
   tmp.rc1 = JetCone1;
   tmp.rc2 = JetCone2;
   tmp.num_pfcan_rc1 = num_rc1_pf;
   tmp.num_pfcan_rc2 = num_rc2_pf;
   tmp.vecrc1 = SumRCone1;
   tmp.vecrc2 = SumRCone2;
   return tmp;  
}

