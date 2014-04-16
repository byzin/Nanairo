/*!
  \file triangle_mesh.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_TRIANGLE_MESH_HPP_
#define _NANAIRO_TRIANGLE_MESH_HPP_

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
class QString;

namespace nanairo {

// Forward declaration
class Ray;
class SceneSettings;

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
                            const Point2& texture_coordinate2);

  //! Calculate the texture coordinate using Barycentric coordinate system
  Point2 textureCoordinate(const Float* barycentric) const;

 private:
  Point2 texture_coordinate_[3];
};

//! Calculate Barycentric coordinate
bool calculateBarycentricCoordinate(const Ray& ray,
                                    const Point3& vertex,
                                    const Vector3* edge,
                                    Float* barycentric,
                                    Float* t);

//! Make a polygon
std::vector<UniquePointer<Geometry>> makeMeshes(const SceneSettings& settings,
                                                const QString& prefix);

//! \} Core 

} // namespace nanairo

#include "triangle_mesh-inl.hpp"

#endif // _NANAIRO_TRIANGLE_MESH_HPP_
