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
#include <tuple>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/matrix.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {


//! \addtogroup Core
//! \{

using Matrix3x3 = zisc::Matrix<Float, 3, 3>;
using Matrix4x4 = zisc::Matrix<Float, 4, 4>;

enum class TransformationType : uint32
{
  kTranslation                = zisc::toHash32("Translation"),
  kScaling                    = zisc::toHash32("Scaling"),
  kRotation                   = zisc::toHash32("Rotation")
};

/*!
  */
class Transformation
{
 public:
  //! Make axises of orthogonal basis from a normal
  static std::tuple<Vector3, Vector3> makeAxisesOfBasis(
      const Vector3& normal) noexcept;

  //! Make a change of basis matrix for converting to the standard basis
  static Matrix3x3 makeChangeOfBasisFromLocal(const Vector3& normal) noexcept;

  //! Make a change of basis matrix for converting to the standard basis
  static Matrix3x3 makeChangeOfBasisToLocal(const Vector3& normal) noexcept;

  //! Make identity matrix
  static Matrix4x4 makeIdentity() noexcept;

  //! Make transformation matrix
  static Matrix4x4 makeTransformation(
      const std::vector<SettingNodeBase*>& settings_list) noexcept;

  //! Make transformation matrix
  static Matrix4x4 makeTransformation(const SettingNodeBase* settings) noexcept;

  //! Make translation matrix
  static Matrix4x4 makeTranslation(const Float x,
                                   const Float y,
                                   const Float z) noexcept;

  //! Make translation matrix
  static Matrix4x4 makeTranslation(const SettingNodeBase* settings) noexcept;

  //! Make scaling matrix
  static Matrix4x4 makeScaling(const Float x,
                               const Float y,
                               const Float z) noexcept;

  //! Make scaling matrix
  static Matrix4x4 makeScaling(const SettingNodeBase* settings) noexcept;

  //! Make rotation matrix
  static Matrix4x4 makeRotation(const Vector3& a, const Vector3& b) noexcept;

  //! Make rotation matrix
  static Matrix4x4 makeRotation(const SettingNodeBase* settings) noexcept;

  //! Make x axis rotation matrix
  static Matrix4x4 makeXAxisRotation(const Float theta) noexcept;

  //! Make y axis rotation matrix
  static Matrix4x4 makeYAxisRotation(const Float theta) noexcept;

  //! Make z axis rotation matrix
  static Matrix4x4 makeZAxisRotation(const Float theta) noexcept;

  //! Apply affine transformation to a point
  static void affineTransform(const Matrix4x4& matrix, Point3* point) noexcept;

  //! Apply affine transformation to a vector
  static void affineTransform(const Matrix4x4& matrix, Vector3* vector) noexcept;
};

//! \} Core

} // namespace nanairo

#include "transformation-inl.hpp"

#endif // NANAIRO_TRANSFORMATION_HPP
