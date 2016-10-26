# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

set(__nanairo_common_root__ ${CMAKE_CURRENT_LIST_DIR})


#
function(makeCommonKeywordFile keyword_file_path)
  configure_file(${__nanairo_common_root__}/keyword.hpp.in
                 ${keyword_file_path})

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
  list(APPEND keyword_code "\n//! \\} Common\n")
  list(APPEND keyword_code "\n} // namespace nanairo\n")
  list(APPEND keyword_code "\n#endif // NANAIRO_KEYWORD_HPP\n")
  # Write the code to file
  file(APPEND ${keyword_file_path} ${keyword_code})
  source_group(NanairoCommon FILES ${keyword_file_path})
endfunction(makeCommonKeywordFile)


#
function(makeCommonConfigFile config_file_path)
  configure_file(${__nanairo_common_root__}/nanairo_common_config-inl.hpp.in
                 ${config_file_path})
  source_group(NanairoCommon FILES ${config_file_path})
endfunction(makeCommonConfigFile)

# Load nanairo common files
function(getNanairoCommon common_source_files)
  set(common_config_dir ${PROJECT_BINARY_DIR}/include/NanairoCommon)
  set(source_files "")
  # keyword.hpp
  set(keyword_file_path ${common_config_dir}/keyword.hpp)
  makeCommonKeywordFile(${keyword_file_path} ${nanairo_keyword_list})
  list(APPEND source_files ${keyword_file_path})
  # nanairo_common_config.hpp
  set(config_file_path ${common_config_dir}/nanairo_common_config-inl.hpp)
  makeCommonConfigFile(${config_file_path})
  list(APPEND source_files ${config_file_path})


  # Output variable
  set(${common_source_files} ${source_files} PARENT_SCOPE)
endfunction(getNanairoCommon)
