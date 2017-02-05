///*!
//  \file thin_lens_camera.cpp
//  \author zin
//  */
//
//#include "thin_lens_camera.hpp"
//// Standard C++ library
//#include <cstddef>
//#include <string>
//// Qt
//#include <QSettings>
//#include <QString>
//// Zisc
//#include "zisc/math.hpp"
//#include "zisc/utility.hpp"
//#include "zisc/unit.hpp"
//// Reflect
//#include "camera.hpp"
//#include "keyword.hpp"
//#include "reflect_config.hpp"
//#include "Color/spectral_distribution.hpp"
//#include "Data/ray.hpp"
//#include "Data/wavelength_samples.hpp"
//#include "Geometry/point.hpp"
//#include "Geometry/transformation.hpp"
//#include "Geometry/vector.hpp"
//#include "RendererBase/color_system.hpp"
//#include "Sampling/sampled_spectra.hpp"
//#include "Sampling/sampler.hpp"
//#include "Utility/floating_point.hpp"
//#include "Utility/size.hpp"
//#include "Utility/value.hpp"
//
//namespace reflect {
//
//using zisc::cast;
//
///*!
//  \details
//  No detailed.
//  */
//ThinLensCamera::ThinLensCamera(const ColorSystem& color_system,
//                               const QSettings& settings,
//                               const QString& prefix,
//                               Film* film) :
//    Camera(color_system, settings, prefix, film),
//    lens_center_{0.0, 0.0, 0.0}
//{
//  initialize(settings, prefix);
//}
//
///*!
//  \details
//  No detailed.
//  */
//Ray ThinLensCamera::generateRay(const std::size_t x,
//                                const std::size_t y,
//                                const WavelengthSamples& wavelengths,
//                                Sampler& sampler) const
//{
//  // Film point
//  const auto coordinate = film().sampleCoordinate(x, y, sampler);
//  const auto pixel_point = film_position_ + coordinate[0] * film_axis1_ +
//                                            coordinate[1] * film_axis2_;
//  // Lens point
//  const double theta = 2.0 * zisc::kPi * sampler.sample(0.0, 1.0);
//  const double r = std::sqrt(sampler.sample(0.0, 1.0));
//  const double lens_x = r * std::cos(theta);
//  const double lens_y = r * std::sin(theta);
//  const auto lens_point = lens_center_ + lens_x * lens_axis1_ + 
//                                         lens_y * lens_axis2_;
//
//  const auto direction = (pixel_point - lens_point).normalized();
//  return Ray{sensorResponse().distribution(wavelengths),
//             lens_point, 
//             direction};
//}
//
///*!
//  \details
//  No detailed.
//  */
//std::string ThinLensCamera::getClBuildOptions() const
//{
//  const std::string camera_definition = 
//      " -D REFLECT_CAMERA_TYPE=" + 
//      std::to_string(cast<int>(CameraType::kThinLensCamera));
////  const std::string position_definition = 
////      " -D REFLECT_PINHOLE_POSITION_X=" + std::to_string(pinhole_position_[0]) +
////      " -D REFLECT_PINHOLE_POSITION_Y=" + std::to_string(pinhole_position_[1]) +
////      " -D REFLECT_PINHOLE_POSITION_Z=" + std::to_string(pinhole_position_[2]);
////  const std::string film_position_definition =
////      " -D REFLECT_FILM_POSITION_X=" + std::to_string(film_position_[0]) +
////      " -D REFLECT_FILM_POSITION_Y=" + std::to_string(film_position_[1]) +
////      " -D REFLECT_FILM_POSITION_Z=" + std::to_string(film_position_[2]);
////  const std::string film_axis1_definition =
////      " -D REFLECT_FILM_AXIS1_X=" + std::to_string(film_axis1_[0]) +
////      " -D REFLECT_FILM_AXIS1_Y=" + std::to_string(film_axis1_[1]) +
////      " -D REFLECT_FILM_AXIS1_Z=" + std::to_string(film_axis1_[2]);
////  const std::string film_axis2_definition =
////      " -D REFLECT_FILM_AXIS2_X=" + std::to_string(film_axis2_[0]) +
////      " -D REFLECT_FILM_AXIS2_Y=" + std::to_string(film_axis2_[1]) +
////      " -D REFLECT_FILM_AXIS2_Z=" + std::to_string(film_axis2_[2]);
////  return position_definition + film_position_definition +
////         film_axis1_definition + film_axis2_definition;
//  return "";
//}
//
//
//
///*!
//  \details
//  No detailed.
//  */
//void ThinLensCamera::initialize(const QSettings& settings, const QString& prefix)
//{
//  // Get lens camera parameter
//  auto key = prefix + "/" + keyword::aperture;
//  const double aperture = value<double>(settings, key);
//  key = prefix + "/" + keyword::focal_distance;
//  const double focal_distance = value<double>(settings, key);
//
//  // Initialize film position
//  key = prefix + "/" + keyword::angle_of_view;
//  const double angle_of_view = value<double>(settings, key);
//  const double theta = zisc::toRadian(angle_of_view * 0.5);
//  const auto distance = Vector3{0.0, 0.0, 1.0} * (zisc::invert(std::tan(theta)));
//
//  const auto d = distance * (focal_distance  / (distance.norm() - focal_distance));
//  const double y = (d.norm() - focal_distance) / focal_distance;
//
//  const double phi = std::atan(cast<double>(heightResolution()) /
//                               cast<double>(widthResolution()));
//  film_axis1_ = y * Vector3{-1.0, 0.0, 0.0} * (2.0 * std::cos(phi));
//  film_axis2_ = y * Vector3{0.0, -1.0, 0.0} * (2.0 * std::sin(phi));
//  film_position_ = lens_center_ + d - 0.5 * (film_axis1_ + film_axis2_);
//
//  // Initialize lens
//  lens_axis1_ = 0.5 * aperture * Vector3{-1.0, 0.0, 0.0};
//  lens_axis2_ = 0.5 * aperture * Vector3{0.0, -1.0, 0.0};
//}
//
///*!
//  \details
//  No detailed.
//  */
//std::size_t ThinLensCamera::printMemberData()
//{
//  std::size_t size = 0;
//  size += Camera::printMemberData();
//  size += PRINT_VARIABLE_DATA(lens_center_);
//  size += PRINT_VARIABLE_DATA(lens_axis1_);
//  size += PRINT_VARIABLE_DATA(lens_axis2_);
//  size += PRINT_VARIABLE_DATA(film_position_);
//  size += PRINT_VARIABLE_DATA(film_axis1_);
//  size += PRINT_VARIABLE_DATA(film_axis2_);
//  return size;
//}
//
///*!
//  \details
//  No detailed.
//  */
//void ThinLensCamera::transform(const Matrix4x4& matrix)
//{
//  affineTransform(matrix, &lens_center_);
//  affineTransform(matrix, &lens_axis1_);
//  affineTransform(matrix, &lens_axis2_);
//  affineTransform(matrix, &film_position_);
//  affineTransform(matrix, &film_axis1_);
//  affineTransform(matrix, &film_axis2_);
//}
//
//} // namespace reflect
