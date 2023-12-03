import os 
import sys

def Make_Config(StudyName, SubName, Sample) :
    ### making config File ###
    #f1 = open( "./NoPUCor_EtaBin_%s_%s.config"%(LowerBinSt,UpperBinSt), 'w')
    f1 = open( "./%s/%s/Data.config"%(StudyName, SubName ), 'w')
    #f1 = open( "./OffCor_JetVetoV1_EtaBin_%s_%s.config"%(LowerBinSt,UpperBinSt), 'w')
    #f1.write('PileUpMCFileName : "Data_Mu_v1.root" \n')
    f1.write('PileUpMCFileName : "PileupMC_v2.root" \n')
    f1.write('PileUpMCFileName : "PileupMC_v2.root" \n')
    #f1.write('PileUpMCFileName : "DataMu_PreScaled.root" \n')
    #f1.write('PileUpDATAFileName : "pileup_DT_UL18.root"\n')
    f1.write('PileUpDATAFileName : "DATAPileUp.root"\n')
    f1.write('trigger : {"HLT_ZeroBias_part","HLT_ZeroBias_v"}\n')
    #f1.write('DoPrescale : "false"\n')
    f1.write('DoPrescale : "true"\n')
    f1.write('DoPUCor : "true"\n')
    #f1.write('DoPUCor : "false"\n')
    f1.write('DoJetVeto : "true"\n')
    f1.write('JetVetoFName : "hotjets-UL18" \n')
    f1.write('JetVetoHName : "h2hot_ul18_plus_hem1516_and_hbp2m1"\n')
    f1.write('EtaBins : {"0","0p5","0p8","1p1","1p3","1p7","1p9","2p1","2p3","2p5","2p8","3p0","3p2","4p7"}\n') 
    f1.close()

def main():
    StudyName = "AppliedJEC_v1"
    SubStudyName = "MuScale"
    #for 
    mkdircmd = "mkdir -p %s/%s"%(StudyName, SubStudyName, Sample)
    os.system(mkdircmd)
    Make_Config(StudyName, SubStudyName, Sample) 
    pass
if __name__ == '__main__':
    main()
