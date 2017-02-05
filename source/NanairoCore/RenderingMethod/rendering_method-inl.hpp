/*!
  \file rendering_method-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RENDERING_METHOD_INL_HPP
#define NANAIRO_RENDERING_METHOD_INL_HPP

#include "rendering_method.hpp"
// Standard C++ library
#include <functional>
#include <limits>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
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
inline
void RenderingMethod::operator()(System& system,
                                 Scene& scene,
                                 const Wavelengths& sampled_wavelengths) noexcept
{
  render(system, scene, sampled_wavelengths);
}

/*!
  \details
  No detailed.
  */
inline
void RenderingMethod::clear() noexcept
{
  if (clear_function_)
    clear_function_();
}

/*!
  \details
  No detailed.
  */
inline
Float RenderingMethod::rayCastEpsilon() const noexcept
{
  return ray_cast_epsilon_;
}

/*!
  */
inline
Float RenderingMethod::calcShadowRayDistance(const Float diff2) const noexcept
{
  constexpr Float distance_epsilon = 0.000001;
  constexpr Float extension = 1.0 + distance_epsilon;
  static_assert(1.0 < extension, "The extension isn't greater than 1.");
  return extension * zisc::sqrt(diff2);
}

/*!
  \details
  No detailed.
  */
inline
IntersectionInfo RenderingMethod::castRay(const World& world,
                                          const Ray& ray,
                                          const Float max_distance) const noexcept
{
  const auto& bvh = world.bvh();
  return bvh.castRay(ray, max_distance);
}

/*!
  */
inline
Ray RenderingMethod::makeShadowRay(const Point3& source,
                                   const Point3& dest,
                                   const Vector3& normal) const noexcept
{
  const auto ray_epsilon = rayCastEpsilon() * normal;
  ZISC_ASSERT(!isZeroVector(ray_epsilon), "Ray epsilon is zero vector.");
  const auto origin = source + ray_epsilon;
  const auto dir = (dest - source).normalized();
  return Ray{origin, dir};
}

/*!
  \details
  No detailed.
  */
inline
RouletteResult RenderingMethod::playRussianRoulette(const uint path,
                                                    const Spectra& weight,
                                                    Sampler& sampler) const noexcept
{
  return russian_roulette_(path, weight, sampler);
}

/*!
  \details
  No detailed.
  */
inline
void RenderingMethod::setClearFunction(
    std::function<void ()>&& clear_function) noexcept
{
  clear_function_ = std::move(clear_function);
}

/*!
  \details
  No detailed.
  */
inline
Ray RenderingMethod::sampleNextRay(const uint length,
                                   const Ray& ray,
                                   const ShaderPointer& bxdf,
                                   const IntersectionInfo& intersection,
                                   Spectra* ray_weight,
                                   Spectra* next_ray_weight,
                                   Sampler& sampler,
                                   Float* inverse_direction_pdf) const noexcept
{
  ZISC_ASSERT(ray_weight != nullptr, "The ray_weight is null.");
  ZISC_ASSERT(next_ray_weight != nullptr, "The next_ray_weight is null.");

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
  const auto next_weight = *ray_weight * weight;
  const auto roulette_result = playRussianRoulette(length, next_weight, sampler);
  if (!roulette_result.result())
    return Ray{};
  // Update ray weight
  const Float inverse_probability = zisc::invert(roulette_result.probability());
  *ray_weight = *ray_weight * inverse_probability;
  *next_ray_weight = next_weight * inverse_probability;

  // Create a next ray
  const Float cos_theta_no = zisc::dot(normal, sampled_vout.direction());
  const auto ray_epsilon = (0.0 < cos_theta_no)
      ? rayCastEpsilon() * normal
      : -rayCastEpsilon() * normal;
  ZISC_ASSERT(!isZeroVector(ray_epsilon), "The ray epsilon is zero vector.");
  return Ray{intersection.point() + ray_epsilon, sampled_vout.direction()};
}

/*!
  \details
  No detailed.
  */
inline
void RenderingMethod::updateSelectedWavelengthInfo(
    const ShaderPointer& bxdf,
    Spectra* weight,
    bool* wavelength_is_selected) const noexcept
{
  ZISC_ASSERT(wavelength_is_selected != nullptr, "The w_is_selected is null.");
  ZISC_ASSERT(weight != nullptr, "The weight is null.");
  const auto& wavelengths = weight->wavelengths();
  const auto index = wavelengths.primaryWavelengthIndex();
  const auto k = wavelengths.primaryInverseProbability();
  if (!(*wavelength_is_selected) && bxdf->wavelengthIsSelected()) {
    const auto intensity = k * weight->intensity(index);
    weight->setIntensity(index, intensity);
    *wavelength_is_selected = true;
  }
}

} // namespace nanairo

#endif // NANAIRO_RENDERING_METHOD_INL_HPP
