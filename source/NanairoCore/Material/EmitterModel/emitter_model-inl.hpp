/*!
  \file emitter_model-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_EMITTER_MODEL_INL_HPP
#define NANAIRO_EMITTER_MODEL_INL_HPP

#include "emitter_model.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Float EmitterModel::radiantExitance() const noexcept
{
  return radiant_exitance_;
}

} // namespace nanairo

#endif // NANAIRO_EMITTER_MODEL_INL_HPP
