# file: document.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.4)


# Functions and macros 

# Build documents
function(buildDocuments)
  find_package(Doxygen REQUIRED)
  if(DOXYGEN_DOT_FOUND)
    set(doxygen_has_dot "YES")
  else()
    set(doxygen_has_dot "NO")
  endif()
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/documents)
  configure_file(${PROJECT_SOURCE_DIR}/documents/Doxyfile.in
                 ${PROJECT_BINARY_DIR}/documents/Doxyfile)
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/documents/reference)
  set(doxygen_html_index_file 
      ${PROJECT_BINARY_DIR}/documents/reference/html/index.html)
  add_custom_command(OUTPUT ${doxygen_html_index_file}
                     COMMAND ${DOXYGEN_EXECUTABLE} 
                     ${PROJECT_BINARY_DIR}/documents/Doxyfile
                     MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/documents/Doxyfile.in
                     WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/documents)
  add_custom_target(Document ALL DEPENDS ${doxygen_html_index_file})
endfunction(buildDocuments)
