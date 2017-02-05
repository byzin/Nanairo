# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2016 Sho Ikeda
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


# Compute the spectral transport parameters
function(computeSpectralTransportParameters resource_dir script_file_dir parameter_file)
  # Initialize python environment
  find_package(PythonInterp 3.6 REQUIRED)
  message(STATUS "PythonInterp path: ${PYTHON_EXECUTABLE}")
  # Copy resources
  set(cmf_x_bar_file "cie_sco_2degree_xbar.csv")
  set(cmf_y_bar_file "cie_sco_2degree_ybar.csv")
  set(cmf_z_bar_file "cie_sco_2degree_zbar.csv")
  file(COPY ${resource_dir}/${cmf_x_bar_file}
            ${resource_dir}/${cmf_y_bar_file}
            ${resource_dir}/${cmf_z_bar_file}
       DESTINATION ${script_file_dir})
  # Compute paramenters
  configure_file(${__nanairo_core_root__}/Color/spectral_transport.py.in
                 ${script_file_dir}/spectral_transport.py)
  add_custom_command(
      OUTPUT ${script_file_dir}/${parameter_file}
      COMMAND ${PYTHON_EXECUTABLE} spectral_transport.py -o ${parameter_file}
      DEPENDS ${script_file_dir}/spectral_transport.py
      WORKING_DIRECTORY ${script_file_dir}
      COMMENT "Compute spectral transport parameters."
      VERBATIM)
endfunction(computeSpectralTransportParameters)


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


# Make NanairoCore resource file
function(makeCoreResource core_resource_file)
  # Set resource files
  file(GLOB resource_files
            ${PROJECT_BINARY_DIR}/resources/spectrum/illuminant/*.csv
            ${PROJECT_BINARY_DIR}/resources/spectrum/observer/*.csv)

  # Make a resource file code
  set(resource_file_code "")
  # Header
  list(APPEND resource_file_code "<RCC>\n")
  list(APPEND resource_file_code "  <qresource>\n")
  # Resources
  set(resource_dir ${PROJECT_BINARY_DIR}/resources)
  foreach(resource_file ${resource_files})
    file(RELATIVE_PATH resource_alias ${resource_dir} ${resource_file})
    list(APPEND resource_file_code
                "    <file alias=\"${resource_alias}\">${resource_file}</file>\n")
  endforeach(resource_file)
  # Footer
  list(APPEND resource_file_code "  </qresource>\n")
  list(APPEND resource_file_code "</RCC>")
  # Write the code to file
  file(WRITE ${core_resource_file} ${resource_file_code})
  source_group(NanairoCore FILES ${core_resource_file})
endfunction(makeCoreResource)


# Load nanairo core files
function(getNanairoCore core_source_files core_definitions)
  # Source files
  findNanairoSourceFiles(${__nanairo_core_root__} source_files)
  # Definitions
  set(definitions "")
  # SpectralTransport
  set(cmf_resource_dir ${PROJECT_SOURCE_DIR}/resources/spectrum/observer)
  set(script_file_dir ${PROJECT_BINARY_DIR}/include/NanairoCore/Color)
  set(parameter_file spectral_transport_parameters.hpp)
  file(MAKE_DIRECTORY ${script_file_dir})
  computeSpectralTransportParameters(${cmf_resource_dir} ${script_file_dir} ${parameter_file})
  list(APPEND source_files ${script_file_dir}/${parameter_file})
  # Config file
  set(config_file_path
      ${PROJECT_BINARY_DIR}/include/NanairoCore/nanairo_core_config.hpp)
  set(config_file_inl_path
      ${PROJECT_BINARY_DIR}/include/NanairoCore/nanairo_core_config-inl.hpp)
  makeCoreConfigFile(${config_file_path} ${config_file_inl_path})
  list(APPEND source_files ${config_file_path})
  # Resource
  set(resource_file ${PROJECT_BINARY_DIR}/resource_code/core_resource.qrc)
  makeCoreResource(${resource_file})
  list(APPEND source_files ${resource_file})


  # Output variables
  set(${core_source_files} ${source_files} PARENT_SCOPE)
  set(${core_definitions} ${definitions} PARENT_SCOPE)
endfunction(getNanairoCore)
