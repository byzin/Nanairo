# file: compiler.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


# Check compiler features
function(checkCompilerHasCxx14Features target)
  set(compiler_feature_list
    cxx_aggregate_default_initializers
    cxx_alias_templates
    cxx_alignas
    cxx_alignof
    cxx_attributes
    cxx_attribute_deprecated
    cxx_auto_type
    cxx_binary_literals
    cxx_constexpr
    cxx_contextual_conversions
#    cxx_decltype_incomplete_return_types
    cxx_decltype
    cxx_decltype_auto
    cxx_default_function_template_args
    cxx_defaulted_functions
    cxx_defaulted_move_initializers
    cxx_delegating_constructors
    cxx_deleted_functions
    cxx_digit_separators
    cxx_enum_forward_declarations
    cxx_explicit_conversions
    cxx_extended_friend_declarations
    cxx_extern_templates
    cxx_final
    cxx_func_identifier
    cxx_generalized_initializers
    cxx_generic_lambdas
    cxx_inheriting_constructors
    cxx_inline_namespaces
    cxx_lambdas
    cxx_lambda_init_captures
    cxx_local_type_template_args
    cxx_long_long_type
    cxx_noexcept
    cxx_nonstatic_member_init
    cxx_nullptr
    cxx_override
    cxx_range_for
    cxx_raw_string_literals
    cxx_reference_qualified_functions
    cxx_relaxed_constexpr
    cxx_return_type_deduction
    cxx_right_angle_brackets
    cxx_rvalue_references
    cxx_sizeof_member
    cxx_static_assert
    cxx_strong_enums
#    cxx_thread_local
    cxx_trailing_return_types
    cxx_unicode_literals
    cxx_uniform_initialization
    cxx_unrestricted_unions
    cxx_user_literals
    cxx_variable_templates
    cxx_variadic_macros
    cxx_variadic_templates
    cxx_template_template_parameters)
  foreach(feature IN LISTS compiler_feature_list)
    target_compile_features(${target} PRIVATE ${feature})
  endforeach()
endfunction(checkCompilerHasCxx14Features)


#
function(setCompilerOption)
  set(option_description "Clang uses libc++ instead of libstdc++ (if compiler supports).")
  setBooleanOption(Z_CLANG_USES_LIBCXX OFF ${option_description})

  set(option_description "Enable C++ sanitizer (Address) (if compiler supports).")
  setBooleanOption(Z_ENABLE_SANITIZER OFF ${option_description})
endfunction(setCompilerOption)


function(getClangCompilerOption cxx_compile_flags cxx_linker_flags cxx_definitions)
  set(compile_flags "")
  set(linker_flags "")
  set(definitions "")
  list(APPEND compile_flags -fconstexpr-steps=${max_constexpr_step})
  if(Z_CLANG_USES_LIBCXX)
    list(APPEND compile_flags -stdlib=libc++)
    list(APPEND linker_flags -stdlib=libc++)
  endif()
  if(Z_ENABLE_SANITIZER)
    list(APPEND compile_flags -fsanitize=address -fno-omit-frame-pointer)
    list(APPEND linker_flags -fsanitize=address)
  endif()
  # Output variables
  set(${cxx_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${cxx_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${cxx_definitions} ${definitions} PARENT_SCOPE)
endfunction(getClangCompilerOption)


function(getGccCompilerOption cxx_compile_flags cxx_linker_flags cxx_definitions)
  set(compile_flags "")
  set(linker_flags "")
  set(definitions "")
  if(Z_ENABLE_SANITIZER)
    list(APPEND compile_flags -fsanitize=address -fno-omit-frame-pointer)
    list(APPEND linker_flags -fsanitize=address)
  endif()
  # Output variables
  set(${cxx_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${cxx_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${cxx_definitions} ${definitions} PARENT_SCOPE)
endfunction(getGccCompilerOption)


# Get compile options
function(getCompilerOption cxx_compile_flags cxx_linker_flags cxx_definitions)
  math(EXPR max_constexpr_step "1 << 24")
  # Options
  if (Z_CLANG)
    getClangCompilerOption(compile_flags linker_flags definitions)
  elseif(Z_GCC)
    getGccCompilerOption(compile_flags linker_flags definitions)
  endif()
  # Output variables
  set(${cxx_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${cxx_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${cxx_definitions} ${definitions} PARENT_SCOPE)
endfunction(getCompilerOption)


function(getClangWarningOption compiler_warning_flags)
  set(warning_flags "")
  if(Z_VISUAL_STUDIO)
    list(APPEND warning_flags /W4
                              -Wno-microsoft-enum-value
                              -Qunused-arguments
                              )
  else()
    list(APPEND warning_flags -Werror
                              -Weverything
                              -Wno-c++98-compat
                              -Wno-c++98-compat-pedantic
                              )
  endif()
  # Output variables
  set(${compiler_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getClangWarningOption)

function(getGccWarningOption compiler_warning_flags)
  set(warning_flags "")
  list(APPEND warning_flags -pedantic
                            -Wall
                            -Wextra
                            -Wcast-align
                            -Wcast-qual
                            -Wctor-dtor-privacy
                            -Wdisabled-optimization
                            -Wformat=2
                            -Winit-self
                            -Wlogical-op
                            -Wmissing-declarations
                            -Wmissing-include-dirs
                            -Wnoexcept -Wold-style-cast
                            -Woverloaded-virtual
                            -Wredundant-decls
                            -Wshadow
                            -Wsign-conversion
                            -Wsign-promo
                            -Wstrict-null-sentinel
                            -Wstrict-overflow=5
                            -Wswitch-default
                            -Wundef
                            -Werror
                            -Wno-unused
                            )
  # Output variables
  set(${compiler_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getGccWarningOption)

#
function(getCxxWarningOption compiler_warning_flags)
  set(compiler_version ${CMAKE_CXX_COMPILER_VERSION})
  set(environment "${CMAKE_SYSTEM_NAME} ${CMAKE_CXX_COMPILER_ID} ${compiler_version}")

  # Clang
  if(Z_CLANG)
    getClangWarningOption(warning_flags)
  # GCC
  elseif(Z_GCC)
    getGccWarningOption(warning_flags)
  else()
    message(WARNING "${environment}: Warning option isn't supported.")
  endif()
  set(${compiler_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getCxxWarningOption)
