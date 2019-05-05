# Nanairo #

![cover](https://github.com/byzin/Nanairo/wiki/image/RaytracingCamp4.png)

**Nanairo** is a physically plausible spectral renderer.

Version: 0.0.5

## Description ##

### Demo ###

![Demo](https://github.com/byzin/Nanairo/wiki/image/NanairoDemo.gif)

### Features ###

**Monte calro ray tracing method**

* Path tracing [[paper](https://dl.acm.org/citation.cfm?id=15902)]
* Light tracing
* ~~Progressive photon mapping~~ [[paper](http://www.cgg.unibe.ch/publications/2011/progressive-photon-mapping-a-probabilistic-approach)]

**BVH**

* Binary radix tree [[paper](https://research.nvidia.com/publication/maximizing-parallelism-construction-bvhs-octrees-and-k-d-trees)]
* Agglomerative treelet restructuring [[paper](http://dl.acm.org/citation.cfm?doid=2790060.2790065)]

**Surface**

* Smooth diffuse surface (Lambert BRDF)
* Smooth conductor surface (Fresnel BRDF)
* Smooth dielectric surface (Fresnel BSDF)
* Rough conductor surface (GGX BRDF) [[paper](https://hal.inria.fr/hal-00996995v2)]
* Rough dielectric surface (GGX BSDF) [[paper](https://hal.inria.fr/hal-00996995v2)]
* Layered diffuse surface (Interfaced Lambertian BRDF) [[paper](https://hal-unilim.archives-ouvertes.fr/hal-01246612/)]
* ~~Cloth surface (Microcylinder cloth BRDF)~~ [[paper](http://dl.acm.org/citation.cfm?id=2451240)]

**Camera**

* Pinhole camera

**Color**

* Spectra (point sampling) [[paper](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.68.1533)]
* Spectral transport from RGB [[paper](http://dl.acm.org/citation.cfm?id=2853793)]

**Geometry**

* Triangle meshes (support Wavefront .obj file)
* ~~Nagata patch (normal interpolation)~~ [[paper](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.129.9689)], ray intersection [[paper](https://www.osapublishing.org/ao/abstract.cfm?uri=ao-49-18-3442)]
* Orthonormal basis [[paper](http://jcgt.org/published/0006/01/01/)]

**Denoising**

* Bayesian Collaborative Denoising for Monte Carlo Rendering [[paper](https://dl.acm.org/citation.cfm?id=3128464)]

**Tone mappling**

* Reinhard [[paper](https://www.cs.utah.edu/~reinhard/cdrom/)]
* Filmic
* Uncharted2

## Prerequisites ##

### Build tools ###

* Git
* C++17 support compiler
* CMake (version 3.14 or later) [[page](http://www.cmake.org/)]

### Dependency Library ###

* Qt (version 5.12.0 or later) [[page](http://qt-project.org/)]

## Usage ##
Please see [wiki](https://github.com/byzin/Nanairo/wiki/Home "NanairoWiki").

## Installation ##

### Test environments ###
Building step is tested in the following environments  

* macOS High Sierra + Apple Clang 9.1 + Qt 5.11.1 + CMake 3.12.1
* macOS High Sierra + GCC 8.1 + Qt 5.11.1 + CMake 3.12.1
* ~~Ubuntu 16.04 + GCC 8.2 + Qt 5.12.3 + CMake 3.14.3~~
* Ubuntu 16.04 + Clang 8.0 + Qt 5.12.3 + CMake 3.14.3
* Windows 10 (64bit) + MinGW GCC 7.3 + Qt 5.11.1 + CMake 3.12.1
* Windows 10 (64bit) + MSVC (Visual Studio 2017) + Qt 5.11.1 + CMake 3.12.1
* Windows 10 (64bit) + Clang 6.1 (Visual Studio 2017) + Qt 5.11.1 + CMake 3.12.1

### Download source code ###

```
% git clone https://github.com/byzin/Nanairo
% cd Nanairo
% git submodule init
% git submodule update
```

### Setup ###
To build Nanairo, you need to add the Qt qmake path to **PATH**.  
On Windows,
```
% set PATH=${qmake_path};%PATH%
```
On Linux and macOS,
```
% export PATH=${qmake_path}:$PATH
```

#### Python environment setup ####
Please see [wiki](https://github.com/byzin/Nanairo/wiki/Python-environment-setup "Python environment setup") for details.

### Build ###
Please see [wiki](https://github.com/byzin/Nanairo/wiki/Home "NanairoWiki")
for details.

First, You need to move the build directory.  
``% cd ${project_root}/build``

Next, we generate a Makefile using cmake.

#### On Linux or macOS ####

GCC  
```
% CC=gcc CXX=g++ cmake -DCMAKE_BUILD_TYPE=Release ..
% cmake --build .
% ./Nanairo -style material
```

Clang  
```
% CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release -DZ_CLANG_USES_LIBCXX=ON ..
% cmake --build .
% ./Nanairo -style material
```

#### On Windows (MSVC) ####

Visual Studio 2017 64bit
```
% cmake -G"Visual Studio 15 2017" -Ax64 -DCMAKE_BUILD_TYPE=Release ..
% cmake --build . --config Release
% Release\Nanairo -style material
```

### Packaging ###

Only **Windows** and **Mac** are supported.

After release building,
```
% cpack -C Release
```

## License ##
[LICENSE](./LICENSE)
except for *3d model* (resources/model) and *spectrum* (resources/spectrum) files.

## Acknowledgement ##
Please see [Acknowledgement.md](./Acknowledgement.md).
