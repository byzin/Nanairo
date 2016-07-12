# file: project.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.4)


# Set project informations
macro(setNanairoProperties)
  # Version
  set(nanairo_version ${PROJECT_VERSION})
  set(nanairo_version_major ${PROJECT_VERSION_MAJOR})
  set(nanairo_version_minor ${PROJECT_VERSION_MINOR})
  set(nanairo_version_patch ${PROJECT_VERSION_PATCH})
endmacro(setNanairoProperties)


# Get source files
#   nanairo_source_files: .cpp and .hpp files
function(getNanairoSourceFiles group_name root_path dir_name nanairo_source_files)
  # Find .cpp and .hpp files
  if ("${dir_name}" STREQUAL "")
    file(GLOB cpp_files ${root_path}/*.cpp)
    file(GLOB hpp_files ${root_path}/*.hpp)
  else()
    file(GLOB cpp_files ${root_path}/${dir_name}/*.cpp)
    file(GLOB hpp_files ${root_path}/${dir_name}/*.hpp)
  endif()
  # Set group name
  if(NOT "${dir_name}" STREQUAL "")
    string(REPLACE "/" "\\\\" nanairo_group_name "\\\\${dir_name}")
  endif()
  source_group(${group_name}${nanairo_group_name} FILES ${cpp_files} ${hpp_files})
  # Add files to output variables
  set(${nanairo_source_files} ${cpp_files} ${hpp_files} PARENT_SCOPE)
endfunction(getNanairoSourceFiles)


#
#
function(getNanairoWarningOption nanairo_warning_flags)
  if(Z_IS_CLANG)
    set(warning_flags -Wno-padded
                      -Wno-covered-switch-default
                      -Wno-float-equal
                      -Wno-sign-conversion
                      -Wno-exit-time-destructors
                      -Wno-global-constructors
                      -Wno-weak-vtables
                      -Wno-undefined-reinterpret-cast)
  elseif(Z_IS_GCC)
    set(warning_flags -Wno-sign-conversion)
  endif()


  # Output variable
  set(${nanairo_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getNanairoWarningOption)


# Build NanairoCore with Zisc
function(buildNanairoCore)
  set(nanairo_core ${PROJECT_NAME}Core)
  add_library(${nanairo_core} STATIC ${core_source_files} ${zisc_source_files})
  # Set Nanairo properties
  set_target_properties(${nanairo_core} PROPERTIES CXX_STANDARD 14
                                                   CXX_STANDARD_REQUIRED ON
                                                   AUTOMOC ON
                                                   AUTORCC ON)
  getCxxWarningOption(cxx_warning_flags)
  getNanairoWarningOption(nanairo_warning_flags)
  checkCompilerHasCxx14Features(${nanairo_core})
  target_compile_options(${nanairo_core} PRIVATE ${cxx_compiler_flags}
                                                 ${qt5_compile_flags}
                                                 ${zisc_compile_flags}
                                                 ${cxx_warning_flags}
                                                 ${nanairo_warning_flags})
  target_include_directories(${nanairo_core} PRIVATE ${PROJECT_SOURCE_DIR}/source
                                                     ${PROJECT_BINARY_DIR}/include
                                                     ${zisc_include_dirs})
  target_include_directories(${nanairo_core} SYSTEM PRIVATE ${qt5_include_dirs})
  target_link_libraries(${nanairo_core} ${CMAKE_THREAD_LIBS_INIT}
                                        ${cxx_linker_flags}
                                        ${qt5_libraries}
                                        ${zisc_linker_flags})
  target_compile_definitions(${nanairo_core} PRIVATE ${cxx_definitions}
                                                     ${core_definitions}
                                                     ${qt5_definitions}
                                                     ${zisc_definitions}
                                                     ${environment_definitions})
endfunction(buildNanairoCore)


#
function(buildNanairo)
  ## Load Nanairo modules
  include(${PROJECT_SOURCE_DIR}/cmake/keyword.cmake)
  getNanairoKeywords(nanairo_keyword_list)
  # NanairoCommon
  include(${PROJECT_SOURCE_DIR}/source/NanairoCommon/config.cmake)
  getNanairoCommon()
  # NanairoCore
  include(${PROJECT_SOURCE_DIR}/source/NanairoCore/config.cmake)
  getNanairoCore(core_source_files core_definitions)
  buildNanairoCore()
  # NanairoRenderer
  include(${PROJECT_SOURCE_DIR}/source/NanairoRenderer/config.cmake)
  getNanairoRenderer(renderer_source_files renderer_definitions)
  # NanairoUI
  include(${PROJECT_SOURCE_DIR}/source/NanairoGui/config.cmake)
  getNanairoGui(gui_source_files gui_definitions)
  # Build Nanairo
  add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/source/main.cpp
                                 ${renderer_source_files}
                                 ${gui_source_files})
  # Set Nanairo properties
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14
                                                   CXX_STANDARD_REQUIRED ON
                                                   AUTOMOC ON
                                                   AUTORCC ON)
  getCxxWarningOption(cxx_warning_flags)
  getNanairoWarningOption(nanairo_warning_flags)
  checkCompilerHasCxx14Features(${PROJECT_NAME})
  target_compile_options(${PROJECT_NAME} PRIVATE ${cxx_compiler_flags}
                                                 ${qt5_compile_flags}
                                                 ${zisc_compile_flags}
                                                 ${cxx_warning_flags}
                                                 ${nanairo_warning_flags})
  target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/source
                                                     ${PROJECT_BINARY_DIR}/include
                                                     ${zisc_include_dirs})
  target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${qt5_include_dirs})
  target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT}
                                        ${PROJECT_NAME}Core
                                        ${cxx_linker_flags}
                                        ${qt5_libraries}
                                        ${zisc_linker_flags})
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${cxx_definitions}
                                                     ${core_definitions}
                                                     ${renderer_definitions}
                                                     ${gui_definitions}
                                                     ${qt5_definitions}
                                                     ${zisc_definitions}
                                                     ${environment_definitions})
endfunction(buildNanairo)
