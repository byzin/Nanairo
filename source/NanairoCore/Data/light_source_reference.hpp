/*!
  \file light_source_reference.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_LIGHT_SOURCE_REFERENCE_HPP_
#define _NANAIRO_LIGHT_SOURCE_REFERENCE_HPP_

// Standard C++ library
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class Object;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class LightSourceReference
{
 public:
  //! Create a light source reference
  LightSourceReference(const Float total_flux, const Object* light_source);


  //! Return the inverse weight of this light source
  Float inverseWeight() const;

  //! Return the light source object
  const Object* object() const;

  //! Return the weight of this light sonrce in the scene
  Float weight() const;

 private:
  //! Initialize
  void initialize(const Float total_flux);


  Float weight_,
        inverse_weight_;
  const Object* light_source_;
};

//! \} Core

} // namespace nanairo

#include "light_source_reference-inl.hpp"

#endif // _NANAIRO_LIGHT_SOURCE_REFERENCE_HPP_
