# file: dependencies.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


# Return a dependency list of a taret
function(getDependencies target_path exepath rpath dependencies)
  include(GetPrerequisites)
  set(deps "")
  get_prerequisites(${target_path} dep_list 0 1 "${exepath}" "" "${rpath}")
  foreach(dependency ${dep_list})
    gp_append_unique(deps ${dependency})
  endforeach(dependency)


  # Output variables
  set(${dependencies} ${deps} PARENT_SCOPE)
endfunction(getDependencies)

#
function(deployUnresolvedDependencies target_path dest_dir exepath rpath)
  include(GetPrerequisites)
  getDependencies(${target_path} "${exepath}" "${rpath}" dependencies)
  foreach(dependency ${dependencies})
    gp_resolved_file_type(${target_path} ${dependency} "${exepath}" "" type "${rpath}")
    if(type MATCHES "(embedded|system)")
      continue()
    endif()

    # Skip Qt files
    gp_resolve_item("${target_path}" "${dependency}" "${exepath}" "" file_path "${rpath}")
    # Skip the dependencies of rpath and executable_path
    if(file_path MATCHES ".*(GL|Qt|rpath|executable_path).*")
      continue()
    endif()

    # Skip if the dependency already deployed
    get_filename_component(dependency_name ${dependency} NAME)
    find_file(${dependency_name}_result ${dependency_name}
                  PATHS ${dest_dir}
                  NO_DEFAULT_PATH)
    if(${dependency_name}_result)
      continue()
    endif()

    message("Updating ${dependency_name}")
    get_filename_component(dependency_path ${file_path} REALPATH)
    set(dest_file_path ${dest_dir}/${dependency_name})
    configure_file(${dependency_path} ${dest_file_path} COPYONLY)
  endforeach(dependency)
endfunction(deployUnresolvedDependencies)

# 
function(saveDependencyList target_path exepath rpath output_dir)
  include(GetPrerequisites)
  getDependencies(${target_path} "${exepath}" "${rpath}" dependencies)

  set(text "")
  foreach(dependency ${dependencies})
    gp_resolved_file_type(${target_path} ${dependency} "${exepath}" "" type "${rpath}")
    gp_resolve_item("${target_path}" "${dependency}" "${exepath}" "" file_path "${rpath}")
    string(APPEND text "${type}: ${file_path}\n")
  endforeach(dependency)

  get_filename_component(target_name ${target_path} NAME)
  file(WRITE ${output_dir}/${target_name}_dependencies.txt ${text})
endfunction(saveDependencyList)
