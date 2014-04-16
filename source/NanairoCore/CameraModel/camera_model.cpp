/*!
  \file camera_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "camera_model.hpp"
// Qt
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "pinhole_camera.hpp"
//#include "thin_lens_camera_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class Film;

/*!
  \details
  No detailed.
  */
CameraModel::CameraModel(const SceneSettings& settings, 
                         const QString& prefix) :
    jittering_{0.0, 0.0}
{
  initialize(settings, prefix);
}

/*!
  \details
  No detailed.
  */
Matrix4x4 CameraModel::distance(const Vector2& value)
{
  const auto v = normal() * value[1];

  const auto matrix = makeTranslationMatrix(v[0], v[1], v[2]);

  transform(matrix);
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 CameraModel::rotate(const Vector2& value)
{
  const auto& p = position();

  auto matrix = makeTranslationMatrix(-p[0], -p[1], -p[2]);
  matrix = makeYAxisRotationMatrix(value[0]) * matrix;
  matrix = makeXAxisRotationMatrix(-value[1]) * matrix;
  matrix = makeTranslationMatrix(p[0], p[1], p[2]) * matrix;

  transform(matrix);
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 CameraModel::translate(const Vector2& value)
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
void CameraModel::initialize(const SceneSettings& settings, 
                             const QString& prefix)
{
  const auto key = prefix + "/" + keyword::jittering;
  is_jittering_enabled_ = settings.booleanValue(key);
}

/*!
  \details
  No detailed.
  */
UniquePointer<CameraModel> makeCameraModel(const SceneSettings& settings, 
                                           const QString& prefix)
{
  using zisc::toHash32;

  CameraModel* camera_model = nullptr;

  const auto camera_type = settings.stringValue(prefix + "/" + keyword::cameraType);
  switch (keyword::toHash32(camera_type)) {
   case toHash32(keyword::pinholeCamera):
    camera_model = new PinholeCamera{settings, prefix};
    break;
//   case toHash32(keyword::thin_lens_camera_model):
//    camera_model = new ThinLensCameraModel{system, settings, prefix, film};
//    break;
   default:
    zisc::raiseError("CameraModelError: Unsupported type is specified.");
    break;
  }
  return UniquePointer<CameraModel>{camera_model};
}

} // namespace nanairo
