/*!
  \file smooth_diffuse_surface.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTH_DIFFUSE_SURFACE_HPP
#define NANAIRO_SMOOTH_DIFFUSE_SURFACE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "surface_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
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
class SmoothDiffuseSurface : public SurfaceModel
{
 public:
  template <uint kSampleSize>
  using ShaderPointer = SurfaceModel::ShaderPointer<kSampleSize>;


  //! Create a smooth diffuse surface
  SmoothDiffuseSurface(
      const QJsonObject& settings,
      const std::vector<const TextureModel*>& texture_list) noexcept;


  //! Make Lambert BRDF
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeLambertBrdf(
      const Point2& texture_coordinate,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Return the surface model size
  std::size_t surfaceSize() const noexcept override;

  //! Return the diffuse surface type
  SurfaceType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const QJsonObject& settings,
                  const std::vector<const TextureModel*>& texture_list) noexcept;


  const TextureModel* reflectance_;
};

//! \} Core

} // namespace nanairo

#include "smooth_diffuse_surface-inl.hpp"

#endif // NANAIRO_SMOOTH_DIFFUSE_SURFACE_HPP
