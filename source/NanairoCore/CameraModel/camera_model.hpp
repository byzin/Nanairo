/*!
  \file camera_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CAMERA_MODEL_HPP
#define NANAIRO_CAMERA_MODEL_HPP

// Standard C++ library
#include <memory>
#include <array>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/memory_pool.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward decralation
class Film;
class SampledSpectra;
class Sampler;
class ShaderModel;
class WavelengthSamples;

//! \addtogroup Core 
//! \{

/*!
  \details
  No detailed.
  */
enum class CameraType : uint32
{
  kPinhole                     = zisc::toHash32("Pinhole"),
  kThinLens                    = zisc::toHash32("ThinLens")
};

/*!
  \details
  No detailed.
  */
class CameraModel
{
 public:
  using ShaderPointer = UniquePointer<ShaderModel>;


  //! Initialize the camera
  CameraModel(const SettingNodeBase* settings) noexcept;

  //! Finalize the camera
  virtual ~CameraModel() noexcept;


  //! Add ray radiance to film
  void addContribution(const Index2d& index,
                       const SampledSpectra& contribution) noexcept;

  //! Calculate the pdf
  virtual Float calcPdf(const Vector3& vout) const noexcept = 0;

  //! Get the pixel location of the film
  virtual bool calcPixelLocation(const Vector3& ray_direction, 
                                 Index2d* index) const noexcept = 0;

  //! Calculate the radiance
  virtual Float calcRadiance(const Vector3& vout) const noexcept = 0;

  //! Calculate the radiance and pdf
  virtual std::array<Float, 2> calcRadianceAndPdf(const Vector3& vout)
      const noexcept = 0;

  //! Return the film
  Film& film() noexcept;

  //! Return the film
  const Film& film() const noexcept;

  //! Return the normal of the camera
  virtual Vector3 getNormal(const Index2d& index) const noexcept = 0;

  //! Make a sensor
  ShaderPointer makeSensor(const Index2d& index,
                           const WavelengthSamples& wavelengths,
                           zisc::MemoryPool& memory_pool) const noexcept;

  //! Return the height resolution of the film
  uint heightResolution() const noexcept;

  //! Return the image resolution
  const Index2d& imageResolution() const noexcept;

  //! Jitter the point on the pixel
  void jitter(Sampler& sampler) noexcept;

  //! Return the jittering
  const Vector2& jittering() const noexcept;

  //! Make a camera
  static std::unique_ptr<CameraModel> makeCamera(const SettingNodeBase* settings)
      noexcept;

  //! Return the camera position
  virtual const Point3& position() const noexcept = 0;

  //! Rotate the camera
  Matrix4x4 rotate(const Vector2& value) noexcept;

  //! Translate the camera horizontally
  Matrix4x4 translateHorizontally(const Vector2& value) noexcept;

  //! Translate the camera vertically
  Matrix4x4 translateVertically(const Vector2& value) noexcept;

  //! Sample ray direction
  virtual SampledDirection sampleDirection(const Index2d& index) const noexcept = 0;

  //! Return the sampled point
  virtual const Point3& sampledLensPoint() const noexcept = 0;

  //! Sample lens point
  virtual void sampleLensPoint(Sampler& sampler) noexcept = 0;

  //! Set a film
  void setFilm(Film* film) noexcept;

  //! Apply affine transformation
  virtual void transform(const Matrix4x4& matrix) noexcept = 0;

  //! Return the camera type
  virtual CameraType type() const noexcept = 0;

  //! Return the width resolution of the film
  uint widthResolution() const noexcept;

  //! Return the X axis vector of the camera coordinate
  virtual Vector3 xAxis() const noexcept = 0;

  //! Return the Y axis vector of the camera coordinate
  virtual Vector3 yAxis() const noexcept = 0;

  //! Return the Z axis vector of the camera coordinate
  virtual Vector3 zAxis() const noexcept = 0;

 private:
  //! Initialize
  void initialize(const SettingNodeBase* settings) noexcept;

  //! Initialize camera film
  virtual void initFilmShape() noexcept = 0;


  Film* film_;
  Vector2 jittering_;
  bool is_jittering_enabled_;
};

//! \} Core

} // namespace nanairo

#include "camera_model-inl.hpp"

#endif // NANAIRO_CAMERA_MODEL_HPP
