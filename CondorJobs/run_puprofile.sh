#!/bin/bash
# HTCondor worker wrapper for PUProfile (data PU profile, prescale-weighted).
# Args: <output root name> <trigger> <list file name>
# The output ROOT file is returned to the submit node by Condor file transfer.

OUTROOT="$1"; TRIG="$2"; shift 2

export XRD_REQUESTTIMEOUT=300
export XRD_STREAMTIMEOUT=300
export XRD_CONNECTIONRETRY=5

echo "=== [WORKER START] $(date) on $(hostname) ==="
echo "out=${OUTROOT} trigger=${TRIG} lists=$*"

WORK_DIR="$(pwd)"
export SCRAM_ARCH="${SCRAM_ARCH:-el9_amd64_gcc12}"
CMSSW_VER="${CMSSW_VER:-CMSSW_15_1_0_patch4}"
source /cvmfs/cms.cern.ch/cmsset_default.sh || { echo "[ERROR] cvmfs setup failed"; exit 1; }
scramv1 project CMSSW "${CMSSW_VER}" > /dev/null || { echo "[ERROR] scram project failed"; exit 1; }
cd "${CMSSW_VER}/src" && eval "$(scramv1 runtime -sh)" && cd "${WORK_DIR}"
echo "X509_USER_PROXY=${X509_USER_PROXY:-<unset>}"

TARBALL="PUProfile.tar.gz"
PKG="$(tar -tzf "${TARBALL}" | head -1 | cut -d/ -f1)"
tar -xzf "${TARBALL}" || { echo "[ERROR] tar failed"; exit 1; }
( cd "${PKG}/PUProfile" && make -f Makefile_puprofile > /dev/null ) || { echo "[ERROR] build failed"; exit 1; }

cat "$@" > merged.list
stdbuf -oL "./${PKG}/PUProfile/PUProfile" merged.list "${OUTROOT}" "${TRIG}" 2>&1 | grep -v "^Added:" | tee run_puprofile.log
RC=${PIPESTATUS[0]}
echo "=== [PUPROFILE DONE] rc=${RC} $(date) ==="
[ ${RC} -eq 0 ] || exit ${RC}
# Unreadable inputs end with 0 events: treat as failure so the job can be resubmitted
grep -q "^Total events in chain: [1-9]" run_puprofile.log || { echo "[ERROR] 0 input events"; exit 2; }
[ -s "${OUTROOT}" ] || { echo "[ERROR] no output ${OUTROOT}"; exit 3; }
echo "=== [WORKER DONE] $(date) ==="
exit 0
