/*!
  \file diffuse-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_DIFFUSE_INL_HPP
#define NANAIRO_DIFFUSE_INL_HPP

#include "diffuse.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

/*!
  */
inline
Float Diffuse::evalPdf(const Vector3& vout, const Vector3& normal) noexcept
{
  constexpr Float k = zisc::invert(zisc::kPi<Float>);
  const Float cos_no = zisc::dot(normal, vout);
  ZISC_ASSERT(zisc::isInBounds(cos_no, 0.0, 1.0), "The cos is out of range [0, 1): ", cos_no);
  const Float pdf = k * cos_no;
  return pdf;
}

/*!
  */
inline
SampledSpectra Diffuse::evalRadiance(const SampledSpectra& reflectance) noexcept
{
  constexpr Float k = zisc::invert(zisc::kPi<Float>);
  const auto f = k * reflectance;
  return f;
}

/*!
  */
inline
SampledDirection Diffuse::sample(Sampler& sampler) noexcept
{
  const auto vout = SampledDirection::sampleOnHemisphere<1>(sampler);
  return vout;
}

/*!
  */
inline
SampledDirection Diffuse::sample(const ShapePoint& point, Sampler& sampler) noexcept
{
  auto vout = sample(sampler);
  {
    const auto vout_d = Transformation::fromLocal(point.tangent(),
                                                  point.bitangent(),
                                                  point.normal(),
                                                  vout.direction());
    vout.setDirection(vout_d);
  }
  return vout;
}

} // namespace nanairo

#endif // NANAIRO_DIFFUSE_INL_HPP
