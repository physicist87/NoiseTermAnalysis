#!/bin/bash
# HTCondor worker for check_merged.py --condor: check one (key, sub) merged file against its inputs.
# Args: <study> <KEY> <sub> <SE host> <SE base>

STUDY="$1"; KEY="$2"; SUB="$3"; SE_HOST="$4"; SE_BASE="$5"

export XRD_REQUESTTIMEOUT=300
export XRD_STREAMTIMEOUT=300
export XRD_CONNECTIONRETRY=5

echo "=== [WORKER START] $(date) on $(hostname) ==="
WORK_DIR="$(pwd)"
export SCRAM_ARCH="${SCRAM_ARCH:-el9_amd64_gcc12}"
CMSSW_VER="${CMSSW_VER:-CMSSW_15_1_0_patch4}"
source /cvmfs/cms.cern.ch/cmsset_default.sh || { echo "[ERROR] cvmfs setup failed"; exit 1; }
scramv1 project CMSSW "${CMSSW_VER}" > /dev/null || { echo "[ERROR] scram project failed"; exit 1; }
cd "${CMSSW_VER}/src" && eval "$(scramv1 runtime -sh)" && cd "${WORK_DIR}"

python3 check_merged.py --study "${STUDY}" --samples "${KEY}" --subs "${SUB}" \
   --se-host "${SE_HOST}" --se-base "${SE_BASE}" --jobs 2 --local
RC=$?
echo "=== [CHECK DONE] rc=${RC} $(date) ==="
exit ${RC}
