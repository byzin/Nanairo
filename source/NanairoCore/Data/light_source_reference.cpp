/*!
  \file light_source_reference.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "light_source_reference.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "object.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
LightSourceReference::LightSourceReference(const Float total_flux,
                                           const Object* light_source) noexcept :
    light_source_{light_source}
{
  initialize(total_flux);
}

/*!
  \details
  No detailed.
  */
void LightSourceReference::initialize(const Float total_flux) noexcept
{
  const Float flux = light_source_->shape().surfaceArea() *
                     light_source_->material().emitter().radiantExitance();
  weight_ = flux / total_flux;
  inverse_weight_ = zisc::invert(weight_);
}

} // namespace nanairo
