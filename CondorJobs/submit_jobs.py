#!/usr/bin/env python3
"""Submit NoiseTerm_Study jobs to HTCondor (modeled on SSBNanoAODANCode CondorJobs).

One job per input list.  Each worker unpacks the package tarball, builds
NoiseTerm_Study, runs one list, and copies every output ROOT file to the SE.

Examples:
  python3 CondorJobs/submit_jobs.py --input-base $INPUT --samples all --dry-run
  python3 CondorJobs/submit_jobs.py --input-base $INPUT --samples C Dv1 MC --test
  python3 CondorJobs/submit_jobs.py --bad-jobs CondorJobs/bad_jobs_Run3_2025_PUPPI_v1.txt
"""
import argparse
import os
import subprocess
import sys
from pathlib import Path

PKG_DIR = Path(__file__).resolve().parent.parent
PKG_NAME = PKG_DIR.name
CONDOR_DIR = PKG_DIR / "CondorJobs"
RUN_SCRIPT = CONDOR_DIR / "run_condor.sh"
TARBALL = CONDOR_DIR / "NoiseTermAnalysis.tar.gz"

DEFAULT_TAG = "Run3_2025_PUPPI_v1"
DEFAULT_SE_HOST = "root://cluster142.knu.ac.kr"
DEFAULT_SE_BASE = "/store/user/sha/JERNoiseStudy/Run3/2025/NoiseTerm"

# KEY, GROUP, config path (relative to configs/<TAG>/), input samples
# Keep in sync with run_noiseterm_run3_2025_puppi.sh
ENTRIES = [
    ("C",      "data", "Central/Data_ZeroBias_Run2025C.config",  ["Data_ZeroBias_Run2025Cv1", "Data_ZeroBias_Run2025Cv2"]),
    ("Dv1",    "data", "Central/Data_ZeroBias_Run2025Dv1.config", ["Data_ZeroBias_Run2025Dv1"]),
    ("Ev1",    "data", "Central/Data_ZeroBias_Run2025Ev1.config", ["Data_ZeroBias_Run2025Ev1"]),
    ("F",      "data", "Central/Data_ZeroBias_Run2025F.config",  ["Data_ZeroBias_Run2025Fv1", "Data_ZeroBias_Run2025Fv2"]),
    ("Gv1",    "data", "Central/Data_ZeroBias_Run2025Gv1.config", ["Data_ZeroBias_Run2025Gv1"]),
    ("MC",     "mc",   "MC/Central/SingleNeutrino_Flat2025.config", ["SingleNeutrino_Flat2025"]),
    ("PURunC", "pu",   "PURunC/Central/SingleNeutrino_Flat2025.config", ["SingleNeutrino_Flat2025"]),
    ("PURunD", "pu",   "PURunD/Central/SingleNeutrino_Flat2025.config", ["SingleNeutrino_Flat2025"]),
    ("PURunE", "pu",   "PURunE/Central/SingleNeutrino_Flat2025.config", ["SingleNeutrino_Flat2025"]),
    ("PURunF", "pu",   "PURunF/Central/SingleNeutrino_Flat2025.config", ["SingleNeutrino_Flat2025"]),
    ("PURunG", "pu",   "PURunG/Central/SingleNeutrino_Flat2025.config", ["SingleNeutrino_Flat2025"]),
]


def list_sort_key(p):
    stem = p.stem
    tail = stem.rsplit("_", 1)[-1]
    return (int(tail) if tail.isdigit() else 10**9, stem)


def build_tarball():
    excludes = ["input", "output", "CondorJobs", ".git"]
    cmd = ["tar", "-czf", str(TARBALL)]
    cmd += [f"--exclude={PKG_NAME}/{e}" for e in excludes]
    cmd += ["--exclude=*.o", "--exclude=NoiseTerm_Study", "--exclude=PUProfile/PUProfile",
            "--exclude=*.zip", "--exclude=.DS_Store", "--exclude=__pycache__",
            "--exclude=*.pyc", "--exclude=Offset_MC_*.root", PKG_NAME]
    subprocess.run(cmd, cwd=PKG_DIR.parent, check=True)
    print(f"[tarball] {TARBALL} ({TARBALL.stat().st_size / 1e6:.1f} MB)")


def write_jdl(args, key, cfgrel, sample, jobs):
    cfgdir = os.path.dirname(cfgrel)
    sub_dir = CONDOR_DIR / "condorSubmit" / args.study / key
    log_dir = CONDOR_DIR / "condorLog" / args.study / key / sample
    sub_dir.mkdir(parents=True, exist_ok=True)
    log_dir.mkdir(parents=True, exist_ok=True)

    queue = sub_dir / f"{sample}.queue"
    with open(queue, "w") as f:
        for lp in jobs:
            f.write(f"{lp.stem}, {lp}, {lp.name}\n")

    jdl = sub_dir / f"{sample}.jdl"
    with open(jdl, "w") as f:
        f.write(
            "Universe   = vanilla\n"
            f"Executable = {RUN_SCRIPT}\n"
            f"Arguments  = \"{args.study} {cfgrel} {cfgdir} {sample} $(InputListName)\"\n"
            "getenv     = False\n"
            f"environment = \"SE_HOST={args.se_host} SE_BASE={args.se_base}\"\n"
            "use_x509userproxy      = true\n"
            "should_transfer_files   = YES\n"
            "when_to_transfer_output = ON_EXIT\n"
            f"transfer_input_files    = {TARBALL},$(InputListPath)\n"
            "transfer_output_files   = \"\"\n"
            f"Log    = {log_dir}/{sample}_$(JobId).log\n"
            f"Output = {log_dir}/{sample}_$(JobId).out\n"
            f"Error  = {log_dir}/{sample}_$(JobId).err\n"
            "RequestCpus   = 1\n"
            "RequestMemory = 8 GB\n"
            "RequestDisk   = 10 GB\n"
            "+JobType      = \"long\"\n"
            f"Queue JobId, InputListPath, InputListName from {queue}\n"
        )
    return jdl


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--study", default=DEFAULT_TAG, help="config tag / output study name")
    ap.add_argument("--input-base", help="InputList dir containing <sample>/<sample>_N.list")
    ap.add_argument("--samples", nargs="+", default=["all"], help="all | data | mc | pu | KEY ...")
    ap.add_argument("--bad-jobs", help="resubmit lines (KEY|SAMPLE|LISTPATH) from check_jobs.py --write-bad")
    ap.add_argument("--se-host", default=os.environ.get("SE_HOST", DEFAULT_SE_HOST))
    ap.add_argument("--se-base", default=os.environ.get("SE_BASE", DEFAULT_SE_BASE))
    ap.add_argument("--test", action="store_true", help="first list of each sample only")
    ap.add_argument("--dry-run", action="store_true", help="build tarball and JDL, do not submit")
    args = ap.parse_args()

    by_key = {e[0]: e for e in ENTRIES}
    todo = {}   # (key, sample) -> [list paths]

    if args.bad_jobs:
        for line in Path(args.bad_jobs).read_text().splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            key, sample, lp = line.split("|")
            todo.setdefault((key, sample), []).append(Path(lp))
    else:
        if not args.input_base:
            ap.error("--input-base is required (or use --bad-jobs)")
        base = Path(args.input_base)
        for key, group, cfgrel, samples in ENTRIES:
            if not any(s in ("all", key, group) for s in args.samples):
                continue
            for sample in samples:
                lists = sorted((base / sample).glob(f"{sample}_*.list"), key=list_sort_key)
                if not lists:
                    print(f"!! skip {key}/{sample}: no {sample}_*.list under {base / sample}")
                    continue
                todo[(key, sample)] = lists[:1] if args.test else lists

    if not todo:
        print("Nothing to submit.")
        return 1

    build_tarball()

    njobs = 0
    for (key, sample), jobs in todo.items():
        cfgrel = by_key[key][2]
        if not (PKG_DIR / "configs" / args.study / cfgrel).is_file():
            print(f"!! skip {key}: config not found configs/{args.study}/{cfgrel}")
            continue
        jdl = write_jdl(args, key, cfgrel, sample, jobs)
        print(f"[{key}] {sample}: {len(jobs)} job(s)  {jdl}")
        njobs += len(jobs)
        if not args.dry_run:
            subprocess.run(["condor_submit", str(jdl)], check=True)

    print(f"{'Prepared' if args.dry_run else 'Submitted'} {njobs} job(s).  SE: {args.se_host}{args.se_base}/{args.study}/...")
    return 0


if __name__ == "__main__":
    sys.exit(main())
