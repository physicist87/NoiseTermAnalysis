#!/usr/bin/env python3
"""Check merged hadd outputs against the sum of their inputs (per histogram entries and sum of weights).

For each requested key, the top-level file and the chosen EtaBin dir(s) are verified:
  merged[h].GetEntries()/GetSumOfWeights()  ==  sum over inputs of the same quantities.
Needs ROOT (after cmsenv) and a grid proxy.

Examples:
  python3 CondorJobs/check_merged.py --samples C
  python3 CondorJobs/check_merged.py --samples C --subs top EtaBin_0_0.261 EtaBin_3.839_5.191 --jobs 6
"""
import argparse
import math
import os
import sys
from multiprocessing import Pool
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from submit_jobs import ENTRIES, DEFAULT_TAG, DEFAULT_SE_HOST, DEFAULT_SE_BASE  # noqa: E402
from run_hadd import collect_inputs  # noqa: E402


def scan(url):
    """{histogram path: (entries, sumw)} for every TH1-derived object in the file (recursive)."""
    import ROOT
    ROOT.gErrorIgnoreLevel = ROOT.kError
    out = {}
    f = ROOT.TFile.Open(url)
    if not f or f.IsZombie():
        return url, None
    def walk(d, prefix):
        for k in d.GetListOfKeys():
            obj = k.ReadObj()
            if obj.InheritsFrom("TDirectory"):
                walk(obj, prefix + k.GetName() + "/")
            elif obj.InheritsFrom("TProfile"):
                # GetSumOfWeights() of a TProfile is the sum of bin means, not additive; use the bin weights
                sumw = sum(obj.GetBinEntries(i) for i in range(obj.GetNbinsX() + 2))
                out[prefix + k.GetName()] = (obj.GetEntries(), sumw)
            elif obj.InheritsFrom("TH1"):
                out[prefix + k.GetName()] = (obj.GetEntries(), obj.GetSumOfWeights())
    walk(f, "")
    f.Close()
    return url, out


def close(a, b):
    return math.isclose(a, b, rel_tol=1e-9, abs_tol=1e-9)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--study", default=DEFAULT_TAG)
    ap.add_argument("--samples", nargs="+", default=["C"])
    ap.add_argument("--subs", nargs="+", default=["top", "EtaBin_0_0.261"], help="'top' or EtaBin_* dir names")
    ap.add_argument("--se-host", default=os.environ.get("SE_HOST", DEFAULT_SE_HOST))
    ap.add_argument("--se-base", default=os.environ.get("SE_BASE", DEFAULT_SE_BASE))
    ap.add_argument("--jobs", type=int, default=6)
    args = ap.parse_args()

    nfail = 0
    for key, group, cfgrel, samples in ENTRIES:
        if not any(s in ("all", key, group) for s in args.samples):
            continue
        cfgdir = os.path.dirname(cfgrel)
        stem = Path(cfgrel).name[:-len(".config")]
        groups = collect_inputs(args.se_host, args.se_base, args.study, cfgdir, samples)
        for sub in args.subs:
            gkey = "" if sub == "top" else sub
            urls = groups.get(gkey)
            if not urls:
                print(f"[{key}] {sub}: no inputs found"); nfail += 1; continue
            dest_dir = f"{args.se_base}/{args.study}_Merged/{cfgdir}/{stem}" + (f"/{gkey}" if gkey else "")
            murl = f"{args.se_host}/{dest_dir}/{stem}.root"
            _, merged = scan(murl)
            if merged is None:
                print(f"[{key}] {sub}: cannot open merged file {murl}"); nfail += 1; continue
            tot = {}
            nbad_in = 0
            with Pool(args.jobs) as p:
                for url, d in p.imap_unordered(scan, urls):
                    if d is None:
                        nbad_in += 1; print(f"   cannot open input {url}"); continue
                    for h, (e, w) in d.items():
                        a = tot.setdefault(h, [0.0, 0.0]); a[0] += e; a[1] += w
            miss = sorted(set(tot) ^ set(merged))
            diff = [h for h in tot if h in merged and not (close(tot[h][0], merged[h][0]) and close(tot[h][1], merged[h][1]))]
            ok = not miss and not diff and not nbad_in
            nfail += 0 if ok else 1
            print(f"[{key}] {cfgdir}/{stem}/{sub}: inputs={len(urls)} hists={len(merged)} "
                  f"name-mismatch={len(miss)} value-mismatch={len(diff)} bad-inputs={nbad_in}  {'OK' if ok else 'FAIL'}")
            for h in (miss + diff)[:5]:
                print(f"     {h}: merged={merged.get(h)} sum-of-inputs={tot.get(h)}")
            top = sorted(merged.items(), key=lambda kv: -kv[1][0])[:3]
            for h, (e, w) in top:
                print(f"     largest: {h} entries={e:.0f} sumw={w:.6g}")
    return 1 if nfail else 0


if __name__ == "__main__":
    sys.exit(main())
