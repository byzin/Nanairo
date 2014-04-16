/*!
  \file rgb_spectra_image.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "color_space.hpp"
#include "hdr_image.hpp"
#include "rgb_color.hpp"
#include "spectra_image_interface.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
RgbSpectraImage::RgbSpectraImage(const uint width, const uint height) :
    SpectraImageInterface(width, height)
{
  initialize();
}

/*!
  \details
  No detailed.
  */
std::size_t RgbSpectraImage::bufferMemorySize() const
{
  constexpr std::size_t pixel_memory_size = sizeof(RgbColor);
  const std::size_t pixels = widthResolution() * heightResolution();
  const std::size_t byte = pixels * pixel_memory_size * 2;
  return byte;
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::clear()
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
                           const QString& /* file_path */) const
{
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::toHdrImage(System& system,
                                 const quint64 cycle,
                                 HdrImage* hdr_image) const
{
  using zisc::cast;

  const Float averager = 1.0 / cast<Float>(cycle);

  std::function<void (const uint)> to_hdr_image{
  [this, &system, hdr_image, averager](const uint y)
  {
    const auto to_xyz_matrix = getRgbToXyzMatrix(system.colorSpace());
    const uint width = widthResolution();
    for (uint index = y * width; index < (y + 1) * width; ++index) {
      const RgbColor rgb{averager * buffer_[index].data()};
      (*hdr_image)[index] = rgb.toXyz(to_xyz_matrix);
    }
  }};

  auto& thread_pool = system.threadPool();
  constexpr uint start = 0;
  auto result = thread_pool.loop(std::move(to_hdr_image), start, heightResolution());
  result.get();
}

/*!
  \details
  No detailed.
  */
SpectraImageType RgbSpectraImage::type() const
{
  return SpectraImageType::Rgb;
}

/*!
  \details
  No detailed.
  */
void RgbSpectraImage::initialize()
{
  const uint buffer_size = widthResolution() * heightResolution();
  buffer_.resize(buffer_size);
  compensation_.resize(buffer_size);
}

} // namespace nanairo
