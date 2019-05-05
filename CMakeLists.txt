# file: CMakeLists.txt
# author: Sho Ikeda 
#
# Copyright (c) 2015-2019 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.10)

function(checkSubmodule submodule_path)
  if(NOT EXISTS ${submodule_path})
    get_filename_component(submodule_name "${submodule_path}" NAME)
    message(FATAL_ERROR "Submodule '${submodule_name}' not found, please init submodules.")
  endif()
endfunction(checkSubmodule)

# Set project information
project(Nanairo LANGUAGES C CXX)
include(${PROJECT_SOURCE_DIR}/cmake/project.cmake)
setNanairoProperties()
message(STATUS "Nanairo version: ${PROJECT_VERSION}")
message(STATUS "Nanairo brief: ${nanairo_brief}")

# Configure build types
set(CMAKE_CONFIGURATION_TYPES "Debug" "RelWithDebInfo" "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(zisc_dir ${CMAKE_SOURCE_DIR}/source/dependencies/zisc)
checkSubmodule(${zisc_dir})

# Build configuration
include(${zisc_dir}/cmake/general.cmake)
restrictBuildDirectory(${PROJECT_SOURCE_DIR}/build)
detectEnvironment(environment_definitions)
# Command option
include(${PROJECT_SOURCE_DIR}/cmake/option.cmake)
initCommandOptions()
#
include(${zisc_dir}/cmake/compiler.cmake)
initCompilerOption()
getCxxCompilerOption(cxx_compiler_flags cxx_linker_flags cxx_definitions)

# Common libraries
# Thread
find_package(Threads REQUIRED)
# LodePNG
loadLodepng(lodepng_include_dir lodepng_library)
# Zisc
include(${zisc_dir}/source/zisc/config.cmake)
loadZisc(zisc_header_files
         zisc_include_dirs
         zisc_compile_flags
         zisc_linker_flags
         zisc_definitions)

# Copy resources to the build directory
file(COPY ${PROJECT_SOURCE_DIR}/resources/image
          ${PROJECT_SOURCE_DIR}/resources/models 
          ${PROJECT_SOURCE_DIR}/resources/spectrum
     DESTINATION ${PROJECT_BINARY_DIR}/resources)

# Document
if(${NANAIRO_BUILD_DOCUMENTS})
  include(${PROJECT_SOURCE_DIR}/cmake/document.cmake)
  buildDocuments()
endif()

buildNanairoCore(core_library core_definitions)

# Applications
buildSimpleNanairoApp()
if(${NANAIRO_BUILD_APP})
  # Library
  include(${PROJECT_SOURCE_DIR}/cmake/qt5.cmake)
  findQt5(5.11 Core Gui Qml Quick)
  # Nanairo app
  buildNanairoApp()
  # Packaging
  include(${PROJECT_SOURCE_DIR}/cmake/packaging.cmake)
  setupPackInfo()
endif()

# Unit tests
if(${NANAIRO_BUILD_TESTS})
  include(${PROJECT_SOURCE_DIR}/test/config.cmake)
  buildUnitTest()
endif()
