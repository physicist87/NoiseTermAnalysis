#ifndef _JetVeto_

#define _JetVeto_
#include <set>
#include <string>
#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <TH2D.h>
#include <TFile.h>
using namespace std;

class JetVeto 
{
    public:
      //declare functions
      JetVeto(TString fileName="", TString histName="");
      virtual ~JetVeto();

      //user define functions

      bool VetoCheck(double eta, double phi);
 
   private:
      //put variables that you want
      TH2D* h_;
      TFile* f_;
};
#endif

#ifdef JetVeto_cxx

JetVeto::JetVeto(TString fileName, TString histName)
{
   /// Read Text Configure file ///
   
   f_ = TFile::Open(Form("./jetveto/%s.root",fileName.Data()),"READ");
   if (f_==NULL) {cout << "NO FILE !!!!!" << endl;} 
   h_ = (TH2D*)f_->Get(histName.Data());
   if (f_==NULL) {cout << "NO HIST !!!!!" << endl;} 
}

JetVeto::~JetVeto()
{
}
#endif
