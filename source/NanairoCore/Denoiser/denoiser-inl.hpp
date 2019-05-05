/*!
  \file denoiser-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_DENOISER_INL_HPP
#define NANAIRO_DENOISER_INL_HPP

#include "denoiser.hpp"
// Zisc
#include "zisc/function_reference.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
const zisc::FunctionReference<void (double)>& Denoiser::progressCallback()
    const noexcept
{
  return progress_callback_;
}

} // namespace nanairo

#endif // NANAIRO_DENOISER_INL_HPP
