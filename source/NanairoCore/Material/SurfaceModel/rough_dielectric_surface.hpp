/*!
  \file rough_dielectric_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_ROUGH_DIELECTRIC_SURFACE_HPP
#define NANAIRO_ROUGH_DIELECTRIC_SURFACE_HPP

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
class RoughDielectricSurface : public SurfaceModel
{
 public:
  template <uint kSampleSize>
  using ShaderPointer = SurfaceModel::ShaderPointer<kSampleSize>;


  //! Create a rough dielectric surface
  RoughDielectricSurface(const SceneSettings& settings, 
                         const QString& prefix,
                         const std::vector<const Texture*>& texture_list) noexcept;


  //! Make a GGX BSDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeGgxDielectricBsdf(
      const Point2& texture_coordinate,
      const bool is_reverse_face,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Return the surface model size
  std::size_t surfaceSize() const noexcept override;

  //! Return the rough dielectric surface type
  SurfaceType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const SceneSettings& settings,
                  const QString& prefix,
                  const std::vector<const Texture*>& texture_list) noexcept;
  

  const Texture* roughness_;
  SpectralDistribution eta_;
};

//! \} Core

} // namespace nanairo

#include "rough_dielectric_surface-inl.hpp"

#endif // NANAIRO_ROUGH_DIELECTRIC_SURFACE_HPP
