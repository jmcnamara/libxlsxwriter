Pod::Spec.new do |s|
  s.name                  = "libxlsxwriter"
  s.version               = "0.3.1"
  s.summary               = "Libxlsxwriter: A C library for creating Excel XLSX files."
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
                            * Worksheet PNG/JPEG images.
                            * Memory optimisation mode for writing large files.
                            * Source code available on [GitHub](https://github.com/jmcnamara/libxlsxwriter).
                            * FreeBSD ref license.
                            * ANSI C.
                            * Works with GCC 4.x, GCC 5.x, Clang, Xcode, MSVC 2015, ICC and TCC.
                            * Works on Linux, FreeBSD, OS X, iOS and Windows.
                            * The only dependency is on `zlib`.
                            DESC

  s.homepage              = "http://libxlsxwriter.github.io"
  s.documentation_url     = "http://libxlsxwriter.github.io"
  s.license               = "FreeBSD"
  s.author                = { "John McNamara" => "jmcnamara@cpan.org" }
  s.source                = { :git => "https://github.com/jmcnamara/libxlsxwriter.git", :tag => "RELEASE_" + s.version.to_s }

  s.header_dir            = "xlsxwriter"
  s.header_mappings_dir   = "include/xlsxwriter"
  s.library               = "z"
  s.compiler_flags        = "-DNOCRYPT=1", "-DNOUNCRYPT=1"

  
  # On the one hand, all the headers need to be inside the xlsxwriter directory for the pod to be built as a
  # module (i.e. framework). On the other hand, the xlsxwriter.h header needs to be at the top level for it
  # to be easily imported when building the pod as a static library. That being said, since we can't use
  # prepare_command in a subspec, we do the copy here and then exclude the unwanted file in each subspec.

  s.prepare_command       = <<-CMD
                            cp include/xlsxwriter.h include/xlsxwriter/xlsxwriter_module.h
                          CMD


  s.default_subspecs = 'Default'

  s.subspec 'Default' do |ss|
    ss.source_files           = "src/**/*.c", "third_party/**/{zip.c,ioapi.c}", "include/**/*.h"

    # Building as a static library: we don't need the "module" header inside the xlsxwriter directory
    # (even though leaving it there wouldn't harm). 
    ss.exclude_files          = "include/xlsxwriter/xlsxwriter_module.h"
  end

  s.subspec 'Framework' do |ss|
    ss.source_files           = "src/**/*.c", "third_party/**/{zip.c,ioapi.c}", "include/**/*.h"
    ss.pod_target_xcconfig    = { 'USER_HEADER_SEARCH_PATHS' => '${PODS_ROOT}/libxlsxwriter/include' }
  
    # Building as a framework: we don't want to have an header outside of the xlsxwriter directory, since
    # it would end outside of the framework.
    ss.exclude_files          = "include/xlsxwriter.h"
  end
end