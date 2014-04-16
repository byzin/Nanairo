# file: packaging.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.0)


# Inner function s and macros

macro(getPackageTempDir generator_name dir_name)
  set(${dir_name} ${PROJECT_BINARY_DIR}/_CPack_Packages/${CPACK_SYSTEM_NAME}/${generator_name}/${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME})
endmacro(getPackageTempDir)

macro(setupPackagingInfoForWindows)
  # Setup installation directories
  install(TARGETS ${PROJECT_NAME}
          DESTINATION bin
          COMPONENT ${PROJECT_NAME})
  install(PROGRAMS ${PROJECT_SOURCE_DIR}/tools/windows/${PROJECT_NAME}.bat
          DESTINATION .
          COMPONENT ${PROJECT_NAME})
  install(DIRECTORY resources
          DESTINATION .
          COMPONENT ${PROJECT_NAME})

  # Execute scripts
  getPackageTempDir(ZIP package_temp_dir)
  set(installation_script 
      "execute_process(COMMAND windeployqt --release ${PROJECT_NAME}.exe
                       WORKING_DIRECTORY ${package_temp_dir}/bin)")
  install(CODE "${installation_script}")
endmacro(setupPackagingInfoForWindows)

macro(setupPackagingInfoForLinux)
  # Install files
  install(TARGETS ${PROJECT_NAME}
          DESTINATION bin
          COMPONENT ${PROJECT_NAME})
  install(PROGRAMS ${PROJECT_SOURCE_DIR}/tools/nanairo_script_linux.sh
          DESTINATION .
          COMPONENT ${PROJECT_NAME}
          RENAME ${PROJECT_NAME})
endmacro(setupPackagingInfoForLinux)


# Functions and macros

## Set the packaging information
function(setupPackagingInfo)
  # Initialize cpack
  include(InstallRequiredSystemLibraries)
  include(CPack)
  # Set version
  set(CPACK_PACKAGE_VERSION_MAJOR ${nanairo_version_major})
  set(CPACK_PACKAGE_VERSION_MINOR ${nanairo_version_minor})
  set(CPACK_PACKAGE_VERSION_PATCH ${nanairo_version_patch})
  
  set(CPACK_PACKAGE_VENDOR "Zin")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY 
      "Nanairo is a physically based spectral renderer.")
  set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/MIT_LICENSE.txt)

  if (z_windows)
    setupPackagingInfoForWindows()
  elseif (z_linux)
    setupPackagingInfoForLinux()
  endif()
endfunction(setupPackagingInfo)
