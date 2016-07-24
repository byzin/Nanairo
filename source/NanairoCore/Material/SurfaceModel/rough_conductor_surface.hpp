/*!
  \file rough_conductor_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP
#define NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP

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
class Texture;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class RoughConductorSurface : public SurfaceModel
{
 public:
  template <uint kSampleSize>
  using ShaderPointer = SurfaceModel::ShaderPointer<kSampleSize>;


  //! Create a rough conductor surface
  RoughConductorSurface(const QJsonObject& settings,
                        const std::vector<const Texture*>& texture_list) noexcept;


  //! Make a GGX BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeGgxConductorBrdf(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Return the surface model size
  std::size_t surfaceSize() const noexcept override;

  //! Return the rough conductor surface type
  SurfaceType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const QJsonObject& settings,
                  const std::vector<const Texture*>& texture_list) noexcept;


  const Texture* roughness_;
  SpectralDistribution reflectance_0deg_;
};

//! \} Core

} // namespace nanairo

#include "rough_conductor_surface-inl.hpp"

#endif // NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP
