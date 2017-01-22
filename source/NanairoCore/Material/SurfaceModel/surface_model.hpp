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
class ShaderModel;
class SpectralDistribution;
class TextureModel;
class WavelengthSamples;

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
  LayeredDiffuse,
  Cloth
};

/*!
  \details
  No detailed.
  */
class SurfaceModel
{
 public:
  using ShaderPointer = UniquePointer<ShaderModel>;


  //! Finalize the surface model
  virtual ~SurfaceModel() noexcept;


  //! Make BxDF
  virtual ShaderPointer makeBxdf(
      const Point2& texture_coordinate,
      const bool is_reverse_face,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      MemoryPool& memory_pool) const noexcept = 0;

  //! Make a surface scattering model
  static UniquePointer<SurfaceModel> makeSurface(
      const QJsonObject& settings,
      const std::vector<const TextureModel*>& texture_list) noexcept;

  //! Return the surface type
  virtual SurfaceType type() const noexcept = 0;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SURFACE_MODEL_HPP
