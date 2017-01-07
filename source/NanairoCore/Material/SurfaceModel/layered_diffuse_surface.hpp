/*!
  \file layered_diffuse_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LAYERED_DIFFUSE_SURFACE_HPP
#define NANAIRO_LAYERED_DIFFUSE_SURFACE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class Sampler;
template <uint> class ShaderModel;
class TextureModel;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class LayeredDiffuseSurface : public SurfaceModel
{
 public:
  template <uint kSampleSize>
  using ShaderPointer = SurfaceModel::ShaderPointer<kSampleSize>;


  //! Create a rough dielectric surface
  LayeredDiffuseSurface(
      const QJsonObject& settings,
      const std::vector<const TextureModel*>& texture_list) noexcept;


  //! Make a interfaced lambertian BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeInterfacedLambertianBrdf(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      Sampler& sampler,
      MemoryPool& memory_pool) const noexcept;

  //! Return the rough dielectric surface type
  SurfaceType type() const noexcept override;

 private:
  //! Calculate the internal reflectance
  void calcInternalReflectance() noexcept;

  //! Initialize
  void initialize(const QJsonObject& settings,
                  const std::vector<const TextureModel*>& texture_list) noexcept;


  const TextureModel* reflectance_;
  const TextureModel* roughness_;
  SpectralDistribution ri_;
  SpectralDistribution eta_;
};

//! \} Core

} // namespace nanairo

#include "layered_diffuse_surface-inl.hpp"

#endif // NANAIRO_LAYERED_DIFFUSE_SURFACE_HPP
