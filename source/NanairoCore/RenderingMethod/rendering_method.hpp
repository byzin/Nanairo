/*!
  \file rendering_method.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "zisc/memory_resource.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Data/rendering_tile.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class PathState;
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
  kPathTracing                = zisc::Fnv1aHash32::hash("PathTracing"),
  kLightTracing               = zisc::Fnv1aHash32::hash("LightTracing"),
  kProbabilisticPpm           = zisc::Fnv1aHash32::hash("ProbabilisticPPM")
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
  using ShaderPointer = zisc::UniqueMemoryPointer<Shader>;


  //! Initialize the rendering method
  RenderingMethod(const System& system, const SettingNodeBase* settings) noexcept;

  //! Finalize the rendering method
  virtual ~RenderingMethod() noexcept {}


  //! Render the scene
  void operator()(System& system,
                  Scene& scene,
                  const Wavelengths& sampled_wavelengths,
                  const uint32 cycle) noexcept;


  //! Initialize the method for rendering
  virtual void initMethod() noexcept;

  //! Make rendering method
  static zisc::UniqueMemoryPointer<RenderingMethod> makeMethod(
      System& system,
      const SettingNodeBase* settings,
      const Scene& scene) noexcept;

  //! Return the ray cast epsilon
  Float rayCastEpsilon() const noexcept;

  //! Render the scene
  virtual void render(System& system,
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths,
                      const uint32 cycle) noexcept = 0;

 protected:
  //! Calculate the number of rendering tiles
  uint calcNumOfTiles(const Index2d& resolution) const noexcept;

  //! Calculate the number of pixel blocks
  uint calcPixelBlockSize(const uint width, const uint height) const noexcept;

  //! Calculate the max distance of the shadow ray
  Float calcShadowRayDistance(const Float diff2) const noexcept;

  //! Find and return the closest intersection of the ray
  IntersectionInfo castRay(
      const World& world,
      const Ray& ray,
      const Float max_distance = std::numeric_limits<Float>::max(),
      const bool expect_no_hit = false) const noexcept;

  //! Get the rendering tile
  RenderingTile getRenderingTile(const Index2d& resolution,
                                 const uint index) const noexcept;

  //! Make a shadow ray
  Ray makeShadowRay(const Point3& source,
                    const Point3& dest,
                    const Vector3& normal,
                    const bool is_in_front) const noexcept;

  //! Play russian roulette
  RouletteResult playRussianRoulette(const Spectra& weight,
                                     Sampler& sampler,
                                     PathState& path_state) const noexcept;

  //! Sample next ray
  Ray sampleNextRay(const Ray& ray,
                    const ShaderPointer& bxdf,
                    const IntersectionInfo& intersection,
                    Spectra* ray_weight,
                    Spectra* next_ray_weight,
                    Sampler& sampler,
                    PathState& path_state,
                    Float* inverse_direction_pdf = nullptr) const noexcept;

  //! Update the wavelength selection info and the weight of the selected wavelength
  void updateSelectedWavelengthInfo(const ShaderPointer& bxdf,
                                    Spectra* weight,
                                    bool* wavelength_is_selected) const noexcept;


 private:
  //! Initialize the rendering method
  void initialize(const SettingNodeBase* settings) noexcept;


  RussianRoulette russian_roulette_;
  Float ray_cast_epsilon_;
};

//! \} Core

} // namespace nanairo

#include "rendering_method-inl.hpp"

#endif // NANAIRO_RENDERING_METHOD_HPP
