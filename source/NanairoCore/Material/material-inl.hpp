/*!
  \file material-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_MATERIAL_INL_HPP_
#define _NANAIRO_MATERIAL_INL_HPP_

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
Material::Material(const SurfaceModel* surface, const EmitterModel* emitter) :
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
const SurfaceModel& Material::surface() const
{
  return *surface_;
}

/*!
  \details
  No detailed.
  */
inline
bool Material::isLightSource() const
{
  return emitter_ != nullptr;
}

/*!
  \details
  No detailed.
  */
inline
const EmitterModel& Material::emitter() const
{
  return *emitter_;
}
  
} // namespace nanairo

#endif // _NANAIRO_MATERIAL_INL_HPP_
