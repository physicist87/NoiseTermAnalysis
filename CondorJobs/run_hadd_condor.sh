#!/bin/bash
# HTCondor worker for run_hadd.py --condor: hadd one (config, EtaBin dir) group and copy it to the SE.
# Args: <input url list file> <SE host> <destination path on the SE>

LIST="$1"; SE_HOST="$2"; DEST="$3"

export XRD_REQUESTTIMEOUT=300
export XRD_STREAMTIMEOUT=300
export XRD_CONNECTIONRETRY=5

echo "=== [WORKER START] $(date) on $(hostname) ==="
echo "list=${LIST} ($(wc -l < "${LIST}") files) dest=${SE_HOST}/${DEST}"

WORK_DIR="$(pwd)"
export SCRAM_ARCH="${SCRAM_ARCH:-el9_amd64_gcc12}"
CMSSW_VER="${CMSSW_VER:-CMSSW_15_1_0_patch4}"
source /cvmfs/cms.cern.ch/cmsset_default.sh || { echo "[ERROR] cvmfs setup failed"; exit 1; }
scramv1 project CMSSW "${CMSSW_VER}" > /dev/null || { echo "[ERROR] scram project failed"; exit 1; }
cd "${CMSSW_VER}/src" && eval "$(scramv1 runtime -sh)" && cd "${WORK_DIR}"

hadd -f -n 100 merged.root $(cat "${LIST}") > hadd.log 2>&1
RC=$?
tail -n 3 hadd.log
[ ${RC} -eq 0 ] && [ -s merged.root ] || { echo "[ERROR] hadd failed rc=${RC}"; exit 1; }

xrdfs "${SE_HOST}" mkdir -p "$(dirname "${DEST}")"
xrdcp -f merged.root "${SE_HOST}/${DEST}" && xrdfs "${SE_HOST}" stat "${DEST}" > /dev/null \
   || { echo "[ERROR] stage-out failed"; exit 1; }
rm -f merged.root
echo "=== [WORKER DONE] $(date) ==="
exit 0
