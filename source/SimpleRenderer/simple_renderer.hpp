/*!
  \file simple_renderer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SIMPLE_RENDERER_HPP
#define NANAIRO_SIMPLE_RENDERER_HPP

// Standard C++ library
#include <fstream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
// Zisc
#include "zisc/function_reference.hpp"
#include "zisc/stopwatch.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"

namespace nanairo {

//! Forward declaration
class SettingNodeBase;
class WavelengthSamples;

/*!
  */
class SimpleRenderer
{
 public:
  using Clock = zisc::Stopwatch::Clock;


  //! Create a renderer
  SimpleRenderer() noexcept;

  //!
  virtual ~SimpleRenderer() noexcept;


  //! Check if the renderer is runnable
  bool isRunnable() const noexcept;

  //! Return the LDR image
  LdrImage& ldrImage() noexcept;

  //! Return the LDR image
  const LdrImage& ldrImage() const noexcept;

  //! Load the rendering scene
  bool loadScene(const SettingNodeBase& settings,
                 std::string* error_message) noexcept;

  //! Return the max FPS
  static constexpr int maxFps() noexcept;

  //! Return the min time per frame
  static constexpr Clock::duration minTimePerFrame() noexcept;

  //! Render the scene image
  void render(const std::string& output_path) noexcept;

  //! Set a log stream
  void setLogStream(std::ostream* log_stream) noexcept;

  //! Set a progress callback
  void setProgressCallback(
      const zisc::FunctionReference<void (double, std::string_view)>& callback)
          noexcept;

  //! Set the renderer state manually
  void setRunnable(const bool is_runnable) noexcept;

 protected:
  //! Set the flag of saving image at each cycle
  void enableSavingAtEachCycle(const bool flag) noexcept;

  //! Make a image path
  std::string makeImagePath(const std::string_view output_path,
                            const uint32 cycle,
                            const std::string_view suffix = "") const noexcept;

  //! Handle camera event
  virtual void handleCameraEvent(uint32* cycle,
                                 Clock::duration* time) noexcept;

  //! Return the HDR image
  HdrImage& hdrImage() noexcept;

  //! Return the HDR image
  const HdrImage& hdrImage() const noexcept;

  //! Initialize the renderer for rendering the scene
  void initForRendering() noexcept;

  //! Return the rendering method
  RenderingMethod& renderingMethod() noexcept;

  //! Return the rendering method
  const RenderingMethod& renderingMethod() const noexcept;

  //! Return the scene 
  Scene& scene() noexcept;

  //! Return the scene 
  const Scene& scene() const noexcept;

  //! Return the system
  System& system() noexcept;

  //! Return the system
  const System& system() const noexcept;

  //! Return the wavelength sampler
  WavelengthSampler& wavelengthSampler() noexcept;

  //! Return the wavelength sampler
  const WavelengthSampler& wavelengthSampler() const noexcept;

  //! Log a message
  void logMessage(const std::string_view& messsage) noexcept;

  //! Output LDR image
  virtual void outputLdrImage(const std::string_view output_path,
                              const uint32 cycle,
                              const std::string_view suffix = "") noexcept;

 private:
  //! Check if the rendered result should be saved
  bool checkImageSavingFlag(const uint32 cycle,
                            const Clock::duration previous_time,
                            uint32* cycle_to_save_image,
                            Clock::duration* time_to_save_image) const noexcept;

  //! Clear work memories of system
  void clearWorkMemory() noexcept;

  //! Return the cycle interval to save image
  uint32 cycleIntervalToSaveImage() const noexcept;

  //! Return the cycle to finish rendering
  uint32 cycleToFinish() const noexcept;

  //! Check if the LDR image is saved at each cycle
  bool isSavingAtEachCycleEnabled() const noexcept;

  //! Check if the LDR image is saved at power of 2 cycles
  bool isSavingAtPowerOf2CyclesEnabled() const noexcept;

  //! Return the next cycle to save image
  uint32 getNextCycleToSaveImage(const uint32 cycle) const noexcept;

  //! Return the next cycle to save image
  Clock::duration getNextTimeToSaveImage(const Clock::duration& time) const noexcept;

  //! Initialize the renderer
  void initialize() noexcept;

  //! Check if it is the cycle to finish rendering
  bool isCycleToFinish(const uint32 cycle) const noexcept;

  //! Check if it is the cycle to save image
  bool isCycleToSaveImage(const uint32 cycle,
                          const uint32 cycle_to_save_image) const noexcept;

  //! Check if it is the time to finish rendering
  bool isTimeToFinish(const Clock::duration& time) const noexcept;

  //! Check if it is the time to save image
  bool isTimeToSaveImage(const Clock::duration& time,
                         const Clock::duration& time_to_save_image) const noexcept;

  //! Notify of rendering progress
  void notifyOfRenderingProgress(const uint32 cycle,
                                 const Clock::duration& time,
                                 const std::string_view& status) const noexcept;

  //! Output rendered image
  void outputDenoisedImage(const std::string& output_path,
                           const uint32 cycle) noexcept;

  //! Output rendered image
  void outputRenderedImage(const std::string& output_path,
                           const uint32 cycle) noexcept;

  //! Process elapsed time per frame
  Clock::duration processElapsedTime(
      const Clock::duration& previous_time) const noexcept;

  //! Process LDR image for LodePNG
  void processLdrForLodepng() noexcept;

  //! Render the scene
  void renderScene(const uint32 cycle) noexcept;

  //! Set the cycle interval to save image
  void setCycleIntervalToSave(const uint32 cycle) noexcept;

  //! Set the cycle to finish rendering
  void setCycleToFinish(const uint32 cycle) noexcept;

  //! Set the flag of saving image at power of 2 cycles
  void enableSavingAtPowerOf2Cycles(const bool flag) noexcept;

  //! Set the time interval to save image
  void setTimeIntervalToSave(const Clock::duration& interval) noexcept;

  //! Set the time to finish rendering
  void setTimeToFinish(const Clock::duration& time) noexcept;

  //! Return the time interval to save image
  const Clock::duration& timeIntervalToSaveImage() const noexcept;

  //! Return the time to finish rendering
  const Clock::duration& timeToFinish() const noexcept;

  //! Apply tone mapping
  void toneMap() noexcept;

  //! Update rendering progress
  void updateRenderingProgress(const uint32 cycle,
                               const Clock::duration& time) noexcept;

  //! Current thread waits for next rendering frame
  void waitForNextFrame(const Clock::duration& wait_time) const noexcept;


  std::unique_ptr<System> system_;
  zisc::UniqueMemoryPointer<Scene> scene_;
  zisc::UniqueMemoryPointer<WavelengthSampler> wavelength_sampler_;
  zisc::UniqueMemoryPointer<RenderingMethod> rendering_method_;
  zisc::UniqueMemoryPointer<HdrImage> hdr_image_;
  zisc::UniqueMemoryPointer<LdrImage> ldr_image_;
  zisc::FunctionReference<void (double, std::string_view)> progress_callback_;
  std::ostream* log_stream_;
  Clock::duration time_to_finish_;
  Clock::duration time_interval_to_save_image_;
  uint32 cycle_to_finish_;
  uint32 cycle_interval_to_save_image_;
  bool is_saving_each_cycle_enabled_;
  bool is_saving_at_power_of_2_cycles_enabled_;
  bool is_runnable_;
};

//! Make a text log stream
std::unique_ptr<std::ofstream> makeTextLogStream(const std::string& output_path);

} // namespace nanairo

#include "simple_renderer-inl.hpp"

#endif // NANAIRO_SIMPLE_RENDERER_HPP
