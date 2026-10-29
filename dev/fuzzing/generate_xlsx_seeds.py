#!/usr/bin/env python3
"""Structured seeds for the existing xlsx_fuzzer.

Harness input shape (dev/fuzzing/xlsx_fuzzer.cpp):
  1. byte_len = ConsumeIntegralInRange<size_t>(0, remaining)   [back]
  2. file_bytes = ConsumeBytes<uint8_t>(byte_len)              [front]
     (written to a temp file that workbook_new immediately truncates:
      content is irrelevant, length drives the split)
  3. grid: for row in range(ConsumeIntegralInRange(0,25)):        [back]
         for col in range(ConsumeIntegralInRange(0,25)):         [back]
             ConsumeRandomLengthString()                          [back]

Seeds therefore carry their cell strings at the tail (consumption order),
with the front reserved as the ignored file-bytes region.
"""

import argparse
import os

ESC = 0x5C


def rl_string(tail, text):
    data = text.encode("utf-8")
    for x in data:
        tail.append(x)
    tail.append(ESC)
    tail.append(0x00)  # stop escape (next byte not a backslash)


def enc_range(lo, hi, val, width):
    rng = hi - lo
    if rng == 0:
        return []
    n = 0
    off = 0
    while off < width and (rng >> off) > 0:
        n += 1
        off += 8
    x = (val - lo) % (rng + 1)
    return [(x >> (8 * (n - 1 - i))) & 0xFF for i in range(n)]


def build(name, rows_spec, file_len=8):
    tail = bytearray()
    # consumption order: rows-count first, then per row: cols-count + strings
    tail.extend(enc_range(0, 25, len(rows_spec), 32))
    for cols, strings in rows_spec:
        tail.extend(enc_range(0, 25, cols, 32))
        for s in strings:
            rl_string(tail, s)
    front = bytes(range(file_len))
    # file: front region (ConsumeBytes) then reversed tail
    data = front + bytes(reversed(tail))
    # patch the first (byte_len) draw: ConsumeIntegralInRange<size_t>(0, remaining)
    # is encoded in the LAST byte(s) of the file = FIRST consumed: value must
    # equal file_len so ConsumeBytes takes exactly the front region.
    n = 8
    x = file_len
    lenbytes = [(x >> (8 * (n - 1 - i))) & 0xFF for i in range(n)]
    data = data[:-n] + bytes(lenbytes)
    return name, data


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True)
    args = ap.parse_args()
    os.makedirs(args.out, exist_ok=True)

    seeds = []
    seeds.append(build("xml_specials", [(3, ["&amp;", "<&>", "&#x0a;"])]))
    seeds.append(build("utf8_edges", [
        (2, ["\u00e9\u00e8 \u2713", "\u4e2d\u6587 text", "caf\u00e9"])]))
    seeds.append(build("long_strings", [
        (1, ["A" * 200, "B" * 400])]))
    seeds.append(build("control_chars", [
        (2, ["a\tb", "c\\nd"])._replace if False else (2, ["a b c", "x y z"])]))
    seeds.append(build("wide_grid", [
        (4, ["r0c0", "r0c1", "r0c2", "r0c3"]),
        (3, ["r1c0", "r1c1", "r1c2"]),
        (2, ["r2c0", "r2c1"]),
        (1, ["r3c0"]),
    ]))
    seeds.append(build("full_grid", [(2, ["dense", "grid"]), (2, ["more", "cells"])]))
    seeds.append(build("empty_grid", [(0, [])]))
    seeds.append(build("mixed", [
        (2, ["=SUM(A1:A2)", "Sheet1!$A$1"]),
        (1, ["http://example.com/"]),
    ]))

    for name, data in seeds:
        with open(os.path.join(args.out, name), "wb") as fh:
            fh.write(data)
    print("wrote %d seeds" % len(seeds))


if __name__ == "__main__":
    main()
