import os 
import sys

def Make_Config(LowerBin,UpperBin, LowerBinSt, UpperBinSt, Sample) :
    ### making config File ###
    f1 = open( "./OffCor_JetVeto_PreSc_v1_EtaBin_%s_%s_%s.config"%(LowerBinSt,UpperBinSt,Sample), 'w')
    #f1.write('PileUpMCFileName : "Data_Mu_v1.root" \n')
    f1.write('PileUpMCFileName : "DataMu_%s.root" \n'%(Sample))
    #f1.write('PileUpDATAFileName : "pileup_DT_UL18.root"\n')
    DataPU = "DATAPileUp.root"
    if Sample == "Data_ZeroBias_Run2018Av2" : DataPU = "v1_RunA.root" 
    if Sample == "Data_ZeroBias_Run2018Bv2" : DataPU = "v1_RunB.root" 
    if Sample == "Data_ZeroCias_Run2018Cv1" : DataPU = "v1_RunC.root" 
    if Sample == "Data_ZeroDias_Run2018Dv1" : DataPU = "v1_RunD.root" 
    
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
    EtaBins = [0,0.5,0.8,1.1,1.3,1.7,1.9,2.1,2.3,2.5,2.8,3.0,3.2,4.7]
    EtaBinsSt = ["0","0p5","0p8","1p1","1p3","1p7","1p9","2p1","2p3","2p5","2p8","3p0","3p2","4p7"]
    Samples = ["Data_ZeroBias_Run2018Av2","Data_ZeroBias_Run2018Bv2","Data_ZeroBias_Run2018Cv1","Data_ZeroBias_Run2018Dv1"]
    for isample in Samples:
        for idx,lower in enumerate(EtaBins):
            if idx < len(EtaBins)-1:
                print "idx %s "%(idx) 
                lowerbinst = EtaBinsSt[idx]
                upper = EtaBins[idx+1]
                upperst = EtaBinsSt[idx+1]
                print "upper %s lower %s"%(upper,lower)
                Make_Config(lower,upper,lowerbinst,upperst,isample) 
    pass
if __name__ == '__main__':
    main()
