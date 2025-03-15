# Clang-tidy的配置文件

## compile llvm+clang 20

```bash
#提前安装C++编译器，ninja，cmake等工具
#如果是Windows，需要visual studio 2022，内网有iso
dnf install g++ cmake

#这里下载的是最新版本的llvm，也可以根据需要下载特定版本的llvm
chcp 65001 # disable showInlcudes
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build
cd build

# windows
mkdir release
cd release
cmake -G Ninja LLVM_INCLUDE_TESTS=NO -DCMAKE_C_FLAGS=/utf-8 -DCMAKE_CXX_FLAGS=/utf-8 -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=host -DLLVM_INCLUDE_TESTS=OFF -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" ../llvm
cmake --build . --parallel # 这一步不一定要做,这个是全部编译

# linux
mkdir release
cd release
cmake -G Ninja LLVM_INCLUDE_TESTS=NO -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=host -DLLVM_INCLUDE_TESTS=OFF -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" ../llvm
cmake --build . --parallel  # 这一步不一定要做,这个是全部编译

# windows & linux
ninja clangd
ninja clang-tidy

#安装官方文档修改文件
#创建 llvm-project\clang-tools-extra\clang-tidy\dkaq 目录，其中有3个文件
#编辑3个文件
#	CMakeLists.txt
#	UnusedVarDiagnostic.cpp
#	UnusedVarDiagnostic.h

#修订 llvm-project\clang-tools-extra\clang-tidy\CMakeLists.txt
#	增加 add_subdirectory(dkaq)
#	增加 dkaqPlugin，再set(ALL_CLANG_TIDY_CHECKS 中

#在ClangTidyForceLinker.h中
# // This anchor is used to force the linker to link the ZirconModule.
#	extern volatile int DkaqModuleAnchorSource;
#		static int LLVM_ATTRIBUTE_UNUSED DkaqModuleAnchorDestination =
#	    DkaqModuleAnchorSource;

bin\clang-tidy --checks=-*,dkaq-unused-variables,dkaq-virtual-destructor -list-checks
bin\clang-tidy --checks=-*,dkaq-unused-variables d:\work\qt\markdown\a.cpp
bin\clang-tidy -list-checks
bin\clang-tidy --checks=* -list-checks
bin\clang-tidy --checks=*  d:\work\qt\markdown\a.cpp
bin\clang-tidy --checks=-*,dkaq-unused-variables,dkaq-virtual-destructor d:\work\qt\markdown\a.cpp

# 把这个clang-tidy替换qt creator安装目录中的clang-tidy或者clangd,根据不同的版本替换
copy D:\sources\llvm-project\release\bin\clang-tidy.exe C:\Qt\qtcreator-14.0.2\bin\clang\bin /y
```

```cmd
C:\Qt\qtcreator-4.11.2\bin\clang\bin>clang-tidy --version
LLVM (http://llvm.org/):
  LLVM version 8.0.1
  DEBUG build.
  Default target: x86_64-pc-windows-msvc
  Host CPU: goldmont
  
C:\Qt\qtcreator-14.0.2\bin\clang\bin>clang-tidy --version
LLVM (http://llvm.org/):
  LLVM version 18.1.7
  Optimized build.
```

## Compile llvm+clang 8.0.1

```bash
# download three 8.0.1 source tar.gz
#	cfe-8.0.1.src.tar.xz
#	clang-tools-extra-8.0.1.src.tar.xz
#	llvm-8.0.1.src.tar.xz
#extract all thress files under a directory, for example: d:\sources\llvm-8.0.1.src
d:\sources\llvm-8.0.1.src
	d:\sources\llvm-8.0.1.src\llvm
	d:\sources\llvm-8.0.1.src\clang
	d:\sources\llvm-8.0.1.src\clang-tools-extra
# 这三个子目录的名字一定不能错
```

```cpp
// insert into .\llvm-8.0.1.src\llvm\include\llvm\Demangle\MicrosoftDemangleNodes.h
#include <string>
#include <cstdint>
```

```cmd
cd d:\sources\llvm-8.0.1.src\llvm
mkdir release
cd release

# configure windows
cmake -G Ninja -DCMAKE_C_FLAGS=/utf-8 -DCMAKE_CXX_FLAGS=/utf-8 -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=host -DLLVM_INCLUDE_TESTS=OFF -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" ..

# configure Linux
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=host -DLLVM_INCLUDE_TESTS=OFF -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" ..

# compile all packages
# cmake --build .

# comile only clang-tidy clangd
ninja clangd
ninja clang-tidy
ninja libclang

bin\clang-tidy  -checks=* --list-checks
bin\clang-tidy --checks=-*,dkaq-unused-variables d:\work\qt\markdown\a.cpp
bin\clang-tidy --checks=-*,llvm-header-guard d:\work\qt\markdown\a.cpp

#use the .clang-tidy configuration in the parent directory of .cpp
D:\sources\llvm-8.0.1.src\llvm\release\bin\clang-tidy.exe -config= d:\work\qt\markdown\a.cpp

#将生成出来的D:\sources\llvm-8.0.1.src\llvm\release\bin\clang-tidy.exe 复制到 qt creator的目录中
copy D:\sources\llvm-8.0.1.src\llvm\release\bin\clang-tidy.exe C:\Qt\qtcreator-4.11.2\bin\clang\bin /y
copy D:\sources\llvm-8.0.1.src\llvm\release\bin\libclang.dll C:\Qt\qtcreator-4.11.2\bin\ /y
```

```cmd
#注意这个文件的大小,有的时候14和4两个版本同时运行，导致这个文件大小异常
C:\Users\david\AppData\Roaming\QtProject\*.ini

D:\sources\qt-creator-opensource-src-4.11.2\src\tools\clangbackend\source\clangtranslationunit.cpp
clang_getDiagnosticSetFromTU()

#需要替换C:\Qt\qtcreator-4.11.2\bin\libclang.dll
#ninja libclang

```

## Compile extractfunc

```bash
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=D:\sources\llvm-project\release ..
cmake --build .
```

## Get LLVM Version in CMake

```CMake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 查找 LLVM
find_package(LLVM REQUIRED CONFIG)

# 打印找到的 LLVM 版本
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
```

## Compile Under Kylin

```bash
# disable kysec
getstatus
setstatus disable
```

# 生成新的标准

- 用提示词生成代码,举例如下:

  ```
  我在给clang-tidy增加一个检查项,希望通过修改clang-tidy的源代码,增加一个C++的功能检查. 希望每一个define的内容,不能改变c++的关键字的用法.例如:define long 1,因为long是C++的关键字,所以这个需要报错.
  ```

-  复制到新文件(将提示词作为注释,增加在文件头部),文件名就是国军标的编号名称.例如 R-1-1-1.h

- 修改头文件(*.h)中的错误提示

- 修改dkaqmodule.cpp,增加这个标准对应的检查项和对应的#include

  ```c++
  #include "R-1-1-1.h"
  
  CheckFactories.registerCheck<DefineKeywordCheck>("dkaq-define-keyword"); 
  ```

- compile & test

  ```
  cd D:\sources\llvm-project\release
  ninja clang-tidy
  
  copy bin\clang-tidy.exe c:\qt\qtcreator-14.0.2\bin\clang\bin\ /y
  ```

  

```

```

