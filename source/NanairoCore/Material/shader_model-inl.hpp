/*!
  \file shader_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SHADER_MODEL_INL_HPP_
#define _NANAIRO_SHADER_MODEL_INL_HPP_

#include "shader_model.hpp"
// Standard C++ library
#include <cstddef>
#include <tuple>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"

namespace nanairo {

// Forward declaration
class SampledDirection;
class Sampler;
class SurfaceModel;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
ShaderModel<kSampleSize>::ShaderModel()
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void ShaderModel<kSampleSize>::operator delete(void*)
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void* ShaderModel<kSampleSize>::operator new(std::size_t, void* memory)
{
  return memory;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
Float ShaderModel<kSampleSize>::evaluatePdf(
    const Vector3* /* vin */, 
    const Vector3* /* vout */,
    const Vector3& /* normal */,
    const Wavelengths& /* wavelengths */) const
{
  zisc::raiseError("The evaluatePdf function is not implemented.");
  return 0.0;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto ShaderModel<kSampleSize>::evaluateRadiance(
    const Vector3* /* vin */, 
    const Vector3* /* vout */,
    const Vector3& /* normal */,
    const Wavelengths& wavelengths) const -> Spectra
{
  zisc::raiseError("The evaluatePdf function is not implemented.");
  return Spectra{wavelengths};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto ShaderModel<kSampleSize>::evaluateRadianceAndPdf(
    const Vector3* /* vin */,
    const Vector3* /* vout */,
    const Vector3& /* normal */,
    const Wavelengths& wavelengths) const -> std::tuple<Spectra, Float>
{
  zisc::raiseError("The evaluateRadianceAndPdf function is not implemented.");
  return std::make_tuple(Spectra{wavelengths}, 0.0);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
auto ShaderModel<kSampleSize>::sample(
    const Vector3* /* vin */,
    const Vector3& /* normal */,
    const Wavelengths& wavelengths,
    Sampler& /* sampler */) const -> std::tuple<SampledDirection, Spectra>
{
  zisc::raiseError("The sample function is not implemented.");
  return std::make_tuple(SampledDirection{}, Spectra{wavelengths});
}

} // namespace nanairo

#endif // _NANAIRO_SHADER_MODEL_INL_HPP_
