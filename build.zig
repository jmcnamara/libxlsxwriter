const std = @import("std");
const Path = std.Build.LazyPath;

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be libcuted by an external
// runner.
pub fn build(b: *std.Build) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});

    const shared = b.option(bool, "SHARED_LIBRARY", "Build the Shared Library [default: false]") orelse false;
    const examples = b.option(bool, "BUILD_EXAMPLES", "Build libxlsxwriter examples [default: false]") orelse false;
    const tests = b.option(bool, "BUILD_TESTS", "Build libxlsxwriter tests [default: false]") orelse false;
    const dtoa = b.option(bool, "USE_DTOA_LIBRARY", "Use the locale independent third party Milo Yip DTOA library [default: off]") orelse false;
    const minizip = b.option(bool, "USE_SYSTEM_MINIZIP", "Use system minizip installation [default: off]") orelse false;
    const md5 = b.option(bool, "USE_OPENSSL_MD5", "Build libxlsxwriter with the OpenSSL MD5 lib [default: off]") orelse false;
    const stdtmpfile = b.option(bool, "USE_STANDARD_TMPFILE", "Use the C standard library's tmpfile() [default: off]") orelse false;

    const lib = if (shared) b.addSharedLibrary(.{
        .name = "xlsxwriter",
        .target = target,
        .optimize = optimize,
        .version = .{
            .major = 1,
            .minor = 1,
            .patch = 6,
        },
    }) else b.addStaticLibrary(.{
        .name = "xlsxwriter",
        .target = target,
        .optimize = optimize,
    });
    lib.pie = true;
    switch (optimize) {
        .Debug, .ReleaseSafe => lib.bundle_compiler_rt = true,
        else => lib.strip = true,
    }
    if (tests)
        lib.defineCMacro("TESTING", null);
    lib.addCSourceFiles(&.{
        "src/vml.c",
        "src/chartsheet.c",
        "src/theme.c",
        "src/content_types.c",
        "src/xmlwriter.c",
        "src/app.c",
        "src/styles.c",
        "src/core.c",
        "src/comment.c",
        "src/utility.c",
        "src/metadata.c",
        "src/custom.c",
        "src/hash_table.c",
        "src/relationships.c",
        "src/drawing.c",
        "src/chart.c",
        "src/shared_strings.c",
        "src/worksheet.c",
        "src/format.c",
        "src/table.c",
        "src/workbook.c",
        "src/packager.c",
    }, cflags);

    // minizip
    if (minizip) {
        lib.addCSourceFiles(switch (target.getOsTag()) {
            .windows => minizip_src ++ [_][]const u8{
                "third_party/minizip/iowin32.c",
            },
            else => minizip_src,
        }, cflags);
    }

    // zig-pkg: download & build zlib (to all targets)
    const zlib_dep = b.dependency("zlib", .{
        .optimize = optimize,
        .target = target,
    });
    const zlib = zlib_dep.artifact("z");
    lib.linkLibrary(zlib);
    lib.installLibraryHeaders(zlib);

    // md5
    if (!md5)
        lib.addCSourceFile(.{ .file = .{ .path = "third_party/md5/md5.c" }, .flags = cflags })
    else
        lib.linkSystemLibrary("crypto");

    // dtoa
    if (dtoa)
        lib.addCSourceFile(.{ .file = Path.relative("third_party/dtoa/emyg_dtoa.c"), .flags = cflags });

    // tmpfileplus
    if (stdtmpfile)
        lib.addCSourceFile(.{ .file = .{ .path = "third_party/tmpfileplus/tmpfileplus.c" }, .flags = cflags })
    else
        lib.defineCMacro("USE_STANDARD_TMPFILE", null);

    lib.addIncludePath(Path.relative("include"));
    lib.addIncludePath(Path.relative("third_party"));
    lib.linkLibC();

    // get headers on include to zig-out/include
    lib.installHeadersDirectory("include", "");

    // get binaries on zig-cache to zig-out
    b.installArtifact(lib);

    // build examples
    if (examples) {
        buildExe(b, .{
            .lib = lib,
            .path = "examples/anatomy.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/array_formula.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/autofilter.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/background.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/chart_area.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/chart_column.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/data_validate.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/hello.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/watermark.c",
        });
        buildExe(b, .{
            .lib = lib,
            .path = "examples/worksheet_protection.c",
        });
    }
    // build tests
    if (tests) {
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/app/test_app.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/chart/test_chart.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/chartsheet/test_chartsheet.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/content_types/test_content_types.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/content_types/test_content_types_write_default.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/content_types/test_content_types_write_override.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/relationships/test_relationships.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/app/test_app_xml_declaration.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/relationships/test_relationships_xml_declaration.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/custom/test_custom_xml_declaration.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/metadata/test_metadata_xml_declaration.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/core/test_core_xml_declaration.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/sst/test_shared_strings.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/workbook/test_workbook.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/xmlwriter/test_xmlwriter.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/table/test_table01.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/table/test_table02.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/table/test_table03.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/table/test_table04.c",
        });
        buildTest(b, .{
            .lib = lib,
            .path = "test/unit/styles/test_styles_write_border.c",
        });
    }
}

fn buildExe(b: *std.Build, info: BuildInfo) void {
    const exe = b.addExecutable(.{
        .name = info.filename(),
        .optimize = info.lib.optimize,
        .target = info.lib.target,
    });
    exe.addCSourceFile(.{ .file = Path.relative(info.path), .flags = cflags });
    exe.linkLibrary(info.lib);
    for (info.lib.include_dirs.items) |include| {
        exe.include_dirs.append(include) catch {};
    }
    exe.linkLibC();
    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step(
        b.fmt("{s}", .{info.filename()}),
        b.fmt("Run the {s} test", .{info.filename()}),
    );
    run_step.dependOn(&run_cmd.step);
}

fn buildTest(b: *std.Build, info: BuildInfo) void {
    const exe = b.addExecutable(.{
        .name = info.filename(),
        .optimize = info.lib.optimize,
        .target = info.lib.target,
    });
    exe.defineCMacro("TESTING", null);
    exe.addCSourceFile(.{ .file = Path.relative(info.path), .flags = cflags });
    exe.addCSourceFile(.{ .file = Path.relative("test/unit/test_all.c"), .flags = cflags });
    exe.addIncludePath(Path.relative("test/unit"));
    for (info.lib.include_dirs.items) |include| {
        exe.include_dirs.append(include) catch {};
    }
    exe.linkLibrary(info.lib);
    exe.linkLibC();
    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step(
        b.fmt("{s}", .{info.filename()}),
        b.fmt("Run the {s} test", .{info.filename()}),
    );
    run_step.dependOn(&run_cmd.step);
}

const cflags = &.{
    "-std=c89",
    "-Wall",
    "-Wextra",
    "-Wno-unused-parameter",
};
const minizip_src: []const []const u8 = &.{
    "third_party/minizip/ioapi.c",
    "third_party/minizip/mztools.c",
    "third_party/minizip/unzip.c",
    "third_party/minizip/zip.c",
};

const BuildInfo = struct {
    lib: *std.Build.Step.Compile,
    path: []const u8,

    fn filename(self: BuildInfo) []const u8 {
        var split = std.mem.splitSequence(u8, std.fs.path.basename(self.path), ".");
        return split.first();
    }
};
