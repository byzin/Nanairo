# Nanairo #

![cover](https://github.com/byzin/Nanairo/wiki/readme/FitnessRoom.png)

**Nanairo** is a physically plausible spectral renderer.

## Description ##

### Demo ###

![Demo](https://github.com/byzin/Nanairo/wiki/readme/nanairo_demo.gif)

### Features ###

**Monte calro ray tracing method**

* Path tracing
* Light tracing

**BVH**

* Binary radix tree [[thesis](https://research.nvidia.com/publication/maximizing-parallelism-construction-bvhs-octrees-and-k-d-trees)]
* Approximate agglomerative clustering [[thesis](http://graphics.cs.cmu.edu/projects/aac/)]
* Agglomerative treelet restructuring [[thesis](http://dl.acm.org/citation.cfm?doid=2790060.2790065)]

**Surface**

* Smooth diffuse surface (Lambert BRDF)
* Smooth conductor surface (Fresnel BRDF)
* Smooth dielectric surface (Fresnel BSDF)
* Rough conductor surface (GGX BRDF) [[thesis](https://hal.inria.fr/hal-00996995v2)]
* Rough dielectric surface (GGX BSDF) [[thesis](https://hal.inria.fr/hal-00996995v2)]

**Camera**

* Pinhole camera

**Colour**

* Spectra (point sampling) [[thesis](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.68.1533)]

**Geometry**

* Triangle meshes (support Wavefront .obj file)
* Nagata patch (normal interpolation) [[thesis](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.129.9689)], ray intersection [[thesis](https://www.osapublishing.org/ao/abstract.cfm?uri=ao-49-18-3442)]

**Tone mappling**

* Reinhard [[thesis](https://www.cs.utah.edu/~reinhard/cdrom/)]
* Modified reinhard
* Filmic

## Requirement ##

### Compiler ###

**OS X or Linux**

* GCC (version 5.1 or lator) [[the GNU Compiler Collection](https://gcc.gnu.org/)]
* Clang (version 3.5 or lator) [[The LLVM Compiler Infrastructure](http://llvm.org/)]

**Windows**

* MinGW GCC (version 5.1 or lator) [[MSYS2](https://msys2.github.io/)]

### Build tools ###

* CMake (version 3.0 or lator) [[CMake](http://www.cmake.org/)]

### Dependency Library ###

* Qt (version 5.4.0 or lator)
    - OS X or Linux: [[Qt Project](http://qt-project.org/)]
    - Windows: [[MSYS2](https://msys2.github.io/)]

## Usage ##
Please see [wiki](https://github.com/byzin/Nanairo/wiki/Home "Nanairo wiki").

## Installation ##

### Test environments ###
Currently, I tested in the following environments  

* Windows 8.1 + MinGW GCC 5.3 + Qt 5.5
* OS X Mavericks + Clang 3.7 + Qt 5.5
* Ubuntu 14.04 + GCC 5.2 + Qt 5.5

### Setup ###
You need to add the Qt qmake path to **$PATH**.  
``% export PATH=${qmake_path}:$PATH``  
For example, on OS X, qmake path is maybe ``${qt_root}/${qt_version}/clang_64/bin``  

### Build ###
First, You need to move the build directory.  
``% cd ${project_root}/build``

Next, we generate a Makefile using cmake.

On OS X or Linux  
``% cmake -DCMAKE_BUILD_TYPE=Release ..``

On Windows (MSYS2)  
``% cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..``

If default compiler is old version, we can specify the compiler to build.  
``% CXX=${c++_compiler_path} cmake ${options} ..``

Make **Nanairo**  
``% make``

## License ##
[MIT license](./MIT-LICENSE.txt)
except for *3d model* (resources/model) and *spectrum* (resources/spectrum) files.

## Acknowledgement ##
Please see [Acknowledgement](./Acknowledgement.md).
