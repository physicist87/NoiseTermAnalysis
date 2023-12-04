#!/bin/tcsh 
setenv SCRAM_ARCH slc6_amd64_gcc530 
source /cvmfs/cms.cern.ch/cmsset_default.csh 
cd /d0/scratch/sha/Analyses/ServiceWork/JERC/NoiseTerm_v1/AnalysisCode/AnCode/v3p1/CMSSW_10_6_8_patch1/src/SingleConeStudy_v4/Analysis_v3/ 
mkdir -p ./output/Cone08_2018_v1/MuScale/Central/Data_ZeroBias_Run2018Av2 
cmsenv 
set inputlists = ("Data_ZeroBias_Run2018Av2_56" ) 
foreach i ( $inputlists )
   mkdir -p output 
   ./NoiseTerm_Study Data_ZeroBias_Run2018Av2/${i}.list Cone08_2018_v1/MuScale/Central/Data_ZeroBias_Run2018Av2/ ${i}.root ./configs/Cone08_2018_v1/MuScale/Central/Data_ZeroBias_Run2018Av2.config 
end 
