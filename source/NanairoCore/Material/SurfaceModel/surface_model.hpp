/*!
  \file surface_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SURFACE_MODEL_HPP
#define NANAIRO_SURFACE_MODEL_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class Sampler;
template <uint> class ShaderModel;
class SpectralDistribution;
class TextureModel;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
enum class SurfaceType : int
{
  SmoothDiffuse = 0,
  SmoothDielectric,
  SmoothConductor,
  RoughDielectric,
  RoughConductor,
  Cloth
};

/*!
  \details
  No detailed.
  */
class SurfaceModel
{
 public:
  template <uint kSampleSize>
  using ShaderPointer = UniquePointer<ShaderModel<kSampleSize>>;


  //! Finalize the surface model
  virtual ~SurfaceModel() noexcept;


  //! Make BxDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeBxdf(
      const Point2& texture_coordinate,
      const bool is_reverse_face,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Make a surface scattering model
  static UniquePointer<SurfaceModel> makeSurface(
      const QJsonObject& settings,
      const std::vector<const TextureModel*>& texture_list) noexcept;

  //! Return the surface type
  virtual SurfaceType type() const noexcept = 0;

 private:
  //! Make a lambert BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeLambertBrdf(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Make a specular BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeSpecularBsdf(
      const bool is_reverse_face,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Make a specular BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeSpecularBrdf(
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Make a GGX dielectric BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeGgxDielectricBsdf(
      const Point2& texture_coordinate,
      const bool is_reverse_face,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Make a GGX conductor BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeGgxConductorBrdf(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Make a microcylinder cloth BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeMicrocylinderClothBrdf(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

};

//! \} Core

} // namespace nanairo

#include "surface_model-inl.hpp"

#endif // NANAIRO_SURFACE_MODEL_HPP
