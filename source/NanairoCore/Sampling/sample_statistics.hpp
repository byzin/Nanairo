/*!
  \file sample_statistics.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLE_STATISTICS_HPP
#define NANAIRO_SAMPLE_STATISTICS_HPP

// Standard C++ library
#include <bitset>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"
#include "NanairoCore/Geometry/point.hpp"

namespace nanairo {

// Forward declaration
class SampledSpectra;

//! \addtogroup Core
//! \{

/*!
  */
class SampleStatistics
{
 public:
  //!
  enum class Type : uint32
  {
    kExpectedValue = 0,
    kVariance,
  };

  using SpectralDistributionPointer =
      SpectralDistribution::SpectralDistributionPointer;
  using Flag = System::SampleStatisticsFlag;


  //! Initialize array memories 
  SampleStatistics(System& system) noexcept;


  //! Add a sample
  void addSample(const Index2d position, const SampledSpectra& sample) noexcept;

  //! Clear samples
  void clear() noexcept;

  //! Return the index of table correspond to the given position
  uint getIndex(const Index2d position) const noexcept;

  //! Return a sample by an index
  SpectralDistribution& getSample(const Index2d position) noexcept;

  //! Return a sample by an index
  const SpectralDistribution& getSample(const Index2d position) const noexcept;

  //! Return a sample squared by an index
  SpectralDistribution& getSampleSquared(const Index2d position) noexcept;

  //! Return a sample squared by an index
  const SpectralDistribution& getSampleSquared(const Index2d position) const noexcept;

  //! Check if the given sample type is enabled
  bool isEnabled(const Type type) const noexcept;

  //! Return the resolution
  Index2d resolution() const noexcept;

  //! Return the sample
  zisc::pmr::vector<SpectralDistributionPointer>& sampleTable() noexcept;

  //! Return the sample
  const zisc::pmr::vector<SpectralDistributionPointer>& sampleTable()
      const noexcept;

  //! Return the sample
  zisc::pmr::vector<SpectralDistributionPointer>& sampleSquaredTable() noexcept;

  //! Return the sample
  const zisc::pmr::vector<SpectralDistributionPointer>& sampleSquaredTable()
      const noexcept;

 private:
  //! Initialize statistics
  void initialize(System& system) noexcept;


  zisc::pmr::vector<SpectralDistributionPointer> sample_;
  zisc::pmr::vector<SpectralDistributionPointer> sample_squared_;
  Index2d resolution_;
  Flag flag_;
};

//! \}

} // namespace nanairo

#include "sample_statistics-inl.hpp"

#endif // NANAIRO_SAMPLE_STATISTICS_HPP
