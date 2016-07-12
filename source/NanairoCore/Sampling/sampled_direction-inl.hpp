/*!
  \file sampled_direction-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Utility/floating_point.hpp"

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
  return 1.0 / inverse_pdf_;
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
  inverse_pdf_ = 1.0 / pdf;
}

/*!
  \details
  No detailed.
  */
template <uint kCosineWeight> inline
SampledDirection sampleDirectionOnHemisphere(Sampler& sampler) noexcept
{
  using zisc::cast;

  // Calculate phi and theta using inverse function method
  const Float u1 = sampler.sample(0.0, 1.0);
  const Float u2 = sampler.sample(0.0, 1.0);

  const Float phi = 2.0 * zisc::kPi<Float> * (u1 - 0.5);
  constexpr Float exponent = 1.0 / cast<Float>(kCosineWeight + 1);
  const Float cos_theta = zisc::pow(1.0 - u2, exponent);
  const Float sin_theta = zisc::sqrt(1.0 - cos_theta * cos_theta);

  const Vector3 direction{sin_theta * zisc::cos(phi), 
                          sin_theta * zisc::sin(phi), 
                          cos_theta};
  ZISC_ASSERT(isUnitVector(direction), "The direction must be unit vector.");

  constexpr Float t = (2.0 * zisc::kPi<Float>) / cast<Float>(kCosineWeight + 1);
  const Float inverse_pdf = t / zisc::power<kCosineWeight>(cos_theta);

  return SampledDirection{direction, inverse_pdf};
}

/*!
  \details
  No detailed.
  */
template <uint kCosineWeight> inline
SampledDirection sampleDirectionOnHemisphere(const Vector3& normal, 
                                             Sampler& sampler) noexcept
{
  auto sampled_direction = 
      sampleDirectionOnHemisphere<kCosineWeight>(sampler);
  const auto basis_matrix = makeChangeOfBasisMatrixFromLocal(normal);
  const auto& direction = sampled_direction.direction();
  sampled_direction.direction() = basis_matrix * direction;
  return sampled_direction;
}

} // namespace nanairo

#endif // NANAIRO_SAMPLED_DIRECTION_INL_HPP
