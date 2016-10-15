/*!
  \file triangle_mesh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TRIANGLE_MESH_HPP
#define NANAIRO_TRIANGLE_MESH_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "geometry.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class Ray;

//! \addtogroup Core 
//! \{

/*!
  \details
  No detailed.
  */
enum class MeshType : int
{
  Flat = 0,
//  NormalSmoothed,
  Smoothed
};

/*!
  \details
  No detailed.
  */
class TriangleMesh : public Geometry
{
 public:
  //! Set vertex texture coordinate
  void setTextureCoordinate(const Point2& texture_coordinate0,
                            const Point2& texture_coordinate1,
                            const Point2& texture_coordinate2) noexcept;

  //! Calculate the texture coordinate using Barycentric coordinate system
  Point2 textureCoordinate(const Float* barycentric) const noexcept;

 private:
  Point2 texture_coordinate_[3];
};

//! Calculate Barycentric coordinate
bool calculateBarycentricCoordinate(const Ray& ray,
                                    const Point3& vertex,
                                    const Vector3* edge,
                                    Float* barycentric,
                                    Float* t) noexcept;

//! Make meshes
std::vector<UniquePointer<Geometry>> makeMeshes(const QJsonObject& settings) noexcept;

//! Make a smoothed mesh
UniquePointer<Geometry> makeSmoothedMesh(const Point3& v0,
                                         const Point3& v1,
                                         const Point3& v2,
                                         const Vector3& n0,
                                         const Vector3& n1,
                                         const Vector3& n2) noexcept;

//! \} Core 

} // namespace nanairo

#include "triangle_mesh-inl.hpp"

#endif // NANAIRO_TRIANGLE_MESH_HPP
