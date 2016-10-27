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
  set(option_description "Clang uses libc++ instead of libstdc++.")
  setBooleanOption(Z_CLANG_USES_LIBCXX OFF ${option_description})

  set(option_description "Enable C++ sanitizer (Address).")
  setBooleanOption(Z_ENABLE_SANITIZER OFF ${option_description})
endfunction(setCompilerOption)


# Get compile options
function(getCompilerOption cxx_compile_flags cxx_linker_flags cxx_definitions)
  set(compile_flags "")
  set(linker_flags "")
  set(definitions "")
  # libc++
  if(Z_CLANG_USES_LIBCXX)
    if(Z_IS_CLANG)
      list(APPEND compile_flags -stdlib=libc++)
      list(APPEND linker_flags -stdlib=libc++)
    else()
      message(WARNING "The compiler doesn't support libc++.")
    endif()
  endif()
  # Sanitizer
  if(Z_ENABLE_SANITIZER)
    if(Z_IS_CLANG OR Z_IS_GCC)
      list(APPEND compile_flags -fsanitize=address -fno-omit-frame-pointer)
      list(APPEND linker_flags -fsanitize=address)
    else()
      message(WARNING "The compiler doesn't support sanitizer.")
    endif()
  endif()

  # Output variables
  set(${cxx_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${cxx_linker_flags} ${linker_flags} PARENT_SCOPE)
endfunction(getCompilerOption)


#
function(getCxxWarningOption compiler_warning_flags)
  set(compiler_version ${CMAKE_CXX_COMPILER_VERSION})
  set(environment "${CMAKE_SYSTEM_NAME} ${CMAKE_CXX_COMPILER_ID} ${compiler_version}")

  # Clang
  if(Z_IS_VISUAL_STUDIO AND Z_IS_CLANG)
    set(warning_flags /W4
                      -Wno-microsoft-enum-value
                      -Qunused-arguments
                      )
  elseif(Z_IS_CLANG)
    set(warning_flags -Werror
                      -Weverything
                      -Wno-c++98-compat
                      -Wno-c++98-compat-pedantic
                      )
  # GCC
  elseif(Z_IS_GCC)
    set(warning_flags -pedantic
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
  else()
    message(WARNING "${environment}: Warning option isn't supported.")
  endif()
  set(${compiler_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getCxxWarningOption)
