/*!
  \file system_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "system_setting_node.hpp"
// Standard C++ library
#include <array>
#include <istream>
#include <ostream>
#include <utility>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Denoiser/denoiser.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"

namespace nanairo {

/*!
  */
void BayesianCollaborativeDenoiserParameters::readData(std::istream* data_stream)
    noexcept
{
  zisc::read(&histogram_bins_, data_stream);
  zisc::read(&histogram_distance_threshold_, data_stream);
  zisc::read(&patch_radius_, data_stream);
  zisc::read(&search_window_radius_, data_stream);
  zisc::read(&number_of_scales_, data_stream);
}

/*!
  */
void BayesianCollaborativeDenoiserParameters::writeData(std::ostream* data_stream)
    const noexcept
{
  zisc::write(&histogram_bins_, data_stream);
  zisc::write(&histogram_distance_threshold_, data_stream);
  zisc::write(&patch_radius_, data_stream);
  zisc::write(&search_window_radius_, data_stream);
  zisc::write(&number_of_scales_, data_stream);
}

/*!
  */
SystemSettingNode::SystemSettingNode(const SettingNodeBase* parent) noexcept :
    SettingNodeBase(parent)
{
}

/*!
  */
BayesianCollaborativeDenoiserParameters&
SystemSettingNode::bayesianCollaborativeDenoiserParameters() noexcept
{
  ZISC_ASSERT(denoiserType() == DenoiserType::kBayesianCollaborative,
              "Invalid denoiser type is specified.");
  auto parameters = zisc::cast<BayesianCollaborativeDenoiserParameters*>(
      denoiser_parameters_.get());
  return *parameters;
}

/*!
  */
const BayesianCollaborativeDenoiserParameters&
SystemSettingNode::bayesianCollaborativeDenoiserParameters() const noexcept
{
  ZISC_ASSERT(denoiserType() == DenoiserType::kBayesianCollaborative,
              "Invalid denoiser type is specified.");
  auto parameters = zisc::cast<const BayesianCollaborativeDenoiserParameters*>(
      denoiser_parameters_.get());
  return *parameters;
}

/*!
  */
RenderingColorMode SystemSettingNode::colorMode() const noexcept
{
  return color_mode_;
}

/*!
  */
ColorSpaceType SystemSettingNode::colorSpace() const noexcept
{
  return color_space_;
}

/*!
  */
DenoiserType SystemSettingNode::denoiserType() const noexcept
{
  return denoiser_type_;
}

/*!
  */
void SystemSettingNode::enableDenoising(const bool flag) noexcept
{
  is_denoising_enabled_ = flag ? kTrue : kFalse;
}

/*!
  */
double SystemSettingNode::exposure() const noexcept
{
  return exposure_;
}

/*!
  */
double SystemSettingNode::gammaCorrection() const noexcept
{
  return gamma_correction_;
}

/*!
  */
const std::array<uint32, 2>& SystemSettingNode::imageResolution() const noexcept
{
  ZISC_ASSERT(CoreConfig::imageWidthMin() <= image_resolution_[0],
              "The image width is smaller than min.");
  ZISC_ASSERT(CoreConfig::imageHeightMin() <= image_resolution_[1],
              "The image height is smaller than min.");
  return image_resolution_;
}

/*!
  */
uint32 SystemSettingNode::imageHeightResolution() const noexcept
{
  ZISC_ASSERT(CoreConfig::imageHeightMin() <= image_resolution_[1],
              "The image height is smaller than min.");
  return image_resolution_[1];
}

/*!
  */
uint32 SystemSettingNode::imageWidthResolution() const noexcept
{
  ZISC_ASSERT(CoreConfig::imageWidthMin() <= image_resolution_[0],
              "The image width is smaller than min.");
  return image_resolution_[0];
}

/*!
  */
void SystemSettingNode::initialize() noexcept
{
  setNumOfThreads(1);
  setSamplerType(SamplerType::kCmj);
  setSamplerSeed(123456789);
  setTerminationTime(0);
  setTerminationCycle(1024);
  setImageWidthResolution(CoreConfig::imageWidthMin());
  setImageHeightResolution(CoreConfig::imageHeightMin());
  setSavingIntervalTime(1 * 60 * 60 * 1000); // per hour
  setSavingIntervalCycle(0);
  setPower2CycleSaving(true);
  // Color
  setColorMode(RenderingColorMode::kRgb);
  setWavelengthSamplerType(WavelengthSamplerType::kRegular);
  setColorSpace(ColorSpaceType::kSRgbD65);
  setGammaCorrection(2.2);
  setToneMappingType(ToneMappingType::kReinhard);
  setExposure(1.0);
  // Denoiser
  setDenoiserType(DenoiserType::kBayesianCollaborative);
  enableDenoising(false);
}

/*!
  */
bool SystemSettingNode::isDenoisingEnabled() const noexcept
{
  return is_denoising_enabled_ == kTrue;
}

/*!
  */
SettingNodeType SystemSettingNode::nodeType() noexcept
{
  return SettingNodeType::kSystem;
}

/*!
  */
uint32 SystemSettingNode::numOfThreads() const noexcept
{
  ZISC_ASSERT(num_of_threads_ != 0, "The num of thread is zero.");
  return num_of_threads_;
}

/*!
  */
bool SystemSettingNode::power2CycleSaving() const noexcept
{
  return power2_cycle_saving_ == kTrue;
}

/*!
  */
uint32 SystemSettingNode::samplerSeed() const noexcept
{
  return sampler_seed_;
}

/*!
  */
SamplerType SystemSettingNode::samplerType() const noexcept
{
  return sampler_type_;
}

/*!
  */
void SystemSettingNode::readData(std::istream* data_stream) noexcept
{
  // Read properties
  zisc::read(&num_of_threads_, data_stream);
  zisc::read(&sampler_type_, data_stream);
  zisc::read(&sampler_seed_, data_stream);
  zisc::read(&termination_time_, data_stream);
  zisc::read(&termination_cycle_, data_stream);
  zisc::read(&saving_interval_time_, data_stream);
  zisc::read(&saving_interval_cycle_, data_stream);
  zisc::read(&image_resolution_, data_stream, sizeof(image_resolution_[0]) * 2);
  zisc::read(&power2_cycle_saving_, data_stream);
  // Color
  zisc::read(&color_mode_, data_stream);
  zisc::read(&wavelength_sampler_type_, data_stream);
  zisc::read(&color_space_, data_stream);
  zisc::read(&gamma_correction_, data_stream);
  zisc::read(&tone_mapping_type_, data_stream);
  zisc::read(&exposure_, data_stream);
  // Denoiser
  {
    zisc::read(&denoiser_type_, data_stream);
    setDenoiserType(denoiser_type_);
  }
  zisc::read(&is_denoising_enabled_, data_stream);
  if (denoiser_parameters_)
    denoiser_parameters_->readData(data_stream);
}

/*!
  */
uint32 SystemSettingNode::savingIntervalCycle() const noexcept
{
  return saving_interval_cycle_;
}

/*!
  */
uint32 SystemSettingNode::savingIntervalTime() const noexcept
{
  return saving_interval_time_;
}

/*!
  */
void SystemSettingNode::setColorMode(const RenderingColorMode mode) noexcept
{
  color_mode_ = mode;
}

/*!
  */
void SystemSettingNode::setColorSpace(const ColorSpaceType color_space) noexcept
{
  color_space_ = color_space;
}

/*!
  */
void SystemSettingNode::setDenoiserType(const DenoiserType denoiser_type) noexcept
{
  denoiser_type_ = denoiser_type;
  // Initialize parameters
  denoiser_parameters_.reset();
  switch (denoiser_type_) {
   case DenoiserType::kBayesianCollaborative: {
    denoiser_parameters_ =
      zisc::UniqueMemoryPointer<BayesianCollaborativeDenoiserParameters>::make(
          dataResource());
    break;
   }
   default:
    break;
  }
}

/*!
  */
void SystemSettingNode::setExposure(const double exposure) noexcept
{
  exposure_ = exposure;
}

/*!
  */
void SystemSettingNode::setGammaCorrection(const double gamma_correction) noexcept
{
  gamma_correction_ = gamma_correction;
}

/*!
  */
void SystemSettingNode::setImageResolution(const uint32 width,
                                           const uint32 height) noexcept
{
  setImageWidthResolution(width);
  setImageHeightResolution(height);
}

/*!
  */
void SystemSettingNode::setImageHeightResolution(const uint32 image_height) noexcept
{
  ZISC_ASSERT(CoreConfig::imageHeightMin() <= image_height,
              "The image height is smaller than min.");
  image_resolution_[1] = image_height;
}

/*!
  */
void SystemSettingNode::setImageWidthResolution(const uint32 image_width) noexcept
{
  ZISC_ASSERT(CoreConfig::imageWidthMin() <= image_width,
              "The image width is smaller than min.");
  image_resolution_[0] = image_width;
}

/*!
  */
void SystemSettingNode::setNumOfThreads(const uint32 num_of_threads) noexcept
{
  ZISC_ASSERT(num_of_threads != 0, "The num of thread is zero.");
  num_of_threads_ = num_of_threads;
}

/*!
  */
void SystemSettingNode::setPower2CycleSaving(const bool power2_cycle_saving) noexcept
{
  power2_cycle_saving_ = power2_cycle_saving ? kTrue : kFalse;
}

/*!
  */
void SystemSettingNode::setSamplerSeed(const uint32 seed) noexcept
{
  sampler_seed_ = seed;
}

/*!
  */
void SystemSettingNode::setSamplerType(const SamplerType type) noexcept
{
  sampler_type_ = type;
}

/*!
  */
void SystemSettingNode::setSavingIntervalCycle(const uint32 interval_cycle) noexcept
{
  saving_interval_cycle_ = interval_cycle;
}

/*!
  */
void SystemSettingNode::setSavingIntervalTime(const uint32 interval_time) noexcept
{
  saving_interval_time_ = interval_time;
}

/*!
  */
void SystemSettingNode::setTerminationCycle(const uint32 termination_cycle) noexcept
{
  termination_cycle_ = termination_cycle;
}

/*!
  */
void SystemSettingNode::setTerminationTime(const uint32 termination_time) noexcept
{
  termination_time_ = termination_time;
}

/*!
  */
void SystemSettingNode::setToneMappingType(
    const ToneMappingType mapping_type) noexcept
{
  tone_mapping_type_ = mapping_type;
}

/*!
  */
void SystemSettingNode::setWavelengthSamplerType(
    const WavelengthSamplerType sampler_type) noexcept
{
  wavelength_sampler_type_ = sampler_type;
}

/*!
  */
uint32 SystemSettingNode::terminationCycle() const noexcept
{
  return termination_cycle_;
}

/*!
  */
uint32 SystemSettingNode::terminationTime() const noexcept
{
  return termination_time_;
}

/*!
  */
ToneMappingType SystemSettingNode::toneMappingType() const noexcept
{
  return tone_mapping_type_;
}

/*!
  */
SettingNodeType SystemSettingNode::type() const noexcept
{
  return SettingNodeType::kSystem;
}

/*!
  */
WavelengthSamplerType SystemSettingNode::wavelengthSamplerType() const noexcept
{
  return wavelength_sampler_type_;
}

/*!
  */
void SystemSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);

  // Write properties
  zisc::write(&num_of_threads_, data_stream);
  zisc::write(&sampler_type_, data_stream);
  zisc::write(&sampler_seed_, data_stream);
  zisc::write(&termination_time_, data_stream);
  zisc::write(&termination_cycle_, data_stream);
  zisc::write(&saving_interval_time_, data_stream);
  zisc::write(&saving_interval_cycle_, data_stream);
  zisc::write(&image_resolution_, data_stream, sizeof(image_resolution_[0]) * 2);
  zisc::write(&power2_cycle_saving_, data_stream);
  // Color
  zisc::write(&color_mode_, data_stream);
  zisc::write(&wavelength_sampler_type_, data_stream);
  zisc::write(&color_space_, data_stream);
  zisc::write(&gamma_correction_, data_stream);
  zisc::write(&tone_mapping_type_, data_stream);
  zisc::write(&exposure_, data_stream);
  // Denoiser
  zisc::write(&denoiser_type_, data_stream);
  zisc::write(&is_denoising_enabled_, data_stream);
  if (denoiser_parameters_)
    denoiser_parameters_->writeData(data_stream);
}

} // namespace nanairo
