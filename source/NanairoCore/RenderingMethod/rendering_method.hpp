/*!
  \file rendering_method.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RENDERING_METHOD_HPP_
#define _NANAIRO_RENDERING_METHOD_HPP_

// Standard C++ library
#include <functional>
#include <limits>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_wavelengths.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
template <uint> class SampledSpectra;
class Sampler;
class SceneSettings;
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
  RenderingMethod(const SceneSettings& settings);

  //! Finalize the rendering method
  virtual ~RenderingMethod() {}


  //! Render the scene
  void operator()(System& system, 
                  Scene& scene, 
                  const Wavelengths& sampled_wavelengths);


  //! Initialize the method
  void clear();

  //! Return the ray cast epsilon
  Float rayCastEpsilon() const;

  //! Render the scene
  virtual void render(System& system, 
                      Scene& scene,
                      const Wavelengths& sampled_wavelengths) = 0;

 protected:
  //! Find and return the closest intersection of the ray
  IntersectionInfo castRay(
      const World& world,
      const Ray& ray,
      const Float max_distance2 = std::numeric_limits<Float>::max()) const;

  //! Play russian roulette
  RouletteResult playRussianRoulette(const uint path,
                                     const Spectra& weight,
                                     Sampler& sampler) const;

  //! Set clear function
  void setClearFunction(std::function<void ()>&& clear_function);

  //! Sample next ray
  Ray sampleNextRay(const uint length,
                    const Ray& ray,
                    const ShaderPointer& bxdf,
                    const IntersectionInfo& intersection,
                    const Spectra* ray_weight,
                    Spectra* next_ray_weight,
                    Sampler& sampler,
                    Float* inverse_direction_pdf = nullptr) const;

  //! Update the wavelength selection info and the weight of the selected wavelength
  void updateSelectedWavelengthInfo(const ShaderPointer& bxdf,
                                    Spectra* weight,
                                    bool* wavelength_is_selected) const;
        

 private:
  //! Initialize the rendering method
  void initialize(const SceneSettings& settings);


  std::function<void ()> clear_function_;
  RussianRouletteFunction<kSampleSize> russian_roulette_;
  Float ray_cast_epsilon_;
};

//! Make rendering method
template <uint kSampleSize>
UniquePointer<RenderingMethod<kSampleSize>> makeRenderingMethod(
    System& system,
    const SceneSettings& settings);

//! \} Core 

} // namespace nanairo

#include "rendering_method-inl.hpp"

#endif // _NANAIRO_RENDERING_METHOD_HPP_
