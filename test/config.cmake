# file: test.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2017 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

set(__test_root__ ${CMAKE_CURRENT_LIST_DIR})


function(getTestWarningOption test_warning_flags)
  set(warning_flags "")
  if(Z_CLANG AND Z_VISUAL_STUDIO)
  elseif(Z_CLANG)
  elseif(Z_GCC)
  endif()


  # Output variable
  set(${test_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction()



# Build tests
function(buildUnitTest)
  # Load GoogleTest
  set(gtest_setting_file ${zisc_path}/cmake/googletest.cmake)
  if(NOT EXISTS ${gtest_setting_file})
    showSubmoduleWarning(zisc)
  endif()
  include(${gtest_setting_file})
  set(gtest_project_root ${__test_root__}/googletest)
  if(NOT EXISTS ${gtest_setting_file})
    showSubmoduleWarning(googletest)
  endif()
  buildGoogleTest(${gtest_project_root} gtest_include_dir gtest_libraries)

  # Build unit tests
  file(GLOB unittest_source_files ${__test_root__}/unittest/*.cpp)
  file(GLOB unittest_header_files ${__test_root__}/unittest/*.hpp)
  add_executable(UnitTest ${unittest_source_files}
                          ${unittest_header_files}
                          ${core_source_files}
                          ${zisc_source_files})
  source_group(UnitTest FILES ${unittest_source_files}
                              ${unittest_header_files})
  # Set unit test properties
  set_target_properties(UnitTest PROPERTIES CXX_STANDARD 14
                                            CXX_STANDARD_REQUIRED ON
                                            AUTOMOC ON
                                            AUTORCC ON)
  getCxxWarningOption(cxx_warning_flags)
  getTestWarningOption(test_warning_flags)
  checkCompilerHasCxx14Features(UnitTest)
  target_compile_options(UnitTest PRIVATE ${cxx_compiler_flags}
                                          ${qt5_compile_flags}
                                          ${zisc_compile_flags}
                                          ${cxx_warning_flags}
                                          ${test_warning_flags})
  target_include_directories(UnitTest PRIVATE ${PROJECT_SOURCE_DIR}/source
                                              ${PROJECT_BINARY_DIR}/include
                                              ${zisc_include_dirs})
  target_include_directories(UnitTest SYSTEM PRIVATE ${qt5_include_dirs}
                                                     ${gtest_include_dir})
  target_link_libraries(UnitTest ${CMAKE_THREAD_LIBS_INIT}
                                 ${cxx_linker_flags}
                                 ${qt5_libraries} 
                                 ${gtest_libraries}
                                 ${zisc_linker_flags}
                                 ${core_library})
  target_compile_definitions(UnitTest PRIVATE ${cxx_definitions}
                                              ${core_definitions}
                                              ${gui_definitions}
                                              ${qt5_definitions}
                                              ${zisc_definitions}
                                              ${environment_definitions})
endfunction(buildUnitTest)
