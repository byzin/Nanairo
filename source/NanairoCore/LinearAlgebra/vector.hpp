/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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

//! Check if the vector contains the specified value
bool hasValue(const Vector3& vector, const Float value) noexcept;

//! Check if the vector is unit vector
bool isUnitVector(const Vector3& vector) noexcept;

//! Check if the vector is zero vector
bool isZeroVector(const Vector3& vector) noexcept;

//! \} Core

} // namespace nanairo

#endif // NANAIRO_VECTOR_HPP
