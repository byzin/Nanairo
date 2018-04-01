/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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

using Vector2 = zisc::Vector<Float, 2>;
using Vector3 = zisc::Vector<Float, 3>;
using Vector4 = zisc::Vector<Float, 4>;

//! Check if the vector contains the specified value
bool hasValue(const Vector3& vector, const Float value) noexcept;

//! Invert the vector
Vector3 invert(const Vector3& vector) noexcept;

//! Check if the vector is unit vector
bool isUnitVector(const Vector3& vector) noexcept;

//! Check if the vector is zero vector
bool isZeroVector(const Vector3& vector) noexcept;

//! \} Core

} // namespace nanairo

#include "vector-inl.hpp"

#endif // NANAIRO_VECTOR_HPP
