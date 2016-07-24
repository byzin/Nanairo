# file: dependencies.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

#
function(deployUnresolvedDependencies target_path dest_dir)
  include(GetPrerequisites)
  get_prerequisites(${target_path} dependencies 1 1 "" "")
  foreach(dependency ${dependencies})
    gp_resolve_item("${target_path}" "${dependency}" "" "" resolved_file_path)
    get_filename_component(dependency_name ${resolved_file_path} NAME)
    find_file(${dependency_name}_result ${dependency_name}
                  PATHS ${dest_dir}
                  NO_DEFAULT_PATH)
    if(NOT ${dependency_name}_result)
      message("Updating ${dependency_name}.")
      get_filename_component(dependency_path ${resolved_file_path} REALPATH)
      set(dest_file_path ${dest_dir}/${dependency_name})
      configure_file(${dependency_path} ${dest_file_path} COPYONLY)
    endif()
  endforeach(dependency)
endfunction(deployUnresolvedDependencies)
