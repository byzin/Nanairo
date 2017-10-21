/*!
  \file rendering_method.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RENDERING_METHOD_HPP
#define NANAIRO_RENDERING_METHOD_HPP

// Standard C++ library
#include <functional>
#include <limits>
#include <memory>
// Zisc
#include "zisc/algorithm.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class SampledSpectra;
class Sampler;
class Scene;
class System;
class ShaderModel;
class World;

//! \addtogroup Core
//! \{

enum class RenderingMethodType : uint32
{
  kPathTracing                = zisc::toHash32("PathTracing"),
  kLightTracing               = zisc::toHash32("LightTracing"),
  kProbabilisticPpm           = zisc::toHash32("ProbabilisticPPM")
};

/*!
  \details
  No detailed.
  */
class RenderingMethod
{
 public:
  using Spectra = SampledSpectra;
  using Wavelengths = SampledWavelengths;
  using Shader = ShaderModel;
  using ShaderPointer = UniquePointer<Shader>;


  //! Initialize the rendering method
  RenderingMethod(const System& system, const SettingNodeBase* settings) noexcept;

  //! Finalize the rendering method
  virtual ~RenderingMethod() noexcept {}


  //! Render the scene
  void operator()(System& system,
                  Scene& scene,
                  const Wavelengths& sampled_wavelengths) noexcept;


  //! Initialize the method for rendering
  void initMethod() noexcept;

  //! Make rendering method
  static std::unique_ptr<RenderingMethod> makeMethod(
      System& system,
      const SettingNodeBase* settings,
      const Scene& scene) noexcept;

  //! Return the ray cast epsilon
  Float rayCastEpsilon() const noexcept;

  //! Render the scene
  virtual void render(System& system,
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths) noexcept = 0;

 protected:
  //! Calculate the num of pixel blocks
  uint calcPixelBlockSize(const uint width, const uint height) const noexcept;

  //! Calculate the max distance of the shadow ray
  Float calcShadowRayDistance(const Float diff2) const noexcept;

  //! Find and return the closest intersection of the ray
  IntersectionInfo castRay(
      const World& world,
      const Ray& ray,
      const Float max_distance = std::numeric_limits<Float>::max(),
      const bool expect_no_hit = false) const noexcept;

  //! Make a shadow ray
  Ray makeShadowRay(const Point3& source,
                    const Point3& dest,
                    const Vector3& normal) const noexcept;

  //! Play russian roulette
  RouletteResult playRussianRoulette(const uint path,
                                     const Spectra& weight,
                                     Sampler& sampler) const noexcept;

  //! Sample next ray
  Ray sampleNextRay(const uint length,
                    const Ray& ray,
                    const ShaderPointer& bxdf,
                    const IntersectionInfo& intersection,
                    Spectra* ray_weight,
                    Spectra* next_ray_weight,
                    Sampler& sampler,
                    Float* inverse_direction_pdf = nullptr) const noexcept;

  //! Set the method initializer
  void setMethodInitializer(std::function<void ()>&& initializer) noexcept;

  //! Update the wavelength selection info and the weight of the selected wavelength
  void updateSelectedWavelengthInfo(const ShaderPointer& bxdf,
                                    Spectra* weight,
                                    bool* wavelength_is_selected) const noexcept;


 private:
  //! Initialize the rendering method
  void initialize(const SettingNodeBase* settings) noexcept;

  //! Return the method initializer
  std::function<void ()>& methodInitializer() noexcept;


  std::function<void ()> method_initializer_;
  RussianRoulette russian_roulette_;
  Float ray_cast_epsilon_;
};

//! \} Core

} // namespace nanairo

#include "rendering_method-inl.hpp"

#endif // NANAIRO_RENDERING_METHOD_HPP
