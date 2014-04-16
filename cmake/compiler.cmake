# file: compiler.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.0)


# Inner functions and macros

# GCC configuration
macro(setGccCompilerOptionsForCxx11)
  # Version check
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.0)
    message(FATAL_ERROR "Use GCC version 4.8 or later.")
  endif()
  # Add Compile options
  add_compile_options(-std=c++11)
endmacro(setGccCompilerOptionsForCxx11)


# Clang configuration
macro(setClangCompilerOptionsForCxx11)
  # Version check
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2.0)
    message(FATAL_ERROR "Use Clang version 3.2 or later.")
  endif()
  # Add Compile options
  add_compile_options(-std=c++11)
  if(z_mac)
    add_compile_options(-stdlib=libc++)
  endif()
endmacro(setClangCompilerOptionsForCxx11)


# GCC configuration
macro(setGccCompilerOptionsForCxx14)
  # Version check
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0.0)
    message(FATAL_ERROR "Use GCC version 5.0 or later.")
  endif()
  # Add Compile options
  add_compile_options(-std=c++14)
endmacro(setGccCompilerOptionsForCxx14)


# Clang configuration
macro(setClangCompilerOptionsForCxx14)
  # Version check
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.5.0)
    message(FATAL_ERROR "Use Clang version 3.5 or later.")
  endif()
  # Add Compile options
  add_compile_options(-std=c++14)
  if(z_mac)
    add_compile_options(-stdlib=libc++)
  endif()
endmacro(setClangCompilerOptionsForCxx14)


# Functions and macros

# Initialize C++ compiler options
macro(setCompilerOptionsForCxx11)
  if(z_gcc)
    setGccCompilerOptionsForCxx11()
  elseif(z_clang)
    setClangCompilerOptionsForCxx11()
  # TODO: Add new compiler initialization
  else()
    message(FATAL_ERROR "\"${CMAKE_CXX_COMPILER_ID}\" doesn't support C++11")
  endif()
endmacro(setCompilerOptionsForCxx11)


# Initialize C++ compiler options
macro(setCompilerOptionsForCxx14)
  if(z_gcc)
    setGccCompilerOptionsForCxx14()
  elseif(z_clang)
    setClangCompilerOptionsForCxx14()
  # TODO: Add new compiler initialization
  else()
    message(FATAL_ERROR "\"${CMAKE_CXX_COMPILER_ID}\" doesn't support C++14")
  endif()
endmacro(setCompilerOptionsForCxx14)


# Set C++ warning flags
macro(setCompilerWarningOptionsForCxx)
  if(z_gcc OR z_clang)
    add_compile_options(-Wall -Wextra)
  else()
    message(WARNING "\"${CMAKE_CXX_COMPILER_ID}\" doesn't support worning option.")
  endif()
endmacro(setCompilerWarningOptionsForCxx)
