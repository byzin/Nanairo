/*!
  \file sensor.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SENSOR_HPP
#define NANAIRO_SENSOR_HPP

// Standard C++ library
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"

namespace nanairo {

// Forward declaration
class CameraModel;
template <uint> class SampledSpectra;
class Sampler;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class Sensor : public GlossyShaderModel<kSampleSize>
{
 public:
  using Spectra = typename ShaderModel<kSampleSize>::Spectra;
  using Wavelengths = typename ShaderModel<kSampleSize>::Wavelengths;

   
  //! Create a Camera sensor
  Sensor(const CameraModel* camera, const uint x, const uint y) noexcept;


  //! Evaluate the BRDF of the area sampling
  Float evaluatePdf(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const Wavelengths& wavelengths) const noexcept override;

  //! Evaluate the BRDF of the area sampling
  Spectra evaluateRadiance(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const Wavelengths& wavelengths) const noexcept override;

  //! Evaluate the BRDF and pdf
  std::tuple<Spectra, Float> evaluateRadianceAndPdf(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const Wavelengths& wavelengths) const noexcept override;

  //! Sample a camera ray direction and evaluate a ray weight
  std::tuple<SampledDirection, Spectra> sample(
      const Vector3* vin,
      const Vector3& normal, 
      const Wavelengths& wavelengths,
      Sampler& sampler) const noexcept override;

  //! Check is wavelength selection occured
  bool wavelengthIsSelected() const noexcept override;

 private:
  //! Return the camera
  const CameraModel& camera() const noexcept;

  //! Initialize sensor
  void initialize(const uint x, const uint y) noexcept;


  const CameraModel* camera_;
  uint x_,
       y_;
};

//! \} Core

} // namespace nanairo

#include "sensor-inl.hpp"

#endif // NANAIRO_SENSOR_HPP
