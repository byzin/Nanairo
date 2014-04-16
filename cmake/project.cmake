# file: project.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0)


# Functions and macros

## Set project informations
macro(setNanairoProperties)
  # Version
  set(nanairo_version ${PROJECT_VERSION})
  set(nanairo_version_major ${PROJECT_VERSION_MAJOR})
  set(nanairo_version_minor ${PROJECT_VERSION_MINOR})
  set(nanairo_version_patch ${PROJECT_VERSION_PATCH})
endmacro(setNanairoProperties)


# Clear the cache of source file variables
macro(clearNanairoSourceFiles)
  set(nanairo_source_files "")
  set(nanairo_cpp_files "")
  set(nanairo_hpp_files "")
endmacro(clearNanairoSourceFiles)


# Add source files to
#   nanairo_source_files: .cpp and .hpp files
#   nanairo_cpp_files: .cpp files
#   nanairo_hpp_files: .hpp files
function(addNanairoSourceFiles group_name root_path dir_name)
  # Find .cpp files
  aux_source_directory(${root_path}/${dir_name} cpp_files)
  # Find .hpp files
  file(GLOB hpp_files ${root_path}/${dir_name}/*.hpp)
  # Set group name
  if(NOT "${dir_name}" STREQUAL "")
    string(REPLACE "/" "\\\\" nanairo_group_name "\\\\${dir_name}")
  endif()
  source_group(${group_name}${nanairo_group_name} FILES ${cpp_files} ${hpp_files})

  # Add files to variables
  set(nanairo_source_files ${nanairo_source_files} ${cpp_files} ${hpp_files} PARENT_SCOPE)
  set(nanairo_cpp_files ${nanairo_cpp_files} ${cpp_files} PARENT_SCOPE)
  set(nanairo_hpp_files ${nanairo_hpp_files} ${hpp_files} PARENT_SCOPE)
endfunction(addNanairoSourceFiles)


macro(addLibraries)
  set(enable_nanairo_opencl FALSE)
  
  # Qt
  include(${PROJECT_SOURCE_DIR}/cmake/qt5.cmake)
  findQt5()
  add_definitions(${qt5_definitions})
  include_directories(${qt5_include_dirs})
  
  # Zisc
  include(${PROJECT_SOURCE_DIR}/source/zisc/config.cmake)
  loadZisc()
  include_directories(${zisc_include_dirs})
  
  ## Load and set source files
  set(nanairo_source_dir ${PROJECT_SOURCE_DIR}/source)
  include_directories(${nanairo_source_dir})
  set(nanairo_config_dir ${PROJECT_BINARY_DIR}/include)
  include_directories(${nanairo_config_dir})
  # NanairoCommon
  include(${PROJECT_SOURCE_DIR}/source/NanairoCommon/config.cmake)
  loadNanairoCommon()

  # NanairoCore
  include(${PROJECT_SOURCE_DIR}/source/NanairoCore/config.cmake)
  buildNanairoCore()

  # NanairoRenderer
  include(${PROJECT_SOURCE_DIR}/source/NanairoRenderer/config.cmake)
  buildNanairoRenderer()

  # NanairoUI
  include(${PROJECT_SOURCE_DIR}/source/NanairoGui/config.cmake)
  buildNanairoGui()

  ## Set library properties
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/libs)

  set(__nanairo_libs__ ${nanairo_renderer_library} 
                       ${nanairo_gui_library}
                       ${nanairo_core_library})
  set_target_properties(${__nanairo_libs__} PROPERTIES
      VERSION ${nanairo_version}
      SOVERSION ${nanairo_version_major})
  set_target_properties(${zisc_library} ${__nanairo_libs__} PROPERTIES
      ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs
      LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs)
endmacro(addLibraries)
