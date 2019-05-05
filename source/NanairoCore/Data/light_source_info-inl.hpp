/*!
  \file light_source_info.hpp-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_SOURCE_INFO_INL_HPP
#define NANAIRO_LIGHT_SOURCE_INFO_INL_HPP

#include "light_source_info.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class Object;

/*!
  */
inline
LightSourceInfo::LightSourceInfo() noexcept :
    inverse_weight_{0.0},
    light_source_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
inline
LightSourceInfo::LightSourceInfo(const Object* light_source) noexcept :
    inverse_weight_{0.0},
    light_source_{light_source}
{
}

/*!
  \details
  No detailed.
  */
inline
LightSourceInfo::LightSourceInfo(const Object* light_source,
                                 const Float weight) noexcept :
    inverse_weight_{0.0},
    light_source_{light_source}
{
  setWeight(weight);
}

/*!
  \details
  No detailed.
  */
inline
Float LightSourceInfo::inverseWeight() const noexcept
{
  return inverse_weight_;
}

/*!
  */
inline
bool LightSourceInfo::isValid() const noexcept
{
  return (object() != nullptr);
}

/*!
  \details
  No detailed.
  */
inline
const Object* LightSourceInfo::object() const noexcept
{
  return light_source_;
}

/*!
  */
inline
void LightSourceInfo::setWeight(const Float weight) noexcept
{
  ZISC_ASSERT(zisc::isInClosedBounds(weight, 0.0, 1.0),
              "The weight of the light source is out of range [0, 1]: ", weight);
  inverse_weight_ = (weight == 0.0) ? 0.0 : zisc::invert(weight);
}

/*!
  \details
  No detailed.
  */
inline
Float LightSourceInfo::weight() const noexcept
{
  ZISC_ASSERT(isValid(), "The info is invalid.");
  return zisc::invert(inverse_weight_);
}

} // namespace nanairo

#endif // NANAIRO_LIGHT_SOURCE_INFO_INL_HPP
