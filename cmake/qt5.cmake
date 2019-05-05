# file: qt5.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015-2019 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


# Defined variables
#   qt5_include_dirs:
#   qt5_libraries:
#   qt5_compile_flags:
#   qt5_definitions:
macro(findQt5 __qt_version__)
  set(__include_dirs__ "")
  set(__libraries__ "")
  set(__compile_flags__ "")
  set(__definitions__ "")

  set(__module_list__ ${ARGN})
  find_package(Qt5 ${__qt_version__} REQUIRED ${__module_list__})
  find_package(Qt5QuickCompiler REQUIRED)
  message(STATUS "Qt5 version: ${Qt5_VERSION}")
  foreach(module ${__module_list__})
    list(APPEND __include_dirs__ ${Qt5${module}_INCLUDE_DIRS})
    list(APPEND __libraries__ ${Qt5${module}_LIBRARIES})
    list(APPEND __compile_flags__ ${Qt5${module}_EXECUTABLE_COMPILE_FLAGS})
    list(APPEND __definitions__ ${Qt5${module}_COMPILE_DEFINITIONS})
  endforeach(module)


  # Output variables
  set(qt5_include_dirs ${__include_dirs__})
  set(qt5_libraries ${__libraries__})
  set(qt5_compile_flags ${__compile_flags__})
  set(qt5_definitions ${__definitions__})
endmacro(findQt5)
