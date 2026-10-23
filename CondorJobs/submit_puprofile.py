#!/usr/bin/env python3
"""Submit PUProfile (prescale-weighted data mu profile) to HTCondor.

The data samples and input lists are those of submit_jobs.py (ENTRIES, group "data").
Each job runs PUProfile on --lists-per-job input lists; the small output ROOT file
comes back by Condor file transfer to CondorJobs/puprofile/<tag>/<KEY>/.
Merge afterwards with --merge (hadd per KEY, Cv1+Cv2 and Fv1+Fv2 together) into
CondorJobs/puprofile/<tag>/DataMuPS_<config stem>.root.

Examples:
  python3 CondorJobs/submit_puprofile.py --input-base $INPUT --dry-run
  python3 CondorJobs/submit_puprofile.py --input-base $INPUT --samples C --test
  python3 CondorJobs/submit_puprofile.py --input-base $INPUT
  python3 CondorJobs/submit_puprofile.py --merge
"""
import argparse
import subprocess
import sys
from pathlib import Path

from submit_jobs import ENTRIES, PKG_DIR, PKG_NAME, CONDOR_DIR, list_sort_key

RUN_SCRIPT = CONDOR_DIR / "run_puprofile.sh"
TARBALL = CONDOR_DIR / "PUProfile.tar.gz"
DEFAULT_TAG = "DataMuPS_v1"


def build_tarball():
    # Only the PUProfile sources: separate from NoiseTermAnalysis.tar.gz, which queued NoiseTerm jobs still read
    cmd = ["tar", "-czf", str(TARBALL), "--exclude=*.o", f"--exclude={PKG_NAME}/PUProfile/PUProfile",
           "--exclude=*.root", f"{PKG_NAME}/PUProfile"]
    subprocess.run(cmd, cwd=PKG_DIR.parent, check=True)
    print(f"[tarball] {TARBALL} ({TARBALL.stat().st_size / 1e3:.1f} kB)")


def out_stem(cfgrel):
    return "DataMuPS_" + Path(cfgrel).stem


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--tag", default=DEFAULT_TAG, help="output sub-directory name")
    ap.add_argument("--input-base", help="InputList dir containing <sample>/<sample>_N.list")
    ap.add_argument("--samples", nargs="+", default=["data"], help="data | KEY ... (data keys only)")
    ap.add_argument("--trigger", default="HLT_ZeroBias")
    ap.add_argument("--lists-per-job", type=int, default=10)
    ap.add_argument("--test", action="store_true", help="one job per sample")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--merge", action="store_true", help="hadd the returned files per KEY")
    args = ap.parse_args()

    top = CONDOR_DIR / "puprofile" / args.tag
    entries = [e for e in ENTRIES if e[1] == "data" and any(s in ("data", e[0]) for s in args.samples)]

    if args.merge:
        for key, _, cfgrel, _ in entries:
            parts = sorted((top / key).glob("*.root"))
            target = top / f"{out_stem(cfgrel)}.root"
            print(f"[{key}] {len(parts)} file(s) -> {target}")
            if parts:
                subprocess.run(["hadd", "-f", str(target)] + [str(p) for p in parts], check=True,
                               stdout=subprocess.DEVNULL)
        return 0

    if not args.input_base:
        ap.error("--input-base is required")
    base = Path(args.input_base)
    build_tarball()
    njobs = 0
    for key, _, cfgrel, samples in entries:
        (top / key).mkdir(parents=True, exist_ok=True)
        log_dir = CONDOR_DIR / "condorLog" / "PUProfile" / args.tag / key
        sub_dir = CONDOR_DIR / "condorSubmit" / "PUProfile" / args.tag
        log_dir.mkdir(parents=True, exist_ok=True)
        sub_dir.mkdir(parents=True, exist_ok=True)
        queue = sub_dir / f"{key}.queue"
        n = 0
        with open(queue, "w") as q:
            for sample in samples:
                lists = sorted((base / sample).glob(f"{sample}_*.list"), key=list_sort_key)
                if not lists:
                    print(f"!! skip {key}/{sample}: no lists under {base / sample}")
                    continue
                chunks = [lists[i:i + args.lists_per_job] for i in range(0, len(lists), args.lists_per_job)]
                if args.test:
                    chunks = chunks[:1]
                for i, ch in enumerate(chunks):
                    # one merged list per job, so the queue file needs no comma-separated path lists
                    lp = sub_dir / key / f"{sample}_{i}.list"
                    lp.parent.mkdir(parents=True, exist_ok=True)
                    lp.write_text("".join(c.read_text() if c.read_text().endswith("\n") else c.read_text() + "\n" for c in ch))
                    out = f"{out_stem(cfgrel)}_{sample}_{i}.root"
                    q.write(f"{sample}_{i} {out} {lp} {lp.name}\n")
                    n += 1
        jdl = sub_dir / f"{key}.jdl"
        with open(jdl, "w") as f:
            f.write(
                "Universe   = vanilla\n"
                f"Executable = {RUN_SCRIPT}\n"
                f"Arguments  = \"$(OutName) {args.trigger} $(ListName)\"\n"
                "getenv     = False\n"
                "use_x509userproxy      = true\n"
                "should_transfer_files   = YES\n"
                "when_to_transfer_output = ON_EXIT\n"
                f"transfer_input_files    = {TARBALL},$(ListPath)\n"
                "transfer_output_files   = $(OutName)\n"
                f"transfer_output_remaps  = \"$(OutName) = {top / key}/$(OutName)\"\n"
                f"Log    = {log_dir}/$(JobId).log\n"
                f"Output = {log_dir}/$(JobId).out\n"
                f"Error  = {log_dir}/$(JobId).err\n"
                "RequestCpus   = 1\n"
                "RequestMemory = 2 GB\n"
                "RequestDisk   = 2 GB\n"
                f"batch_name    = PUProfile_{args.tag}_{key}{'_test' if args.test else ''}\n"
                f"Queue JobId, OutName, ListPath, ListName from {queue}\n"
            )
        print(f"[{key}] {n} job(s)  {jdl}")
        njobs += n
        if not args.dry_run:
            subprocess.run(["condor_submit", str(jdl)], check=True)
    print(f"{'Prepared' if args.dry_run else 'Submitted'} {njobs} job(s).  Output: {top}/<KEY>/, then --merge")
    return 0


if __name__ == "__main__":
    sys.exit(main())
