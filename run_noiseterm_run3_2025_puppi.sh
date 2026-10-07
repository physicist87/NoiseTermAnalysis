#!/bin/bash
# Run NoiseTerm_Study over the Run3 2025 PUPPI "Central" (nominal) samples.
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
# file under input/<InputSample>/ must be named <InputSample>_<N>.list.
#
# Layout (matches the Cone08_2018_v1 precedent):
#   configs/<TAG>/Central/<DataSample>.config          Data, one per run period
#   configs/<TAG>/MC/Central/<MCSample>.config         MC, nominal (full-Run merged PU target)
#   configs/<TAG>/PURun<Era>/Central/<MCSample>.config MC, PU target = one run period only
#
# input/ directories keep the CRAB-version names used by the server input
# lists (Cv1, Cv2, Fv1, Fv2, ...), while configs are per run period, so the
# ENTRIES table below maps each config to the input directories it runs on.
# The output directory mirrors the config directory plus the input sample:
#   output/<TAG>/<config dir>/<InputSample>/
# (so Cv1 and Cv2 stay separate and can be hadd-ed per run period later).
#
# Usage:
#   ./run_noiseterm_run3_2025_puppi.sh                run everything
#   ./run_noiseterm_run3_2025_puppi.sh data           all Data run periods
#   ./run_noiseterm_run3_2025_puppi.sh mc             nominal MC only
#   ./run_noiseterm_run3_2025_puppi.sh pu             PURunC..G MC variants
#   ./run_noiseterm_run3_2025_puppi.sh C F PURunD     any mix of KEY / group names
#   DRYRUN=1 ./run_noiseterm_run3_2025_puppi.sh ...   print what would run, run nothing
#
# Run from anywhere - this script cd's to its own directory first.

set -e
cd "$(dirname "$0")"

TAG="Run3_2025_PUPPI_v1"
EXE="./NoiseTerm_Study"

# KEY | GROUP | config path (relative to configs/${TAG}/) | input samples (comma separated)
ENTRIES=(
   "C|data|Central/Data_ZeroBias_Run2025C.config|Data_ZeroBias_Run2025Cv1,Data_ZeroBias_Run2025Cv2"
   "Dv1|data|Central/Data_ZeroBias_Run2025Dv1.config|Data_ZeroBias_Run2025Dv1"
   "Ev1|data|Central/Data_ZeroBias_Run2025Ev1.config|Data_ZeroBias_Run2025Ev1"
   "F|data|Central/Data_ZeroBias_Run2025F.config|Data_ZeroBias_Run2025Fv1,Data_ZeroBias_Run2025Fv2"
   "Gv1|data|Central/Data_ZeroBias_Run2025Gv1.config|Data_ZeroBias_Run2025Gv1"
   "MC|mc|MC/Central/SingleNeutrino_Flat2025.config|SingleNeutrino_Flat2025"
   "PURunC|pu|PURunC/Central/SingleNeutrino_Flat2025.config|SingleNeutrino_Flat2025"
   "PURunD|pu|PURunD/Central/SingleNeutrino_Flat2025.config|SingleNeutrino_Flat2025"
   "PURunE|pu|PURunE/Central/SingleNeutrino_Flat2025.config|SingleNeutrino_Flat2025"
   "PURunF|pu|PURunF/Central/SingleNeutrino_Flat2025.config|SingleNeutrino_Flat2025"
   "PURunG|pu|PURunG/Central/SingleNeutrino_Flat2025.config|SingleNeutrino_Flat2025"
)

SELECTORS=("$@")
if [ ${#SELECTORS[@]} -eq 0 ]; then
   SELECTORS=("all")
fi

selected() {
   local key="$1" group="$2" sel
   for sel in "${SELECTORS[@]}"; do
      if [ "$sel" = "all" ] || [ "$sel" = "$key" ] || [ "$sel" = "$group" ]; then
         return 0
      fi
   done
   return 1
}

if [ -z "$DRYRUN" ] && [ ! -x "$EXE" ]; then
   echo "!! ${EXE} not found or not executable - build it first (make -f Makefile_noiseterm)"
   exit 1
fi

NRUN=0
NSKIP=0

for ENTRY in "${ENTRIES[@]}"; do
   IFS='|' read -r KEY GROUP CFGREL INPUTS <<< "$ENTRY"

   if ! selected "$KEY" "$GROUP"; then
      continue
   fi

   CONFIG="./configs/${TAG}/${CFGREL}"
   CFGDIR=$(dirname "$CFGREL")                    # e.g. Central, MC/Central, PURunC/Central

   if [ ! -f "$CONFIG" ]; then
      echo "!! skipping ${KEY}: config not found at ${CONFIG}"
      NSKIP=$((NSKIP + 1))
      continue
   fi

   for SAMPLE in ${INPUTS//,/ }; do
      OUTDIR="${TAG}/${CFGDIR}/${SAMPLE}/"

      shopt -s nullglob
      LISTS=(./input/${SAMPLE}/${SAMPLE}_*.list)
      shopt -u nullglob

      if [ ${#LISTS[@]} -eq 0 ]; then
         echo "!! skipping ${KEY} / ${SAMPLE}: no input/${SAMPLE}/${SAMPLE}_*.list found"
         NSKIP=$((NSKIP + 1))
         continue
      fi

      if [ -z "$DRYRUN" ]; then
         mkdir -p "./output/${OUTDIR}"
      fi

      for LISTPATH in "${LISTS[@]}"; do
         LISTNAME=$(basename "$LISTPATH")          # e.g. Data_ZeroBias_Run2025Cv1_1.list
         OUTROOT="${LISTNAME%.list}.root"           # e.g. Data_ZeroBias_Run2025Cv1_1.root
         echo "=== ${KEY} : ${SAMPLE} : ${LISTNAME} ==="
         if [ -n "$DRYRUN" ]; then
            echo "    ${EXE} ${SAMPLE}/${LISTNAME} ${OUTDIR} ${OUTROOT} ${CONFIG}"
         else
            ${EXE} "${SAMPLE}/${LISTNAME}" "${OUTDIR}" "${OUTROOT}" "${CONFIG}"
         fi
         NRUN=$((NRUN + 1))
      done
   done
done

echo ""
echo "Done. ${NRUN} job(s) run, ${NSKIP} skipped. Output under ./output/${TAG}/<config dir>/<InputSample>/"
