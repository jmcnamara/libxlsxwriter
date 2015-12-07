Pod::Spec.new do |s|
  s.name                  = "libxlsxwriter"
  s.version               = "0.1.8"
  s.summary               = "A C library for creating Excel XLSX files."
  s.ios.deployment_target = "6.0"
  s.osx.deployment_target = "10.8"

  s.description           = <<-DESC
                            Libxlsxwriter is a C library that can be used to write text, numbers, formulas and hyperlinks to multiple worksheets in an Excel 2007+ XLSX file.

                            It supports features such as:

                            * 100% compatible Excel XLSX files.
                            * Full Excel formatting.
                            * Merged cells.
                            * Defined names.
                            * Autofilters.
                            * Memory optimisation mode for writing large files.
                            * Source code available on [GitHub](https://github.com/jmcnamara/libxlsxwriter).
                            * FreeBSD @ref license.
                            * ANSI C.
                            * Works with GCC 4.x, GCC 5.x, Clang, Xcode, MSVS 2015, ICC and TCC.
                            * Works on Linux, FreeBSD, OS X, iOS and Windows.
                            * The only dependency is on `zlib`.
                            DESC

  s.homepage              = "http://libxlsxwriter.github.io"
  s.documentation_url     = "http://libxlsxwriter.github.io"
  s.license               = "FreeBSD"
  s.author                = { "John McNamara" => "jmcnamara@cpan.org" }

  s.source                = { :git => "https://github.com/jmcnamara/libxlsxwriter.git", :tag => "RELEASE_" + s.version.to_s }
  s.source_files          = "src/**/*.c", "third_party/**/{zip.c,ioapi.c}", "include/**/*.h"

  s.header_dir            = "xlsxwriter"
  s.header_mappings_dir   = "include/xlsxwriter"
  s.library               = "z"
  
  s.prepare_command = <<-CMD
                        sed -i '' 's#xlsxwriter/third_party/queue.h#queue.h#g' ./include/xlsxwriter/*.h
                        sed -i '' 's#xlsxwriter/third_party/minizip/##g' ./include/xlsxwriter/*.h
                   CMD
end
