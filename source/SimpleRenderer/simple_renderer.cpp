/*!
  \file simple_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "simple_renderer.hpp"
// Standard C++ library
#include <array>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <limits>
#include <memory>
#include <mutex>
#include <ostream>
#include <ratio>
#include <string>
#include <string_view>
// LodePNG
#ifdef NANAIRO_HAS_LODEPNG
#include "lodepng.h"
#endif // NANAIRO_HAS_LODEPNG
// Zisc
#include "zisc/error.hpp"
#include "zisc/function_reference.hpp"
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
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Denoiser/denoiser.hpp"
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
  log_stream_{nullptr},
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
  hdr_image_.reset();
  ldr_image_.reset();
}

/*!
  */
bool SimpleRenderer::loadScene(const SettingNodeBase& settings,
                               std::string* /* error_message */) noexcept
{
  using namespace std::string_literals;
  using zisc::cast;

  setRunnable(true);

  const auto scene_settings = castNode<SceneSettingNode>(&settings);
  {
    const auto message = "Construct a scene \""s +
                         scene_settings->sceneName().data() + "\".";
    logMessage(message);
  }

  // System
  const auto system_settings = 
      castNode<SystemSettingNode>(scene_settings->systemSettingNode());
  system_ = std::make_unique<System>(system_settings);

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
    const auto method_settings = scene_settings->renderingMethodSettingNode();
    rendering_method_ = RenderingMethod::makeMethod(system(),
                                                    method_settings,
                                                    scene());
  }

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
    auto termination_cycle = system_settings->terminationCycle();
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
  uint32 cycle = 0;
  uint32 cycle_to_save_image = getNextCycleToSaveImage(cycle);
  auto previous_time = system().stopwatch().elapsedTime();
  auto time_to_save_image = getNextTimeToSaveImage(previous_time);
  bool rendering_flag = true;

  initForRendering();
  updateRenderingProgress(cycle, previous_time);

  // Main render loop
  while (rendering_flag) {
    ++cycle;

    const bool is_last_cycle = isCycleToFinish(cycle) ||
                               isTimeToFinish(previous_time);
    rendering_flag = isRunnable() && !is_last_cycle;

    clearWorkMemory();
    handleCameraEvent(&cycle, &previous_time);

    // Render
    renderScene(cycle);

    // Save image
    bool saving_image = checkImageSavingFlag(cycle,
                                             previous_time,
                                             &cycle_to_save_image,
                                             &time_to_save_image);
    saving_image = saving_image || !rendering_flag;

    // Update rendered image and and rendering progress
    if (saving_image)
      outputRenderedImage(output_path, cycle);
    auto current_time = processElapsedTime(previous_time);
    updateRenderingProgress(cycle, current_time);

    // Compute denoised image and update rendering progress
    if (saving_image && is_last_cycle) {
      const auto& statistics = scene().film().sampleStatistics();
      if (statistics.isEnabled(SampleStatistics::Type::kDenoisedExpectedValue)) {
        clearWorkMemory();
        outputDenoisedImage(output_path, cycle);
        current_time = processElapsedTime(previous_time);
        updateRenderingProgress(cycle, current_time);
      }
    }

    previous_time = current_time;
  }
}

/*!
  */
void SimpleRenderer::setLogStream(std::ostream* log_stream) noexcept
{
  log_stream_ = log_stream;
}

/*!
  */
void SimpleRenderer::setProgressCallback(
    const zisc::FunctionReference<void (double, std::string_view)>& callback)
        noexcept
{
  progress_callback_ = callback;
}

/*!
  */
void SimpleRenderer::enableSavingAtEachCycle(const bool flag) noexcept
{
  is_saving_each_cycle_enabled_ = flag;
}

/*!
  */
void SimpleRenderer::handleCameraEvent(uint32*,
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
void SimpleRenderer::logMessage(const std::string_view& message) noexcept
{
  if (log_stream_ != nullptr)
    (*log_stream_) << message << std::endl;
}

/*!
  */
void SimpleRenderer::outputLdrImage(const std::string_view output_path,
                                    const uint32 cycle,
                                    const std::string_view suffix) noexcept
{
#ifdef NANAIRO_HAS_LODEPNG
  processLdrForLodepng();

  const auto ldr_image_path = makeImagePath(output_path, cycle, suffix);
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
  static_cast<void>(suffix);
#endif // NANAIRO_HAS_LODEPNG
}

/*!
  */
inline
bool SimpleRenderer::checkImageSavingFlag(
    const uint32 cycle,
    const Clock::duration previous_time,
    uint32* cycle_to_save_image,
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
double SimpleRenderer::getCurrentFps(const uint32 cycle,
                                     const Clock::duration& time) const noexcept
{
  static_assert(std::milli::den <= Clock::period::den, 
                "The tick period should be smaller or equal than milli.");

  using zisc::cast;

  constexpr uint64 k = cast<uint64>(Clock::period::den);
  const auto time_count = time.count();
  const double fps = cast<double>(k * cycle) /
                     cast<double>((time_count == 0) ? 1 : time_count);

  return fps;
}

/*!
  */
std::array<int, 4> SimpleRenderer::getCurrentTime(const Clock::duration& time)
    const noexcept
{
  static_assert(std::milli::den <= Clock::period::den, 
                "The tick period should be smaller or equal than milli.");

  using zisc::cast;
  using std::chrono::duration_cast;

  const auto h = duration_cast<std::chrono::hours>(time);
  auto rest_time = time - h;
  const int hours = cast<int>(h.count());

  const auto m = duration_cast<std::chrono::minutes>(rest_time);
  rest_time = time - m;
  const int minutes = cast<int>(m.count());

  const auto s = duration_cast<std::chrono::seconds>(rest_time);
  rest_time = time - s;
  const int seconds = cast<int>(s.count());

  const auto mi = duration_cast<std::chrono::milliseconds>(rest_time);
  const int millis = cast<int>(mi.count());

  return {{hours, minutes, seconds, millis}};
}

/*!
  */
void SimpleRenderer::initialize() noexcept
{
}

/*!
  */
void SimpleRenderer::notifyOfDenoisingProgress(const double progress) const noexcept 
{
  using namespace std::string_literals;
  if (progress_callback_) {
    const auto elapsed_time = system().stopwatch().elapsedTime();
    const auto times = getCurrentTime(elapsed_time);

    auto status = "Denoising...                     0000 h 00 m 00.000 s"s;
    std::sprintf(status.data(),
                 "Denoising...                     %04d h %02d m %02d.%03d s",
                 times[0],
                 times[1],
                 times[2],
                 times[3]);
    progress_callback_(progress, status);
  }
}

/*!
  */
void SimpleRenderer::notifyOfRenderingProgress(
    const uint32 cycle,
    const Clock::duration& time,
    const std::string_view& status) const noexcept
{
  if (progress_callback_) {
    const double cycle_progress = (0 < cycleToFinish())
        ? zisc::cast<double>(cycle) / zisc::cast<double>(cycleToFinish())
        : 0.0;
    const auto time_to_finish = timeToFinish().count();
    const double time_progress = (0 < time_to_finish)
        ? zisc::cast<double>(time.count()) / zisc::cast<double>(time_to_finish)
        : 0.0;
    double progress = zisc::max(cycle_progress, time_progress);
    progress = zisc::clamp(progress, 0.0, 1.0);
    progress_callback_(progress, status);
  }
}

/*!
  */
inline
void SimpleRenderer::outputDenoisedImage(
    const std::string& output_path,
    const uint32 cycle) noexcept
{
  auto& sample_statistics = scene().film().sampleStatistics();
  auto& denoiser = system().denoiser();

  // Set progress callback
  auto notify_progress = [this](const double progress)
  {
    notifyOfDenoisingProgress(progress);
  };
  denoiser.setProgressCallback(notify_progress);

  // Start denoising
  denoiser.denoise(system(), cycle, &sample_statistics);

  // Convert sampled value to HDR imave
  auto& hdr_image = hdrImage();
  hdr_image.toHdr(system(), 1, sample_statistics.denoisedSampleTable());

  toneMap();
  outputLdrImage(output_path, cycle, "cycle-denoised");
}

/*!
  */
inline
void SimpleRenderer::outputRenderedImage(
    const std::string& output_path,
    const uint32 cycle) noexcept
{
  const auto& film = scene().film();
  const auto& sample_statistics = film.sampleStatistics();

  // Convert sampled value to HDR imave
  auto& hdr_image = hdrImage();
  hdr_image.toHdr(system(), cycle, sample_statistics.sampleTable());

  toneMap();
  outputLdrImage(output_path, cycle, "cycle");
}

/*!
  */
inline
auto SimpleRenderer::processElapsedTime(
    const Clock::duration& previous_time) const noexcept -> Clock::duration
{
  auto elapsed_time = system().stopwatch().elapsedTime();
  const auto elapsed_frame_time = elapsed_time - previous_time;
  if (elapsed_frame_time < minTimePerFrame()) {
    waitForNextFrame(minTimePerFrame() - elapsed_frame_time);
    elapsed_time = system().stopwatch().elapsedTime();
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
void SimpleRenderer::renderScene(const uint32 cycle) noexcept
{
  auto& sampler = system().globalSampler();
  PathState path_state{cycle};
  path_state.setDimension(SampleDimension::kWavelengthSample1);

  const auto& wavelength_sampler = wavelengthSampler();
  const auto sampled_wavelengths = wavelength_sampler(sampler, path_state);

  auto& method = renderingMethod();
  method.render(system(), scene(), sampled_wavelengths, cycle);

  auto& sample_statistics = scene().film().sampleStatistics();
  sample_statistics.update(system(), sampled_wavelengths.wavelengths(), cycle);
}

/*!
  */
void SimpleRenderer::setCycleIntervalToSave(const uint32 cycle) noexcept
{
  cycle_interval_to_save_image_ = (cycle == 0)
      ? std::numeric_limits<uint32>::max()
      : cycle;
}

/*!
  */
void SimpleRenderer::setCycleToFinish(const uint32 cycle) noexcept
{
  cycle_to_finish_ = (cycle == 0)
      ? std::numeric_limits<uint32>::max()
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
  const auto& tone_map = system().toneMappingOperator();
  tone_map.map(system(), hdrImage(), &ldrImage());
}

/*!
  */
void SimpleRenderer::updateRenderingProgress(const uint32 cycle,
                                             const Clock::duration& time) noexcept
{
  using namespace std::string_literals;

  const double fps = getCurrentFps(cycle, time);
  const auto times = getCurrentTime(time);

  auto status = "000.00 fps,  0000000000 cycles,  0000 h 00 m 00.000 s"s;
  std::sprintf(status.data(),
               "%06.2lf fps,  %010u cycles,  %04d h %02d m %02d.%03d s",
               fps,
               cycle,
               times[0],
               times[1],
               times[2],
               times[3]);

  logMessage(status);
  notifyOfRenderingProgress(cycle, time, status);
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

/*!
  */
std::unique_ptr<std::ofstream> makeTextLogStream(const std::string& output_path)
{
  using namespace std::string_literals;
  auto log_stream = std::make_unique<std::ofstream>();
  const auto log_file_name = "log.txt"s;
  const auto log_file_path = (output_path.empty())
      ? log_file_name
      : output_path + "/" + log_file_name;
  log_stream->open(log_file_path);
  if (!log_stream->is_open()) {
    std::cerr << "Error: Log file (\"" << log_file_path << "\") open failed."
              << std::endl;
    log_stream.reset();
  }
  return log_stream;
}

} // namespace nanairo
