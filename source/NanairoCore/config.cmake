# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.4)

set(__nanairo_core_root__ ${CMAKE_CURRENT_LIST_DIR})


#
macro(getNanairoCoreFiles dir_name nanairo_source_files)
  getNanairoSourceFiles(NanairoCore
                        ${__nanairo_core_root__}
                        "${dir_name}"
                        ${nanairo_source_files})
endmacro(getNanairoCoreFiles)


# Set the sampler type
function(getNanairoSamplerType sampler_header_path sampler_type)
  if(NANAIRO_SAMPLER STREQUAL "PCG")
    set(header_path "zisc/pcg_engine.hpp")
    set(type "zisc::PcgMcgRxsMXs32")
  elseif(NANAIRO_SAMPLER STREQUAL "XSadd")
    set(header_path "zisc/xsadd_engine.hpp")
    set(type "zisc::XsaddEngine")
  elseif(NANAIRO_SAMPLER STREQUAL "dSFMT")
    set(header_path "zisc/dsfmt_engine.hpp")
    set(type "zisc::Dsfmt19937")
  elseif(NANAIRO_SAMPLER STREQUAL "Xorshift")
    set(header_path "zisc/xorshift_engine.hpp")
    set(type "zisc::XorshiftEngine")
  else()
    message(FATAL_ERROR "${NANAIRO_SAMPLER} is invalid sampler type.")
  endif()
  set(${sampler_header_path} ${header_path} PARENT_SCOPE)
  set(${sampler_type} ${type} PARENT_SCOPE)
endfunction(getNanairoSamplerType)


# Make Nanairo Core config file
function(makeCoreConfigFile config_file_path)
  getNanairoSamplerType(nanairo_core_sampler_header_path nanairo_core_sampler_type)
  configure_file(${__nanairo_core_root__}/nanairo_core_config.hpp.in
                 ${config_file_path})
  source_group(NanairoCore FILES ${config_file_path})
endfunction(makeCoreConfigFile)


# Load nanairo core files
function(getNanairoCore core_source_files core_definitions)
  # Source files
  getNanairoCoreFiles("" nanairo_files)
  getNanairoCoreFiles(CameraModel camera_model_files)
  getNanairoCoreFiles(Color color_files)
  getNanairoCoreFiles(Data data_files)
  getNanairoCoreFiles(DataStructure data_structure_files)
  getNanairoCoreFiles(Geometry geometry_files)
  getNanairoCoreFiles(LinearAlgebra linear_algebra_files)
  getNanairoCoreFiles(Material material_files)
  getNanairoCoreFiles(Material/Bxdf bxdf_files)
  getNanairoCoreFiles(Material/EmitterModel emitter_model_files)
  getNanairoCoreFiles(Material/Light light_files)
  getNanairoCoreFiles(Material/Sensor sensor_files)
  getNanairoCoreFiles(Material/SurfaceModel surface_model_files)
  getNanairoCoreFiles(Material/Texture texture_files)
  getNanairoCoreFiles(RenderingMethod rendering_method_files)
  getNanairoCoreFiles(Sampling sampling_files)
  getNanairoCoreFiles(ToneMapping tone_mapping_files)
  getNanairoCoreFiles(Utility utility_files)
  set(source_files ${nanairo_files}
                   ${camera_model_files}
                   ${color_files}
                   ${data_files}
                   ${data_structure_files}
                   ${geometry_files}
                   ${linear_algebra_files}
                   ${material_files}
                   ${bxdf_files}
                   ${emitter_model_files}
                   ${light_files}
                   ${sensor_files}
                   ${surface_model_files}
                   ${texture_files}
                   ${rendering_method_files}
                   ${sampling_files}
                   ${tone_mapping_files}
                   ${utility_files})
  # Definitions
  if(NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY)
    list(APPEND definitions -DNANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY)
  elseif(NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY)
    list(APPEND definitions -DNANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY)
  endif()
  # Config file
  set(config_dir ${PROJECT_BINARY_DIR}/include/NanairoCore)
  makeCoreConfigFile(${config_dir}/nanairo_core_config.hpp)
  # Resource
#  makeQtResources(core_resource_file ${PROJECT_SOURCE_DIR}/resources/resource.prc)
  set(core_resource_file ${PROJECT_SOURCE_DIR}/resources/resource.qrc)
  source_group(NanairoCore FILES ${core_resource_file})


  # Output variables
  set(${core_source_files} ${source_files} ${core_resource_file} PARENT_SCOPE)
  set(${core_definitions} ${definitions} PARENT_SCOPE)
endfunction(getNanairoCore)
