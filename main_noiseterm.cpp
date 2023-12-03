////////////////////////////////////////////////
//                                            //
//                                            //
//  Author: Seungkyu Ha, seungkyu.ha@cern.ch  //
//                                            //
//                                            //
////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include <TROOT.h>
#include <TUnixSystem.h>
#include <TChain.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>

#include "analysis/NoiseTree.h"
#include "NoiseTerm.hpp"

using namespace std;

TROOT root ("Plots", "Program for CMS Analysis");

//argc: # of arguments, argv:array for arguments
int main(int argc, char **argv)
{
   printf("The number of options is: %i\n",argc-1);
  
   int ReturnIndex(TString tmp); 
   if(argc<2)
   {
      printf("At least, you have to set 1, 2\n");
      printf("1. Input filelist\n");
      printf("2. Output file\n");
      printf("3. Config file\n");
      return 1;
   }

   for (int iopt=0; iopt<argc; iopt++)
   {
      printf("Option %i = %s\n",iopt,argv[iopt]);
   }

   ///read input options

   //cout << "argc: " << argc << endl;

   char *flist = argv[1];
   printf("Input filelist = %s\n",flist);

   char *studyname = argv[2];
   printf("Study name = %s\n",studyname);

   char *outname = argv[3];
   printf("Output file name = %s\n",outname);

   char *confname = argv[4];
   printf("Config file name = %s\n",confname);

   //merge files
   FILE *filelist;
   char filename[1000];
   string filelistDir, filelistName, filelistPath;

   filelistDir = "./input/";
 
   cout << endl;
   filelistName = argv[1];
   filelistPath = filelistDir + filelistName;
   filelist = fopen(filelistPath.c_str(),"r");
  
   std::vector<double> entries_pertree;

   entries_pertree.clear();

   while(filelist==NULL)
   {
      cout << "File not found, please try again." << endl;
      cout << "Filelist you want to use: " << filelistDir;
      cin >> filelistName;      
      filelistPath = filelistDir + filelistName;
      filelist=fopen(filelistPath.c_str(),"r");
   }

   TChain *ch = new TChain("pf/T");

   cout << endl;
   cout << "start merging file(s)" << endl;
   
   while (fscanf(filelist, "%s", filename) != EOF)
   {
      cout << "adding: " << filename << endl;
      ch->Add(filename, 0);
      entries_pertree.push_back(ch->GetEntries());
   }
   cout << "Total number of events after merging root files: " << ch->GetEntries() << endl;
   string ConfName =confname;
   //ReturnIndex(filelistName);
   //NoiseTree *noise = new NoiseTree(ch,1);

   NoiseTerm *noise = new NoiseTerm(ch,ConfName);
  
   // Setup cuts. 
   noise->SetOutputFileName(studyname,outname);


   // Run event loop
   noise->Start();
   noise->Loop();
   noise->End();
   
   cout << "End processing..." << endl << endl; 
    
   return 0;
}

int ReturnIndex(TString tmp){
   cout << " " << endl;
   char tb = '_';
   cout << "tmp1 " << tmp << endl;
   tmp.ReplaceAll(".list","");
   tmp.Remove(0,tmp.Last(tb)+1);
   cout << "Check tmp : " << tmp << endl;
   return tmp.Atoi();
   //return 1;
} 
