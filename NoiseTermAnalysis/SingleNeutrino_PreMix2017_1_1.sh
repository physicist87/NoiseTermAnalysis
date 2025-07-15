#!/bin/bash
# Load the necessary LCG environment
source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh

# Preload the library you need
export LD_PRELOAD="/usr/lib64/libpdcap.so"

# Create necessary directories
mkdir -p ./output/JECSummer20L2Relative_2017_v1/MC/Central/SingleNeutrino_PreMix2017

# Initialize the CMS environment
#eval `scramv1 runtime -sh`

# Input list array
inputlists=("SingleNeutrino_PreMix2017_21")

# Loop over input lists
for i in "${inputlists[@]}"
do
   mkdir -p output
   ./NoiseTerm_Study SingleNeutrino_PreMix2017/${i}.list \
      JECSummer20L2Relative_2017_v1/MC/Central/SingleNeutrino_PreMix2017/ \
      ${i}.root \
      ./configs/JECSummer20L2Relative_2017_v1/MC/Central/SingleNeutrino_PreMix2017.config
done
 
