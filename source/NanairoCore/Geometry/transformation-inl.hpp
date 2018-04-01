/*!
  \file transformation-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TRANSFORMATION_INL_HPP
#define NANAIRO_TRANSFORMATION_INL_HPP

#include "transformation.hpp"
// Standard C++ library
#include <cmath>
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/matrix.hpp"
// Nanairo
#include "vector.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {


/*!
  \details
  "Building an Orthonormal Basis from a 3D Unit Vector Without Normalization".
  */
inline
std::tuple<Vector3, Vector3> Transformation::calcDefaultTangent(const Vector3& normal) noexcept
{
  const auto& n = normal;
  ZISC_ASSERT(isUnitVector(n), "The normal isn't unit vector.");
  const Float sign = (n[2] < 0.0) ? -1.0 : 1.0;
  ZISC_ASSERT((sign + n[2]) != 0.0, "Zero division is occured.");
  const Float a = -zisc::invert(sign + n[2]);
  const Float b = n[0] * n[1] * a;
  const Vector3 b1{1.0 + sign * zisc::power<2>(n[0]) * a, sign * b, -sign * n[0]};
  const Vector3 b2{b, sign + zisc::power<2>(n[1]) * a, -n[1]};
  return std::make_tuple(b1, b2);
}

/*!
  */
inline
Vector3 Transformation::fromLocal(const Vector3& e1,
                                  const Vector3& e2,
                                  const Vector3& e3,
                                  const Vector3& v) noexcept
{
  Vector3 v_dash;
  for (uint i = 0; i < 3; ++i) {
    const Vector3 e{e1[i], e2[i], e3[i]};
    v_dash[i] = zisc::dot(e, v);
  }
  return v_dash;
}

/*!
  */
inline
Vector3 Transformation::toLocal(const Vector3& e1,
                                const Vector3& e2,
                                const Vector3& e3,
                                const Vector3& v) noexcept
{
  const Vector3 v_dash{zisc::dot(e1, v), zisc::dot(e2, v), zisc::dot(e3, v)};
  return v_dash;
}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4 Transformation::makeIdentity() noexcept
{
  return Matrix4x4{1.0, 0.0, 0.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   0.0, 0.0, 0.0, 1.0};
}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4 Transformation::makeTranslation(const Float x,
                                          const Float y,
                                          const Float z) noexcept
{
  return Matrix4x4{1.0, 0.0, 0.0,   x,
                   0.0, 1.0, 0.0,   y,
                   0.0, 0.0, 1.0,   z,
                   0.0, 0.0, 0.0, 1.0};
}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4 Transformation::makeScaling(const Float x,
                                      const Float y,
                                      const Float z) noexcept
{
  return Matrix4x4{  x, 0.0, 0.0, 0.0,
                   0.0,   y, 0.0, 0.0,
                   0.0, 0.0,   z, 0.0,
                   0.0, 0.0, 0.0, 1.0};
}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4 Transformation::makeXAxisRotation(const Float theta) noexcept
{
  return Matrix4x4{1.0, 0.0            ,  0.0            , 0.0,
                   0.0, std::cos(theta), -std::sin(theta), 0.0,
                   0.0, std::sin(theta),  std::cos(theta), 0.0,
                   0.0, 0.0            ,  0.0            , 1.0};
}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4 Transformation::makeYAxisRotation(const Float theta) noexcept
{
  return Matrix4x4{ std::cos(theta), 0.0, std::sin(theta), 0.0,
                    0.0            , 1.0, 0.0            , 0.0,
                   -std::sin(theta), 0.0, std::cos(theta), 0.0,
                    0.0            , 0.0, 0.0            , 1.0};
}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4 Transformation::makeZAxisRotation(const Float theta) noexcept
{
  return Matrix4x4{std::cos(theta), -std::sin(theta), 0.0, 0.0,
                   std::sin(theta),  std::cos(theta), 0.0, 0.0,
                   0.0            ,  0.0            , 1.0, 0.0,
                   0.0            ,  0.0            , 0.0, 1.0};
}

} // namespace nanairo

#endif // NANAIRO_TRANSFORMATION_INL_HPP
