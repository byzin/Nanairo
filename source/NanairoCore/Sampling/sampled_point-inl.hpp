/*!
  \file sampled_point-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_POINT_INL_HPP
#define NANAIRO_SAMPLED_POINT_INL_HPP

#include "sampled_point.hpp"
// Zisc
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SampledPoint::SampledPoint() noexcept :
    inverse_pdf_{1.0}
{
}

/*!
  \details
  No detailed.
  */
inline
SampledPoint::SampledPoint(const Point3& point, const Float inverse_pdf) noexcept :
    point_{point},
    inverse_pdf_{inverse_pdf}
{
}

/*!
  \details
  No detailed.
  */
inline
Float SampledPoint::inversePdf() const noexcept
{
  return inverse_pdf_;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& SampledPoint::point() const noexcept
{
  return point_;
}

/*!
  \details
  No detailed.
  */
inline
Float SampledPoint::pdf() const noexcept
{
  return zisc::invert(inverse_pdf_);
}

/*!
  \details
  No detailed.
  */
inline
void SampledPoint::setInversePdf(const Float inverse_pdf) noexcept
{
  inverse_pdf_ = inverse_pdf;
}

/*!
  \details
  No detailed.
  */
inline
void SampledPoint::setPoint(const Point3& point) noexcept 
{
  point_ = point;
}

/*!
  \details
  No detailed.
  */
inline
void SampledPoint::setPdf(const Float pdf) noexcept
{
  inverse_pdf_ = zisc::invert(pdf);
}

} // namespace nanairo

#endif // NANAIRO_SAMPLED_POINT_INL_HPP
