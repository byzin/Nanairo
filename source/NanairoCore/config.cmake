# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__nanairo_core_root__ ${CMAKE_CURRENT_LIST_DIR})


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

  if(NANAIRO_USE_EFFICIENT_MEMORY_MANAGER)
    list(APPEND definitions "NANAIRO_USE_EFFICIENT_MEMORY_MANAGER")
  endif()

  # Output variables
  set(${core_source_files} ${source_files} PARENT_SCOPE)
  set(${core_definitions} ${definitions} PARENT_SCOPE)
endfunction(getNanairoCore)
