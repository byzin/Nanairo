/*!
  \file camera_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "camera_model.hpp"
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "pinhole_camera.hpp"
//#include "thin_lens_camera_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class Film;

/*!
  */
CameraModel::~CameraModel() noexcept
{
}

/*!
  \details
  No detailed.
  */
CameraModel::CameraModel(const QJsonObject& settings) noexcept
    : jittering_{0.0, 0.0}
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
Matrix4x4 CameraModel::rotate(const Vector2& value) noexcept
{
  const auto& p = position();
  auto matrix = makeTranslationMatrix(-p[0], -p[1], -p[2]);
  matrix = makeYAxisRotationMatrix(value[0]) * matrix;
  matrix = makeXAxisRotationMatrix(value[1]) * matrix;
  matrix = makeTranslationMatrix(p[0], p[1], p[2]) * matrix;
  transform(matrix);
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 CameraModel::translateHorizontally(const Vector2& value) noexcept
{
  const auto v = xAxis() * -value[0] + yAxis() * -value[1];
  auto matrix = makeTranslationMatrix(v[0], v[1], v[2]);
  transform(matrix);
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 CameraModel::translateVertically(const Vector2& value) noexcept
{
  const auto v = normal() * (value[0] + value[1]);
  const auto matrix = makeTranslationMatrix(v[0], v[1], v[2]);
  transform(matrix);
  return matrix;
}

/*!
  \details
  No detailed.
  */
void CameraModel::initialize(const QJsonObject& settings) noexcept
{
  is_jittering_enabled_ = boolValue(settings, keyword::jittering);
}

/*!
  \details
  No detailed.
  */
UniquePointer<CameraModel> makeCameraModel(const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  CameraModel* camera_model = nullptr;

  const auto camera_type = stringValue(settings, keyword::cameraType);
  switch (keyword::toHash32(camera_type)) {
    case toHash32(keyword::pinholeCamera): {
      camera_model = new PinholeCamera{settings};
      break;
    }
//   case toHash32(keyword::thin_lens_camera_model):
//    camera_model = new ThinLensCameraModel{system, settings, prefix, film};
//    break;
    default:
      zisc::raiseError("CameraModelError: Unsupported type is specified.");
  }
  return UniquePointer<CameraModel>{camera_model};
}

} // namespace nanairo
