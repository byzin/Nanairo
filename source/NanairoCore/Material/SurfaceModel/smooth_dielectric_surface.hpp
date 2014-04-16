/*!
  \file smooth_dielectric_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SMOOTH_DIELECTRIC_SURFACE_HPP_
#define _NANAIRO_SMOOTH_DIELECTRIC_SURFACE_HPP_

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
class SceneSettings;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SmoothDielectricSurface : public SurfaceModel
{
 public:
  template <uint kSampleSize>
  using ShaderPointer = SurfaceModel::ShaderPointer<kSampleSize>;


  //! Create a smooth dielectric surface
  SmoothDielectricSurface(const SceneSettings& settings, 
                          const QString& prefix);


  //! Make a Fresnel BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeSpecularBsdf(
      const bool is_reverse_face,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const;

  //! Return the surface model size
  std::size_t surfaceSize() const override;

  //! Return the diffuse surface type
  SurfaceType type() const override;

 private:
  //! Initialize
  void initialize(const SceneSettings& settings, const QString& prefix);


  SpectralDistribution eta_; //!< Interior exterior ratio of refractive index
};

//! \} Core

} // namespace nanairo

#include "smooth_dielectric_surface-inl.hpp"

#endif // _NANAIRO_SMOOTH_DIELECTRIC_SURFACE_HPP_
  
