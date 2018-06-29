/*!
  \file wavelength_sampler.hpp
  \author Sho Ikeda
  */

#ifndef NANAIRO_WAVELENGTH_SAMPLER_HPP
#define NANAIRO_WAVELENGTH_SAMPLER_HPP

// Zisc
#include "zisc/fnv_1a_hash_engine.hpp"
// Nanairo
#include "sampled_wavelengths.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class PathState;
class Sampler;
class World;

//! \addtogroup Core
//! \{

enum class WavelengthSamplerType : uint32
{
  kRegular                    = zisc::Fnv1aHash32::hash("Regular"),
  kRandom                     = zisc::Fnv1aHash32::hash("Random"),
  kStratified                 = zisc::Fnv1aHash32::hash("Stratified")
};

/*!
  \brief Spectra wavelength sampler
  */
class WavelengthSampler
{
 public:
  //! Initialize a sampler
  WavelengthSampler(const World& world, const SettingNodeBase* settings) noexcept;

  //! Sample wavelengths
  SampledWavelengths operator()(Sampler& sampler,
                                PathState& path_state) const noexcept;


  //! Sample wavelengths
  SampledWavelengths sample(Sampler& sampler,
                            PathState& path_state) const noexcept;

  //! Sample RGB wavelengths
  static SampledWavelengths sampleRgb(Sampler& sampler,
                                      PathState& path_state) noexcept;

  //! Sample wavelengths regularly
  static SampledWavelengths sampleRegularly(Sampler& sampler,
                                            PathState& path_state) noexcept;

  //! Sample wavelengths randomly
  static SampledWavelengths sampleRandomly(Sampler& sampler,
                                           PathState& path_state) noexcept;

  //! Sample wavelengths using stratified sampling method
  static SampledWavelengths sampleStratified(Sampler& sampler,
                                             PathState& path_state) noexcept;

 private:
  enum class SamplerType : int
  {
    kRegular,
    kRandom,
    kStratified,
    kRgb
  };


  //!
  void initialize(const World& world, const SettingNodeBase* settings) noexcept;


  SamplerType type_;
};

//! \} Core

} // namespace nanairo

#include "wavelength_sampler-inl.hpp"

#endif // NANAIRO_WAVELENGTH_SAMPLER_HPP
