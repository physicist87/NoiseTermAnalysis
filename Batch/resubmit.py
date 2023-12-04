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
    StudyName = "AppliedJEC_v1"
    SubName = "MuScale"
    StudyName = "StudyRun2017_v5p1"
    StudyName = "StudySC2017_v2"
    StudyName = "StudyRun2017_v5p2"

    Study = "%s/%s"%(StudyName,SubName) 
    SystOpt = "PileUpUp"
    SystOpt = "PileUpDown"
    SystOpt = "All"
    #SystOpt = "PileUpDown"

    StudyName = "StudySC2018_v2"
    StudyName = "StudySC2018_v5"
    StudyName = "StudySC2018_v7"
    StudyName = "StudySC2018_v8"
    StudyName = ""
    StudyName = "Cone05_2018_v1"
    StudyName = "Cone03_2018_v1"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    SubName = "NoMuScale"
    SubName = "NoMuScale"
    SubName = "MuScale"
    Study = "%s/%s/%s"%(StudyName,SubName, SystOpt) 
    Study = "%s/%s"%(StudyName,SubName) 
    #HaddFiles("SingleNeutrino_Flat2018",StduyName,40)
    #HaddFiles("SingleNeutrino_Flat2018",StduyName,40)
#     resubmit("Data_ZeroBias_Run2018Av2", StduyName, 222, "./configs/EtaBin_%s_%s.config"%(lower,upper))
#     resubmit("Data_ZeroBias_Run2018Bv2", StduyName, 105, "./configs/EtaBin_%s_%s.config"%(lower,upper))
#     resubmit("Data_ZeroBias_Run2018Cv1", StduyName, 106, "./configs/EtaBin_%s_%s.config"%(lower,upper))
#     resubmit("Data_ZeroBias_Run2018Dv1", StduyName, 504, "./configs/EtaBin_%s_%s.config"%(lower,upper))
     #resubmit("Data_ZeroBias_Run2018Bv2", StduyName, 105 )
     #resubmit("Data_ZeroBias_Run2018Cv1", StduyName, 106 )
     #resubmit("Data_ZeroBias_Run2018Dv1", StduyName, 504 )
    #Make_CondorScr("Data_ZeroBias_Run2017Bv1",Study,98,98,  "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Cv1",Study,207,207,  "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Dv1",Study,110,110, "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Ev1",Study,172,172, "./configs/%s/Data.config"%(Study))
    #Make_CondorScr("Data_ZeroBias_Run2017Fv1",Study,234,234, "./configs/%s/Data.config"%(Study))
    #resubmit("Data_ZeroBias_Run2017Bv2",Study,245,  "./configs/%s/Data_PURunB.config"%(Study))

    SystOpt = ["JECUp","JECDown"]
    SystOpt = ["Central"]
    #resubmit("Data_ZeroBias_Run2018Av2", Study, 56, "./configs/%s/Data.config"%(Study))
    #resubmit("Data_ZeroBias_Run2018Bv2", Study, 27,"./configs/%s/Data.config"%(Study))
    #resubmit("Data_ZeroBias_Run2018Cv1", Study, 27, "./configs/%s/Data.config"%(Study))
    #resubmit("Data_ZeroBias_Run2018Dv1", Study,  126, "./configs/%s/Data.config"%(Study))
    for isys in SystOpt: 
        Study = "%s/%s/%s"%(StudyName, SubName, isys) 

        print Study
        resubmit("Data_ZeroBias_Run2018Av2", Study, 56, "./configs/%s/Data_ZeroBias_Run2018Av2.config"%(Study))
        resubmit("Data_ZeroBias_Run2018Bv2", Study, 27,"./configs/%s/Data_ZeroBias_Run2018Bv2.config"%(Study))
        resubmit("Data_ZeroBias_Run2018Cv1", Study, 27, "./configs/%s/Data_ZeroBias_Run2018Cv1.config"%(Study))
        resubmit("Data_ZeroBias_Run2018Dv1", Study,  126, "./configs/%s/Data_ZeroBias_Run2018Dv1.config"%(Study))
