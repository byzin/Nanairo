# file: dependencies.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


#
function(deployUnresolvedDependencies target_path dest_dir exepath rpath)
  include(GetPrerequisites)
  get_prerequisites(${target_path} dependencies 1 1 "${exepath}" "" "${rpath}")
  foreach(dependency ${dependencies})
    gp_resolve_item("${target_path}" "${dependency}" "" "" resolved_file_path)

    # Skip the dependencies of rpath and executable_path
    if(resolved_file_path MATCHES .*rpath.* OR
       resolved_file_path MATCHES .*executable_path.*)
      continue()
    endif()

    # Skip the dependencies already deployed
    get_filename_component(dependency_name ${resolved_file_path} NAME)
    find_file(${dependency_name}_result ${dependency_name}
                  PATHS ${dest_dir}
                  NO_DEFAULT_PATH)
    if(${dependency_name}_result)
      continue()
    endif()

    message("Updating ${dependency_name}.")
    get_filename_component(dependency_path ${resolved_file_path} REALPATH)
    set(dest_file_path ${dest_dir}/${dependency_name})
    configure_file(${dependency_path} ${dest_file_path} COPYONLY)
  endforeach(dependency)
endfunction(deployUnresolvedDependencies)
