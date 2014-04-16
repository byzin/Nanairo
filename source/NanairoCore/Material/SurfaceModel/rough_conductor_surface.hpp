/*!
  \file rough_conductor_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP_
#define _NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP_

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class Sampler;
class SceneSettings;
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
  RoughConductorSurface(const SceneSettings& settings,
                        const QString& prefix,
                        const std::vector<const Texture*>& texture_list);


  //! Make a GGX BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeGgxConductorBrdf(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const;

  //! Return the surface model size
  std::size_t surfaceSize() const override;

  //! Return the rough conductor surface type
  SurfaceType type() const override;

 private:
  //! Initialize
  void initialize(const SceneSettings& settings,
                  const QString& prefix,
                  const std::vector<const Texture*>& texture_list);


  const Texture* roughness_;
  SpectralDistribution reflectance_0deg_;
};

//! \} Core

} // namespace nanairo

#include "rough_conductor_surface-inl.hpp"

#endif // _NANAIRO_ROUGH_CONDUCTOR_SURFACE_HPP_
