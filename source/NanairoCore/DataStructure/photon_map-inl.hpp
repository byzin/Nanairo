/*!
  \file photon_map-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PHOTON_MAP_INL_HPP
#define NANAIRO_PHOTON_MAP_INL_HPP

#include "photon_map.hpp"
// Standard C++ library
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
constexpr bool PhotonMap::threadingIsEnabled() noexcept
{
  return true;
}

} // namespace nanairo

#endif // NANAIRO_PHOTON_MAP_INL_HPP
