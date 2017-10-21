/*!
  \file camera_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "camera_model.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "pinhole_camera.hpp"
//#include "thin_lens_camera_model.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Setting/camera_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
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
CameraModel::CameraModel(const SettingNodeBase* settings) noexcept
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
  auto matrix = Transformation::makeTranslation(-p[0], -p[1], -p[2]);
  {
    const auto rotation_matrix = (zisc::abs(value[0]) < zisc::abs(value[1]))
        ? Transformation::makeXAxisRotation(-value[1])
        : Transformation::makeZAxisRotation(value[0]);
    matrix = rotation_matrix * matrix;
  }
  matrix = Transformation::makeTranslation(p[0], p[1], p[2]) * matrix;
  transform(matrix);
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 CameraModel::translateHorizontally(const Vector2& value) noexcept
{
  const auto v = -value[0] * xAxis() + value[1] * yAxis();
  auto matrix = Transformation::makeTranslation(v[0], v[1], v[2]);
  transform(matrix);
  return matrix;
}

/*!
  \details
  No detailed.
  */
Matrix4x4 CameraModel::translateVertically(const Vector2& value) noexcept
{
  const auto v = (value[0] + value[1]) * zAxis();
  const auto matrix = Transformation::makeTranslation(v[0], v[1], v[2]);
  transform(matrix);
  return matrix;
}

/*!
  \details
  No detailed.
  */
void CameraModel::initialize(const SettingNodeBase* settings) noexcept
{
  const auto camera_settings = castNode<CameraSettingNode>(settings);

  {
    is_jittering_enabled_ = camera_settings->jittering();
  }
}

/*!
  \details
  No detailed.
  */
std::unique_ptr<CameraModel> CameraModel::makeCamera(
    const SettingNodeBase* settings) noexcept
{
  const auto camera_settings = castNode<CameraSettingNode>(settings);

  std::unique_ptr<CameraModel> camera_model;
  switch (camera_settings->cameraType()) {
   case CameraType::kPinhole: {
    camera_model = std::make_unique<PinholeCamera>(settings);
    break;
   }
   case CameraType::kThinLens: {
    zisc::raiseError("Not implemented");
//  ase toHash32(keyword::thin_lens_camera_model):
//  camera_model = new ThinLensCameraModel{system, settings, prefix, film};
    break;
   }
   default: {
    zisc::raiseError("CameraModelError: Unsupported type is specified.");
    break;
   }
  }
  return camera_model; 
}

} // namespace nanairo
