import os
import sys
import subprocess


def CheckRootFile(FileName,Nums):
    runCheck =True
    for idx_ in range(1,Nums+1) :
        if os.path.isfile(FileName+"_%s.root"%(idx_)):
            print " ok %s " %(FileName+"_%s.root"%(idx_))
            pass
        else :
            print " No File :  %s" %(FileName+"_%s.root"%(idx_))
            runCheck = False
    return runCheck
    pass

def HaddFiles(sampleName,outputName,NumFileList) :

    path = os.getcwd()

    os.chdir("%s/%s/%s"%(path, outputName, sampleName))
    ### Merge file ### 
    if CheckRootFile(sampleName,NumFileList) == True:
        print "Do Hadd Sys!!!"
        cmd = "hadd %s.root %s_*.root"%(sampleName,sampleName)
        print cmd
        os.system(cmd)
        cmd = "rm -rf %s_*.root "%(sampleName)
        print cmd
        #os.system(cmd)
    else : print "Skip hadd !!!"
    os.chdir(path)


    EtaBinsSt = ["0","0p5","0p8","1p1","1p3","1p7","1p9","2p1","2p3","2p5","2p8","3p0","3p2","4p7"]
    EtaBinsSt = ["0", "0p261","0p522","0p783","1p044","1p305","1p566","1p740","1p930","2p043","2p172","2p322","2p500","2p650","2p853","2p964","3p139","3p489","3p839","5p191"]
# /d0/scratch/sha/Analyses/ServiceWork/JERC/NoiseTerm_v1/AnalysisCode/CMSSW_10_6_8_patch1/src/ForCondor_v1/JobSubmit_v2/ApplyJEC_v4/output/AppliedJEC_v1/MuScale/EtaBin_0_0p5/Data_ZeroBias_Run2018Av2
    for idx,lower in enumerate(EtaBinsSt):
        if idx < len(EtaBinsSt)-1:
            print "idx %s "%(idx) 
            upper = EtaBinsSt[idx+1]
            etabins= "EtaBin_%s_%s"%(lower, upper) 
            print path
            os.chdir("%s/%s/%s/%s"%(path, outputName, sampleName, etabins))
            os.system ('pwd') 
     
#    CheckRootFile(sampleName,NumFileList)
            if CheckRootFile(sampleName,NumFileList) == True:
                print "Do Hadd Sys!!!"
                cmd = "hadd %s.root %s_*.root"%(sampleName,sampleName)
                print cmd
                os.system(cmd)
                cmd = "rm -rf %s_*.root "%(sampleName)
                print cmd
                #os.system(cmd)
            else : print "Skip hadd !!!"

            os.chdir(path)
    pass


if __name__ == '__main__':
    #HaddFiles("QCD_Pt-15to7000_TuneCP5_Flat2018","Study_v11",80)
    ##HaddFiles("SingleNeutrino_v1","Test_v2",40,40)
    ##HaddFiles("SingleNeutrino_v1","Study_v11",40)
    #HaddFiles("Run2018Av2","Study_v11p1",277)
    #HaddFiles("Run2018Bv2","Study_v11p1",131)
    #HaddFiles("Run2018Cv1","Study_v11p1",132)
    #HaddFiles("Run2018Dv1","Study_v11p1",630)

    #HaddFiles("Data_ZeroBias_Run2018Av2","Study_v11p1",222)
    #HaddFiles("Data_ZeroBias_Run2018Bv2","Study_v11p1",131)
    #HaddFiles("Data_ZeroBias_Run2018Cv1","Study_v11p1",132)
    #HaddFiles("Data_ZeroBias_Run2018Dv1","Study_v11p1",630)
    #HaddFiles("QCD_Pt-15to7000_TuneCP5_Flat2018","Study_v12",80)
    #HaddFiles("SingleNeutrino_v1","Study_v12",40)
    #HaddFiles("SingleNeutrino_Flat2018","NPUvsNPV_v6",40)
    #HaddFiles("SingleNeutrino_Flat2018","NPUvsNPV_v6_NoPUCor",40)
    #HaddFiles("Data_ZeroBias_Run2018Av2","NPUvsNPV_v6",222)
    #HaddFiles("Data_ZeroBias_Run2018Bv2","NPUvsNPV_v6",105)
    #HaddFiles("Data_ZeroBias_Run2018Cv1","NPUvsNPV_v6",105)
    #HaddFiles("Data_ZeroBias_Run2018Dv1","NPUvsNPV_v6",504)
    #HaddFiles("SingleNeutrino_Flat2018","NPUvsNPV_v7",40)
    #HaddFiles("SingleNeutrino_Flat2018","NPUvsNPV_v7_NoPUCor",40)
    #HaddFiles("SingleNeutrino_Flat2018","NPUvsNPVall_v7",40)
    #HaddFiles("SingleNeutrino_Flat2018","NPUvsNPVall_v7_NoPUCor",40)

    StudyName = "TestPFCHS_v1"
    StudyName = "NewPFCHS_v3"
    StudyName = "AppliedJEC_v4"
    StudyName = "StudyRun2018_v6"
    StudyName = "StudyRun2018_v7"
    StudyName = "NonAppliedL1Off_2018_v1"
    StudyName = "StudySC2018_v2"
    StudyName = "StudySC2018_v4"
    StudyName = "StudySC2018_v5"
    StudyName = "StudySC2017_v5"
    StudyName = "StudySC2017_v5p1"
    StudyName = "Fix30_2017_v1"
    StudyName = "StudySC2017_v7"
    StudyName = "Fix30_2017_v1p2"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "Central"
    Study = "%s/%s"%(StudyName, SubName) 
    RunPeriod = ["PURunA","PURunB","PURunC","PURunD"]
    RunPeriod = ["PURunB","PURunC","PURunD","PURunE","PURunF"]
    RunPeriod = ["MC"]
    SystOpt = ["L1RC","L1RCNega","L1Nega","L1Cor","All","AllNega","AllRCNega","Central"]
    SystOpt = ["Central","L1RC","L1RCNega","L1Nega","L1Cor","All","AllNega","AllRCNega"]
    SystOpt = ["L1RCNega","L1Nega"]
    SystOpt = ["NoPU","NoJEC"]
    SystOpt = ["Central","PileUpUp","PileUpDown", "JECUp","JECDown"]
    SystOpt = ["Central"]
    for ipu in RunPeriod:
        for isys in SystOpt: 

            Study = "%s/%s/%s"%(StudyName, ipu, isys) 
            #if ipu == "PURunA" or ipu == "PURunB": HaddFiles("SingleNeutrino_Flat2018",Study,40)
            #if ipu == "PURunC" or ipu == "PURunD": HaddFiles("SingleNeutrino_Flat2018_HEM",Study,79)
            #if ipu == "MC" or ipu == "mc": 
                #HaddFiles("SingleNeutrino_Flat2018",Study,30)
            HaddFiles("SingleNeutrino_PreMix2017",Study, 21)
#                HaddFiles("SingleNeutrino_PreMix2018",Study,216)
            #if ipu == "PURunA" or ipu == "PURunB": HaddFiles("SingleNeutrino_NoPU",Study,40)
            #if ipu == "PURunC" or ipu == "PURunD": HaddFiles("SingleNeutrino_NoPU_HEM",Study,79)
    #HaddFiles("SingleNeutrino_Flat2018",StduyName,40)
    #HaddFiles("SingleNeutrino_Flat2018",StduyName,40)
    #HaddFiles("Data_ZeroBias_Run2018Av2", Study, 222 )
    #HaddFiles("Data_ZeroBias_Run2018Bv2", Study, 105 )
    #HaddFiles("Data_ZeroBias_Run2018Cv1", Study, 106 )
    #HaddFiles("Data_ZeroBias_Run2018Dv1", Study, 504 )

