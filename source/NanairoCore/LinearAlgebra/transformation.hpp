/*!
  \file transformation.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TRANSFORMATION_HPP
#define NANAIRO_TRANSFORMATION_HPP

// Standard C++ library
#include <vector>
// Zisc
#include "zisc/matrix.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QJsonArray;
class QJsonObject;

namespace nanairo {


//! \addtogroup Core
//! \{

using Matrix3x3 = zisc::Matrix<Float, 3, 3>;
using Matrix4x4 = zisc::Matrix<Float, 4, 4>;

//! Make a change of basis matrix for converting to the standard basis
Matrix3x3 makeChangeOfBasisMatrixFromLocal(const Vector3& normal) noexcept;

//! Make a change of basis matrix for converting to the standard basis
Matrix3x3 makeChangeOfBasisMatrixToLocal(const Vector3& normal) noexcept;

//! Make identity matrix
Matrix4x4 makeIdentityMatrix() noexcept;

//! Make transformation matrix
Matrix4x4 makeTransformationMatrix(const QJsonArray& settings) noexcept;

//! Make translation matrix
Matrix4x4 makeTranslationMatrix(const Float x, const Float y, const Float z) noexcept;

//! Make translation matrix
Matrix4x4 makeTranslationMatrix(const QJsonObject& settings) noexcept;

//! Make scaling matrix
Matrix4x4 makeScalingMatrix(const Float x, const Float y, const Float z) noexcept;

//! Make scaling matrix
Matrix4x4 makeScalingMatrix(const QJsonObject& settings) noexcept;

//! Make rotation matrix
Matrix4x4 makeRotationMatrix(const Vector3& a, const Vector3& b) noexcept;

//! Make rotation matrix
Matrix4x4 makeRotationMatrix(const QJsonObject& settings) noexcept;

//! Make x axis rotation matrix
Matrix4x4 makeXAxisRotationMatrix(const Float theta) noexcept;

//! Make y axis rotation matrix
Matrix4x4 makeYAxisRotationMatrix(const Float theta) noexcept;

//! Make z axis rotation matrix
Matrix4x4 makeZAxisRotationMatrix(const Float theta) noexcept;

//! Apply affine transformation to a point
void affineTransform(const Matrix4x4& matrix, Point3* point) noexcept;

//! Apply affine transformation to a vector
void affineTransform(const Matrix4x4& matrix, Vector3* vector) noexcept;

//! \} Core 

} // namespace nanairo

#include "transformation-inl.hpp"

#endif // NANAIRO_TRANSFORMATION_HPP
