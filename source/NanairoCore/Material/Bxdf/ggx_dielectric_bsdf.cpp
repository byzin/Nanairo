/*!
  \file ggx_dielectric_bsdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "ggx_dielectric_bsdf.hpp"
// Standard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/path_state.hpp"
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/microfacet_ggx.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
GgxDielectricBsdf::GgxDielectricBsdf(
    const Float roughness_x,
    const Float roughness_y,
    const Float n) noexcept :
        roughness_x_{roughness_x},
        roughness_y_{roughness_y},
        n_{n}
{
}

/*!
  \details
  No detailed.
  */
Float GgxDielectricBsdf::evalPdf(
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
  ZISC_ASSERT(isUnitVector(vout_d), "The vout isn't unit vector.");

  // Check if the ray is reflected or refracted
  const Float cos_no = vout_d[2];
  const bool is_reflection = (0.0 < cos_no);

  // Calculate the microfacet normal
  const auto m_normal = (is_reflection)
      ? Microfacet::calcReflectionHalfVector(vin_d, vout_d)
      : Microfacet::calcRefractionHalfVector(vin_d, vout_d, n_);
  Float pdf = 0.0;
  const Float cos_mi = zisc::dot(m_normal, vin_d);
  const bool is_valid =
      (0.0 < m_normal[2]) &&
      (is_reflection || Fresnel::checkSnellsLaw(n_, cos_mi, zisc::dot(m_normal, vout_d)));
  if (is_valid) {
    // Calculate the fresnel term
    const Float fresnel = Fresnel::evalFresnel(n_, cos_mi);

    // Evaluate the pdf
    pdf = (is_reflection)
        ? (fresnel *
           MicrofacetGgx::evalReflectionPdf(roughness_x_, roughness_y_, vin_d, m_normal))
        : ((1.0 - fresnel) *
           MicrofacetGgx::evalRefractionPdf(roughness_x_, roughness_y_, vin_d, vout_d, m_normal, n_));
  }

  return pdf;
}

/*!
  \details
  No detailed.
  */
SampledSpectra GgxDielectricBsdf::evalRadiance(
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
  ZISC_ASSERT(isUnitVector(vout_d), "The vout isn't unit vector.");

  // Check if the ray is reflected or refracted
  const Float cos_no = vout_d[2];
  const bool is_reflection = (0.0 < cos_no);

  // Calculate the microfacet normal
  const auto m_normal = (is_reflection)
      ? Microfacet::calcReflectionHalfVector(vin_d, vout_d)
      : Microfacet::calcRefractionHalfVector(vin_d, vout_d, n_);
  SampledSpectra radiance{wavelengths};
  const Float cos_mi = zisc::dot(m_normal, vin_d);
  const bool is_valid =
      (0.0 < m_normal[2]) &&
      (is_reflection || Fresnel::checkSnellsLaw(n_, cos_mi, zisc::dot(m_normal, vout_d)));
  if (is_valid) {
    // Calculate the fresnel term
    const Float fresnel = Fresnel::evalFresnel(n_, cos_mi);

    // Evaluate the radiance
    const Float f = (is_reflection)
        ? MicrofacetGgx::evalReflectance(roughness_x_, roughness_y_, vin_d, vout_d, m_normal, fresnel)
        : MicrofacetGgx::evalTransmittance(roughness_x_, roughness_y_, vin_d, vout_d, m_normal, n_, fresnel);
    radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);
  }

  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> GgxDielectricBsdf::evalRadianceAndPdf(
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
  ZISC_ASSERT(isUnitVector(vout_d), "The vout isn't unit vector.");

  // Check if the ray is reflected or refracted
  const Float cos_no = vout_d[2];
  const bool is_reflection = 0.0 < cos_no;

  // Calculate the microfacet normal
  const auto m_normal = (is_reflection)
      ? Microfacet::calcReflectionHalfVector(vin_d, vout_d)
      : Microfacet::calcRefractionHalfVector(vin_d, vout_d, n_);
  SampledSpectra radiance{wavelengths};
  Float pdf = 0.0;
  const Float cos_mi = zisc::dot(m_normal, vin_d);
  const Float cos_mo = zisc::dot(m_normal, vout_d);
  const bool is_valid =
      (0.0 < m_normal[2]) &&
      (0.0 < cos_mi) &&
      (is_reflection || Fresnel::checkSnellsLaw(n_, cos_mi, cos_mo));
  if (is_valid) {
    // Calculate the fresnel term
    const Float fresnel = Fresnel::evalFresnel(n_, cos_mi);

    // Evaluate the radiance
    const Float f = (is_reflection)
        ? MicrofacetGgx::evalReflectance(roughness_x_, roughness_y_, vin_d, vout_d, m_normal, fresnel, &pdf)
        : MicrofacetGgx::evalTransmittance(roughness_x_, roughness_y_, vin_d, vout_d, m_normal, n_, fresnel, &pdf);
    radiance.setIntensity(wavelengths.primaryWavelengthIndex(), f);

    // Evaluate the pdf
    pdf = (is_reflection) ? fresnel * pdf : (1.0 - fresnel) * pdf;
  }

  return std::make_tuple(radiance, pdf);
}

/*!
  */
bool GgxDielectricBsdf::isReflective() const noexcept
{
  return true;
}

/*!
  */
bool GgxDielectricBsdf::isTransmissive() const noexcept
{
  return true;
}

/*!
  */
std::tuple<SampledDirection, SampledSpectra> GgxDielectricBsdf::sample(
    const Vector3* vin,
    const WavelengthSamples& wavelengths,
    Sampler& sampler,
    PathState& path_state,
    const IntersectionInfo* info) const noexcept
{
  ZISC_ASSERT(info != nullptr, "The info is null.");
  const auto& point = info->shapePoint();
  // Transform the incident vector
  const auto vin_d = Transformation::toLocal(point.tangent(),
                                             point.bitangent(),
                                             point.normal(),
                                             -(*vin));
  ZISC_ASSERT(zisc::isInClosedBounds(vin_d[2], 0.0, 1.0),
              "The vin isn't in the upper hemisphere.");
  ZISC_ASSERT(isUnitVector(vin_d), "The vin isn't unit vector.");

  // Sample a microfacet normal
  const auto m_normal = MicrofacetGgx::sampleNormal(roughness_x_, roughness_y_,
                                                    vin_d, sampler, path_state);

  // Evaluate the fresnel term
  const Float cos_mi = zisc::dot(m_normal.direction(), vin_d);
  const Float g2 = Fresnel::evalG2(n_, cos_mi);
  const bool is_perfect_reflection = g2 <= 0.0;
  const Float g = (!is_perfect_reflection) ? zisc::sqrt(g2) : 0.0;
  const Float fresnel = (!is_perfect_reflection)
      ? Fresnel::evalFresnelFromG(cos_mi, g)
      : 1.0; // Perfect reflection

  // Determine a reflection or a refraction
  path_state.setDimension(path_state.dimension() + 1);
  const bool is_reflection = is_perfect_reflection ||
      (sampler.draw1D(path_state) < fresnel);
  auto vout = (is_reflection)
      ? Microfacet::calcReflectionDirection(vin_d, m_normal)
      : Microfacet::calcRefractionDirection(vin_d, m_normal, n_, g);

  SampledSpectra weight{wavelengths};
  const Float cos_no = vout.direction()[2];
  if ((is_reflection && (0.0 < cos_no)) || (!is_reflection && (cos_no < 0.0))) {
    // Evaluate the weight
    const Float w = MicrofacetGgx::evalWeight(roughness_x_,
                                              roughness_y_,
                                              vin_d,
                                              vout.direction(),
                                              m_normal.direction());
    ZISC_ASSERT(0.0 <= w, "The weight is negative.");
    weight.setIntensity(wavelengths.primaryWavelengthIndex(), w);

    // Update the pdf of the outgoing direction
    vout.setInversePdf((is_reflection)
        ? vout.inversePdf() / fresnel
        : vout.inversePdf() / (1.0 - fresnel));

    // Transformation the reflection direction
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
bool GgxDielectricBsdf::wavelengthIsSelected() const noexcept
{
  return true;
}

} // namespace nanairo
