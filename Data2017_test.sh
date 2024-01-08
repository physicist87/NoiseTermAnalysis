#!/bin/tcsh 
setenv SCRAM_ARCH slc6_amd64_gcc530 
source /cvmfs/cms.cern.ch/cmsset_default.csh 
setenv LD_PRELOAD "/usr/lib64/libpdcap.so" 
cd /d0/scratch/sha/Analyses/ServiceWork/JERC/NoiseTerm_v1/AnalysisCode/AnCode/v3p3/CMSSW_10_6_30/src/SingleConeStudy_v4/Analysis_v3p1/NoiseTermAnalysis/ 
mkdir -p ./output/JECSummer20L2Relative_2017_v1/MuScale/Central/Data_ZeroBias_Run2017Bv2 
cmsenv 
set inputlists = ("Data_ZeroBias_Run2017Bv2_test" ) 
foreach i ( $inputlists )
   mkdir -p output 
   ./NoiseTerm_Study Data_ZeroBias_Run2017Bv2/${i}.list JECSummer20L2Relative_2017_v1/MuScale/Central/Data_ZeroBias_Run2017Bv2/ ${i}.root ./configs/JECSummer20L2Relative_2017_v1/MuScale/Central/Data_PURunB.config 
end 
