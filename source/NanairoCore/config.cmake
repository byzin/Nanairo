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


  # Output variables
  set(${sampler_header_path} ${header_path} PARENT_SCOPE)
  set(${sampler_type} ${type} PARENT_SCOPE)
endfunction(getCoreSamplerType)


# Make Nanairo Core config file
function(makeCoreConfigFile config_file_path config_file_inl_path)
  getCoreSamplerType(nanairo_core_sampler_header_path nanairo_core_sampler_type)
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
  if(NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY)
    list(APPEND definitions -DNANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY)
  elseif(NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY)
    list(APPEND definitions -DNANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY)
  endif()
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
