#!/usr/bin/env python3
"""Check NoiseTerm Condor outputs on the SE against the input lists.

A list counts as complete when <list stem>.root exists in the sample dir AND in
every EtaBin_* dir below it (a job that died during stage-out leaves a partial set).

Examples:
  python3 CondorJobs/check_jobs.py --input-base $INPUT --samples C
  python3 CondorJobs/check_jobs.py --input-base $INPUT --samples all --skip-queued --write-bad CondorJobs/bad_jobs_Run3_2025_PUPPI_v1.txt
  python3 CondorJobs/submit_jobs.py --bad-jobs CondorJobs/bad_jobs_Run3_2025_PUPPI_v1.txt
"""
import argparse
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from submit_jobs import ENTRIES, DEFAULT_TAG, DEFAULT_SE_HOST, DEFAULT_SE_BASE, list_sort_key  # noqa: E402


def xrd_ls(host, path):
    """Return names (basename) listed under path; empty set if the dir does not exist."""
    r = subprocess.run(["xrdfs", host, "ls", path], capture_output=True, text=True)
    if r.returncode != 0:
        return None
    return [line.rsplit("/", 1)[-1] for line in r.stdout.split() if line.strip()]


def queued_set():
    """(cfgrel, listname) of jobs still in the queue (idle/running/held)."""
    r = subprocess.run(["condor_q", "-allusers", "-af", "Arguments"], capture_output=True, text=True)
    out = set()
    for line in r.stdout.splitlines():
        t = line.strip().strip('"').split()
        if len(t) >= 5:
            out.add((t[1], t[4]))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--study", default=DEFAULT_TAG)
    ap.add_argument("--input-base", required=True)
    ap.add_argument("--samples", nargs="+", default=["all"], help="all | data | mc | pu | KEY ...")
    ap.add_argument("--se-host", default=os.environ.get("SE_HOST", DEFAULT_SE_HOST))
    ap.add_argument("--se-base", default=os.environ.get("SE_BASE", DEFAULT_SE_BASE))
    ap.add_argument("--skip-queued", action="store_true", help="do not report lists that are still in the condor queue")
    ap.add_argument("--write-bad", help="write KEY|SAMPLE|LISTPATH lines for submit_jobs.py --bad-jobs")
    ap.add_argument("--show", type=int, default=10, help="max bad list names printed per sample")
    args = ap.parse_args()

    queued = queued_set() if args.skip_queued else set()
    base = Path(args.input_base)
    bad_lines, tot_exp, tot_ok = [], 0, 0

    print(f"{'key':8s} {'sample':28s} {'expected':>8s} {'complete':>8s} {'missing':>7s} {'partial':>7s} {'queued':>6s}")
    for key, group, cfgrel, samples in ENTRIES:
        if not any(s in ("all", key, group) for s in args.samples):
            continue
        cfgdir = os.path.dirname(cfgrel)
        for sample in samples:
            lists = sorted((base / sample).glob(f"{sample}_*.list"), key=list_sort_key)
            if not lists:
                continue
            sdir = f"{args.se_base}/{args.study}/{cfgdir}/{sample}"
            top = xrd_ls(args.se_host, sdir)
            top = top or []
            etadirs = [n for n in top if n.startswith("EtaBin_")]
            have_top = {n for n in top if n.endswith(".root")}
            per_eta = {d: set(xrd_ls(args.se_host, f"{sdir}/{d}") or []) for d in etadirs}

            n_ok = n_miss = n_part = n_q = 0
            bad_names = []
            for lp in lists:
                fn = lp.stem + ".root"
                if fn in have_top and etadirs and all(fn in per_eta[d] for d in etadirs) and len(etadirs) == 19:
                    n_ok += 1
                    continue
                if args.skip_queued and (cfgrel, lp.name) in queued:
                    n_q += 1
                    continue
                if fn in have_top or any(fn in per_eta[d] for d in etadirs):
                    n_part += 1
                else:
                    n_miss += 1
                bad_names.append(lp.name)
                bad_lines.append(f"{key}|{sample}|{lp}")
            tot_exp += len(lists)
            tot_ok += n_ok
            print(f"{key:8s} {sample:28s} {len(lists):8d} {n_ok:8d} {n_miss:7d} {n_part:7d} {n_q:6d}")
            if bad_names:
                shown = ", ".join(bad_names[:args.show]) + (" ..." if len(bad_names) > args.show else "")
                print(f"           bad: {shown}")

    print(f"TOTAL expected={tot_exp} complete={tot_ok} not complete={tot_exp - tot_ok} (to resubmit: {len(bad_lines)})")
    if args.write_bad:
        Path(args.write_bad).write_text("\n".join(bad_lines) + ("\n" if bad_lines else ""))
        print(f"wrote {len(bad_lines)} line(s) to {args.write_bad}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
