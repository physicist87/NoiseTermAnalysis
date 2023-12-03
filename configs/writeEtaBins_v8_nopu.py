import os 
import sys

#def Make_Config(StudyName, SubName) :
def Make_Config(StudyName, RunOpt, PUOpt, JECOpt, Sample) :
    ### making config File ###
    #f1 = open( "./NoPUCor_EtaBin_%s_%s.config"%(LowerBinSt,UpperBinSt), 'w')
    #f1 = open( "./%s/%s/Data.config"%(StudyName, SubName ), 'w')
    f1 = open( "./%s/%s/%s/%s.config"%(StudyName, RunOpt, PUOpt, Sample), 'w')
    #f1 = open( "./OffCor_JetVetoV1_EtaBin_%s_%s.config"%(LowerBinSt,UpperBinSt), 'w')
    #f1.write('PileUpMCFileName : "Data_Mu_v1.root" \n')
    #f1.write('PileUpMCFileName : "PileupMC_v2.root" \n')
    #f1.write('PileUpMCFileName : "DataMu_PreScaled.root" \n')
    #f1.write('PileUpMCFileName : "Data_MuPre_ZBRun2017.root" \n')

    #f1.write('PileUpMCFileName : "Data_Mu_v1.root" \n')
    MCPUPileUp = "PileupMC_v3.root"
    if Sample.count('PreMix') : MCPUPileUp = "PileupMC_PreMixv3.root"
    f1.write('PileUpMCFileName : "%s" \n'%(MCPUPileUp))
    #f1.write('PileUpDATAFileName : "pileup_DT_UL18.root"\n')
    DataPU = "DATAPileUp.root"
    if PUOpt == "Central" :

        if RunOpt.count('MC')  : DataPU = "DATAPileUp.root"
        if RunOpt.count('RunA')  : DataPU = "v1_RunA.root" 
        if RunOpt.count('RunB')  : DataPU = "v1_RunB.root" 
        if RunOpt.count('RunC')  : DataPU = "v1_RunC.root" 
        if RunOpt.count('RunD')  : DataPU = "v1_RunD.root"
    elif PUOpt =="PileUpUp" :  

        if RunOpt.count('MC')  : DataPU = "DATAPileUp_Up.root"
        if RunOpt.count('RunA')  : DataPU = "v1_RunA_Up.root"
        if RunOpt.count('RunB')  : DataPU = "v1_RunB_Up.root"
        if RunOpt.count('RunC')  : DataPU = "v1_RunC_Up.root" 
        if RunOpt.count('RunD')  : DataPU = "v1_RunD_Up.root"

    elif PUOpt == "PileUpDown":  

        if RunOpt.count('MC')  : DataPU = "DATAPileUp_Down.root"
        if RunOpt.count('RunA')  : DataPU = "v1_RunA_Down.root"
        if RunOpt.count('RunB')  : DataPU = "v1_RunB_Down.root"
        if RunOpt.count('RunC')  : DataPU = "v1_RunC_Down.root" 
        if RunOpt.count('RunD')  : DataPU = "v1_RunD_Down.root"

    else :
        print "Wrong!" 
        if RunOpt.count('MC')  : DataPU = "DATAPileUp.root"
        if RunOpt.count('RunA')  : DataPU = "v1_RunA.root" 
        if RunOpt.count('RunB')  : DataPU = "v1_RunB.root" 
        if RunOpt.count('RunC')  : DataPU = "v1_RunC.root" 
        if RunOpt.count('RunD')  : DataPU = "v1_RunD.root"


    f1.write('PileUpDATAFileName : "%s"\n'%(DataPU))

    if PUOpt != "NoPU": 
        f1.write('DoPUCor : "true"\n')
    else : 
        f1.write('DoPUCor : "false"\n')
    
    f1.write('trigger : {"HLT_ZeroBias_part","HLT_ZeroBias_v"}\n')
    f1.write('DoJetVeto : "true"\n')
    f1.write('JetVetoFName : "hotjets-UL18" \n')
    f1.write('JetVetoHName : "h2hot_ul18_plus_hem1516_and_hbp2m1"\n')
    #f1.write('EtaBins : {"0","0p5","0p8","1p1","1p3","1p7","1p9","2p1","2p3","2p5","2p8","3p0","3p2","4p7"}\n') 
    f1.write('EtaBins : {"0", "0p261", "0p522", "0p783", "1p044", "1p305", "1p566", "1p740", "1p930", "2p043", "2p172", "2p322", "2p500", "2p650", "2p853", "2p964", "3p139", "3p489", "3p839", "5p191"}\n')
    if JECOpt != "NoJEC": 
        #f1.write('JECApply : "true"\n')
        f1.write('JECApply : "true"\n')
        #f1.write('JECType : "%s"\n'%(SysOpt))
        f1.write('JECType : "%s"\n'%(JECOpt))
        #f1.write('JECApply : "false"\n')
    f1.close() 

def main():
    Samples = ["SingleNeutrino_Flat2018", "SingleNeutrino_Flat2018_HEM"]
    Samples = ["SingleNeutrino_Flat2018", "SingleNeutrino_PreMix2018"]
    RunPeriod = ["PURunA","PURunB","PURunC","PURunD"]
    RunPeriod = ["PURunB","PURunC","PURunD","PURunE","PURunF","MC"]
    RunPeriod = ["MC","PURunA","PURunB","PURunC","PURunD"]
    #for 
    #mkdircmd = "mkdir -p %s/%s"%(StudyName, SubStudyName)
    #os.system(mkdircmd)
    #Make_Config(StudyName, SubStudyName)

    StudyName = "NonAppliedJEC_v4"
    StudyName = "StudyRun2018_v7"
    StudyName = "StudyRun2018_v6"
    StudyName = "StudySC2018_v2"
    StudyName = "StudySC2018_v5"
    JECOpt = ["L1RC","L1RCNega","L1Nega","L1Cor","All","AllNega","AllRCNega"]
    JECOpt = ["Central"]
    PUSyst = ["PileUpUp","PileUpDown"]
    PUSyst = ["NoPU"]
    jetopt = "Central"
    jetopt = "NoJEC"
    for isample in Samples:
        for irun in RunPeriod:
            for ipusys in PUSyst:
                mkdircmd = "mkdir -p %s/%s/%s"%(StudyName, irun, ipusys)
                os.system(mkdircmd)
                Make_Config(StudyName, irun, ipusys, jetopt, isample)
                pass

    pass
if __name__ == '__main__':
    main()
