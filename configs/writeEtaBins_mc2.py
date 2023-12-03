import os 
import sys

def Make_Config(StudyName, LowerBin, UpperBin, LowerBinSt, UpperBinSt, Sample) :
    ### making config File ###
    #f1 = open( "./%s_EtaBin_%s_%s_%s_%s.config"%(StudyName, LowerBinSt, UpperBinSt, Sample, RunOpt), 'w')
    f1 = open( "./%s_EtaBin_%s_%s_%s.config"%(StudyName, LowerBinSt, UpperBinSt, Sample), 'w')
    #f1.write('PileUpMCFileName : "Data_Mu_v1.root" \n')
    MCPUPileUp = "PileupMC_v2.root"
    DataPU = "DATAPileUpAtoB.root"
    if Sample.count('HEM') : 
        MCPUPileUp = "PUSingleNeuFlatHEM.root"
        DataPU = "DATAPileUpCtoD.root"
    f1.write('PileUpMCFileName : "%s" \n'%(MCPUPileUp))
    f1.write('PileUpDATAFileName : "%s"\n'%(DataPU))
    f1.write('trigger : {"HLT_ZeroBias_part","HLT_ZeroBias_v"}\n')
    #f1.write('DoPUCor : "false"\n')
    f1.write('DoPUCor : "true"\n')
    f1.write('EtabinLower : %s\n'%(LowerBin))
    f1.write('EtabinUpper : %s\n'%(UpperBin))
    f1.write('DoJetVeto : "true"\n')
    f1.write('JetVetoFName : "hotjets-UL18" \n')
    f1.write('JetVetoHName : "h2hot_ul18_plus_hem1516_and_hbp2m1"\n')
    f1.close()

def main():
    Samples = ["Data_ZeroBias_Run2018Av2","Data_ZeroBias_Run2018Bv2","Data_ZeroBias_Run2018Cv1","Data_ZeroBias_Run2018Dv1"]
    Samples = ["SingleNeutrino_Flat2018", "SingleNeutrino_Flat2018_HEM"]
    #StudyName = "OffCor_JetVetoV1p2"
    StudyName = "TestPFCHS_v1"
    for isample in Samples:
        for idx,lower in enumerate(EtaBins):
            if idx < len(EtaBins)-1:
                print "idx %s "%(idx) 
                lowerbinst = EtaBinsSt[idx]
                upper = EtaBins[idx+1]
                upperst = EtaBinsSt[idx+1]
                print "upper %s lower %s"%(upper,lower)
                Make_Config(StudyName, lower,upper,lowerbinst,upperst,isample)
                pass
    pass
if __name__ == '__main__':
    main()
