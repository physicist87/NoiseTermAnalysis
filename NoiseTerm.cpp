#define NoiseTerm_cxx

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "TMath.h"
#include "TVector2.h"
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
      // JEC (L2Relative+L3Absolute(+L2L3Residual for DATA), no L1FastJet) and
      // the jet veto map are both applied via NoiseTermCorrections
      // (correctionlib) below, per-random-cone - see GetCorrectedJetPt()/
      // ShouldVetoJet() calls further down. Replaces the old per-event
      // FactorizedJetCorrector + txt-file JEC setup that used to live here.

      vector<int> v_rc1jet_numpf, v_rc2jet_numpf;
      vector<float> v_rc1jet_eta, v_rc1jet_phi, v_rc1jet_pt, v_rc1jet_energy;
      vector<float> v_rc2jet_eta, v_rc2jet_phi, v_rc2jet_pt, v_rc2jet_energy;
      vector<float> v_rc_diff;
      std::vector<bool> v_isRC;
      std::vector<RandCone> v_rcinfo;
 
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
      v_rcinfo.clear();
      v_rc_diff.clear();
      vector<double> vEtaBins = {0,0.261, 0.522, 0.783, 1.044, 1.305, 1.566, 1.740, 1.930, 2.043, 2.172, 2.322, 2.500, 2.650, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
      //cout << "jentry : " << jentry  << endl;
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
            v_rcinfo.push_back(tmp);
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

         if (corrections_->ShouldVetoJet(v_rc1jet_eta.at(i),v_rc1jet_phi.at(i))) {continue;}
         if (corrections_->ShouldVetoJet(v_rc2jet_eta.at(i),v_rc2jet_phi.at(i))) {continue;}

//         double diff_ = rc_diff->at(i);
         // RC pt is clamped to 15 GeV before JEC lookup (kept from the old
         // logic - avoids an unreliable JEC evaluation in the very-low-pt
         // region where a random cone's raw pt sum can be close to zero).
         double jetrc1_pt= v_rc1jet_pt.at(i);
         double jetrc2_pt= v_rc2jet_pt.at(i);
         if (jetrc1_pt < 15.) {jetrc1_pt = 15.;}
         if (jetrc2_pt < 15.) {jetrc2_pt = 15.;}

         double corrected_rc1_pt = corrections_->GetCorrectedJetPt(
            15.0, v_rc1jet_eta.at(i), v_rc1jet_phi.at(i), jetArea_, rho, (unsigned int)run);
         double corrected_rc2_pt = corrections_->GetCorrectedJetPt(
            15.0, v_rc2jet_eta.at(i), v_rc2jet_phi.at(i), jetArea_, rho, (unsigned int)run);
         double jec_rc1 = corrected_rc1_pt / 15.0;
         double jec_rc2 = corrected_rc2_pt / 15.0;

         //if (v_rc1jet_numpf.at(i) == 0 ) {cout << "WRONG!!" << endl;}
         double diff_ = (v_rc1jet_pt.at(i)*jec_rc1)-(v_rc2jet_pt.at(i)*jec_rc2);
         double avg_ = ((v_rc1jet_pt.at(i)*jec_rc1)+(v_rc2jet_pt.at(i)*jec_rc2))/2;
         FillHisto(h_mu[i][nidx],mu, evtWeight);
         FillHisto(h_pvx[i][nidx],NPV_, evtWeight);
         FillHisto(h_Num_pf_RC1[i][nidx], v_rc1jet_numpf.at(i), evtWeight);
         FillHisto(h_Num_pf_RC2[i][nidx], v_rc2jet_numpf.at(i), evtWeight);
         FillHisto(h_Noise_Diff[i][nidx], diff_, evtWeight);
         // split by cone pT (both pT values include the JEC factor, like diff_)
         const double splitPtCut = 5.0;
         const int splitCat = (v_rc1jet_pt.at(i)*jec_rc1 >= splitPtCut ? 2 : 0) + (v_rc2jet_pt.at(i)*jec_rc2 >= splitPtCut ? 1 : 0);
         FillHisto(h_Noise_Diff_cat[i][nidx][splitCat], diff_, evtWeight);
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
         FillHisto(h_Noise_Diff_cat[i][8][splitCat], diff_, evtWeight);
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

         // PF-type / HCAL-depth decomposition (Mikko stages 2-4); same JEC factors as the total.
         FillPFTypeHistos(i, nidx, v_rcinfo.at(i), jec_rc1, jec_rc2, evtWeight);
         FillPFTypeHistos(i, 8,    v_rcinfo.at(i), jec_rc1, jec_rc2, evtWeight);
      }

      //cout << "genjet_pt->size() " << genjet_pt->size() << endl;
       
   }//event loop
  
   printf("Total processed number of events: %lld\n", __tot_evt);


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
      for (int k = 0; k < 4; ++k) {
         h_Noise_Diff_cat[index][i][k] = new TH1D(Form("h_Noise_Diff_cat%d_%d",k+1,i+1),Form("h_Noise_Diff cat%d (%s);pT RC1 - pT RC2", k+1, npustr.Data()),10000,-50,50); h_Noise_Diff_cat[index][i][k]->Sumw2();
      }
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
      // h_RC_Phi1vsPhi2 / h_RC_Eta1vsEta2 are never filled; keep them (same names) but with 1x1 bins.
      // At full binning they cost ~38 MB each per (eta bin, PU bin), i.e. ~6.5 GB per job.
      h_RC_Phi1vsPhi2[index][i] = new TH2D(Form("h_RC_Phi1vsPhi2_%d",i+1),Form("Dist. #phi of RC1 vs RC2 (%s)", npustr.Data()),1,-3.2,3.2,1,-3.2,3.2); h_RC_Phi1vsPhi2[index][i]->Sumw2();
      h_RC_Eta1vsEta2[index][i] = new TH2D(Form("h_RC_Eta1vsEta2_%d",i+1),Form("Dist. #eta of RC1 vs RC2 (%s)", npustr.Data()),1,-5.2,5.2,1,-5.2,5.2); h_RC_Eta1vsEta2[index][i]->Sumw2();
      h_Noise_DiffvsRCpt1[index][i] = new TH2D(Form("h_Noise_DiffvsRCpt1_%d",i+1),Form("Noise Term vs  (%s); p_{T} RC1 - p_{T} RC2; p_{T} RC1", npustr.Data()),1000,-50,50,400, -200, 200); h_Noise_DiffvsRCpt1[index][i]->Sumw2();
      BookPFTypeHistos(index, i, npustr);
      h_Noise_DiffvsRCpt2[index][i] = new TH2D(Form("h_Noise_DiffvsRCpt2_%d",i+1),Form("Noise Term vs  (%s); p_{T} RC1 - p_{T} RC2; p_{T} RC2", npustr.Data()),1000,-50,50,400, -200, 200); h_Noise_DiffvsRCpt2[index][i]->Sumw2();
   }
}

static const char* kGrpName[kNGrp + 1] = {"CH", "NH", "PH", "HFH", "HFE", "OTH", "ALL"};
static const char* kMName[kNMode] = {"Puppi", "Raw", "CHS"};
static const int kPairA[kNPair] = {gCH, gCH, gNH, gHFH};
static const int kPairB[kNPair] = {gNH, gPH, gPH, gHFE};

void NoiseTerm::BookPFTypeHistos(int index, int i, const TString &npustr)
{
   for (int m = 0; m < kNMode; ++m) {
      for (int g = 0; g <= kNGrp; ++g) {
         h_dPT_grp[index][i][g][m] = new TH1D(Form("h_d%s_%s_%d", kMName[m], kGrpName[g], i+1),
            Form("dPT RC1-RC2 %s %s (%s); dPT", kMName[m], kGrpName[g], npustr.Data()), 1000, -50, 50);
         h_dPT_grp[index][i][g][m]->Sumw2();
         h_ET_grp[index][i][g][m] = new TH1D(Form("h_ET%s_%s_%d", kMName[m], kGrpName[g], i+1),
            Form("cone ET %s %s (%s); ET", kMName[m], kGrpName[g], npustr.Data()), 500, 0, 50);
         h_ET_grp[index][i][g][m]->Sumw2();
         p_d2VsAvg_grp[index][i][g][m] = new TProfile(Form("p_d2VsAvg%s_%s_%d", kMName[m], kGrpName[g], i+1),
            Form("<dPT^2> vs mean ET %s %s (%s); (ET1+ET2)/2; <dPT^2>", kMName[m], kGrpName[g], npustr.Data()), 60, 0, 30);
      }
      for (int k = 0; k < kNDepth; ++k) {
         h_dPT_depth[index][i][k][m] = new TH1D(Form("h_d%s_Depth%d_%d", kMName[m], k+1, i+1),
            Form("dPT RC1-RC2 %s HCAL depth %d (%s); dPT", kMName[m], k+1, npustr.Data()), 1000, -50, 50);
         h_dPT_depth[index][i][k][m]->Sumw2();
         h_ET_depth[index][i][k][m] = new TH1D(Form("h_ET%s_Depth%d_%d", kMName[m], k+1, i+1),
            Form("cone ET %s HCAL depth %d (%s); ET", kMName[m], k+1, npustr.Data()), 500, 0, 50);
         h_ET_depth[index][i][k][m]->Sumw2();
         p_d2VsAvg_depth[index][i][k][m] = new TProfile(Form("p_d2VsAvg%s_Depth%d_%d", kMName[m], k+1, i+1),
            Form("<dPT^2> vs mean ET %s HCAL depth %d (%s); (ET1+ET2)/2; <dPT^2>", kMName[m], k+1, npustr.Data()), 60, 0, 30);
      }
      h_dPT_noJEC[index][i][m] = new TH1D(Form("h_dNoJEC%s_ALL_%d", kMName[m], i+1),
         Form("dPT RC1-RC2 %s ALL, no JEC (%s); dPT", kMName[m], npustr.Data()), 1000, -50, 50);
      h_dPT_noJEC[index][i][m]->Sumw2();
      h_ET_noJEC[index][i][m] = new TH1D(Form("h_ETNoJEC%s_ALL_%d", kMName[m], i+1),
         Form("cone ET %s ALL, no JEC (%s); ET", kMName[m], npustr.Data()), 500, 0, 50);
      h_ET_noJEC[index][i][m]->Sumw2();
      for (int p = 0; p < kNPair; ++p) {
         h2_dPT_pair[index][i][p][m] = new TH2D(Form("h2_d%s_%s_%s_%d", kMName[m], kGrpName[kPairA[p]], kGrpName[kPairB[p]], i+1),
            Form("dPT %s vs dPT %s, %s (%s); dPT %s; dPT %s", kGrpName[kPairA[p]], kGrpName[kPairB[p]], kMName[m], npustr.Data(),
                 kGrpName[kPairA[p]], kGrpName[kPairB[p]]), 80, -20, 20, 80, -20, 20);
      }
   }
   h_DepthFracMean[index][i] = new TH1D(Form("h_DepthFracMean_%d", i+1),
      Form("mean over hadronic PF of sum_K pf_hcalDepthK/pf_energy (%s); sum of depth fractions", npustr.Data()), 300, 0, 3);
   h_DepthFracMean[index][i]->Sumw2();
}

void NoiseTerm::FillPFTypeHistos(int ieta, int islot, const RandCone &rc, double jec1, double jec2, double w)
{
   for (int m = 0; m < kNMode; ++m) {
      const double *g1 = (m == 0) ? rc.gP1 : (m == 1) ? rc.gR1 : rc.gC1;
      const double *g2 = (m == 0) ? rc.gP2 : (m == 1) ? rc.gR2 : rc.gC2;
      const double *d1 = (m == 0) ? rc.dP1 : (m == 1) ? rc.dR1 : rc.dC1;
      const double *d2 = (m == 0) ? rc.dP2 : (m == 1) ? rc.dR2 : rc.dC2;

      // CHS uses the PUPPI L2L3 factor (no AK4PFchs JEC for 2025); h_*NoJEC* keep the uncorrected totals.
      double x1[kNGrp + 1], x2[kNGrp + 1];
      double s1 = 0., s2 = 0., u1 = 0., u2 = 0.;
      for (int g = 0; g < kNGrp; ++g) {
         x1[g] = g1[g] * jec1;  x2[g] = g2[g] * jec2;
         s1 += x1[g];  s2 += x2[g];
         u1 += g1[g];  u2 += g2[g];
      }
      x1[kNGrp] = s1;  x2[kNGrp] = s2;
      FillHisto(h_dPT_noJEC[ieta][islot][m], u1 - u2, w);
      FillHisto(h_ET_noJEC[ieta][islot][m], u1, w);
      FillHisto(h_ET_noJEC[ieta][islot][m], u2, w);

      for (int g = 0; g <= kNGrp; ++g) {
         const double d = x1[g] - x2[g];
         const double a = 0.5 * (x1[g] + x2[g]);
         FillHisto(h_dPT_grp[ieta][islot][g][m], d, w);
         FillHisto(h_ET_grp[ieta][islot][g][m], x1[g], w);
         FillHisto(h_ET_grp[ieta][islot][g][m], x2[g], w);
         p_d2VsAvg_grp[ieta][islot][g][m]->Fill(a, d * d, w);
      }
      for (int k = 0; k < kNDepth; ++k) {
         const double e1 = d1[k] * jec1, e2 = d2[k] * jec2;
         FillHisto(h_dPT_depth[ieta][islot][k][m], e1 - e2, w);
         FillHisto(h_ET_depth[ieta][islot][k][m], e1, w);
         FillHisto(h_ET_depth[ieta][islot][k][m], e2, w);
         p_d2VsAvg_depth[ieta][islot][k][m]->Fill(0.5 * (e1 + e2), (e1 - e2) * (e1 - e2), w);
      }
      for (int p = 0; p < kNPair; ++p) {
         FillHisto(h2_dPT_pair[ieta][islot][p][m],
                   x1[kPairA[p]] - x2[kPairA[p]], x1[kPairB[p]] - x2[kPairB[p]], w);
      }
   }
   if (rc.nHad1 > 0) FillHisto(h_DepthFracMean[ieta][islot], rc.depFrac1 / rc.nHad1, w);
   if (rc.nHad2 > 0) FillHisto(h_DepthFracMean[ieta][islot], rc.depFrac2 / rc.nHad2, w);
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
   bool isPFInCone1 = false;
   bool isPFInCone2 = false;

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

   int NumOfSelct = 0;
   int maxSelct = 10; // retry budget per eta bin for the random-cone search (was 5)

   double RangeEta = (upperEta - lowerEta) * 2;

   double depositERC1 = 0;
   double depositERC2 = 0;

   double depositETRC1 = 0;
   double depositETRC2 = 0;

   int num_rc1_pf = 0;
   int num_rc2_pf = 0;

   // PF-type / HCAL-depth ET sums per cone (reset on every retry, copied to the result at the end).
   RandCone acc;
   const std::vector<float>* depthVec[kNDepth] = {
      pf_hcalDepth1, pf_hcalDepth2, pf_hcalDepth3, pf_hcalDepth4,
      pf_hcalDepth5, pf_hcalDepth6, pf_hcalDepth7 };

   double total_weight1 = 0.;
   double total_weight2 = 0.;

   // Cone acceptance threshold on the cumulative in-cone weight. PFCHS
   // particles always carry weight 1.0, so a single accepted particle
   // already clears this (same behavior as before). PUPPI particles carry
   // pf_puppiW, so this is effectively a Sum(pf_puppiW) > 0 requirement -
   // matches the threshold OffsetTreeMaker's SetRandomCone_PUPPI uses for
   // its usePuppiWsumCut mode.
   const double kWeightSumCut = 0.0001;

   // Loop invariants hoisted out of the per-PF loop.
   const bool isCHS   = (JetType_ == "AK4PFchs");
   const bool isPuppi = (JetType_ == "AK4PFPuppi");
   const double RCone2 = RCone * RCone;

   while ((isPFInCone1 == false || isPFInCone2 == false) && NumOfSelct < maxSelct)
   {
      NumOfSelct++;
      phi1 = rand->Uniform(-1 * TMath::Pi(), TMath::Pi());
      phi2 = rand->Uniform(-1 * TMath::Pi(), TMath::Pi());

      double rndValue = rand->Rndm();

      if (rndValue < .5) { eta1 = -upperEta + rndValue * RangeEta; }
      else { eta1 = lowerEta + (rndValue - .5) * RangeEta; }

      rndValue = rand->Rndm();
      if (rndValue < .5) { eta2 = -upperEta + rndValue * RangeEta; }
      else { eta2 = lowerEta + (rndValue - .5) * RangeEta; }

      if (eta1 * eta2 > 0.) { eta2 = -1 * eta2; }

      rndmCone1.SetPtEtaPhiE(500, eta1, phi1, 500);
      rndmCone2.SetPtEtaPhiE(2000, eta2, phi2, 2000);

      // Require the two cones to be well-separated before doing any
      // per-particle work - retry with a fresh random draw if not.
      if (fabs(rndmCone2.DeltaR(rndmCone1)) < 2 * RCone) { continue; }

      // Jet veto map: retry with a fresh random draw if either cone center
      // falls in a vetoed region. Previously this check lived inside the
      // per-particle loop below, where a "continue" only skipped that one
      // particle instead of retrying the cone draw - so the veto never
      // actually took effect. Moved here to match the veto's intent.
      if (corrections_->ShouldVetoJet(eta1, phi1)) { continue; }
      if (corrections_->ShouldVetoJet(eta2, phi2)) { continue; }

      SumRCone1.SetXYZT(0, 0, 0, 0);
      SumRCone2.SetXYZT(0, 0, 0, 0);
      depositERC1 = 0; depositERC2 = 0;
      depositETRC1 = 0; depositETRC2 = 0;
      num_rc1_pf = 0; num_rc2_pf = 0;
      total_weight1 = 0.; total_weight2 = 0.;
      acc = RandCone();   // zero all per-group / per-depth sums

      bool overlayed_ = false;

      for (unsigned int i = 0; i < pf_pt->size(); i++)
      {
         if (isCHS && !IsPFCHS(pf_type->at(i))) { continue; }

         // Cone membership from plain dEta/dPhi arithmetic on the stored PF
         // eta/phi. Equivalent to TLorentzVector::DeltaR() < RCone, without
         // rebuilding a four-vector (sinh/cos/sin) and recomputing eta/phi
         // (log/atan2) for every PF candidate and cone.
         double dEta1 = pf_eta->at(i) - eta1;
         double dPhi1 = TVector2::Phi_mpi_pi(pf_phi->at(i) - phi1);
         double dEta2 = pf_eta->at(i) - eta2;
         double dPhi2 = TVector2::Phi_mpi_pi(pf_phi->at(i) - phi2);

         bool inCone1 = (dEta1 * dEta1 + dPhi1 * dPhi1 < RCone2);
         bool inCone2 = (dEta2 * dEta2 + dPhi2 * dPhi2 < RCone2);

         if (!inCone1 && !inCone2) { continue; }

         clusterVectorE.SetPtEtaPhiE(pf_pt->at(i), pf_eta->at(i), pf_phi->at(i), pf_energy->at(i));

         double weight = isPuppi ? pf_puppiW->at(i) : 1.0;

         // group + per-depth ET of this candidate. The ntuple stores pf_hcalDepthK as an energy in GeV
         // (fraction x hcalEnergy), so the depth fraction of the candidate is pf_hcalDepthK / pf_energy.
         const int ptype = pf_type->at(i);
         const int grp = (ptype == 0 || ptype == 1) ? (int)gCH
                       : (ptype == 2) ? (int)gNH
                       : (ptype == 3) ? (int)gPH
                       : (ptype == 4) ? (int)gHFH
                       : (ptype == 5) ? (int)gHFE : (int)gOTH;
         const double etPF = pf_et->at(i);
         const double ePF  = pf_energy->at(i);
         double depET[kNDepth];
         double depFracSum = 0.;
         for (int k = 0; k < kNDepth; ++k) {
            double eDep = (depthVec[k] && i < depthVec[k]->size()) ? depthVec[k]->at(i) : 0.;
            double f = (ePF > 0.) ? eDep / ePF : 0.;
            depET[k] = etPF * f;
            depFracSum += f;
         }
         const bool isHadPF = (ptype == 0 || ptype == 1 || ptype == 2 || ptype == 4);
         const bool isCHSPF = IsPFCHS(ptype);

         bool tempInCone1 = false;
         bool tempInCone2 = false;

         if (inCone1)
         {
            SumRCone1 = SumRCone1 + clusterVectorE;
            tempInCone1 = true;
            depositERC1 += pf_energy->at(i) * weight;
            depositETRC1 += pf_et->at(i) * weight;
            acc.gP1[grp] += etPF * weight;  acc.gR1[grp] += etPF;
            for (int k = 0; k < kNDepth; ++k) { acc.dP1[k] += depET[k] * weight; acc.dR1[k] += depET[k]; }
            if (isCHSPF) { acc.gC1[grp] += etPF;  for (int k = 0; k < kNDepth; ++k) acc.dC1[k] += depET[k]; }
            if (isHadPF) { acc.depFrac1 += depFracSum; acc.nHad1++; }
            total_weight1 += weight;
            num_rc1_pf++;
         }

         if (inCone2)
         {
            SumRCone2 = SumRCone2 + clusterVectorE;
            tempInCone2 = true;
            depositERC2 += pf_energy->at(i) * weight;
            depositETRC2 += pf_et->at(i) * weight;
            acc.gP2[grp] += etPF * weight;  acc.gR2[grp] += etPF;
            for (int k = 0; k < kNDepth; ++k) { acc.dP2[k] += depET[k] * weight; acc.dR2[k] += depET[k]; }
            if (isCHSPF) { acc.gC2[grp] += etPF;  for (int k = 0; k < kNDepth; ++k) acc.dC2[k] += depET[k]; }
            if (isHadPF) { acc.depFrac2 += depFracSum; acc.nHad2++; }
            total_weight2 += weight;
            num_rc2_pf++;
         }

         // Same PF candidate landed in both cones - discard this cone pair
         // and retry with a fresh random draw.
         if (tempInCone1 && tempInCone2) { overlayed_ = true; break; }
      }

      if (overlayed_) { continue; }

      isPFInCone1 = (total_weight1 > kWeightSumCut);
      isPFInCone2 = (total_weight2 > kWeightSumCut);
   }

   double diff_ = -99999999;

   RandCone tmp;
   bool isRC = false;
   if ((NumOfSelct <= maxSelct) && isPFInCone1 && isPFInCone2)
   {
      isRC = true;

      diff_ = depositETRC1 - depositETRC2;
      JetCone1.SetPtEtaPhiE(depositETRC1, eta1, phi1, depositETRC1);
      JetCone2.SetPtEtaPhiE(depositETRC2, eta2, phi2, depositETRC2);
      tmp = acc;   // keep the per-group / per-depth sums of the accepted cone pair
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

