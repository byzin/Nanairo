/*!
  \file geometry.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GEOMETRY_HPP
#define NANAIRO_GEOMETRY_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
#include <tuple>
#include <utility>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class Aabb;
class IntersectionInfo;
class Ray;
class Sampler;
class SceneSettings;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Geometry
{
 public:
  //!
  virtual ~Geometry() noexcept;


  //! Return the bounding box
  virtual Aabb boundingBox() const noexcept = 0;

  //! Return the geometry size
  virtual std::size_t geometrySize() const noexcept = 0;

  //! Return the ray traversal cost
  virtual Float getTraversalCost() const noexcept = 0;

  //! Return the surface area of the geometry
  Float surfaceArea() const noexcept;

  //! Test ray-geometry intersection
  virtual bool testIntersection(const Ray& ray,
                                IntersectionInfo* intersection) const noexcept = 0;

  //! Sample a point randomly on the surface of the geometry
  virtual std::tuple<SampledPoint, Vector3> samplePoint(Sampler& sampler) const noexcept = 0;

  //! Set surface area of geometry
  void setSurfaceArea(const Float surface_area) noexcept;

  //! Apply affine transformation
  virtual void transform(const Matrix4x4& matrix) noexcept = 0;

 private:
  Float surface_area_;
};

//! Make geometries
std::vector<UniquePointer<Geometry>> makeGeometry(const SceneSettings& settings,
                                                  const QString& prefix) noexcept;

//! \} Core

} // namespace nanairo

#include "geometry-inl.hpp"

#endif // NANAIRO_GEOMETRY_HPP
