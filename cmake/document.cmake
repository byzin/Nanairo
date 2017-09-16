# file: document.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2017 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

# Functions and macros 


# Build documents
function(buildDocuments)
  message(STATUS "Configure for building Nanario documentation with Doxygen.")
  find_package(Doxygen REQUIRED)
  if(DOXYGEN_DOT_FOUND)
    set(doxygen_has_dot "YES")
  else()
    set(doxygen_has_dot "NO")
    message(WARNING "DOT not found. Execute Doxygen without DOT.")
  endif()
  set(doxygen_dir ${PROJECT_BINARY_DIR}/documents)
  set(doxygen_output_dir ${doxygen_dir}/reference)
  file(MAKE_DIRECTORY ${doxygen_output_dir})
  configure_file(${PROJECT_SOURCE_DIR}/documents/Doxyfile.in
                 ${doxygen_dir}/Doxyfile
                 @ONLY)
  add_custom_target(doc
                    COMMAND ${DOXYGEN_EXECUTABLE} ${doxygen_dir}/Doxyfile
                    WORKING_DIRECTORY ${doxygen_output_dir}
                    COMMENT "Generating Nanairo documentation with Doxygen"
                    VERBATIM)
endfunction(buildDocuments)
