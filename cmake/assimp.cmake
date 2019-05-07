# file: assimp.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2019 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


function(parseAssimpFlags assimp_compile_flags)
  set(compile_flags "")
  foreach(flag IN LISTS ARGN)
    if(NOT flag MATCHES "^SHELL:")
      list(APPEND compile_flags ${flag})
    endif()
  endforeach(flag)


  # Output variables
  set(${assimp_compile_flags} ${compile_flags} PARENT_SCOPE)
endfunction(parseAssimpFlags)


# Build assimp and get library info
function(buildAssimp assimp_project_root assimp_include_dir assimp_libraries)
  include(ExternalProject)
  set(assimp_build_dir ${PROJECT_BINARY_DIR}/assimp)
  set(assimp_build_command ${CMAKE_COMMAND} --build . --config ${CMAKE_BUILD_TYPE})

  getCxxCompilerOption(assimp_flags assimp_linker_flgas assimp_definitions)
  parseAssimpFlags(assimp_flags ${assimp_flags})
  string(REPLACE ";" " " assimp_flags "${assimp_flags}")
  string(REPLACE ";" " " assimp_linker_flgas "${assimp_linker_flgas}")
  string(REPLACE ";" " " assimp_definitions "${assimp_definitions}")
  set(assimp_cmake_options "")

  if(Z_RELEASE_MODE)
    set(assimp_postfix "")
  else()
    set(assimp_postfix "d")
  endif()
  if(Z_VISUAL_STUDIO)
    if(MSVC70 OR MSVC71)
      set(msvc_postfix "vc70")
    elseif(MSVC80)
      set(msvc_postfix "vc80")
    elseif(MSVC90)
      set(msvc_postfix "vc90")
    elseif(MSVC10)
      set(msvc_postfix "vc100")
    elseif(MSVC11)
      set(msvc_postfix "vc110")
    elseif(MSVC12)
      set(msvc_postfix "vc120")
    elseif(MSVC14)
      set(msvc_postfix "vc140")
    else()
      set(msvc_postfix "vc150")
    endif()
    set(msvc_postfix "-${msvc_postfix}-mt")
    set(assimp_postfix "${msvc_postfix}${assimp_postfix}")

    list(APPEND assimp_cmake_options -DASSIMP_INSTALL_PDB=OFF
        -DCMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE=${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE})
    set(libassimp_lib_name "assimp${assimp_postfix}.lib")
  elseif(Z_XCODE)
    set(assimp_toolset ${CMAKE_GENERATOR_TOOLSET})
    set(assimp_c_compiler ${CMAKE_C_COMPILER})
    set(assimp_cxx_compiler ${CMAKE_CXX_COMPILER})
    set(libassimp_lib_name "libassimp${assimp_postfix}.a")
  else()
    set(assimp_toolset ${CMAKE_GENERATOR_TOOLSET})
    set(assimp_c_compiler ${CMAKE_C_COMPILER})
    set(assimp_cxx_compiler ${CMAKE_CXX_COMPILER})
    set(libassimp_lib_name "libassimp${assimp_postfix}.a")
  endif()

  externalproject_add(Assimp
                      PREFIX ${assimp_build_dir}
                      SOURCE_DIR ${assimp_project_root}
                      CMAKE_GENERATOR ${CMAKE_GENERATOR}
                      CMAKE_GENERATOR_PLATFORM ${CMAKE_GENERATOR_PLATFORM}
                      CMAKE_GENERATOR_TOOLSET ${assimp_toolset}
                      CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                 -DCMAKE_INSTALL_PREFIX=${assimp_build_dir}
                                 -DCMAKE_C_COMPILER=${assimp_c_compiler}
                                 -DCMAKE_CXX_COMPILER=${assimp_cxx_compiler}
                                 -DCMAKE_C_FLAGS=${assimp_flags}
                                 -DCMAKE_CXX_FLAGS=${assimp_flags}
                                 -DCMAKE_EXE_LINKER_FLAGS=${assimp_linker_flgas}
                                 -DBUILD_SHARED_LIBS=OFF
                                 -DASSIMP_DOUBLE_PRECISION=ON
                                 -DASSIMP_NO_EXPORT=ON
                                 -DASSIMP_BUILD_TESTS=OFF
                                 -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
                                 -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=OFF
                                 -DASSIMP_BUILD_OBJ_IMPORTER=ON
                                 ${assimp_cmake_options}
                      BUILD_COMMAND ${assimp_build_command})
  # Set libraries
  # libassimp
  add_library(assimp STATIC IMPORTED)
  set(libassimp_path ${assimp_build_dir}/lib/${libassimp_lib_name})
  set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${libassimp_path})
  add_dependencies(assimp Assimp)


  # Output variables
  set(${assimp_include_dir} ${assimp_build_dir}/include PARENT_SCOPE)
  set(${assimp_libraries} assimp PARENT_SCOPE)
endfunction(buildAssimp)
