/*!
  \file filmic.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "filmic.hpp"
// Nanairo
#include "tone_mapping_operator.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Filmic::Filmic(const System& system, const QJsonObject& settings) noexcept :
    ToneMappingOperator(system, settings)
{
}

/*!
  \details
  No detailed.
  */
Float Filmic::tonemap(const Float x) const noexcept
{
  constexpr Float a = 2.51;
  constexpr Float b = 0.03;
  constexpr Float c = 2.43;
  constexpr Float d = 0.59;
  constexpr Float e = 0.14;
  return (x * (a * x + b)) / (x * (c * x + d) + e);
}

} // namespace nanairo
