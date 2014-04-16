/*!
  \file renderer_utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RENDERER_UTILITY_HPP_
#define _NANAIRO_RENDERER_UTILITY_HPP_

// Qt
#include <QString>
// Zisc
#include "zisc/stopwatch.hpp"

namespace nanairo {

//! \addtogroup Renderer
//! \{

//! Get time string
QString getTimeString(const zisc::Stopwatch::Clock::duration time);

//! \} Renderer

} // namespace nanairo

#include "renderer_utility-inl.hpp"

#endif // _NANAIRO_RENDERER_UTILITY_HPP_
