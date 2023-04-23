const std = @import("std");

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

    const shared = b.option(bool, "Shared", "Build the Shared Library [default: false]") orelse false;

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
    }, &.{
        "-Wall",
        "-Wextra",
    });
    lib.addIncludePath("include");
    lib.addIncludePath("third_party");
    lib.linkLibC();
    lib.installHeadersDirectory("include", "");
    b.installArtifact(lib);
}
