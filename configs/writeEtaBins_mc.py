import os 
import sys

def Make_Config(StudyName, RunOpt, Sample) :
    ### making config File ###
    #f1 = open( "./%s/EtaBin_%s_%s_%s_%s.config"%(StudyName, LowerBinSt, UpperBinSt, Sample, RunOpt), 'w')
    f1 = open( "./%s/%s/%s.config"%(StudyName, RunOpt, Sample), 'w')
    #f1.write('PileUpMCFileName : "Data_Mu_v1.root" \n')
    MCPUPileUp = "PileupMC_v2.root"
    if Sample.count('HEM') : MCPUPileUp = "PUSingleNeuFlatHEM.root"
    f1.write('PileUpMCFileName : "%s" \n'%(MCPUPileUp))
    #f1.write('PileUpDATAFileName : "pileup_DT_UL18.root"\n')
    DataPU = "DATAPileUp.root"
    if RunOpt.count('RunA')  : DataPU = "v1_RunA.root" 
    if RunOpt.count('RunB')  : DataPU = "v1_RunB.root" 
    if RunOpt.count('RunC')  : DataPU = "v1_RunC.root" 
    if RunOpt.count('RunD')  : DataPU = "v1_RunC.root" 
    
    f1.write('PileUpDATAFileName : "%s"\n'%(DataPU))
    f1.write('trigger : {"HLT_ZeroBias_part","HLT_ZeroBias_v"}\n')
    #f1.write('DoPUCor : "false"\n')
    f1.write('DoPrescale : "true"\n')
    f1.write('DoPUCor : "true"\n')
    #f1.write('DoPUCor : "false"\n') ## For NoPU
    f1.write('DoJetVeto : "true"\n')
    f1.write('JetVetoFName : "hotjets-UL18" \n')
    f1.write('JetVetoHName : "h2hot_ul18_plus_hem1516_and_hbp2m1"\n')
    #f1.write('EtaBins : {"0","0p5","0p8","1p1","1p3","1p7","1p9","2p1","2p3","2p5","2p8","3p0","3p2","4p7"} \n') 
    f1.write('EtaBins : {"0", "0p261", "0p522", "0p783", "1p044", "1p305", "1p566", "1p740", "1p930", "2p043", "2p172", "2p322", "2p500", "2p650", "2p853", "2p964", "3p139", "3p489", "3p839", "5p191"}\n') 
    f1.write('JECApply : "true"\n')
    #f1.write('JECApply : "false"\n')
    f1.close()

def main():
    Samples = ["SingleNeutrino_Flat2018", "SingleNeutrino_Flat2018_HEM"]
    #Samples = ["SingleNeutrino_NoPU", "SingleNeutrino_NoPU_HEM"]
    RunPeriod = ["PURunA","PURunC"]
    RunPeriod = ["PURunA","PURunB","PURunC","PURunD"]
    #StudyName = "OffCor_JetVetoV1p2"
    StudyName = "TestPFCHS_v1"
    StudyName = "NewPFCHS_v2"
    StudyName = "NewPFCHS_v4"
    StudyName = "AppliedJEC_v1"
    StudyName = "AppliedJEC_v2"
    StudyName = "AppliedJEC_v3"
    StudyName = "AppliedJEC_v4"
    StudyName = "NonAppliedJEC_v4"
    StudyName = "StudyRun2018_v7"
    StudyName = "StudyRun2018_v6"
    StudyName = "StudySC2018_v2"
    for isample in Samples:
        for ipu in RunPeriod:
            mkdircmd = "mkdir -p %s/%s"%(StudyName, ipu)
            os.system(mkdircmd)
            Make_Config(StudyName, ipu ,isample)
            pass
    pass
if __name__ == '__main__':
    main()
