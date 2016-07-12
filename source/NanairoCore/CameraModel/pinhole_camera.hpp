/*!
  \file pinhole_camera.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PINHOLE_CAMERA_HPP
#define NANAIRO_PINHOLE_CAMERA_HPP

// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "camera_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class SampledDirection;
class Sampler;
class SceneSettings;

//! \addtogroup Core 
//! \{

/*!
 \details
 No detailed.
 */
class PinholeCamera : public CameraModel
{
 public:
  //! Create a pinhole camera
  PinholeCamera(const SceneSettings& settings, const QString& prefix) noexcept;


  //! Calculate the pdf
  Float calcPdf(const Vector3& vout) const noexcept override;

  //! Calculate the radiance
  Float calcRadiance(const Vector3& vout) const noexcept override;

  //! Calculate the radiance and pdf
  std::tuple<Float, Float> calcRadianceAndPdf(const Vector3& vout) const noexcept override;

  //! Get the pixel location of the film
  bool getPixelLocation(const Vector3& ray_direction, 
                        uint* x, 
                        uint* y) const noexcept override;

  //! Return the camera position
  const Point3& position() const noexcept override;

  //! Sample ray direction
  SampledDirection sampleDirection(const uint x, const uint y) const noexcept override;

  //! Sample pinhole point
  void sampleLensPoint(Sampler& sampler) noexcept override;

  //! Apply affine transformation
  void transform(const Matrix4x4& matrix) noexcept override;

  //! Return the pinhole camera type
  CameraType type() const noexcept override;

  //! Return the x axis vector
  Vector3 xAxis() const noexcept override;

  //! Return the y axis vector
  Vector3 yAxis() const noexcept override;

 private:
  //! Calculate the inverse pdf
  Float calcInversePdf(const Float cos_theta) const noexcept;

  //! Return the film area
  Float filmArea() const noexcept;

  //! Initialize
  void initialize(const SceneSettings& settings, const QString& prefix) noexcept;

  //! Initialize camera film
  void initializeFilm() noexcept override;


  Point3 pinhole_position_; //!< The position of pinhole
  Point3 film_position_; //!< Center point of film
  Vector3 film_axis1_,
          film_axis2_;
  Float square_axis1_,
        square_axis2_,
        inverse_square_axis1_,
        inverse_square_axis2_;
  Float film_area_;
  Float angle_of_view_;
};

//! \} Core 

} // namespace nanairo

#endif // NANAIRO_PINHOLE_CAMERA_HPP
