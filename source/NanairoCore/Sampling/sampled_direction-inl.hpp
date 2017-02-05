/*!
  \file sampled_direction-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_DIRECTION_INL_HPP
#define NANAIRO_SAMPLED_DIRECTION_INL_HPP

#include "sampled_direction.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "sampler.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SampledDirection::SampledDirection() noexcept :
    inverse_pdf_{0.0}
{
}

/*!
  \details
  No detailed.
  */
inline
SampledDirection::SampledDirection(const Vector3& direction, 
                                   const Float inverse_pdf) noexcept :
    direction_{direction},
    inverse_pdf_{inverse_pdf}
{
}

/*!
  \details
  No detailed.
  */
inline
Vector3& SampledDirection::direction() noexcept
{
  return direction_;
}

/*!
  \details
  No detailed.
  */
inline
const Vector3& SampledDirection::direction() const noexcept
{
  return direction_;
}

/*!
  \details
  No detailed.
  */
inline
Float SampledDirection::inversePdf() const noexcept
{
  return inverse_pdf_;
}

/*!
  \details
  No detailed.
  */
inline
Float SampledDirection::pdf() const noexcept
{
  return zisc::invert(inverse_pdf_);
}

/*!
  \details
  No detailed.
  */
template <uint kCosineWeight> inline
SampledDirection SampledDirection::sampleOnHemisphere(
    const Vector3& normal,
    Sampler& sampler) noexcept
{
  auto sampled_direction = sampleOnHemisphere<kCosineWeight>(sampler);
  const auto basis_matrix = Transformation::makeChangeOfBasisFromLocal(normal);
  const auto& direction = sampled_direction.direction();
  sampled_direction.direction() = basis_matrix * direction;
  return sampled_direction;
}

/*!
  \details
  No detailed.
  */
inline
void SampledDirection::setDirection(const Vector3& direction) noexcept
{
  direction_ = direction;
}

/*!
  \details
  No detailed.
  */
inline
void SampledDirection::setInversePdf(const Float inverse_pdf) noexcept
{
  inverse_pdf_ = inverse_pdf;
}

/*!
  \details
  No detailed.
  */
inline
void SampledDirection::setPdf(const Float pdf) noexcept
{
  inverse_pdf_ = (pdf != 0.0) ? zisc::invert(pdf) : 0.0;
}

/*!
  \details
  No detailed.
  */
template <uint kCosineWeight> inline
SampledDirection SampledDirection::SampledDirection::sampleOnHemisphere(
    Sampler& sampler) noexcept
{
  using zisc::cast;

  // Calculate phi and theta using inverse function method
  const Float u1 = sampler.sample();
  const Float u2 = sampler.sample();

  constexpr Float exponent = zisc::invert(cast<Float>(kCosineWeight + 1));
  const Float cos_theta = zisc::pow(1.0 - u1, exponent);
  const Float sin_theta = zisc::sqrt(1.0 - cos_theta * cos_theta);
  const Float phi = 2.0 * zisc::kPi<Float> * (u2 - 0.5);

  const Vector3 direction{sin_theta * zisc::cos(phi),
                          sin_theta * zisc::sin(phi),
                          cos_theta};
  ZISC_ASSERT(isUnitVector(direction), "The direction isn't unit vector.");

  constexpr Float t = (2.0 * zisc::kPi<Float>) / cast<Float>(kCosineWeight + 1);
  const Float inverse_pdf = t / zisc::power<kCosineWeight>(cos_theta);

  return SampledDirection{direction, inverse_pdf};
}

} // namespace nanairo

#endif // NANAIRO_SAMPLED_DIRECTION_INL_HPP
