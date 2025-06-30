Pod::Spec.new do |s|
  s.name                  = "libxlsxwriter"
  s.version               = "1.2.3"
  s.summary               = "Libxlsxwriter: A C library for creating Excel XLSX files."
  s.ios.deployment_target = "12.0"
  s.osx.deployment_target = "10.13"

  s.description           = <<-DESC
                            Libxlsxwriter is a C library that can be used to write text, numbers, formulas and hyperlinks to multiple worksheets in an Excel 2007+ XLSX file.

                            It supports features such as:

                            * 100% compatible Excel XLSX files.
                            * Full Excel formatting.
                            * Merged cells.
                            * Defined names.
                            * Autofilters.
                            * Charts.
                            * Data validation and dropdown lists.
                            * Conditional formatting.
                            * Worksheet PNG/JPEG/GIF images.
                            * Cell comments.
                            * Support for adding Macros.
                            * Memory optimization mode for writing large files.
                            * Source code available on [GitHub](https://github.com/jmcnamara/libxlsxwriter).
                            * FreeBSD license.
                            * ANSI C.
                            * Works with GCC, Clang, Xcode, MSVC 2015, ICC, TCC, MinGW, MingGW-w64/32.
                            * Works on Linux, FreeBSD, OpenBSD, OS X, iOS and Windows. Also works on MSYS/MSYS2 and Cygwin.
                            * Compiles for 32 and 64 bit.
                            * Compiles and works on big and little endian systems.
                            * The only dependency is on `zlib`.
                            DESC

  s.homepage              = "http://libxlsxwriter.github.io"
  s.documentation_url     = "http://libxlsxwriter.github.io"
  s.license               = "FreeBSD"
  s.author                = { "John McNamara" => "jmcnamara@cpan.org" }

  s.source                = { :git => "https://github.com/jmcnamara/libxlsxwriter.git", :tag => "v" + s.version.to_s }
  s.source_files          = "src/*.c", "third_party/**/{zip.c,ioapi.c,tmpfileplus.c,md5.c}", "include/**/*.h"
  s.preserve_paths        = [ 'third_party/**/*.h' ]
  s.header_dir            = "xlsxwriter"
  s.header_mappings_dir   = "include"
  s.library               = "z"
  s.compiler_flags        = "-DNOCRYPT=1", "-DNOUNCRYPT=1"
  s.pod_target_xcconfig   = { 'USER_HEADER_SEARCH_PATHS' => '${PODS_ROOT}/libxlsxwriter/include' }
  s.module_map            = "cocoapods/libxlsxwriter.modulemap"
  s.prepare_command       = <<-CMD
                            cp cocoapods/libxlsxwriter-umbrella.h include/xlsxwriter/libxlsxwriter-umbrella.h
                          CMD
end
