/*!
  \file transformation.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_TRANSFORMATION_HPP_
#define _NANAIRO_TRANSFORMATION_HPP_

// Standard C++ library
#include <vector>
// Zisc
#include "zisc/matrix.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class SceneSettings;

//! \addtogroup Core
//! \{

using Matrix3x3 = zisc::Matrix<Float, 3, 3>;
using Matrix4x4 = zisc::Matrix<Float, 4, 4>;

//! Make a change of basis matrix for converting to the standard basis
Matrix3x3 makeChangeOfBasisMatrixFromLocal(const Vector3& normal);

//! Make a change of basis matrix for converting to the standard basis
Matrix3x3 makeChangeOfBasisMatrixToLocal(const Vector3& normal);

//! Make identity matrix
Matrix4x4 makeIdentityMatrix();

//! Make transformation matrix
Matrix4x4 makeTransformationMatrix(const SceneSettings& settings, 
                                   const QString& prefix);

//! Make translation matrix
Matrix4x4 makeTranslationMatrix(const Float x, const Float y, const Float z);

//! Make translation matrix
Matrix4x4 makeTranslationMatrix(const SceneSettings& settings, 
                                const QString& prefix);

//! Make scaling matrix
Matrix4x4 makeScalingMatrix(const Float x, const Float y, const Float z);

//! Make scaling matrix
Matrix4x4 makeScalingMatrix(const SceneSettings& settings, 
                            const QString& prefix);

//! Make rotation matrix
Matrix4x4 makeRotationMatrix(const Vector3& a, const Vector3& b);

//! Make rotation matrix
Matrix4x4 makeRotationMatrix(const SceneSettings& settings, 
                             const QString& prefix);

//! Make x axis rotation matrix
Matrix4x4 makeXAxisRotationMatrix(const Float theta);

//! Make y axis rotation matrix
Matrix4x4 makeYAxisRotationMatrix(const Float theta);

//! Make z axis rotation matrix
Matrix4x4 makeZAxisRotationMatrix(const Float theta);

//! Apply affine transformation to a point
void affineTransform(const Matrix4x4& matrix, Point3* point);

//! Apply affine transformation to a vector
void affineTransform(const Matrix4x4& matrix, Vector3* vector);

//! \} Core 

} // namespace nanairo

#include "transformation-inl.hpp"

#endif // _NANAIRO_TRANSFORMATION_HPP_
