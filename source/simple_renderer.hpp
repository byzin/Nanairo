/*!
  \file simple_renderer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SIMPLE_RENDERER_HPP
#define NANAIRO_SIMPLE_RENDERER_HPP

// Standard C++ library
#include <memory>
#include <string>
// Zisc
#include "zisc/stopwatch.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

//! Forward declaration
class SettingNodeBase;

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

  //! Load the rendering scene
  bool loadScene(const SettingNodeBase& settings,
                 std::string* error_message) noexcept;

  //! Return the max FPS
  static constexpr int maxFps() noexcept;

  //! Return the min time per frame
  static constexpr Clock::duration minTimePerFrame() noexcept;

  //! Render the scene image
  void render(const std::string& output_path) noexcept;

  //! Set the renderer state manually
  void setRunnable(const bool is_runnable) noexcept;

 protected:
  //! Set the flag of saving image at each cycle
  void enableSavingAtEachCycle(const bool flag) noexcept;

  //! Make a image path
  std::string makeImagePath(const std::string& output_path,
                            const uint64 cycle) const noexcept;

  //! Handle camera event
  virtual void handleCameraEvent(zisc::Stopwatch* stopwatch,
                                 uint64* cycle,
                                 Clock::duration* time) noexcept;

  //! Return the HDR image
  HdrImage& hdrImage() noexcept;

  //! Return the HDR image
  const HdrImage& hdrImage() const noexcept;

  //! Initialize the renderer for rendering the scene
  void initForRendering() noexcept;

  //! Return the LDR image
  LdrImage& ldrImage() noexcept;

  //! Return the LDR image
  const LdrImage& ldrImage() const noexcept;

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

  //! Return the tone mapping
  ToneMappingOperator& toneMappingOperator() noexcept;

  //! Return the tone mapping 
  const ToneMappingOperator& toneMappingOperator() const noexcept;

  //! Return the wavelength sampler
  WavelengthSampler& wavelengthSampler() noexcept;

  //! Return the wavelength sampler
  const WavelengthSampler& wavelengthSampler() const noexcept;

  //! Log a message
  virtual void logMessage(const std::string& messsage) noexcept;

  //! Output LDR image
  virtual void outputLdrImage(const std::string& output_path,
                              const uint64 cycle) noexcept;

  //! Notify of updating rendering information
  virtual void notifyOfRenderingInfo(const std::string& info) const noexcept;

 private:
  //! Convert Spectra image to HDR image
  void convertSpectraToHdr(const uint64 cycle) noexcept;

  //! Return the cycle to finish rendering
  uint64 cycleToFinish() const noexcept;

  //! Check if the LDR image is saved at each cycle
  bool isSavingAtEachCycleEnabled() const noexcept;

  //! Check if the LDR image is saved at power of 2 cycles
  bool isSavingAtPowerOf2CyclesEnabled() const noexcept;

  //! Return the next cycle to save image
  uint64 getNextCycleToSaveImage(const uint64 cycle) const noexcept;

  //! Return the next cycle to save image
  Clock::duration getNextTimeToSaveImage(const Clock::duration& time) const noexcept;

  //! Initialize the renderer
  void initialize() noexcept;

  //! Check if it is the cycle to finish rendering
  bool isCycleToFinish(const uint64 cycle) const noexcept;

  //! Check if it is the cycle to save image
  bool isCycleToSaveImage(const uint64 cycle,
                          const uint64 cycle_to_save_image) const noexcept;

  //! Check if it is the time to finish rendering
  bool isTimeToFinish(const Clock::duration& time) const noexcept;

  //! Check if it is the time to save image
  bool isTimeToSaveImage(const Clock::duration& time,
                         const Clock::duration& time_to_save_image) const noexcept;

  //! Process elapsed time per frame
  Clock::duration processElapsedTime(
      const zisc::Stopwatch& stopwatch,
      const Clock::duration& previous_time) const noexcept;

  //! Process LDR image for LodePNG
  void processLdrForLodepng() noexcept;

  //! Render the scene
  void renderScene() noexcept;

  //! Set the cycle to finish rendering
  void setCycleToFinish(const uint64 cycle) noexcept;

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

  //! Update rendering info
  void updateRenderingInfo(const uint64 cycle, const Clock::duration& time) noexcept;

  //! Current thread waits for next rendering frame
  void waitForNextFrame(const Clock::duration& wait_time) const noexcept;


  std::unique_ptr<HdrImage> hdr_image_;
  std::unique_ptr<LdrImage> ldr_image_;
  UniquePointer<RenderingMethod> rendering_method_;
  std::unique_ptr<Scene> scene_;
  std::unique_ptr<System> system_;
  UniquePointer<ToneMappingOperator> tone_mapping_operator_;
  std::unique_ptr<WavelengthSampler> wavelength_sampler_;
  Clock::duration time_to_finish_;
  Clock::duration time_interval_to_save_image_;
  uint64 cycle_to_finish_;
  bool is_saving_each_cycle_enabled_;
  bool is_saving_at_power_of_2_cycles_enabled_;
  bool is_runnable_;
};

} // namespace nanairo

#include "simple_renderer-inl.hpp"

#endif // NANAIRO_SIMPLE_RENDERER_HPP
