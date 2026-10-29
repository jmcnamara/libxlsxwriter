#!/usr/bin/env python3
"""Structured seed generation for workbook_props_fuzzer (mirrors the harness)."""

import argparse
import os

ESC = 0x5C


class Tail:
    def __init__(self):
        self.b = bytearray()

    def byte(self, v):
        self.b.append(v & 0xFF)

    def u32(self, v):
        for i in (3, 2, 1, 0):
            self.b.append((v >> (8 * i)) & 0xFF)

    def dbl(self, x):
        self.enc_range(-100000, 100000, x, 32)

    def s(self, text):
        data = text.encode("utf-8")[:46]
        assert ESC not in data
        for x in data:
            self.b.append(x)
        self.b.append(ESC)
        self.b.append(0x00)

    def enc_range(self, lo, hi, val, width):
        rng = hi - lo
        if rng == 0:
            return
        n = 0
        off = 0
        while off < width and (rng >> off) > 0:
            n += 1
            off += 8
        x = (val - lo) % (rng + 1)
        for i in range(n):
            shift = 8 * (n - 1 - i)
            self.b.append((x >> shift) & 0xFF)

    def file_bytes(self):
        return bytes(reversed(self.b))


def o0(t, a):
    t.s(a["s"])


def o1(t, a):
    t.s(a["s1"])
    t.s(a["s2"])
    t.s(a["s3"])
    t.s(a["s4"])
    t.byte(a["b"])


def o2(t, a):
    t.byte(a["b1"])
    t.s(a["s"])
    t.dbl(a["d"])
    t.u32(a["u"])
    t.byte(a["b2"])


def o3(t, a):
    t.s(a["s"])
    t.s(a["s2"])


def o4(t, a):
    t.s(a["s"])


def o5(t, a):
    t.byte(a["b1"])
    t.s(a["s"])
    t.byte(a["z1"])
    t.byte(a["z2"])


def o6(t, a):
    pass


def o7(t, a):
    t.s(a["s"])


def o8(t, a):
    t.byte(a["b"])


_OPS = [o0, o1, o2, o3, o4, o5, o6, o7, o8]


def build(name, mode, ops):
    t = Tail()
    for op, a in ops:
        t.byte(op)
        _OPS[op](t, a)
    data = t.file_bytes()
    # the last byte is the mode byte (first consumed by the harness)
    data = data[:-1] + bytes([mode])
    return name, data


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True)
    args = ap.parse_args()
    os.makedirs(args.out, exist_ok=True)

    seeds = []

    seeds.append(build("props_basic", 0, [
        (1, {"s1": "Doc Title", "s2": "Fuzz Author", "s3": "Fuzz Co",
             "s4": "Category X", "b": 0}),
        (2, {"b1": 0, "s": "PropName", "d": 1600, "u": 42, "b2": 1}),
        (2, {"b1": 1, "s": "Ratio", "d": 320, "u": 7, "b2": 0}),
        (6, {}),
    ]))

    seeds.append(build("props_escaped", 0, [
        (1, {"s1": "Title & <tags> \"quoted\"", "s2": "author\u2019s name",
             "s3": "A&B<C>", "s4": "\u00e9\u00e8 category", "b": 1}),
        (2, {"b1": 0, "s": "xml&prop", "d": 0, "u": 0, "b2": 0}),
        (6, {}),
    ]))

    seeds.append(build("custom_all_types", 0, [
        (2, {"b1": 0, "s": "s-prop", "d": 0, "u": 0, "b2": 0}),
        (2, {"b1": 1, "s": "n-prop", "d": -3200, "u": 0, "b2": 0}),
        (2, {"b1": 2, "s": "i-prop", "d": 0, "u": 2147483647, "b2": 0}),
        (2, {"b1": 3, "s": "b-prop", "d": 0, "u": 0, "b2": 1}),
        (2, {"b1": 4, "s": "d-prop", "d": 0, "u": 0, "b2": 20}),
    ]))

    seeds.append(build("define_names", 0, [
        (3, {"s": "MyRange", "s2": "=Sheet1!$A$1:$A$5"}),
        (3, {"s": "bad name", "s2": "not a range"}),
        (3, {"s": "\u00e9range", "s2": "=Sheet1!$A$1"}),
        (4, {"s": "Sheet1"}),
        (4, {"s": "bad:name"}),
        (0, {"s": "Sheet 2"}),
        (0, {"s": "bad:name?*"}),
    ]))

    seeds.append(build("epoch_vba_size", 0, [
        (5, {"b1": 1, "s": "VBAName", "z1": 1, "z2": 1}),
        (7, {"s": "http://example.com/"}),
        (8, {"b": 30}),
        (6, {}),
    ]))

    seeds.append(build("constant_memory", 1, [
        (6, {}),
        (6, {}),
        (0, {"s": "Second Sheet"}),
        (7, {"s": "https://example.org/"}),
    ]))

    seeds.append(build("zip64_mode", 2, [
        (1, {"s1": "zip64 doc", "s2": "a", "s3": "b", "s4": "c", "b": 0}),
        (6, {}),
    ]))

    for name, data in seeds:
        with open(os.path.join(args.out, name), "wb") as fh:
            fh.write(data)
    print("wrote %d seeds to %s" % (len(seeds), args.out))


if __name__ == "__main__":
    main()
