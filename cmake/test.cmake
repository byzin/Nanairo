# file: test.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0)


# Inner functions and macros

# Build the google test
macro(buildGoogleTest)
  add_library(GoogleTest SHARED ${PROJECT_SOURCE_DIR}/test/gtest/gtest-all.cc
                                ${PROJECT_SOURCE_DIR}/test/gtest/gtest/gtest.h)
endmacro(buildGoogleTest)


# Functions and macros

# Build tests
function(buildTest)
  buildGoogleTest()
  include_directories(${PROJECT_SOURCE_DIR}/test/gtest)
  aux_source_directory(${PROJECT_SOURCE_DIR}/test/UnitTest unit_test_sources)
  add_executable(UnitTest ${unit_test_sources} ${nanairo_core_resource_file})
  target_link_libraries(UnitTest GoogleTest 
                                 ${nanairo_dependencies}
                                 ${qt5_libraries} 
                                 ${nanairo_core_library})
endfunction(buildTest)
