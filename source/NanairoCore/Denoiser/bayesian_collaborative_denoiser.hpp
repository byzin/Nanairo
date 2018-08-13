/*!
  \file bayesian_collaborative_denoiser.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BAYESIAN_COLLABORATIVE_DENOISER_HPP
#define NANAIRO_BAYESIAN_COLLABORATIVE_DENOISER_HPP

// Standard C++ library
#include <array>
#include <bitset>
#include <cstdlib>
#include <vector>
// Zisc
#include "zisc/arith_array.hpp"
#include "zisc/matrix.hpp"
#include "zisc/memory_resource.hpp"
// Nanairo
#include "denoiser.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/rendering_tile.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class SampleStatistics;
class SpectralDistribution;
class System;

/*!
  */
class BayesianCollaborativeDenoiser : public Denoiser
{
 public:
  //! Initialize a denoiser
  BayesianCollaborativeDenoiser(const SettingNodeBase* settings) noexcept;


  //! Denoise input value
  void denoise(System& system,
               const uint32 cycle,
               SampleStatistics* statistics) const noexcept override;

  //! Return the histogram bins
  uint histogramBins() const noexcept;

 private:
  //! Return the order of chunk tiles
  static constexpr std::array<Index2d, 9> getChunkTileOrder() noexcept;

  //! Return the size of covariance matrix
  static constexpr uint getCovarianceMatrixSize(const uint dimension) noexcept;


  using SimilarPatchMask = std::bitset<2048>;
  template <uint kDimension>
  using SpectraArray = zisc::ArithArray<Float, kDimension>;
  template <uint kDimension>
  using Matrix = zisc::Matrix<Float, kDimension, kDimension>;
  template <uint kDimension>
  using CovarianceFactors = zisc::ArithArray<Float,
                                             getCovarianceMatrixSize(kDimension)>;

  template <uint kDimension>
  struct Parameters
  {
    //! Set resource
    Parameters(System& system) noexcept;
    //! Calculate a upscaed parameter
    template <uint kN>
    static void upscaleAdd(
        const Index2d& low_res,
        const zisc::pmr::vector<zisc::ArithArray<Float, kN>>& low_res_table,
        const Index2d& high_res,
        zisc::pmr::vector<zisc::ArithArray<Float, kN>>* high_res_table,
        const Index2d& range) noexcept;
    //! Calculate a downscaled parameter
    template <uint kN>
    static void downscaleAverage(
        const Index2d& high_res,
        const zisc::pmr::vector<zisc::ArithArray<Float, kN>>& high_res_table,
        const Index2d& low_res,
        zisc::pmr::vector<zisc::ArithArray<Float, kN>>* low_res_table,
        const Index2d& range) noexcept;
    //! Downscale the resolutions of parameters
    void downscaleOf(System& system,
                     const Parameters& high_res_p) noexcept;
    //! Initialize parameters
    void init(System& system,
              const uint32 cycle,
              const uint histogram_bins,
              const SampleStatistics& statistics) noexcept;

    zisc::pmr::vector<SpectraArray<kDimension>> sample_value_table_;
    zisc::pmr::vector<SpectraArray<kDimension>> histogram_table_;
    zisc::pmr::vector<CovarianceFactors<kDimension>> covariance_factor_table_;
    zisc::pmr::vector<SpectraArray<kDimension>> denoised_value_table_;
    Index2d resolution_;
    uint num_of_samples_;
    uint histogram_bins_;
  };

  struct PixelMarker
  {
    static constexpr uint kMarkerRepBits = 6; //!< 2^6=64bits
    using Marker = std::bitset<(1 << kMarkerRepBits)>;

    //! Initialize a marker
    PixelMarker(System& system) noexcept;
    //! Clear mark flags
    void clear() noexcept;
    //! Check if a pixel is marked
    bool isMarked(const uint index) const noexcept;
    //! Mark a pixel
    void mark(const uint index) noexcept;

    zisc::pmr::vector<Marker> marker_table_;
  };


  //! Aggregate denoised values
  template <uint kDimension>
  void aggregate(
      System& system,
      const zisc::pmr::vector<int>& estimates_counter,
      Parameters<kDimension>* parameters) const noexcept;

  //! Aggregate denoised values
  template <uint kDimension>
  void aggregateFinal(
      System& system,
      const Parameters<kDimension>& parameters,
      SampleStatistics* statistics) const noexcept;

  //! Calculate an enpirical mean
  template <uint kN>
  zisc::ArithArray<Float, kN> calcEmpiricalMean(
      const Index2d& resolution,
      RenderingTile& search_window,
      const Index2d& patch_offset,
      const SimilarPatchMask& similar_mask,
      const zisc::ArithArray<Float, kN>* table) const noexcept;

  //! Calculate an enpirical covariance matrix
  template <uint kDimension>
  CovarianceFactors<kDimension> calcEmpiricalCovarianceMatrix(
      const Index2d& resolution,
      RenderingTile& search_window,
      const Index2d& patch_offset,
      const SimilarPatchMask& similar_mask,
      const zisc::pmr::vector<SpectraArray<kDimension>>& value_table,
      const SpectraArray<kDimension>& value_mean) const noexcept;

  //! Calculate staging denoised values
  template <uint kDimension>
  void calcStagingDenoisedValue(
      const Index2d& resolution,
      RenderingTile& search_window,
      const Index2d& patch_offset,
      const SimilarPatchMask& similar_mask,
      const SpectraArray<kDimension>& expected_mean,
      const Matrix<kDimension>& covariance_mean,
      const Matrix<kDimension>& expected_covariance,
      const zisc::pmr::vector<SpectraArray<kDimension>>& value_table,
      zisc::pmr::vector<SpectraArray<kDimension>>* staging_value_table) const noexcept;

  //! Calculate a distance of 2 histograms
  template <uint kDimension>
  Float calcHistogramDistance(
      const SpectraArray<kDimension>& histogram_lhs,
      const SpectraArray<kDimension>& histogram_rhs,
      uint* num_of_non_both0) const noexcept;

  //! Calculate a histogram patch distance of 2 patches
  template <uint kDimension>
  Float calcHistogramPatchDistance(
      const Parameters<kDimension>& parameters,
      const Index2d& center_pixel_lhs,
      const Index2d& center_pixel_rhs) const noexcept;

  //! Denoise a chunk
  template <uint kDimension>
  void denoiseChunk(
      System& system,
      const Index2d& chunk_resolution,
      const Index2d& tile_position,
      Parameters<kDimension>* parameters,
      zisc::pmr::vector<SpectraArray<kDimension>>* staging_value_table,
      zisc::pmr::vector<int>* estimates_counter,
      PixelMarker* pixel_marker) const noexcept;

  //! Denoise input value
  template <uint kDimension>
  void denoiseMultiscale(
      System& system,
      const uint32 cycle,
      SampleStatistics* statistics) const noexcept;

  //! Denoise input value
  template <uint kDimension>
  void denoisePixels(
      const Index2d& main_pixel,
      Parameters<kDimension>* parameters,
      zisc::pmr::vector<SpectraArray<kDimension>>* staging_value_table,
      zisc::pmr::vector<int>* estimates_counter,
      PixelMarker* pixel_marker) const noexcept;

  //! Denoise only main patch
  template <uint kDimension>
  void denoiseOnlyMainPatch(
      const Index2d& main_pixel,
      const SimilarPatchMask& similar_mask,
      Parameters<kDimension>* parameters,
      zisc::pmr::vector<int>* estimates_counter) const noexcept;

  template <uint kDimension>
  void denoiseSelectedPatches(
      const Index2d& main_pixel,
      const SimilarPatchMask& similar_mask,
      Parameters<kDimension>* parameters,
      zisc::pmr::vector<SpectraArray<kDimension>>* staging_value_table,
      zisc::pmr::vector<int>* estimates_counter,
      PixelMarker* pixel_marker) const noexcept;

  //! Return the chunk resolution
  Index2d getChunkResolution(Index2d resolution) const noexcept;

  //! Return the length of chunk on side
  uint getChunkSize() const noexcept;

  //! Return the number of pixels in a patch
  uint getNumOfPatchPixels() const noexcept;

  //! Return the number of pixels in a search window
  uint getNumOfSearchWindowPixels() const noexcept;

  //! Return the color patch dimension
  template <uint kDimension>
  uint getPatchDimension() const noexcept;

  //! Initialize the denoiser
  void initialize(const SettingNodeBase* settings) noexcept;

  //! Make a patch
  RenderingTile makePatch(const Index2d& center_pixel) const noexcept;

  //! Make a search window
  RenderingTile makeSearchWindow(const Index2d& resolution,
                                 const Index2d& center_pixel) const noexcept;

  //! Make a chunk tile
  RenderingTile makeChunkTile(const Index2d& resolution,
                              const Index2d& chunk_position,
                              const Index2d& tile_position) const noexcept;

  //! Merge a low and a high resolution buffers
  template <uint kDimension>
  void merge(
      System& system,
      Parameters<kDimension>* low_res_p,
      Parameters<kDimension>* high_res_p,
      zisc::pmr::vector<SpectraArray<kDimension>>* staging_value_table) const noexcept;

  //! Select similar patches
  template <uint kDimension>
  SimilarPatchMask selectSimilarPatches(
      const Parameters<kDimension>& parameters,
      const Index2d& main_pixel) const noexcept;

  //! Convert to a matrix
  template <uint kDimension>
  Matrix<kDimension> toMatrix(const CovarianceFactors<kDimension>& factors)
      const noexcept;


  Float histogram_distance_threshold_;
  uint histogram_bins_;
  uint patch_radius_;
  uint search_radius_;
  uint num_of_scales_;
};

} // namespace nanairo

#include "bayesian_collaborative_denoiser-inl.hpp"

#endif // NANAIRO_BAYESIAN_COLLABORATIVE_DENOISER_HPP
