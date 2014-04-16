# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0)

set(__nanairo_core_root__ ${CMAKE_CURRENT_LIST_DIR})


# Inner functions and macros

# Add nanairo core source files
macro(addNanairoCoreFiles dir_name)
  addNanairoSourceFiles(NanairoCore ${__nanairo_core_root__} "${dir_name}")
endmacro(addNanairoCoreFiles)


# Set the sampler type
macro(setNanairoSamplerType)
  if(NANAIRO_SAMPLER STREQUAL "PCG")
    set(nanairo_core_sampler_header_path "zisc/pcg_engine.hpp")
    set(nanairo_core_sampler_type "zisc::PcgMcgRxsMXs32")
  elseif(NANAIRO_SAMPLER STREQUAL "XSadd")
    set(nanairo_core_sampler_header_path "zisc/xsadd_engine.hpp")
    set(nanairo_core_sampler_type "zisc::XsaddEngine")
  elseif(NANAIRO_SAMPLER STREQUAL "dSFMT")
    set(nanairo_core_sampler_header_path "zisc/dsfmt_engine.hpp")
    set(nanairo_core_sampler_type "zisc::Dsfmt19937")
  elseif(NANAIRO_SAMPLER STREQUAL "Xorshift")
    set(nanairo_core_sampler_header_path "zisc/xorshift_engine.hpp")
    set(nanairo_core_sampler_type "zisc::XorshiftEngine")
  else()
    message(FATAL_ERROR "${NANAIRO_SAMPLER} is invalid sampler type.")
  endif()
endmacro(setNanairoSamplerType)


# Make Nanairo Core config file
function(makeCoreConfigFile)
  setNanairoSamplerType()
  set(core_config_dir ${PROJECT_BINARY_DIR}/include/NanairoCore)
  configure_file(${__nanairo_core_root__}/nanairo_core_config.hpp.in
                 ${core_config_dir}/nanairo_core_config.hpp)
  source_group(NanairoCore FILES ${core_config_dir}/nanairo_core_config.hpp)
endfunction(makeCoreConfigFile)


# Functions and files

# Load nanairo core files
function(buildNanairoCore)
  clearNanairoSourceFiles()
  addNanairoCoreFiles("")
  addNanairoCoreFiles(CameraModel)
  addNanairoCoreFiles(Color)
  addNanairoCoreFiles(Data)
  addNanairoCoreFiles(DataStructure)
  addNanairoCoreFiles(Geometry)
  addNanairoCoreFiles(LinearAlgebra)
  addNanairoCoreFiles(Material)
  addNanairoCoreFiles(Material/Bxdf)
  addNanairoCoreFiles(Material/EmitterModel)
  addNanairoCoreFiles(Material/Light)
  addNanairoCoreFiles(Material/Sensor)
  addNanairoCoreFiles(Material/SurfaceModel)
  addNanairoCoreFiles(Material/Texture)
  addNanairoCoreFiles(RenderingMethod)
  addNanairoCoreFiles(Sampling)
  addNanairoCoreFiles(ToneMapping)
  addNanairoCoreFiles(Utility)

  if(NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY)
    add_definitions(-DNANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY)
  elseif(NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY)
    add_definitions(-DNANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY)
  endif()

  makeCoreConfigFile()

  # Build nanairo core
  add_library(NanairoCore STATIC ${nanairo_source_files})
  target_link_libraries(NanairoCore ${qt5_libraries})
  set_target_properties(NanairoCore PROPERTIES AUTOMOC ON)


  set(nanairo_core_library NanairoCore PARENT_SCOPE)
endfunction(buildNanairoCore)
