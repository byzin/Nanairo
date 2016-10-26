# file: config.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__nanairo_gui_root__ ${CMAKE_CURRENT_LIST_DIR})


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


#
macro(setGuiProperties)
  # Keywords
  setGuiKeywords(${nanairo_keyword_list})
  # QML versions
  set(qt_quick_version 2.6)
  set(qt_quick_controls_version 1.5)
  set(qt_quick_controls_style_version 1.4)
  set(qt_quick_layouts_version 1.3)
  set(qt_quick_dialogs_version 1.2)
  set(qt_quick_window_version 2.2)
  # Window
  set(main_window_width 960)
  set(main_window_height 540)
  set(default_block_width 160)
  set(default_block_height 64)
  # Tab item
  set(tab_item_width 960)
  set(tab_item_height 494)
  # Setting block
  set(item_offset 4)
  set(block_offset 16)
  set(default_combobox_width 120)
  set(default_combobox_height 16)
  set(default_spinbox_width 120)
  set(default_spinbox_height 20)
  set(default_button_width 50)
  set(default_button_height 16)
  set(default_checkbox_height 16)
  set(default_switch_width 120)
  set(default_switch_height 16)
  set(default_text_field_width 140)
  set(default_text_field_height 20)
  set(default_color_view_width 140)
  set(default_color_view_height 20)
  # System
  math(EXPR integer32_max "(1 << 31) - 1")
  # Color
  set(default_darker_scale 1.2)
  set(default_lighter_scale 0.83)
  # RenderingMethod
  set(rendering_tab_item_width 768)
  set(rendering_tab_item_height 494)
  # BVH
  set(bvh_tab_item_width ${rendering_tab_item_width})
  set(bvh_tab_item_height ${rendering_tab_item_height})
  # Object
  set(object_tree_width 256)
  set(object_tree_item_margin 2)
  math(EXPR object_tree_item_width
            "${object_tree_width} - 2 * ${object_tree_item_margin}")
  set(object_tree_item_height 16)
  set(object_tree_item_button_size 16)
  set(tree_view_margin 8)
  math(EXPR object_tree_view_width
            "${object_tree_width} + 2 * ${tree_view_margin}")
  set(object_tree_view_height ${tab_item_height})
  math(EXPR object_setting_view_width
            "${tab_item_width} - ${object_tree_view_width}")
  set(object_setting_view_height ${tab_item_height})
  set(default_image_button_size 24)
  # Object item
  set(default_indent_width 4)
  set(object_setting_tab_width 688)
  set(object_setting_tab_height 468)
  set(camera_tab_item_width 496)
  set(camera_tab_item_height 468)
  # Transformation
  set(transformation_list_width 444)
  set(transformation_list_margin ${tree_view_margin})
  set(transformation_item_margin 2)
  math(EXPR transformation_item_width
            "${transformation_list_width} - 2 * ${transformation_item_margin}")
  set(transformation_item_height 48)
  set(transformation_max_value 10000.0)
  # Material
  set(material_list_width 160)
  set(material_view_margin 8)
  set(material_list_item_margin 2)
  math(EXPR material_list_item_width
            "${material_list_width} - 2 * ${material_list_item_margin}")
  set(material_list_item_height 16)
  math(EXPR material_list_view_width "${material_list_width} + 2 * ${material_view_margin}")
  set(material_setting_view_width 592)
  set(material_setting_view_height 494)
endmacro(setGuiProperties)


# Configure GUI source files
function(configureGuiQmlFiles output_dir gui_qml_files)
  setGuiProperties()
  set(qml_files "")
  file(GLOB_RECURSE qml_in_files ${__nanairo_gui_root__}/*.qml.in)
  foreach(qml_in_file ${qml_in_files})
    file(RELATIVE_PATH qml_alias ${__nanairo_gui_root__} ${qml_in_file})
    string(REPLACE ".in" "" qml_alias ${qml_alias})
    set(qml_file ${output_dir}/${qml_alias})
    configure_file(${qml_in_file} ${qml_file})
    list(APPEND qml_files ${qml_file})
  endforeach(qml_in_file)


  # Output variables
  set(${gui_qml_files} ${qml_files} PARENT_SCOPE)
endfunction(configureGuiQmlFiles)


#
function(copyGuiResourceFiles output_dir gui_resource_files)
  # GUI elements
  file(GLOB_RECURSE gui_resources ${__nanairo_gui_root__}/*.png
                                  ${__nanairo_gui_root__}/*.js)
  set(resource_files "")
  ## Copy resources to build resource dir
  foreach(resource_in_file ${gui_resources})
    file(RELATIVE_PATH resource_alias ${__nanairo_gui_root__} ${resource_in_file})
    set(resource_file ${output_dir}/${resource_alias})
    configure_file(${resource_in_file} ${resource_file} COPYONLY)
    list(APPEND resource_files ${resource_file})
  endforeach(resource_in_file)
  # Scene files
  set(scene_resource_dir ${PROJECT_SOURCE_DIR}/resources)
  file(GLOB_RECURSE scene_files ${scene_resource_dir}/scene/*.nana)
  foreach(scene_in_file ${scene_files})
    file(RELATIVE_PATH scene_alias ${scene_resource_dir} ${scene_in_file})
    set(scene_file ${output_dir}/${scene_alias})
    configure_file(${scene_in_file} ${scene_file} COPYONLY)
    list(APPEND resource_files ${scene_file})
  endforeach(scene_in_file)

  # Output variable
  set(${gui_resource_files} ${resource_files} PARENT_SCOPE)
endfunction(copyGuiResourceFiles)


# Make Nanairo GUI config file
function(makeGuiConfigFile config_file_path)
  configure_file(${__nanairo_gui_root__}/nanairo_gui_config-inl.hpp.in
                 ${config_file_path})
  source_group(NanairoGui FILES ${config_file_path})
endfunction(makeGuiConfigFile)


# Make Nanairo UI resource file
function(makeGuiResource gui_resource_file)
  set(resource_dir ${PROJECT_BINARY_DIR}/resource_code)
  file(MAKE_DIRECTORY ${resource_dir})

  configureGuiQmlFiles(${resource_dir} qml_files)
  copyGuiResourceFiles(${resource_dir} resource_files)

  # Make a resource file code
  set(resource_file_code "")
  # Header
  list(APPEND resource_file_code "<RCC>\n")
  list(APPEND resource_file_code "  <qresource prefix=\"/NanairoGui\">\n")
  # Resources
  foreach(gui_file ${qml_files} ${resource_files})
    file(RELATIVE_PATH resource_alias ${resource_dir} ${gui_file})
    list(APPEND resource_file_code
                "    <file alias=\"${resource_alias}\">${gui_file}</file>\n")
  endforeach(gui_file)
  # Footer
  list(APPEND resource_file_code "  </qresource>\n")
  list(APPEND resource_file_code "</RCC>\n")
  # Write the code to file
  file(WRITE ${gui_resource_file} ${resource_file_code})
  source_group(NanairoGui FILES ${gui_resource_file})
endfunction(makeGuiResource)


# Load nanairo gui files
function(getNanairoGui gui_source_files gui_definitions)
  # Source files
  findNanairoSourceFiles(${__nanairo_gui_root__} source_files)
  # Config file
  set(config_file_path
      ${PROJECT_BINARY_DIR}/include/NanairoGui/nanairo_gui_config-inl.hpp)
  makeGuiConfigFile(${config_file_path})
  list(APPEND source_files ${config_file_path})
  # Resource
  set(resource_file ${PROJECT_BINARY_DIR}/resource_code/gui_resource.qrc)
  makeGuiResource(${resource_file})
  list(APPEND source_files ${resource_file})


  # Output variables
  set(${gui_source_files} ${source_files} PARENT_SCOPE)
endfunction(getNanairoGui)
