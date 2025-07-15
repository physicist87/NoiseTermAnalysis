#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TPaveStats.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <TKey.h>
#include <TStyle.h>
#include <TString.h>
#include <TSystem.h>
#include "./src/tdrStyle.cpp"
#include "./src/CMS_lumi.C"
using namespace std;

struct confint{
   double findLeft;
   double leftbin;
   double findRight;
   double rightbin;
   double Inteval_;
   double frac_;
   double CIrmsLeft;
   double CIrmsRight;
   double CIrms_Value;
   double RMS_Value;
   double convfact;
};
void CalNoiseTerm_SR_RMS2_v3()
{ 
   //void Calc(TString StudyName, TString sample);
   void Calc(TString StudyName, TString sample, TString hName);
   TString EtaBinsSt[20] = {"0", "0p261","0p522","0p783","1p044","1p305","1p566","1p740","1p930","2p043","2p172","2p322","2p500","2p650","2p853","2p964","3p139","3p489","3p839","5p191"};
   TString studyname = "";
   TString sample= "Data";
   vector<TString> v_Syst;
   v_Syst.push_back("Central");
//   v_Syst.push_back("Central_All");
/*   v_Syst.push_back("L1RC");
   v_Syst.push_back("L1RCNega");
   v_Syst.push_back("L1Nega");
   v_Syst.push_back("L1Cor");
   v_Syst.push_back("All");
   v_Syst.push_back("AllNega");
   v_Syst.push_back("AllRCNega");*/
   //v_Syst.push_back("PileUpUp");
   //v_Syst.push_back("PileUpDown");
   //v_Syst.push_back("NoJEC");
 //  v_Syst.push_back("NoPU");
   //v_Syst.push_back("JECUp");
   //v_Syst.push_back("JECDown");
   TString hName = "h_Noise_Diff";
   for (int i= 0; i < 20; ++i)
   {
      if(i != 19){

         for (int isys = 0; isys < v_Syst.size(); isys++){

            //TString studyname = Form("StudySC2018_v5/MC/Central/EtaBin_%s_%s",EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //TString studyname = Form("StudySC2018_v5/MuScale/Central/EtaBin_%s_%s",EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //TString studyname = Form("StudySC2018_v5/MuScale/L1RCNega/EtaBin_%s_%s",EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //TString studyname = Form("StudySC2018_v5/MC/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //TString studyname = Form("Fix30_2018_v1/MuScale/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //TString studyname = Form("StudySC2018_v5/MuScale/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //TString studyname = Form("StudySC2018_v6/MuScale/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //TString studyname = Form("StudySC2018_v7/MuScale/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //studyname = Form("StudySC2018_v7p1/MuScale/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //studyname = Form("TestPUMethod_v1/MuScale/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //studyname = Form("Cone05_2018_v1/MuScale/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            studyname = Form("Cone03_2018_v1/MuScale/%s/EtaBin_%s_%s",v_Syst[isys].Data(),EtaBinsSt[i].Data(),EtaBinsSt[i+1].Data());
            //TString sample= "SingleNeutrino_Flat2018";
            //TString sample= "Data_ZeroBias_Run2018Av2";
            sample= "Data";
            //TString sample= "Data_ZeroBias_Run2017Fv2";
            //TString sample= "SingleNeutrino_NoPU";
            //TString sample= "SingleNeutrino_Flat2017";
            //sample= "SingleNeutrino_Flat2017v2";
            hName = "h_Noise_Diff";
            Calc(studyname,sample, hName);
            hName = "h_Noise_Rho_Diff";
            Calc(studyname,sample, hName);

            sample= "Data_ZeroBias_Run2018Av2";
            hName = "h_Noise_Diff";
            Calc(studyname,sample, hName);
            hName = "h_Noise_Rho_Diff";
            Calc(studyname,sample, hName);

            sample= "Data_ZeroBias_Run2018Bv2";
            hName = "h_Noise_Diff";
            Calc(studyname,sample, hName);
            hName = "h_Noise_Rho_Diff";
            Calc(studyname,sample, hName);

            sample= "Data_ZeroBias_Run2018Cv1";
            hName = "h_Noise_Diff";
            Calc(studyname,sample, hName);
            hName = "h_Noise_Rho_Diff";
            Calc(studyname,sample, hName);

            sample= "Data_ZeroBias_Run2018Dv1";
            hName = "h_Noise_Diff";
            Calc(studyname,sample, hName);
            hName = "h_Noise_Rho_Diff";
            Calc(studyname,sample, hName);

         }
      }
      

   }

   for (int isys = 0; isys < v_Syst.size(); isys++){
       studyname = Form("StudySC2018_v7p1/MuScale/%s/AllEtaBin",v_Syst[isys].Data());
       studyname = Form("TestPUMethod_v1/MuScale/%s/AllEtaBin",v_Syst[isys].Data());
       sample= "Data";
       hName = "h_Noise_Diff";
       Calc(studyname,sample, hName);
       hName = "h_Noise_Rho_Diff";
       Calc(studyname,sample, hName);

       sample= "Data_ZeroBias_Run2018Av2";
       hName = "h_Noise_Diff";
       Calc(studyname,sample, hName);
       hName = "h_Noise_Rho_Diff";
       Calc(studyname,sample, hName);

       sample= "Data_ZeroBias_Run2018Bv2";
       hName = "h_Noise_Diff";
       Calc(studyname,sample, hName);
       hName = "h_Noise_Rho_Diff";
       Calc(studyname,sample, hName);

       sample= "Data_ZeroBias_Run2018Cv1";
       hName = "h_Noise_Diff";
       Calc(studyname,sample, hName);
       hName = "h_Noise_Rho_Diff";
       Calc(studyname,sample, hName);

       sample= "Data_ZeroBias_Run2018Dv1";
       hName = "h_Noise_Diff";
       Calc(studyname,sample, hName);
       hName = "h_Noise_Rho_Diff";
       Calc(studyname,sample, hName);
   }
}
void Calc(TString StudyName, TString sample, TString hName)
{
   void SaveHist(TH1D* htmp, double min_, double max_,TString xName, double rebin, TString sample, TString StudyName);
   void SaveHistFit(TH1D* htmp, double min_, double max_,TString xName, double rebin);
   double ConfidenceAndrea(TH1D* hist, double confLevel = 0.68);
   cout << "--- Draw---" << endl;
   TFile* ftmp = TFile::Open(Form("../../RootFiles/%s/%s.root",StudyName.Data(),sample.Data()),"READ");
   // h_Noise_Diff
   cout << "ftmp Name " << ftmp->GetName() << endl;
   TH1D* tmp;
   if (ftmp == NULL) cout << "No File!!" << endl;
   else {
      for (int i = 0; i < 9; ++i) 
      {
         //tmp = (TH1D*)ftmp->Get(Form("h_Noise_Diff_%d",i+1));
         tmp = (TH1D*)ftmp->Get(Form("%s_%d",hName.Data(),i+1));
         
         //if (tmp != NULL) {SaveHist(tmp, -20,20, "p_{T}(R.C.1) - p_{T}(R.C.2) [GeV]",10, sample,StudyName);
         //if (tmp != NULL) {SaveHist(tmp, -1,1, "p_{T}(R.C.1) - p_{T}(R.C.2) [GeV]",10, sample,StudyName);
         if (tmp != NULL) {SaveHist(tmp, -20,20, "p_{T}(R.C.1) - p_{T}(R.C.2) [GeV]",10, sample,StudyName);
            //ConfidenceAndrea(tmp);
         }
         else {cout << "NoHIST !!"<< endl;} 
      }
   }
   ftmp->Close();
}
void SaveHist(TH1D* htmp, double min_, double max_,TString xName, double rebin,TString sample, TString StudyName)
{

   double ConfidenceAndrea(TH1D* hist, double confLevel = 0.68);
   //double CISigRC(TH1D* hist, double confLevel = 0.68);
   struct confint CISigRC(TH1D* hist, double confLevel = 0.68);
   if (htmp->GetEntries() == 0) {return;}
   TCanvas* c1 = new TCanvas(Form("c1_%s",htmp->GetName()),"canvas",800,800);
   c1->cd();
   double RMS_H = htmp->GetRMS();
   int W = 1200;
   int H = 1200;
   int H_ref = 1200;
   int W_ref = 1200;

   float T = 0.08*H_ref;
   float B = 0.12*H_ref;
   float L = 0.12*W_ref;
   float R = 0.04*W_ref;
   c1->SetFillColor(0);
   c1->SetLeftMargin(L/W+0.02);
   c1->SetRightMargin(R/W);
   c1->SetTopMargin(T/H);
   c1->SetBottomMargin(B/H);
   c1->SetFillColor(0);
   c1->SetGridy();
   //htmp->Scale(1/htmp->Integral());
   //htmp->SetTitle(Form("#Delta p_{T} %s",StudyName.Data()));
   htmp->SetTitle("");
   htmp->SetMinimum(0);
   htmp->SetLineColor(1);
   htmp->Draw("HistE");

   vector<double> v_ConIntV;
   vector<confint> v_confint;
   v_ConIntV.push_back(0.68);
   v_ConIntV.push_back(0.87);
   v_ConIntV.push_back(0.95);
   v_ConIntV.push_back(0.98);
   v_ConIntV.push_back(0.99);
   for (unsigned int ic = 0; ic < v_ConIntV.size(); ++ic)
   {
      v_confint.push_back( CISigRC(htmp, v_ConIntV[ic]) );
   }
   int MeanBin = htmp->FindBin(htmp->GetMean());
   double Total_Integral_ = htmp->Integral();
   double Mean_ = htmp->GetMean();
   double Median_ = 0.0;
   double p[3] = { 0.25, 0.50, 0.75};
   double q[3];
   htmp->GetQuantiles(3,q,p);
   std::cout << "first quartile (25th percentile) = " << q[0] << std::endl;
   std::cout << "median (50th percentile) = " << q[1] << std::endl;
   std::cout << "third quartile (75th percentile) = " << q[2] << std::endl;
//   double xconfrange = ConfidenceAndrea(htmp)/(2*0.9945);
//   cout << "xconfrange : " << xconfrange  << endl;
    
   TLine* xLeft[5];
   TLine* xRight[5];
   htmp->Rebin(rebin);
   htmp->Draw("HistE");
   TLine* xmean = new TLine(htmp->GetMean(), 0, htmp->GetMean(), htmp->GetMaximum() );
   for (unsigned int ic = 0; ic < v_ConIntV.size(); ++ic){
      xLeft[ic] = new TLine(v_confint[ic].CIrmsLeft, 0, v_confint[ic].CIrmsLeft, htmp->GetMaximum()*0.6 );
      xRight[ic] = new TLine(v_confint[ic].CIrmsRight, 0, v_confint[ic].CIrmsRight, htmp->GetMaximum()*0.6 );
      xLeft[ic]->SetLineWidth(2);
      xLeft[ic]->SetLineColor(4+ic);
      xLeft[ic]->SetLineStyle(2);
      xRight[ic]->SetLineWidth(2);
      xRight[ic]->SetLineColor(4+ic);
      xRight[ic]->SetLineStyle(2);

   }
   xmean->SetLineWidth(2);
   xmean->SetLineColor(2);
   xmean->SetLineStyle(2);

 
   htmp->GetXaxis()->SetTitle(xName.Data());
   if( (min_ != -1) && (max_ != -1) ){
      htmp->GetXaxis()->SetRangeUser(min_,max_);
   }

   RMS_H = htmp->GetRMS();

   TLegend *leg = new TLegend(0.15,0.5,0.44,0.8);
   //leg->AddEntry((TObject*)0,Form("N^{PU} %.3f",CIrms_Value/(2*sqrt(2))), "");
   //leg->AddEntry((TObject*)0,Form("#sigma_{RC} %.3f",CIrms_Value), "");
   //leg->AddEntry((TObject*)0,Form("N^{PU} %.3f",(RMS_H)/sqrt(2)), "");
   leg->AddEntry(xLeft[0],Form("N^{PU}_{#sigma_{RC} 68%} %.3f",( v_confint[0].CIrms_Value/(v_confint[0].convfact))/(2*sqrt(2))), "l");
   leg->AddEntry(xLeft[1],Form("N^{PU}_{#sigma_{RC} 87%} %.3f",( v_confint[1].CIrms_Value/(v_confint[1].convfact))/(2*sqrt(2))), "l");
   leg->AddEntry(xLeft[2],Form("N^{PU}_{#sigma_{RC} 95%} %.3f",( v_confint[2].CIrms_Value/(v_confint[2].convfact))/(2*sqrt(2))), "l");
   leg->AddEntry(xLeft[3],Form("N^{PU}_{#sigma_{RC} 98%} %.3f",( v_confint[3].CIrms_Value/(v_confint[3].convfact))/(2*sqrt(2))), "l");
   leg->AddEntry(xLeft[4],Form("N^{PU}_{#sigma_{RC} 99%} %.3f",( v_confint[4].CIrms_Value/(v_confint[4].convfact))/(2*sqrt(2))), "l");
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->SetTextSize(0.03);
   //leg->SetTextSize(0.02);
   leg->SetTextFont(42);

   leg->Draw();

   TLegend *leg_rms = new TLegend(0.65,0.5,0.95,0.8);
   //leg_rms->AddEntry((TObject*)0,Form("N^{PU} %.3f",CIrms_Value/(2*sqrt(2))), "");
   //leg_rms->AddEntry((TObject*)0,Form("#sigma_{RC} %.3f",CIrms_Value), "");
   leg_rms->AddEntry((TObject*)0,Form("N^{PU}_{All} %.3f",(RMS_H)/sqrt(2)), "");
   leg_rms->AddEntry(xLeft[0],Form("N^{PU}_{RMS 68%} %.3f",( v_confint[0].RMS_Value)/(sqrt(2))), "l");
   leg_rms->AddEntry(xLeft[1],Form("N^{PU}_{RMS 87%} %.3f",( v_confint[1].RMS_Value)/(sqrt(2))), "l");
   leg_rms->AddEntry(xLeft[2],Form("N^{PU}_{RMS 95%} %.3f",( v_confint[2].RMS_Value)/(sqrt(2))), "l");
   leg_rms->AddEntry(xLeft[3],Form("N^{PU}_{RMS 98%} %.3f",( v_confint[3].RMS_Value)/(sqrt(2))), "l");
   leg_rms->AddEntry(xLeft[4],Form("N^{PU}_{RMS 99%} %.3f",( v_confint[4].RMS_Value)/(sqrt(2))), "l");
   leg_rms->SetLineColor(1);
   leg_rms->SetLineStyle(1);
   leg_rms->SetLineWidth(1);
   leg_rms->SetFillColor(0);
   leg_rms->SetFillStyle(0);
   leg_rms->SetTextSize(0.03);
   //leg_rms->SetTextSize(0.02);
   leg_rms->SetTextFont(42);

   leg_rms->Draw();

   for (unsigned int ic = 0; ic < v_ConIntV.size(); ++ic){
      xLeft[ic]->Draw("same");
      xRight[ic]->Draw("same");
   }
   xmean->Draw("same");
 
   CMS_lumi(c1, 4, 11);
   c1->Update();
//   htmp->GetXaxis()->SetTitleSize(0.6);
   gSystem->mkdir(Form("./plots_range20"));
   gSystem->mkdir(Form("./plots_range20/%s",StudyName.Data()));
   gSystem->Exec(Form("mkdir -p ./plots_range20/%s/%s",StudyName.Data(),sample.Data()));
   c1->SaveAs(Form("./plots_range20/%s/%s/%s.pdf",StudyName.Data(),sample.Data(),htmp->GetName()));
   htmp->SetMinimum(0.00001);
   c1->SetLogy();
   c1->SaveAs(Form("./plots_range20/%s/%s/%sLog.pdf",StudyName.Data(),sample.Data(),htmp->GetName()));
   FILE *datfile;
   string datName = Form("./plots_range20/%s/%s/Noise_%s.txt",StudyName.Data(),sample.Data(),htmp->GetName());
   datfile = fopen(datName.c_str(),"w");
   fprintf(datfile,"NoiseTerm:  %1.4f \n"  , v_confint[0].CIrms_Value/(2*sqrt(2)));
   fprintf(datfile,"NoiseTerm_SR1:  %1.4f \n"  , (v_confint[0].CIrms_Value/(v_confint[0].convfact))/(2*sqrt(2)));
   fprintf(datfile,"NoiseTerm_SR2:  %1.4f \n"  , (v_confint[1].CIrms_Value/(v_confint[1].convfact))/(2*sqrt(2)));
   fprintf(datfile,"NoiseTerm_SR3:  %1.4f \n"  , (v_confint[2].CIrms_Value/(v_confint[2].convfact))/(2*sqrt(2)));
   fprintf(datfile,"NoiseTerm_SR4:  %1.4f \n"  , (v_confint[3].CIrms_Value/(v_confint[3].convfact))/(2*sqrt(2)));
   fprintf(datfile,"NoiseTerm_SR5:  %1.4f \n"  , (v_confint[4].CIrms_Value/(v_confint[4].convfact))/(2*sqrt(2)));

   fprintf(datfile,"NoiseTermRMS:  %1.4f \n"  , (RMS_H)/(sqrt(2)));
   fprintf(datfile,"NoiseTermRMS1:  %1.4f \n"  , (v_confint[0].RMS_Value)/(sqrt(2)));
   fprintf(datfile,"NoiseTermRMS2:  %1.4f \n"  , (v_confint[1].RMS_Value)/(sqrt(2)));
   fprintf(datfile,"NoiseTermRMS3:  %1.4f \n"  , (v_confint[2].RMS_Value)/(sqrt(2)));
   fprintf(datfile,"NoiseTermRMS4:  %1.4f \n"  , (v_confint[3].RMS_Value)/(sqrt(2)));
   fprintf(datfile,"NoiseTermRMS5:  %1.4f \n"  , (v_confint[4].RMS_Value)/(sqrt(2)));


   fprintf(datfile,"SigmaRC:  %1.4f \n"  , v_confint[0].CIrms_Value);
   fprintf(datfile,"StdDev:  %1.4f \n"  , 2*RMS_H);
   fprintf(datfile,"Mean:  %1.6f \n"  , Mean_);
   fprintf(datfile,"Median:  %1.6f \n"  , q[1]);
   fclose(datfile);
}

double ConfidenceAndrea(TH1D* hist, double confLevel = 0.68){
    cout << "confLevel : "<< confLevel << endl;
    int ix = hist->GetXaxis()->FindBin(hist->GetMean());
    int ixlow = ix;
    int ixhigh = ix;
    int nb = hist->GetNbinsX();
    double ntot = hist->Integral();
    double nsum = hist->GetBinContent(ix);
    double width = hist->GetBinWidth(ix);
    if (ntot==0) return 0;
    int nlow =0;
    int nhigh =0;
    //nlow = (ixlow>0) ? hist->GetBinContent(ixlow-1) : 0; // min_value = val if val < min_value else min_value
    //nhigh =  (ixhigh<nb) ? hist->GetBinContent(ixlow-1) :  0;
    //cout << "nlow : " << nlow << " nhigh : " << nhigh  << endl;
    /*if (ixlow>0) { nlow = hist->GetBinContent(ixlow-1);}
    else {nlow =  0; }
    if (ixhigh<nb) { nhigh = hist->GetBinContent(ixhigh-1); }
    else {nhigh = 0; }
    cout << "nlow : " << nlow << " nhigh : " << nhigh  << " nb : "<< nb << endl;*/
    while (nsum < confLevel * ntot){
 
        if (ixlow>0) { nlow = hist->GetBinContent(ixlow-1);}
        else {nlow =  0; }
        if (ixhigh<nb) { nhigh = hist->GetBinContent(ixhigh-1); }
        else {nhigh = 0; }
        //nlow = (ixlow>0) ? hist->GetBinContent(ixlow-1) : 0; // min_value = val if val < min_value else min_value
        //nhigh =  (ixhigh<nb) ? hist->GetBinContent(ixlow-1) :  0;
//        cout << "nlow : " << nlow << " nhigh : " << nhigh  << endl;
        //cout << "max(nlow,nhigh) : " << max(nlow,nhigh) << endl;
        if (nsum+max(nlow,nhigh) < confLevel * ntot){
            if (nlow>=nhigh && ixlow>0){
                nsum += nlow;
                ixlow -=1;
                width += hist->GetBinWidth(ixlow);
            }
            else if (ixhigh<nb){
                nsum += nhigh;
                ixhigh+=1;
                width += hist->GetBinWidth(ixhigh);
            }
            else { cout << " ERROR !!" << endl;}
        }
        else{
            cout << "? " << endl;
            if (nlow>nhigh){width += hist->GetBinWidth(ixlow-1) * (confLevel * ntot - nsum) / nlow;}
            else{width += hist->GetBinWidth(ixhigh+1) * (confLevel * ntot - nsum) / nhigh;}
            nsum = ntot;
       }
    }
//    cout << "ix : " << ix  << "ixlow " << ixlow << " ixhigh : " << ixhigh << endl;
    //return width;
    return nsum;
}
struct confint CISigRC(TH1D* hist, double confLevel = 0.68){
   cout << "CISigRC !!" << endl;
   double FindConvFact(double confLevel= 0.68);
   confint conf_tmp; 
   double findLeft = 0;
   int leftbin = 0;
   double findRight = 0;
   int rightbin = 0;
   int MeanBin = hist->FindBin(hist->GetMean());
   double Total_Integral_ = hist->Integral();
   for (int ibin = MeanBin; ibin >0; --ibin){
      findLeft += hist->GetBinContent(ibin);
      leftbin=ibin;
      if (findLeft/hist->Integral()> (confLevel/2) ) {break;}
   }
   for (int ibin = MeanBin; ibin < hist->GetNbinsX(); ++ibin){
				      findRight += hist->GetBinContent(ibin);
      rightbin=ibin;
      if (findRight/hist->Integral()> (confLevel/2) ) {break;}
   }
   double Inteval_ = hist->Integral(leftbin,rightbin);
   double frac_ = Inteval_/Total_Integral_;
   //if (frac_ > 0.68) {cout << "wrong !! " << endl;}
   if (frac_ > confLevel) {cout << "wrong !! " << endl;}
   double CIrmsLeft = hist->GetXaxis()->GetBinLowEdge(leftbin);
   double CIrmsRight = hist->GetXaxis()->GetBinUpEdge(rightbin);
   double CIrms_Value = abs(CIrmsLeft) + abs(CIrmsRight);
   TH1D* h_clone = (TH1D*)hist->Clone(Form("%s_clone",hist->GetName()));
   h_clone->GetXaxis()->SetRangeUser(CIrmsLeft,CIrmsRight);
  
   conf_tmp.findLeft = findLeft;
   conf_tmp.leftbin = leftbin;
   conf_tmp.findRight = findRight;
   conf_tmp.rightbin = rightbin;
   conf_tmp.Inteval_ = Inteval_;
   conf_tmp.frac_ = frac_;
   conf_tmp.CIrmsLeft = CIrmsLeft;
   conf_tmp.CIrmsRight = CIrmsRight;
   conf_tmp.CIrms_Value = CIrms_Value;
   conf_tmp.RMS_Value = h_clone->GetRMS();
   conf_tmp.convfact = FindConvFact(confLevel);
   
   h_clone->GetXaxis()->UnZoom();
   return conf_tmp;
}

double FindConvFact(double confLevel= 0.68)
{
   double conf = 0.;
   if (confLevel == 0.68) {conf = 0.9945;}
   if (confLevel == 0.87) {conf = 1.514;}
   if (confLevel == 0.95) {conf = 1.960;}
   if (confLevel == 0.98) {conf = 2.326;}
   if (confLevel == 0.99) {conf = 2.576;}
   return conf;
}

