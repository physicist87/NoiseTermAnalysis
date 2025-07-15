#!/bin/tcsh 
setenv SCRAM_ARCH slc6_amd64_gcc530 
source /cvmfs/cms.cern.ch/cmsset_default.csh 
cd /d0/scratch/sha/Analyses/ServiceWork/JERC/NoiseTerm_v1/Analysis/v1/NoiseTermAnalysis/ 
mkdir -p ./output/JECSummer20L2Relative_2018_v1/PURunD/Central/SingleNeutrino_Flat2018 
cmsenv 
set inputlists = ("SingleNeutrino_Flat2018_19" ) 
foreach i ( $inputlists )
   mkdir -p output 
   ./NoiseTerm_Study SingleNeutrino_Flat2018/${i}.list JECSummer20L2Relative_2018_v1/PURunD/Central/SingleNeutrino_Flat2018/ ${i}.root ./configs/JECSummer20L2Relative_2018_v1/PURunD/Central/SingleNeutrino_Flat2018.config 
end 
