# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0)


set(__zisc_root__ ${CMAKE_CURRENT_LIST_DIR})


# Inner functions and macros
macro(setZiscOption variable value doc_string)
  set(${variable} ${value} CACHE BOOL ${doc_string})
endmacro(setZiscOption)

function(processZiscOption)
  if(ZISC_MATH_FAST_POW)
    add_definitions(-DZISC_MATH_FAST_POW)
  endif()
  if(ZISC_MATH_FAST_SQRT)
    add_definitions(-DZISC_MATH_FAST_SQRT)
  endif()
  if(ZISC_MATH_FAST_CBRT)
    add_definitions(-DZISC_MATH_FAST_CBRT)
  endif()
  if(ZISC_MATH_FAST_EXP)
    add_definitions(-DZISC_MATH_FAST_EXP)
  endif()
  if(ZISC_MATH_FAST_LOG)
    add_definitions(-DZISC_MATH_FAST_LOG)
  endif()
  if(ZISC_MATH_FAST_SINCOS)
    add_definitions(-DZISC_MATH_FAST_SINCOS)
  endif()
  if(ZISC_MATH_FAST_ARCSINCOS)
    add_definitions(-DZISC_MATH_FAST_ARCSINCOS)
  endif()
  if(ZISC_MATH_FAST_ARCTAN)
    add_definitions(-DZISC_MATH_FAST_ARCTAN)
  endif()
endfunction(processZiscOption)

function(initZiscOption)
  # Math
  set(option_description "Use fast approximate pow() instead of std::pow()")
  setZiscOption(ZISC_MATH_FAST_POW OFF ${option_description})

  set(option_description "Use fat approximate sqrt() instead of std::sqrt()")
  setZiscOption(ZISC_MATH_FAST_SQRT OFF ${option_description})

  set(option_description "Use fat approximate cbrt() instead of std::cbrt()")
  setZiscOption(ZISC_MATH_FAST_CBRT OFF ${option_description})

  set(option_description "Use fast approximate exp() instead of std::exp()")
  setZiscOption(ZISC_MATH_FAST_EXP OFF ${option_description})

  set(option_description "Use fast approximate log() instead of std::log()")
  setZiscOption(ZISC_MATH_FAST_LOG OFF ${option_description})

  set(option_description "Use fast approximate sin() and cos() instead of std::sin() and std::cos()")
  setZiscOption(ZISC_MATH_FAST_SINCOS OFF ${option_description})

  #  set(option_description "Use fast approximate tan() instead of std::tan()")
  #  setZiscOption(ZISC_MATH_FAST_TAN OFF ${option_description})

  set(option_description "Use fast approximate arcSin() and arcCos() instead of std::asin() and std::acos()")
  setZiscOption(ZISC_MATH_FAST_ARCSINCOS OFF ${option_description})

  set(option_description "Use fast approximate arcTan() instead of std::atan()")
  setZiscOption(ZISC_MATH_FAST_ARCTAN OFF ${option_description})
endfunction(initZiscOption)


# Functions and macros

# Defined variables
#   zisc_include_dirs:
function(loadZisc)
  ## Set version information
  set(zisc_version_major 0)
  set(zisc_version_minor 7)
  set(zisc_version_patch 0)
  set(zisc_version "${zisc_version_major}.${zisc_version_minor}.${zisc_version_patch}")
  message(STATUS "## Zisc version: ${zisc_version}")
  
  initZiscOption()
  processZiscOption()
  
  ## Make configuration header file
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
  set(zisc_config_path ${PROJECT_BINARY_DIR}/include/zisc)
  configure_file(${__zisc_root__}/zisc/zisc_config.hpp.in
                 ${zisc_config_path}/zisc_config.hpp)
 
  ## Set header files
  file(GLOB hpp_files ${__zisc_root__}/zisc/*.hpp)
  set(hpp_files ${hpp_files} ${zisc_config_path}/zisc_config.hpp)
  source_group(Zisc FILES ${hpp_files})

  ## Set variables 
  set(zisc_include_dirs ${__zisc_root__} ${PROJECT_BINARY_DIR}/include PARENT_SCOPE)
endfunction(loadZisc)
