///*!
//  \file shader_model_test.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2018 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef NANAIRO_SHADER_MODEL_TEST_HPP
//#define NANAIRO_SHADER_MODEL_TEST_HPP
//
//// Nanairo
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/system.hpp"
//#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
//#include "NanairoCore/Material/TextureModel/texture_model.hpp"
//#include "NanairoCore/Data/intersection_info.hpp"
//#include "NanairoCore/Data/wavelength_samples.hpp"
//#include "NanairoCore/Utility/unique_pointer.hpp"
//
////! Make a test value texture
//nanairo::UniquePointer<nanairo::TextureModel> makeTestValueTexture(
//    const nanairo::System& system,
//    const nanairo::Float value);
//
////! Make test wavelength samples
//nanairo::WavelengthSamples makeTestWavelengthSamples();
//
////! 
//
////! Test importance sampling of a BxDF
//void testBxdfSampling(
//    const nanairo::SurfaceModel& surface_model,
//    const nanairo::IntersectionInfo& intersection,
//    const nanairo::WavelengthSamples& wavelengths,
//    nanairo::Sampler& sampler,
//    nanairo::MemoryPool& memory_pool,
//    const char* bxdf_name,
//    const bool compare_reflectance = true);
//
////! Test importance sampling of a BxDF
//void testBxdfImportanceSampling(
//    const nanairo::SurfaceModel& surface_model,
//    const nanairo::IntersectionInfo& intersection,
//    const nanairo::WavelengthSamples& wavelengths,
//    nanairo::Sampler& sampler,
//    nanairo::MemoryPool& memory_pool,
//    const char* bxdf_name);
//
////! Test Energy conservation of a BxDF
//void testBxdfEnergyConservation(
//    const nanairo::SurfaceModel& surface_model,
//    const nanairo::IntersectionInfo& intersection,
//    const nanairo::WavelengthSamples& wavelengths,
//    nanairo::Sampler& sampler,
//    nanairo::MemoryPool& memory_pool,
//    const char* bxdf_name);
//
////! Test Helmholtz reciprocity of a BxDF
//void testBxdfHelmholtzReciprocity(
//    const nanairo::SurfaceModel& surface_model,
//    const nanairo::IntersectionInfo& intersection,
//    const nanairo::WavelengthSamples& wavelengths,
//    nanairo::Sampler& sampler,
//    nanairo::MemoryPool& memory_pool,
//    const char* bxdf_name);
//
//#endif // NANAIRO_SHADER_MODEL_TEST_HPP
