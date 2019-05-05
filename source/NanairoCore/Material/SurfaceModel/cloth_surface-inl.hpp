/*!
  \file cloth_surface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CLOTH_SURFACE_INL_HPP
#define NANAIRO_CLOTH_SURFACE_INL_HPP

#include "cloth_surface.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

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
