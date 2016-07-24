/*!
  \file sampled_wavelengths.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "sampled_wavelengths.hpp"
// Standard C++ library
#include <algorithm>
#include <iterator>
#include <tuple>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/compensated_summation.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Material/material.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SpectralDistribution calculateLightPdf(const World& world) noexcept
{
  constexpr uint exponent = 1; // Balance heuristics
//  constexpr uint exponent = 2; // Power heuristics

  const auto& light_source_list = world.lightSourceList();

  zisc::CompensatedSummation<Float> sum{0.0};
  for (const auto& light_source : light_source_list) {
    auto object = light_source.object();
    const Float p = object->geometry().surfaceArea() *
                    object->material().emitter().radiantExitance();
    sum.add(zisc::power<exponent>(p));
  }

  const Float inverse_sum = 1.0 / sum.get();
  SpectralDistribution pdf;
  for (const auto& light_source : light_source_list) {
    auto object = light_source.object();
    const auto& emitter = object->material().emitter();
    const Float p = object->geometry().surfaceArea();
    const Float weight = zisc::power<exponent>(p) * inverse_sum;
    pdf += emitter.powerDistribution() * weight;
  }
  return pdf.normalized();
}

} // namespace nanairo
