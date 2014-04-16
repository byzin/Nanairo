/*!
  \file material.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_MATERIAL_HPP_
#define _NANAIRO_MATERIAL_HPP_

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
  Material(const SurfaceModel* surface, const EmitterModel* emitter);


  //! Return the emitter model
  const EmitterModel& emitter() const;

  //! Return the surface scattering model
  const SurfaceModel& surface() const;

  //! Check if this material is light source
  bool isLightSource() const;

 private:
  const SurfaceModel* surface_;
  const EmitterModel* emitter_;
};

//! \} Core

} // namespace nanairo

#include "material-inl.hpp"

#endif // _NANAIRO_MATERIAL_HPP_
