# file: packaging.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

#
macro(setNanairoPackPropertiesForMac)
  set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
  set(CPACK_BUNDLE_PLIST ${PROJECT_BINARY_DIR}/packaging/${PROJECT_NAME}.plist)
  set(CPACK_BUNDLE_ICON ${PROJECT_BINARY_DIR}/packaging/${PROJECT_NAME}.icns)
  set(CPACK_BUNDLE_STARTUP_COMMAND ${PROJECT_BINARY_DIR}/packaging/exec_script.sh)
endmacro(setNanairoPackPropertiesForMac)

#
macro(setNanairoPackProperties)
  # CPACK_PACKAGE_NAME uses default value
  set(CPACK_PACKAGE_VENDOR "Sho Ikeda") 
  # CPACK_PACKAGE_DIRECTORY uses default value
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  # CPACK_PACKAGE_DESCRIPTION_FILE is not set
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY 
      "Nanairo is a physically plausible spectral renderer")
  # CPACK_PACKAGE_FILE_NAME uses default value
  # CPACK_PAKCAGE_INSTALL_DIRECTORY is not set
  # CPACK_PACKAGE_ICON is not set
  # CPACK_PROJECT_CONFIG_FILE is not set
  set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/MIT-LICENSE.txt)
  # CPACK_RESOURCE_FILE_README is not set
  # CPACK_RESOURCE_FILE_WELCOME is not set
  # CPACK_MONOLITHIC_INSTALL is not set
  # CPACK_OUTPUT_CONFIG_FILE is not set
  # CPACK_PACKAGE_EXECUTABLES is not set
  # CPACK_STRIP_FILES is not set
  # CPACK_VERBATIM_VARIABLES is not set
  # CPACK_CMAKE_GENERATOR uses default value
  # CPACK_INSTALL_CMAKE_PROJECTS is not set
  # CPACK_SYSTEM_NAME uses default value
  # CPACK_PACKAGE_VERSION uses default value
  # CPACK_TOPLEVEL_TAG is not set
  # CPACK_INSTALL_COMMANDS is not set
  # CPACK_INSTALLED_DIRECTORIES is not set
  # CPACK_PACKAGE_INSTALL_REGISTRY_KEY is not set
  # CPACK_CREATE_DESKTOP_LINKS is not set

  if(Z_WINDOWS)
    set(CPACK_GENERATOR "ZIP")
  elseif(Z_MAC)
    set(CPACK_GENERATOR "Bundle")
    setNanairoPackPropertiesForMac()
  else()
    set(CPACK_GENERATOR "TGZ")
  endif()
endmacro(setNanairoPackProperties)


#
function(setNanairoPackInfoForLinux)
  message(WARNING "Packaging isn't implemented.")
#  configure_file(${PROJECT_SOURCE_DIR}/packaging/linux_install_script.cmake.in
#                 ${PROJECT_BINARY_DIR}/packaging/install_script.cmake
#                 @ONLY)
#  install(SCRIPT ${PROJECT_BINARY_DIR}/packaging/install_script.cmake)
endfunction(setNanairoPackInfoForLinux)


#
function(setNanairoPackInfoForMac)
  message(WARNING "Packaging function isn't implemented.")
  configure_file(${PROJECT_SOURCE_DIR}/packaging/mac_plist.in
                 ${PROJECT_BINARY_DIR}/packaging/${PROJECT_NAME}.plist
                 @ONLY)
  configure_file(${PROJECT_SOURCE_DIR}/packaging/icon.icns
                 ${PROJECT_BINARY_DIR}/packaging/${PROJECT_NAME}.icns
                 COPYONLY)
  configure_file(${PROJECT_SOURCE_DIR}/packaging/mac_exec_script.sh.in
                 ${PROJECT_BINARY_DIR}/packaging/exec_script.sh
                 @ONLY)
  configure_file(${PROJECT_SOURCE_DIR}/packaging/mac_install_script.cmake.in
                 ${PROJECT_BINARY_DIR}/packaging/install_script.cmake
                 @ONLY)
  install(SCRIPT ${PROJECT_BINARY_DIR}/packaging/install_script.cmake)
endfunction(setNanairoPackInfoForMac)


#
function(setNanairoPackInfoForWindows)
  configure_file(${PROJECT_SOURCE_DIR}/packaging/windows_exec_script.cmd.in
                 ${PROJECT_BINARY_DIR}/packaging/exec_script.cmd
                 @ONLY)
  install(PROGRAMS ${PROJECT_BINARY_DIR}/packaging/exec_script.cmd
          DESTINATION .
          RENAME ${PROJECT_NAME}.cmd
          COMPONENT ${PROJECT_NAME})
  configure_file(${PROJECT_SOURCE_DIR}/packaging/windows_install_script.cmake.in
                 ${PROJECT_BINARY_DIR}/packaging/install_script.cmake
                 @ONLY)
  install(SCRIPT ${PROJECT_BINARY_DIR}/packaging/install_script.cmake)
endfunction(setNanairoPackInfoForWindows)


#
function(setCommonNanairoPackInfo)
  install(TARGETS ${PROJECT_NAME}
          DESTINATION bin
          COMPONENT ${PROJECT_NAME})
  install(DIRECTORY resources
          DESTINATION .
          COMPONENT ${PROJECT_NAME})
endfunction(setCommonNanairoPackInfo)


## Set the packaging information
function(setupPackInfo)
  # Initialize cpack
  set(CPACK_BUNDLE_NAME "Test")
  include(InstallRequiredSystemLibraries)
  setNanairoPackProperties()
  include(CPack)

  setCommonNanairoPackInfo()
  if(Z_LINUX)
    setNanairoPackInfoForLinux()
  elseif(Z_MAC)
    setNanairoPackInfoForMac()
  elseif(Z_WINDOWS)
    setNanairoPackInfoForWindows()
  else()
    message(FATAL_ERROR "Packaging isn't supported: ${CMAKE_SYSTEM_NAME}")
  endif()
endfunction(setupPackInfo)
