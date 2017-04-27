/*!
  \file transformation-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
  */
inline
std::tuple<Vector3, Vector3> Transformation::makeAxisesOfBasis(
    const Vector3& normal) noexcept
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
  \details
  Make a change of basis matrix for converting from the standard basis 
  using "Building an Orthonormal Basis from a 3D Unit Vector Without Normalization".
  */
inline
Matrix3x3 Transformation::makeChangeOfBasisFromLocal(const Vector3& normal) noexcept
{
  const auto& n = normal;
  const auto axises = makeAxisesOfBasis(n);
  const auto& b1 = std::get<0>(axises);
  const auto& b2 = std::get<1>(axises);
  return Matrix3x3{b1[0], b2[0], n[0],
                   b1[1], b2[1], n[1],
                   b1[2], b2[2], n[2]};
}

/*!
  \details
  Make a change of basis matrix for converting to the standard basis 
  using "Building an Orthonormal Basis from a 3D Unit Vector Without Normalization".
  */
inline
Matrix3x3 Transformation::makeChangeOfBasisToLocal(const Vector3& normal) noexcept
{
  const auto& n = normal;
  const auto axises = makeAxisesOfBasis(n);
  const auto& b1 = std::get<0>(axises);
  const auto& b2 = std::get<1>(axises);
  return Matrix3x3{b1[0], b1[1], b1[2],
                   b2[0], b2[1], b2[2],
                    n[0],  n[1],  n[2]};
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
