/*!
  \file diffuse.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_DIFFUSE_HPP
#define NANAIRO_DIFFUSE_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

// Forward declaration
class Sampler;
class ShapePoint;

//! \addtogroup Core
//! \{

/*!
  */
class Diffuse
{
 public:
  //! Evaluate the pdf of the diffuse
  static Float evalPdf(const Vector3& vout, const Vector3& normal) noexcept;

  //! Evaluate the radiance of the diffuse
  static SampledSpectra evalRadiance(const SampledSpectra& reflectance) noexcept;

  //! Sample a reflection direction on the default basis
  static SampledDirection sample(Sampler& sampler) noexcept;

  //! Sample a reflection direction on the point
  static SampledDirection sample(const ShapePoint& point, Sampler& sampler) noexcept;
};

//! \} Core

} // namespace nanairo

#include "diffuse-inl.hpp"

#endif // NANAIRO_DIFFUSE_HPP
