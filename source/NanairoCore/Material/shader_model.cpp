/*!
  \file shader_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "shader_model.hpp"
// Standard C++ library
#include <cstddef>
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_chunk.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class SampledDirection;
class Sampler;
class SurfaceModel;

/*!
  \details
  No detailed.
  */
ShaderModel::ShaderModel() noexcept
{
}

/*!
  \details
  No detailed.
  */
void ShaderModel::operator delete(void* shader, zisc::MemoryChunk* chunk) noexcept
{
  zisc::cast<ShaderModel*>(shader)->~ShaderModel();
  chunk->setFree(true);
}

/*!
  \details
  No detailed.
  */
void* ShaderModel::operator new(std::size_t, zisc::MemoryChunk* chunk) noexcept
{
  chunk->setFree(false);
  return chunk->data<void>();
}

/*!
  \details
  No detailed.
  */
Float ShaderModel::evalPdf(
    const Vector3* /* vin */, 
    const Vector3* /* vout */,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* /* info */) const noexcept
{
  zisc::raiseError("The evalPdf function is not implemented.");
  return 0.0;
}

/*!
  \details
  No detailed.
  */
SampledSpectra ShaderModel::evalRadiance(
    const Vector3* /* vin */, 
    const Vector3* /* vout */,
    const WavelengthSamples& wavelengths,
    const IntersectionInfo* /* info */) const noexcept
{
  zisc::raiseError("The evalPdf function is not implemented.");
  return SampledSpectra{wavelengths};
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> ShaderModel::evalRadianceAndPdf(
    const Vector3* /* vin */,
    const Vector3* /* vout */,
    const WavelengthSamples& wavelengths,
    const IntersectionInfo* /* info */) const noexcept
{
  zisc::raiseError("The evalRadianceAndPdf function is not implemented.");
  return std::make_tuple(SampledSpectra{wavelengths}, 0.0);
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> ShaderModel::sample(
    const Vector3* /* vin */,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    const IntersectionInfo* /* info */) const noexcept
{
  zisc::raiseError("The sample function is not implemented.");
  return std::make_tuple(SampledDirection{}, SampledSpectra{wavelengths});
}

} // namespace nanairo
