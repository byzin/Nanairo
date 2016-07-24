# file: CMakeLists.txt
# author: Sho Ikeda 
#
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.4)

## Set project information
project(Nanairo LANGUAGES CXX)
include(${PROJECT_SOURCE_DIR}/cmake/project.cmake)
setNanairoProperties()
message(STATUS "## Nanairo version: ${PROJECT_VERSION}")

# Debug info
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Build configuration
include(${PROJECT_SOURCE_DIR}/cmake/general.cmake)
restrictBuildDirectory(${PROJECT_SOURCE_DIR}/build)
detectEnvironment(environment_definitions)
# Command option
include(${PROJECT_SOURCE_DIR}/cmake/option.cmake)
setCommandOptions()
#
include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)
setCompilerOption()
getCompilerOption(cxx_compiler_flags cxx_linker_flags cxx_definitions)

# Libraries
# Thread
find_package(Threads REQUIRED)
# Qt5
include(${PROJECT_SOURCE_DIR}/cmake/qt5.cmake)
findQt5(5.6 Core Gui Qml Quick)
# Zisc
include(${PROJECT_SOURCE_DIR}/source/zisc/source/zisc/config.cmake)
loadZisc(zisc_source_files
         zisc_include_dirs
         zisc_compile_flags
         zisc_linker_flags
         zisc_definitions)

# Copy resources
file(COPY ${PROJECT_SOURCE_DIR}/resources/image
          ${PROJECT_SOURCE_DIR}/resources/models 
          ${PROJECT_SOURCE_DIR}/resources/spectrum
     DESTINATION ${PROJECT_BINARY_DIR}/resources)

# Build
buildNanairo()

# Document
if(${NANAIRO_BUILD_DOCUMENTS})
  include(${PROJECT_SOURCE_DIR}/cmake/document.cmake)
  buildDocuments()
endif()

# Unit test
# if(${NANAIRO_BUILD_TEST})
#   include(${PROJECT_SOURCE_DIR}/cmake/test.cmake)
#   buildTest()
# endif()

include(${PROJECT_SOURCE_DIR}/cmake/packaging.cmake)
setupPackInfo()
