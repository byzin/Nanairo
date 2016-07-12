# file: test.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.4)


# Inner functions and macros

# Build the google test
function(buildGoogleTest google_library)
  add_library(GoogleTest SHARED ${PROJECT_SOURCE_DIR}/test/gtest/gtest-all.cc
                                ${PROJECT_SOURCE_DIR}/test/gtest/gtest/gtest.h)
  set_target_properties(GoogleTest PROPERTIES CXX_STANDARD 14
                                              CXX_STANDARD_REQUIRED ON)
  checkCompilerHasCxx14Features(GoogleTest)
  target_compile_options(GoogleTest PRIVATE ${cxx_compiler_flags})
  target_link_libraries(GoogleTest ${CMAKE_THREAD_LIBS_INIT}
                                   ${cxx_linker_flags})
  target_compile_definitions(GoogleTest PRIVATE ${cxx_definitions}
                                                ${environment_definitions})


  # Output
  set(${google_library} GoogleTest PARENT_SCOPE)
endfunction(buildGoogleTest)


# Build tests
function(buildTest)
  buildGoogleTest(google_library)

  aux_source_directory(${PROJECT_SOURCE_DIR}/test/UnitTest unit_test_sources)
  add_executable(UnitTest ${unit_test_sources})
  set_target_properties(UnitTest PROPERTIES CXX_STANDARD 14
                                            CXX_STANDARD_REQUIRED ON)
  getCxxWarningOption(cxx_warning_flags)
  getNanairoWarningOption(nanairo_warning_flags)
  checkCompilerHasCxx14Features(UnitTest)
  target_compile_options(UnitTest PRIVATE ${cxx_compiler_flags}
                                          ${qt5_compile_flags}
                                          ${zisc_compile_flags}
                                          ${cxx_warning_flags}
                                          ${nanairo_warning_flags})
  target_include_directories(UnitTest PRIVATE ${PROJECT_SOURCE_DIR}/source
                                              ${PROJECT_BINARY_DIR}/include
                                              ${zisc_include_dirs})
  target_include_directories(UnitTest SYSTEM PRIVATE ${qt5_include_dirs}
                                                     ${PROJECT_SOURCE_DIR}/test/gtest)
  target_link_libraries(UnitTest ${PROJECT_NAME}Core
                                 GoogleTest 
                                 ${cxx_linker_flags}
                                 ${qt5_libraries} 
                                 ${zisc_linker_flags})
  target_compile_definitions(UnitTest PRIVATE ${cxx_definitions}
                                              ${core_definitions}
                                              ${renderer_definitions}
                                              ${gui_definitions}
                                              ${qt5_definitions}
                                              ${zisc_definitions}
                                              ${environment_definitions})
endfunction(buildTest)
