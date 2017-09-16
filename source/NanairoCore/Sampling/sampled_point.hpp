/*!
  \file sampled_point.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SAMPLED_POINT_HPP
#define NANAIRO_SAMPLED_POINT_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"

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
  SampledPoint() noexcept;

  //! Create sample
  SampledPoint(const Point3& point, const Float inverse_pdf) noexcept;


  //! Return the inverse pdf
  Float inversePdf() const noexcept;

  //! Return the sampled point
  const Point3& point() const noexcept;

  //! Return the pdf (probability density function) of the sampled point
  Float pdf() const noexcept;

  //! Set inverse pdf
  void setInversePdf(const Float inverse_pdf) noexcept;

  //! Set point
  void setPoint(const Point3& point) noexcept;

  //! Set pdf
  void setPdf(const Float pdf) noexcept;

 private:
  Point3 point_;
  Float inverse_pdf_;
};

//! \} Core

} // namespace nanairo

#include "sampled_point-inl.hpp"

#endif // NANAIRO_SAMPLED_POINT_HPP
