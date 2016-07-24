///*!
//  \file rough_plastic_surface-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2016 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef _NANAIRO_ROUGH_PLASTIC_SURFACE_INL_HPP_
//#define _NANAIRO_ROUGH_PLASTIC_SURFACE_INL_HPP_
//
//#include "rough_plastic_surface.hpp"
//// Standard C++ library
//#include <tuple>
//// Zisc
//#include "zisc/aligned_memory_pool.hpp"
//#include "zisc/math.hpp"
//#include "zisc/utility.hpp"
//// Nanairo
//#include "fresnel.hpp"
//#include "microfacet.hpp"
//#include "microfacet_ggx.hpp"
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/Color/spectral_distribution.hpp"
//#include "NanairoCore/Data/intersection_info.hpp"
//#include "NanairoCore/Data/wavelength_samples.hpp"
//#include "NanairoCore/LinearAlgebra/vector.hpp"
//#include "NanairoCore/Material/shader_model.hpp"
//#include "NanairoCore/Material/Bxdf/ggx_plastic_brdf.hpp"
//#include "NanairoCore/Material/Texture/texture.hpp"
//#include "NanairoCore/Sampling/sampled_spectra.hpp"
//#include "NanairoCore/Utility/floating_point.hpp"
//#include "NanairoCore/Utility/unique_pointer.hpp"
//
//namespace nanairo {
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize>
//auto RoughPlasticSurface::makeGgxPlasticBrdf(
//    const Vector3* vin,
//    const IntersectionInfo& intersection,
//    const WavelengthSamples<kSampleSize>& wavelengths,
//    Sampler& sampler,
//    MemoryPool& memory_pool) const -> ShaderPointer<kSampleSize>
//{
//  using Brdf = GgxPlasticBrdf<kSampleSize>;
//
//  // Get the roughness
//  constexpr Float threshold = 0.001;
//  Float roughness = roughness_->floatValue(intersection.textureCoordinate());
//  roughness = (roughness < threshold) ? threshold * threshold : roughness * roughness;
//
//  // Sample microfacet normal
//  const auto& normal = intersection.normal();
//  Float cos_theta_i = 0.0,
//        cos_theta_m = 0.0,
//        cos_theta_mi = 0.0;
//  const auto m_normal =
//      sampleGgxMicrofacetNormal(roughness, *vin, normal, sampler,
//                                &cos_theta_i, &cos_theta_m, &cos_theta_mi);
//
//  // Evaluate the fresnel term
//  const auto wavelength = wavelengths[wavelengths.primaryWavelengthIndex()];
//  const Float n = intersection.isReverseFace()
//      ? 1.0 / eta_.getByWavelength(wavelength)
//      : eta_.getByWavelength(wavelength);
//  const auto g = evaluateFresnelG(n, cos_theta_mi);
//  const Float fresnel = (std::get<0>(g))
//      ? solveFresnelDielectricEquation(cos_theta_mi, std::get<1>(g))
//      : 1.0; // Perfect reflection
//
//  // Calculate the reflection direction
//  auto vout = getMicrofacetReflectionDirection(*vin, m_normal, cos_theta_mi);
//  const Float cos_theta_o = zisc::dot(normal, vout.direction());
//  //! todo: Is it correct operation?
//  if (cos_theta_o < 0.0) {
//    vout.setInversePdf(0.0);
//    auto brdf = memory_pool.allocate<Brdf>(roughness, normal, n, vout, 0.0, 0.0);
//    return ShaderPointer<kSampleSize>{brdf};
//  }
//
//  // Evaluate the weight
//  constexpr Float k = 1.0 / zisc::kPi;
//  const auto diffuse_color = 
//      diffuse_color_->wavelengthValue(intersection.textureCoordinate(), wavelength) * 
//      k;
//  const Float diffuse_term = 
//      Brdf::evaluateDiffuseTerm(roughness, cos_theta_i, cos_theta_o, 
//                                cos_theta_mi, diffuse_color) *
//      cos_theta_o * vout.inversePdf();
//  const Float cos_theta_mo = cos_theta_mi;
//  const auto ggx_term = fresnel *
//                        evaluateGgxWeight(roughness, cos_theta_i, cos_theta_o,
//                                          cos_theta_m, cos_theta_mi, cos_theta_mo);
//  const Float weight = diffuse_term + ggx_term;
//
//  // Make GGX BRDF
//  auto brdf = memory_pool.allocate<Brdf>(roughness, normal, n,
//                                         vout, weight, diffuse_color);
//  return ShaderPointer<kSampleSize>{brdf};
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize> inline
//SurfaceModel::ShaderPointer<kSampleSize> makeGgxPlasticBrdf(
//    const SurfaceModel* surface,
//    const Vector3* vin,
//    const IntersectionInfo& intersection,
//    const WavelengthSamples<kSampleSize>& wavelengths,
//    Sampler& sampler,
//    MemoryPool& memory_pool)
//{
//  using zisc::cast;
//
//  auto plastic_surface = cast<const RoughPlasticSurface*>(surface);
//  return plastic_surface->makeGgxPlasticBrdf(vin,
//                                             intersection,
//                                             wavelengths,
//                                             sampler,
//                                             memory_pool);
//}
//
//} // namespace nanairo
//
//#endif // _NANAIRO_ROUGH_PLASTIC_SURFACE_INL_HPP_
