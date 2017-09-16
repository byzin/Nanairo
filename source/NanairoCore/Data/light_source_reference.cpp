/*!
  \file light_source_reference.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "light_source_reference.hpp"
// Zisc
#include "zisc/math.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "object.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
LightSourceReference::LightSourceReference() noexcept :
    inverse_weight_{0.0},
    light_source_{nullptr}
{
}

/*!
  \details
  No detailed.
  */
LightSourceReference::LightSourceReference(const Object* light_source) noexcept :
    inverse_weight_{0.0},
    light_source_{light_source}
{
}

/*!
  \details
  No detailed.
  */
LightSourceReference::LightSourceReference(const Object* light_source,
                                           const Float weight) noexcept :
    inverse_weight_{0.0},
    light_source_{light_source}
{
  setWeight(weight);
}

/*!
  */
void LightSourceReference::setWeight(const Float weight) noexcept
{
  ZISC_ASSERT(0.0 <= weight, "The weight of the light source isn't positive.");
  inverse_weight_ = (weight == 0.0) ? 0.0 : zisc::invert(weight);
}

/*!
  \details
  No detailed.
  */
void LightSourceReference::initialize() noexcept
{
}

} // namespace nanairo
