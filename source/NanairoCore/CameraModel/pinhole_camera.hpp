/*!
  \file pinhole_camera.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PINHOLE_CAMERA_HPP
#define NANAIRO_PINHOLE_CAMERA_HPP

// Standard C++ library
#include <array>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "camera_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class PathState;
class Ray;
class SampledDirection;
class Sampler;

//! \addtogroup Core
//! \{

/*!
 \details
 No detailed.
 */
class PinholeCamera : public CameraModel
{
 public:
  class FilmShape
  {
   public:
    //! Initialize the film
    FilmShape(const Point3& c) noexcept;


    //! Return the center point of the film
    Point3 center() const noexcept;

    //! Return the film axises
    const std::array<Vector3, 2>& edge() const noexcept;

    //! Initialize the film shape
    void initialize(const Float aspect_ratio,
                    const Float angle_of_view) noexcept;

    //! Return the normal of the film
    const Vector3& normal() const noexcept;

    //! Set the center of the film
    void setCenter(const Point3& c) noexcept;

    //! Return the surface area of the front side of the film
    Float surfaceArea() const noexcept;

    //! Test ray-film intersection
    bool testIntersection(const Ray& ray, Point2* st) const noexcept;

    //! Apply affine transformation
    void transform(const Matrix4x4& matrix) noexcept;

    //! Return the vertex0 of the film
    const Point3& vertex0() const noexcept;

   private:
    //! Calculate the normal vector
    Vector3 calcNormal(const Vector3& e1, const Vector3& e2) const noexcept;


    Float surface_area_;
    Point3 vertex0_; //!< The left bottom point of the film
    std::array<Vector3, 2> edge_;
    Vector3 normal_;
  };


  //! Create a pinhole camera
  PinholeCamera(const SettingNodeBase* settings) noexcept;


  //! Return the angle of view
  Float angleOfView() const noexcept;

  //! Calculate the pdf
  Float calcPdf(const Vector3& vout) const noexcept override;

  //! Calculate the pixel location of the film
  bool calcPixelLocation(const Vector3& ray_direction, 
                         Index2d* index) const noexcept override;

  //! Calculate the radiance
  Float calcRadiance(const Vector3& vout) const noexcept override;

  //! Calculate the radiance and pdf
  std::array<Float, 2> calcRadianceAndPdf(const Vector3& vout)
      const noexcept override;

  //! Return the film shape
  const FilmShape& filmShape() const noexcept;

  //! Return the normal of the pinhole camera
  Vector3 getNormal(const Index2d& index) const noexcept override;

  //! Return the camera position
  const Point3& position() const noexcept override;

  //! Sample ray direction
  SampledDirection sampleDirection(const Index2d& index) const noexcept override;

  //! Return the sampled lens point
  const Point3& sampledLensPoint() const noexcept override;

  //! Sample pinhole point
  void sampleLensPoint(Sampler& sampler,
                       const PathState& path_state) noexcept override;

  //! Apply affine transformation
  void transform(const Matrix4x4& matrix) noexcept override;

  //! Return the pinhole camera type
  CameraType type() const noexcept override;

  //! Return the x axis vector of the camera coordinate
  Vector3 xAxis() const noexcept override;

  //! Return the y axis vector of the camera coordinate
  Vector3 yAxis() const noexcept override;

  //! Return the z axis vector of the camera coordinate
  Vector3 zAxis() const noexcept override;

 private:
  //! Calculate the inverse pdf
  Float calcInversePdf(const Float cos_theta) const noexcept;

  //! Initialize the film shape
  void initFilmShape() noexcept override;

  //! Initialize
  void initialize(const SettingNodeBase* settings) noexcept;


  Point3 pinhole_position_; //!< The position of pinhole
  FilmShape film_shape_;
  Float angle_of_view_;
};

//! \} Core

} // namespace nanairo

#include "pinhole_camera-inl.hpp"

#endif // NANAIRO_PINHOLE_CAMERA_HPP
