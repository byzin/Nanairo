/*!
  \file spectra_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "spectra_image.hpp"
// Standard C++ library
#include <cstddef>
#include <functional>
#include <vector>
// Qt
#include <QByteArray>
#include <QFile>
#include <QtGlobal>
// Zisc
#include "zisc/math.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "color_space.hpp"
#include "hdr_image.hpp"
#include "rgb_color.hpp"
#include "spectral_distribution.hpp"
#include "xyz_color.hpp"
#include "xyz_color_matching_function.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Utility/file.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SpectraImage::SpectraImage(const uint width, const uint height) noexcept :
    SpectraImageInterface(width, height)
{
  initialize();
}

/*!
  \details
  No detailed.
  */
void SpectraImage::addContribution(
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
  for (uint i = 0; i < SampledSpectra::size(); ++i) {
    const uint index = getIndex(contribution.wavelength(i));
    c = compensation[index];
    tmp1 = contribution.intensity(i) - c;
    tmp2 = pixel[index] + tmp1;
    compensation[index] = (tmp2 - pixel[index]) - tmp1;
    pixel[index] = tmp2;
  }
}

/*!
  \details
  No detailed.
  */
void SpectraImage::clear() noexcept
{
  for (auto& pixel : buffer_)
    pixel.fill(0.0);
  for (auto& compensation : compensation_)
    compensation.fill(0.0);
}

/*!
  \details
  No detailed.
  */
void SpectraImage::save(const quint64 /* cycle */,
                        const QString& /* file_path */) const noexcept
{
  ZISC_ASSERT(false, "Not implemented.");
}

/*!
  \details
  No detailed.
  */
void SpectraImage::toHdrImage(System& system,
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
    const auto& cmf = system.xyzColorMatchingFunction();
    for (uint index = begin; index < end; ++index)
      (*hdr_image)[index] = cmf.toXyzForEmitter(buffer_[index]) * averager;
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
SpectraImageType SpectraImage::type() const noexcept
{
  return SpectraImageType::Spectra;
}

/*!
  \details
  No detailed.
  */
void SpectraImage::initialize() noexcept
{
  const uint buffer_size = widthResolution() * heightResolution();
  buffer_.resize(buffer_size);
  compensation_.resize(buffer_size);
}

} // namespace nanairo
