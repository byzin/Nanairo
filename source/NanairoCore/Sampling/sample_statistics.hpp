/*!
  \file sample_statistics.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLE_STATISTICS_HPP
#define NANAIRO_SAMPLE_STATISTICS_HPP

// Standard C++ library
#include <bitset>
#include <vector>
// Zisc
#include "zisc/compensated_summation.hpp"
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
class WavelengthSamples;

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
    kBayesianCollaborativeValues,
    kDenoisedExpectedValue,
  };

  using SpectralDistributionPointer =
      SpectralDistribution::SpectralDistributionPointer;
  using Flag = System::SampleStatisticsFlag;


  //! Initialize array memories 
  SampleStatistics(System& system) noexcept;


  //! Add a sample
  void addSample(const Index2d position,
                 const SampledSpectra& sample) noexcept;

  //! Clear samples
  void clear() noexcept;

  //! Return the covariance factor
  zisc::pmr::vector<zisc::CompensatedSummation<Float>>& covarianceFactorTable()
      noexcept;

  //! Return the covariance factor
  const zisc::pmr::vector<zisc::CompensatedSummation<Float>>& covarianceFactorTable()
      const noexcept;

  //! Return the denoised sample
  zisc::pmr::vector<SpectralDistributionPointer>& denoisedSampleTable() noexcept;

  //! Return the denoised sample
  const zisc::pmr::vector<SpectralDistributionPointer>& denoisedSampleTable()
      const noexcept;

  //! Return the index of covariance factor
  uint getFactorIndex(const uint i) const noexcept;

  //! Return the index of table correspond to the given position
  uint getIndex(const Index2d position) const noexcept;

  //! Check if the given sample type is enabled
  bool isEnabled(const Type type) const noexcept;

  //! Return the histogram
  zisc::pmr::vector<SpectralDistributionPointer>& histogramTable() noexcept;

  //! Return the histogram
  const zisc::pmr::vector<SpectralDistributionPointer>& histogramTable()
      const noexcept;

  //! Return the number of covariance factors
  uint numOfCovarianceFactors() const noexcept;

  //! Return the sample
  zisc::pmr::vector<SpectralDistributionPointer>& prevSampleTable() noexcept;

  //! Return the sample
  const zisc::pmr::vector<SpectralDistributionPointer>& prevSampleTable()
      const noexcept;

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

  //! Update statistics info
  void update(System& system,
              const WavelengthSamples& wavelengths,
              const uint32 cycle) noexcept;

 private:
  //! Initialize statistics
  void initialize(System& system) noexcept;

  //! Update covariance matrix factors
  void updateCovarianceFactor(const WavelengthSamples& wavelengths,
                              const std::size_t pixel_index) noexcept;

  //! Update histogram
  void updateHistogram(const System& system,
                       const WavelengthSamples& wavelengths,
                       const std::size_t pixel_index) noexcept;

  //! Update previous sample value
  void updatePrevSample(const WavelengthSamples& wavelengths,
                        const std::size_t pixel_index) noexcept;

  //! Update sample squared value
  void updateSampleSquared(const WavelengthSamples& wavelengths,
                           const std::size_t pixel_index) noexcept;


  zisc::pmr::vector<SpectralDistributionPointer> sample_;
  zisc::pmr::vector<SpectralDistributionPointer> prev_sample_;
  zisc::pmr::vector<SpectralDistributionPointer> sample_squared_;
  zisc::pmr::vector<SpectralDistributionPointer> histogram_;
  zisc::pmr::vector<zisc::CompensatedSummation<Float>> covariance_factor_;
  zisc::pmr::vector<SpectralDistributionPointer> denoised_sample_;
  Index2d resolution_;
  Flag flag_;
};

//! \}

} // namespace nanairo

#include "sample_statistics-inl.hpp"

#endif // NANAIRO_SAMPLE_STATISTICS_HPP
