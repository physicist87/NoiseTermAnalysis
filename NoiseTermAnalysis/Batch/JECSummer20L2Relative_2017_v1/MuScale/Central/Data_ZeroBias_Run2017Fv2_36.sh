#!/bin/tcsh 
setenv SCRAM_ARCH slc6_amd64_gcc530 
source /cvmfs/cms.cern.ch/cmsset_default.csh 
setenv LD_PRELOAD "/usr/lib64/libpdcap.so" 
cd /d0/scratch/sha/Analyses/ServiceWork/JERC/NoiseTerm_v1/Analysis/CMSSW_10_6_30/src/v1/NoiseTermAnalysis/ 
mkdir -p ./output/JECSummer20L2Relative_2017_v1/MuScale/Central/Data_ZeroBias_Run2017Fv2 
cmsenv 
set inputlists = ("Data_ZeroBias_Run2017Fv2_35" ) 
foreach i ( $inputlists )
   mkdir -p output 
   ./NoiseTerm_Study Data_ZeroBias_Run2017Fv2/${i}.list JECSummer20L2Relative_2017_v1/MuScale/Central/Data_ZeroBias_Run2017Fv2/ ${i}.root ./configs/JECSummer20L2Relative_2017_v1/MuScale/Central/Data_PURunF.config 
end 
