# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015-2019 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__nanairo_gui_root__ ${CMAKE_CURRENT_LIST_DIR})


function(makeRendererKeywordFile keyword_file_path)
  configure_file(${__nanairo_gui_root__}/keyword.hpp.in ${keyword_file_path})

  # Make a keyword list
  set(keyword_list ${ARGN})
  list(LENGTH keyword_list list_size)
  math(EXPR length "${list_size} / 2")
  # Make a keyword code
  set(keyword_code "")
  foreach(number RANGE 1 ${length})
    math(EXPR variable_index "(${number} - 1) * 2")
    math(EXPR keyword_index "${variable_index} + 1")
    list(GET keyword_list ${variable_index} variable)
    list(GET keyword_list ${keyword_index} keyword)
    list(APPEND keyword_code "constexpr char ${variable}[] = \"${keyword}\"\;\n")
  endforeach()
  list(APPEND keyword_code "\n} // namespace keyword\n")
  list(APPEND keyword_code "\n//! \\} Gui\n")
  list(APPEND keyword_code "\n} // namespace nanairo\n")
  list(APPEND keyword_code "\n#endif // NANAIRO_KEYWORD_HPP\n")
  # Write the code to file
  file(APPEND ${keyword_file_path} ${keyword_code})
  source_group(NanairoGui FILES ${keyword_file_path})
endfunction(makeRendererKeywordFile)


#
function(setGuiKeywords)
  set(keyword_list ${ARGN})
  list(LENGTH keyword_list list_size)
  math(EXPR length "${list_size} / 2")
  foreach(number RANGE 1 ${length})
    math(EXPR variable_index "(${number} - 1) * 2")
    math(EXPR keyword_index "${variable_index} + 1")
    list(GET keyword_list ${variable_index} variable)
    list(GET keyword_list ${keyword_index} keyword)
    set(${variable} ${keyword} PARENT_SCOPE)
  endforeach()
endfunction(setGuiKeywords)


# Configure GUI source files
function(getGuiFiles gui_files)
  # QML files
  set(qml_files "")
  file(GLOB_RECURSE qml_source_files ${__nanairo_gui_root__}/Gui/*.qml)
  foreach(qml_source_file IN LISTS qml_source_files)
    if(NOT qml_source_file MATCHES ${__nanairo_gui_root__}/Gui/dummydata/.*\.qml)
      list(APPEND qml_files ${qml_source_file})
    endif()
  endforeach(qml_source_file)
  # Resource files
  file(GLOB_RECURSE resource_files ${__nanairo_gui_root__}/Gui/*.png)

  # Output variables
  set(${gui_files} ${qml_files} ${resource_files} PARENT_SCOPE)
endfunction(getGuiFiles)


# Make Nanairo GUI config file
function(makeGuiConfigFile config_file_path)
  configure_file(${__nanairo_gui_root__}/nanairo_gui_config-inl.hpp.in
                 ${config_file_path})
  source_group(NanairoGui FILES ${config_file_path})
endfunction(makeGuiConfigFile)


# Make Nanairo UI resource file
function(makeGuiResource gui_resource_dir gui_resource_file gui_resources)
  file(MAKE_DIRECTORY ${gui_resource_dir})

  setGuiKeywords(${nanairo_keyword_list})
  getGuiFiles(gui_files)

  # Make a resource file code
  set(resource_file_code "")
  # Header
  list(APPEND resource_file_code "<RCC>\n")
  list(APPEND resource_file_code "  <qresource prefix=\"/NanairoGui\">\n")
  # GUI files
  foreach(gui_file IN LISTS gui_files)
    file(RELATIVE_PATH gui_alias ${__nanairo_gui_root__}/Gui ${gui_file})
    list(APPEND resource_file_code
                "    <file alias=\"${gui_alias}\">${gui_file}</file>\n")
  endforeach(gui_file)
  # Definitions file
  configure_file(${__nanairo_gui_root__}/Gui/definitions.js
                 ${gui_resource_dir}/definitions.js)
  list(APPEND resource_file_code "    <file alias=\"definitions.js\">${gui_resource_dir}/definitions.js</file>\n")
  # Scene files
  set(scene_resource_dir ${PROJECT_SOURCE_DIR}/resources)
  file(GLOB_RECURSE scene_files ${scene_resource_dir}/scene/*.nana)
  foreach(scene_file IN LISTS scene_files)
    file(RELATIVE_PATH scene_alias ${scene_resource_dir} ${scene_file})
    list(APPEND resource_file_code
                "    <file alias=\"${scene_alias}\">${scene_file}</file>\n")
  endforeach(scene_file)
  # Footer
  list(APPEND resource_file_code "  </qresource>\n")
  list(APPEND resource_file_code "</RCC>\n")
  # Write the code to file
  file(WRITE ${gui_resource_dir}/${gui_resource_file} ${resource_file_code})
  source_group(NanairoGui FILES ${gui_resource_dir}/${gui_resource_file})

  # Compile qml files
  qtquick_compiler_add_resources(qt_resources
                                 ${gui_resource_dir}/${gui_resource_file})
  set(${gui_resources} ${qt_resources} PARENT_SCOPE)
endfunction(makeGuiResource)


# Load nanairo gui files
function(getNanairoGui gui_source_files gui_definitions)
  set(gui_config_dir ${PROJECT_BINARY_DIR}/include/NanairoGui)
  set(source_files "")
  # Source files
  findNanairoSourceFiles(${__nanairo_gui_root__} source_files)
  # Keyword
  set(keyword_file_path ${gui_config_dir}/keyword.hpp)
  makeRendererKeywordFile(${keyword_file_path} ${nanairo_keyword_list})
  list(APPEND source_files ${keyword_file_path})
  # Config file
  set(config_file_path ${gui_config_dir}/nanairo_gui_config-inl.hpp)
  makeGuiConfigFile(${config_file_path})
  list(APPEND source_files ${config_file_path})
  # Resource
  set(resource_dir ${PROJECT_BINARY_DIR}/resource_code)
  makeGuiResource(${resource_dir} gui_resource.qrc gui_resources)
  list(APPEND source_files ${gui_resources})


  # Output variables
  set(${gui_source_files} ${source_files} PARENT_SCOPE)
endfunction(getNanairoGui)
