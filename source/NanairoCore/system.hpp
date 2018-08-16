/*!
  \file system.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SYSTEM_HPP
#define NANAIRO_SYSTEM_HPP

// Standard C++ library
#include <array>
#include <bitset>
#include <vector>
// Zisc
#include "zisc/memory_manager.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/point.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/simple_memory_resource.hpp"
#include "zisc/stopwatch.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "Color/color_space.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "Sampling/Sampler/sampler.hpp"
#include "Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class Denoiser;
class ToneMappingOperator;
class XyzColorMatchingFunction;

//! \addtogroup Core
//! \{

enum class RenderingColorMode : uint32
{
  kRgb                        = zisc::Fnv1aHash32::hash("RGB"),
  kSpectra                    = zisc::Fnv1aHash32::hash("Spectra")
};

/*!
  \details
  No detailed.
  */
class System : public zisc::NonCopyable<System>
{
 public:
#ifdef NANAIRO_USES_EFFICIENT_MEMORY_MANAGER
  using MemoryManager = zisc::DynamicMemoryManager<CoreConfig::memoryPoolSize()>;
#else // NANAIRO_USES_EFFICIENT_MEMORY_MANAGER
  using MemoryManager = zisc::SimpleMemoryResource;
#endif // NANAIRO_USES_EFFICIENT_MEMORY_MANAGER
  using SampleStatisticsFlag = std::bitset<32>;


  //! Initialize the renderer system
  System(const SettingNodeBase* settings) noexcept;

  //! Finalize the renderer system
  ~System() noexcept;


  // System
  //! Calculate the range of the task id
  template <typename Integer>
  static std::array<Integer, 2> calcTaskRange(const Integer range,
                                              const uint num_of_tasks,
                                              const uint task_id) noexcept;

  //! Calculate the range of the task id
  template <typename Integer>
  std::array<Integer, 2> calcTaskRange(const Integer range,
                                       const uint task_id) const noexcept;

  //! Return the memory manager for data allocation
  MemoryManager& dataMemoryManager() noexcept;

  //! Return the global memory manager
  MemoryManager& globalMemoryManager() noexcept;

  //! Return the global sampler
  Sampler& globalSampler() noexcept;

  //! Return the image resolution 
  const Index2d& imageResolution() const noexcept;

  //! Return the image height resolution
  uint imageHeightResolution() const noexcept;

  //! Return the image width resolution
  uint imageWidthResolution() const noexcept;

  //! Return a sampler
  Sampler& localSampler(const uint index) noexcept;

  //! Return the thread manager
  zisc::ThreadManager& threadManager() noexcept;

  //! Return the thread manager
  const zisc::ThreadManager& threadManager() const noexcept;

  //! Return the thread's memory manager
  MemoryManager& threadMemoryManager(const uint thread_number) noexcept;

  // Color system
  //! Return the color mode
  RenderingColorMode colorMode() const noexcept;

  //! Return the color space
  ColorSpaceType colorSpace() const noexcept;

  //! Return the denoiser
  Denoiser& denoiser() noexcept;

  //! Return the denoiser
  const Denoiser& denoiser() const noexcept;

  //! Return the gamma
  Float gamma() const noexcept;

  //! Check if system has a denoiser
  bool hasDenoiser() const noexcept;

  //! Check if the renderer is RGB rendering mode
  bool isRgbMode() const noexcept;

  //! Check if the renderer is spectra rendering mode
  bool isSpectraMode() const noexcept;

  //! Return the sampler seed
  uint32 samplerSeed() const noexcept;

  //! Return the sampler type
  SamplerType samplerType() const noexcept;

  //! Return the flag of sample statistics
  SampleStatisticsFlag& sampleStatisticsFlag() noexcept;

  //! Return the flag of sample statistics
  const SampleStatisticsFlag& sampleStatisticsFlag() const noexcept;

  //! Return the system stopwatch
  zisc::Stopwatch& stopwatch() noexcept;

  //! Return the system stopwatch
  const zisc::Stopwatch& stopwatch() const noexcept;

  //! Return the tone mapping
  const ToneMappingOperator& toneMappingOperator() const noexcept;

  //! Return the XYZ color matching function
  const XyzColorMatchingFunction& xyzColorMatchingFunction() const noexcept;

 private:
  //! Initialize the renderer system
  void initialize(const SettingNodeBase* settings) noexcept;


  std::vector<MemoryManager> memory_manager_list_;
  zisc::pmr::vector<zisc::UniqueMemoryPointer<Sampler>> sampler_list_;
  zisc::UniqueMemoryPointer<zisc::ThreadManager> thread_manager_;
  zisc::UniqueMemoryPointer<XyzColorMatchingFunction> xyz_color_matching_function_;
  zisc::UniqueMemoryPointer<ToneMappingOperator> tone_mapping_operator_;
  zisc::UniqueMemoryPointer<Denoiser> denoiser_;
  zisc::Stopwatch stopwatch_;
  Float gamma_;
  Index2d image_resolution_;
  SamplerType sampler_type_;
  uint32 sampler_seed_;
  RenderingColorMode color_mode_;
  ColorSpaceType color_space_;
  SampleStatisticsFlag statistics_flag_;
};

//! \} Core

} // namespace nanairo

#include "system-inl.hpp"

#endif // NANAIRO_SYSTEM_HPP
