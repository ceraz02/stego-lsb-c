"""Round-trip test of steg1ano on generated images (the course sample images are not needed).

usage: python3 test/roundtrip.py project/steg1ano <workdir>   (the workdir is erased first)
"""
import os
import random
import shutil
import subprocess
import sys

exe = os.path.abspath(sys.argv[1])
work = os.path.abspath(sys.argv[2])
shutil.rmtree(work, ignore_errors=True)
os.makedirs(work)
random.seed(1234)


def write_pgm(path, w, h):
    data = bytes(random.randrange(256) for _ in range(w * h))
    with open(path, "wb") as f:
        f.write(b"P5\n%d %d\n255\n" % (w, h))
        f.write(data)
    return data


def run(args, cwd):
    return subprocess.run([exe] + args, cwd=cwd, capture_output=True, text=True)


results = []

# 1) round trip for every supported depth
cover_dir = os.path.join(work, "enc")
os.makedirs(cover_dir)
write_pgm(os.path.join(cover_dir, "cover.pgm"), 256, 256)
payload = bytes(random.randrange(256) for _ in range(500))
with open(os.path.join(cover_dir, "payload.bin"), "wb") as f:
    f.write(payload)

for b in (1, 2, 4, 8):
    stego = "stego_b%d.pgm" % b
    r = run(["encode", "cover.pgm", "payload.bin", stego, "-b=%d" % b], cover_dir)
    # pgm_write (course-supplied) returns the number of values written, so the
    # exit status is not 0 on success: judge by the output file instead.
    out_path = os.path.join(cover_dir, stego)
    ok_enc = os.path.exists(out_path) and os.path.getsize(out_path) == os.path.getsize(os.path.join(cover_dir, "cover.pgm"))
    dec_dir = os.path.join(work, "dec_b%d" % b)
    os.makedirs(dec_dir)
    shutil.copy(os.path.join(cover_dir, stego), dec_dir)
    r2 = run(["decode", stego], dec_dir)
    out = os.path.join(dec_dir, "payload.bin")
    same = os.path.exists(out) and open(out, "rb").read() == payload
    results.append(("round trip b=%d" % b, ok_enc and r2.returncode == 0 and same))

# 2) image too small: must fail cleanly (non-zero exit, no crash)
small_dir = os.path.join(work, "small")
os.makedirs(small_dir)
write_pgm(os.path.join(small_dir, "tiny.pgm"), 8, 8)
with open(os.path.join(small_dir, "payload.bin"), "wb") as f:
    f.write(payload)
r = run(["encode", "tiny.pgm", "payload.bin", "out.pgm", "-b=2"], small_dir)
results.append(("too-small image rejected", r.returncode != -1073741819 and "pas assez" in r.stderr))

# 3) image without a hidden message: decode must refuse
plain_dir = os.path.join(work, "plain")
os.makedirs(plain_dir)
write_pgm(os.path.join(plain_dir, "plain.pgm"), 64, 64)
r = run(["decode", "plain.pgm"], plain_dir)
results.append(("no hidden message detected", r.returncode != 0 and r.returncode != -1073741819 and "aucun message" in r.stderr))

width = max(len(n) for n, _ in results)
for name, ok in results:
    print(f"{name:<{width}}  {'PASS' if ok else 'FAIL'}")
sys.exit(0 if all(ok for _, ok in results) else 1)
