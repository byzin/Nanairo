/*!
  \file vector-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_VECTOR_INL_HPP
#define NANAIRO_VECTOR_INL_HPP

#include "vector.hpp"
// Standard C++ library
#include <limits>
// Zisc
#include "zisc/math.hpp"
#include "zisc/vector.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
Vector3 invert(const Vector3& vector) noexcept
{
  constexpr Float m = std::numeric_limits<Float>::max();
  Vector3 inverse_vector{(vector[0] != 0.0) ? zisc::invert(vector[0]) : m,
                         (vector[1] != 0.0) ? zisc::invert(vector[1]) : m,
                         (vector[2] != 0.0) ? zisc::invert(vector[2]) : m};
  return inverse_vector;
}

} // namespace nanairo

#endif // NANAIRO_VECTOR_INL_HPP
