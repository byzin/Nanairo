# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__nanairo_renderer_root__ ${CMAKE_CURRENT_LIST_DIR})


# Make Nanairo Renderer config file
function(makeRendererConfigFile config_file_path)
  configure_file(${__nanairo_renderer_root__}/nanairo_renderer_config.hpp.in
                 ${config_file_path})
  source_group(NanairoRenderer FILES ${config_file_path})
endfunction(makeRendererConfigFile)


# Load nanairo renderer files
function(getNanairoRenderer renderer_source_files renderer_definitions)
  # Source files
  findNanairoSourceFiles(${__nanairo_renderer_root__} source_files)
  # Config file
  set(config_file_path ${PROJECT_BINARY_DIR}/include/NanairoRenderer)
  makeRendererConfigFile(${config_file_path}/nanairo_renderer_config.hpp)
  list(APPEND source_files ${config_file_path})


  # Output variables
  set(${renderer_source_files} ${source_files} PARENT_SCOPE)
endfunction(getNanairoRenderer)
