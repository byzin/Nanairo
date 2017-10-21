/*!
  \file transformation.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "transformation.hpp"
// Standard C++ library
#include <cmath>
#include <utility>
#include <vector>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/matrix.hpp"
#include "zisc/unit.hpp"
// Nanairo
#include "point.hpp"
#include "vector.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/transformation_setting_node.hpp"

namespace nanairo {

/*!
  */
Matrix4x4 Transformation::makeTransformation(
    const std::vector<SettingNodeBase*>& settings_list) noexcept
{
  auto matrix = makeIdentity();
  for (const auto settings : settings_list) {
    // m = (next transformation) * (prev transformation)
    matrix = matrix * makeTransformation(settings);
  }
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 Transformation::makeTransformation(const SettingNodeBase* settings) noexcept
{
  const auto transformation_settings = castNode<TransformationSettingNode>(settings);

  auto matrix = makeIdentity();
  if (transformation_settings->isEnabled()) {
    switch (transformation_settings->transformationType()) {
     case TransformationType::kTranslation: {
      matrix = makeTranslation(settings);
      break;
     }
     case TransformationType::kScaling: {
      matrix = makeScaling(settings);
      break;
     }
     case TransformationType::kRotation: {
      matrix = makeRotation(settings);
      break;
     }
     default: {
      zisc::raiseError("TransformationError: Unsupported type is specified.");
      break;
     }
    }
  }
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 Transformation::makeTranslation(const SettingNodeBase* settings) noexcept
{
  const auto transformation_settings = castNode<TransformationSettingNode>(settings);

  const Float x = transformation_settings->x();
  const Float y = transformation_settings->y();
  const Float z = transformation_settings->z();
  return makeTranslation(x, y, z);
}

/*!
  \details
  No detailed.
  */
Matrix4x4 Transformation::makeScaling(const SettingNodeBase* settings) noexcept
{
  const auto transformation_settings = castNode<TransformationSettingNode>(settings);

  const Float x = transformation_settings->x();
  const Float y = transformation_settings->y();
  const Float z = transformation_settings->z();
  return makeScaling(x, y, z);
}

/*!
  \details
  No detailed.
  */
Matrix4x4 Transformation::makeRotation(const SettingNodeBase* settings) noexcept
{
  const auto transformation_settings = castNode<TransformationSettingNode>(settings);

  auto matrix = makeIdentity();
  {
    const Float x = transformation_settings->x();
    if (x != 0.0)
      matrix = makeXAxisRotation(x) * matrix;
  }
  {
    const Float y = transformation_settings->y();
    if (y != 0.0)
      matrix = makeYAxisRotation(y) * matrix;
  }
  {
    const Float z = transformation_settings->z();
    if (z != 0.0)
      matrix = makeZAxisRotation(z) * matrix;
  }
  return matrix;
}

/*!
  \details
  No detailed.
  */
void Transformation::affineTransform(const Matrix4x4& matrix,
                                     Point3* point) noexcept
{
  const auto new_point = matrix * makePoint4(*point, 1.0);
  ZISC_ASSERT(zisc::isInBounds(new_point[3], 1.0 - 1.0e-6, 1.0 + 1.0e-6),
              "The transformation is failed.");
  *point = takePoint3(new_point);
}

/*!
  \details
  Please see the details of this algoriths below URL.
  http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
  */
void Transformation::affineTransform(const Matrix4x4& matrix,
                                     Vector3* vector) noexcept
{
  const Matrix3x3 normal_matrix{matrix(0, 0), matrix(0, 1), matrix(0, 2),
                                matrix(1, 0), matrix(1, 1), matrix(1, 2),
                                matrix(2, 0), matrix(2, 1), matrix(2, 2)};
  *vector = normal_matrix * (*vector);
}

} // namespace nanairo
