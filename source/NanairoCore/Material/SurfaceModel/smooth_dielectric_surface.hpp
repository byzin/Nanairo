/*!
  \file smooth_dielectric_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTH_DIELECTRIC_SURFACE_HPP
#define NANAIRO_SMOOTH_DIELECTRIC_SURFACE_HPP

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
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SmoothDielectricSurface : public SurfaceModel
{
 public:
  using ShaderPointer = SurfaceModel::ShaderPointer;


  //! Create a smooth dielectric surface
  SmoothDielectricSurface(const QJsonObject& settings) noexcept;


  //! Make a Fresnel BRDF
  ShaderPointer makeBxdf(
      const Point2& texture_coordinate,
      const bool is_reverse_face,
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      MemoryPool& memory_pool) const noexcept override;

  //! Return the diffuse surface type
  SurfaceType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const QJsonObject& settings) noexcept;


  SpectralDistribution eta_; //!< Interior exterior ratio of refractive index
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SMOOTH_DIELECTRIC_SURFACE_HPP
