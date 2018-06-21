/*!
  \file simple_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "simple_renderer.hpp"
// Standard C++ library
#include <chrono>
#include <cstdio>
#include <fstream>
#include <limits>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <string_view>
// LodePNG
#ifdef NANAIRO_HAS_LODEPNG
#include "lodepng.h"
#endif // NANAIRO_HAS_LODEPNG
// Zisc
#include "zisc/error.hpp"
#include "zisc/stopwatch.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/CameraModel/film.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/Color/rgba_32.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/sample_statistics.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/Setting/scene_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/system_setting_node.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"

namespace nanairo {

/*!
  */
SimpleRenderer::SimpleRenderer() noexcept : 
  is_saving_each_cycle_enabled_{false},
  is_runnable_{false}
{
  initialize();
}

/*!
  */
SimpleRenderer::~SimpleRenderer() noexcept
{
  // Destroy before the memory resources are destroyed
  scene_.reset();
  wavelength_sampler_.reset();
  rendering_method_.reset();
  tone_mapping_operator_.reset();
  hdr_image_.reset();
  ldr_image_.reset();
}

/*!
  */
bool SimpleRenderer::loadScene(const SettingNodeBase& settings,
                               std::string* /* error_message */) noexcept
{
  using zisc::cast;

  setRunnable(true);

  const auto scene_settings = castNode<SceneSettingNode>(&settings);

  // System
  const auto system_settings = 
      castNode<SystemSettingNode>(scene_settings->systemSettingNode());
  const auto method_settings = scene_settings->renderingMethodSettingNode();
  system_ = std::make_unique<System>(system_settings, method_settings);

  std::mutex data_mutex;
  auto& data_resource = system_->dataMemoryManager();
  data_resource.setMutex(&data_mutex);

  // Scene
  scene_ = zisc::UniqueMemoryPointer<Scene>::make(&data_resource,
                                                  system(),
                                                  scene_settings);

  // Wavelength sampler
  {
    const auto& world = scene().world();
    wavelength_sampler_ = zisc::UniqueMemoryPointer<WavelengthSampler>::make(
        &data_resource,
        world,
        system_settings);
  }

  // Rendering method
  {
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
    hdr_image_ = zisc::UniqueMemoryPointer<HdrImage>::make(&data_resource,
                                                           image_resolution[0],
                                                           image_resolution[1],
                                                           &data_resource);
    ldr_image_ = zisc::UniqueMemoryPointer<LdrImage>::make(&data_resource,
                                                           image_resolution[0],
                                                           image_resolution[1],
                                                           &data_resource);
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
    enableSavingAtPowerOf2Cycles(system_settings->power2CycleSaving());
  }
  {
    const auto time = system_settings->savingIntervalTime();
    const auto saving_interval_time = std::chrono::duration_cast<Clock::duration>(
        std::chrono::milliseconds{time});
    setTimeIntervalToSave(saving_interval_time);
  }
  {
    const auto cycle = system_settings->savingIntervalCycle();
    setCycleIntervalToSave(cycle);
  }

  data_resource.setMutex(nullptr);

  return isRunnable();
}

/*!
  */
void SimpleRenderer::render(const std::string& output_path) noexcept
{
  uint64 cycle = 0;
  uint64 cycle_to_save_image = getNextCycleToSaveImage(cycle);
  auto previous_time = Clock::duration::zero();
  auto time_to_save_image = getNextTimeToSaveImage(previous_time);
  bool rendering_flag = true;

  std::ofstream text_log_stream;
  initLogger(output_path, &std::cout, &text_log_stream);

  initForRendering();
  updateRenderingInfo(cycle, previous_time);

  // Main render loop
  logMessage("Start rendering.");
  zisc::Stopwatch stopwatch;
  stopwatch.start();
  while (rendering_flag) {
    ++cycle;

    rendering_flag = isRunnable() &&
                     !isCycleToFinish(cycle) && !isTimeToFinish(previous_time);

    clearWorkMemory();
    handleCameraEvent(&stopwatch, &cycle, &previous_time);

    // Render
    renderScene(cycle);

    // Save image
    bool saving_image = checkImageSavingFlag(cycle,
                                             previous_time,
                                             &cycle_to_save_image,
                                             &time_to_save_image);
    saving_image = saving_image || !rendering_flag;
    if (saving_image) {
      convertSpectraToHdr(cycle);
      toneMap();
      outputLdrImage(output_path, cycle);
    }

    // Update time
    previous_time = processElapsedTime(stopwatch, previous_time);
    updateRenderingInfo(cycle, previous_time);
  }
}

/*!
  */
void SimpleRenderer::enableSavingAtEachCycle(const bool flag) noexcept
{
  is_saving_each_cycle_enabled_ = flag;
}

/*!
  */
void SimpleRenderer::handleCameraEvent(zisc::Stopwatch*,
                                       uint64*,
                                       Clock::duration*) noexcept
{
}

/*!
  */
void SimpleRenderer::initForRendering() noexcept
{
  if (isRunnable()) {
    renderingMethod().initMethod();
    scene().film().clear();
  }
}

/*!
  */
void SimpleRenderer::initLogger(const std::string& output_path,
                                std::ostream* console_log_stream,
                                std::ofstream* text_log_stream) noexcept
{
  // Console log stream
  log_stream_list_[0] = console_log_stream;

  // Text log stream
  std::string text_log_path{"log.txt"};
  if (!output_path.empty())
    text_log_path = output_path + "/" + text_log_path;
  text_log_stream->open(text_log_path);
  log_stream_list_[1] = text_log_stream;
}

/*!
  */
void SimpleRenderer::logMessage(const std::string_view& message) noexcept
{
  for (auto log_stream : log_stream_list_) {
    if (log_stream != nullptr)
      (*log_stream) << message << std::endl;
  }
}

/*!
  */
void SimpleRenderer::outputLdrImage(const std::string& output_path,
                                    const uint64 cycle) noexcept
{
#ifdef NANAIRO_HAS_LODEPNG
  processLdrForLodepng();

  const auto ldr_image_path = makeImagePath(output_path, cycle);
  const auto& ldr_image = ldrImage();
  const auto& buffer = ldr_image.data();
  auto error = lodepng::encode(ldr_image_path,
                               zisc::treatAs<const uint8*>(buffer.data()),
                               ldr_image.widthResolution(),
                               ldr_image.heightResolution());
  if (error) {
    const auto message = "LodePNG error[" + std::to_string(error) + "]: " +
                         lodepng_error_text(error);
    logMessage(message);
  }
#else // NANAIRO_HAS_LODEPNG
  static_cast<void>(output_path);
  static_cast<void>(cycle);
#endif // NANAIRO_HAS_LODEPNG
}

/*!
  */
void SimpleRenderer::notifyOfRenderingInfo(const std::string_view&) const noexcept
{
}

/*!
  */
inline
bool SimpleRenderer::checkImageSavingFlag(
    const uint64 cycle,
    const Clock::duration previous_time,
    uint64* cycle_to_save_image,
    Clock::duration* time_to_save_image) const noexcept
{
  // Save image
  bool saving_image = isSavingAtEachCycleEnabled();
  if (isCycleToSaveImage(cycle, *cycle_to_save_image)) {
    *cycle_to_save_image = getNextCycleToSaveImage(*cycle_to_save_image);
    saving_image = true;
  }
  if (isTimeToSaveImage(previous_time, *time_to_save_image)) {
    *time_to_save_image = getNextTimeToSaveImage(*time_to_save_image);
    saving_image = true;
  }
  return saving_image;
}

/*!
  */
inline
void SimpleRenderer::clearWorkMemory() noexcept
{
  const auto& threads = system().threadManager();

  // Clear memory resources
  auto& global_memory = system().globalMemoryManager();
  global_memory.reset();
  for (uint i = 0; i < threads.numOfThreads(); ++i) {
    auto& memory_manager = system().threadMemoryManager(i);
    memory_manager.reset();
  }
}

/*!
  */
inline
void SimpleRenderer::convertSpectraToHdr(const uint64 cycle) noexcept
{
  const auto& film = scene().film();
  const auto& sample_statistics = film.sampleStatistics();
  auto& hdr_image = hdrImage();
  hdr_image.toHdr(system(), cycle, sample_statistics.sampleTable());
}

/*!
  */
void SimpleRenderer::initialize() noexcept
{
  log_stream_list_.fill(nullptr);
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
void SimpleRenderer::renderScene(const uint64 cycle) noexcept
{
  auto& sampler = system().globalSampler();
  const auto& wavelength_sampler = wavelengthSampler();
  auto& method = renderingMethod();

  const auto sampled_wavelengths = wavelength_sampler(sampler);
  method.render(system(), scene(), sampled_wavelengths, cycle);
}

/*!
  */
void SimpleRenderer::setCycleIntervalToSave(const uint64 cycle) noexcept
{
  cycle_interval_to_save_image_ = (cycle == 0)
      ? std::numeric_limits<uint64>::max()
      : cycle;
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
void SimpleRenderer::enableSavingAtPowerOf2Cycles(const bool flag) noexcept
{
  is_saving_at_power_of_2_cycles_enabled_ = flag;
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
void SimpleRenderer::toneMap() noexcept
{
  const auto& tone_map = toneMappingOperator();
  tone_map.map(system(), hdrImage(), &ldrImage());
}

/*!
  */
void SimpleRenderer::updateRenderingInfo(const uint64 cycle,
                                         const Clock::duration& time) noexcept
{
  using namespace std::string_literals;
  using zisc::cast;

  // FPS
  double fps = 0.0;
  {
    constexpr uint64 k = cast<uint64>(Clock::period::den);
    const auto time_count = time.count();
    fps = cast<double>(k * cycle) / cast<double>((time_count == 0) ? 1 : time_count);
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

  char info_string[64];
  std::sprintf(info_string,
               "%06.2lf fps,  %010u cycles,  %02d h %02d m %02d.%03d s",
               fps,
               cast<uint32>(cycle),
               hours,
               minutes,
               seconds,
               millis);
  logMessage(info_string);
  notifyOfRenderingInfo(info_string);
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
