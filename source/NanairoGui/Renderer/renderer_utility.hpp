/*!
  \file renderer_utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RENDERER_UTILITY_HPP
#define NANAIRO_RENDERER_UTILITY_HPP

// Qt
#include <QString>
// Zisc
#include "zisc/stopwatch.hpp"

namespace nanairo {

//! \addtogroup Renderer
//! \{

//! Get time string
QString getTimeString(const zisc::Stopwatch::Clock::duration time) noexcept;

//! \} Renderer

} // namespace nanairo

#include "renderer_utility-inl.hpp"

#endif // NANAIRO_RENDERER_UTILITY_HPP
