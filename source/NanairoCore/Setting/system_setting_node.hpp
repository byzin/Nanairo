/*!
  \file system_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SYSTEM_SETTING_NODE_HPP
#define NANAIRO_SYSTEM_SETTING_NODE_HPP

// Standard C++ library
#include <array>
#include <istream>
#include <ostream>
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class SystemSettingNode : public SettingNodeBase
{
 public:
  // Return the rendering color mode
  RenderingColorMode colorMode() const noexcept;

  //! Return the color space
  ColorSpaceType colorSpace() const noexcept;

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
  virtual void initialize() noexcept override;

  //! Print data
  virtual void log() const noexcept override;

  //! Return the num of threads used for rendering
  uint32 numOfThreads() const noexcept;

  //! Return the power2 cycle saving flag
  bool power2CycleSaving() const noexcept;

  //! Return the seed of random number generator
  uint32 randomSeed() const noexcept;

  //! Read the setting datafrom the stream
  virtual void readData(std::istream* data_stream) noexcept override;

  //! Return the saving interval cycle
  uint32 savingIntervalCycle() const noexcept;

  //! Return the saving interval time in milliseconds
  uint32 savingIntervalTime() const noexcept;

  //! Set the rendering color mode
  void setColorMode(const RenderingColorMode mode) noexcept;

  //! Set the color space
  void setColorSpace(const ColorSpaceType color_space) noexcept;

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

  //! Set the seed of random number generator
  void setRandomSeed(const uint32 seed) noexcept;

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
  virtual SettingNodeType type() const noexcept override;

  //! Return the wavelength sampler type
  WavelengthSamplerType wavelengthSamplerType() const noexcept;

  //! Write the setting data to the stream
  virtual void writeData(std::ostream* data_stream) const noexcept override;

 private:
  uint32 num_of_threads_;
  uint32 random_seed_;
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
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SYSTEM_SETTING_NODE_HPP