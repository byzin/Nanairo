# file: general.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0)


# Inner functions and macros

# Prohibit building on all directories except debug and release directory.
function(restrictBuildDirectory)
  set(build_dir "${PROJECT_SOURCE_DIR}/build")
  if(NOT PROJECT_BINARY_DIR MATCHES "${build_dir}(/*)?")
    message(FATAL_ERROR "\"${PROJECT_BINARY_DIR}\" dir is not allowed to build.\n"
                        "Please build on \"${build_dir}\".")
  endif()
endfunction(restrictBuildDirectory)


# Set build type
macro(setBuildType)
  # Build type
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
  endif()
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(z_debug ON)
    add_definitions(-DZ_DEBUG)
  elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(z_release_debug ON)
    add_definitions(-DZ_RELEASE_DEBUG)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(z_release ON)
    add_definitions(-DZ_RELEASE)
  else()
    message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
  endif()
  message(STATUS "## Build type: ${CMAKE_BUILD_TYPE}")
endmacro(setBuildType)


# Detect platform
macro(detectPlatform)
  # Detect OS
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(z_windows ON)
    add_definitions(-DZ_WINDOWS)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(z_linux ON)
    add_definitions(-DZ_LINUX)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(z_mac ON)
    add_definitions(-DZ_MAC)
  else()
    message(WARNING "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
  endif()
  message(STATUS "## System: ${CMAKE_SYSTEM_NAME}")

  # Detect C++ compiler
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(z_gcc ON)
    add_definitions(-DZ_GCC)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR 
         CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(z_clang ON)
    add_definitions(-DZ_CLANG)
  else()
    message(WARNING "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
  endif()
  message(STATUS "## Compiler: ${CMAKE_CXX_COMPILER_ID}")

  # Detect environment
  if(CMAKE_GENERATOR MATCHES ".*Makefiles")
    set(z_makefile ON)
    set(z_cui ON)
  elseif (CMAKE_GENERATOR STREQUAL "Xcode")
    set(z_xcode ON)
    set(z_ide ON)
  else()
    message(WARNING "Unsupported environment: ${CMAKE_GENERATOR}")
  endif()
  message(STATUS "## Develop environment: ${CMAKE_GENERATOR}")
endmacro(detectPlatform)


# Functions and macros

# Initialize
macro(initializeBuildEnvironment)
  restrictBuildDirectory()
  setBuildType()
  detectPlatform()
endmacro(initializeBuildEnvironment)
