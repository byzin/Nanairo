/*!
  \file shader_model_test.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SHADER_MODEL_TEST_HPP_
#define _NANAIRO_SHADER_MODEL_TEST_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"

//! Test importance sampling of a BxDF
void testBxdfSampling(
    const nanairo::SurfaceModel& surface_model,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name);

//! Test importance sampling of a BxDF
void testBxdfImportanceSampling(
    const nanairo::SurfaceModel& surface_model,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name);

//! Test Energy conservation of a BxDF
void testBxdfEnergyConservation(
    const nanairo::SurfaceModel& surface_model,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name);

//! Test Helmholtz reciprocity of a BxDF
void testBxdfHelmholtzReciprocity(
    const nanairo::SurfaceModel& surface_model,
    const nanairo::IntersectionInfo& intersection,
    const nanairo::WavelengthSamples<1>& wavelengths,
    nanairo::Sampler& sampler,
    nanairo::MemoryPool& memory_pool,
    const char* bxdf_name);

#endif // _NANAIRO_SHADER_MODEL_TEST_HPP_
