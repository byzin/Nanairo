/*!
  \file sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLER_HPP
#define NANAIRO_SAMPLER_HPP

// Standard C++ library
#include <array>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

// Forward declaration
class PathState;

//! \addtogroup Core
//! \{

/*!
  */
enum class SamplerType : uint32
{
  kPcg                        = zisc::Fnv1aHash32::hash("PCG"),
  kXoshiro                    = zisc::Fnv1aHash32::hash("Xoshiro"),
  kCmj                        = zisc::Fnv1aHash32::hash("Correlated Multi-Jittered"),
};

/*!
  */
enum class SampleDimension : uint32
{
  kBase                       = 10, //!< offset
  kWavelengthSample1,
  kWavelengthSample2,
  kWavelengthSample3,
  kPrimaryWavelengthSelection,
  kCameraJittering,
  kCameraLensSample,
  kSensorSample1,
  kSensorSample2,
  kSensorSample3,
  kBxdfSample1,
  kBxdfSample2,
  kBxdfSample3,
  kLightSample1,
  kLightSample2,
  kLightSample3,
  kLightSourceSelection,
  kLightPointSample,
  kRussianRoulette,
  kBounce,
};

/*!
  \details
  No detailed.
  */
class Sampler
{
 public:
  //! Destroy a sampler
  virtual ~Sampler() noexcept;


  //! Sample a [0, 1) float random number
  virtual Float draw1D(const PathState& state) noexcept = 0;

  //! Sample two [0, 1) float random numbers
  virtual std::array<Float, 2> draw2D(const PathState& state) noexcept = 0;

  //! Make a sampler
  static zisc::UniqueMemoryPointer<Sampler> make(
      const SamplerType type,
      const uint32 seed,
      zisc::pmr::memory_resource* mem_resource) noexcept;
};

//! \} Core

} // namespace nanairo


#endif // NANAIRO_SAMPLER_HPP
