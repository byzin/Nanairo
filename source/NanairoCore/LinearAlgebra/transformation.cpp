/*!
  \file transformation.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "transformation.hpp"
// Standard C++ library
#include <cmath>
#include <utility>
// Qt
#include <QJsonArray>
#include <QJsonObject>
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
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Matrix4x4 makeTransformationMatrix(const QJsonArray& settings) noexcept
{
  using zisc::toHash32;

  auto matrix = makeIdentityMatrix();

  const int count = settings.count();
  for (int index = count - 1; 0 <= index; --index) {
    const auto transformation_settings = SceneValue::toObject(settings[index]);
    const bool is_enabled = SceneValue::toBool(transformation_settings,
                                               keyword::enabled);
    if (!is_enabled)
      continue;
    const auto type = SceneValue::toString(transformation_settings, keyword::type);
    switch (keyword::toHash32(type)) {
      case toHash32(keyword::translation): {
        matrix = makeTranslationMatrix(transformation_settings) * matrix;
        break;
      }
      case toHash32(keyword::scaling): {
        matrix = makeScalingMatrix(transformation_settings) * matrix;
        break;
      }
      case toHash32(keyword::rotation): {
        matrix = makeRotationMatrix(transformation_settings) * matrix;
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
Matrix4x4 makeTranslationMatrix(const QJsonObject& settings) noexcept
{
  const auto value = SceneValue::toArray(settings, keyword::value);
  const Float x = SceneValue::toFloat<Float>(value[0]); 
  const Float y = SceneValue::toFloat<Float>(value[1]); 
  const Float z = SceneValue::toFloat<Float>(value[2]); 
  return makeTranslationMatrix(x, y, z);
}

/*!
  \details
  No detailed.
  */
Matrix4x4 makeScalingMatrix(const QJsonObject& settings) noexcept
{
  const auto value = SceneValue::toArray(settings, keyword::value);
  const Float x = SceneValue::toFloat<Float>(value[0]); 
  const Float y = SceneValue::toFloat<Float>(value[1]); 
  const Float z = SceneValue::toFloat<Float>(value[2]); 
  return makeScalingMatrix(x, y, z);
}

/*!
  \details
  No detailed.
  */
Matrix4x4 makeRotationMatrix(const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  const auto angle = SceneValue::toFloat<Float>(settings, keyword::angle);
  const auto unit = SceneValue::toString(settings, keyword::unit);
  const Float theta = (unit == keyword::degreeUnit)
      ? zisc::toRadian(angle)
      : angle;

  Matrix4x4 matrix;
  const auto axis = SceneValue::toString(settings, keyword::axis);
  switch (keyword::toHash32(axis)) {
    case toHash32(keyword::xAxis): {
      matrix = makeXAxisRotationMatrix(theta);
      break;
    }
    case toHash32(keyword::yAxis): {
      matrix = makeYAxisRotationMatrix(theta);
      break;
    }
    case toHash32(keyword::zAxis): {
      matrix = makeZAxisRotationMatrix(theta);
      break;
    }
    default: {
      zisc::raiseError("TransformationError: Unsupported axis is specified.");
      break;
    }
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
