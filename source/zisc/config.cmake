# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.4)


set(__zisc_root__ ${CMAKE_CURRENT_LIST_DIR})


function(setZiscOption)
  # Math
  set(option_description "Use efficient power functions instead of std.")
  setBooleanOption(ZISC_MATH_EFFICIENT_POWER OFF ${option_description})

  set(option_description "Use efficient exponential functions instead of std.")
  setBooleanOption(ZISC_MATH_EFFICIENT_EXPONENTIAL OFF ${option_description})

  set(option_description "Use efficient trigonometric functions instead of std.")
  setBooleanOption(ZISC_MATH_EFFICIENT_TRIGONOMETRIC OFF ${option_description})

  set(option_description "Enable zisc debugging.")
  if(Z_IS_DEBUG_BUILD)
    setBooleanOption(ZISC_ENABLE_ASSERTION ON ${option_description})
  else()
    setBooleanOption(ZISC_ENABLE_ASSERTION OFF ${option_description})
  endif()
endfunction(setZiscOption)


function(getZiscOption zisc_compile_flags zisc_linker_flags zisc_definitions)
  if(${ZISC_MATH_EFFICIENT_POWER})
    list(APPEND definitions ZISC_MATH_EFFICIENT_POWER)
  endif()
  if(${ZISC_MATH_EFFICIENT_EXPONENTIAL})
    list(APPEND definitions ZISC_MATH_EFFICIENT_EXPONENTIAL)
  endif()
  if(${ZISC_MATH_EFFICIENT_TRIGONOMETRIC})
    list(APPEND definitions ZISC_MATH_EFFICIENT_TRIGONOMETRIC)
  endif()
  if(${ZISC_ENABLE_ASSERTION})
    list(APPEND definitions ZISC_ASSERTION)
  endif()

  set(${zisc_definitions} ${definitions} PARENT_SCOPE)
endfunction(getZiscOption)


# Defined variables
#   zisc_include_dirs:
#   zisc_definitions: 
function(loadZisc zisc_source_files zisc_include_dirs zisc_compile_flags zisc_linker_options zisc_definitions)
  ## Set version information
  set(zisc_version_major 0)
  set(zisc_version_minor 9)
  set(zisc_version_patch 0)
  set(zisc_version ${zisc_version_major}.${zisc_version_minor}.${zisc_version_patch})
  message(STATUS "## Zisc version: ${zisc_version}")
  
  setZiscOption()
  getZiscOption(compile_flags linker_flags definitions)
  
  ## Make configuration header file
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
  set(zisc_config_path ${PROJECT_BINARY_DIR}/include/zisc)
  configure_file(${__zisc_root__}/zisc/zisc_config.hpp.in
                 ${zisc_config_path}/zisc_config.hpp)
 
  ## Set source code
  file(GLOB cpp_files ${__zisc_root__}/zisc/*.cpp
                      ${__zisc_root__}/zisc/packrat_parser/*.cpp)
  file(GLOB hpp_files ${__zisc_root__}/zisc/*.hpp
                      ${__zisc_root__}/zisc/packrat_parser/*.hpp)
  set(hpp_files ${hpp_files} ${zisc_config_path}/zisc_config.hpp)
  source_group(Zisc FILES ${cpp_files} ${hpp_files})


  ## Output variables
  set(${zisc_source_files} ${cpp_files} ${hpp_files} PARENT_SCOPE)
  set(${zisc_include_dirs} ${__zisc_root__}
                           ${PROJECT_BINARY_DIR}/include PARENT_SCOPE)
  set(${zisc_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zisc_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${zisc_definitions} ${definitions} PARENT_SCOPE)
endfunction(loadZisc)
