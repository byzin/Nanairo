///*!
//  \file ggx_plastic_brdf.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef _NANAIRO_GGX_PLASTIC_BRDF_HPP_
//#define _NANAIRO_GGX_PLASTIC_BRDF_HPP_
//
//// Standard C++ library
//#include <tuple>
//// Nanairo
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/LinearAlgebra/vector.hpp"
//#include "NanairoCore/Material/shader_model.hpp"
//#include "NanairoCore/Sampling/sampled_direction.hpp"
//#include "NanairoCore/Sampling/sampled_spectra.hpp"
//
//namespace nanairo {
//
////! \addtogroup Core
////! \{
//
///*!
//  \details
//  No detailed.
//  */
//template <uint kSampleSize>
//class GgxPlasticBrdf : public GlossyShaderModel<kSampleSize>
//{
// public:
//  using Spectra = typename ShaderModel<kSampleSize>::Spectra;
//  using Wavelengths = typename ShaderModel<kSampleSize>::Wavelengths;
//
//
//  //! Create a GGX plastic BRDF
//  GgxPlasticBrdf(const Float roughness,
//                 const Vector3& normal,
//                 const Float n,
//                 const SampledDirection& vout,
//                 const Float weight,
//                 const Float diffuse_color);
//
//
//  //! Evaluate the diffuse term
//  static Float evaluateDiffuseTerm(const Float roughness, 
//                                   const Float cos_theta_i,
//                                   const Float cos_theta_o,
//                                   const Float cos_theta_mi,
//                                   const Float diffuse_color);
//
//  //! Evaluate the weight (Fs(i, o, n) * cos(theta_o) / pdf) of solid angle sampling
//  Spectra evaluateWeight(const Vector3* vin,
//                         const SampledDirection* vout,
//                         const Wavelengths& wavelengths) const override;
//
//  //! Evaluate the radiance of the area sampling
//  Spectra evaluateRadiance(const Vector3* vin,
//                           const Vector3* vout,
//                           const Wavelengths& wavelengths) const override;
//
//  //! Evaluate the radiance of the area sampling
//  std::tuple<Spectra, Float> evaluateRadianceAndPdf(
//      const Vector3* vin,
//      const Vector3* vout,
//      const Wavelengths& wavelengths) const override;
//
//  //! Sample a next direction
//  SampledDirection sampleDirection(Sampler& sampler) const override;
//
//  //! Check if wavelength selection occured
//  bool wavelengthIsSelected() const override;
//
// private:
//  const Float n_;
//  const Float weight_;
//  const SampledDirection vout_;
//  const Float roughness_;
//  const Float diffuse_color_;
//  const Vector3* normal_;
//};
//
////! \}
//
//} // namespace nanairo
//
//#include "ggx_plastic_brdf-inl.hpp"
//
//#endif // _NANAIRO_GGX_PLASTIC_BRDF_HPP_
