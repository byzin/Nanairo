# file: project.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
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
  set(warning_flags "")

  # Suppress warning
  if(NANAIRO_SUPPRESS_EXCESSIVE_WARNING)
    if(Z_CLANG)
      list(APPEND warning_flags -Wno-covered-switch-default
                                -Wno-documentation-unknown-command
                                -Wno-exit-time-destructors
                                -Wno-float-equal
                                -Wno-global-constructors
                                -Wno-padded
                                -Wno-sign-conversion
                                -Wno-switch-enum
                                -Wno-undefined-reinterpret-cast
                                -Wno-weak-vtables
                                )
    elseif(Z_GCC)
      list(APPEND warning_flags -Wno-sign-conversion
                                -Wno-strict-overflow
                                )
    endif()
  endif()


  # Output variable
  set(${nanairo_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getNanairoWarningOption)


macro(includeZisc target)
  if(NANAIRO_SUPPRESS_EXCESSIVE_WARNING)
    target_include_directories(${target} SYSTEM PRIVATE ${zisc_include_dirs})
  else()
    target_include_directories(${target} PRIVATE ${zisc_include_dirs})
  endif()
endmacro(includeZisc)


function(loadLodepng lodepng_include_dir lodepng_library)
  set(lodepng_dir ${PROJECT_SOURCE_DIR}/source/dependencies/lodepng)
  if(NOT EXISTS ${lodepng_dir})
    showSubmoduleWarning(lodepng)
  endif()
  set(source_files ${lodepng_dir}/lodepng.cpp ${lodepng_dir}/lodepng.h)
  set(lodepng_name "lodepng")
  source_group(${lodepng_name} FILES ${source_files})
  add_library(${lodepng_name} STATIC ${source_files})
  ## Set lodepng properties
  set_target_properties(${lodepng_name} PROPERTIES CXX_STANDARD 17
                                                   CXX_STANDARD_REQUIRED ON)
  target_compile_options(${lodepng_name} PRIVATE ${cxx_compiler_flags})
  target_include_directories(${lodepng_name} PRIVATE ${lodepng_dir})
  target_link_libraries(${lodepng_name} ${CMAKE_THREAD_LIBS_INIT}
                                        ${cxx_linker_flags})
  target_compile_definitions(${lodepng_name} PRIVATE ${cxx_definitions})


  set(${lodepng_include_dir} ${lodepng_dir} PARENT_SCOPE)
  set(${lodepng_library} ${lodepng_name} PARENT_SCOPE)
endfunction(loadLodepng)


#
function(buildNanairoCore core_library core_definitions)
  # Load Nanairo core
  include(${PROJECT_SOURCE_DIR}/source/NanairoCore/config.cmake)
  getNanairoCore(core_source_files core_definitions)
  # Build Core
  set(core_name NanairoCore)
  add_library(${core_name} STATIC ${core_source_files})
  # Set properties
  set_target_properties(${core_name} PROPERTIES CXX_STANDARD 17
                                                CXX_STANDARD_REQUIRED ON)
  getCxxWarningOption(cxx_warning_flags)
  getNanairoWarningOption(nanairo_warning_flags)
  target_compile_options(${core_name} PRIVATE ${cxx_compiler_flags}
                                              ${zisc_compile_flags}
                                              ${cxx_warning_flags}
                                              ${nanairo_warning_flags})
  target_include_directories(${core_name} PRIVATE ${PROJECT_SOURCE_DIR}/source
                                                  ${PROJECT_BINARY_DIR}/include)
  includeZisc(${core_name})
  target_link_libraries(${core_name} ${CMAKE_THREAD_LIBS_INIT}
                                     ${cxx_linker_flags}
                                     ${zisc_linker_flags})
  target_compile_definitions(${core_name} PRIVATE ${cxx_definitions}
                                                  ${core_definitions}
                                                  ${zisc_definitions}
                                                  ${environment_definitions})
  setStaticAnalyzer(${core_name})


  # Output variables
  set(${core_library} ${core_name} PARENT_SCOPE)
endfunction(buildNanairoCore)


#
function(buildSimpleNanairoApp)
  ## Load Nanairo modules
  include(${PROJECT_SOURCE_DIR}/cmake/keyword.cmake)
  getNanairoKeywords(nanairo_keyword_list)
  ## Build SimpleNanairo
  set(nanairo_source_files ${PROJECT_SOURCE_DIR}/source/simple_nanairo.cpp
                           ${PROJECT_SOURCE_DIR}/source/simple_renderer.cpp
                           ${PROJECT_SOURCE_DIR}/source/simple_renderer.hpp
                           ${PROJECT_SOURCE_DIR}/source/simple_renderer-inl.hpp)
  set(app_name "SimpleNanairo")
  add_executable(${app_name} ${nanairo_source_files}
                             ${core_source_files}
                             ${zisc_header_files})
  ## Set SimpleNanairo properties
  set_target_properties(${app_name} PROPERTIES CXX_STANDARD 17
                                               CXX_STANDARD_REQUIRED ON)
  getCxxWarningOption(cxx_warning_flags)
  getNanairoWarningOption(nanairo_warning_flags)
  target_compile_options(${app_name} PRIVATE ${cxx_compiler_flags}
                                             ${zisc_compile_flags}
                                             ${cxx_warning_flags}
                                             ${nanairo_warning_flags})
  target_include_directories(${app_name} PRIVATE ${PROJECT_SOURCE_DIR}/source
                                                 ${PROJECT_BINARY_DIR}/include)
  includeZisc(${app_name})
  target_include_directories(${app_name} SYSTEM PRIVATE
      ${lodepng_include_dir}
      ${PROJECT_SOURCE_DIR}/source/dependencies/cxxopts/include)
  target_link_libraries(${app_name} ${CMAKE_THREAD_LIBS_INIT}
                                    ${cxx_linker_flags}
                                    ${zisc_linker_flags}
                                    ${lodepng_library}
                                    ${core_library})
  target_compile_definitions(${app_name} PRIVATE ${cxx_definitions}
                                                 ${core_definitions}
                                                 ${zisc_definitions}
                                                 ${environment_definitions}
                                                 NANAIRO_HAS_LODEPNG)
  setStaticAnalyzer(${app_name})
endfunction(buildSimpleNanairoApp)


#
function(buildNanairoApp)
  ## Load Nanairo modules
  include(${PROJECT_SOURCE_DIR}/cmake/keyword.cmake)
  getNanairoKeywords(nanairo_keyword_list)
  # NanairoGUI
  include(${PROJECT_SOURCE_DIR}/source/NanairoGui/config.cmake)
  getNanairoGui(gui_source_files gui_definitions)
  # Build Nanairo
  set(nanairo_source_files ${PROJECT_SOURCE_DIR}/source/main.cpp
                           ${PROJECT_SOURCE_DIR}/source/simple_renderer.cpp 
                           ${PROJECT_SOURCE_DIR}/source/simple_renderer.hpp 
                           ${PROJECT_SOURCE_DIR}/source/simple_renderer-inl.hpp)
  if(Z_WINDOWS)
    set(executable_options WIN32)
    list(APPEND nanairo_source_files ${PROJECT_SOURCE_DIR}/packaging/icon.rc)
  elseif(Z_MAC)
    set(executable_options MACOSX_BUNDLE)
  endif()
  set(app_name ${PROJECT_NAME})
  add_executable(${app_name} ${executable_options} ${nanairo_source_files}
                                                   ${core_source_files}
                                                   ${gui_source_files}
                                                   ${zisc_header_files})
  # Set Nanairo properties
  set_target_properties(${app_name} PROPERTIES CXX_STANDARD 17
                                               CXX_STANDARD_REQUIRED ON
                                               AUTOMOC ON
                                               AUTORCC ON)
  getCxxWarningOption(cxx_warning_flags)
  getNanairoWarningOption(nanairo_warning_flags)
  target_compile_options(${app_name} PRIVATE ${cxx_compiler_flags}
                                             ${qt5_compile_flags}
                                             ${zisc_compile_flags}
                                             ${cxx_warning_flags}
                                             ${nanairo_warning_flags})
  target_include_directories(${app_name} PRIVATE ${PROJECT_SOURCE_DIR}/source
                                                 ${PROJECT_BINARY_DIR}/include)
  includeZisc(${app_name})
  target_include_directories(${app_name} SYSTEM PRIVATE ${qt5_include_dirs})
  target_link_libraries(${app_name} ${CMAKE_THREAD_LIBS_INIT}
                                    ${cxx_linker_flags}
                                    ${qt5_libraries}
                                    ${zisc_linker_flags}
                                    ${core_library})
  target_compile_definitions(${app_name} PRIVATE ${cxx_definitions}
                                                 ${core_definitions}
                                                 ${gui_definitions}
                                                 ${qt5_definitions}
                                                 ${zisc_definitions}
                                                 ${environment_definitions})
  setStaticAnalyzer(${app_name})
  # Create symlink for the executable for Mac
  if(Z_MAC)
    set(exec_path ${app_name}.app/Contents/MacOS/${app_name})
    add_custom_command(TARGET ${app_name} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E create_symlink
                          ${exec_path}
                          ${app_name}
                       WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                       COMMENT "Creating the symlink to \"${exec_path}\"."
                       VERBATIM)
  endif()
endfunction(buildNanairoApp)
