/*!
  \file cloth_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CLOTH_SURFACE_INL_HPP
#define NANAIRO_CLOTH_SURFACE_INL_HPP

#include "cloth_surface.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "fresnel.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/wavelength_samples.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/Bxdf/microcylinder_cloth_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  */
inline
Float ClothSurface::eta() const noexcept
{
  return eta_;
}

/*!
  */
inline
Float ClothSurface::gammaR() const noexcept
{
  return gamma_r_;
}

/*!
  */
inline
Float ClothSurface::gammaV() const noexcept
{
  return gamma_v_;
}

/*!
  */
inline
Float ClothSurface::kD() const noexcept
{
  return k_d_;
}

/*!
  */
inline
Float ClothSurface::rho() const noexcept
{
  return rho_;
}

} // namespace nanairo

#endif // NANAIRO_CLOTH_SURFACE_INL_HPP
