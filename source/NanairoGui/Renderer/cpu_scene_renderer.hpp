/*!
  \file cpu_scene_renderer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class HdrImage;
class Scene;
class System;
class RenderingMethod;
class ToneMappingOperator;
class WavelengthSampler;

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

  //! Free buffers
  ~CpuSceneRenderer() noexcept override;

 private:
  //! Convert the spectra buffer to HDR XYZ buffer
  void convertSpectraToHdr(const quint64 cycle) noexcept override;

  //! Handle the camera event
  void handleCameraEvent() noexcept override;

  //! Initialize the cpu renderer
  void initializeRenderer(const SettingNodeBase* settings) noexcept override;

  //! Render image
  void render(const quint64 cycle) noexcept override;

  //! Return the instance of a rendering method
  const RenderingMethod& renderingMethod() const noexcept;

  //! Return the instance of a rendering method
  RenderingMethod& renderingMethod() noexcept;

  //! Tonemap
  void toneMap() noexcept override;

  //! Return the instance of a wavelength sampler
  const WavelengthSampler& wavelengthSampler() const noexcept;

  //! Return the instance of a wavelength sampler
  WavelengthSampler& wavelengthSampler() noexcept;


  UniquePointer<System> system_;
  UniquePointer<Scene> scene_;
  UniquePointer<ToneMappingOperator> tone_mapping_operator_;
  UniquePointer<HdrImage> hdr_image_;
  UniquePointer<WavelengthSampler> wavelength_sampler_;
  UniquePointer<RenderingMethod> rendering_method_;
};

//! \} Renderer

} // namespace nanairo

#endif // NANAIRO_CPU_SCENE_RENDERER_HPP
