/*!
  \file camera_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_CAMERA_MODEL_HPP_
#define _NANAIRO_CAMERA_MODEL_HPP_

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
  CameraModel(const SceneSettings& settings, const QString& prefix);

  //! Finalize the camera
  virtual ~CameraModel() {}


  //! Add ray radiance to film
  template <uint kSampleSize>
  void addContribution(const uint x, 
                       const uint y, 
                       const SampledSpectra<kSampleSize>& contribution);

  //! Calculate the pdf
  virtual Float calcPdf(const Vector3& vout) const = 0;

  //! Calculate the radiance
  virtual Float calcRadiance(const Vector3& vout) const = 0;

  //! Calculate the radiance and pdf
  virtual std::tuple<Float, Float> calcRadianceAndPdf(const Vector3& vout) const = 0;

  //! Return the film
  Film& film();

  //! Return the film
  const Film& film() const;

  //! Get the pixel location of the film
  virtual bool getPixelLocation(const Vector3& ray_direction, 
                                uint* x, 
                                uint* y) const = 0;

  //! Make a sensor
  template <uint kSampleSize>
  ShaderPointer<kSampleSize> makeSensor(
      const uint x,
      const uint y,
      const WavelengthSamples<kSampleSize>& wavelengths,
      MemoryPool& memory_pool) const;

  //! Return the camera normal
  const Vector3& normal() const;

  //! Return the height resolution of the film
  uint heightResolution() const;

  //! Jitter the point on the pixel
  void jitter(Sampler& sampler);

  //! Return the jittering
  const Vector2& jittering() const;

  //! Return the camera position
  virtual const Point3& position() const = 0;

  //! Rotate the camera
  Matrix4x4 distance(const Vector2& value);

  //! Rotate the camera
  Matrix4x4 rotate(const Vector2& value);

  //! Rotate the camera
  Matrix4x4 translate(const Vector2& value);

  //! Return the sampled point
  const Point3& sampledLensPoint() const;

  //! Sample lens point
  virtual void sampleLensPoint(Sampler& sampler) = 0;

  //! Sample ray direction
  virtual SampledDirection sampleDirection(const uint x, const uint y) const = 0;

  //! Set a film
  void setFilm(Film* film);

  //! Apply affine transformation
  virtual void transform(const Matrix4x4& matrix) = 0;

  //! Return the camera type
  virtual CameraType type() const = 0;

  //! Return the width resolution of the film
  uint widthResolution() const;

  //! Return the X axis vector
  virtual Vector3 xAxis() const = 0;

  //! Return the Y axis vector
  virtual Vector3 yAxis() const = 0;

 protected:
  //! Set normal
  void setNormal(const Vector3& normal);

  //! Set sampled lens point
  void setSampledLensPoint(const Point3& sampled_point);

 private:
  //! Initialize
  void initialize(const SceneSettings& settings, const QString& prefix);

  //! Initialize camera film
  virtual void initializeFilm() = 0;


  Film* film_;
  Vector3 normal_;
  Point3 sampled_lens_point_;
  Vector2 jittering_;
  bool is_jittering_enabled_;
};

//! Make a camera
UniquePointer<CameraModel> makeCameraModel(const SceneSettings& settings, 
                                           const QString& prefix);

//! \} Core

} // namespace nanairo

#include "camera_model-inl.hpp"

#endif // _NANAIRO_CAMERA_MODEL_HPP_
