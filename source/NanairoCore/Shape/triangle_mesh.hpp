/*!
  \file triangle_mesh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TRIANGLE_MESH_HPP
#define NANAIRO_TRIANGLE_MESH_HPP

// Standard C++ library
#include <memory>
#include <vector>
// Nanairo
#include "shape.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class Face;
struct MeshParameters;

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
class TriangleMesh
{
 public:
  //! Make meshes
  static std::vector<std::unique_ptr<Shape>> makeMeshes(
      const SettingNodeBase* settings) noexcept;

 private:
  //! Return the vertices of the face
  static std::array<Point3, 3> getVertices(const MeshParameters& parameters,
                                           const Face& face) noexcept;

  //! Return the UVs of the face
  static std::array<Point2, 3> getUvs(const MeshParameters& parameters,
                                      const Face& face) noexcept;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_TRIANGLE_MESH_HPP
