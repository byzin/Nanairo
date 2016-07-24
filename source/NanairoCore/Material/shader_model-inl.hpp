/*!
  \file shader_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SHADER_MODEL_INL_HPP
#define NANAIRO_SHADER_MODEL_INL_HPP

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
ShaderModel<kSampleSize>::ShaderModel() noexcept
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void ShaderModel<kSampleSize>::operator delete(void*) noexcept
{
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void* ShaderModel<kSampleSize>::operator new(std::size_t, void* memory) noexcept
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
    const Wavelengths& /* wavelengths */) const noexcept
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
    const Wavelengths& wavelengths) const noexcept -> Spectra
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
    const Wavelengths& wavelengths) const noexcept -> std::tuple<Spectra, Float>
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
    Sampler& /* sampler */) const noexcept -> std::tuple<SampledDirection, Spectra>
{
  zisc::raiseError("The sample function is not implemented.");
  return std::make_tuple(SampledDirection{}, Spectra{wavelengths});
}

} // namespace nanairo

#endif // NANAIRO_SHADER_MODEL_INL_HPP
