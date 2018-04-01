/*!
  \file film-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FILM_INL_HPP
#define NANAIRO_FILM_INL_HPP

#include "film.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo 
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectra_image_interface.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Float Film::aspectRatio() const noexcept
{
  const auto& r = imageResolution();
  const auto& inv_r = inverseResolution();
  return zisc::cast<Float>(r[0]) * inv_r[1];
}

/*!
  \details
  No detailed.
  */
inline
Point2 Film::coordinate(const Index2d& index,
                        const Vector2& jittering) const noexcept 
{
  const auto& inv_r = inverseResolution();
  const Float s = (zisc::cast<Float>(index[0]) + jittering[0]) * inv_r[0];
  const Float t = (zisc::cast<Float>(index[1]) + jittering[1]) * inv_r[1];
  ZISC_ASSERT(zisc::isInClosedBounds(s, 0.0, 1.0),
              "The coordinate s is out of the range [0, 1].");
  ZISC_ASSERT(zisc::isInClosedBounds(t, 0.0, 1.0),
              "The coordinate t is out of the range [0, 1].");
  return Point2{s, t};
}

/*!
  */
inline
void Film::clear() noexcept
{
  spectraBuffer().clear();
}

/*!
  \details
  No detailed.
  */
inline
uint Film::heightResolution() const noexcept
{
  return spectra_buffer_->heightResolution();
}

/*!
  \details
  No detailed.
  */
inline
Float Film::inverseAspectRatio() const noexcept
{
  const auto& r = imageResolution();
  const auto& inv_r = inverseResolution();
  return zisc::cast<Float>(r[1]) * inv_r[0];
}

/*!
  */
inline
const Index2d& Film::imageResolution() const noexcept
{
  return spectra_buffer_->resolution();
}

/*!
  \details
  No detailed.
  */
inline
SpectraImageInterface& Film::spectraBuffer() noexcept
{
  return *spectra_buffer_;
}

/*!
  \details
  No detailed.
  */
inline
const SpectraImageInterface& Film::spectraBuffer() const noexcept
{
  return *spectra_buffer_;
}

/*!
  \details
  No detailed.
  */
inline
uint Film::widthResolution() const noexcept
{
  return spectra_buffer_->widthResolution();
}

/*!
  */
inline
const Point2& Film::inverseResolution() const noexcept
{
  return inverse_resolution_;
}

} // namespace nanairo

#endif // NANAIRO_FILM_INL_HPP
