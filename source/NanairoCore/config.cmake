# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2017 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__nanairo_core_root__ ${CMAKE_CURRENT_LIST_DIR})


# Set the sampler type
function(getCoreSamplerType sampler_header_path sampler_type)
  if(NANAIRO_SAMPLER STREQUAL "Xorshift128+")
    set(header_path "zisc/xorshift_engine.hpp")
    set(type "zisc::Xorshift128Plus")
  elseif(NANAIRO_SAMPLER STREQUAL "SplitMix64")
    set(header_path "zisc/split_mix64_engine.hpp")
    set(type "zisc::SplitMix64Engine")
  elseif(NANAIRO_SAMPLER STREQUAL "PCG")
    set(header_path "zisc/pcg_engine.hpp")
    set(type "zisc::PcgMcgRxsMXs")
  elseif(NANAIRO_SAMPLER STREQUAL "Xoroshiro128+")
    set(header_path "zisc/xoroshiro128_plus_engine.hpp")
    set(type "zisc::Xoroshiro128PlusEngine")
  else()
    message(FATAL_ERROR "${NANAIRO_SAMPLER} is invalid sampler type.")
  endif()


  # Output variables
  set(${sampler_header_path} ${header_path} PARENT_SCOPE)
  set(${sampler_type} ${type} PARENT_SCOPE)
endfunction(getCoreSamplerType)


# Configure the spectral transport parameters
function(configureSpectralTransportParameters config_file_path)
  set(parameter_dir ${__nanairo_core_root__}/Color/SpectralTransportParameters)
  set(parameter_impl_header "spectral_transport_parameters_${NANAIRO_SHORTEST_WAVELENGTH}_${NANAIRO_LONGEST_WAVELENGTH}_${NANAIRO_WAVELENGTH_RESOLUTION}.hpp")
  if(EXISTS ${parameter_dir}/${parameter_impl_header})
    configure_file(
        ${parameter_dir}/spectral_transport_parameters.hpp.in
        ${config_file_path}
        @ONLY)
  else()
    message(FATAL_ERROR "Parameter file (\"${parameter_impl_header}\") not found.")
  endif()
  source_group(NanairoCore FILES ${config_file_path})
endfunction(configureSpectralTransportParameters)


# Make Nanairo Core config file
function(makeCoreConfigFile config_file_path config_file_inl_path)
  # Sampler setting
  getCoreSamplerType(nanairo_core_sampler_header_path nanairo_core_sampler_type)
  # Path tracing setting
    set(NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_IS_ENABLED "true")
    set(NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_IS_ENABLED "true")
  if(NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY)
    set(NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_IS_ENABLED "false")
  elseif(NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY)
    set(NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_IS_ENABLED "false")
  endif()
  if(NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY AND
     NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY)
    message(FATAL_ERROR "'NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY' and 'NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY' can not be specified together.")
  endif()

  configure_file(${__nanairo_core_root__}/nanairo_core_config.hpp.in
                 ${config_file_path})
  configure_file(${__nanairo_core_root__}/nanairo_core_config-inl.hpp.in
                 ${config_file_inl_path})
  source_group(NanairoCore FILES ${config_file_path})
endfunction(makeCoreConfigFile)


# Load nanairo core files
function(getNanairoCore core_source_files core_definitions)
  # Source files
  findNanairoSourceFiles(${__nanairo_core_root__} source_files)
  # Definitions
  set(definitions "")
  set(parameter_file_path
      ${PROJECT_BINARY_DIR}/include/NanairoCore/Color/SpectralTransportParameter/spectral_transport_parameters.hpp)
  configureSpectralTransportParameters(${parameter_file_path})
  list(APPEND source_files ${parameter_file_path})
  # Config file
  set(config_file_path
      ${PROJECT_BINARY_DIR}/include/NanairoCore/nanairo_core_config.hpp)
  set(config_file_inl_path
      ${PROJECT_BINARY_DIR}/include/NanairoCore/nanairo_core_config-inl.hpp)
  makeCoreConfigFile(${config_file_path} ${config_file_inl_path})
  list(APPEND source_files ${config_file_path})


  # Output variables
  set(${core_source_files} ${source_files} PARENT_SCOPE)
  set(${core_definitions} ${definitions} PARENT_SCOPE)
endfunction(getNanairoCore)
