#!/usr/bin/env python3
"""hadd the per-list NoiseTerm outputs on the SE: one file per (config, EtaBin dir).

Input : <se-base>/<study>/<cfgdir>/<sample>/[EtaBin_*/]<list>.root   (samples of a key are merged together)
Output: <se-base>/<study>_Merged/<cfgdir>/<stem>/[EtaBin_*/]<stem>.root   (stem = config file name without .config)

Needs ROOT (run after cmsenv) and a valid grid proxy.

Examples:
  python3 CondorJobs/run_hadd.py --samples C --dry-run
  python3 CondorJobs/run_hadd.py --samples C --jobs 6
  python3 CondorJobs/run_hadd.py --samples all --jobs 8
"""
import argparse
import os
import shutil
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from submit_jobs import ENTRIES, DEFAULT_TAG, DEFAULT_SE_HOST, DEFAULT_SE_BASE  # noqa: E402


def xrd_ls(host, path):
    r = subprocess.run(["xrdfs", host, "ls", path], capture_output=True, text=True)
    if r.returncode != 0:
        return None
    return sorted(line.rsplit("/", 1)[-1] for line in r.stdout.split() if line.strip())


def xrd_exists(host, path):
    return subprocess.run(["xrdfs", host, "stat", path], capture_output=True).returncode == 0


def collect_inputs(host, se_base, study, cfgdir, samples):
    """{sub: [root:// urls]} where sub is '' (top-level files) or an EtaBin_* dir name."""
    groups = {}
    for sample in samples:
        sdir = f"{se_base}/{study}/{cfgdir}/{sample}"
        names = xrd_ls(host, sdir)
        if names is None:
            print(f"!! {sdir}: not found on SE")
            continue
        for n in names:
            if n.endswith(".root"):
                groups.setdefault("", []).append(f"{host}/{sdir}/{n}")
            elif n.startswith("EtaBin_"):
                for f in xrd_ls(host, f"{sdir}/{n}") or []:
                    if f.endswith(".root"):
                        groups.setdefault(n, []).append(f"{host}/{sdir}/{n}/{f}")
    return groups


def merge_one(host, se_base, study, cfgdir, stem, sub, urls, tmpdir, overwrite):
    dest_dir = f"{se_base}/{study}_Merged/{cfgdir}/{stem}" + (f"/{sub}" if sub else "")
    dest = f"{dest_dir}/{stem}.root"
    tag = f"{cfgdir.replace('/', '_')}_{stem}_{sub or 'top'}"
    if not overwrite and xrd_exists(host, dest):
        return tag, "skip (exists)", len(urls)
    local = Path(tmpdir) / f"{tag}.root"
    r = subprocess.run(["hadd", "-f", "-n", "100", str(local)] + urls, capture_output=True, text=True)
    if r.returncode != 0 or not local.is_file():
        tail = "\n".join((r.stdout + r.stderr).splitlines()[-5:])
        return tag, f"HADD FAILED rc={r.returncode}\n{tail}", len(urls)
    subprocess.run(["xrdfs", host, "mkdir", "-p", dest_dir], capture_output=True)
    c = subprocess.run(["xrdcp", "-f", str(local), f"{host}/{dest}"], capture_output=True, text=True)
    ok = c.returncode == 0 and xrd_exists(host, dest)
    size = local.stat().st_size / 1e6
    local.unlink()
    return tag, (f"ok {size:.1f} MB" if ok else f"STAGEOUT FAILED {c.stderr.strip()[-200:]}"), len(urls)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--study", default=DEFAULT_TAG)
    ap.add_argument("--samples", nargs="+", default=["all"], help="all | data | mc | pu | KEY ...")
    ap.add_argument("--se-host", default=os.environ.get("SE_HOST", DEFAULT_SE_HOST))
    ap.add_argument("--se-base", default=os.environ.get("SE_BASE", DEFAULT_SE_BASE))
    ap.add_argument("--jobs", type=int, default=6, help="parallel hadd processes")
    ap.add_argument("--tmpdir", default=os.environ.get("TMPDIR", "/tmp"), help="local scratch for merged files")
    ap.add_argument("--overwrite", action="store_true", help="redo merges whose output already exists")
    ap.add_argument("--dry-run", action="store_true", help="only list what would be merged")
    args = ap.parse_args()

    if not args.dry_run and shutil.which("hadd") is None:
        sys.exit("hadd not found: run cmsenv first")

    tasks = []
    for key, group, cfgrel, samples in ENTRIES:
        if not any(s in ("all", key, group) for s in args.samples):
            continue
        cfgdir = os.path.dirname(cfgrel)
        stem = Path(cfgrel).name[:-len(".config")] if cfgrel.endswith(".config") else Path(cfgrel).stem
        groups = collect_inputs(args.se_host, args.se_base, args.study, cfgdir, samples)
        print(f"[{key}] {cfgdir}/{stem}: {len(groups)} output file(s), "
              f"{sum(len(v) for v in groups.values())} input file(s)")
        for sub, urls in sorted(groups.items()):
            tasks.append((cfgdir, stem, sub, urls))

    if args.dry_run or not tasks:
        for cfgdir, stem, sub, urls in tasks[:25]:
            print(f"   would merge {len(urls):4d} -> {cfgdir}/{stem}/{sub or '(top)'}")
        print(f"{len(tasks)} merge task(s).")
        return 0

    scratch = tempfile.mkdtemp(prefix="hadd_", dir=args.tmpdir)
    print(f"{len(tasks)} merge task(s), {args.jobs} in parallel, scratch {scratch}")
    t0, nbad, done = time.time(), 0, 0
    try:
        with ThreadPoolExecutor(max_workers=args.jobs) as ex:
            futs = [ex.submit(merge_one, args.se_host, args.se_base, args.study, c, s, sub, u, scratch, args.overwrite)
                    for c, s, sub, u in tasks]
            for f in as_completed(futs):
                tag, status, n = f.result()
                done += 1
                if not (status.startswith("ok") or status.startswith("skip")):
                    nbad += 1
                print(f"[{done:3d}/{len(tasks)}] {tag:60s} {n:4d} files  {status}", flush=True)
    finally:
        shutil.rmtree(scratch, ignore_errors=True)
    print(f"done in {(time.time() - t0) / 60:.1f} min, {nbad} failed.")
    return 1 if nbad else 0


if __name__ == "__main__":
    sys.exit(main())
