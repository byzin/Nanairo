/*!
  \file shader_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SHADER_MODEL_HPP
#define NANAIRO_SHADER_MODEL_HPP

// Standard C++ library
#include <cstddef>
#include <tuple>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
//class IntersectionInfo;
class SampledDirection;
template <uint> class SampledSpectra;
class Sampler;
class SurfaceModel;
template <uint> class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
enum class ShaderType : int
{
  Diffuse = 0,
  Specular,
  Glossy
};

/*!
 \brief Shader class
 \details
 No detailed.
  */
template <uint kSampleSize>
class ShaderModel
{
 public:
  using Spectra = SampledSpectra<kSampleSize>;
  using Wavelengths = WavelengthSamples<kSampleSize>;


  //! Create a shader model
  ShaderModel() noexcept;

  //! Finalize the shader model
  virtual ~ShaderModel() noexcept {}


  // Dummy delete function
  static void operator delete(void*) noexcept;

  //! Dummy new function
  static void* operator new(std::size_t, void* memory) noexcept;


  //! Evaluate the pdf
  virtual Float evalPdf(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const Wavelengths& wavelengths) const noexcept;

  //! Evaluate the radiance
  virtual Spectra evalRadiance(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const Wavelengths& wavelengths) const noexcept;

  //! Evaluate the radiance and pdf
  virtual std::tuple<Spectra, Float> evalRadianceAndPdf(
      const Vector3* vin,
      const Vector3* vout,
      const Vector3& normal,
      const Wavelengths& wavelengths) const noexcept;

  //! Sample a reflection direction and evaluate a reflection weight
  virtual std::tuple<SampledDirection, Spectra> sample(
      const Vector3* vin, 
      const Vector3& normal,
      const Wavelengths& wavelengths,
      Sampler& sampler) const noexcept;

  //! Return the shader type
  virtual ShaderType type() const noexcept = 0;

  //! Sample a next direction
//  virtual SampledDirection sampleDirection(Sampler& sampler) const = 0;

  //! Check if wavelength selection occured
  virtual bool wavelengthIsSelected() const noexcept = 0;
};

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class DiffuseShaderModel : public ShaderModel<kSampleSize>
{
 public:
  //! Return the diffuse type
  ShaderType type() const noexcept override
  {
    return ShaderType::Diffuse;
  }
};

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class SpecularShaderModel : public ShaderModel<kSampleSize>
{
 public:
  //! Return the specular type
  ShaderType type() const noexcept override
  {
    return ShaderType::Specular;
  }
};

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class GlossyShaderModel : public ShaderModel<kSampleSize>
{
 public:
  //! Return the glossy type
  ShaderType type() const noexcept override
  {
    return ShaderType::Glossy;
  }
};

//! \} Core

} // namespace nanairo

#include "shader_model-inl.hpp"

#endif // NANAIRO_SHADER_MODEL_HPP
