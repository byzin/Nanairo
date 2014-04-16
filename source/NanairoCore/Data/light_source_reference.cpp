/*!
  \file light_source_reference.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "light_source_reference.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "object.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/geometry.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
LightSourceReference::LightSourceReference(const Float total_flux,
                                           const Object* light_source) :
    light_source_{light_source}
{
  initialize(total_flux);
}

/*!
  \details
  No detailed.
  */
void LightSourceReference::initialize(const Float total_flux)
{
  const Float flux = light_source_->geometry().surfaceArea() *
                     light_source_->material().emitter().radiantExitance();
  weight_ = flux / total_flux;
  inverse_weight_ = 1.0 / weight_;
}

} // namespace nanairo
