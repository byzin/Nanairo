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
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class HdrImage;
class Scene;
class System;
template <uint> class RenderingMethod;
class ToneMappingOperator;
template <uint> class WavelengthSampler;

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
  using RgbRenderingMethod = RenderingMethod<3>;
  using RgbWavelengthSampler = WavelengthSampler<3>;
  using SpectraRenderingMethod = RenderingMethod<CoreConfig::wavelengthSampleSize()>;
  using SpectraWavelengthSampler = WavelengthSampler<CoreConfig::wavelengthSampleSize()>;


  //! Convert the spectra buffer to HDR XYZ buffer
  void convertSpectraToHdr(const quint64 cycle) noexcept override;

  //! Handle the camera event
  void handleCameraEvent() noexcept override;

  //! Initialize the cpu renderer
  void initializeRenderer(const QJsonObject& settings) noexcept override;

  //! Render image
  void render(const quint64 cycle) noexcept override;

  //! Return the RGB rendering method
  const RgbRenderingMethod& rgbRenderingMethod() const noexcept;

  //! Return the RGB rendering method
  RgbRenderingMethod& rgbRenderingMethod() noexcept;

  //! Return the RGB wavelength sampler
  const RgbWavelengthSampler& rgbSampler() const noexcept;

  //! Return the RGB wavelength sampler
  RgbWavelengthSampler& rgbSampler() noexcept;

  //! Tonemap
  void toneMap() noexcept override;

  //! Return the spectra rendering method
  const SpectraRenderingMethod& renderingMethod() const noexcept;

  //! Return the spectra rendering method
  SpectraRenderingMethod& renderingMethod() noexcept;

  //! Return the wavelength sampler
  const SpectraWavelengthSampler& wavelengthSampler() const noexcept;

  //! Return the wavelength sampler
  SpectraWavelengthSampler& wavelengthSampler() noexcept;


  UniquePointer<System> system_;
  UniquePointer<Scene> scene_;
  UniquePointer<ToneMappingOperator> tone_mapping_operator_;
  UniquePointer<HdrImage> hdr_image_;
  void* wavelength_sampler_;
  void* rendering_method_;
};

//! \} Renderer

} // namespace nanairo

#endif // NANAIRO_CPU_SCENE_RENDERER_HPP
