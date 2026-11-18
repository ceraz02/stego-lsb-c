# stego-lsb-c

LSB steganography in C: hide a binary file inside a PGM image by changing the least significant bits of its pixels, and recover it later.

Pair project, December 2023 to January 2024, from my first months of C programming at engineering school (module "Informatique", semester 5).

## How it works

An image is a grid of 8-bit grey pixels. Replacing the lowest 1, 2, 4 or 8 bits of each pixel with message bits changes the picture very little, and the message can be read back from those bits.

`steg1ano encode cover.pgm secret.dat stego.pgm -b=2` hides a file in three consecutive blocks, written pixel after pixel, row after row:

1. a header: a canary value (`0xDEADBEEF`), the length of the file name and the length of the content;
2. the file name;
3. the file content.

`steg1ano decode stego.pgm` does not need to be told the depth: it tries 1, 2, 4 and 8 bits per pixel until the canary appears, then reads the header, the name and the content and writes the file back under its original name. If no canary is found it reports that the image hides nothing.

## What is mine and what is supplied

I implemented the library behind the program:

| File | Content |
|---|---|
| `src/byte_get.c`, `byte_set.c` | bit-level access: read or write a range of bits, the lowest bits of a byte |
| `src/contents.c` | read a whole file into memory, write a buffer to a file |
| `src/image.c` | create, free and access an image (a contiguous pixel array with row pointers) |
| `src/options.c` | command-line option helpers |
| `src/pgm.c` | `pgm_read`: binary PGM (P5) reader with comment handling and header checks |
| `src/stego_encode.c`, `stego_decode.c` | block encoding and decoding, canary detection, the three-block layout |
| `test/roundtrip.py` | end-to-end test on generated images (see below) |

Supplied by the course and kept so the project builds and can be tested: the headers with the function contracts (`include/`), the command-line driver `project/steg1ano.c`, `pgm_write` and `byte_print`, the unit tests in `test/lvl-*`, and the `Makefile` (its `my-box-stylish` and `my-env-stylish` targets install debugging tools and are not needed).

Not done: the optional extensions (any depth from 1 to 8, colour PPM images, one-time-pad encryption, pixel permutation, steganalysis).

## Verification

- **Course unit tests:** the six that do not need image files pass (built with gcc 13). The other three (`lvl-2-pgm_ok`, `lvl-2-stego`, `lvl-3-pgm_read_with_comments`) read the course's sample images, which the course does not put in git, so they cannot run from this repository.
- **Round trip on generated images** (`test/roundtrip.py`): hides a 500-byte file in a 256 x 256 image at 1, 2, 4 and 8 bits per pixel and checks that the recovered file is identical; checks that an image that is too small is refused cleanly; checks that an image without a message is refused. 6 of 6 pass.
- **Memory:** I used Valgrind during the course. I have not re-run it on the final version in this repository.

## Review fixes

After the project was handed in, a review of `stego_encode.c` and `stego_decode.c` found and fixed these (see the last commit):

- The pixel bounds check ran after the write and with the wrong condition, so an image that was too small could be overrun. It now runs before every pixel access, and the round-trip test covers it.
- The file to hide was read twice, leaking the first buffer, and some error paths did not free memory.
- The result of writing the decoded file was compared with the wrong value, so write errors went unnoticed.
- The decoded file name was not guaranteed to end with a null character, and the canary buffer was not cleared between depth attempts.
- `pgm_write` (supplied) opened its file in text mode, which corrupts binary output on Windows; it now uses `"wb"`.

## Limits

- Only 8-bit binary PGM images (P5), no colour.
- `decode` trusts the lengths stored in the image: a crafted image could ask for a very large allocation.
- The file name is stored exactly as given on the command line, including any directory part, and decode writes to that path.
- The supplied driver uses `pgm_write`'s return value (the number of values written) as the exit status of `encode`, so the status is not 0 on success.

## Build and run

```sh
make CC=gcc project/steg1ano          # the Makefile defaults to clang
make CC=gcc tests                     # builds the unit tests in test/
python3 test/roundtrip.py project/steg1ano /tmp/stego_work
```

Example:

```sh
project/steg1ano encode image.pgm secret.dat stego.pgm -b=2
project/steg1ano decode stego.pgm
```

## Licence

The files listed above as mine are under the MIT licence (see `LICENSE`). The course-supplied files keep the course's terms.
