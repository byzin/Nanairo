/*!
  \file rendering_method-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_RENDERING_METHOD_INL_HPP_
#define _NANAIRO_RENDERING_METHOD_INL_HPP_

#include "rendering_method.hpp"
// Standard C++ library
#include <functional>
#include <limits>
#include <utility>
// Qt
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "path_tracing.hpp"
#include "light_tracing.hpp"
#include "probabilistic_ppm.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class Scene;
class System;

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
RenderingMethod<kSampleSize>::RenderingMethod(const SceneSettings& settings) :
    clear_function_{[](){}}
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void RenderingMethod<kSampleSize>::operator()(System& system,
                                              Scene& scene,
                                              const Wavelengths& sampled_wavelengths)
{
  render(system, scene, sampled_wavelengths);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void RenderingMethod<kSampleSize>::clear()
{
  clear_function_();
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
Float RenderingMethod<kSampleSize>::rayCastEpsilon() const
{
  return ray_cast_epsilon_;
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
IntersectionInfo RenderingMethod<kSampleSize>::castRay(
    const World& world,
    const Ray& ray,
    const Float max_distance2) const
{
  const auto& bvh = world.bvh();
  return bvh.castRay(ray, max_distance2);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
RouletteResult RenderingMethod<kSampleSize>::playRussianRoulette(
    const uint path,
    const Spectra& weight,
    Sampler& sampler) const
{
  return russian_roulette_(path, weight, sampler);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void RenderingMethod<kSampleSize>::setClearFunction(
    std::function<void ()>&& clear_function)
{
  clear_function_ = std::move(clear_function);
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void RenderingMethod<kSampleSize>::initialize(const SceneSettings& settings)
{
  const QString prefix{keyword::renderingMethod};

  // Russian rolette
  russian_roulette_ = makeRussianRoulette<kSampleSize>(settings, prefix);

  // Ray cast epsilon
  auto key = prefix + "/" + keyword::rayCastEpsilon;
  ray_cast_epsilon_ = settings.realValue(key);
  ZISC_ASSERT(0.0 < ray_cast_epsilon_, "Ray cast epsilon must be greater than 0.");
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
Ray RenderingMethod<kSampleSize>::sampleNextRay(
    const uint length,
    const Ray& ray,
    const ShaderPointer& bxdf,
    const IntersectionInfo& intersection,
    const Spectra* ray_weight,
    Spectra* next_ray_weight,
    Sampler& sampler,
    Float* inverse_direction_pdf) const
{
  ZISC_ASSERT(ray_weight != nullptr, "Must not be NULL.");
  ZISC_ASSERT(next_ray_weight != nullptr, "Must not be NULL.");

  // Sample next direction 
  const auto& wavelengths = ray_weight->wavelengths();
  const auto& vin = ray.direction();
  const auto& normal = intersection.normal();

  const auto result = bxdf->sample(&vin, normal, wavelengths, sampler);
  const auto& sampled_vout = std::get<0>(result);
  const auto& weight = std::get<1>(result);

  if (inverse_direction_pdf != nullptr)
    *inverse_direction_pdf = sampled_vout.inversePdf();

  // Play russian roulette
  const auto roulette_result = 
      playRussianRoulette(length, weight, sampler);
  if (!roulette_result.result())
    return Ray{};

  // Update ray weight
  const Float inverse_probability = 1.0 / roulette_result.probability();
  *next_ray_weight = *next_ray_weight * weight * inverse_probability;

  // Create a next ray
  const Float cos_theta_no = zisc::dot(normal, sampled_vout.direction());
  const auto ray_epsilon = (0.0 < cos_theta_no)
      ? rayCastEpsilon() * normal
      : -rayCastEpsilon() * normal;
  return Ray{intersection.point() + ray_epsilon, sampled_vout.direction()};
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
void RenderingMethod<kSampleSize>::updateSelectedWavelengthInfo(
    const ShaderPointer& bxdf,
    Spectra* weight,
    bool* wavelength_is_selected) const
{
  ZISC_ASSERT(wavelength_is_selected != nullptr, "Must not be NULL.");
  ZISC_ASSERT(weight != nullptr, "Must not be NULL");
  const auto& wavelengths = weight->wavelengths();
  const auto index = wavelengths.primaryWavelengthIndex();
  const auto k = wavelengths.primaryInverseProbability();
  if (!(*wavelength_is_selected) && bxdf->wavelengthIsSelected()) {
    const auto intensity = k * weight->intensity(index);
    weight->setIntensity(index, intensity);
    *wavelength_is_selected = true;
  }
}

/*!
  \details
  No detailed.
  */
template <uint kSampleSize> inline
UniquePointer<RenderingMethod<kSampleSize>> makeRenderingMethod(
    System& system,
    const SceneSettings& settings)
{
  using zisc::toHash32;

  RenderingMethod<kSampleSize>* method = nullptr;

  const QString prefix{keyword::renderingMethod};
  auto key = prefix + "/" + keyword::renderingMethodType;
  const auto method_type = settings.stringValue(key);

  bool glossy_photon_map = false;
  if (method_type == keyword::probabilisticPpm) {
    key = prefix + "/" + keyword::probabilisticPpm + "/" + keyword::glossyPhotonMap;
    glossy_photon_map = settings.booleanValue(key);
  }

  switch (keyword::toHash32(method_type)) {
   case toHash32(keyword::pathTracing):
    method = new PathTracing<kSampleSize>{settings};
    break;
   case toHash32(keyword::lightTracing):
    method = new LightTracing<kSampleSize>{system, settings};
    break;
//   case toHash32(keyword::probabilisticPpm):
//    if (glossy_photon_map)
//      method = new ProbabilisticPpm<kSampleSize, true>{system, settings};
//    else
//      method = new ProbabilisticPpm<kSampleSize, false>{system, settings};
//    break;
   default:
    zisc::raiseError("RenderingMethodError: Unsupported type is speficied.");
    break;
  }
  return UniquePointer<RenderingMethod<kSampleSize>>{method};
}

} // namespace nanairo

#endif // _NANAIRO_RENDERING_METHOD_INL_HPP_
