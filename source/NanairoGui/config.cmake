# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0)

set(__nanairo_gui_root__ ${CMAKE_CURRENT_LIST_DIR})


# Inner functions and macros

# Make a javascript file whitch contains Nanairo keywords
function(makeNanairoGuiKeywordFile dest_dir)
  configure_file(${__nanairo_gui_root__}/keyword.js.in ${dest_dir}/keyword.js)

  set(keyword_list ${ARGN})
  list(LENGTH keyword_list list_size)
  math(EXPR length "${list_size} / 2")
  foreach(number RANGE 1 ${length})
    math(EXPR variable_index "(${number} - 1) * 2")
    math(EXPR keyword_index "${variable_index} + 1")
    list(GET keyword_list ${variable_index} variable)
    list(GET keyword_list ${keyword_index} keyword)
    file(APPEND ${dest_dir}/keyword.js "var ${variable} = \"${keyword}\"\n")
  endforeach()
endfunction(makeNanairoGuiKeywordFile)


# Make Nanairo UI resource file
function(makeNanairoGuiResource)
  set(resource_directory ${PROJECT_BINARY_DIR}/resource)
  set(resource_file ${resource_directory}/nanairo_gui.qrc)

  file(MAKE_DIRECTORY ${resource_directory})

  file(WRITE ${resource_file} "<RCC>\n")
  file(APPEND ${resource_file} "  <qresource prefix=\"/\">\n")

  file(GLOB_RECURSE __nanairo_gui_files__ RELATIVE ${resource_directory}
      ${__nanairo_gui_root__}/*.qml
      ${__nanairo_gui_root__}/*.js
      ${__nanairo_gui_root__}/*.png)
  foreach(ui_file ${__nanairo_gui_files__})
    string(REGEX MATCH ".*(NanairoGui/.*)" __ui_alias__ ${ui_file})
    file(APPEND ${resource_file} "    <file alias=\"${CMAKE_MATCH_1}\">${ui_file}</file>\n")
  endforeach(ui_file)

  makeNanairoGuiKeywordFile(${resource_directory} ${nanairo_keyword_list})

  file(APPEND ${resource_file} "    <file alias=\"NanairoGui/keyword.js\">${resource_directory}/keyword.js</file>\n")

  file(APPEND ${resource_file} "  </qresource>\n")
  file(APPEND ${resource_file} "</RCC>\n")

  set(nanairo_gui_resource_path ${resource_file} PARENT_SCOPE)
endfunction(makeNanairoGuiResource)


# Make Nanairo GUI config file
macro(makeGuiConfigFile)
  configure_file(${__nanairo_gui_root__}/nanairo_gui_config.hpp.in
                 ${nanairo_config_dir}/NanairoGui/nanairo_gui_config.hpp)
  source_group(NanairoGui FILES ${nanairo_config_dir}/NanairoGui/nanairo_gui_config.hpp)
endmacro(makeGuiConfigFile)


# Add nanairo ui source files
macro(addNanairoGuiFiles dir_name)
  addNanairoSourceFiles(NanairoGui ${__nanairo_gui_root__} "${dir_name}")
endmacro(addNanairoGuiFiles)


# Functions and macros

# Build NanairoGui
# defined variable
#   nanairo_gui_library: 
function(buildNanairoGui)
  # Set source files
  clearNanairoSourceFiles()
  addNanairoGuiFiles("")
  addNanairoGuiFiles("Utility")

  makeNanairoGuiResource()
  makeGuiConfigFile()

  add_library(NanairoGui STATIC ${nanairo_source_files})
  target_link_libraries(NanairoGui ${qt5_libraries} 
                                   ${nanairo_core_library}
                                   ${nanairo_renderer_library})
  set_target_properties(NanairoGui PROPERTIES AUTOMOC ON)

  set(nanairo_gui_library NanairoGui PARENT_SCOPE)
endfunction(buildNanairoGui)
