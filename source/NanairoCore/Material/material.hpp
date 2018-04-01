/*!
  \file material.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MATERIAL_HPP
#define NANAIRO_MATERIAL_HPP

namespace nanairo {

// Forward declaration
class EmitterModel;
class SurfaceModel;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Material
{
 public:
  //! Create a material
  Material(const SurfaceModel* surface, const EmitterModel* emitter) noexcept;


  //! Return the emitter model
  const EmitterModel& emitter() const noexcept;

  //! Return the surface scattering model
  const SurfaceModel& surface() const noexcept;

  //! Check if this material is light source
  bool isLightSource() const noexcept;

 private:
  const SurfaceModel* surface_;
  const EmitterModel* emitter_;
};

//! \} Core

} // namespace nanairo

#include "material-inl.hpp"

#endif // NANAIRO_MATERIAL_HPP
