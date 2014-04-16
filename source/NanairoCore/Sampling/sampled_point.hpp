/*!
  \file sampled_point.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SAMPLED_POINT_HPP_
#define _NANAIRO_SAMPLED_POINT_HPP_

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SampledPoint
{
 public:
  //! Create sample
  SampledPoint();

  //! Create sample
  SampledPoint(const Point3& point, const Float inverse_pdf);


  //! Return the inverse pdf
  Float inversePdf() const;

  //! Return the sampled point
  const Point3& point() const;

  //! Return the pdf (probability density function) of the sampled point
  Float pdf() const;

  //! Set inverse pdf
  void setInversePdf(const Float inverse_pdf);

  //! Set point
  void setPoint(const Point3& point);

  //! Set pdf
  void setPdf(const Float pdf);

 private:
  Point3 point_;
  Float inverse_pdf_;
};

//! \} Core

} // namespace nanairo

#include "sampled_point-inl.hpp"

#endif // _NANAIRO_SAMPLED_POINT_HPP_
