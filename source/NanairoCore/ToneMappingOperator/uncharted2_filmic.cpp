/*!
  \file uncharted2_filmic.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "uncharted2_filmic.hpp"
// Nanairo
#include "tone_mapping_operator.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Uncharted2Filmic::Uncharted2Filmic(const System& system,
                                   const SettingNodeBase* settings) noexcept :
    ToneMappingOperator(system, settings)
{
}

/*!
  */
Float Uncharted2Filmic::tonemap(const Float x) const noexcept
{
  constexpr Float a = 0.15;
  constexpr Float b = 0.50;
  constexpr Float c = 0.10;
  constexpr Float d = 0.20;
  constexpr Float e = 0.02;
  constexpr Float f = 0.30;
  return (x * (a * x + c * b) + d * e) / (x * (a * x + b) + d * f) - e / f;
}

} // namespace nanairo
