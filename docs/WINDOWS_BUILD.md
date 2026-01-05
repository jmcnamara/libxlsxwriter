# Building libxlsxwriter on Windows with Visual Studio 2026

This guide provides step-by-step instructions for building libxlsxwriter as a DLL on Windows using Visual Studio 18 2026, with static linking to zlib.

## Prerequisites

- Visual Studio 2026 (VS 18) with C++ Desktop Development workload
- CMake 3.15 or later (included with Visual Studio or install separately)
- Git for Windows

## Setting Up the Build Environment

Before building, you must initialize the Visual C++ environment for your target architecture.

### For 32-bit Builds

Open a **Command Prompt** and run:

```cmd
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat"
```

### For 64-bit Builds

Open a **Command Prompt** and run:

```cmd
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
```

> **Note:** If you have Visual Studio Professional or Enterprise edition, replace `Community` with `Professional` or `Enterprise` in the path.

> **Important:** You must use separate command prompt windows for 32-bit and 64-bit builds, or re-run the appropriate vcvars batch file when switching architectures.

## Directory Structure

After following these instructions, you will have:

```
C:\dev\
├── zlib\
│   ├── build32\          # 32-bit zlib static library
│   └── build64\          # 64-bit zlib static library
└── libxlsxwriter\
    ├── build32\          # 32-bit libxlsxwriter DLL
    └── build64\          # 64-bit libxlsxwriter DLL
```

## Step 1: Create Working Directory

Open **Developer Command Prompt for VS 2026** or **PowerShell** and run:

```cmd
mkdir C:\dev
cd C:\dev
```

## Step 2: Clone and Build zlib

### Clone zlib Repository

```cmd
git clone https://github.com/madler/zlib.git
cd zlib
```

### Build 32-bit Static zlib Library

First, set up the 32-bit build environment:

```cmd
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat"
```

Then build zlib:

```cmd
mkdir build32
cd build32

cmake .. -G "Visual Studio 18 2026" -A Win32 ^
    -DCMAKE_INSTALL_PREFIX=C:/dev/zlib/install32 ^
    -DBUILD_SHARED_LIBS=OFF

cmake --build . --config Release
cmake --install . --config Release

cd ..
```

### Build 64-bit Static zlib Library

Set up the 64-bit build environment (open a new command prompt or re-initialize):

```cmd
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
```

Then build zlib:

```cmd
cd C:\dev\zlib
mkdir build64
cd build64

cmake .. -G "Visual Studio 18 2026" -A x64 ^
    -DCMAKE_INSTALL_PREFIX=C:/dev/zlib/install64 ^
    -DBUILD_SHARED_LIBS=OFF

cmake --build . --config Release
cmake --install . --config Release

cd ../..
```

## Step 3: Clone and Build libxlsxwriter

### Clone libxlsxwriter Repository

```cmd
git clone https://github.com/jmcnamara/libxlsxwriter.git
cd libxlsxwriter
```

### Build 32-bit libxlsxwriter DLL

Set up the 32-bit build environment:

```cmd
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat"
```

Then build libxlsxwriter:

```cmd
cd C:\dev\libxlsxwriter
mkdir build32
cd build32

cmake .. -G "Visual Studio 18 2026" -A Win32 ^
    -DZLIB_ROOT=C:/dev/zlib/install32 ^
    -DZLIB_LIBRARY=C:/dev/zlib/install32/lib/zlibstatic.lib ^
    -DZLIB_INCLUDE_DIR=C:/dev/zlib/install32/include ^
    -DBUILD_SHARED_LIBS=ON ^
    -DUSE_STATIC_MSVC_RUNTIME=OFF

cmake --build . --config Release

cd ..
```

The 32-bit DLL will be located at: `build32\Release\xlsxwriter.dll`

### Build 64-bit libxlsxwriter DLL

Set up the 64-bit build environment (open a new command prompt or re-initialize):

```cmd
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
```

Then build libxlsxwriter:

```cmd
cd C:\dev\libxlsxwriter
mkdir build64
cd build64

cmake .. -G "Visual Studio 18 2026" -A x64 ^
    -DZLIB_ROOT=C:/dev/zlib/install64 ^
    -DZLIB_LIBRARY=C:/dev/zlib/install64/lib/zlibstatic.lib ^
    -DZLIB_INCLUDE_DIR=C:/dev/zlib/install64/include ^
    -DBUILD_SHARED_LIBS=ON ^
    -DUSE_STATIC_MSVC_RUNTIME=OFF

cmake --build . --config Release

cd ..
```

The 64-bit DLL will be located at: `build64\Release\xlsxwriter.dll`

## Step 4: Verify Build Output

After successful builds, you should have the following files:

### 32-bit Build
```
libxlsxwriter\build32\Release\
├── xlsxwriter.dll          # Dynamic library
├── xlsxwriter.lib          # Import library for linking
└── xlsxwriter.pdb          # Debug symbols (optional)
```

### 64-bit Build
```
libxlsxwriter\build64\Release\
├── xlsxwriter.dll          # Dynamic library
├── xlsxwriter.lib          # Import library for linking
└── xlsxwriter.pdb          # Debug symbols (optional)
```

## Optional: Build Static Library with Static MSVC Runtime

If you prefer a fully static build (no C runtime DLL dependency), use these options:

```cmd
cmake .. -G "Visual Studio 18 2026" -A x64 ^
    -DZLIB_ROOT=C:/dev/zlib/install64 ^
    -DZLIB_LIBRARY=C:/dev/zlib/install64/lib/zlibstatic.lib ^
    -DZLIB_INCLUDE_DIR=C:/dev/zlib/install64/include ^
    -DBUILD_SHARED_LIBS=OFF ^
    -DUSE_STATIC_MSVC_RUNTIME=ON

cmake --build . --config Release
```

## Optional: Build Debug Configuration

To build debug versions, replace `Release` with `Debug`:

```cmd
cmake --build . --config Debug
```

## Using the Library in Your Project

### For DLL Builds

1. Copy `xlsxwriter.dll` to your application directory or system PATH
2. Link against `xlsxwriter.lib` (import library)
3. Include headers from `libxlsxwriter\include\`

### CMake Integration

```cmake
find_package(ZLIB REQUIRED)

add_subdirectory(path/to/libxlsxwriter)

target_link_libraries(your_app PRIVATE xlsxwriter)
```

### Visual Studio Project Settings

1. Add to **Additional Include Directories**: `C:\dev\libxlsxwriter\include`
2. Add to **Additional Library Directories**: `C:\dev\libxlsxwriter\build64\Release`
3. Add to **Additional Dependencies**: `xlsxwriter.lib`

## Troubleshooting

### CMake Cannot Find zlib

Ensure the paths are correct and use forward slashes (`/`) in CMake paths:
```cmd
-DZLIB_ROOT=C:/dev/zlib/install64
```

### Linker Errors with zlib

If you get unresolved external symbols for zlib functions, verify:
- You're linking against `zlibstatic.lib` (not `zlib.lib`)
- The architecture matches (Win32 with Win32, x64 with x64)

### Missing vcruntime DLLs

If the DLL requires Visual C++ runtime, either:
- Install the Visual C++ Redistributable on target machines
- Build with `-DUSE_STATIC_MSVC_RUNTIME=ON` for static runtime

### Build Errors with Examples

To skip building examples:
```cmd
cmake .. -DBUILD_EXAMPLES=OFF ...
```

## Complete Build Script

Save as `build_all.bat`:

```batch
@echo off
setlocal

set DEV_DIR=C:\dev
set GENERATOR="Visual Studio 18 2026"
set VCVARS_PATH=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build

:: Create dev directory
mkdir %DEV_DIR% 2>nul
cd /d %DEV_DIR%

:: Clone zlib
echo === Cloning zlib ===
git clone https://github.com/madler/zlib.git

:: Clone libxlsxwriter
echo === Cloning libxlsxwriter ===
git clone https://github.com/jmcnamara/libxlsxwriter.git

:: ============================================================
:: Build 32-bit versions
:: ============================================================
echo === Setting up 32-bit build environment ===
call "%VCVARS_PATH%\vcvars32.bat"

echo === Building 32-bit zlib ===
cd /d %DEV_DIR%\zlib
mkdir build32 2>nul
cd build32
cmake .. -G %GENERATOR% -A Win32 -DCMAKE_INSTALL_PREFIX=%DEV_DIR%/zlib/install32 -DBUILD_SHARED_LIBS=OFF
cmake --build . --config Release
cmake --install . --config Release

echo === Building 32-bit libxlsxwriter ===
cd /d %DEV_DIR%\libxlsxwriter
mkdir build32 2>nul
cd build32
cmake .. -G %GENERATOR% -A Win32 ^
    -DZLIB_ROOT=%DEV_DIR%/zlib/install32 ^
    -DZLIB_LIBRARY=%DEV_DIR%/zlib/install32/lib/zlibstatic.lib ^
    -DZLIB_INCLUDE_DIR=%DEV_DIR%/zlib/install32/include ^
    -DBUILD_SHARED_LIBS=ON
cmake --build . --config Release

:: ============================================================
:: Build 64-bit versions
:: ============================================================
echo === Setting up 64-bit build environment ===
call "%VCVARS_PATH%\vcvars64.bat"

echo === Building 64-bit zlib ===
cd /d %DEV_DIR%\zlib
mkdir build64 2>nul
cd build64
cmake .. -G %GENERATOR% -A x64 -DCMAKE_INSTALL_PREFIX=%DEV_DIR%/zlib/install64 -DBUILD_SHARED_LIBS=OFF
cmake --build . --config Release
cmake --install . --config Release

echo === Building 64-bit libxlsxwriter ===
cd /d %DEV_DIR%\libxlsxwriter
mkdir build64 2>nul
cd build64
cmake .. -G %GENERATOR% -A x64 ^
    -DZLIB_ROOT=%DEV_DIR%/zlib/install64 ^
    -DZLIB_LIBRARY=%DEV_DIR%/zlib/install64/lib/zlibstatic.lib ^
    -DZLIB_INCLUDE_DIR=%DEV_DIR%/zlib/install64/include ^
    -DBUILD_SHARED_LIBS=ON
cmake --build . --config Release

:: ============================================================
:: Done
:: ============================================================
echo.
echo === Build Complete ===
echo 32-bit DLL: %DEV_DIR%\libxlsxwriter\build32\Release\xlsxwriter.dll
echo 64-bit DLL: %DEV_DIR%\libxlsxwriter\build64\Release\xlsxwriter.dll
echo.

pause
```

## Additional Resources

- [libxlsxwriter Documentation](https://libxlsxwriter.github.io/)
- [CMake Documentation](https://cmake.org/documentation/)
- [zlib Home Page](https://zlib.net/)
