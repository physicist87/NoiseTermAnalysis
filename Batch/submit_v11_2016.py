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
        f.write('setenv LD_PRELOAD "/usr/lib64/libpdcap.so" \n')
        f.write('cd '+ path + " \n")
        #f.write('mkdir -p ./output/%s/%s \n'%(sampleName,outputName))
        f.write('mkdir -p ./output/%s/%s \n'%(outputName,sampleName))
        f.write('cmsenv \n')
        SampList = MakeSampleIdxList(SampleFile,Lists[i])
        print "SampList %s"%(SampList)
        f.write('set inputlists = (%s) \n'%(SampList) )
        f.write('foreach i ( $inputlists )\n')
        f.write('   mkdir -p output \n')
        f.write('   ./NoiseTerm_Study Fix30GeV/%s/${i}.list %s/%s/ ${i}.root %s \n'%(sampleName,outputName,sampleName, ConfigName))
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
        f1.write('request_memory = 20000 \n')
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
    SubName = "NoMuScale"
    #StudyName = "AppliedJEC_v4"

    StudyName = "StudySC2017_v5"
    StudyName = "Fix30_2016_v1"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    StudyName = "StudySC2016_v7"
    StudyName = "Fix30_2016_v1p2"

    #Study = "%s/%s/%s"%(StudyName, SubName, SystOpt) 
    #Study = "%s/%s"%(StudyName, SubName) 
    #Make_CondorScr("Data_ZeroBias_Run2017Bv1",Study,98,98,  "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Cv1",Study,207,207,  "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Dv1",Study,110,110, "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Ev1",Study,172,172, "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Fv1",Study,234,234, "./configs/%s/Data.config"%(Study))

    #Make_CondorScr("Data_ZeroBias_Run2017Bv1",Study,98,98,  "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Cv1",Study,207,207,  "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Dv1",Study,110,110, "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Ev1",Study,172,172, "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Fv1",Study,234,234, "./configs/%s/Data.config"%(Study))

    #Make_CondorScr("Data_ZeroBias_Run2017Bv1",Study,98,98,  "./configs/%s/Data_PURunB.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Cv1",Study,207,207,  "./configs/%s/Data_PURunC.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Dv1",Study,110,110, "./configs/%s/Data_PURunD.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Ev1",Study,172,172, "./configs/%s/Data_PURunE.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Fv1",Study,234,234, "./configs/%s/Data_PURunF.config"%(Study))


    SystOpt = ["PileUpUp","PileUpDown", "JECUp","JECDown"]
    SystOpt = ["PileUpUp","PileUpDown"]
    SystOpt = ["JECUp","JECDown"]
    SystOpt = ["PileUpUp","PileUpDown"]
    SystOpt = ["JECDown"]
    SystOpt = ["NoPU"]
    SystOpt = ["JECUp","JECDown", "PileUpUp","PileUpDown"]
    SystOpt = ["Central"]

    for isys in SystOpt: 
        Study = "%s/%s/%s"%(StudyName, SubName, isys) 

        Make_CondorScr("Data_ZeroBias_Run2016Bv1",Study,21,21,  "./configs/%s/Data_ZeroBias_Run2016Bv1.config"%(Study))
        Make_CondorScr("Data_ZeroBias_Run2016Cv1",Study,8,8,  "./configs/%s/Data_ZeroBias_Run2016Cv1.config"%(Study))
        Make_CondorScr("Data_ZeroBias_Run2016Dv1",Study,12,12, "./configs/%s/Data_ZeroBias_Run2016Dv1.config"%(Study))
        Make_CondorScr("Data_ZeroBias_Run2016Ev1",Study,11,11, "./configs/%s/Data_ZeroBias_Run2016Ev1.config"%(Study))
        Make_CondorScr("Data_ZeroBias_Run2016F_e_v1",Study,7,7, "./configs/%s/Data_ZeroBias_Run2016F_e_v1.config"%(Study))
        Make_CondorScr("Data_ZeroBias_Run2016F_l_v1",Study,2,2, "./configs/%s/Data_ZeroBias_Run2016F_l_v1.config"%(Study))
        Make_CondorScr("Data_ZeroBias_Run2016Gv1",Study,18,18, "./configs/%s/Data_ZeroBias_Run2016Gv1.config"%(Study))
        Make_CondorScr("Data_ZeroBias_Run2016Hv1",Study,20,20, "./configs/%s/Data_ZeroBias_Run2016Hv1.config"%(Study))
