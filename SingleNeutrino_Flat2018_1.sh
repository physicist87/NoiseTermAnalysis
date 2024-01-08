#!/bin/tcsh 
mkdir -p ./output/Cone08_2018_v1/PURunA/Central/SingleNeutrino_Flat2018 
cmsenv 
set inputlists = ("SingleNeutrino_Flat2018_30" ) 
foreach i ( $inputlists )
   mkdir -p output 
   ./NoiseTerm_Study SingleNeutrino_Flat2018/${i}.list Cone08_2018_v1/PURunA/Central/SingleNeutrino_Flat2018/ ${i}.root ./configs/Cone08_2018_v1/PURunA/Central/SingleNeutrino_Flat2018.config 
end 
