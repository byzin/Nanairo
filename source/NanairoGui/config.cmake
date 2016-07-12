# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.4)

set(__nanairo_gui_root__ ${CMAKE_CURRENT_LIST_DIR})


# Make a javascript file whitch contains Nanairo keywords
function(makeNanairoGuiKeywordFile keyword_dir)
  configure_file(${__nanairo_gui_root__}/keyword.js.in
                 ${keyword_dir}/keyword.js)

  set(keyword_list ${ARGN})
  list(LENGTH keyword_list list_size)
  math(EXPR length "${list_size} / 2")
  foreach(number RANGE 1 ${length})
    math(EXPR variable_index "(${number} - 1) * 2")
    math(EXPR keyword_index "${variable_index} + 1")
    list(GET keyword_list ${variable_index} variable)
    list(GET keyword_list ${keyword_index} keyword)
    file(APPEND ${keyword_dir}/keyword.js "var ${variable} = \"${keyword}\"\n")
  endforeach()
endfunction(makeNanairoGuiKeywordFile)


# Make Nanairo UI resource file
function(makeNanairoGuiResource gui_resource_file)
  set(resource_directory ${PROJECT_BINARY_DIR}/build_resources)
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

  set(${gui_resource_file} ${resource_file} PARENT_SCOPE)
endfunction(makeNanairoGuiResource)


# Make Nanairo GUI config file
function(makeGuiConfigFile config_file_path)
  configure_file(${__nanairo_gui_root__}/nanairo_gui_config.hpp.in
                 ${config_file_path})
  source_group(NanairoGui FILES ${config_file_path})
endfunction(makeGuiConfigFile)


# Add nanairo ui source files
macro(getNanairoGuiFiles dir_name nanairo_source_files)
  getNanairoSourceFiles(NanairoGui 
                        ${__nanairo_gui_root__} 
                        "${dir_name}"
                        ${nanairo_source_files})
endmacro(getNanairoGuiFiles)


# Functions and macros

# Build NanairoGui
# defined variable
#   nanairo_gui_library: 
function(getNanairoGui gui_source_files gui_definitions)
  # Set source files
  getNanairoGuiFiles("" nanairo_files)
  getNanairoGuiFiles("Utility" utility_files)
  set(source_files ${nanairo_files} ${utility_files})
  # Resource
  makeNanairoGuiResource(gui_resource_file)
  # Config file
  set(config_dir ${PROJECT_BINARY_DIR}/include/NanairoGui)
  makeGuiConfigFile(${config_dir}/nanairo_gui_config.hpp)
  # Output variables
  set(${gui_source_files} ${source_files} ${gui_resource_file} PARENT_SCOPE)
endfunction(getNanairoGui)
