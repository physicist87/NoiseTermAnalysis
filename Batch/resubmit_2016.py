import os

def resubmit(SampleName, StudyName, NumFiles, Config):
    path = os.getcwd()
    print "%s "%(StudyName)
    path=path.replace("/Batch", "/")
    total_ = 0
    for idx in range(1,NumFiles+1):
        fileName = "%s/output/%s/%s/%s_%s.root"%(path,StudyName,SampleName,SampleName,idx)
        if os.path.isfile(fileName): 
            file_size = os.path.getsize(r'%s'%fileName) 
            if file_size < 10000:
                print "Resubmit (wrong finished job) !! %s "%(fileName)
                Make_CondorScr(SampleName, idx, StudyName, Config)
                total_ = total_+1
            else : 
                pass
                #print "Good !! %s "%(fileName)
             
        else :
            print "Resubmit (missed job) !! %s "%(fileName)
            Make_CondorScr(SampleName, idx, StudyName, Config)
            total_ = total_+1
        
        pass
    print "%s total : %s "%(StudyName,total_)
    pass

def Make_CondorScr(sampleName, idx_, outputDir, ConfigName) :
    path = os.getcwd()
    os.system("mkdir -p %s/%s"%(outputDir,sampleName))
    path=path.replace("/Batch", "/")
    os.getcwd()
    os.system ('pwd')
    os.system ('mkdir -p Log')
    print path
    SampleFile = sampleName
    SubFile = SampleFile + "_"
    #qsubcmd = 'qsub -q short '
    runSh = outputDir + "_resubmit_"+SampleFile + "_%s.sh" % (idx_)
    print "runSh : %s"%(runSh)
    f = open( runSh, 'w')
    f.write('#!/bin/tcsh \n')
    f.write('setenv SCRAM_ARCH slc6_amd64_gcc530 \n')
    f.write('source /cvmfs/cms.cern.ch/cmsset_default.csh \n')
    #f.write('setenv LD_PRELOAD "/usr/lib64/libpdcap.so" \n')
    f.write('cd '+ path + " \n")
    #f.write('mkdir -p ./output/%s/%s \n'%(sampleName,outputDir))
    f.write('mkdir -p ./output/%s/%s \n'%(outputDir,sampleName))
    f.write('cmsenv \n')
    SampList = MakeSampleIdxList(SampleFile,idx_)
    print "SampList %s"%(SampList)
    f.write('set inputlists = (%s) \n'%(SampList) )
    f.write('foreach i ( $inputlists )\n')
    f.write('   mkdir -p output \n')
    f.write('   ./NoiseTerm_Study %s/${i}.list %s/%s/ ${i}.root %s \n'%(sampleName,outputDir,sampleName, ConfigName))
    f.write('end \n')
    #runcmd = "./ssb_analysis "+ SampleFile + "_%s.list " % (i) + SampleFile + "_%s.root " % (i) + " 0 "
    #f.write(runcmd)
    f.close()
    runchMod = "chmod 755 " + runSh
    os.system (runchMod)

    ### making submit File ###
    f1 = open( "./%s/%s/resubmit_condor_%s_%s.submit"%(outputDir,sampleName,SampleFile,idx_) , 'w')
    f1.write('# Unix submit description file\n')
    f1.write('Universe = vanilla\n')
    #f1.write('x509userproxy = $ENV(X509_USER_PROXY)\n')
    f1.write('Executable = ./%s\n'%(runSh))
    f1.write('request_memory = 20000 \n')
    f1.write('should_transfer_files   = Yes\n')
    f1.write('Output      = ./%s/%s/resubmit_%s_%s.output\n'%(outputDir,sampleName,SampleFile,idx_))
    f1.write('error       = ./%s/%s/resubmit_errors_%s_%s.txt\n'%(outputDir,sampleName,SampleFile,idx_))
    f1.write('log         = ./%s/%s/resubmit_test_%s_%s.log\n'%(outputDir,sampleName,SampleFile,idx_))
    f1.write('Queue 1\n')
    f1.close()
    subchMod = "condor_submit " + "./%s/%s/resubmit_condor_%s_%s.submit"%(outputDir,sampleName,SampleFile,idx_)
    os.system (subchMod)
    pass

def MakeSampleIdxList(Sample,index_):
    SampleList =""
    print "index_ %s in MakeSampleIdxList "%(index_)
    SampleList += '"%s_%s"'%(Sample,index_)
    SampleList += " "
    pass
    print SampleList
    return SampleList
    pass


if __name__ == '__main__':

    StudyName = "TestPFCHS_v1"
    SubName = "NoMuScale"
    #StudyName = "AppliedJEC_v4"

    StudyName = "StudySC2017_v5"
    StudyName = "Fix30_2016_v1"
    StudyName = "StudySC2016_v7"
    StudyName = "StudySC2016_v8"
    StudyName = "Cone05_2016_v1"
    StudyName = "Cone03_2016_v1"
    StudyName = "Cone08_2016_v1"
    StudyName = "JECSummer20L2Relative_2016_v1"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"

    SystOpt = ["PileUpUp","PileUpDown", "JECUp","JECDown"]
    SystOpt = ["PileUpUp","PileUpDown"]
    SystOpt = ["JECUp","JECDown"]
    SystOpt = ["PileUpUp","PileUpDown"]
    SystOpt = ["JECDown"]
    SystOpt = ["Central","JECUp","JECDown", "PileUpUp","PileUpDown"]
    SystOpt = ["Central"]


    for isys in SystOpt: 
        Study = "%s/%s/%s"%(StudyName, SubName, isys) 

        print Study
        resubmit("Data_ZeroBias_Run2016Bv1",Study,21,  "./configs/%s/Data_ZeroBias_Run2016Bv1.config"%(Study))
        resubmit("Data_ZeroBias_Run2016Cv1",Study,8, "./configs/%s/Data_ZeroBias_Run2016Cv1.config"%(Study))
        resubmit("Data_ZeroBias_Run2016Dv1",Study,12, "./configs/%s/Data_ZeroBias_Run2016Dv1.config"%(Study))
        resubmit("Data_ZeroBias_Run2016Ev1",Study,11, "./configs/%s/Data_ZeroBias_Run2016Ev1.config"%(Study))
        resubmit("Data_ZeroBias_Run2016F_e_v1",Study,7,"./configs/%s/Data_ZeroBias_Run2016F_e_v1.config"%(Study))
        resubmit("Data_ZeroBias_Run2016F_l_v1",Study,2,"./configs/%s/Data_ZeroBias_Run2016F_l_v1.config"%(Study))
        resubmit("Data_ZeroBias_Run2016Gv1",Study,18, "./configs/%s/Data_ZeroBias_Run2016Gv1.config"%(Study))
        resubmit("Data_ZeroBias_Run2016Hv1",Study,20, "./configs/%s/Data_ZeroBias_Run2016Hv1.config"%(Study))
