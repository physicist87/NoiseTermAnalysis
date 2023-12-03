import os 
import sys

def Make_Config(StudyName, SubName) :
    ### making config File ###
    #f1 = open( "./NoPUCor_EtaBin_%s_%s.config"%(LowerBinSt,UpperBinSt), 'w')
    f1 = open( "./%s/%s/Data.config"%(StudyName, SubName ), 'w')
    #f1 = open( "./OffCor_JetVetoV1_EtaBin_%s_%s.config"%(LowerBinSt,UpperBinSt), 'w')
    #f1.write('PileUpMCFileName : "Data_Mu_v1.root" \n')
    #f1.write('PileUpMCFileName : "PileupMC_v2.root" \n')
    #f1.write('PileUpMCFileName : "DataMu_PreScaled.root" \n')
    #f1.write('PileUpMCFileName : "Data_MuPre_ZBRun2017.root" \n')
    f1.write('PileUpMCFileName : "Data_MuPre_ZBRun2017_v2.root" \n')
    #f1.write('PileUpDATAFileName : "pileup_DT_UL18.root"\n')
    #f1.write('PileUpDATAFileName : "DATAPileUp.root"\n')
    f1.write('PileUpDATAFileName : "DataPUUL17_Total.root"\n')
    f1.write('trigger : {"HLT_ZeroBias_part","HLT_ZeroBias_v"}\n')
    if SubName == "NoMuScale":
        f1.write('DoPrescale : "false"\n')
        f1.write('DoPUCor : "false"\n')
    else : 
        f1.write('DoPrescale : "true"\n')
        f1.write('DoPUCor : "true"\n')
    f1.write('DoJetVeto : "true"\n')
    f1.write('JetVetoFName : "hotjets-UL17_v2" \n')
    f1.write('JetVetoHName : "h2hot_ul17_plus_hep17_plus_hbpw89"\n')
    #f1.write('EtaBins : {"0","0p5","0p8","1p1","1p3","1p7","1p9","2p1","2p3","2p5","2p8","3p0","3p2","4p7"}\n') 
    f1.write('EtaBins : {"0", "0p261", "0p522", "0p783", "1p044", "1p305", "1p566", "1p740", "1p930", "2p043", "2p172", "2p322", "2p500", "2p650", "2p853", "2p964", "3p139", "3p489", "3p839", "5p191"}\n') 
    #f1.write('JECApply : "true"\n')
    if SubName.count("NoJEC") > 0: f1.write('JECApply : "false"\n')
    else : f1.write('JECApply : "true"\n')
    f1.close() 

def main():
    StudyName = "AppliedJEC_v1"
    StudyName = "NonAppliedJEC_v3"
    StudyName = "AppliedJEC_v4"
    StudyName = "NonAppliedJEC_v4"
    StudyName = "StudyRun2017_v2"
    StudyName = "StudyRun2017_v4"
    StudyName = "StudyRun2017_v5"
    StudyName = "StudyRun2017_v6"
    StudyName = "StudyRun2017_v7"
    SubStudyName = "MuScale"
    SubStudyName = "NoMuScale"
    SubStudyName = "MuScale"
    SubStudyName = "NoMuScale"
    SubStudyName = "MuScale"
    SubStudyName = "NoJEC"
    SubStudyName = "MuScale"
    #for 
    mkdircmd = "mkdir -p %s/%s"%(StudyName, SubStudyName)
    os.system(mkdircmd)
    Make_Config(StudyName, SubStudyName) 
    pass
if __name__ == '__main__':
    main()
