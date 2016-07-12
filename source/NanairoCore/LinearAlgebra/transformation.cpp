/*!
  \file transformation.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "transformation.hpp"
// Standard C++ library
#include <cmath>
#include <utility>
// Qt
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/matrix.hpp"
#include "zisc/unit.hpp"
// Nanairo
#include "point.hpp"
#include "vector.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Matrix4x4 makeTransformationMatrix(const SceneSettings& settings, 
                                   const QString& prefix) noexcept
{
  using zisc::toHash32;

  auto matrix = makeIdentityMatrix();

  const QString p = prefix + "/" + keyword::transformation;
  const int count = settings.intValue(p + "/" + keyword::count);
  for (int index = 0; index < count; ++index) {
    const auto item_id = (p + "/%1").arg(index);
    if (!settings.booleanValue(item_id + "/" + keyword::active))
      continue;
    const auto type = settings.stringValue(item_id + "/" + keyword::type);
    switch (keyword::toHash32(type)) {
     case toHash32(keyword::translation):
      matrix = makeTranslationMatrix(settings, item_id) * matrix;
      break;
     case toHash32(keyword::scaling):
      matrix = makeScalingMatrix(settings, item_id) * matrix;
      break;
     case toHash32(keyword::rotation):
      matrix = makeRotationMatrix(settings, item_id) * matrix;
      break;
     default:
      zisc::raiseError("TransformationError: Unsupported type is specified.");
      break;
    }
  }
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 makeTranslationMatrix(const SceneSettings& settings, const QString& prefix) noexcept
{
  const Float x = settings.realValue(prefix + "/" + keyword::x);
  const Float y = settings.realValue(prefix + "/" + keyword::y);
  const Float z = settings.realValue(prefix + "/" + keyword::z);
  return makeTranslationMatrix(x, y, z);
}

/*!
  \details
  No detailed.
  */
Matrix4x4 makeScalingMatrix(const SceneSettings& settings, const QString& prefix) noexcept
{
  const Float x = settings.realValue(prefix + "/" + keyword::x);
  const Float y = settings.realValue(prefix + "/" + keyword::y);
  const Float z = settings.realValue(prefix + "/" + keyword::z);
  return makeScalingMatrix(x, y, z);
}

/*!
  \details
  No detailed.
  */
Matrix4x4 makeRotationMatrix(const SceneSettings& settings, const QString& prefix) noexcept
{
  using zisc::toHash32;

  const auto unit = settings.stringValue(prefix + "/" + keyword::unit);
  const Float angle = settings.realValue(prefix + "/" + keyword::angle);
  const Float theta = (unit == keyword::degreeUnit)
      ? zisc::toRadian(angle)
      : angle;

  Matrix4x4 matrix;
  const auto axis = settings.stringValue(prefix + "/" + keyword::axis);
  switch (keyword::toHash32(axis)) {
   case toHash32(keyword::x):
    matrix = makeXAxisRotationMatrix(theta);
    break;
   case toHash32(keyword::y):
    matrix = makeYAxisRotationMatrix(theta);
    break;
   case toHash32(keyword::z):
    matrix = makeZAxisRotationMatrix(theta);
    break;
   default:
    zisc::raiseError("TransformationError: Unsupported axis is specified.");
    break;
  }
  return matrix;
}

/*!
  \details
  No detailed.
  */
void affineTransform(const Matrix4x4& matrix, Point3* point) noexcept
{
  *point = takePoint3(matrix * makePoint4(*point, 1.0));
}

/*!
  \details
  Please see the details of this algoriths below URL.
  http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
  */
void affineTransform(const Matrix4x4& matrix, Vector3* vector) noexcept
{
  const Matrix3x3 normal_matrix{matrix(0, 0), matrix(0, 1), matrix(0, 2),
                                matrix(1, 0), matrix(1, 1), matrix(1, 2),
                                matrix(2, 0), matrix(2, 1), matrix(2, 2)};
  *vector = normal_matrix * *vector;
}

} // namespace nanairo
