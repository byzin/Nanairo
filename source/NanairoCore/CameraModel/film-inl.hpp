/*!
  \file film-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sample_statistics.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
Float Film::aspectRatio() const noexcept
{
  const auto r = imageResolution();
  const Float aspect_ratio = zisc::cast<Float>(r[0]) / zisc::cast<Float>(r[1]);
  return aspect_ratio;
}

/*!
  \details
  No detailed.
  */
inline
Point2 Film::coordinate(const Index2d& index,
                        const Vector2& jittering) const noexcept 
{
  using zisc::cast;

  const auto r = imageResolution();
  const Float s = (cast<Float>(index[0]) + jittering[0]) / cast<Float>(r[0]);
  const Float t = (cast<Float>(index[1]) + jittering[1]) / cast<Float>(r[1]);
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
  sample_statistics_.clear();
}

/*!
  \details
  No detailed.
  */
inline
uint Film::heightResolution() const noexcept
{
  const auto r = imageResolution();
  return r[1];
}

/*!
  */
inline
Index2d Film::imageResolution() const noexcept
{
  return sample_statistics_.resolution();
}

/*!
  */
inline
SampleStatistics& Film::sampleStatistics() noexcept
{
  return sample_statistics_;
}

/*!
  */
inline
const SampleStatistics& Film::sampleStatistics() const noexcept
{
  return sample_statistics_;
}

/*!
  \details
  No detailed.
  */
inline
uint Film::widthResolution() const noexcept
{
  const auto r = imageResolution();
  return r[0];
}

} // namespace nanairo

#endif // NANAIRO_FILM_INL_HPP
