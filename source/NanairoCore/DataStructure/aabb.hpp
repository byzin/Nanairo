/*!
  \file aabb.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_AABB_HPP
#define NANAIRO_AABB_HPP

// Standard C++ library
#include <tuple>
#include <utility>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"

namespace nanairo {

// Forward declaration
class Ray;

//! \addtogroup Core
//! \{

/*!
 \brief AABB (Axis Aligned Bounding Box)
 \details
 No detailed.
 */
class Aabb
{
 public:
  //! Create AABB
  Aabb() noexcept;

  //! Create AABB
  Aabb(const Point3& min, const Point3& max) noexcept;


  //! Return the centroid point of AABB
  Point3 centroid() const noexcept;

  //! Test ray-AABB intersection
  std::tuple<bool, Float> testIntersection(const Ray& ray) const noexcept;

  //! Return the longest axis number
  uint longestAxis() const noexcept;

  //! Return the maximym point of AABB
  const Point3& maxPoint() const noexcept;

  //! Return the minimum point of AABB
  const Point3& minPoint() const noexcept;

  //! Set the maximum point of AABB
  void setMaxPoint(const Point3& point) noexcept;

  //! Set the minimum point of AABB
  void setMinPoint(const Point3& point) noexcept;

  //! Return the surface area
  Float surfaceArea() const noexcept;

 private:
  Point3 point_[2]; // Minimum and maximum point
};

//! Combine bounding boxs
Aabb combineBoundingBox(const Aabb& a, const Aabb& b) noexcept;

//! \} Core

} // namespace nanairo

#include "aabb-inl.hpp"

#endif // NANAIRO_AABB_HPP
