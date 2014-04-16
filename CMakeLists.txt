# file: CMakeLists.txt
# author: Sho Ikeda 
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.0)


## The project information
project(Nanairo VERSION 0.1.0 LANGUAGES CXX)
message(STATUS "## Nanairo version: ${PROJECT_VERSION}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(${PROJECT_SOURCE_DIR}/cmake/project.cmake)
setNanairoProperties()

## Command option
include(${PROJECT_SOURCE_DIR}/cmake/option.cmake)
setCommandOptions()
if(NANAIRO_DEBUG)
  add_definitions(-DZISC_DEBUG)
  add_definitions(-DNANAIRO_DEBUG)
endif()

## Build configuration
include(${PROJECT_SOURCE_DIR}/cmake/general.cmake)
initializeBuildEnvironment()
include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)
setCompilerOptionsForCxx14()
setCompilerWarningOptionsForCxx()

# Set dependencies
if(z_linux)
  set(nanairo_dependencies pthread)
endif()

## Copy resources
if(NOT EXISTS ${PROJECT_BINARY_DIR}/resources)
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/resources)
  file(COPY ${PROJECT_SOURCE_DIR}/resources/image
            ${PROJECT_SOURCE_DIR}/resources/models 
            ${PROJECT_SOURCE_DIR}/resources/spectrum
       DESTINATION ${PROJECT_BINARY_DIR}/resources)
endif()

## Set nanairo settings
include(${PROJECT_SOURCE_DIR}/cmake/keyword.cmake)
setNanairoKeywords()


addLibraries()

##MakeConfigFiles()
if(NANAIRO_BUILD_DOCUMENTS)
  include(${PROJECT_SOURCE_DIR}/cmake/document.cmake)
  buildDocuments()
endif()


## Make execution file
addQtResources(nanairo_core_resource_file 
               ${PROJECT_SOURCE_DIR}/resources/resource.qrc)
addQtResources(nanairo_gui_resource_file 
               ${PROJECT_BINARY_DIR}/resource/nanairo_gui.qrc)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/source/main.cpp 
                               ${nanairo_gui_resource_file} 
                               ${nanairo_core_resource_file})
target_link_libraries(${PROJECT_NAME} ${qt5_libraries} -pthread
                                      ${nanairo_renderer_library}
                                      ${nanairo_gui_library})

if (NANAIRO_TEST)
  include(${PROJECT_SOURCE_DIR}/cmake/test.cmake)
  buildTest()
endif()

## Build extra tools
#if(NANAIRO_BUILD_EXTRA_TOOLS)
#  include(${PROJECT_SOURCE_DIR}/tools/spectra_rmse_calculator/config.cmake)
#  BuildSpectraRmseCalculator()
#endif()
#
#BuildSizeChecker()

include(${PROJECT_SOURCE_DIR}/cmake/packaging.cmake)
setupPackagingInfo()
