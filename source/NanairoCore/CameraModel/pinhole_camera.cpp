/*!
  \file pinhole_camera.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "pinhole_camera.hpp"
// Standard C++ library
#include <cmath>
#include <tuple>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/unit.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "camera_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
PinholeCamera::PinholeCamera(const QJsonObject& settings) noexcept :
    CameraModel(settings),
    pinhole_position_{0.0, 0.0, 0.0},
    film_position_{0.0, 0.0, -1.0},
    film_axis1_{-1.0, 0.0, 0.0},
    film_axis2_{0.0, -1.0, 0.0}
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
Float PinholeCamera::calcPdf(const Vector3& vout) const noexcept
{
  const Float cos_theta_no = zisc::dot(normal(), vout);
  const Float pdf = zisc::invert(filmArea() * zisc::power<3>(cos_theta_no));
  ZISC_ASSERT(0.0 < pdf, "The pdf is minus.");
  return pdf;
}

/*!
  */
Float PinholeCamera::calcRadiance(const Vector3& vout) const noexcept
{
  const Float cos_theta_no = zisc::dot(normal(), vout);
  const Float f = zisc::invert(filmArea() * zisc::power<4>(cos_theta_no));
  ZISC_ASSERT(0.0 < f, "The radiance is minus.");
  return f;
}

/*!
  */
std::tuple<Float, Float> PinholeCamera::calcRadianceAndPdf(
    const Vector3& vout) const noexcept
{
  const Float cos_theta_no = zisc::dot(normal(), vout);
  const Float f = zisc::invert(filmArea() * zisc::power<4>(cos_theta_no));
  const Float pdf = f * cos_theta_no;
  ZISC_ASSERT(0.0 < f, "The radiance is minus.");
  ZISC_ASSERT(0.0 < pdf, "The pdf is minus.");
  return std::make_tuple(f, pdf);
}

/*!
  \details
  No detailed.
  */
bool PinholeCamera::getPixelLocation(const Vector3& ray_direction, 
                                     uint* x, 
                                     uint* y) const noexcept
{
  using zisc::cast;

  const Float cos_theta = zisc::dot(normal(), ray_direction);
  if (cos_theta == 0.0)
    return false;
  const auto& lens_point = sampledLensPoint();
  const auto film_top_left = film_position_ - 0.5 * (film_axis1_ + film_axis2_);
  const Float t = zisc::dot(normal(), film_top_left - lens_point) / cos_theta;
  if (t < 0.0)
    return false;

  const auto hit_point = lens_point + (t * ray_direction);
  const auto am = hit_point - film_top_left;
  const Float dot_axis1_am = zisc::dot(am, film_axis1_);
  const Float dot_axis2_am = zisc::dot(am, film_axis2_);
  const bool is_hit = zisc::isInClosedBounds(dot_axis1_am, 0.0, square_axis1_) &&
                      zisc::isInClosedBounds(dot_axis2_am, 0.0, square_axis2_);
  if (is_hit) {
    const Float pixel_x = (1.0 - dot_axis1_am * inverse_square_axis1_);
    const Float pixel_y = (1.0 - dot_axis2_am * inverse_square_axis2_);
    ZISC_ASSERT(zisc::isInBounds(pixel_x, 0.0, 1.0),
                "Pixel x is out of film range [0.0, 1.0)");
    ZISC_ASSERT(zisc::isInBounds(pixel_y, 0.0, 1.0),
                "Pixel y is out of film range [0.0, 1.0)");
    const auto width = cast<Float>(widthResolution());
    const auto height = cast<Float>(heightResolution());
    *x = cast<uint>(pixel_x * width);
    *y = cast<uint>(pixel_y * height);
  }
  return is_hit;
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
SampledDirection PinholeCamera::sampleDirection(const uint x, const uint y) const noexcept
{
  using zisc::cast;

  // Pinhole point
  const auto& pinhole_point = sampledLensPoint();
  // Film point
  const auto p = film().coordinate(x, y, jittering());
  const auto film_point = film_position_ +
                          p[0] * film_axis1_ +
                          p[1] * film_axis2_;
  // Ray direction
  const auto direction = (pinhole_point - film_point).normalized();

  const Float cos_theta = zisc::dot(normal(), direction);
  ZISC_ASSERT(zisc::isInClosedBounds(cos_theta, 0.0, 1.0),
              "Invalid direction is sampled.");
  return SampledDirection{direction, calcInversePdf(cos_theta)};
}

/*!
  \details
  No detailed.
  */
void PinholeCamera::sampleLensPoint(Sampler& /* sampler */) noexcept
{
  setSampledLensPoint(pinhole_position_);
}

/*!
  \details
  No detailed.
  */
CameraType PinholeCamera::type() const noexcept
{
  return CameraType::Pinhole;
}

/*!
  \details
  No detailed.
  */
inline
Float PinholeCamera::calcInversePdf(const Float cos_theta) const noexcept
{
  return filmArea() * zisc::power<3>(cos_theta);
}

/*!
  \details
  No detailed.
  */
inline
Float PinholeCamera::filmArea() const noexcept
{
  return film_area_;
}

/*!
  \details
  No detailed.
  */
void PinholeCamera::initialize(const QJsonObject& settings) noexcept
{
  const Float angle = SceneValue::toFloat<Float>(settings, keyword::angleOfView);
  angle_of_view_ = zisc::toRadian(angle);
  ZISC_ASSERT(zisc::isInBounds(angle_of_view_, 0.0, zisc::kPi<Float>),
              "The angle of view is out of the range [0, pi).");
  setNormal((pinhole_position_ - film_position_).normalized());
}

/*!
  \details
  No detailed.
  */
void PinholeCamera::initializeFilm() noexcept
{
  const Float theta = angle_of_view_ * 0.5;
  const Float r = std::tan(theta);

  const Float phi = std::atan(film().inverseAspectRatio());

  const Float w = 2.0 * r * std::cos(phi);
  const Float h = 2.0 * r * std::sin(phi);

  film_axis1_ = w * film_axis1_.normalized();
  film_axis2_ = h * film_axis2_.normalized();
  square_axis1_ = film_axis1_.squareNorm();
  square_axis2_ = film_axis2_.squareNorm();
  inverse_square_axis1_ = zisc::invert(square_axis1_);
  inverse_square_axis2_ = zisc::invert(square_axis2_);

  film_area_ = w * h;
}

/*!
 \details
 No detailed.
 */
void PinholeCamera::transform(const Matrix4x4& matrix) noexcept
{
  Transformation::affineTransform(matrix, &pinhole_position_);
  Transformation::affineTransform(matrix, &film_position_);
  Transformation::affineTransform(matrix, &film_axis1_);
  Transformation::affineTransform(matrix, &film_axis2_);

  square_axis1_ = film_axis1_.squareNorm();
  square_axis2_ = film_axis2_.squareNorm();
  inverse_square_axis1_ = zisc::invert(square_axis1_);
  inverse_square_axis2_ = zisc::invert(square_axis2_);

  setNormal((pinhole_position_ - film_position_).normalized());
  film_position_ = pinhole_position_ - normal();
}

/*!
  \details
  No detailed.
  */
Vector3 PinholeCamera::xAxis() const noexcept
{
  return film_axis1_.normalized();
}

/*!
  \details
  No detailed.
  */
Vector3 PinholeCamera::yAxis() const noexcept
{
  return film_axis2_.normalized();
}

} // namespace nanairo
