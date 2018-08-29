# building_rime

![CC License](https://github.com/buaabyl/building_rime/raw/master/cc.png)
This work is licensed under a Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License.

Table of Contents
- [build zlib](#build-zlib)
- [build marisa](#build-marisa)
- [build boost](#build-boost)
- [build opencc](#build-opencc)
- [build leveldb](#build-leveldb)
- [build yaml-cpp](#build-yaml-cpp)
- [build glog](#build-glog)
- [build pthreads](#build-pthreads)
- [build rime](#build-rime)

-----


This repository record how to build rime step by step.

toolchain:

- [Microsoft Visual C++ Build Tools 2015.3](http://landinghub.visualstudio.com/visual-cpp-build-tools)
- [CMake 3.8.x](https://cmake.org/files/v3.8/cmake-3.8.2-win64-x64.zip)


I will install all the library to `D:\syslib`

```
set SYSLIB=D:\syslib
set SYSLIB_INCLUDE_PATH=D:\syslib\include
set SYSLIB_LIB32_PATH=D:\syslib\lib32
set SYSLIB_LIB64_PATH=D:\syslib\lib64
```


`link type`: Microsoft VC have this mixed build type:
- MT: link runtime library statically, vcruntime140.dll or api-ms-xxxx.dll.
- MD: link runtime library dynamically, depend on vcruntime140.dll and api-ms-xxxx.dll.
- x86: address mode is i386.
- x86_64: alias amd64 or x64, address mode is 64bits.
- static library: build a library as `.lib`.
- dynamic library: build a library as `.dll` + `.lib`.

For rime will built as `rime.dll`, we must have `/MD` runtime, no matter library is static or dynamic.
But I perfer static library.

## build zlib
modify zconf.h make sure not include `unistd.h`
the default `Makefiles.msc` using `/MD` flags, so no need to change.

build 32bits

```
cd zlib-1.2.3
nmake -f win32/Makefile.msc
cp zconf.h D:\syslib\include
cp zlib.h D:\syslib\include
cp zlib.lib D:\syslib\lib32\zlib1.lib
```

build 64bits

```
cd zlib-1.2.3
nmake -f win32/Makefile.msc
cp zlib.lib D:\syslib\lib64\zlib1.lib
```

## build marisa
marisa-trunk [https://github.com/s-yata/marisa-trie](marisa-master)

build 32bits

```
cp include\marisa.h D:\syslib\include
cp -r include\marisa D:\syslib\include
cl /nologo /MD /I include /I lib /c /EHsc \
    /Fo./ \
    lib\marisa\*.cc \
    lib\marisa\grimoire\io\*.cc \
    lib\marisa\grimoire\trie\*.cc \
    lib\marisa\grimoire\vector\*.cc 
lib /nologo *.obj /out:D:\syslib\lib32\marisa.lib
```

build 64bits

```
cl /nologo /MD /I include /I lib /c /EHsc \
    /Fo./ \
    lib\marisa\*.cc \
    lib\marisa\grimoire\io\*.cc \
    lib\marisa\grimoire\trie\*.cc \
    lib\marisa\grimoire\vector\*.cc 
lib /nologo *.obj /out:D:\syslib\lib64\marisa.lib
```


## build boost
I choose [https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.7z](boost_1_64_0.7z).

boost library naming is a bit confused, have a look:

`libboost_system-vc140-mt-1_64.lib` or `libboost_system-vc140-mt-gd-1_64.lib`
- libboost_system: main library name
- vc140: build compiler version
- mt: means boost is static link, vcruntime is dynamic link!
- gd: debug version
- 1_64: boost version

`libboost_system-vc140-mt-s-1_64.lib` or `libboost_system-vc140-mt-gds-1_64.lib`
- libboost_system: main library name
- vc140: build compiler version
- mt: means boost is static link 
- gd: debug version
- s: vcruntime is static link too
- 1_64: boost version

Boost is build with `bjam.exe` tools, let's build it first.
Tips: make sure `ucrtbased.dll` in `PATH`, or you will got error at the very beginning!

```
7za x boost_1_64_0.7z
cd boost_1_64_0
call `bootstrap.bat`, we got `bjam.exe`
```

build x86+MD:

```
bjam toolset=msvc-14.0 variant=release link=static runtime-link=shared architecture=x86 address-model=32 stage
mv stage\lib stage\lib32-md
```

build x86+MT:

```
bjam toolset=msvc-14.0 variant=release link=static runtime-link=static architecture=x86 address-model=32 stage
mv stage\lib stage\lib32-mt
```

build amd64+MD:

```
bjam toolset=msvc-14.0 variant=release link=static runtime-link=shared architecture=x86 address-model=64 stage
mv stage\lib stage\lib64-md
```

build amd64+MT:

```
bjam toolset=msvc-14.0 variant=release link=static runtime-link=static architecture=x86 address-model=64 stage
mv stage\lib stage\lib64-mt
```

copy `boost_1_64\boost` to `D:\syslib\include`

copy `boost_1_64_0\stage\lib32-md` to `D:\syslib\lib32`

copy `boost_1_64_0\stage\lib64-md` to `D:\syslib\lib64`.


## build opencc
[Opencc](https://github.com/BYVoid/OpenCC), 
I download this release [opencc-1.0.5](https://github.com/BYVoid/OpenCC/releases/tag/ver.1.0.5).

please convert `OpenCC-ver.1.0.5\src\PhraseExtract.cpp` to `UTF-8 with BOM` first, or you will fatal...

build 32bits

```
cd OpenCC-ver.1.0.5/build32
cmake -G "NMake Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_SYSTEM_INCLUDE_PATH=D:\syslib\include \
    -DCMAKE_SYSTEM_LIBRARY_PATH=D:\syslib\lib32 \
    -DCMAKE_INSTALL_PREFIX=D:\syslib \
    ..
nmake
nmake install
mv D:\syslib\lib\opencc.lib D:\syslib\lib32\opencc.lib
mv D:\syslib\bin D:\syslib\bin32
```

build 64bits

```
cd OpenCC-ver.1.0.5/build64
cmake -G "NMake Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_SYSTEM_INCLUDE_PATH=D:\syslib\include \
    -DCMAKE_SYSTEM_LIBRARY_PATH=D:\syslib\lib64 \
    -DCMAKE_INSTALL_PREFIX=D:\syslib \
    ..
nmake
nmake install
mv D:\syslib\lib\opencc.lib D:\syslib\lib64\opencc.lib
mv D:\syslib\bin D:\syslib\bin64
```

we can check opencc.exe with `pedump` tool, which will list all depend DLLs.

```
pedump d:\syslib\bin32\opencc.exe
d:\syslib\bin\opencc.exe: PE32 executable (32bits) (console) i386 (Wed Jun 07 15:38:01 2017)
  Import KERNEL32.dll
  Import MSVCP140.dll
  Import VCRUNTIME140.dll
  Import api-ms-win-crt-runtime-l1-1-0.dll
  Import api-ms-win-crt-stdio-l1-1-0.dll
  Import api-ms-win-crt-filesystem-l1-1-0.dll
  Import api-ms-win-crt-string-l1-1-0.dll
  Import api-ms-win-crt-heap-l1-1-0.dll
  Import api-ms-win-crt-math-l1-1-0.dll
  Import api-ms-win-crt-locale-l1-1-0.dll

pedump d:\syslib\bin64\opencc.exe
d:\syslib\bin64\opencc.exe: PE32+ executable (console) x86_64 (Wed Jun 07 15:46:22 2017)
  Import KERNEL32.dll
  Import MSVCP140.dll
  Import VCRUNTIME140.dll
  Import api-ms-win-crt-runtime-l1-1-0.dll
  Import api-ms-win-crt-stdio-l1-1-0.dll
  Import api-ms-win-crt-filesystem-l1-1-0.dll
  Import api-ms-win-crt-string-l1-1-0.dll
  Import api-ms-win-crt-heap-l1-1-0.dll
  Import api-ms-win-crt-math-l1-1-0.dll
  Import api-ms-win-crt-locale-l1-1-0.dll
```


## build leveldb
leveldb offical repo not support windows, we need 3rd porting.
[leveldb-windows 915d663 on Jul 29, 2015](https://github.com/bureau14/leveldb)

build 32bits

```
cd leveldb-1.18/build32
cmake -G "NMake Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_SYSTEM_INCLUDE_PATH=D:\syslib\include \
    -DCMAKE_SYSTEM_LIBRARY_PATH=D:\syslib\lib32 \
    -DCMAKE_INSTALL_PREFIX=D:\syslib \
    ..
nmake
nmake install
mv D:\syslib\bin\leveldbutil.exe D:\syslib\bin32\leveldbutil.exe
cp leveldb.lib D:\syslib\lib32
cp -r ..\include\leveldb D:\syslib\include
```

build 64bits

```
cd leveldb-1.18/build64
cmake -G "NMake Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_SYSTEM_INCLUDE_PATH=D:\syslib\include \
    -DCMAKE_SYSTEM_LIBRARY_PATH=D:\syslib\lib64 \
    -DCMAKE_INSTALL_PREFIX=D:\syslib \
    ..
nmake
nmake install
mv D:\syslib\bin\leveldbutil.exe D:\syslib\bin64\leveldbutil.exe
cp leveldb.lib D:\syslib\lib64
```


## build yaml-cpp
[Yaml-cpp 0.5.3](https://github.com/jbeder/yaml-cpp/releases/tag/yaml-cpp-0.5.3).

build 32bits first, I don't need unitest of this library, so no need to build Gtest and Gmock.
and just ingore test at `CMakeLists.txt`

```
if(YAML_CPP_BUILD_TOOLS)
-   add_subdirectory(test)
+   # add_subdirectory(test)
    add_subdirectory(util)
endif()
```

build 32bits

```
cd yaml-cpp-yaml-cpp-0.5.3/build32
cmake -G "NMake Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_SYSTEM_INCLUDE_PATH=D:\syslib\include \
    -DCMAKE_SYSTEM_LIBRARY_PATH=D:\syslib\lib32 \
    -DCMAKE_INSTALL_PREFIX=D:\syslib \
    ..
nmake
nmake install
mv D:\syslib\lib\libyaml-cppmd.lib D:\syslib\lib32\libyaml-cppmd.lib
mklink D:\syslib\lib32\lyaml-cpp.lib D:\syslib\lib32\llibyaml-cppmd.lib
```

build 64bits

```
cd yaml-cpp-yaml-cpp-0.5.3/build64
cmake -G "NMake Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_SYSTEM_INCLUDE_PATH=D:\syslib\include \
    -DCMAKE_SYSTEM_LIBRARY_PATH=D:\syslib\lib64 \
    -DCMAKE_INSTALL_PREFIX=D:\syslib \
    ..
nmake
nmake install
mv D:\syslib\lib\libyaml-cppmd.lib D:\syslib\lib64\libyaml-cppmd.lib
mklink D:\syslib\lib64\lyaml-cpp.lib D:\syslib\lib64\llibyaml-cppmd.lib
```

## build glog
modify `glog-0.3.5\src\windows\config.h` and `glog-0.3.5\src\glog\config.h.in`

```
- #ifndef GOOGLE_GLOG_DLL_DECL
- # define GOOGLE_GLOG_IS_A_DLL  1   /* not set if you're statically linking */
- # define GOOGLE_GLOG_DLL_DECL  __declspec(dllexport)
- # define GOOGLE_GLOG_DLL_DECL_FOR_UNITTESTS  __declspec(dllimport)
- #endif
+ #define GOOGLE_GLOG_DLL_DECL
+ #define GOOGLE_GLOG_DLL_DECL_FOR_UNITTESTS
```

modify `glog-0.3.5\src\windows\glog\logging.h`

```
- #ifndef GOOGLE_GLOG_DLL_DECL
- # if defined(_WIN32) && !defined(__CYGWIN__)
- #   define GOOGLE_GLOG_DLL_DECL  __declspec(dllimport)
- # else
- #   define GOOGLE_GLOG_DLL_DECL
- # endif
- #endif
+ #define GOOGLE_GLOG_DLL_DECL
```

build 32bits

```
cd glog-0.3.5\build32
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=D:\syslib ..
nmake
nmake install
mv D:\syslib\lib\glog.lib D:\syslib\lib32\glog.lib
```

build 64bits

```
cd glog-0.3.5\build64
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=D:\syslib ..
nmake
nmake install
mv D:\syslib\lib\glog.lib D:\syslib\lib64\glog.lib
```

## build pthreads
[pthreads-w32](ftp://sourceware.org/pub/pthreads-win32/pthreads-w32-2-9-1-release.zip)
can be used, but as an optional library.

build 32bits

```
cd pthreads.2
cp pthread.h D:\syslib\include
cp sched.h D:\syslib\include
cp semaphore.h D:\syslib\include
cl /nologo /I . /MD /DHAVE_STRUCT_TIMESPEC /DPTW32_STATIC_LIB /c pthread.c
lib /nologo pthread.obj /out:D:\syslib\lib32\pthread.lib
```

build 64bits

```
cl /nologo /I . /MD /DHAVE_STRUCT_TIMESPEC /DPTW32_STATIC_LIB /c pthread.c
lib /nologo pthread.obj /out:D:\syslib\lib64\pthread.lib
```

## build rime
[rime-1.2.9](https://github.com/rime/librime/releases/tag/rime-1.2.9)

modify `CMakeLists.txt`, `cmake/FindGlog.cmake`, `cmake/FindOpencc.cmake`, `src/gear/simplifier.cc`.

patch file [librime-1.2.9-vc14.patch](https://github.com/buaabyl/building_rime/blob/master/librime-1.2.9-vc14.patch).

build 32bits

```
cd librime-rime-1.2.9\build32
cmake -G "NMake Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSTEM_INCLUDE_PATH=D:\syslib\include \
    -DCMAKE_SYSTEM_LIBRARY_PATH=D:\syslib\lib32 \
    ..
nmake
```

build 64bits
```
cd librime-rime-1.2.9\build64
cmake -G "NMake Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSTEM_INCLUDE_PATH=D:\syslib\include \
    -DCMAKE_SYSTEM_LIBRARY_PATH=D:\syslib\lib64 \
    ..
nmake
```

Result:
- .\bin\rime_api_console.exe
- .\bin\rime_patch.exe
- .\CMakeFiles\3.8.2\CompilerIdC\CMakeCCompilerId.exe
- .\CMakeFiles\3.8.2\CompilerIdCXX\CMakeCXXCompilerId.exe
- .\lib\rime.dll

copy `.\lib\rime.dll` to `.\bin` and type `bin\rime_api_console.exe` for testing:)
