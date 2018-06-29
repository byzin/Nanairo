/*!
  \file shape.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SHAPE_HPP
#define NANAIRO_SHAPE_HPP

// Standard C++ library
#include <memory>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_test_result.hpp"
#include "NanairoCore/Data/shape_point.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class Aabb;
class IntersectionInfo;
class PathState;
class Ray;
class Sampler;
class System;

//! \addtogroup Core
//! \{

/*!
  */
enum class ShapeType : uint32
{
  kPlane                      = zisc::Fnv1aHash32::hash("Plane"),
  kMesh                       = zisc::Fnv1aHash32::hash("Mesh")
};

/*!
  \details
  No detailed.
  */
class Shape
{
 public:
  //! Create a shape
  Shape() noexcept;

  //!
  virtual ~Shape() noexcept;


  //! Return the bounding box
  virtual Aabb boundingBox() const noexcept = 0;

  //! Return the point and the normal by the st coordinate
  virtual ShapePoint getPoint(const Point2& st) const noexcept = 0;

  //! Return the ray traversal cost
  virtual Float getTraversalCost() const noexcept = 0;

  //! Make geometries
  static zisc::pmr::vector<zisc::UniqueMemoryPointer<Shape>> makeShape(
      System& system,
      const SettingNodeBase* settings) noexcept;

  //! Return the surface area of the shape
  Float surfaceArea() const noexcept;

  //! Test ray-shape intersection
  virtual IntersectionTestResult testIntersection(
      const Ray& ray,
      IntersectionInfo* intersection) const noexcept = 0;

  //! Sample a point randomly on the surface of the shape
  virtual ShapePoint samplePoint(Sampler& sampler,
                                 const PathState& path_state) const noexcept = 0;

  //! Set surface area of shape
  void setSurfaceArea(const Float surface_area) noexcept;

  //! Apply affine transformation
  void transform(const Matrix4x4& matrix) noexcept;

 protected:
  //! Calculate the surface area of the front side of the shape
  virtual Float calcSurfaceArea() const noexcept = 0;

  //! Apply affine transformation
  virtual void transformShape(const Matrix4x4& matrix) noexcept = 0;

 private:
  Float surface_area_;
};

//! \} Core

} // namespace nanairo

#include "shape-inl.hpp"

#endif // NANAIRO_SHAPE_HPP
