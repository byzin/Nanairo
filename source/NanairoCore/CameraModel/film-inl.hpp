/*!
  \file film-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_FILM_INL_HPP_
#define _NANAIRO_FILM_INL_HPP_

#include "film.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo 
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectra_image_interface.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Float Film::aspectRatio() const
{
  using zisc::cast;
  return cast<Float>(widthResolution()) * inverse_height_;
}

/*!
  \details
  No detailed.
  */
inline
Point2 Film::coordinate(const uint x, const uint y, const Vector2& jittering) const 
{
  using zisc::cast;
  const Float u = 0.5 - (cast<Float>(x) + jittering[0]) * inverse_width_;
  const Float v = 0.5 - (cast<float>(y) + jittering[1]) * inverse_height_;
  ZISC_ASSERT((-0.5 <= u) && (u <= 0.5), 
              "The coordinate u must be [0, 1].");
  ZISC_ASSERT((-0.5 <= v) && (v <= 0.5), 
              "The coordinate v must be [0, 1].");
  return Point2{u, v};
}

/*!
  \details
  No detailed.
  */
inline
uint Film::heightResolution() const
{
  return spectra_buffer_->heightResolution();
}

/*!
  \details
  No detailed.
  */
inline
Float Film::inverseAspectRatio() const
{
  using zisc::cast;
  return cast<Float>(heightResolution()) * inverse_width_;
}

/*!
  \details
  No detailed.
  */
inline
SpectraImageInterface& Film::spectraBuffer()
{
  return *spectra_buffer_;
}

/*!
  \details
  No detailed.
  */
inline
const SpectraImageInterface& Film::spectraBuffer() const
{
  return *spectra_buffer_;
}

/*!
  \details
  No detailed.
  */
inline
uint Film::widthResolution() const
{
  return spectra_buffer_->widthResolution();
}

} // namespace nanairo

#endif // _NANAIRO_FILM_INL_HPP_
