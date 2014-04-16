///*!
//  \file thin_lens_camera.hpp
//  \author zin
//  */
//
//#ifndef _REFLECT_THIN_LENS_CAMERA_HPP_
//#define _REFLECT_THIN_LENS_CAMERA_HPP_
//
//// Standard C++ library
//#include <cstddef>
//#include <string>
//// Reflect
//#include "camera.hpp"
//#include "LinearAlgebra/point.hpp"
//#include "LinearAlgebra/vector.hpp"
//
//namespace reflect {
//
//// Forward declaration
//class ColorSystem;
//class Film;
//class Sampler;
//class WavelengthSamples;
//
////! \addtogroup Camera
////! \{
//
///*!
//  \details
//  No detailed.
//  */
//class ThinLensCamera : public Camera
//{
// public:
//  //! Create a lens camera
//  ThinLensCamera(const ColorSystem& color_system,
//                 const QSettings& settings, 
//                 const QString& prefix, 
//                 Film* film);
//
//
//  //! Generate ray
//  Ray generateRay(const std::size_t x,
//                  const std::size_t y,
//                  const WavelengthSamples& wavelengths,
//                  Sampler& sampler) const override;
//
//  //! Return the OpenCL build options for the thin lens camera
//  std::string getClBuildOptions() const override;
//
//  //! Print member variable data
//  static std::size_t printMemberData();
//
//  //! Apply affine transformation
//  void transform(const Matrix4x4& matrix) override;
//
// private:
//  //! Initialize
//  void initialize(const QSettings& settings, const QString& prefix);
//
//
//  Point3 lens_center_;
//  Vector3 lens_axis1_,
//          lens_axis2_;
//  Point3 film_position_;
//  Vector3 film_axis1_,
//          film_axis2_;
//};
//
////! \} Camera
//
//} // namespace reflect
//
//#endif // _REFLECT_THIN_LENS_CAMERA_HPP_
