// swift-tools-version: 5.7
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "libxlsxwriter",
    products: [
        .library(
            name: "libxlsxwriter",
            targets: ["libxlsxwriter"]),
    ],
    targets: [
        .target(
            name: "libxlsxwriter",
            path: ".",
            exclude: [
                "src/Makefile",
            ],
            sources: [
                "include",
                "src",
                "third_party/minizip/zip.c",
                "third_party/minizip/ioapi.c",
                "third_party/tmpfileplus/tmpfileplus.c",
                "third_party/md5/md5.c"
            ],
            publicHeadersPath: "include",
            linkerSettings: [
                .linkedLibrary("z")
            ]),
        .testTarget(
            name: "libxlsxwritertests",
            dependencies: ["libxlsxwriter"],
            path: ".",
            sources: ["test/swift"],
            linkerSettings: [
                .linkedLibrary("z")
            ]
        )
    ]
)
