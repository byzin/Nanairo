/*!
  \file shader_model.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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

namespace zisc {

// Forward declaration
class MemoryChunk;

} // namespace zisc

namespace nanairo {

// Forward declaration
//class IntersectionInfo;
class IntersectionInfo;
class SampledDirection;
class SampledSpectra;
class Sampler;
class SurfaceModel;
class WavelengthSamples;

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
class ShaderModel
{
 public:
  //! Create a shader model
  ShaderModel() noexcept;

  //! Finalize the shader model
  virtual ~ShaderModel() noexcept {}


  //! Delete a new shader
  static void operator delete(void* shader, zisc::MemoryChunk* chunk) noexcept;

  //! Create a new shader
  static void* operator new(std::size_t, zisc::MemoryChunk* chunk) noexcept;


  //! Evaluate the pdf
  virtual Float evalPdf(
      const Vector3* vin,
      const Vector3* vout,
      const WavelengthSamples& wavelengths,
      const IntersectionInfo* info = nullptr) const noexcept;

  //! Evaluate the radiance
  virtual SampledSpectra evalRadiance(
      const Vector3* vin,
      const Vector3* vout,
      const WavelengthSamples& wavelengths,
      const IntersectionInfo* info = nullptr) const noexcept;

  //! Evaluate the radiance and pdf
  virtual std::tuple<SampledSpectra, Float> evalRadianceAndPdf(
      const Vector3* vin,
      const Vector3* vout,
      const WavelengthSamples& wavelengths,
      const IntersectionInfo* info = nullptr) const noexcept;

  //! Check if the shader is emissive
  virtual bool isEmissive() const noexcept;

  //! Check if the shader is reflective
  virtual bool isReflective() const noexcept;

  //! Check if the shader is transmissive
  virtual bool isTransmissive() const noexcept;

  //! Sample a reflection direction and evaluate a reflection weight
  virtual std::tuple<SampledDirection, SampledSpectra> sample(
      const Vector3* vin, 
      const WavelengthSamples& wavelengths,
      Sampler& sampler,
      const IntersectionInfo* info = nullptr) const noexcept;

  //! Return the shader type
  virtual ShaderType type() const noexcept = 0;

  //! Check if wavelength selection occured
  virtual bool wavelengthIsSelected() const noexcept = 0;

 protected:
  static void operator delete(void*) {}

 private:
  static void operator delete[](void*) = delete;
  static void* operator new(std::size_t) = delete;
  static void* operator new[](std::size_t) = delete;
};

/*!
  \details
  No detailed.
  */
class DiffuseShaderModel : public ShaderModel
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
class SpecularShaderModel : public ShaderModel
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
class GlossyShaderModel : public ShaderModel
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

#endif // NANAIRO_SHADER_MODEL_HPP
