#!/bin/bash
# Run NoiseTerm_Study over all Run3 2025 PUPPI "Central" (nominal) samples.
#
# Follows the same argv convention as the old run scripts (e.g.
# SingleNeutrino_Flat2018_1.sh, Data_ZeroBias_Run2018Av2_1.sh):
#   ./NoiseTerm_Study <input list, relative to ./input/> \
#                      <output subdir, relative to ./output/, trailing slash> \
#                      <output root filename> \
#                      <config file path>
#
# main_noiseterm.cpp derives the random-seed index from the list filename
# itself (text after the last "_", with ".list" stripped) - so each .list
# file under input/<Sample>/ must be named <Sample>_<N>.list, matching what
# was already set up under input/.
#
# Run from anywhere - this script cd's to its own directory first.

set -e
cd "$(dirname "$0")"

TAG="Run3_2025_PUPPI_v1"
VARIATION="Central"

# Sample names = the input/<Sample>/ directory names = the
# configs/${TAG}/${VARIATION}/<Sample>.config basenames.
SAMPLES=(
   "Data_ZeroBias_Run2025Cv1"
   "Data_ZeroBias_Run2025Dv1"
   "SingleNeutrino_Flat2025"
)

for SAMPLE in "${SAMPLES[@]}"; do
   CONFIG="./configs/${TAG}/${VARIATION}/${SAMPLE}.config"
   OUTDIR="${TAG}/${VARIATION}/${SAMPLE}/"

   if [ ! -f "$CONFIG" ]; then
      echo "!! skipping ${SAMPLE}: config not found at ${CONFIG}"
      continue
   fi

   mkdir -p "./output/${OUTDIR}"

   shopt -s nullglob
   LISTS=(./input/${SAMPLE}/${SAMPLE}_*.list)
   shopt -u nullglob

   if [ ${#LISTS[@]} -eq 0 ]; then
      echo "!! skipping ${SAMPLE}: no input/${SAMPLE}/${SAMPLE}_*.list found"
      continue
   fi

   for LISTPATH in "${LISTS[@]}"; do
      LISTNAME=$(basename "$LISTPATH")           # e.g. Data_ZeroBias_Run2025Cv1_1.list
      OUTROOT="${LISTNAME%.list}.root"            # e.g. Data_ZeroBias_Run2025Cv1_1.root
      echo "=== ${SAMPLE} : ${LISTNAME} ==="
      ./NoiseTerm_Study "${SAMPLE}/${LISTNAME}" "${OUTDIR}" "${OUTROOT}" "${CONFIG}"
   done
done

echo ""
echo "Done. Output under ./output/${TAG}/${VARIATION}/<Sample>/"
