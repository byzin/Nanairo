/*!
  \file simple_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "simple_renderer.hpp"
// Standard C++ library
#include <chrono>
#include <cstdio>
#include <limits>
#include <memory>
#include <string>
// LodePNG
#include "lodepng.h"
// Zisc
#include "zisc/error.hpp"
#include "zisc/stopwatch.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/Color/rgba_32.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/Setting/scene_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/system_setting_node.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"

namespace nanairo {

/*!
  */
SimpleRenderer::SimpleRenderer(const SettingNodeBase& settings) noexcept : 
  enabling_save_at_each_cycle_{false}
{
  initialize(settings);
}

/*!
  */
void SimpleRenderer::renderImage() noexcept
{
  uint64 cycle = 0;
  uint64 cycle_to_save_image = getNextCycleToSaveImage(cycle);
  auto previous_time = Clock::duration::zero();
  auto time_to_save_image = getNextTimeToSaveImage(previous_time);

  // Main render loop
  logMessage("Start rendering.");
  zisc::Stopwatch stopwatch;
  stopwatch.start();
  while (!(isCycleToFinish(cycle) || isTimeToFinish(previous_time))) {
    ++cycle;

    // Render
    render();

    // Save image
    bool saving_image = enablingSaveAtEachCycle();
    if (isCycleToSaveImage(cycle, cycle_to_save_image)) {
      cycle_to_save_image = getNextCycleToSaveImage(cycle_to_save_image);
      saving_image = true;
    }
    if (isTimeToSaveImage(previous_time, time_to_save_image)) {
      time_to_save_image = getNextTimeToSaveImage(time_to_save_image);
      saving_image = true;
    }
    if (saving_image) {
      convertSpectraToHdr(cycle);
      toneMap();
      outputLdrImage(cycle);
    }

    // Update time
    previous_time = processElapsedTime(stopwatch, previous_time);
    logRenderingInfo(cycle, previous_time);
  }
}

/*!
  */
void SimpleRenderer::logMessage(const std::string& message) noexcept
{
  std::cout << message << std::endl;
}

/*!
  */
void SimpleRenderer::outputLdrImage(const uint64 cycle) noexcept
{
  const std::string ldr_name = std::to_string(cycle) + "cycle.png";
  const auto& ldr_image = ldrImage();
  const auto buffer = zisc::treatAs<const std::vector<uint8>*>(&ldr_image.data());
  processLdrForLodepng();
  auto error = lodepng::encode(ldr_name.c_str(),
                               *buffer,
                               ldr_image.widthResolution(),
                               ldr_image.heightResolution());
  if (error) {
    const auto message = "LodePNG error[" + std::to_string(error) + "]: " +
                         lodepng_error_text(error);
    logMessage(message);
  }
}

/*!
  */
void SimpleRenderer::setEnablingSaveAtEachCycle(const bool flag) noexcept
{
  enabling_save_at_each_cycle_ = flag;
}

/*!
  */
inline
void SimpleRenderer::convertSpectraToHdr(const uint64 cycle) noexcept
{
  const auto& film = scene().film();
  const auto& spectra_image = film.spectraBuffer();
  spectra_image.toHdrImage(system(), cycle, &hdrImage());
}

/*!
  */
inline
uint64 SimpleRenderer::cycleToFinish() const noexcept
{
  return cycle_to_finish_;
}

/*!
  */
inline
bool SimpleRenderer::enablingSaveAtEachCycle() const noexcept
{
  return enabling_save_at_each_cycle_;
}

/*!
  */
inline
bool SimpleRenderer::enablingSaveAtPowerOf2Cycles() const noexcept
{
  return enabling_save_at_power_of_2_cycles_;
}

/*!
  */
inline
uint64 SimpleRenderer::getNextCycleToSaveImage(const uint64 cycle) const noexcept
{
  const uint64 next_cycle = (enablingSaveAtPowerOf2Cycles())
      ? ((cycle == 0) ? 1 : (cycle << 1))
      : std::numeric_limits<uint64>::max();
  return next_cycle;
}

/*!
  */
inline
auto SimpleRenderer::getNextTimeToSaveImage(const Clock::duration& time)
    const noexcept -> Clock::duration
{
  const auto next_time = (timeIntervalToSaveImage() == Clock::duration::max())
      ? timeIntervalToSaveImage()
      : time + timeIntervalToSaveImage();
  return next_time;
}

/*!
  */
void SimpleRenderer::initialize(const SettingNodeBase& settings) noexcept
{
  using zisc::cast;

  const auto scene_settings = castNode<SceneSettingNode>(&settings);

  // System
  const auto system_settings = 
      castNode<SystemSettingNode>(scene_settings->systemSettingNode());
  system_ = std::make_unique<System>(system_settings);

  // Scene
  scene_ = std::make_unique<Scene>(system(), scene_settings);

  // Wavelength sampler
  {
    const auto& world = scene().world();
    wavelength_sampler_ = std::make_unique<WavelengthSampler>(world,
                                                              system_settings);
  }

  // Rendering method
  {
    const auto method_settings = scene_settings->renderingMethodSettingNode();
    rendering_method_ = RenderingMethod::makeMethod(system(),
                                                    method_settings,
                                                    scene());
  }

  // Tone mapping
  tone_mapping_operator_ = ToneMappingOperator::makeOperator(system(),
                                                             system_settings);

  // images
  {
    const auto& image_resolution = system_settings->imageResolution();
    hdr_image_ = std::make_unique<HdrImage>(image_resolution[0],
                                            image_resolution[1]);
    ldr_image_ = std::make_unique<LdrImage>(image_resolution[0],
                                            image_resolution[1]);
  }

  //
  {
    auto termination_cycle = cast<uint64>(system_settings->terminationCycle());
    setCycleToFinish(termination_cycle);
  }
  {
    const auto time = system_settings->terminationTime();
    const auto termination_time = std::chrono::duration_cast<Clock::duration>(
        std::chrono::milliseconds{time});
    setTimeToFinish(termination_time);
  }
  {
    setEnablingSaveAtPowerOf2Cycles(system_settings->power2CycleSaving());
  }
  {
    const auto time = system_settings->savingIntervalTime();
    const auto saving_interval_time = std::chrono::duration_cast<Clock::duration>(
        std::chrono::milliseconds{time});
    setTimeIntervalToSave(saving_interval_time);
  }
}

/*!
  */
inline
bool SimpleRenderer::isCycleToFinish(const uint64 cycle) const noexcept
{
  const bool is_finish_cycle = (cycleToFinish() <= cycle);
  return is_finish_cycle;
}

/*!
  */
inline
bool SimpleRenderer::isCycleToSaveImage(
    const uint64 cycle,
    const uint64 cycle_to_save_image) const noexcept
{
  const bool is_saving_cycle = (cycle_to_save_image <= cycle);
  return is_saving_cycle;
}

/*!
  */
inline
bool SimpleRenderer::isTimeToFinish(const Clock::duration& time) const noexcept
{
  const bool is_finish_time = (timeToFinish() <= time);
  return is_finish_time;
}

/*!
  */
inline
bool SimpleRenderer::isTimeToSaveImage(
    const Clock::duration& time,
    const Clock::duration& time_to_save_image) const noexcept
{
  const bool is_saving_time = (time_to_save_image <= time);
  return is_saving_time;
}

/*!
  */
void SimpleRenderer::logRenderingInfo(const uint64 cycle,
                                      const Clock::duration& time) noexcept
{
  using namespace std::string_literals;
  using zisc::cast;

  auto info_string = "000.00 fps,  00000000 cycles,  00 h 00 m 00.000 s"s;

  // FPS
  double fps = 0.0;
  {
    constexpr uint64 k = cast<uint64>(Clock::period::den);
    fps = cast<double>(k * cycle) / cast<double>(time.count());
  }

  // Time
  int hours = 0,
      minutes = 0,
      seconds = 0,
      millis = 0;
  {
    using std::chrono::duration_cast;
    const auto h = duration_cast<std::chrono::hours>(time);
    auto rest_time = time - h;
    hours = cast<int>(h.count());
    const auto m = duration_cast<std::chrono::minutes>(rest_time);
    rest_time = time - m;
    minutes = cast<int>(m.count());
    const auto s = duration_cast<std::chrono::seconds>(rest_time);
    rest_time = time - s;
    const auto mi = duration_cast<std::chrono::milliseconds>(rest_time);
    seconds = cast<int>(s.count());
    millis = cast<int>(mi.count());
  }

  std::sprintf(&info_string[0],
               "%06.2lf fps,  %08d cycles,  %02d h %02d m %02d.%03d s",
               fps,
               cast<int>(cycle),
               hours,
               minutes,
               seconds,
               millis);
  logMessage(info_string);
}

/*!
  */
inline
auto SimpleRenderer::processElapsedTime(
    const zisc::Stopwatch& stopwatch,
    const Clock::duration& previous_time) const noexcept -> Clock::duration
{
  auto elapsed_time = stopwatch.elapsedTime();
  const auto elapsed_frame_time = elapsed_time - previous_time;
  if (elapsed_frame_time < minTimePerFrame()) {
    waitForNextFrame(minTimePerFrame() - elapsed_frame_time);
    elapsed_time = stopwatch.elapsedTime();
  }
  return elapsed_time;
}

/*!
  */
void SimpleRenderer::processLdrForLodepng() noexcept
{
  auto& ldr_image = ldrImage();
  auto& buffer = ldr_image.data();
  for (auto& pixel : buffer) {
    const auto red = pixel.red();
    pixel.setRed(pixel.blue());
    pixel.setBlue(red);
  }
}

/*!
  */
inline
void SimpleRenderer::render() noexcept
{
  auto& sampler = system().globalSampler();
  const auto& wavelength_sampler = wavelengthSampler();
  auto& method = renderingMethod();

  const auto sampled_wavelengths = wavelength_sampler(sampler);
  method.render(system(), scene(), sampled_wavelengths);
}

/*!
  */
void SimpleRenderer::setCycleToFinish(const uint64 cycle) noexcept
{
  cycle_to_finish_ = (cycle == 0)
      ? std::numeric_limits<uint64>::max()
      : cycle;
}

/*!
  */
void SimpleRenderer::setEnablingSaveAtPowerOf2Cycles(const bool flag) noexcept
{
  enabling_save_at_power_of_2_cycles_ = flag;
}

/*!
  */
void SimpleRenderer::setTimeIntervalToSave(const Clock::duration& interval) noexcept
{
  time_interval_to_save_image_ = (interval == Clock::duration::zero())
      ? Clock::duration::max()
      : interval;
}

/*!
  */
void SimpleRenderer::setTimeToFinish(const Clock::duration& time) noexcept
{
  time_to_finish_ = (time == Clock::duration::zero())
      ? Clock::duration::max()
      : time;
}

/*!
  */
inline
auto SimpleRenderer::timeIntervalToSaveImage() const noexcept
    -> const Clock::duration&
{
  return time_interval_to_save_image_;
}

/*!
  */
inline
auto SimpleRenderer::timeToFinish() const noexcept -> const Clock::duration&
{
  return time_to_finish_;
}

/*!
  */
inline
void SimpleRenderer::toneMap() noexcept
{
  const auto& tone_map = toneMappingOperator();
  tone_map.map(system(), hdrImage(), &ldrImage());
}

/*!
  */
inline
void SimpleRenderer::waitForNextFrame(const Clock::duration& wait_time) const noexcept
{
  ZISC_ASSERT(Clock::duration::zero() < wait_time,
              "The wait time is less or equal than zero.");
  std::this_thread::sleep_for(wait_time);
}

} // namespace nanairo
