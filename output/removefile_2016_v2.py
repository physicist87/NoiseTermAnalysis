import os
import sys
import subprocess


def MergedFile(FileName):
    runCheck =True
    if os.path.isfile(FileName+".root"):
        print " ok %s " %(FileName+".root")
        pass
    else :
        print " No File :  %s" %(FileName+".root")
        runCheck = False
    return runCheck
    pass


def RemoveFiles(sampleName,outputName,NumFileList) :
    path = os.getcwd()
    print path
    os.chdir("%s/%s/%s"%(path,outputName,sampleName))
    os.system ('pwd') 
     
#    MergedFile(sampleName,NumFileList)
    if MergedFile(sampleName) == True:
        #for idx in NumFileList:
        print "Do Remove the Files!"
        for idx_ in range(1,NumFileList+1) :
            FileName = "%s_%s.root"%(sampleName,idx_)
            print "FileName : %s"%(FileName)
            if os.path.isfile(FileName):   
                print "??"
                cmd = "rm -rf %s_%s.root"%(sampleName,idx_)
                print cmd
                os.system(cmd)
            else : 
                print "No file !! %s "%(FileName)
                pass
            pass
        EtaBinsSt = ["0", "0p261","0p522","0p783","1p044","1p305","1p566","1p740","1p930","2p043","2p172","2p322","2p500","2p650","2p853","2p964","3p139","3p489","3p839","5p191"]
        for idx,lower in enumerate(EtaBinsSt):
            if idx < len(EtaBinsSt)-1:
                upper = EtaBinsSt[idx+1]
                etabins= "EtaBin_%s_%s"%(lower, upper)
                targetdir = "%s/%s/%s/%s"%(path, outputName, sampleName, etabins)
                os.chdir(targetdir)
                os.system ('pwd') 
     
                if MergedFile(sampleName) == True:
                    for idx_ in range(1,NumFileList+1) :
                        FileName = "%s_%s.root"%(sampleName,idx_)
                        print "FileName in %s : %s"%(etabins,FileName)
                        if os.path.isfile(FileName):
                            print "Good "
                            cmd = "rm -rf %s_%s.root"%(sampleName,idx_)
                            print cmd
                            os.system(cmd)
                        else : 
                            print "No file !!"
    
                        pass 

        pass
    else : print "Skip Remove Files !!!"
    os.chdir(path)
    pass 




if __name__ == '__main__':



    StudyName = "StudySC2016_v5_bk"
    StudyName = "StudySC2016_v5"
    StudyName = "StudySC2016_v7"
    SubName = "NoMuScale"
    SubName = "MuScale"
    Study = "%s/%s/Central"%(StudyName, SubName)
 
    SystOpt = ["Central","L1RC","L1RCNega","L1Nega","L1Cor","All","AllNega","AllRCNega"]
    SystOpt = ["L1RCNega","L1Nega"]
    SystOpt = ["PileUpUp","PileUpDown"]
    SystOpt = ["NoJEC"]
    SystOpt = ["PileUpUp","PileUpDown"]
    SystOpt = ["Central"]
    SystOpt = ["JECUp","JECDown"]
    SystOpt = ["Central","JECUp","JECDown", "PileUpUp","PileUpDown"]
    SystOpt = ["JECDown"]
    SystOpt = ["PileUpUp","PileUpDown"]
    SystOpt = ["Central","JECUp","JECDown"]
    SystOpt = ["Central","JECUp","JECDown", "PileUpUp","PileUpDown"]
    #Study = "%s/%s"%(StudyName, SubName) 
    print Study
    #HaddFiles("Data_ZeroBias_Run2018Av2", Study, 56)
    #HaddFiles("Data_ZeroBias_Run2018Bv2", Study, 27)
    #HaddFiles("Data_ZeroBias_Run2018Cv1", Study, 27)
    #HaddFiles("Data_ZeroBias_Run2018Dv1", Study, 126)
    for isys in SystOpt: 
        Study = "%s/%s/%s"%(StudyName, SubName, isys) 
        print Study


        RemoveFiles("Data_ZeroBias_Run2016Bv1",Study,21)
        RemoveFiles("Data_ZeroBias_Run2016Cv1",Study,8)
        RemoveFiles("Data_ZeroBias_Run2016Dv1",Study,12)
        RemoveFiles("Data_ZeroBias_Run2016Ev1",Study,11)
        RemoveFiles("Data_ZeroBias_Run2016F_e_v1",Study,7)
        RemoveFiles("Data_ZeroBias_Run2016F_l_v1",Study,2)
        RemoveFiles("Data_ZeroBias_Run2016Gv1",Study,18)
        RemoveFiles("Data_ZeroBias_Run2016Hv1",Study,20)
