/*!
  \file system_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SYSTEM_SETTING_NODE_HPP
#define NANAIRO_SYSTEM_SETTING_NODE_HPP

// Standard C++ library
#include <array>
#include <istream>
#include <ostream>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Denoiser/denoiser.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

// Bayesian collaborative denoiser
struct BayesianCollaborativeDenoiserParameters : public NodeParameterBase
{
  //! Read the parameters from the streamm
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  uint32 histogram_bins_ = 16;
  double histogram_distance_threshold_ = 0.75;
  uint32 patch_radius_ = 1;
  uint32 search_window_radius_ = 6;
  uint32 number_of_scales_ = 2;
};

/*!
  */
class SystemSettingNode : public SettingNodeBase
{
 public:
  //! Create a system settings
  SystemSettingNode(const SettingNodeBase* parent) noexcept;


  //! Return the BayesianCollaborativeDenoiser parameters
  BayesianCollaborativeDenoiserParameters&
  bayesianCollaborativeDenoiserParameters() noexcept;

  //! Return the BayesianCollaborativeDenoiser parameters
  const BayesianCollaborativeDenoiserParameters&
  bayesianCollaborativeDenoiserParameters() const noexcept;

  // Return the rendering color mode
  RenderingColorMode colorMode() const noexcept;

  //! Return the color space
  ColorSpaceType colorSpace() const noexcept;

  //! Return the denoiser type
  DenoiserType denoiserType() const noexcept;

  //! Enable denoising
  void enableDenoising(const bool flag) noexcept;

  //! Return the exposure time in seconds
  double exposure() const noexcept;

  //! Return the gamma correction value
  double gammaCorrection() const noexcept;

  //! Return the image resolution
  const std::array<uint32, 2>& imageResolution() const noexcept;

  //! Return the image height resolution
  uint32 imageHeightResolution() const noexcept;

  //! Return the image width resolution
  uint32 imageWidthResolution() const noexcept;

  //! Initialize a systemm node
  void initialize() noexcept override;

  //! Check if denoising is enabled
  bool isDenoisingEnabled() const noexcept;

  //! Return the node type
  static SettingNodeType nodeType() noexcept;

  //! Return the num of threads used for rendering
  uint32 numOfThreads() const noexcept;

  //! Return the power2 cycle saving flag
  bool power2CycleSaving() const noexcept;

  //! Read the setting datafrom the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Return the seed of a sampler
  uint32 samplerSeed() const noexcept;

  //! Return the sampler type
  SamplerType samplerType() const noexcept;

  //! Return the saving interval cycle
  uint32 savingIntervalCycle() const noexcept;

  //! Return the saving interval time in milliseconds
  uint32 savingIntervalTime() const noexcept;

  //! Set the rendering color mode
  void setColorMode(const RenderingColorMode mode) noexcept;

  //! Set the color space
  void setColorSpace(const ColorSpaceType color_space) noexcept;

  //! Set the denoiser type
  void setDenoiserType(const DenoiserType denoiser_type) noexcept;

  //! Set the exposure time in seconds
  void setExposure(const double exposure) noexcept;

  //! Set the gamma correction value
  void setGammaCorrection(const double gamma_correction) noexcept;

  //! Set the image resolution
  void setImageResolution(const uint32 width, const uint32 height) noexcept;

  //! Set the image height resolution
  void setImageHeightResolution(const uint32 image_height) noexcept;

  //! Set the image width resolution
  void setImageWidthResolution(const uint32 image_width) noexcept;

  //! Set the num of threads used for rendering
  void setNumOfThreads(const uint32 num_of_threads) noexcept;

  //! Set the power2 cycle saving flag
  void setPower2CycleSaving(const bool power2_cycle_saving) noexcept;

  //! Set the seed of a sampler
  void setSamplerSeed(const uint32 seed) noexcept;

  //! Set the sampler type
  void setSamplerType(const SamplerType type) noexcept;

  //! Set the saving interval cycle
  void setSavingIntervalCycle(const uint32 interval_cycle) noexcept;

  //! Set the saving interval time in millisecond
  void setSavingIntervalTime(const uint32 interval_time) noexcept;

  //! Set the termination cycle
  void setTerminationCycle(const uint32 termination_cycle) noexcept;

  //! Set the termination time in milliseconds
  void setTerminationTime(const uint32 termination_time) noexcept;

  //! Set tone mapping type
  void setToneMappingType(const ToneMappingType mapping_type) noexcept;

  //! Set the wavelength sampler type
  void setWavelengthSamplerType(const WavelengthSamplerType sampler_type) noexcept;

  //! Return the termination cycle
  uint32 terminationCycle() const noexcept;

  //! Return the termination time in milliseconds
  uint32 terminationTime() const noexcept;

  //! Return tone mapping type
  ToneMappingType toneMappingType() const noexcept;

  //! Return the setting node type
  SettingNodeType type() const noexcept override;

  //! Return the wavelength sampler type
  WavelengthSamplerType wavelengthSamplerType() const noexcept;

  //! Write the setting data to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  uint32 num_of_threads_;
  SamplerType sampler_type_;
  uint32 sampler_seed_;
  uint32 termination_time_,
         termination_cycle_;
  uint32 saving_interval_time_,
         saving_interval_cycle_;
  std::array<uint32, 2> image_resolution_;
  uint8 power2_cycle_saving_;
  // Color
  RenderingColorMode color_mode_;
  WavelengthSamplerType wavelength_sampler_type_;
  ColorSpaceType color_space_;
  double gamma_correction_;
  ToneMappingType tone_mapping_type_;
  double exposure_;
  // Denoiser
  zisc::UniqueMemoryPointer<NodeParameterBase> denoiser_parameters_;
  DenoiserType denoiser_type_;
  uint8 is_denoising_enabled_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SYSTEM_SETTING_NODE_HPP
