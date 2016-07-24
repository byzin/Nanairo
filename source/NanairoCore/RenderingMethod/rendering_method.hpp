/*!
  \file rendering_method.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RENDERING_METHOD_HPP
#define NANAIRO_RENDERING_METHOD_HPP

// Standard C++ library
#include <functional>
#include <limits>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class IntersectionInfo;
template <uint> class SampledSpectra;
class Sampler;
class Scene;
class System;
template <uint> class ShaderModel;
class World;

//! \addtogroup Core 
//! \{

/*!
  \details
  No detailed.
  */
template <uint kSampleSize>
class RenderingMethod
{
 public:
  using Spectra = SampledSpectra<kSampleSize>;
  using Wavelengths = SampledWavelengths<kSampleSize>;
  using Shader = ShaderModel<kSampleSize>;
  using ShaderPointer = UniquePointer<Shader>;


  //! Initialize the rendering method
  RenderingMethod(const QJsonObject& settings) noexcept;

  //! Finalize the rendering method
  virtual ~RenderingMethod() noexcept {}


  //! Render the scene
  void operator()(System& system, 
                  Scene& scene, 
                  const Wavelengths& sampled_wavelengths) noexcept;


  //! Initialize the method
  void clear() noexcept;

  //! Return the ray cast epsilon
  Float rayCastEpsilon() const noexcept;

  //! Render the scene
  virtual void render(System& system, 
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths) noexcept = 0;

 protected:
  //! Find and return the closest intersection of the ray
  IntersectionInfo castRay(
      const World& world,
      const Ray& ray,
      const Float max_distance2 = std::numeric_limits<Float>::max()) const noexcept;

  //! Play russian roulette
  RouletteResult playRussianRoulette(const uint path,
                                     const Spectra& weight,
                                     Sampler& sampler) const noexcept;

  //! Set clear function
  void setClearFunction(std::function<void ()>&& clear_function) noexcept;

  //! Sample next ray
  Ray sampleNextRay(const uint length,
                    const Ray& ray,
                    const ShaderPointer& bxdf,
                    const IntersectionInfo& intersection,
                    const Spectra* ray_weight,
                    Spectra* next_ray_weight,
                    Sampler& sampler,
                    Float* inverse_direction_pdf = nullptr) const noexcept;

  //! Update the wavelength selection info and the weight of the selected wavelength
  void updateSelectedWavelengthInfo(const ShaderPointer& bxdf,
                                    Spectra* weight,
                                    bool* wavelength_is_selected) const noexcept;
        

 private:
  //! Initialize the rendering method
  void initialize(const QJsonObject& settings) noexcept;


  std::function<void ()> clear_function_;
  RussianRouletteFunction<kSampleSize> russian_roulette_;
  Float ray_cast_epsilon_;
};

//! Make rendering method
template <uint kSampleSize>
UniquePointer<RenderingMethod<kSampleSize>> makeRenderingMethod(
    System& system,
    const QJsonObject& settings) noexcept;

//! \} Core 

} // namespace nanairo

#include "rendering_method-inl.hpp"

#endif // NANAIRO_RENDERING_METHOD_HPP
