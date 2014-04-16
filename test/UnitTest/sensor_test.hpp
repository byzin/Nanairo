/*!
  \file sensor_test.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SENSOR_TEST_HPP_
#define _NANAIRO_SENSOR_TEST_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

/*!
  \details
  No detailed.
  */
void testSensorSampling(
    const nanairo::CameraModel& camera,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* sensor_name);

void testSensorEnergyConservation(
    const nanairo::CameraModel& camera,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* sensor_name);

void testSensorImportanceSampling(
    const nanairo::CameraModel& camera,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* sensor_name);

#endif // _NANAIRO_SENSOR_TEST_HPP_
