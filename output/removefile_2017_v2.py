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


    StudyName = "NonAppliedL1Off_2017_v1"
    StudyName = "AllJEC_2017_v1"
    StudyName = "AllJEC_v1"
    SubName = "NoMuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    StudyName = "StudyRun2017_v1p2"
    StudyName = "StudyRun2017_v2"
    StudyName = "StudyRun2017_v3"
    StudyName = "StudyRun2017_v3"
    StudyName = "StudyRun2017_v4_NoJEC"
    StudyName = "StudyRun2017_v5"
    StudyName = "StudyRun2017_v5_NoJEC"
    StudyName = "StudyRun2017_v5"
    StudyName = "StudyRun2017_v5p1"
    StudyName = "StudyRun2017_v5_NoJEC"
    StudyName = "StudySC2017_v2"
    StudyName = "StudyRun2017_v5p2"
    StudyName = "StudyRun2017_v5p2_bk"
    StudyName = "StudySC2018_v5"
    StudyName = "StudySC2017_v5"
    StudyName = "StudySC2017_v5_bk"
    StudyName = "Fix30_2017_v1p2"
    StudyName = "StudySC2017_v7"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "MuScale"
    #SubName = "NoMuScale"
    Study = "%s/%s/PileUpUp"%(StudyName, SubName) 
    Study = "%s/%s/PileUpDown"%(StudyName, SubName) 
    Study = "%s/%s/All"%(StudyName, SubName) 
    Study = "%s/%s/JECUp"%(StudyName, SubName) 
    Study = "%s/%s/JECDown"%(StudyName, SubName) 
    Study = "%s/%s"%(StudyName, SubName) 



    SystOpt = ["L1RC","L1RCNega","L1Nega","L1Cor","All","AllNega","AllRCNega", "JECUp","JECDown", "PileUpUp","PileUpDown"]
    SystOpt = [ "PileUpUp","PileUpDown"]
    SystOpt = ["NoJEC"]
    SystOpt = ["Central"]
    SystOpt = ["Central","JECUp","JECDown", "PileUpUp","PileUpDown"]
    print Study

    for isys in SystOpt: 
        Study = "%s/%s/%s"%(StudyName, SubName, isys) 
        print Study

        RemoveFiles("Data_ZeroBias_Run2017Bv2",Study, 49)
        RemoveFiles("Data_ZeroBias_Run2017Cv2",Study, 104)
        RemoveFiles("Data_ZeroBias_Run2017Dv2",Study, 55)
        RemoveFiles("Data_ZeroBias_Run2017Ev2",Study, 86)
        RemoveFiles("Data_ZeroBias_Run2017Fv2",Study, 118)
