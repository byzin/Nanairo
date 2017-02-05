/*!
  \file rgb_spectra_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rgb_spectra_image.hpp"
// Standard C++ library
#include <cstddef>
#include <future>
#include <vector>
#include <utility>
// Qt
#include <QtGlobal>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "color_conversion.hpp"
#include "color_space.hpp"
#include "hdr_image.hpp"
#include "rgb_color.hpp"
#include "spectra_image_interface.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RgbSpectraImage::RgbSpectraImage(const uint width, const uint height) noexcept :
    SpectraImageInterface(width, height)
{
  initialize();
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::addContribution(
    const uint x,
    const uint y,
    const SampledSpectra& contribution) noexcept
{
  volatile Float c = 0.0;
  volatile Float tmp1 = 0.0;
  volatile Float tmp2 = 0.0;

  const uint pixel_index = widthResolution() * y + x;
  auto& pixel = buffer_[pixel_index];
  auto& compensation = compensation_[pixel_index];
  for (uint index = 0; index < 3; ++index) {
    c = compensation[index];
    tmp1 = contribution.intensity(2 - index) - c;
    tmp2 = pixel[index] + tmp1;
    compensation[index] = (tmp2 - pixel[index]) - tmp1;
    pixel[index] = tmp2;
  }
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::clear() noexcept
{
  for (auto& pixel : buffer_) {
    pixel[0] = 0.0;
    pixel[1] = 0.0;
    pixel[2] = 0.0;
  }
  for (auto& compensation : compensation_) {
    compensation[0] = 0.0;
    compensation[1] = 0.0;
    compensation[2] = 0.0;
  }
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::save(const quint64 /* cycle */,
                           const QString& /* file_path */) const noexcept
{
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::toHdrImage(System& system,
                                 const quint64 cycle,
                                 HdrImage* hdr_image) const noexcept
{
  using zisc::cast;

  const Float averager = zisc::invert(cast<Float>(cycle));
  auto to_hdr_image = [this, &system, hdr_image, averager](const int thread_id)
  {
    // Set the calculation range
    const auto range = system.calcThreadRange(hdr_image->numOfPixels(), thread_id);
    const auto begin = std::get<0>(range);
    const auto end = std::get<1>(range);
    // Write to HDR image buffer
    const auto to_xyz_matrix = getRgbToXyzMatrix(system.colorSpace());
    for (uint index = begin; index < end; ++index) {
      const auto rgb = averager * buffer_[index].data();
      const RgbColor rgb_color{rgb[0], rgb[1], rgb[2]};
      (*hdr_image)[index] = ColorConversion::toXyz(rgb_color, to_xyz_matrix);
    }
  };
  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  const uint end = thread_pool.numOfThreads();
  auto result = thread_pool.enqueueLoop(to_hdr_image, start, end);
  result.get();
}

/*!
  \details
  No detailed.
  */
SpectraImageType RgbSpectraImage::type() const noexcept
{
  return SpectraImageType::Rgb;
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::initialize() noexcept
{
  const uint buffer_size = widthResolution() * heightResolution();
  buffer_.resize(buffer_size);
  compensation_.resize(buffer_size);
}

} // namespace nanairo
