/*!
  \file sampled_point-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SAMPLED_POINT_INL_HPP_
#define _NANAIRO_SAMPLED_POINT_INL_HPP_

#include "sampled_point.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
SampledPoint::SampledPoint() :
    inverse_pdf_{1.0}
{
}

/*!
  \details
  No detailed.
  */
inline
SampledPoint::SampledPoint(const Point3& point, const Float inverse_pdf) :
    point_{point},
    inverse_pdf_{inverse_pdf}
{
}

/*!
  \details
  No detailed.
  */
inline
Float SampledPoint::inversePdf() const
{
  return inverse_pdf_;
}

/*!
  \details
  No detailed.
  */
inline
const Point3& SampledPoint::point() const
{
  return point_;
}

/*!
  \details
  No detailed.
  */
inline
Float SampledPoint::pdf() const
{
  return 1.0 / inverse_pdf_;
}

/*!
  \details
  No detailed.
  */
inline
void SampledPoint::setInversePdf(const Float inverse_pdf)
{
  inverse_pdf_ = inverse_pdf;
}

/*!
  \details
  No detailed.
  */
inline
void SampledPoint::setPoint(const Point3& point)
{
  point_ = point;
}

/*!
  \details
  No detailed.
  */
inline
void SampledPoint::setPdf(const Float pdf)
{
  inverse_pdf_ = 1.0 / pdf;
}

} // namespace nanairo

#endif // _NANAIRO_SAMPLED_POINT_INL_HPP_
