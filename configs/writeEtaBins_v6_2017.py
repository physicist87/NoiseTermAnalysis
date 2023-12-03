import os 
import sys

#def Make_Config(StudyName, SubName) :
def Make_Config(StudyName, SubName, SystOpt, RunOpt) :
    ### making config File ###
    #f1 = open( "./NoPUCor_EtaBin_%s_%s.config"%(LowerBinSt,UpperBinSt), 'w')
    #f1 = open( "./%s/%s/Data.config"%(StudyName, SubName ), 'w')
    f1 = open( "./%s/%s/%s/Data_%s.config"%(StudyName, SubName, SystOpt, RunOpt), 'w')
    #f1 = open( "./OffCor_JetVetoV1_EtaBin_%s_%s.config"%(LowerBinSt,UpperBinSt), 'w')
    #f1.write('PileUpMCFileName : "Data_Mu_v1.root" \n')
    #f1.write('PileUpMCFileName : "PileupMC_v2.root" \n')
    #f1.write('PileUpMCFileName : "DataMu_PreScaled.root" \n')
    #f1.write('PileUpMCFileName : "Data_MuPre_ZBRun2017.root" \n')
    #f1.write('PileUpDATAFileName : "pileup_DT_UL18.root"\n')
    #f1.write('PileUpDATAFileName : "DATAPileUp.root"\n')
#    f1.write('PileUpDATAFileName : "DataPUUL17_Total.root"\n')
    DataPU = "DATAPileUp.root"
    MCPUPileUp = "PileupMC_2017.root"
    SysOpt = "Central"
    if SystOpt == "JECUp" : 
        SysOpt = "Up"
    elif SystOpt == "JECDown" : 
        SysOpt = "Down"
    else : SysOpt =  SystOpt
    #else : print "WRONG!!!!"
        
    if RunOpt.count('RunB')  : 
        DataPU = "For2017_v2/DataPUUL17_RunB.root"
        MCPUPileUp = "For2017_v2/Data_MuPre_ZBRun2017Bv2.root"
    elif RunOpt.count('RunC')  : 
        DataPU = "For2017_v2/DataPUUL17_RunC.root"
        MCPUPileUp = "For2017_v2/Data_MuPre_ZBRun2017Cv2.root"
    elif RunOpt.count('RunD')  : 
        DataPU = "For2017_v2/DataPUUL17_RunD.root"
        MCPUPileUp = "For2017_v2/Data_MuPre_ZBRun2017Dv2.root"
    elif RunOpt.count('RunE')  : 
        DataPU = "For2017_v2/DataPUUL17_RunE.root"
        MCPUPileUp = "For2017_v2/Data_MuPre_ZBRun2017Ev2.root"
    elif RunOpt.count('RunF')  : 
        DataPU = "For2017_v2/DataPUUL17_RunF.root"
        MCPUPileUp = "For2017_v2/Data_MuPre_ZBRun2017Fv2.root"
    else : 
        DataPU = "For2017_v2/DataPUUL17_Totalv2.root"
        MCPUPileUp = "For2017_v2/PileupMC_2017.root"
 
    f1.write('PileUpMCFileName : "%s" \n'%(MCPUPileUp))
    f1.write('PileUpDATAFileName : "%s"\n'%(DataPU))
    if SubName == "NoMuScale":
        f1.write('DoPrescale : "false"\n')
        f1.write('DoPUCor : "false"\n')
    else : 
        f1.write('DoPrescale : "true"\n')
        f1.write('DoPUCor : "true"\n')
    f1.write('trigger : {"HLT_ZeroBias_part","HLT_ZeroBias_v"}\n')
    f1.write('DoJetVeto : "true"\n')
    f1.write('JetVetoFName : "hotjets-UL17_v2" \n')
    f1.write('JetVetoHName : "h2hot_ul17_plus_hep17_plus_hbpw89"\n')
    #f1.write('EtaBins : {"0","0p5","0p8","1p1","1p3","1p7","1p9","2p1","2p3","2p5","2p8","3p0","3p2","4p7"}\n') 
    f1.write('EtaBins : {"0", "0p261", "0p522", "0p783", "1p044", "1p305", "1p566", "1p740", "1p930", "2p043", "2p172", "2p322", "2p500", "2p650", "2p853", "2p964", "3p139", "3p489", "3p839", "5p191"}\n') 
    #f1.write('JECApply : "true"\n')
    f1.write('JECApply : "true"\n')
    f1.write('JECType : "%s"\n'%(SystOpt))
    #f1.write('JECApply : "false"\n')
    f1.close() 

def main():
    StudyName = "AppliedJEC_v1"
    StudyName = "NonAppliedJEC_v3"
    StudyName = "AppliedJEC_v4"
    StudyName = "NonAppliedJEC_v4"
    StudyName = "StudyRun2017_v3"
    StudyName = "StudyRun2017_v4_NoJEC"
    StudyName = "StudyRun2017_v5"
    StudyName = "StudyRun2017_v5_NoJEC"
    StudyName = "StudyRun2017_v5"
    StudyName = "StudyRun2017_v5p1"
    StudyName = "StudyRun2017_v5p2"
    StudyName = "StudySC2017_v2"
    StudyName = "StudySC2017_v5"


    SubStudyName = "NoMuScale"
    SubStudyName = "NoMuScale"
    SubStudyName = "MuScale"

    Samples = ["SingleNeutrino_Flat2017"]
    Samples = ["Data"]
    RunPeriod = ["PURunA","PURunB","PURunC","PURunD"]
    RunPeriod = ["PURunB","PURunC","PURunD","PURunE","PURunF"]
    #for 
    SystOpt = "PileUpUp"
    SystOpt = "PileUpDown"
    SystOpt = "JECDown"
    SystOpt = "JECUp"
    SystOpt = "JECUp"
    SystOpt = "JECDown"
    SystOpt = "L1Cor"
    SystOpt = "L1RC"
    SystOpt = "All"
    SystOpt = "Central"
    SystOpt = "JECUp"
    SystOpt = "JECDown"
    mkdircmd = "mkdir -p %s/%s/%s"%(StudyName, SubStudyName, SystOpt)
    os.system(mkdircmd)
    #Make_Config(StudyName, SubStudyName)
    for ipu in RunPeriod:
        Make_Config(StudyName, SubStudyName, SystOpt, ipu)
        pass
    pass
if __name__ == '__main__':
    main()
