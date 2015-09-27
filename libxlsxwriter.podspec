Pod::Spec.new do |s|
  s.name                = "libxlsxwriter"
  s.version             = "0.1.6"
  s.summary             = "A C library for creating Excel XLSX files."
  s.platform            = :ios
  s.platform            = :osx

  s.description         = <<-DESC
                          Libxlsxwriter is a C library that can be used to write text, numbers, formulas and hyperlinks to multiple worksheets in an Excel 2007+ XLSX file.

                          It supports features such as:

                          * 100% compatible Excel XLSX files
                          * Full Excel formatting
                          * Merged cells
                          * Autofilters
                          * Defined names
                          * Memory optimisation mode for writing large files
                          * Source code available on GitHub
                          * FreeBSD license
                          * ANSI C
                          * Works with GCC 4.4, 4.6, 4.7, 4.8, 4.9, Clang, ICC and TCC.
                          * Works on Linux, FreeBSD, OS X and iOS.
                          * The only dependency is on zlib
                          DESC

  s.homepage            = "http://github.com/jmcnamara/libxlsxwriter"
  s.documentation_url   = "http://libxlsxwriter.github.io"
  s.license             = "FreeBSD"
  s.author              = { "John McNamara" => "jmcnamara@cpan.org" }

  s.source              = { :git => "https://github.com/jmcnamara/libxlsxwriter.git", :tag => "RELEASE_0.1.6" }
  s.source_files        = "src/**/*.c", "third_party/**/{zip.c,ioapi.c}", "include/**/*.h"

  s.header_dir          = "xlsxwriter"
  s.header_mappings_dir = "include/xlsxwriter"
  s.library             = "z"
  s.compiler_flags      = "-DNOCRYPT=1", "-DNOUNCRYPT=1"
end
