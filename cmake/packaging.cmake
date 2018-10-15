# file: packaging.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


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
  set(CPACK_PACKAGE_CHECKSUM SHA3_256)
  # CPACK_PROJECT_CONFIG_FILE is not set
  set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
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
  else()
    set(CPACK_GENERATOR "TXZ")
  endif()
endmacro(setNanairoPackProperties)


#
function(setNanairoPackInfoForLinux)
  configure_file(${PROJECT_SOURCE_DIR}/packaging/linux_exec_script.sh.in
                 ${PROJECT_BINARY_DIR}/packaging/${PROJECT_NAME}
                 @ONLY)
  configure_file(${PROJECT_SOURCE_DIR}/packaging/linux_install_script.cmake.in
                 ${PROJECT_BINARY_DIR}/packaging/install_script.cmake
                 @ONLY)
  install(SCRIPT ${PROJECT_BINARY_DIR}/packaging/install_script.cmake)
endfunction(setNanairoPackInfoForLinux)


#
function(setNanairoPackInfoForMac)
  # Properties
  set_target_properties(${PROJECT_NAME} PROPERTIES
      MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
      MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
      MACOSX_BUNDLE_COPYRIGHT "© 2018 Sho Ikeda"
      MACOSX_BUNDLE_GUI_IDENTIFIER "com.ShoIkeda.${PROJECT_NAME}"
      MACOSX_BUNDLE_ICON_FILE "${PROJECT_NAME}.icns"
      MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} ${PROJECT_VERSION}"
      MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")

  # Execute install script
  set(body_exec_name "NanairoRender")
  configure_file(${PROJECT_SOURCE_DIR}/packaging/mac_exec_script.sh.in
                 ${PROJECT_BINARY_DIR}/packaging/${PROJECT_NAME}
                 @ONLY)
  configure_file(${PROJECT_SOURCE_DIR}/packaging/mac_install_script.cmake.in
                 ${PROJECT_BINARY_DIR}/packaging/install_script.cmake
                 @ONLY)
  install(SCRIPT ${PROJECT_BINARY_DIR}/packaging/install_script.cmake
          COMPONENT ${PROJECT_NAME})
endfunction(setNanairoPackInfoForMac)


#
function(setNanairoPackInfoForWindows)
  # Execute install script
  configure_file(${PROJECT_SOURCE_DIR}/packaging/windows_exec_script.cmd.in
                 ${PROJECT_BINARY_DIR}/packaging/${PROJECT_NAME}.cmd
                 @ONLY)
  configure_file(${PROJECT_SOURCE_DIR}/packaging/windows_install_script.cmake.in
                 ${PROJECT_BINARY_DIR}/packaging/install_script.cmake
                 @ONLY)
  install(SCRIPT ${PROJECT_BINARY_DIR}/packaging/install_script.cmake
          COMPONENT ${PROJECT_NAME})
endfunction(setNanairoPackInfoForWindows)


#
function(setCommonNanairoPackInfo)
  install(TARGETS ${PROJECT_NAME}
          DESTINATION .
          COMPONENT ${PROJECT_NAME})
  install(DIRECTORY ${PROJECT_BINARY_DIR}/resources
          DESTINATION .
          COMPONENT ${PROJECT_NAME})
endfunction(setCommonNanairoPackInfo)


## Set the packaging information
function(setupPackInfo)
  # Initialize cpack
  set(CPACK_BUNDLE_NAME "${CMAKE_PROJECT_NAME}")
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
