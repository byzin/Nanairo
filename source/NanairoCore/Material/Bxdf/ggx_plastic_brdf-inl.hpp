///*!
//  \file ggx_plastic_brdf-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2016 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef _NANAIRO_GGX_PLASTIC_BRDF_INL_HPP_
//#define _NANAIRO_GGX_PLASTIC_BRDF_INL_HPP_
//
//#include "ggx_plastic_brdf.hpp"
//// Standard C++ library
//#include <tuple>
//// Zisc
//#include "zisc/error.hpp"
//#include "zisc/math.hpp"
//// Nanairo
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/LinearAlgebra/vector.hpp"
//#include "NanairoCore/Material/shader_model.hpp"
//#include "NanairoCore/Material/SurfaceModel/microfacet_ggx.hpp"
//#include "NanairoCore/Sampling/sampled_direction.hpp"
//#include "NanairoCore/Sampling/sampled_spectra.hpp"
//
//namespace nanairo {
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize> inline
//GgxPlasticBrdf<kSampleSize>::GgxPlasticBrdf(const Float roughness,
//                                            const Vector3& normal,
//                                            const Float n,
//                                            const SampledDirection& vout,
//                                            const Float weight,
//                                            const Float diffuse_color) :
//    n_{n},
//    weight_{weight},
//    vout_{vout},
//    roughness_{roughness},
//    diffuse_color_{diffuse_color},
//    normal_{&normal}
//{
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize> inline
//Float GgxPlasticBrdf<kSampleSize>::evaluateDiffuseTerm(const Float roughness,
//                                                       const Float cos_theta_i,
//                                                       const Float cos_theta_o,
//                                                       const Float cos_theta_mi,
//                                                       const Float diffuse_color)
//{
//  // Calculate the diffuse fresnel
//  const Float f_d90 = 0.5 + 2.0 * zisc::power<2>(cos_theta_mi) * roughness;
//  const Float diffuse_fresnel = 
//      (1.0 + (f_d90 - 1.0) * zisc::power<5>(1.0 - cos_theta_i)) *
//      (1.0 + (f_d90 - 1.0) * zisc::power<5>(1.0 - cos_theta_o));
//
//  // Calc the diffuse reflectance
//  return diffuse_color * diffuse_fresnel;
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize> inline
//auto GgxPlasticBrdf<kSampleSize>::evaluateWeight(
//    const Vector3* /* vin */,
//    const SampledDirection* /* vout */,
//    const Wavelengths& wavelengths) const -> Spectra
//{
//  Spectra weight{wavelengths};
//  weight.setIntensity(wavelengths.primaryWavelengthIndex(), weight_);
//  return weight;
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize> inline
//auto GgxPlasticBrdf<kSampleSize>::evaluateRadiance(
//    const Vector3* vin,
//    const Vector3* vout,
//    const Wavelengths& wavelengths) const -> Spectra
//{
//  const Float cos_theta_i = -zisc::dot(*normal_, *vin);
//  const Float cos_theta_o = zisc::dot(*normal_, *vout);
//  Float cos_theta_mi = 0.0;
//  const Float ggx_term = 
//      evaluateGgxReflectance(roughness_, *vin, *vout, *normal_,
//                             n_, cos_theta_i, cos_theta_o, nullptr, &cos_theta_mi);
//  const Float diffuse_term = evaluateDiffuseTerm(roughness_, cos_theta_i, cos_theta_o,
//                                                 cos_theta_mi, diffuse_color_);
//  const Float f = diffuse_term + ggx_term;
//  NANAIRO_CHECK_POSITIVE_FLOAT(f);
//
//  Spectra reflectance{wavelengths};
//  reflectance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
//  return reflectance;
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize> inline
//auto GgxPlasticBrdf<kSampleSize>::evaluateRadianceAndPdf(
//    const Vector3* vin,
//    const Vector3* vout,
//    const Wavelengths& wavelengths) const -> std::tuple<Spectra, Float>
//{
//  const Float cos_theta_i = -zisc::dot(*normal_, *vin);
//  const Float cos_theta_o = zisc::dot(*normal_, *vout);
//  Float cos_theta_mi = 0.0;
//  Float pdf = 0.0;
//  const Float ggx_term = 
//      evaluateGgxReflectance(roughness_, *vin, *vout, *normal_,
//                             n_, cos_theta_i, cos_theta_o, &pdf, &cos_theta_mi);
//  const Float diffuse_term = evaluateDiffuseTerm(roughness_, cos_theta_i, cos_theta_o,
//                                                 cos_theta_mi, diffuse_color_);
//  const Float f = diffuse_term + ggx_term;
//  NANAIRO_CHECK_POSITIVE_FLOAT(f);
//
//  Spectra reflectance{wavelengths};
//  reflectance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
//  return std::make_tuple(reflectance, pdf);
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize> inline
//SampledDirection GgxPlasticBrdf<kSampleSize>::sampleDirection(
//    Sampler& /* sampler */) const
//{
//  return vout_;
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize> inline
//bool GgxPlasticBrdf<kSampleSize>::wavelengthIsSelected() const
//{
//  return true;
//}
//
//} // namespace nanairo
//
//#endif // _NANAIRO_GGX_PLASTIC_BRDF_INL_HPP_
