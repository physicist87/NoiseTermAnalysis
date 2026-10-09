#!/bin/bash
# HTCondor worker wrapper for NoiseTerm_Study (modeled on SSB run_condor_v1.sh).
# Args: <study> <config path rel. to configs/<study>/> <config dir> <sample> <list file name>
# Env : SE_HOST, SE_BASE (set by submit_jobs.py through the JDL)

STUDY="$1"; CFGREL="$2"; CFGDIR="$3"; SAMPLE="$4"; LISTNAME="$5"
OUTROOT="${LISTNAME%.list}.root"
OUTDIR="${STUDY}/${CFGDIR}/${SAMPLE}/"
SE_HOST="${SE_HOST:-root://cluster142.knu.ac.kr}"
: "${SE_BASE:?SE_BASE not set}"

export XRD_REQUESTTIMEOUT=300
export XRD_STREAMTIMEOUT=300
export XRD_CONNECTIONRETRY=5
export XRD_WORKERTHREADS=4

echo "=== [WORKER START] $(date) on $(hostname) ==="
echo "study=${STUDY} cfg=${CFGREL} sample=${SAMPLE} list=${LISTNAME}"

WORK_DIR="$(pwd)"
export SCRAM_ARCH="${SCRAM_ARCH:-el9_amd64_gcc12}"
CMSSW_VER="${CMSSW_VER:-CMSSW_15_1_0_patch4}"
source /cvmfs/cms.cern.ch/cmsset_default.sh || { echo "[ERROR] cvmfs setup failed"; exit 1; }
scramv1 project CMSSW "${CMSSW_VER}" > /dev/null || { echo "[ERROR] scram project failed"; exit 1; }
cd "${CMSSW_VER}/src" && eval "$(scramv1 runtime -sh)" && cd "${WORK_DIR}"

# Fail fast if the SE is not reachable/authenticated from this worker
# (needs use_x509userproxy = true in the JDL and a valid grid proxy at submit time).
echo "X509_USER_PROXY=${X509_USER_PROXY:-<unset>}"
xrdfs "${SE_HOST}" stat "${SE_BASE}" > /dev/null 2>&1 \
   || { echo "[ERROR] SE preflight failed (${SE_HOST}${SE_BASE}); check grid proxy"; exit 1; }

TARBALL="NoiseTermAnalysis.tar.gz"
PKG="$(tar -tzf "${TARBALL}" | head -1 | cut -d/ -f1)"
tar -xzf "${TARBALL}" || { echo "[ERROR] tar failed"; exit 1; }
cd "${PKG}" || exit 1

make -f Makefile_noiseterm clean > /dev/null 2>&1
make -f Makefile_noiseterm -j4 || { echo "[ERROR] build failed"; exit 1; }
[ -x ./NoiseTerm_Study ] || { echo "[ERROR] NoiseTerm_Study not built"; exit 1; }

mkdir -p "input/${SAMPLE}" "output/${OUTDIR}"
mv "${WORK_DIR}/${LISTNAME}" "input/${SAMPLE}/${LISTNAME}"

stdbuf -oL ./NoiseTerm_Study "${SAMPLE}/${LISTNAME}" "${OUTDIR}" "${OUTROOT}" "./configs/${STUDY}/${CFGREL}" 2>&1 | tee run_analysis.log
RC=${PIPESTATUS[0]}
echo "=== [ANALYSIS DONE] rc=${RC} $(date) ==="
[ ${RC} -eq 0 ] || exit ${RC}
# An unreadable input list/auth problem still ends with rc=0 in the binary: treat 0 events as failure
if grep -q "Total number of events after merging root files: 0$" run_analysis.log; then
   echo "=== [ANALYSIS FAILED] 0 input events read ==="
   exit 2
fi

# Output = one top-level file + one file per EtaBin dir; copy all, keep relative layout
NFAIL=0
while IFS= read -r f; do
   REL="${f#output/}"
   DEST="${SE_BASE}/${REL}"
   xrdfs "${SE_HOST}" mkdir -p "$(dirname "${DEST}")"
   xrdcp -f "${f}" "${SE_HOST}/${DEST}" && xrdfs "${SE_HOST}" stat "${DEST}" > /dev/null \
      || { echo "[ERROR] stage-out failed: ${REL}"; NFAIL=$((NFAIL+1)); }
done < <(find "output/${OUTDIR}" -name "${OUTROOT}" | sort)

[ ${NFAIL} -eq 0 ] || { echo "=== [STAGEOUT FAILED] ${NFAIL} file(s) ==="; exit 1; }
echo "=== [WORKER DONE] $(date) ==="
exit 0
