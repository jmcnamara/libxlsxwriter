#!/usr/bin/env python3
"""Structured seed generation for worksheet_api_fuzzer.

Emits deterministic byte seeds shaped exactly like worksheet_api_fuzzer.cpp
consumes them. Mirror rules (FuzzedDataProvider, all back-consumption):

  * ConsumeIntegral<T>() / ConsumeIntegralInRange<T>(lo, hi): pulls bytes off
    the tail; n bytes where n covers (hi-lo) within T's width; first byte read
    becomes the high byte; result = lo + (value % (range+1)).
  * ConsumeRandomLengthString(64): reads up to 64 tail bytes, stopping early
    at a backslash escape; we append ESC(0x5C) + 0x00 to end each string, so
    seed strings must not contain 0x5C.

The seed builder therefore appends draws in consumption order to a tail list;
the file is the reversed tail (plus a no-op front filler for mutation).
Run from dev/fuzzing/build.sh: --out <dir>.
"""

import argparse
import os

ESC = 0x5C


class Tail:
    def __init__(self):
        self.b = bytearray()

    def raw(self, bytelist):
        for x in bytelist:
            self.b.append(x & 0xFF)

    def byte(self, v):
        self.raw([v])

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
        out = []
        for i in range(n):
            shift = 8 * (n - 1 - i)
            out.append((x >> shift) & 0xFF)
        self.raw(out)

    def row(self, v):
        self.enc_range(0, 200, v, 32)

    def col(self, v):
        self.enc_range(0, 60, v, 16)

    def wild_row(self, v):
        self.enc_range(0, 65535, v, 32)

    def wild_col(self, v):
        self.enc_range(0, 16383, v, 32)

    def dbl(self, x):
        # draw_double: ConsumeIntegralInRange<int32_t>(-1000000, 1000000) / 64.0
        self.enc_range(-1000000, 1000000, x, 32)

    def u32(self, v):
        self.enc_range(0, 0xFFFFFFFF, v, 32)

    def urange800(self, v):
        self.enc_range(0, 800, v, 32)

    def s(self, text):
        data = text.encode("utf-8")[:62]
        assert ESC not in data, "seed strings must not contain a backslash"
        self.raw(data)
        self.raw([ESC, 0x00])

    def file_bytes(self):
        return bytes(reversed(self.b))


def emit(tail, ops):
    tail.byte(0)  # stream-header format-index byte (drawn once before the loop)
    for op, args in ops:
        tail.byte(op)
        _OPS[op](tail, args)


def build(name, ops):
    t = Tail()
    emit(t, ops)
    return name, t.file_bytes()


def o0(t, a):  # write_string
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.byte(a["f"])


def o1(t, a):  # write_number bounded (dbl is the pre-division value)
    t.row(a["r"])
    t.col(a["c"])
    t.dbl(a["d"])
    t.byte(a["f"])


def o2(t, a):  # write_number raw double bits
    t.row(a["r"])
    t.col(a["c"])
    t.enc_range(0, 0xFFFFFFFFFFFFFFFF, a["bits"], 64)
    t.byte(a["f"])


def o3(t, a):  # write_formula
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.byte(a["f"])


def o4(t, a):  # write_formula_num
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.byte(a["f"])
    t.dbl(a["d"])


def o5(t, a):  # write_formula_str
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.s(a["s2"])
    t.byte(a["f"])


def o6o7(t, a):  # write_(dynamic_)array_formula
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])
    t.s(a["s"])
    t.byte(a["f"])


def o8(t, a):  # write_dynamic_formula
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.byte(a["f"])


def o9(t, a):  # write_blank
    t.row(a["r"])
    t.col(a["c"])
    t.byte(a["f"])


def o10(t, a):  # write_boolean
    t.row(a["r"])
    t.col(a["c"])
    t.byte(a["b"])
    t.byte(a["f"])


def o11(t, a):  # write_datetime (y/m/d/h/mi/s addends)
    t.row(a["r"])
    t.col(a["c"])
    for x in ("y", "mo", "d", "h", "mi", "s"):
        t.byte(a[x])
    t.byte(a["f"])


def o12(t, a):  # write_unixtime low bytes: b1,b2,b3, high, low, fmt
    t.row(a["r"])
    t.col(a["c"])
    t.raw(a["bytes"])
    t.byte(a["f"])


def o13(t, a):  # write_unixtime raw: u32 + 3 bytes, fmt
    t.row(a["r"])
    t.col(a["c"])
    t.u32(a["u"])
    t.raw(a["bytes"])
    t.byte(a["f"])


def o14(t, a):  # write_rich_string
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.s(a["s2"])
    t.s(a["s3"])
    t.byte(a["b"])
    t.byte(a["f"])


def o15(t, a):  # write_comment
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])


def o16(t, a):  # write_comment_opt
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.s(a["s2"])
    t.byte(a["vis"])
    t.byte(a["w"])
    t.byte(a["h"])


def o17(t, a):  # write_url
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.byte(a["f"])


def o18(t, a):  # write_url_opt
    t.row(a["r"])
    t.col(a["c"])
    t.s(a["s"])
    t.s(a["s2"])
    t.byte(a["f"])


def o19(t, a):  # merge_range
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])
    t.s(a["s"])
    t.byte(a["f"])


def o20(t, a):  # autofilter
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])


def o21(t, a):  # add_table
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])
    t.s(a["s"])
    t.s(a["s2"])
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["b3"])


def o22(t, a):  # data_validation_range
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])
    t.s(a["s"])
    t.s(a["s2"])
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["b3"])
    t.dbl(a["d1"])
    t.dbl(a["d2"])
    t.dbl(a["d3"])


def o23(t, a):  # conditional_format_range
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])
    t.s(a["s"])
    t.s(a["s2"])
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["f"])
    t.dbl(a["d1"])
    t.dbl(a["d2"])
    t.dbl(a["d3"])
    t.dbl(a["d4"])
    t.byte(a["b3"])
    t.u32(a["u"])


def o24(t, a):  # filter_column
    t.col(a["c"])
    t.s(a["s"])
    t.byte(a["b"])
    t.dbl(a["d"])


def o25(t, a):  # filter_column2
    t.col(a["c"])
    t.s(a["s"])
    t.s(a["s2"])
    t.byte(a["b1"])
    t.dbl(a["d1"])
    t.byte(a["b2"])
    t.dbl(a["d2"])
    t.byte(a["b3"])


def o26(t, a):  # filter_list
    t.col(a["c"])
    t.s(a["s"])
    t.s(a["s2"])


def o27(t, a):  # set_column
    t.col(a["c"])
    t.col(a["c2"])
    t.dbl(a["d"])
    t.byte(a["f"])


def o28(t, a):  # set_column_opt
    t.col(a["c"])
    t.col(a["c2"])
    t.byte(a["b"])
    t.dbl(a["d"])
    t.byte(a["f"])


def o29(t, a):  # set_column_pixels
    t.col(a["c"])
    t.col(a["c2"])
    t.urange800(a["px"])
    t.byte(a["f"])


def o30(t, a):  # set_row
    t.row(a["r"])
    t.dbl(a["d"])
    t.byte(a["f"])


def o31(t, a):  # set_row_opt
    t.row(a["r"])
    t.byte(a["b"])
    t.dbl(a["d"])
    t.byte(a["f"])


def o32(t, a):  # set_row_pixels
    t.row(a["r"])
    t.urange800(a["px"])
    t.byte(a["f"])


def o33(t, a):  # set_header / set_footer
    t.s(a["s"])
    t.s(a["s2"])


def o34(t, a):  # set_header_opt / set_footer_opt
    t.s(a["s"])
    t.s(a["s2"])
    t.dbl(a["d"])


def o35(t, a):  # print_area
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])


def o36(t, a):  # repeat_rows/cols
    t.row(a["r"])
    t.row(a["r2"])
    t.col(a["c"])
    t.col(a["c2"])
    t.byte(a["b"])


def o37(t, a):  # set_selection from A1 parsing
    t.s(a["s"])
    t.s(a["s2"])


def o38(t, a):  # freeze/split panes
    t.row(a["r"])
    t.col(a["c"])
    t.row(a["r2"])
    t.col(a["c2"])
    t.byte(a["b"])


def o39(t, a):  # protect
    t.s(a["s"])
    t.byte(a["b"])


def o40(t, a):  # page setup group
    t.byte(a["b1"])
    t.byte(a["b2"])


def o41(t, a):  # set_margins
    for k in ("d1", "d2", "d3", "d4"):
        t.dbl(a[k])


def o42(t, a):  # fit_to_pages + print flags
    t.byte(a["b1"])
    t.byte(a["b2"])
    t.byte(a["b3"])


def o43(t, a):  # tab color / visibility group
    t.byte(a["b1"])
    if a["b1"] % 7 == 0:
        t.u32(a["u"])


def o44(t, a):  # outline/comments/default-row group
    t.byte(a["b1"])
    sub = a["b1"] % 6
    if sub == 0:
        t.byte(a["b2"])
        t.byte(a["b3"])
    elif sub == 1:
        t.byte(a["b2"])
    elif sub == 2:
        t.s(a["s"])
    elif sub == 3:
        t.dbl(a["d"])
        t.byte(a["b2"])
    elif sub == 4:
        t.row(a["r"])
        t.col(a["c"])


def o45(t, a):  # pagebreaks
    for i in range(8):
        t.byte(a["h%d" % i])
    for i in range(8):
        t.byte(a["v%d" % i])
    t.byte(a["b"])


def o46(t, a):  # second worksheet name
    t.s(a["s"])
    t.s(a["s2"])


def o47(t, a):  # wildcard full-range write
    t.wild_row(a["r"])
    t.wild_col(a["c"])
    t.s(a["s"])
    t.byte(a["f"])


def o48(t, a):  # A1 parse
    t.s(a["s"])


def o49(t, a):  # ignore_errors
    t.s(a["s"])
    t.byte(a["b"])


_OPS = [o0, o1, o2, o3, o4, o5, o6o7, o6o7, o8, o9, o10, o11, o12, o13, o14,
        o15, o16, o17, o18, o19, o20, o21, o22, o23, o24, o25, o26, o27, o28,
        o29, o30, o31, o32, o33, o34, o35, o36, o37, o38, o39, o40, o41, o42,
        o43, o44, o45, o46, o47, o48, o49]


F = 1  # format index used across seeds


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True)
    args = ap.parse_args()
    os.makedirs(args.out, exist_ok=True)

    seeds = []

    seeds.append(build("write_types", [
        (0, {"r": 0, "c": 0, "s": "Hello World", "f": F}),
        (1, {"r": 1, "c": 1, "d": 3200, "f": F}),
        (2, {"r": 2, "c": 2, "bits": 0x7FF8000000000000, "f": F}),  # NaN
        (3, {"r": 3, "c": 3, "s": "=SUM(A1:A2)", "f": F}),
        (4, {"r": 4, "c": 4, "s": "=1+2", "f": F, "d": 960}),
        (5, {"r": 5, "c": 5, "s": "=\"A\"&\"B\"", "s2": "AB", "f": F}),
        (6, {"r": 6, "c": 0, "r2": 6, "c2": 3, "s": "=SUM(A1:D1)", "f": F}),
        (7, {"r": 7, "c": 0, "r2": 7, "c2": 2, "s": "=UNIQUE(A1:A9)", "f": F}),
        (8, {"r": 8, "c": 0, "s": "=RAND()", "f": F}),
        (9, {"r": 9, "c": 9, "f": F}),
        (10, {"r": 10, "c": 10, "b": 1, "f": F}),
        (11, {"r": 11, "c": 11, "y": 20, "mo": 7, "d": 13, "h": 15,
              "mi": 30, "s": 45, "f": F}),
        (12, {"r": 12, "c": 12, "bytes": [0x80, 0x2B, 0x2F, 0x00, 0x00], "f": F}),
        (13, {"r": 13, "c": 13, "u": 1000000, "bytes": [0x00, 0x00, 0x00], "f": F}),
    ]))

    seeds.append(build("rich_comments", [
        (14, {"r": 0, "c": 0, "s": "bold part", "s2": " plain", "s3": " tail",
              "b": 1, "f": F}),
        (15, {"r": 1, "c": 1, "s": "a comment with, comma and more"}),
        (16, {"r": 2, "c": 2, "s": "comment with options", "s2": "Author Name",
              "vis": 1, "w": 32, "h": 16}),
        (14, {"r": 3, "c": 0, "s": "utf8 \u00e9\u00e8 caf\u00e9", "s2": " \u2713",
              "s3": " end", "b": 3, "f": F}),
    ]))

    seeds.append(build("urls_merge", [
        (17, {"r": 0, "c": 0, "s": "http://example.com/", "f": F}),
        (18, {"r": 1, "c": 0, "s": "mailto:user@example.com", "s2": "Mail me",
              "f": F}),
        (17, {"r": 2, "c": 0, "s": "internal:'Sheet 2'!A1", "f": F}),
        (19, {"r": 4, "c": 0, "r2": 5, "c2": 2, "s": "merged text", "f": F}),
        (20, {"r": 0, "c": 0, "r2": 10, "c2": 5}),
    ]))

    seeds.append(build("table", [
        (21, {"r": 0, "c": 0, "r2": 4, "c2": 2, "s": "Alpha", "s2": "Total",
              "b1": 1, "b2": 9, "b3": 1}),
        (21, {"r": 6, "c": 0, "r2": 9, "c2": 3, "s": "Beta", "s2": "Sum",
              "b1": 0, "b2": 0, "b3": 0}),
        (0, {"r": 1, "c": 0, "s": "data", "f": F}),
    ]))

    seeds.append(build("data_validation", [
        (22, {"r": 0, "c": 0, "r2": 20, "c2": 5, "s": "=A1", "s2": "Range",
              "b1": 1, "b2": 0, "b3": 5, "d1": 640, "d2": 0, "d3": 12800}),
        (22, {"r": 22, "c": 8, "r2": 30, "c2": 9, "s": "open", "s2": "high",
              "b1": 5, "b2": 0, "b3": 6, "d1": 0, "d2": 0, "d3": 0}),  # list
        (22, {"r": 2, "c": 2, "r2": 3, "c2": 3, "s": "=B1", "s2": "between",
              "b1": 3, "b2": 6, "b3": 1, "d1": 500, "d2": 64, "d3": 640}),
    ]))

    seeds.append(build("cond_format", [
        (23, {"r": 0, "c": 0, "r2": 10, "c2": 5, "s": "=A1", "s2": "B1:C5",
              "b1": 1, "b2": 5, "f": F, "d1": 3200, "d2": 0, "d3": 1600,
              "d4": 3200, "b3": 1, "u": 4}),
        (23, {"r": 12, "c": 0, "r2": 20, "c2": 5, "s": "", "s2": "",
              "b1": 12, "b2": 0, "f": F, "d1": 640, "d2": 0, "d3": 0,
              "d4": 640, "b3": 9, "u": 64}),  # data bar 2010
        (23, {"r": 22, "c": 0, "r2": 30, "c2": 3, "s": "", "s2": "",
              "b1": 17, "b2": 0, "f": F, "d1": 0, "d2": 0, "d3": 0,
              "d4": 0, "b3": 17, "u": 384}),  # icon set
    ]))

    seeds.append(build("filters", [
        (20, {"r": 0, "c": 0, "r2": 12, "c2": 4}),
        (24, {"c": 1, "s": "match me", "b": 0, "d": 320}),
        (25, {"c": 2, "s": "a", "s2": "b", "b1": 0, "d1": 0, "b2": 1,
              "d2": 640, "b3": 1}),
        (26, {"c": 3, "s": "x", "s2": "y"}),
    ]))

    seeds.append(build("rows_cols", [
        (27, {"c": 0, "c2": 5, "d": 1024, "f": F}),
        (28, {"c": 6, "c2": 9, "b": 5, "d": 2048, "f": F}),
        (29, {"c": 10, "c2": 12, "px": 120, "f": F}),
        (30, {"r": 0, "d": 1280, "f": F}),
        (31, {"r": 5, "b": 7, "d": 1920, "f": F}),
        (32, {"r": 9, "px": 200, "f": F}),
    ]))

    seeds.append(build("header_footer", [
        (33, {"s": "&L&\"Arial,Bold\"&12Left&Ccenter&R&DSheet", "s2": "&C&P of &N"}),
        (34, {"s": "&Limg&Cimg&Ropt", "s2": "footer opt", "d": 4}),
    ]))

    seeds.append(build("print_selection", [
        (35, {"r": 0, "c": 0, "r2": 99, "c2": 20}),
        (36, {"r": 0, "r2": 2, "c": 0, "c2": 1, "b": 1}),
        (37, {"s": "A1", "s2": "B5"}),
        (38, {"r": 1, "c": 1, "r2": 0, "c2": 0, "b": 1}),
        (45, {"h0": 1, "h1": 2, "h2": 3, "h3": 0, "h4": 0, "h5": 0, "h6": 0,
              "h7": 0, "v0": 5, "v1": 10, "v2": 15, "v3": 0, "v4": 0,
              "v5": 0, "v6": 0, "v7": 0, "b": 1}),
    ]))

    seeds.append(build("protect_pagesetup", [
        (39, {"s": "secret", "b": 0x2A}),
        (40, {"b1": 0, "b2": 50}),
        (40, {"b1": 1, "b2": 9}),
        (41, {"d1": 6, "d2": 6, "d3": 6, "d4": 6}),
        (42, {"b1": 1, "b2": 1, "b3": 0}),
        (43, {"b1": 0, "u": 0xFF9900}),
    ]))

    seeds.append(build("misc_group", [
        (44, {"b1": 0, "b2": 15, "b3": 5}),
        (44, {"b1": 1, "b2": 1}),
        (44, {"b1": 2, "s": "Fuzz Author"}),
        (44, {"b1": 3, "d": 1440, "b2": 1}),
        (44, {"b1": 4, "r": 10, "c": 3}),
        (44, {"b1": 5}),
    ]))

    seeds.append(build("sheets_wild", [
        (46, {"s": "Sheet 2", "s2": "second sheet data"}),
        (46, {"s": "bad:name?*", "s2": "should fail validation"}),
        (47, {"r": 65535, "c": 16383, "s": "corner", "f": F}),
        (47, {"r": 60000, "c": 16000, "s": "sparse", "f": F}),
        (48, {"s": "AZ55"}),
    ]))

    seeds.append(build("a1_ignore_errors", [
        (48, {"s": "ZZ99"}),
        (48, {"s": "!!!not-a-cell"}),
        (37, {"s": "!!!", "s2": "@@@"}),
        (49, {"s": "A1:C10", "b": 0}),
        (49, {"s": "garbage range", "b": 5}),
    ]))

    seeds.append(build("long_utf8_strings", [
        (0, {"r": 0, "c": 0,
             "s": "\u00e9\u00e8\u00ea \u2713 \u4e2d\u6587 caf\u00e9 na\u00efve "
                  "emoji? \u2603 snow \u2014 dash", "f": F}),
        (3, {"r": 1, "c": 0, "s": "=\"\u00e9\u00e9\"&\"\u2713\"", "f": F}),
        (14, {"r": 2, "c": 0, "s": "\u00e9\u00e9\u00e9", "s2": "\u4e2d\u6587",
              "s3": "\u2713", "b": 2, "f": F}),
    ]))

    seeds.append(build("datetime_edges", [
        (11, {"r": 0, "c": 0, "y": 0, "mo": 1, "d": 0, "h": 0, "mi": 0,
              "s": 0, "f": F}),  # 1900-01-00 epoch edge
        (11, {"r": 1, "c": 0, "y": 0, "mo": 2, "d": 27, "h": 0, "mi": 0,
              "s": 0, "f": F}),  # 1900-02-28
        (11, {"r": 2, "c": 0, "y": 200, "mo": 11, "d": 30, "h": 23,
              "mi": 59, "s": 59, "f": F}),  # 2100 leap-non-edge
        (12, {"r": 3, "c": 0, "bytes": [0x00, 0x00, 0x00, 0x00, 0x80], "f": F}),
        (13, {"r": 4, "c": 0, "u": 0xFFFFFFFF, "bytes": [0xFF, 0xFF, 0xFF], "f": F}),
    ]))

    for name, data in seeds:
        path = os.path.join(args.out, name)
        with open(path, "wb") as fh:
            fh.write(data)
    print("wrote %d seeds to %s" % (len(seeds), args.out))


if __name__ == "__main__":
    main()
