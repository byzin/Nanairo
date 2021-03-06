/*!
  \file hdr_image.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_HDR_IMAGE_HPP
#define NANAIRO_HDR_IMAGE_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/point.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "xyz_color.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/SpectralDistribution/spectral_distribution.hpp"

namespace nanairo {

// Forward declaration
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class HdrImage
{
 public:
  //! Create a hdr image
  HdrImage(const uint width,
           const uint height,
           zisc::pmr::memory_resource* data_resource) noexcept;

  //! Create a hdr image
  HdrImage(const Index2d& resolution,
           zisc::pmr::memory_resource* data_resource) noexcept;


  //! Returh the pixel color by index
  XyzColor& operator[](const uint index) noexcept;

  //! Returh the pixel color by index
  const XyzColor& operator[](const uint index) const noexcept;


  //! Return the buffer data
  zisc::pmr::vector<XyzColor>& data() noexcept;

  //! Return the buffer data
  const zisc::pmr::vector<XyzColor>& data() const noexcept;

  //! Return the pixel color by index
  XyzColor& get(const uint index) noexcept;

  //! Return the pixel color by index
  const XyzColor& get(const uint index) const noexcept;

  //! Return the pixel color
  XyzColor& get(const uint x, const uint y) noexcept;

  //! Return the pixel color
  const XyzColor& get(const uint x, const uint y) const noexcept;

  //! Return the pixel color
  XyzColor& get(const Index2d& index) noexcept;

  //! Return the pixel color
  const XyzColor& get(const Index2d& index) const noexcept;

  //! Return the height resolution
  uint heightResolution() const noexcept;

  //! Return the buffer memory size
  std::size_t memorySize() const noexcept;

  //! Return the num of pixels
  uint numOfPixels() const noexcept;

  //! Return the resolution
  const Index2d& resolution() const noexcept;

  //! Return the buffer length
  uint size() const noexcept;

  //! Set pixel color
  void set(const uint ndex, const XyzColor& color) noexcept;

  //! Set pixel color
  void set(const uint x, const uint y, const XyzColor& color) noexcept;

  //! Set pixel color
  void set(const Index2d& index, const XyzColor& color) noexcept;

  //! Convert a sample table to a HDR image
  void toHdr(System& system,
             const uint64 num_of_samples,
             const zisc::pmr::vector<SpectralDistribution::SpectralDistributionPointer>& sample_table) noexcept;

  //! Return the height resolution
  uint widthResolution() const noexcept;

 private:
  //! Initialize
  void initialize() noexcept;

  //! Convert a XY coordinate to a index
  uint toIndex(const uint x, const uint y) const noexcept;


  zisc::pmr::vector<XyzColor> buffer_;
  Index2d resolution_;
};

//! \} Core

} // namespace nanairo

#include "hdr_image-inl.hpp"

#endif // NANAIRO_HDR_IMAGE_HPP
