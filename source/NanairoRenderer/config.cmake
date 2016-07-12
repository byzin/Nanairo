# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.4)


set(__nanairo_renderer_root__ ${CMAKE_CURRENT_LIST_DIR})


# Inner functions and macros

# Add nanairo renderer source files
macro(getNanairoRendererFiles dir_name nanairo_source_files)
  getNanairoSourceFiles(NanairoRenderer
                        ${__nanairo_renderer_root__}
                        "${dir_name}"
                        ${nanairo_source_files})
endmacro(getNanairoRendererFiles)


# Make Nanairo Renderer config file
function(makeRendererConfigFile config_file_path)
  configure_file(${__nanairo_renderer_root__}/nanairo_renderer_config.hpp.in
                 ${config_file_path})
  source_group(NanairoRenderer FILES ${config_file_path})
endfunction(makeRendererConfigFile)


# Build NanairoRenderer
# Defined variables
#   nanairo_renderer_library
function(getNanairoRenderer renderer_source_files renderer_definitions)
  # Source files
  getNanairoRendererFiles("" nanairo_files)
  set(source_files ${nanairo_files})
  # Config file
  set(renderer_config_path ${PROJECT_BINARY_DIR}/include/NanairoRenderer)
  makeRendererConfigFile(${renderer_config_path}/nanairo_renderer_config.hpp)


  # Output variables
  set(${renderer_source_files} ${source_files} PARENT_SCOPE)
endfunction(getNanairoRenderer)
