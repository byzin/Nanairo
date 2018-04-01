/*!
  \file material-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MATERIAL_INL_HPP
#define NANAIRO_MATERIAL_INL_HPP

#include "material.hpp"
// Zisc
#include "zisc/error.hpp"

namespace nanairo {

// Forward declaration
class EmitterModel;
class SurfaceModel;

/*!
  \details
  No detailed.
  */
inline
Material::Material(const SurfaceModel* surface, const EmitterModel* emitter) noexcept :
    surface_{surface},
    emitter_{emitter}
{
  ZISC_ASSERT(surface_ != nullptr, "Surface is null.");
}

/*!
  \details
  No detailed.
  */
inline
const SurfaceModel& Material::surface() const noexcept
{
  return *surface_;
}

/*!
  \details
  No detailed.
  */
inline
bool Material::isLightSource() const noexcept
{
  return emitter_ != nullptr;
}

/*!
  \details
  No detailed.
  */
inline
const EmitterModel& Material::emitter() const noexcept
{
  return *emitter_;
}

} // namespace nanairo

#endif // NANAIRO_MATERIAL_INL_HPP
