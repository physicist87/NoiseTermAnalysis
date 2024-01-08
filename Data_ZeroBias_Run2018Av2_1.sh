#!/bin/tcsh 
mkdir -p ./output/Cone08_2018_v1/MuScale/Central/Data_ZeroBias_Run2018Av2 
cmsenv 
set inputlists = ("Data_ZeroBias_Run2018Av2_56" ) 
foreach i ( $inputlists )
   mkdir -p output 
   ./NoiseTerm_Study Data_ZeroBias_Run2018Av2/${i}.list Cone08_2018_v1/MuScale/Central/Data_ZeroBias_Run2018Av2/ ${i}.root ./configs/Cone08_2018_v1/MuScale/Central/Data_ZeroBias_Run2018Av2.config 
end 
