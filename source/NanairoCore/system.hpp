/*!
  \file system.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SYSTEM_HPP
#define NANAIRO_SYSTEM_HPP

// Standard C++ library
#include <tuple>
#include <vector>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/thread_pool.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class XyzColorMatchingFunction;
class Sampler;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class System
{
 public:
  //! Initialize the renderer system
  System(const QJsonObject& settings) noexcept;

  //! Finalize the renderer system
  ~System() noexcept;


  // System
  //! Calculate the range of indices
  template <typename Integer>
  static std::tuple<Integer, Integer> calcThreadRange(const Integer num,
                                                      const Integer num_of_threads,
                                                      const int thread_id) noexcept;

  //! Calculate the range of indices
  template <typename Integer>
  std::tuple<Integer, Integer> calcThreadRange(const Integer num,
                                               const int thread_id) const noexcept;

  //! Return the global memory pool
  MemoryPool& globalMemoryPool() noexcept;

  //! Return the global sampler
  Sampler& globalSampler() noexcept;

  //! Return the image height resolution
  uint imageHeightResolution() const noexcept;

  //! Return the image width resolution
  uint imageWidthResolution() const noexcept;

  //! Return the thread pool
  zisc::ThreadPool& threadPool() noexcept;

  //! Return the thread pool
  const zisc::ThreadPool& threadPool() const noexcept;

  //! Return the thread's memory pool
  MemoryPool& threadMemoryPool(const uint thread_number) noexcept;

  //! Return the thread's sampler
  Sampler& threadSampler(const uint thread_number) noexcept;

  // Color system
  //! Return the color space
  uint32 colorSpace() const noexcept;

  //! Return the gamma
  Float gamma() const noexcept;

  //! Check if the renderer is RGB rendering mode
  bool isRgbRenderingMode() const noexcept;

  //! Return the XYZ color matching function
  const XyzColorMatchingFunction& xyzColorMatchingFunction() const noexcept;

 private:
  //! Initialize the renderer system
  void initialize(const QJsonObject& settings) noexcept;


  std::vector<Sampler> sampler_list_;
  std::vector<MemoryPool> memory_pool_list_;
  UniquePointer<zisc::ThreadPool> thread_pool_;
  UniquePointer<XyzColorMatchingFunction> xyz_color_matching_function_;
  Float gamma_;
  uint image_width_resolution_,
       image_height_resolution_;
  uint32 color_space_;
  bool is_rgb_rendering_mode_;
};

//! \} Core

} // namespace nanairo

#include "system-inl.hpp"

#endif // NANAIRO_SYSTEM_HPP
