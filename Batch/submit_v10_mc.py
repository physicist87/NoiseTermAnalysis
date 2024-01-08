import os
import sys
import subprocess


def Make_CondorScr(sampleName,outputName,NumFileList,NumJob, ConfigName) :
    path = os.getcwd()
    os.system("mkdir -p %s/%s"%(outputName,sampleName))
    path=path.replace("/Batch", "/")
    os.getcwd()
    os.system ('pwd')
    os.system ('mkdir -p Log')
    os.system ('ls')
    print path
    numjobs = NumJob
    numfiles = NumFileList
    SampleFile = sampleName
    SubFile = SampleFile + "_"
    #qsubcmd = 'qsub -q short '
    Lists = MakeSeparateList(numfiles,numjobs)
    for i in range(0,numjobs):
        idx_ = i +1
        runing = outputName + "/"+SampleFile + "_%s.sh" % (idx_)
        f = open( runing, 'w')
        f.write('#!/bin/tcsh \n')
        f.write('setenv SCRAM_ARCH slc6_amd64_gcc530 \n')
        f.write('source /cvmfs/cms.cern.ch/cmsset_default.csh \n')
        #f.write('setenv LD_PRELOAD "/usr/lib64/libpdcap.so" \n')
        f.write('cd '+ path + " \n")
        #f.write('mkdir -p ./output/%s/%s \n'%(sampleName,outputName))
        f.write('mkdir -p ./output/%s/%s \n'%(outputName,sampleName))
        f.write('cmsenv \n')
        SampList = MakeSampleIdxList(SampleFile,Lists[i])
        print "SampList %s"%(SampList)
        f.write('set inputlists = (%s) \n'%(SampList) )
        f.write('foreach i ( $inputlists )\n')
        f.write('   mkdir -p output \n')
        f.write('   ./NoiseTerm_Study %s/${i}.list %s/%s/ ${i}.root %s \n'%(sampleName,outputName,sampleName, ConfigName))
        f.write('end \n')
        #runcmd = "./ssb_analysis "+ SampleFile + "_%s.list " % (i) + SampleFile + "_%s.root " % (i) + " 0 "
        #f.write(runcmd)
        f.close()
        runchMod = "chmod 755 " + runing
        os.system (runchMod)

        ### making submit File ###
        f1 = open( "./%s/%s/condor_%s_%s.submit"%(outputName,sampleName,SampleFile,idx_) , 'w')
        f1.write('# Unix submit description file\n')
        f1.write('Universe = vanilla\n')
        #f1.write('x509userproxy = $ENV(X509_USER_PROXY)\n')
        f1.write('Executable = ./%s/%s_%s.sh\n'%(outputName,SampleFile,idx_))
        f1.write('request_memory = 5000 \n')
        f1.write('should_transfer_files   = Yes\n')
        f1.write('Output      = ./%s/%s/%s_%s.output\n'%(outputName,sampleName,SampleFile,idx_))
        f1.write('error       = ./%s/%s/errors_%s_%s.txt\n'%(outputName,sampleName,SampleFile,idx_))
        f1.write('log         = ./%s/%s/test_%s_%s.log\n'%(outputName,sampleName,SampleFile,idx_))
        f1.write('Queue 1\n')
        f1.close()
        subchMod = "condor_submit " + "./%s/%s/condor_%s_%s.submit"%(outputName,sampleName,SampleFile,idx_)
        os.system (subchMod)
    pass

def MakeSampleIdxList(Sample,Lists):
    SampleList =""
    for index_ in Lists:
        print "index_ %s in MakeSampleIdxList "%(index_)
        SampleList += '"%s_%s"'%(Sample,index_)
        SampleList += " "
        pass
        #SamleList
    print SampleList
    return SampleList
    pass

def MakeSeparateList(NumFiles, NumJob):
    quo = NumFiles/NumJob
    seplists=[]
    for inx_ in range(NumJob) :
        emptyarray =[]
        seplists.append(emptyarray)
    print "size of seplists : %s "%len(seplists)
    for inumfile in  range (1,NumFiles+1):
        #print "%s"%(inumfile%NumJob)
        seplists[inumfile%NumJob].append(inumfile)
    for idx_ in seplists:
        print "content of %s "%(idx_)
    return seplists
    pass


#Make_CondorScr("SingleNeutrino_v1",40,40)
if __name__ == '__main__':

    #EtaBinsSt = ["0p8","1p1","1p3","1p7","1p9","2p1","2p3","2p5","2p8","3p0","3p2","4p7"]
    #EtaBinsSt = ["0p5","0p8"]
    StudyName = "TestPFCHS_v1"
    StudyName = "AppliedJEC_v1"
    StudyName = "AppliedJEC_v3"
    StudyName = "AppliedJEC_v4"
    StudyName = "StudyRun2017_v1"
    StudyName = "StudyRun2017_v2"
    StudyName = "StudyRun2017_v4_NoJEC"
    StudyName = "StudyRun2017_v5_NoJEC"
    StudyName = "StudyRun2017_v5"
    StudyName = "StudyRun2017_v5p1"
    StudyName = "StudySC2017_v1"
    StudyName = "StudySC2017_v2"
    StudyName = "StudySC2018_v2"
    StudyName = "StudySC2018_v4"
    StudyName = "Fix30_2018_v1"
    StudyName = "StudySC2018_v5"
    StudyName = "StudySC2018_v6"
    StudyName = "StudySC2018_v7"
    StudyName = "StudySC2018_v8"
    StudyName = "Cone04_2018_v1"
    StudyName = "Cone03_2018_v1"
    StudyName = "Cone05_2018_v1"
    StudyName = "Cone08_2018_v1"
    StudyName = "JECSummer20L2Relative_2018_v1"
    #RunPeriod = ["PURunB","PURunC","PURunD","PURunE","PURunF","MC"]
    RunPeriod = ["MC"]
    RunPeriod = ["PURunA","PURunB","PURunC","PURunD"]

    RunPeriod = ["PURunB","PURunC","PURunD","PURunE","PURunF","MC"]
    RunPeriod = ["PURunA","PURunB","PURunC","PURunD","PURunE","PURunF","MC"]
    RunPeriod = ["PURunA","PURunB","PURunC","PURunD"]
    RunPeriod = ["PURunD"]
    RunPeriod = ["MC"]
    RunPeriod = ["PURunA","PURunB","PURunC","PURunD","MC"]
    SystOpt = ["L1RC","L1RCNega","L1Nega","L1Cor","All","AllNega","AllRCNega","Central"]
    SystOpt = ["L1RCNega","L1Nega"]
    SystOpt = ["JECUp","JECDown"]
    SystOpt = ["PileUpUp","PileUpDown","JECUp","JECDown"]
    SystOpt = ["NoJEC","NoPU"]
    SystOpt = ["Central"]
    SystOpt = ["Central","PileUpUp","PileUpDown","JECUp","JECDown"]
    SystOpt = ["NoJEC","NoPU"]
    SystOpt = ["Central","NoPU"]
    SystOpt = ["PileUpUp","PileUpDown","JECUp","JECDown","Central"]
    SystOpt = ["NoJEC"]
    SystOpt = ["Central"]
    for ipu in RunPeriod:
        for isys in SystOpt: 
            #Study = "%s/%s"%(StudyName, ipu) 
            Study = "%s/%s/%s"%(StudyName, ipu, isys) 
            #Study = "%s/%s"%(StudyName, ipu) 
            #Make_CondorScr("SingleNeutrino_Flat2017",Study,221,221, "./configs/%s/SingleNeutrino_Flat2017.config"%(Study))
            #Make_CondorScr("SingleNeutrino_Flat2017v2",Study,224,60, "./configs/%s/SingleNeutrino_Flat2017v2.config"%(Study))
            #Make_CondorScr("SingleNeutrino_Flat2017v2",Study,224,60, "./configs/%s/SingleNeutrino_Flat2017v2.config"%(Study))
       # Make_CondorScr("SingleNeutrino_Flat2017",Study,221,221, "./configs/%s/SingleNeutrino_Flat2017.config"%(Study))
      
            #if ipu == "PURunA" or ipu == "PURunB":Make_CondorScr("SingleNeutrino_Flat2018",Study,40,40, "./configs/%s/SingleNeutrino_Flat2018.config"%(Study))
            #if ipu == "PURunC" or ipu == "PURunD":Make_CondorScr("SingleNeutrino_Flat2018_HEM",Study,79,79, "./configs/%s/SingleNeutrino_Flat2018_HEM.config"%(Study))
            #if ipu == "MC" or ipu == "mc":
            Make_CondorScr("SingleNeutrino_Flat2018",Study,30, 30, "./configs/%s/SingleNeutrino_Flat2018.config"%(Study))
    print Study
    #Make_CondorScr("Data_ZeroBias_Run2018Av2",Study,222,50, "./configs/%s/Data.config"%(Study))
