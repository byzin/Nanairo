/*!
  \file wavelength_sampler.hpp
  \author Sho Ikeda
  */

#ifndef NANAIRO_WAVELENGTH_SAMPLER_HPP
#define NANAIRO_WAVELENGTH_SAMPLER_HPP

// Nanairo
#include "sampled_wavelengths.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declawation
class QJsonObject;

namespace nanairo {

// Forward declaration
class Sampler;
class World;

//! \addtogroup Core
//! \{

/*!
  \brief Spectra wavelength sampler
  */
class WavelengthSampler
{
 public:
  //! Initialize a sampler
  WavelengthSampler(const World& world, const QJsonObject& settings) noexcept;

  //! Sample wavelengths
  SampledWavelengths operator()(Sampler& sampler) const noexcept;


  //! Sample wavelengths
  SampledWavelengths sample(Sampler& sampler) const noexcept;

  //! Sample RGB wavelengths
  static SampledWavelengths sampleRgb(Sampler& sampler) noexcept;

  //! Sample wavelengths regularly
  static SampledWavelengths sampleRegularly(Sampler& sampler) noexcept;

  //! Sample wavelengths randomly
  static SampledWavelengths sampleRandomly(Sampler& sampler) noexcept;

  //! Sample wavelengths using stratified sampling method
  static SampledWavelengths sampleStratified(Sampler& sampler) noexcept;

 private:
  enum class SamplerType : int
  {
    kRegular,
    kRandom,
    kStratified,
    kRgb
  };


  //!
  void initialize(const World& world, const QJsonObject& settings) noexcept;


  SamplerType type_;
};

//! \} Core

} // namespace nanairo

#include "wavelength_sampler-inl.hpp"

#endif // NANAIRO_WAVELENGTH_SAMPLER_HPP
