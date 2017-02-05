# file: project.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


# Retrieve Nanairo version from README.md
function(retrieveNanairoVersion version_major version_minor version_patch)
  set(version_is_found OFF)
  # Parse README.md and find Nanairo version
  file(STRINGS ${PROJECT_SOURCE_DIR}/README.md readme_text)
  foreach(readme_line ${readme_text})
    if(readme_line MATCHES "Version: *([0-9]+)\\.([0-9]+)\\.([0-9]+) *")
      set(version_is_found ON)
      set(major ${CMAKE_MATCH_1})
      set(minor ${CMAKE_MATCH_2})
      set(patch ${CMAKE_MATCH_3})
      break()
    endif()
  endforeach(readme_line)
  if(NOT version_is_found)
    message(FATAL_ERROR "Nanairo version isn't found in README.md.")
  endif()


  # Output variables
  set(${version_major} ${major} PARENT_SCOPE)
  set(${version_minor} ${minor} PARENT_SCOPE)
  set(${version_patch} ${patch} PARENT_SCOPE)
endfunction(retrieveNanairoVersion)


# Set project informations
macro(setNanairoProperties)
  # Version
  retrieveNanairoVersion(PROJECT_VERSION_MAJOR
                         PROJECT_VERSION_MINOR
                         PROJECT_VERSION_PATCH)
  set(PROJECT_VERSION 
      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
  set(nanairo_version ${PROJECT_VERSION})
  set(nanairo_version_major ${PROJECT_VERSION_MAJOR})
  set(nanairo_version_minor ${PROJECT_VERSION_MINOR})
  set(nanairo_version_patch ${PROJECT_VERSION_PATCH})
endmacro(setNanairoProperties)


# Find source files (.cpp and .hpp) from root_path recursively,
# and set the group information
function(findNanairoSourceFiles root_path nanairo_source_files)
  file(GLOB_RECURSE source_files ${root_path}/*.cpp ${root_path}/*.hpp)
  foreach(source_file ${source_files})
    string(REGEX MATCH ".*/source/(.*)/.*\\.(cpp|hpp)" result "${source_file}")
    string(REPLACE "/"  "\\\\" group_name "${CMAKE_MATCH_1}")
    source_group("${group_name}" FILES "${source_file}")
  endforeach(source_file)


  # Output variable
  set(${nanairo_source_files} ${source_files} PARENT_SCOPE)
endfunction(findNanairoSourceFiles)


# Get the warning options of the compiler
function(getNanairoWarningOption nanairo_warning_flags)
  if(Z_VISUAL_STUDIO)
    set(warning_flags -Wno-microsoft-enum-value
                      -Qunused-arguments
                      -Wno-sign-compare
                      )
  elseif(Z_CLANG)
    set(warning_flags -Wno-padded
                      -Wno-covered-switch-default
                      -Wno-float-equal
                      -Wno-sign-conversion
                      -Wno-exit-time-destructors
                      -Wno-global-constructors
                      -Wno-weak-vtables
                      -Wno-undefined-reinterpret-cast
                      )
  elseif(Z_GCC)
    set(warning_flags -Wno-sign-conversion
                      -Wno-strict-overflow
                      -Wno-sign-compare
                      )
  endif()


  # Output variable
  set(${nanairo_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getNanairoWarningOption)


#
function(buildNanairo)
  ## Load Nanairo modules
#  include(${PROJECT_SOURCE_DIR}/cmake/keyword.cmake)
#  getNanairoKeywords(nanairo_keyword_list)
#  # NanairoCommon
#  include(${PROJECT_SOURCE_DIR}/source/NanairoCommon/config.cmake)
#  getNanairoCommon(common_source_files)
#  # NanairoCore
#  include(${PROJECT_SOURCE_DIR}/source/NanairoCore/config.cmake)
#  getNanairoCore(core_source_files core_definitions)
  # NanairoRenderer
  include(${PROJECT_SOURCE_DIR}/source/NanairoRenderer/config.cmake)
  getNanairoRenderer(renderer_source_files renderer_definitions)
  # NanairoUI
  include(${PROJECT_SOURCE_DIR}/source/NanairoGui/config.cmake)
  getNanairoGui(gui_source_files gui_definitions)
  # Build Nanairo
  set(nanairo_source_files ${PROJECT_SOURCE_DIR}/source/main.cpp)
  if(Z_WINDOWS)
    set(executable_options WIN32)
    list(APPEND nanairo_source_files ${PROJECT_SOURCE_DIR}/packaging/icon.rc)
  elseif(Z_MAC)
    set(executable_options MACOSX_BUNDLE)
  endif()
  add_executable(${PROJECT_NAME} ${executable_options}
                                 ${nanairo_source_files}
                                 ${common_source_files}
                                 ${core_source_files}
                                 ${renderer_source_files}
                                 ${gui_source_files}
                                 ${zisc_source_files})
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
  # Create symlink for the executable for Mac
  if(Z_MAC)
    set(exec_path ${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E create_symlink
                          ${exec_path}
                          ${PROJECT_NAME}
                       WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                       COMMENT "Creating the symlink to \"${exec_path}\"."
                       VERBATIM)
  endif()
endfunction(buildNanairo)
