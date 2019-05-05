/*!
  \file bayesian_collaborative_denoiser.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
template <uint kDimension>
class BayesianCollaborativeDenoiser : public Denoiser
{
 public:
  //! Initialize a denoiser
  BayesianCollaborativeDenoiser(const SettingNodeBase* settings) noexcept;


  //! Return the dimension of denoised color
  static constexpr uint dimension() noexcept;

  //! Denoise input value
  void denoise(System& system,
               const uint32 cycle,
               SampleStatistics* statistics) const noexcept override;

  //! Return the histogram bins
  uint histogramBins() const noexcept;

 private:
  //! Return the size of covariance matrix
  static constexpr uint getCovarianceMatrixSize(const uint dimension) noexcept;


  using SimilarPatchMask = std::bitset<2048>;
  using SpectraArray = zisc::ArithArray<Float, kDimension>;
  using CovarianceFactors = zisc::ArithArray<Float,
                                             getCovarianceMatrixSize(kDimension)>;
  using CovarianceMatrix = zisc::Matrix<Float, kDimension, kDimension>;

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
    static void downscaleSum(
        const Index2d& high_res,
        const zisc::pmr::vector<zisc::ArithArray<Float, kN>>& high_res_table,
        const Index2d& low_res,
        zisc::pmr::vector<zisc::ArithArray<Float, kN>>* low_res_table,
        const Index2d& range,
        const uint table_offset = 0) noexcept;

    //! Downscale the resolutions of parameters
    void downscaleOf(System& system,
                     const Parameters& high_res_p) noexcept;

    //! Initialize parameters
    void init(System& system,
              const uint32 cycle,
              const uint histogram_bins,
              const SampleStatistics& statistics) noexcept;

    zisc::pmr::vector<SpectraArray> sample_value_table_;
    zisc::pmr::vector<SpectraArray> histogram_table_;
    zisc::pmr::vector<CovarianceFactors> covariance_factor_table_;
    zisc::pmr::vector<SpectraArray> denoised_value_table_;
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
  void aggregate(System& system,
                 const zisc::pmr::vector<int>& estimates_counter,
                 Parameters* parameter) const noexcept;

  //! Aggregate denoised values
  void aggregateFinal(System& system,
                      const Parameters& parameter,
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
  CovarianceFactors calcEmpiricalCovarianceMatrix(
      const Index2d& resolution,
      RenderingTile& search_window,
      const Index2d& patch_offset,
      const SimilarPatchMask& similar_mask,
      const zisc::pmr::vector<SpectraArray>& value_table,
      const SpectraArray& value_mean) const noexcept;

  //! Calculate staging denoised values
  void calcStagingDenoisedValue(
      const Index2d& resolution,
      RenderingTile& search_window,
      const Index2d& patch_offset,
      const SimilarPatchMask& similar_mask,
      const SpectraArray& expected_mean,
      const CovarianceMatrix& covariance_mean,
      const CovarianceMatrix& expected_covariance,
      const zisc::pmr::vector<SpectraArray>& value_table,
      zisc::pmr::vector<SpectraArray>* staging_value_table) const noexcept;

  //! Calculate a distance of 2 histograms
  Float calcHistogramDistance(
      const SpectraArray& histogram_lhs,
      const SpectraArray& histogram_rhs,
      uint* num_of_non_both0) const noexcept;

  //! Calculate a histogram patch distance of 2 patches
  Float calcHistogramPatchDistance(
      const Parameters& parameter,
      const Index2d& center_pixel_lhs,
      const Index2d& center_pixel_rhs) const noexcept;

  //! Denoise a chunk
  void denoiseChunks(System& system,
                     const Index2d& chunk_resolution,
                     const Index2d& tile_position,
                     Parameters* parameter,
                     zisc::pmr::vector<SpectraArray>* staging_value_table,
                     zisc::pmr::vector<int>* estimates_counter,
                     PixelMarker* pixel_marker) const noexcept;

  //! Denoise input value
  void denoiseMultiscale(System& system,
                         const uint32 cycle,
                         SampleStatistics* statistics) const noexcept;

  //! Denoise input value
  void denoisePixels(const Index2d& main_pixel,
                     Parameters* parameter,
                     zisc::pmr::vector<SpectraArray>* staging_value_table,
                     zisc::pmr::vector<int>* estimates_counter,
                     PixelMarker* pixel_marker) const noexcept;

  //! Denoise only main patch
  void denoiseOnlyMainPatch(
      const Index2d& main_pixel,
      const SimilarPatchMask& similar_mask,
      Parameters* parameter,
      zisc::pmr::vector<int>* estimates_counter) const noexcept;

  void denoiseSelectedPatches(
      const Index2d& main_pixel,
      const SimilarPatchMask& similar_mask,
      Parameters* parameter,
      zisc::pmr::vector<SpectraArray>* staging_value_table,
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
  uint getPatchDimension() const noexcept;

  //! Return the order of chunk tiles
  static constexpr std::array<Index2d, 9> getChunkTileOrder() noexcept;

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
  void merge(
      System& system,
      Parameters* low_res_p,
      Parameters* high_res_p,
      zisc::pmr::vector<SpectraArray>* staging_value_table) const noexcept;

  //! Notify the denoising progress
  void notifyProgress(const double progress) const noexcept;

  //! Notify the denoising progress
  void notifyProgress(const uint iteration, const uint tile_number) const noexcept;

  //! Compute parameters for denoising
  void prepare(System& system,
               zisc::pmr::vector<Parameters>* parameters) const noexcept;

  //! Select similar patches
  SimilarPatchMask selectSimilarPatches(
      const Parameters& parameter,
      const Index2d& main_pixel) const noexcept;

  //! Convert to a matrix
  CovarianceMatrix toMatrix(const CovarianceFactors& factors)const noexcept;


  Float histogram_distance_threshold_ = 0.75;
  uint histogram_bins_ = 16;
  uint patch_radius_ = 1;
  uint search_radius_ = 3;
  uint num_of_scales_ = 2;
};

// Type aliases
using RgbBcDenoiser = BayesianCollaborativeDenoiser<3>;
using SpectraBcDenoiser = BayesianCollaborativeDenoiser<CoreConfig::spectraSize()>;

} // namespace nanairo

#include "bayesian_collaborative_denoiser-inl.hpp"

#endif // NANAIRO_BAYESIAN_COLLABORATIVE_DENOISER_HPP
