# file: general.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

# Set boolean value option
macro(setBooleanOption variable value doc_string)
  set(${variable} ${value} CACHE BOOL ${doc_string})
endmacro(setBooleanOption)


# Set string value option
macro(setStringOption variable value doc_string)
  set(${variable} ${value} CACHE STRING ${doc_string})
endmacro(setStringOption)


# Prohibit building on all directories except debug and release directory.
function(restrictBuildDirectory build_dir)
  if(NOT PROJECT_BINARY_DIR MATCHES "${build_dir}(/*)?")
    message(FATAL_ERROR "\"${PROJECT_BINARY_DIR}\" dir is not allowed to build.\n"
                        "Please build on \"${build_dir}\".")
  endif()
endfunction(restrictBuildDirectory)


# Check development environment
function(detectEnvironment environment_definitions)
  # Detect Platform
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(Z_IS_WINDOWS ON PARENT_SCOPE)
    set(Z_PLATFORM_DEFINITION Z_WINDOWS)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(Z_IS_LINUX ON PARENT_SCOPE)
    set(Z_PLATFORM_DEFINITION Z_LINUX)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(Z_IS_MAC ON PARENT_SCOPE)
    set(Z_PLATFORM_DEFINITION Z_MAC)
  else()
    set(unsupported_platform ON)
  endif()
  list(APPEND definitions ${Z_PLATFORM_DEFINITION})

  if(unsupported_platform)
    message(WARNING "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
  else()
    message(STATUS "## Platform: ${CMAKE_SYSTEM_NAME}")
  endif()

  # Detect C++ compiler
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(Z_IS_GCC ON PARENT_SCOPE)
    set(Z_COMPILER_DEFINITION Z_GCC)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR 
         CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(Z_IS_CLANG ON PARENT_SCOPE)
    set(Z_COMPILER_DEFINITION Z_CLANG)
  else()
    set(unsupported_compiler ON)
  endif()
  list(APPEND definitions ${Z_COMPILER_DEFINITION})

  if(unsupported_compiler)
    message(WARNING "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
  else()
    message(STATUS "## Compiler: ${CMAKE_CXX_COMPILER_ID}")
  endif()

  # Detect environment
  if(CMAKE_GENERATOR MATCHES ".*Makefiles")
    set(Z_IS_MAKEFILE ON PARENT_SCOPE)
    set(Z_GENERATOR_DEFINITION Z_MAKEFILE)
  elseif(CMAKE_GENERATOR MATCHES "Visual Studio.*")
    set(Z_IS_VISUAL_STUDIO ON PARENT_SCOPE)
    set(Z_GENERATOR_DEFINITION Z_VISUAL_STUDIO)
  else()
    set(unsupported_generator ON)
  endif()
  list(APPEND definitions ${Z_GENERATOR_DEFINITION})

  if(unsupported_generator)
    message(WARNING "Unsupported generator: ${CMAKE_GENERATOR}")
  else()
    message(STATUS "## Generator: ${CMAKE_GENERATOR}")
  endif()


  # Detect build type
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(Z_IS_DEBUG_BUILD ON PARENT_SCOPE)
    set(Z_BUILD_TYPE_DEFINITION Z_DEBUG_BUILD)
  elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(Z_IS_DEBUG_BUILD ON PARENT_SCOPE)
    set(Z_IS_RELEASE_BUILD ON PARENT_SCOPE)
    set(Z_BUILD_TYPE_DEFINITION DZ_DEBUG_BUILD Z_RELEASE_BUILD)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Z_IS_RELEASE_BUILD ON PARENT_SCOPE)
    set(Z_BUILD_TYPE_DEFINITION Z_RELEASE_BUILD)
  else()
    set(unsupported_build_type ON)
  endif()
  list(APPEND definitions ${Z_BUILD_TYPE_DEFINITION})

  if(unsupported_build_type)
    message(WARNING "Unsupported build type: ${CMAKE_BUILD_TYPE}")
  else()
    message(STATUS "## Build type: ${CMAKE_BUILD_TYPE}")
  endif()

  set(${environment_definitions} ${definitions} PARENT_SCOPE)
endfunction(detectEnvironment)


#
function(setIncludeWhatYouUse target)
  find_program(iwyu include-what-you-use)
  if(iwyu)
    set_target_properties(${target} 
                          PROPERTIES CXX_INCLUDE_WHAT_YOU_USE include-what-you-use)
  else()
    message(WARNING "${target}: Could not find include-what-you-use.")
  endif()
endfunction(setIncludeWhatYouUse)
