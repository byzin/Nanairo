/*!
  \file light_source_sampler.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "light_source_sampler.hpp"
// Standard C++ library
#include <vector>
// Zisc
#include "zisc/compensated_summation.hpp"
#include "zisc/cumulative_distribution_function.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
LightSourceSampler::LightSourceSampler(
    const std::vector<LightSourceReference>& light_source_list) noexcept
{
  initialize(light_source_list);
}

/*!
  \details
  No detailed.
  */
void LightSourceSampler::initialize(
    const std::vector<LightSourceReference>& light_source_list) noexcept
{
  std::vector<const LightSourceReference*> light_list;
  std::vector<Float> pdf;

  light_list.reserve(light_source_list.size());
  pdf.reserve(light_source_list.size());
  for (const auto& light_source : light_source_list) {
    light_list.emplace_back(&light_source);
    pdf.emplace_back(light_source.weight());
  }

  cdf_ = new LightSourceCdf{light_list.begin(), light_list.end(),
                            pdf.begin(), pdf.end()};

  zisc::CompensatedSummation<Float> total_flux{0.0};
  for (const auto& light_source : light_source_list) {
    const auto object = light_source.object();
    const auto flux = object->shape().surfaceArea() *
                      object->material().emitter().radiantExitance();
    total_flux.add(flux);
  }
  inverse_total_flux_ = zisc::invert(total_flux.get());
}

} // namespace nanairo
