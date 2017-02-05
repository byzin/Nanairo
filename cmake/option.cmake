# file: option.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


# Validate options
function(validateOptions)
  # Validate options
  set(lambda_min ${NANAIRO_SHORTEST_WAVELENGTH})
  set(lambda_max ${NANAIRO_LONGEST_WAVELENGTH})

  if(${lambda_min} LESS 360)
    message(FATAL_ERROR "The shortest wavelength isn't greater than equal to 360nm.")
  endif()
  if(830 LESS ${lambda_max})
    message(FATAL_ERROR "The longest wavelength isn't less than equal to 830nm.")
  endif()
  if(NOT (${lambda_min} LESS ${lambda_max}))
    message(FATAL_ERROR "The shortest wavelength isn't less than to the longest.")
  endif()

  set(delta_lambda ${NANAIRO_WAVELENGTH_RESOLUTION})
  if(NOT 0 LESS ${delta_lambda})
    message(FATAL_ERROR "Wavelength resolution isn't positive number.")
  endif()
  math(EXPR range "${lambda_max} - ${lambda_min}")
  math(EXPR rest "${range} % ${delta_lambda}")
  if(NOT ${rest} EQUAL 0)
    message(FATAL_ERROR "Invalied wavelength resolution is specified.")
  endif()

  math(EXPR spectra_size "${range} / ${delta_lambda}")
  set(sample_size ${NANAIRO_WAVELENGTH_SAMPLE_SIZE})
  # !(0 < sampleSize <= spectraSize)
  if(NOT (0 LESS ${sample_size} AND (${sample_size} LESS ${spectra_size} OR
                                     ${sample_size} EQUAL ${spectra_size})))
    message(FATAL_ERROR "Invalid wavelength sample size is specified.")
  endif()
endfunction(validateOptions)


# Set command options
function(setCommandOptions)
  # Utility options
  set(option_description "Build documents using Doxygen")
  setBooleanOption(NANAIRO_BUILD_DOCUMENTS OFF ${option_description})

  set(option_description "Build extra tools")
  setBooleanOption(NANAIRO_BUILD_EXTRA_TOOLS OFF ${option_description})

  # Rendering options
  set(option_description "Set the floating point type of the computation in rendering.")
  setStringOption(NANAIRO_FLOATING_POINT_TYPE "double" ${option_description})
 
  set(option_description "Set the max number of objects that a BVH node can contain.")
  setStringOption(NANAIRO_MAX_NUM_OF_OBJECTS 8 ${option_description})

  set(option_description "Set max FPS")
  setStringOption(NANAIRO_MAX_FPS 50 ${option_description})

  set(option_description "Select 64bit random number generator from \"Xorshift128+\", \"SplitMix64\", \"PCG\", \"Xoroshiro128+\"")
  setStringOption(NANAIRO_SAMPLER "Xoroshiro128+" ${option_description})

  set(option_description "The hash key which is used as the default random seed")
  setStringOption(NANAIRO_RANDOM_SEED_KEY "NanairoRenderer" ${option_description})

  set(option_description "Build unit tests")
  setBooleanOption(NANAIRO_BUILD_TESTS OFF ${option_description})

  Set(option_description "The size of a memory pool")
  math(EXPR __memory_size__ "1 << 15")
  setStringOption(NANAIRO_MEMORY_POOL_SIZE ${__memory_size__} ${option_description})

  set(option_description "The shortest wavelength of the spectra range")
  setStringOption(NANAIRO_SHORTEST_WAVELENGTH 380 ${option_description})

  set(option_description "The longest wavelength of the spectra range")
  setStringOption(NANAIRO_LONGEST_WAVELENGTH 780 ${option_description})

  set(option_description "The resolution of wavelength. Select from 1 or 5 or 10.")
  setStringOption(NANAIRO_WAVELENGTH_RESOLUTION 10 ${option_description})

  set(option_description "The sample size of wavelengths in a cycle of progressive monte calro ray tracing method (It is must be 3).")
  setStringOption(NANAIRO_WAVELENGTH_SAMPLE_SIZE 3 ${option_description})

  set(option_description "Enable only the explicit connection of path tracing.")
  setBooleanOption(NANAIRO_PATH_TRACING_EXPLICIT_CONNECTION_ONLY OFF ${option_description})

  set(option_description "Enable only the implicit connection of path tracing.")
  setBooleanOption(NANAIRO_PATH_TRACING_IMPLICIT_CONNECTION_ONLY OFF ${option_description})

  set(option_description "Set the heuristic parameter of the MIS weight calculation (1: balance heuristic, 2: power heuristic).")
  setStringOption(NANAIRO_MIS_HEURISTIC_BETA 2 ${option_description})

  validateOptions()
endfunction(setCommandOptions)
