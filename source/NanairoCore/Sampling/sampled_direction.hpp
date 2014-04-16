/*!
  \file sampled_direction.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SAMPLED_DIRECTION_HPP_
#define _NANAIRO_SAMPLED_DIRECTION_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

// Forward declaration
class Sampler;

//! \addtogroup Core 
//! \{

/*!
  \details
  No detailed.
  */
class SampledDirection
{
 public:
  //! Create sample
  SampledDirection();

  //! Create sample
  SampledDirection(const Vector3& direction, const Float inverse_pdf);


  //! Return the sampled direction
  Vector3& direction();

  //! Return the sampled direction
  const Vector3& direction() const;

  //! Return the inverse pdf
  Float inversePdf() const;

  //! Return the pdf (probability density function) of the sampled direction
  Float pdf() const;

  //! Set direction
  void setDirection(const Vector3& direction);

  //! Set inverse pdf
  void setInversePdf(const Float inverse_pdf);

  //! Set pdf of the sampled direction
  void setPdf(const Float pdf);

 private:
  Vector3 direction_;
  Float inverse_pdf_;
};

//! Sample direction on hemisphere following cos theta pdf
template <uint kN>
SampledDirection sampleDirectionOnHemisphere(Sampler& sampler);

//! Sample direction on hemisphere following cos theta pdf
template <uint kN>
SampledDirection sampleDirectionOnHemisphere(const Vector3& normal, Sampler& sampler);

//! \} Core

} // namespace nanairo

#include "sampled_direction-inl.hpp"

#endif // _NANAIRO_SAMPLED_DIRECTION_HPP_
