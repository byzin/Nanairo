/*!
  \file plane.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PLANE_HPP
#define NANAIRO_PLANE_HPP

// Standard C++ library
#include <cstddef>
#include <tuple>
#include <utility>
// Nanairo
#include "shape.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"

namespace nanairo {

// Forward declaration
class Aabb;
class IntersectionInfo;
class Ray;
class Sampler;

//! \addtogroup Core 
//! \{

/*!
 \brief 3D plane
 \details
 No detailed.
 */
class Plane : public Shape
{
 public:
  //! Initialize
  Plane() noexcept;

  //! Get bounding box
  Aabb boundingBox() const noexcept override;

  //! Return the cost of a ray-plane intersection test
  Float getTraversalCost() const noexcept override;

  //! Test ray-plane intersection
  bool testIntersection(const Ray& ray, 
                        IntersectionInfo* intersection) const noexcept override;

  //! Sample a point randomly on the surface of the plane 
  std::tuple<SampledPoint, Vector3, Point2> samplePoint(
      Sampler& sampler) const noexcept override;

  //! Apply affine transformation
  void transform(const Matrix4x4& matrix) noexcept override;
 
 private:
  //! Initialize
  void initialize() noexcept;


  Point3 top_left_;
  Vector3 normal_;
  Vector3 axis1_,
          axis2_;
  Float square_width_,
        square_height_;
  Float inverse_square_width_,
        inverse_square_height_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_PLANE_HPP
