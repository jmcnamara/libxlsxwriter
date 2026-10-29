#!/usr/bin/env python3
"""Structured seed generation for chart_fuzzer (mirrors chart_fuzzer.cpp)."""

import argparse
import os

ESC = 0x5C


class Tail:
    def __init__(self):
        self.b = bytearray()

    def byte(self, v):
        self.b.append(v & 0xFF)

    def u16(self, v):
        self.b.append((v >> 8) & 0xFF)
        self.b.append(v & 0xFF)

    def row(self, v):
        self.enc_range(0, 40, v, 32)

    def col(self, v):
        self.enc_range(0, 12, v, 16)

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


F = 0  # unused placeholder for symmetry with pair 1


def emit(tail, ops):
    for op, a in ops:
        tail.byte(op)
        _OPS[op](tail, a)


def o0(t, a):
    t.byte(a["type"])


def o1(t, a):
    t.s(a["s1"])
    t.s(a["s2"])


def o2(t, a):
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])


def o4(t, a):
    t.s(a["s"])
    t.row(a["r"])
    t.col(a["c"])
    t.byte(a["b"])


def o5(t, a):
    t.byte(a["lw"])
    t.byte(a["dash"])
    t.byte(a["trans"])
    t.byte(a["ptype"])


def o7(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["b3"])
    t.byte(a["size"])


def o8(t, a):
    t.byte(a["b1"])
    t.dbl(a["d"])
    t.byte(a["b2"])
    t.byte(a["b3"])


def o9(t, a):
    t.byte(a["b"])


def o10(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.s(a["s"])
    t.row(a["r"])
    t.col(a["c"])
    if a["b2"] % 5 in (2, 3):
        t.byte(a["extra"])


def o11(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.dbl(a["d1"])
    t.dbl(a["d2"])
    t.u16(a["w"])


def o12(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.u16(a["w"])


def o13(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["w"])
    t.byte(a["p"])


def o14(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["dash"])


def o15(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["b3"])


def o16(t, a):
    t.byte(a["b1"])
    t.s(a["s"])
    t.row(a["r"])
    t.col(a["c"])


def o17(t, a):
    t.byte(a["b1"])
    sub = a["b1"] % 3
    if sub == 0:
        t.byte(a["pos"])
    elif sub == 2:
        for k in ("x", "y", "w", "h"):
            t.dbl(a[k])


def o18(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])


def o19(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["trans"])
    t.byte(a["ptype"])
    t.dbl(a["lx"])
    t.dbl(a["ly"])


def o20(t, a):
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.u16(a["w"])


def o21(t, a):
    t.row(a["r"])
    t.col(a["c"])


def o22(t, a):
    t.row(a["r"])
    t.col(a["c"])
    t.byte(a["xo"])
    t.byte(a["yo"])


def o23(t, a):
    t.s(a["s"])
    t.s(a["s2"])
    t.byte(a["b"])


def o3(t, a):
    o2(t, a)


def o6(t, a):
    pass  # points op consumes no fuzz bytes


_OPS = [o0, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14,
        o15, o16, o17, o18, o19, o20, o21, o22, o23]


def build(name, ops):
    t = Tail()
    emit(t, ops)
    return name, t.file_bytes()


def series_block(chart_type):
    """add chart + two series with ranges + labels."""
    return [
        (0, {"type": chart_type}),
        (1, {"s1": "=Sheet1!$A$2:$A$8", "s2": "=Sheet1!$B$2:$B$8"}),
        (4, {"s": "series one", "r": 1, "c": 0, "b": 1}),
        (2, {"r": 1, "c": 0, "r2": 8, "c2": 0}),
        (3, {"r": 1, "c": 1, "r2": 8, "c2": 1}),
        (1, {"s1": "=Sheet1!$A$2:$A$8", "s2": "=Sheet1!$C$2:$C$8"}),
        (7, {"b1": 7, "b2": 1, "b3": 3, "size": 8}),
        (21, {"r": 0, "c": 8}),
    ]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True)
    args = ap.parse_args()
    os.makedirs(args.out, exist_ok=True)

    seeds = []

    # chart-type sweep: each family with series + insert
    for name, ctype in [("column", 7), ("bar", 4), ("line", 10), ("area", 1),
                        ("pie", 16), ("doughnut", 17), ("scatter", 18),
                        ("radar", 23)]:
        seeds.append(build("type_%s" % name, series_block(ctype)))

    seeds.append(build("points_fill", [
        (0, {"type": 7}),
        (1, {"s1": "=Sheet1!$B$2:$B$8", "s2": ""}),
        (5, {"lw": 5, "dash": 4, "trans": 40, "ptype": 11}),
        (6, {}),
        (9, {"b": 1}),
        (21, {"r": 1, "c": 1}),
    ]))

    seeds.append(build("errorbars", [
        (0, {"type": 10}),
        (1, {"s1": "=Sheet1!$B$2:$B$8", "s2": "=Sheet1!$C$2:$C$8"}),
        (8, {"b1": 1, "d": 1600, "b2": 2, "b3": 1}),
        (8, {"b1": 3, "d": 0, "b2": 0, "b3": 0}),
        (21, {"r": 2, "c": 2}),
    ]))

    seeds.append(build("axis_numeric", [
        (0, {"type": 7}),
        (1, {"s1": "=Sheet1!$B$2:$B$8", "s2": ""}),
        (11, {"b1": 0, "b2": 0, "d1": 0, "d2": 6400, "w": 2}),
        (11, {"b1": 1, "b2": 1, "d1": 0, "d2": 0, "w": 10}),  # log base 10
        (11, {"b1": 0, "b2": 2, "d1": -32000, "d2": 0, "w": 0}),  # crossing neg
        (11, {"b1": 1, "b2": 3, "d1": 0, "d2": 0, "w": 0}),  # crossing max
        (11, {"b1": 0, "b2": 5, "d1": 0, "d2": 0, "w": 333}),  # major/minor units
        (12, {"b1": 0, "b2": 0, "w": 5}),
        (12, {"b1": 1, "b2": 2, "w": 3}),
        (15, {"b1": 0, "b2": 1, "b3": 9}),
        (21, {"r": 0, "c": 8}),
    ]))

    seeds.append(build("axis_names_look", [
        (0, {"type": 10}),
        (1, {"s1": "=Sheet1!$A$2:$A$8", "s2": "=Sheet1!$B$2:$B$8"}),
        (10, {"b1": 0, "b2": 0, "s": "X axis name \u00e9", "r": 0, "c": 0, "extra": 0}),
        (10, {"b1": 1, "b2": 3, "s": "0.0%", "r": 0, "c": 0, "extra": 2}),
        (10, {"b1": 0, "b2": 4, "s": "#,##0", "r": 0, "c": 0, "extra": 0}),
        (13, {"b1": 0, "b2": 0, "w": 3, "p": 6}),
        (13, {"b1": 1, "b2": 3, "w": 0, "p": 0}),  # reverse
        (13, {"b1": 1, "b2": 4, "w": 0, "p": 0}),  # axis off
        (14, {"b1": 0, "b2": 2, "dash": 2}),
        (21, {"r": 3, "c": 0}),
    ]))

    seeds.append(build("title_legend", [
        (0, {"type": 7}),
        (1, {"s1": "=Sheet1!$B$2:$B$8", "s2": ""}),
        (16, {"b1": 0, "s": "Chart title \u2713", "r": 0, "c": 0}),
        (16, {"b1": 1, "s": "", "r": 0, "c": 1}),
        (16, {"b1": 3, "s": "", "r": 0, "c": 0}),  # title off
        (17, {"b1": 0, "pos": 4}),
        (17, {"b1": 2, "x": 50, "y": 60, "w": 100, "h": 100}),
        (18, {"b1": 1, "b2": 2}),
        (19, {"b1": 0, "b2": 3, "trans": 50, "ptype": 3, "lx": 30, "ly": 30}),
        (19, {"b1": 1, "b2": 1, "trans": 10, "ptype": 9, "lx": 0, "ly": 0}),
        (20, {"b1": 0, "b2": 30, "w": 0}),
        (20, {"b1": 2, "b2": 50, "w": 0}),
        (20, {"b1": 5, "b2": 0, "w": 0}),
        (20, {"b1": 6, "b2": 1, "w": 0}),
        (21, {"r": 0, "c": 8}),
    ]))

    seeds.append(build("chartsheet_opts", [
        (0, {"type": 16}),
        (1, {"s1": "=Sheet1!$B$2:$B$8", "s2": "=Sheet1!$A$2:$A$8"}),
        (22, {"r": 2, "c": 2, "xo": 10, "yo": 12}),
        (23, {"s": "", "s2": "&CChartsheet header", "b": 20}),
        (23, {"s": "CS 2", "s2": "", "b": 30}),
    ]))

    seeds.append(build("string_series_names", [
        (0, {"type": 7}),
        (1, {"s1": "attackers\u2019 series & <name>", "s2": "=Sheet1!$B$2:$B$8"}),
        (4, {"s": "literal name", "r": 0, "c": 0, "b": 1}),
        (21, {"r": 0, "c": 8}),
    ]))

    for name, data in seeds:
        with open(os.path.join(args.out, name), "wb") as fh:
            fh.write(data)
    print("wrote %d seeds to %s" % (len(seeds), args.out))


if __name__ == "__main__":
    main()
