/*!
  \file light_source_info.hpp.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_SOURCE_INFO_HPP
#define NANAIRO_LIGHT_SOURCE_INFO_HPP

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
class LightSourceInfo
{
 public:
  //! Create a invalid light source info
  LightSourceInfo() noexcept;

  //! Create a light source info
  LightSourceInfo(const Object* light_source) noexcept;

  //! Create a light source info
  LightSourceInfo(const Object* light_source, const Float weight) noexcept;


  //! Return the inverse weight of this light source
  Float inverseWeight() const noexcept;

  //! Check if the info is valid
  bool isValid() const noexcept;

  //! Return the light source object
  const Object* object() const noexcept;

  //! Set the weight of the light source
  void setWeight(const Float weight) noexcept;

  //! Return the weight of this light sonrce in the scene
  Float weight() const noexcept;

 private:
  //! Initialize
  void initialize() noexcept;


  Float inverse_weight_;
  const Object* light_source_;
};

//! \} Core

} // namespace nanairo

#include "light_source_info-inl.hpp"

#endif // NANAIRO_LIGHT_SOURCE_INFO_HPP
