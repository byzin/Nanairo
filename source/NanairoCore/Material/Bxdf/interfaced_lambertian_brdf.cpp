/*!
  \file interfaced_lambertian_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "interfaced_lambertian_brdf.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/layered_diffuse.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
InterfacedLambertianBrdf::InterfacedLambertianBrdf(
    const Float k_d,
    const Float roughness_x,
    const Float roughness_y,
    const Float n,
    Sampler& sampler) noexcept :
        sampler_{&sampler},
        k_d_{k_d},
        roughness_x_{roughness_x},
        roughness_y_{roughness_y},
        n_{n},
        re_{LayeredDiffuse::calcRe(n)}
{
}

/*!
  \details
  No detailed.
  */
Float InterfacedLambertianBrdf::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto& point = info->shapePoint();
  // Transform vectors
  const auto vin_d = Transformation::toLocal(point.tangent(),
                                             point.bitangent(),
                                             point.normal(),
                                             -(*vin));
  ZISC_ASSERT(zisc::isInClosedBounds(vin_d[2], 0.0, 1.0),
              "The vin isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vin_d), "The vin isn't unit vector.");
  const auto vout_d = Transformation::toLocal(point.tangent(),
                                              point.bitangent(),
                                              point.normal(),
                                              *vout);
  ZISC_ASSERT(zisc::isInClosedBounds(vout_d[2], 0.0, 1.0),
              "The vout isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vout_d), "The vout isn't unit vector.");

  const Float pdf = LayeredDiffuse::evalPdf(roughness_x_,
                                            roughness_y_,
                                            vin_d,
                                            vout_d,
                                            n_,
                                            k_d_,
                                            re_);

  return pdf;
}

/*!
  \details
  No detailed.
  */
SampledSpectra InterfacedLambertianBrdf::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& wavelengths,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto& point = info->shapePoint();
  // Transform vectors
  const auto vin_d = Transformation::toLocal(point.tangent(),
                                             point.bitangent(),
                                             point.normal(),
                                             -(*vin));
  ZISC_ASSERT(zisc::isInClosedBounds(vin_d[2], 0.0, 1.0),
              "The vin isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vin_d), "The vin isn't unit vector.");
  const auto vout_d = Transformation::toLocal(point.tangent(),
                                              point.bitangent(),
                                              point.normal(),
                                              *vout);
  ZISC_ASSERT(zisc::isInClosedBounds(vout_d[2], 0.0, 1.0),
              "The vout isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vout_d), "The vout isn't unit vector.");

  const Float f = LayeredDiffuse::evalReflectance(roughness_x_,
                                                  roughness_y_,
                                                  vin_d,
                                                  vout_d,
                                                  n_,
                                                  k_d_,
                                                  re_,
                                                  *sampler_);
  SampledSpectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);

  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> InterfacedLambertianBrdf::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& wavelengths,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto& point = info->shapePoint();
  // Transform vectors
  const auto vin_d = Transformation::toLocal(point.tangent(),
                                             point.bitangent(),
                                             point.normal(),
                                             -(*vin));
  ZISC_ASSERT(zisc::isInClosedBounds(vin_d[2], 0.0, 1.0),
              "The vin isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vin_d), "The vin isn't unit vector.");
  const auto vout_d = Transformation::toLocal(point.tangent(),
                                              point.bitangent(),
                                              point.normal(),
                                              *vout);
  ZISC_ASSERT(zisc::isInClosedBounds(vout_d[2], 0.0, 1.0),
              "The vout isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vout_d), "The vout isn't unit vector.");

  Float pdf = 0.0;
  const Float f = LayeredDiffuse::evalReflectance(roughness_x_,
                                                  roughness_y_,
                                                  vin_d,
                                                  vout_d,
                                                  n_,
                                                  k_d_,
                                                  re_,
                                                  *sampler_,
                                                  &pdf);
  SampledSpectra radiance{wavelengths};
  radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);

  return std::make_tuple(radiance, pdf);
}

/*!
  */
bool InterfacedLambertianBrdf::isReflective() const noexcept
{
  return true;
}

/*!
  */
std::tuple<SampledDirection, SampledSpectra> InterfacedLambertianBrdf::sample(
    const Vector3* vin,
    const WavelengthSamples& wavelengths,
    Sampler& sampler,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto& point = info->shapePoint();
  // Transform vectors
  const auto vin_d = Transformation::toLocal(point.tangent(),
                                             point.bitangent(),
                                             point.normal(),
                                             -(*vin));
  ZISC_ASSERT(zisc::isInClosedBounds(vin_d[2], 0.0, 1.0),
              "The vin isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vin_d), "The vin isn't unit vector.");

  auto result = LayeredDiffuse::sample(roughness_x_,
                                       roughness_y_,
                                       vin_d,
                                       n_,
                                       k_d_,
                                       re_,
                                       sampler);
  // Calculate the reflection direction
  auto& vout = std::get<0>(result);
  const Float cos_no = vout.direction()[2];
  if (0.0 < cos_no)
  {
    const auto vout_dir = Transformation::fromLocal(point.tangent(),
                                                    point.bitangent(),
                                                    point.normal(),
                                                    vout.direction());
    ZISC_ASSERT(isUnitVector(vout_dir), "The vout isn't unit vector.");
    vout.setDirection(vout_dir);
  }
  // Calculate the weight
  SampledSpectra weight{wavelengths};
  if (0.0 < cos_no)
  {
    const Float w = std::get<1>(result);
    ZISC_ASSERT(0.0 <= w, "The weight is negative.");
    weight.setIntensity(wavelengths.primaryWavelengthIndex(), w);
  }

  return std::make_tuple(vout, weight);
}

/*!
  \details
  No detailed.
  */
bool InterfacedLambertianBrdf::wavelengthIsSelected() const noexcept
{
  return true;
}

} // namespace nanairo
