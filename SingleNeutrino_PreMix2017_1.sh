#!/bin/tcsh 
setenv SCRAM_ARCH slc6_amd64_gcc530 
source /cvmfs/cms.cern.ch/cmsset_default.csh 
setenv LD_PRELOAD "/usr/lib64/libpdcap.so" 
cd /d0/scratch/sha/Analyses/ServiceWork/JERC/NoiseTerm_v1/AnalysisCode/AnCode/v3p3/CMSSW_10_6_30/src/SingleConeStudy_v4/Analysis_v3p1/NoiseTermAnalysis/ 
mkdir -p ./output/JECSummer20L2Relative_2017_v1/MC/Central/SingleNeutrino_PreMix2017 
cmsenv 
set inputlists = ("SingleNeutrino_PreMix2017_21" ) 
foreach i ( $inputlists )
   mkdir -p output 
   ./NoiseTerm_Study SingleNeutrino_PreMix2017/${i}.list JECSummer20L2Relative_2017_v1/MC/Central/SingleNeutrino_PreMix2017/ ${i}.root ./configs/JECSummer20L2Relative_2017_v1/MC/Central/SingleNeutrino_PreMix2017.config 
end 
