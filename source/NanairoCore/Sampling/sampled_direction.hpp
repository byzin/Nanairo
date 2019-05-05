/*!
  \file sampled_direction.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_DIRECTION_HPP
#define NANAIRO_SAMPLED_DIRECTION_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

// Forward declaration
class PathState;
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
  SampledDirection() noexcept;

  //! Create sample
  SampledDirection(const Vector3& direction, const Float inverse_pdf) noexcept;


  //! Return the sampled direction
  Vector3& direction() noexcept;

  //! Return the sampled direction
  const Vector3& direction() const noexcept;

  //! Return the inverse pdf
  Float inversePdf() const noexcept;

  //! Return the pdf (probability density function) of the sampled direction
  Float pdf() const noexcept;

  //! Sample direction on hemisphere following cos(theta)^kN pdf
  template <uint kN>
  static SampledDirection sampleOnHemisphere(Sampler& sampler,
                                             const PathState& path_state) noexcept;

  //! Set direction
  void setDirection(const Vector3& direction) noexcept;

  //! Set inverse pdf
  void setInversePdf(const Float inverse_pdf) noexcept;

  //! Set pdf of the sampled direction
  void setPdf(const Float pdf) noexcept;

 private:
  Vector3 direction_;
  Float inverse_pdf_;
};

//! \} Core

} // namespace nanairo

#include "sampled_direction-inl.hpp"

#endif // NANAIRO_SAMPLED_DIRECTION_HPP
