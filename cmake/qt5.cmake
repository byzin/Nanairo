# file: qt5.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0.0)


# Functions and macros

# Add Qt resource file
function(addQtResources output_variable resource_path)
  set(resource_dir ${PROJECT_BINARY_DIR}/resource)
  file(MAKE_DIRECTORY ${resource_dir})
  set(current_binary_dir ${CMAKE_CURRENT_BINARY_DIR})
  set(CMAKE_CURRENT_BINARY_DIR ${resource_dir})

  qt5_add_resources(resource_files ${resource_path})
  source_group(NanairoResource FILES ${resource_files})
  set(${output_variable} ${resource_files} PARENT_SCOPE)

  set(CMAKE_CURRENT_BINARY_DIR ${current_binary_dir})
endfunction(addQtResources)

# Defined variables
#   qt5_definitions:
#   qt5_include_dirs: 
#   qt5_libraries: 
#   qt5_root_dir: 
macro(findQt5)
  set(__qt5_version__ 5.4)

  # QtCore
  find_package(Qt5Core ${__qt5_version__} REQUIRED)
  set(qt5_definitions ${qt5_definitions} ${Qt5Core_DEFINITIONS})
  set(qt5_include_dirs ${qt5_include_dirs} ${Qt5Core_INCLUDE_DIRS})
  set(qt5_libraries ${qt5_libraries} ${Qt5Core_LIBRARIES})

  # QtGui
  find_package(Qt5Gui ${__qt5_version__} REQUIRED)
  set(qt5_definitions ${qt5_definitions} ${Qt5Gui_DEFINITIONS})
  set(qt5_include_dirs ${qt5_include_dirs} ${Qt5Gui_INCLUDE_DIRS})
  set(qt5_libraries ${qt5_libraries} ${Qt5Gui_LIBRARIES})

  # QtWidgets
  #  find_package(Qt5Widgets ${__qt5_version__} REQUIRED)
  #  set(qt5_definitions ${qt5_definitions} ${Qt5Widgets_DEFINITIONS})
  #  set(qt5_include_dirs ${qt5_include_dirs} ${Qt5Widgets_INCLUDE_DIRS})
  #  set(qt5_libraries ${qt5_libraries} ${Qt5Widgets_LIBRARIES})

  # QtQml
  find_package(Qt5Qml ${__qt5_version__} REQUIRED)
  set(qt5_definitions ${qt5_definitions} ${QtQml_DEFINITIONS})
  set(qt5_include_dirs ${qt5_include_dirs} ${Qt5Qml_INCLUDE_DIRS})
  set(qt5_libraries ${qt5_libraries} ${Qt5Qml_LIBRARIES})

  # QtQuick
  find_package(Qt5Quick ${__qt5_version__} REQUIRED)
  set(qt5_definitions ${qt5_definitions} ${QtQuick_DEFINITIONS})
  set(qt5_include_dirs ${qt5_include_dirs} ${Qt5Quick_INCLUDE_DIRS})
  set(qt5_libraries ${qt5_libraries} ${Qt5Quick_LIBRARIES})

  # Set other variables
  get_target_property(__qt5_core_location__ ${Qt5Core_LIBRARIES} LOCATION)
  get_filename_component(__qt5_core_dir__ ${__qt5_core_location__} DIRECTORY)
  get_filename_component(__qt5_lib_dir__ ${__qt5_core_dir__} DIRECTORY)
  get_filename_component(qt5_root_dir ${__qt5_lib_dir__} DIRECTORY)
endmacro(findQt5)
