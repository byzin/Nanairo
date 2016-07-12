/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_VECTOR_HPP
#define NANAIRO_VECTOR_HPP

// Zisc
#include "zisc/vector.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

using Vector3 = zisc::Vector<Float, 3>;

//! \} Core

} // namespace nanairo

#endif // NANAIRO_VECTOR_HPP
