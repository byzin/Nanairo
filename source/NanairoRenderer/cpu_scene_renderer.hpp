/*!
  \file cpu_scene_renderer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CPU_SCENE_RENDERER_HPP
#define NANAIRO_CPU_SCENE_RENDERER_HPP

// NanairoRenderer
#include "scene_renderer_base.hpp"
// Qt
#include <QtGlobal>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class HdrImage;
class Scene;
class System;
template <uint> class RenderingMethod;
class ToneMappingMethod;

//! \addtogroup Renderer
//! \{

/*!
  \details
  No detailed.
  */
class CpuSceneRenderer : public SceneRendererBase
{
 public:
  //! Initialize
  CpuSceneRenderer() noexcept;

 private:
  //! Convert the spectra buffer to HDR XYZ buffer
  void convertSpectraToHdr(const quint64 cycle) noexcept override;

  //! Handle the camera event
  void handleCameraEvent() noexcept override;

  //! Initialize the cpu renderer
  void initializeRenderer(const QJsonObject& settings) noexcept override;

  //! Render image
  void render(const quint64 cycle) noexcept override;

  //! Tonemap
  void toneMap() noexcept override;


  WavelengthSampler<3> rgb_sampler_;
  WavelengthSampler<kWavelengthSampleSize> wavelength_sampler_;
  UniquePointer<RenderingMethod<3>> rgb_rendering_method_;
  UniquePointer<RenderingMethod<kWavelengthSampleSize>> rendering_method_;
  UniquePointer<System> system_;
  UniquePointer<Scene> scene_;
  UniquePointer<ToneMappingMethod> tone_mapping_method_;
  UniquePointer<HdrImage> hdr_image_;
};

//! \} Renderer

} // namespace nanairo

#endif // NANAIRO_CPU_SCENE_RENDERER_HPP
