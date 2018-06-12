/*!
  \file film.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "film.hpp"
// Standard C++ library
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/camera_setting_node.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Film::Film(System& system, const SettingNodeBase* settings) noexcept :
    sample_statistics_{system}
{
  initialize(system, settings);
}

/*!
  \details
  No detailed.
  */
void Film::initialize(System& /* system */,
                      const SettingNodeBase* /* settings */) noexcept
{
}

} // namespace nanairo
