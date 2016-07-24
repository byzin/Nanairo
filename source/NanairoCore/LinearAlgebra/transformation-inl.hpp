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
// Zisc
#include "zisc/error.hpp"
#include "zisc/matrix.hpp"
// Nanairo
#include "vector.hpp"
#include "NanairoCore/nanairo_core_config.hpp"  
#include "NanairoCore/Utility/floating_point.hpp"

namespace nanairo {

/*!
  \details
  Make a change of basis matrix for converting from the standard basis 
  using "Building an Orthonormal Basis from a 3D Unit Vector Without Normalization".
  */
inline
Matrix3x3 makeChangeOfBasisMatrixFromLocal(const Vector3& normal) noexcept
{
  const auto& n = normal;

  Vector3 b1{-1.0,  0.0,  0.0};
  Vector3 b2{ 0.0, -1.0,  0.0};
  if (-0.999999 < n[2]) {
    const Float t = 1.0 / (1.0 + n[2]);
    // b1 vector
    b1[0] = 1.0 - (n[0] * n[0] * t);
    b1[1] = -n[0] * n[1] * t;
    b1[2] = -n[0];
    ZISC_ASSERT(isUnitVector(b1), "The b1 vector must be a unit vector.");
    // b2 vector
    b2[0] = b1[1];
    b2[1] = 1.0 - (n[1] * n[1] * t);
    b2[2] = -n[1];
    ZISC_ASSERT(isUnitVector(b2), "The b2 vector must be a unit vector.");
  }
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
Matrix3x3 makeChangeOfBasisMatrixToLocal(const Vector3& normal) noexcept
{
  const auto& n = normal;

  Vector3 b1{-1.0,  0.0,  0.0};
  Vector3 b2{ 0.0, -1.0,  0.0};
  if (-0.999999 < n[2]) {
    const Float t = 1.0 / (1.0 + n[2]);
    // b1 vector
    b1[0] = 1.0 - (n[0] * n[0] * t);
    b1[1] = -n[0] * n[1] * t;
    b1[2] = -n[0];
    ZISC_ASSERT(isUnitVector(b1), "The b1 vector must be a unit vector.");
    // b2 vector
    b2[0] = b1[1];
    b2[1] = 1.0 - (n[1] * n[1] * t);
    b2[2] = -n[1];
    ZISC_ASSERT(isUnitVector(b2), "The b2 vector must be a unit vector.");
  }
  return Matrix3x3{b1[0], b1[1], b1[2],
                   b2[0], b2[1], b2[2],
                    n[0],  n[1],  n[2]};
}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4 makeIdentityMatrix() noexcept
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
Matrix4x4 makeTranslationMatrix(const Float x, const Float y, const Float z) noexcept
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
Matrix4x4 makeScalingMatrix(const Float x, const Float y, const Float z) noexcept
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
//inline
//Matrix4x4 makeRotationMatrix(const Vector3& a, const Vector3& b)
//{
//  auto matrix = makeIdentityMatrix();
//  if (a == b)
//    return matrix;
//  for (uint i = 0; i < 3; ++i)
//    matrix(0, i) = a[i];
//  const auto c = zisc::cross(a, b).normalized();
//  for (uint i = 0; i < 3; ++i)
//    matrix(2, i) = c[i];
//  const auto d = zisc::cross(c, a).normalized();
//  for (uint i = 0; i < 3; ++i)
//    matrix(1, i) = d[i];
//  return matrix;
//}

/*!
  \details
  No detailed.
  */
inline
Matrix4x4 makeXAxisRotationMatrix(const Float theta) noexcept
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
Matrix4x4 makeYAxisRotationMatrix(const Float theta) noexcept
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
Matrix4x4 makeZAxisRotationMatrix(const Float theta) noexcept
{
  return Matrix4x4{std::cos(theta), -std::sin(theta), 0.0, 0.0,
                   std::sin(theta),  std::cos(theta), 0.0, 0.0,
                   0.0            ,  0.0            , 1.0, 0.0,
                   0.0            ,  0.0            , 0.0, 1.0};
}

} // namespace nanairo

#endif // NANAIRO_TRANSFORMATION_INL_HPP
