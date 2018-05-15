/*!
  \file pinhole_camera.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "pinhole_camera.hpp"
// Standard C++ library
#include <array>
#include <cmath>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/unit.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "camera_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Shape/plane.hpp"
#include "NanairoCore/Setting/camera_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
PinholeCamera::PinholeCamera(const SettingNodeBase* settings) noexcept :
    CameraModel(settings),
    pinhole_position_{0.0, 0.0, 0.0},
    film_shape_{Point3{0.0, -1.0, 0.0}}
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
Float PinholeCamera::calcPdf(const Vector3& vout) const noexcept
{
  const auto& shape = filmShape();

  const Float cos_theta_no = zisc::dot(shape.normal(), vout);
  const Float pdf = zisc::invert(shape.surfaceArea() * zisc::power<3>(cos_theta_no));
  ZISC_ASSERT(0.0 < pdf, "The pdf is minus: ", pdf);
  return pdf;
}

/*!
  */
Float PinholeCamera::calcRadiance(const Vector3& vout) const noexcept
{
  const auto& shape = filmShape();

  const Float cos_theta_no = zisc::dot(shape.normal(), vout);
  const Float f = zisc::invert(shape.surfaceArea() * zisc::power<4>(cos_theta_no));
  ZISC_ASSERT(0.0 < f, "The radiance is minus: ", f);
  return f;
}

/*!
  */
std::array<Float, 2> PinholeCamera::calcRadianceAndPdf(const Vector3& vout)
    const noexcept
{
  const auto& shape = filmShape();

  const Float cos_theta_no = zisc::dot(shape.normal(), vout);
  const Float f = zisc::invert(shape.surfaceArea() * zisc::power<4>(cos_theta_no));
  const Float pdf = f * cos_theta_no;
  ZISC_ASSERT(0.0 < f, "The radiance is minus.");
  ZISC_ASSERT(0.0 < pdf, "The pdf is minus.");
  return std::array<Float, 2>{{f, pdf}};
}

/*!
  \details
  No detailed.
  */
bool PinholeCamera::calcPixelLocation(const Vector3& ray_direction, 
                                      Index2d* index) const noexcept
{
  ZISC_ASSERT(index != nullptr, "The index is null.");

  const auto& shape = filmShape();

  const auto ray = Ray::makeRay<false>(sampledLensPoint(), ray_direction);
  Point2 st;
  const bool is_hit = shape.testIntersection(ray, &st);
  if (is_hit) {
    const auto w = zisc::cast<Float>(widthResolution());
    const auto h = zisc::cast<Float>(heightResolution());
    (*index)[0] = zisc::cast<uint32>(st[0] * w);
    (*index)[1] = zisc::cast<uint32>(st[1] * h);
  }
  return is_hit;
}

/*!
  */
Vector3 PinholeCamera::getNormal(const Index2d& /* index */) const noexcept
{
  const auto& shape = filmShape();
  return shape.normal();
}

/*!
  \details
  No detailed.
  */
const Point3& PinholeCamera::position() const noexcept
{
  return pinhole_position_;
}

/*!
  \details
  No detailed.
  */
SampledDirection PinholeCamera::sampleDirection(const Index2d& index) const noexcept
{
  // Pinhole point
  const auto& pinhole_point = sampledLensPoint();
  // Film point
  const auto& shape = filmShape();
  const auto& e = shape.edge();
  const auto st = film().coordinate(index, jittering());
  const auto film_point = shape.vertex0() + (st[0] * e[0] + st[1] * e[1]);
  // Ray direction
  const auto direction = (pinhole_point - film_point).normalized();

  const Float cos_theta = zisc::dot(shape.normal(), direction);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_theta, 0.0, 1.0),
              "Invalid direction is sampled.");
  return SampledDirection{direction, calcInversePdf(cos_theta)};
}

/*!
  */
const Point3& PinholeCamera::sampledLensPoint() const noexcept
{
  return pinhole_position_;
}

/*!
  \details
  No detailed.
  */
void PinholeCamera::sampleLensPoint(Sampler& /* sampler */) noexcept
{
}

/*!
  \details
  No detailed.
  */
CameraType PinholeCamera::type() const noexcept
{
  return CameraType::kPinhole;
}

/*!
 \details
 No detailed.
 */
void PinholeCamera::transform(const Matrix4x4& matrix) noexcept
{
  Transformation::affineTransform(matrix, &pinhole_position_);
  film_shape_.transform(matrix);
  {
    const auto film_center = pinhole_position_ - film_shape_.normal();
    film_shape_.setCenter(film_center);
  }
}

/*!
  \details
  No detailed.
  */
Vector3 PinholeCamera::xAxis() const noexcept
{
  const auto& shape = filmShape();
  const auto& e = shape.edge();
  return e[0].normalized();
}

/*!
  \details
  No detailed.
  */
Vector3 PinholeCamera::yAxis() const noexcept
{
  const auto& shape = filmShape();
  const auto& e = shape.edge();
  return e[1].normalized();
}

/*!
  */
Vector3 PinholeCamera::zAxis() const noexcept
{
  const auto& shape = filmShape();
  return shape.normal();
}

/*!
  \details
  No detailed.
  */
inline
Float PinholeCamera::calcInversePdf(const Float cos_theta) const noexcept
{
  const auto& shape = filmShape();
  const auto inv_pdf = shape.surfaceArea() * zisc::power<3>(cos_theta);
  return inv_pdf;
}

/*!
  */
void PinholeCamera::initFilmShape() noexcept
{
  const Float aspect_ratio = film().aspectRatio();
  const Float angle_of_view = angleOfView();
  film_shape_.initialize(aspect_ratio, angle_of_view);
}

/*!
  \details
  No detailed.
  */
void PinholeCamera::initialize(const SettingNodeBase* settings) noexcept
{
  const auto camera_settings = castNode<CameraSettingNode>(settings);

  const auto& parameters = camera_settings->pinholeCameraParameters();
  {
    angle_of_view_ = zisc::toRadian(parameters.angle_of_view_);
    ZISC_ASSERT(zisc::isInBounds(angle_of_view_, 0.0, zisc::kPi<Float>),
                "The angle of view is out of the range [0, pi).");
  }
}

/*!
  */
PinholeCamera::FilmShape::FilmShape(const Point3& c) noexcept :
    surface_area_{1.0},
    edge_{{Vector3{-1.0, 0.0, 0.0}, Vector3{0.0, 0.0, 1.0}}}
{
  normal_ = calcNormal(edge_[0], edge_[1]);
  setCenter(c);
}

/*!
  \details
  No detailed.
  */
void PinholeCamera::FilmShape::initialize(const Float aspect_ratio,
                                          const Float angle_of_view) noexcept
{
  ZISC_ASSERT(zisc::isInBounds(angle_of_view, 0.0, zisc::kPi<Float>),
              "The angle of view is invalid: ", angle_of_view);

  const Float theta = 0.5 * angle_of_view;
  const Float phi = std::atan(zisc::invert(aspect_ratio));

  const Float r = std::tan(theta);
  const Float w = 2.0 * r * std::cos(phi);
  const Float h = 2.0 * r * std::sin(phi);

  edge_[0] = w * edge_[0].normalized();
  edge_[1] = h * edge_[1].normalized();

  surface_area_ = w * h;
}

/*!
  */
void PinholeCamera::FilmShape::transform(const Matrix4x4& matrix) noexcept
{
  // Scaling is prevented
  Transformation::affineTransform(matrix, &vertex0_);
  {
    std::array<Vector3, 2> e{{edge_[0], edge_[1]}};
    const Float w = e[0].norm();
    const Float h = e[1].norm();
    Transformation::affineTransform(matrix, &e[0]);
    Transformation::affineTransform(matrix, &e[1]);
    normal_ = calcNormal(e[0], e[1]);
    e[1] = zisc::cross(normal_, e[0]);
    edge_[0] = w * e[0].normalized();
    edge_[1] = h * e[1].normalized();
  }
}

} // namespace nanairo
