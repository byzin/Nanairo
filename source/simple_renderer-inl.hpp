/*!
  \file simple_renderer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SIMPLE_RENDERER_INL_HPP
#define NANAIRO_SIMPLE_RENDERER_INL_HPP

#include "simple_renderer.hpp"
// Standard C++ library
#include <chrono>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"

namespace nanairo {

/*!
  */
inline
bool SimpleRenderer::isRunnable() const noexcept
{
  return is_runnable_;
}

/*!
  */
inline
constexpr int SimpleRenderer::maxFps() noexcept
{
  constexpr int max_fps = 50;
  return max_fps;
}

/*!
  */
inline
constexpr auto SimpleRenderer::minTimePerFrame() noexcept -> Clock::duration
{
  constexpr std::chrono::nanoseconds t{1'000'000'000 / maxFps()};
  constexpr auto time_per_frame = std::chrono::duration_cast<Clock::duration>(t);
  return time_per_frame;
}

/*!
  */
inline
void SimpleRenderer::setRunnable(const bool is_runnable) noexcept
{
  is_runnable_ = is_runnable;
}

/*!
  */
inline
std::string SimpleRenderer::makeImagePath(const std::string& output_path,
                                          const uint64 cycle) const noexcept
{
  std::string ldr_path = std::to_string(cycle) + "cycle.png";
  if (!output_path.empty())
    ldr_path = output_path + "/" + ldr_path;
  return ldr_path;
}

/*!
  */
inline
HdrImage& SimpleRenderer::hdrImage() noexcept
{
  ZISC_ASSERT(hdr_image_.get() != nullptr, "The HDR image is null.");
  return *hdr_image_;
}

/*!
  */
inline
const HdrImage& SimpleRenderer::hdrImage() const noexcept
{
  ZISC_ASSERT(hdr_image_.get() != nullptr, "The HDR image is null.");
  return *hdr_image_;
}

/*!
  */
inline
LdrImage& SimpleRenderer::ldrImage() noexcept
{
  ZISC_ASSERT(ldr_image_.get() != nullptr, "The LDR image is null.");
  return *ldr_image_;
}

/*!
  */
inline
const LdrImage& SimpleRenderer::ldrImage() const noexcept
{
  ZISC_ASSERT(ldr_image_.get() != nullptr, "The LDR image is null.");
  return *ldr_image_;
}

/*!
  */
inline
RenderingMethod& SimpleRenderer::renderingMethod() noexcept
{
  ZISC_ASSERT(rendering_method_.get() != nullptr, "The rendering method is null.");
  return *rendering_method_;
}

/*!
  */
inline
const RenderingMethod& SimpleRenderer::renderingMethod() const noexcept
{
  ZISC_ASSERT(rendering_method_.get() != nullptr, "The rendering method is null.");
  return *rendering_method_;
}

/*!
  */
inline
Scene& SimpleRenderer::scene() noexcept
{
  ZISC_ASSERT(scene_.get() != nullptr, "The scene is null.");
  return *scene_;
}

/*!
  */
inline
const Scene& SimpleRenderer::scene() const noexcept
{
  ZISC_ASSERT(scene_.get() != nullptr, "The scene is null.");
  return *scene_;
}

/*!
  */
inline
System& SimpleRenderer::system() noexcept
{
  ZISC_ASSERT(system_.get() != nullptr, "The system is null.");
  return *system_;
}

/*!
  */
inline
const System& SimpleRenderer::system() const noexcept
{
  ZISC_ASSERT(system_.get() != nullptr, "The system is null.");
  return *system_;
}

/*!
  */
inline
ToneMappingOperator& SimpleRenderer::toneMappingOperator() noexcept
{
  ZISC_ASSERT(tone_mapping_operator_.get() != nullptr, "The tone maping is null.");
  return *tone_mapping_operator_;
}

/*!
  */
inline
const ToneMappingOperator& SimpleRenderer::toneMappingOperator() const noexcept
{
  ZISC_ASSERT(tone_mapping_operator_.get() != nullptr, "The tone maping is null.");
  return *tone_mapping_operator_;
}

/*!
  */
inline
WavelengthSampler& SimpleRenderer::wavelengthSampler() noexcept
{
  ZISC_ASSERT(wavelength_sampler_.get() != nullptr, "The sampler is null.");
  return *wavelength_sampler_;
}

/*!
  */
inline
const WavelengthSampler& SimpleRenderer::wavelengthSampler() const noexcept
{
  ZISC_ASSERT(wavelength_sampler_.get() != nullptr, "The sampler is null.");
  return *wavelength_sampler_;
}

} // namespace nanairo

#endif // NANAIRO_SIMPLE_RENDERER_INL_HPP
