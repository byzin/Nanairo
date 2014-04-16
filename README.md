# Nanairo #

## Abstract ##
**Nanairo** is a physically plausible spectral renderer.

![cover](https://github.com/byzin/Nanairo/wiki/image/FitnessRoom.png "FitnessRoom")

## System requirements ##

### Compiler ###

#### On OS X or Linux ####

* GCC (version 4.8 or lator) [[the GNU Compiler Collection](https://gcc.gnu.org/)]
* Clang (version 5.2 or lator) [[The LLVM Compiler Infrastructure](http://llvm.org/)]

#### On Windows ####

* MinGW GCC (version 4.8 or lator) [[Qt64-NG](http://sourceforge.net/projects/qt64ng/)]

### Build tools ###

* CMake (version 3.0 or lator) [[CMake](http://www.cmake.org/)]

### Libraries ###

* Qt (version 5.4.0 or lator) [OS X or Linux: [[Qt Project](http://qt-project.org/)], Windows:
 [[Qt64-NG](http://sourceforge.net/projects/qt64ng/)]]

## Setup ##
You need to add the Qt qmake path to **$PATH**.  
``% export PATH=${qmake_path}:$PATH``  
For example, on OS X, qmake path is maybe ``${qt_root}/${qt_version}/clang_64/bin``  

## Test environments ##
Currently, I tested in the following environments  

* Windows 8.1 + MinGW GCC 4.9 + Qt 5.4 
* OS X Mavericks + Clang 3.6 + Qt 5.4
* Ubuntu 14.04 + GCC 4.8 + Qt 5.4

## How to compile ##
First, You need to move the build directory.  
``% cd ${project_root}/build``

Next, we generate a Makefile using cmake.

On OS X or Linux  
``% cmake -DCMAKE_BUILD_TYPE=Release ..``

On Windows (MSYS)  
``% cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..``

If default compiler is old version, we can specify the compiler to build.  
``% CC=${c_compiler_path} CXX=${c++_compiler_path} cmake ${options} ..``

Make **Nanairo**  
``% make``

## How to use ##
Please see [wiki](https://github.com/byzin/Nanairo/wiki/Home "Nanairo wiki").

## License ##
[MIT license](./MIT-LICENSE.txt)
except for *3d model* (resources/model) and *spectrum* (resources/spectrum) files.

## Acknowledgement ##
Please see [Acknowledgement](./Acknowledgement.md).
