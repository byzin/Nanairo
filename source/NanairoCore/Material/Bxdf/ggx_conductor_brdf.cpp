/*!
  \file ggx_conductor_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "ggx_conductor_brdf.hpp"
// Sstandard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/microfacet_ggx.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
GgxConductorBrdf::GgxConductorBrdf(
    const Float roughness_x,
    const Float roughness_y,
    const SampledSpectra& n,
    const SampledSpectra& eta) noexcept :
        n_{n},
        eta_{eta},
        roughness_x_{roughness_x},
        roughness_y_{roughness_y}
{
}

/*!
  \details
  No detailed.
  */
Float GgxConductorBrdf::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
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

  // Calculate the microfacet normal
  const auto m_normal = Microfacet::calcReflectionHalfVector(vin_d, vout_d);
  ZISC_ASSERT(zisc::isInClosedBounds(m_normal[2], 0.0, 1.0),
              "The microfacet normal isn't in the upper hemisphere.");

  // Evaluate the pdf
  const Float pdf = MicrofacetGgx::evalReflectionPdf(roughness_x_,
                                                     roughness_y_,
                                                     vin_d,
                                                     m_normal);

  return pdf;
}

/*!
  \details
  No detailed.
  */
SampledSpectra GgxConductorBrdf::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  const auto& point = info->shapePoint();
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

  // Calculate the microfacet normal
  const auto m_normal = Microfacet::calcReflectionHalfVector(vin_d, vout_d);
  ZISC_ASSERT(zisc::isInClosedBounds(m_normal[2], 0.0, 1.0),
              "The microfacet normal isn't in the upper hemisphere.");

  // Evaluate the radiance
  const auto radiance = MicrofacetGgx::evalReflectance(roughness_x_,
                                                       roughness_y_,
                                                       vin_d,
                                                       vout_d,
                                                       m_normal,
                                                       n_,
                                                       eta_);

  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> GgxConductorBrdf::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  const auto& point = info->shapePoint();
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

  // Calculate the microfacet normal
  const auto m_normal = Microfacet::calcReflectionHalfVector(vin_d, vout_d);
  ZISC_ASSERT(zisc::isInClosedBounds(m_normal[2], 0.0, 1.0),
              "The microfacet normal isn't in the upper hemisphere.");

  // Evaluate the radiance and the pdf
  Float pdf = 0.0;
  const auto radiance = MicrofacetGgx::evalReflectance(roughness_x_,
                                                       roughness_y_,
                                                       vin_d,
                                                       vout_d,
                                                       m_normal,
                                                       n_,
                                                       eta_,
                                                       &pdf);

  return std::make_tuple(radiance, pdf);
}

/*!
  */
bool GgxConductorBrdf::isReflective() const noexcept
{
  return true;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> GgxConductorBrdf::sample(
    const Vector3* vin,
    const WavelengthSamples& wavelengths,
    Sampler& sampler,
    const IntersectionInfo* info) const noexcept
{
  const auto& point = info->shapePoint();
  const auto vin_d = Transformation::toLocal(point.tangent(),
                                             point.bitangent(),
                                             point.normal(),
                                             -(*vin));
  ZISC_ASSERT(zisc::isInClosedBounds(vin_d[2], 0.0, 1.0),
              "The vin isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vin_d), "The vin isn't unit vector.");

  // Sample a microfacet normal
  const auto m_normal = MicrofacetGgx::sampleNormal(roughness_x_,
                                                    roughness_y_,
                                                    vin_d,
                                                    sampler);
  // Get the reflection direction
  auto vout = Microfacet::calcReflectionDirection(vin_d, m_normal);

  SampledSpectra weight{wavelengths};
  const Float cos_no = vout.direction()[2];
  if (0.0 < cos_no) {
    // Evaluate the fresnel term
    const Float cos_mi = zisc::dot(m_normal.direction(), vin_d);
    const auto fresnel = Fresnel::evalFresnel(n_, eta_, cos_mi);

    // Evaluate the weight
    weight = fresnel * MicrofacetGgx::evalWeight(roughness_x_,
                                                 roughness_y_,
                                                 vin_d,
                                                 vout.direction(),
                                                 m_normal.direction());
    ZISC_ASSERT(!weight.hasNegative(), "The Weight contains negative.");

    // Transform the reflection direction
    const auto vout_dir = Transformation::fromLocal(point.tangent(),
                                                    point.bitangent(),
                                                    point.normal(),
                                                    vout.direction());
    ZISC_ASSERT(isUnitVector(vout_dir), "The vout isn't unit vector.");
    vout.setDirection(vout_dir);
  }
  else {
    vout.setPdf(0.0);
  }

  return std::make_tuple(vout, weight);
}

/*!
  \details
  No detailed.
  */
bool GgxConductorBrdf::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo
