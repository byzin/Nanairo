# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0)


set(__nanairo_renderer_root__ ${CMAKE_CURRENT_LIST_DIR})


# Inner functions and macros

# Add nanairo renderer source files
macro(addNanairoRendererFiles dir_name)
  addNanairoSourceFiles(NanairoRenderer ${__nanairo_renderer_root__} "${dir_name}")
endmacro(addNanairoRendererFiles)


# Make Nanairo Renderer config file
function(makeRendererConfigFile)
  set(renderer_config_path ${PROJECT_BINARY_DIR}/include/NanairoRenderer)
  configure_file(${__nanairo_renderer_root__}/nanairo_renderer_config.hpp.in
                 ${renderer_config_path}/nanairo_renderer_config.hpp)
  source_group(NanairoRenderer FILES ${renderer_config_path}/nanairo_renderer_config.hpp)
endfunction(makeRendererConfigFile)

# Functions and macros

# Build NanairoRenderer
# Defined variables
#   nanairo_renderer_library
function(buildNanairoRenderer)
  # Set source files
  clearNanairoSourceFiles()
  addNanairoRendererFiles("")

  makeRendererConfigFile()

  # Build nanairo renderer
  add_library(NanairoRenderer STATIC ${nanairo_source_files})
  target_link_libraries(NanairoRenderer ${qt5_libraries} ${nanairo_core_library})
  set_target_properties(NanairoRenderer PROPERTIES AUTOMOC ON)


  set(nanairo_renderer_library NanairoRenderer PARENT_SCOPE)
endfunction(buildNanairoRenderer)
