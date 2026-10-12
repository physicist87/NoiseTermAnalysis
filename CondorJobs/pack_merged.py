#!/usr/bin/env python3
"""Pack the merged outputs on the SE into one tar (+ sha256) to copy to another machine.

Run on the server (needs xrdcp/xrdfs and a grid proxy; no ROOT needed).
ROOT files are already compressed, so the tar is uncompressed (gzip would gain only a few %).

Output: <outdir>/<study>_Merged_<label>.tar  and  <tar>.sha256   (label = all | selected keys)
Tar layout: <study>_Merged/<cfgdir>/<stem>/[EtaBin_*/]<stem>.root

Examples:
  python3 CondorJobs/pack_merged.py --dry-run
  python3 CondorJobs/pack_merged.py --samples all --outdir /tmp
  python3 CondorJobs/pack_merged.py --samples C MC

On the Mac:
  rsync -avP --partial sha@cms01.knu.ac.kr:/tmp/Run3_2025_PUPPI_v1_Merged_all.tar* .
  shasum -a 256 -c Run3_2025_PUPPI_v1_Merged_all.tar.sha256
  tar xf Run3_2025_PUPPI_v1_Merged_all.tar
"""
import argparse
import hashlib
import os
import shutil
import subprocess
import sys
import tarfile
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from submit_jobs import ENTRIES, DEFAULT_TAG, DEFAULT_SE_HOST, DEFAULT_SE_BASE  # noqa: E402


def list_root_files(host, path):
    r = subprocess.run(["xrdfs", host, "ls", "-R", path], capture_output=True, text=True)
    if r.returncode != 0:
        return None
    return sorted(l.strip() for l in r.stdout.splitlines() if l.strip().endswith(".root"))


def fetch(host, remote, local):
    local.parent.mkdir(parents=True, exist_ok=True)
    r = subprocess.run(["xrdcp", "-f", "-s", f"{host}/{remote}", str(local)], capture_output=True, text=True)
    ok = r.returncode == 0 and local.is_file() and local.stat().st_size > 0
    return remote, ok, r.stderr.strip()[-200:]


def sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 24), b""):
            h.update(chunk)
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--study", default=DEFAULT_TAG)
    ap.add_argument("--samples", nargs="+", default=["all"], help="all | data | mc | pu | KEY ...")
    ap.add_argument("--se-host", default=os.environ.get("SE_HOST", DEFAULT_SE_HOST))
    ap.add_argument("--se-base", default=os.environ.get("SE_BASE", DEFAULT_SE_BASE))
    ap.add_argument("--outdir", default="/tmp", help="where the tar is written (and the temporary staging dir)")
    ap.add_argument("--jobs", type=int, default=6, help="parallel xrdcp")
    ap.add_argument("--keep", action="store_true", help="keep the staging dir after tarring")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    merged_root = f"{args.se_base}/{args.study}_Merged"
    files = []
    for key, group, cfgrel, samples in ENTRIES:
        if not any(s in ("all", key, group) for s in args.samples):
            continue
        cfgdir = os.path.dirname(cfgrel)
        stem = Path(cfgrel).name[:-len(".config")] if cfgrel.endswith(".config") else Path(cfgrel).stem
        rdir = f"{merged_root}/{cfgdir}/{stem}"
        lst = list_root_files(args.se_host, rdir)
        if lst is None:
            sys.exit(f"[{key}] {rdir}: not found on SE (run run_hadd.py first)")
        print(f"[{key}] {cfgdir}/{stem}: {len(lst)} file(s)")
        files += lst

    print(f"total {len(files)} file(s)")
    if args.dry_run or not files:
        return 0

    label = "all" if "all" in args.samples else "_".join(args.samples)
    top = f"{args.study}_Merged"
    outdir = Path(args.outdir)
    stage = outdir / f"{top}_stage_{os.getpid()}"
    tar_path = outdir / f"{top}_{label}.tar"
    t0 = time.time()
    try:
        with ThreadPoolExecutor(max_workers=args.jobs) as ex:
            futs = [ex.submit(fetch, args.se_host, f, stage / top / f[len(merged_root) + 1:]) for f in files]
            nbad = 0
            for i, fu in enumerate(futs, 1):
                remote, ok, err = fu.result()
                if not ok:
                    nbad += 1
                    print(f"  FETCH FAILED {remote}: {err}")
                if i % 20 == 0 or i == len(futs):
                    print(f"  fetched {i}/{len(futs)}", flush=True)
        if nbad:
            sys.exit(f"{nbad} fetch failure(s); nothing packed")
        with tarfile.open(tar_path, "w") as tf:
            tf.add(stage / top, arcname=top)
        digest = sha256(tar_path)
        Path(f"{tar_path}.sha256").write_text(f"{digest}  {tar_path.name}\n")
    finally:
        if not args.keep:
            shutil.rmtree(stage, ignore_errors=True)
    print(f"wrote {tar_path} ({tar_path.stat().st_size / 1e9:.2f} GB) and {tar_path}.sha256 in {(time.time() - t0) / 60:.1f} min")
    return 0


if __name__ == "__main__":
    sys.exit(main())
