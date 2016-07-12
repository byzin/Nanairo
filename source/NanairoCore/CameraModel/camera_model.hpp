/*!
  \file camera_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CAMERA_MODEL_HPP
#define NANAIRO_CAMERA_MODEL_HPP

// Standard C++ library
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward decralation
class QString;

namespace nanairo {

// Forward decralation
class Film;
template <uint> class SampledSpectra;
class Sampler;
class SceneSettings;
template <uint> class ShaderModel;
template <uint> class WavelengthSamples;

//! \addtogroup Core 
//! \{

/*!
  \details
  No detailed.
  */
enum class CameraType : int
{
  Pinhole = 0,
  ThinLens
};

/*!
  \details
  No detailed.
  */
class CameraModel
{
 public:
  template <uint kSampleSize>
  using ShaderPointer = UniquePointer<ShaderModel<kSampleSize>>;


  //! Initialize the camera
  CameraModel(const SceneSettings& settings, const QString& prefix) noexcept;

  //! Finalize the camera
  virtual ~CameraModel() noexcept;


  //! Add ray radiance to film
  template <uint kSampleSize>
  void addContribution(const uint x, 
                       const uint y, 
                       const SampledSpectra<kSampleSize>& contribution) noexcept;

  //! Calculate the pdf
  virtual Float calcPdf(const Vector3& vout) const noexcept = 0;

  //! Calculate the radiance
  virtual Float calcRadiance(const Vector3& vout) const noexcept = 0;

  //! Calculate the radiance and pdf
  virtual std::tuple<Float, Float> calcRadianceAndPdf(const Vector3& vout) const noexcept = 0;

  //! Return the film
  Film& film() noexcept;

  //! Return the film
  const Film& film() const noexcept;

  //! Get the pixel location of the film
  virtual bool getPixelLocation(const Vector3& ray_direction, 
                                uint* x, 
                                uint* y) const noexcept = 0;

  //! Make a sensor
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeSensor(
      const uint x,
      const uint y,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const noexcept;

  //! Return the camera normal
  const Vector3& normal() const noexcept;

  //! Return the height resolution of the film
  uint heightResolution() const noexcept;

  //! Jitter the point on the pixel
  void jitter(Sampler& sampler) noexcept;

  //! Return the jittering
  const Vector2& jittering() const noexcept;

  //! Return the camera position
  virtual const Point3& position() const noexcept = 0;

  //! Rotate the camera
  Matrix4x4 distance(const Vector2& value) noexcept;

  //! Rotate the camera
  Matrix4x4 rotate(const Vector2& value) noexcept;

  //! Rotate the camera
  Matrix4x4 translate(const Vector2& value) noexcept;

  //! Return the sampled point
  const Point3& sampledLensPoint() const noexcept;

  //! Sample lens point
  virtual void sampleLensPoint(Sampler& sampler) noexcept = 0;

  //! Sample ray direction
  virtual SampledDirection sampleDirection(const uint x, const uint y) const noexcept = 0;

  //! Set a film
  void setFilm(Film* film) noexcept;

  //! Apply affine transformation
  virtual void transform(const Matrix4x4& matrix) noexcept = 0;

  //! Return the camera type
  virtual CameraType type() const noexcept = 0;

  //! Return the width resolution of the film
  uint widthResolution() const noexcept;

  //! Return the X axis vector
  virtual Vector3 xAxis() const noexcept = 0;

  //! Return the Y axis vector
  virtual Vector3 yAxis() const noexcept = 0;

 protected:
  //! Set normal
  void setNormal(const Vector3& normal) noexcept;

  //! Set sampled lens point
  void setSampledLensPoint(const Point3& sampled_point) noexcept;

 private:
  //! Initialize
  void initialize(const SceneSettings& settings, const QString& prefix) noexcept;

  //! Initialize camera film
  virtual void initializeFilm() noexcept = 0;


  Film* film_;
  Vector3 normal_;
  Point3 sampled_lens_point_;
  Vector2 jittering_;
  bool is_jittering_enabled_;
};

//! Make a camera
UniquePointer<CameraModel> makeCameraModel(const SceneSettings& settings, 
                                           const QString& prefix) noexcept;

//! \} Core

} // namespace nanairo

#include "camera_model-inl.hpp"

#endif // NANAIRO_CAMERA_MODEL_HPP
