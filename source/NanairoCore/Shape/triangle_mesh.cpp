/*!
  \file triangle_mesh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "triangle_mesh.hpp"
// Standard C++ library
#include <array>
#include <memory>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "flat_mesh.hpp"
#include "shape.hpp"
#include "smoothed_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/face.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/single_object_setting_node.hpp"

namespace nanairo  {

/*!
  \details
  No detailed.
  */
std::vector<std::unique_ptr<Shape>> TriangleMesh::makeMeshes(
    const SettingNodeBase* settings) noexcept
{
  const auto object_settings = castNode<SingleObjectSettingNode>(settings);

  const auto& parameters = object_settings->meshParameters();
  std::vector<std::unique_ptr<Shape>> mesh_list;
  mesh_list.reserve(parameters.face_list_.size());

  //! \todo Add smoothed mesh
  //! \todo Add quadrangle mesh
  for (const auto& face : parameters.face_list_) {
    std::array<Point3, 3> vertices;
    {
      const auto& vertex_indices = face.triangleVertexIndices();
      for (uint i = 0; i < 3; ++i) {
        auto& vertex = vertices[i];
        const auto& vertex_data = parameters.vertex_list_[vertex_indices[i]];
        for (uint axis = 0; axis < 3; ++axis)
          vertex[axis] = zisc::cast<Float>(vertex_data[axis]);
      }
    }
    auto mesh = std::make_unique<FlatMesh>(vertices[0], vertices[1], vertices[2]);
    if (face.hasVuv()) {
      std::array<Point2, 3> vuvs;
      const auto& vuv_indices = face.triangleVuvIndices();
      for (uint i = 0; i < 3; ++i) {
        auto& vuv = vuvs[i];
        const auto& vuv_data = parameters.vuv_list_[vuv_indices[i]];
        for (uint axis = 0; axis < 2; ++axis)
          vuv[axis] = zisc::cast<Float>(vuv_data[axis]);
      }
      mesh->setTextureCoordinate(vuvs[0], vuvs[1], vuvs[2]);
    }
    mesh_list.emplace_back(std::move(mesh));
  }

  return mesh_list;
}

/*!
  \details
  No detailed.
  */
std::unique_ptr<Shape> TriangleMesh::makeSmoothedMesh(
    const Point3& vertex1,
    const Point3& vertex2,
    const Point3& vertex3,
    const Vector3& normal1,
    const Vector3& normal2,
    const Vector3& normal3) noexcept
{
  std::unique_ptr<TriangleMesh> mesh;

  const auto c1 = zisc::dot(normal1, normal2);
  const auto c2 = zisc::dot(normal2, normal3);
  const auto c3 = zisc::dot(normal1, normal3);
  constexpr Float threshold = 0.9999;
  // If the mesh is mostly the same as flat, make a flat mesh instead of the smoothed.
  if ((threshold < c1) && (threshold < c2) && (threshold < c3))
    mesh = std::make_unique<FlatMesh>(vertex1, vertex2, vertex3);
  else
    mesh = std::make_unique<SmoothedMesh>(vertex1, vertex2, vertex3, normal1, normal2, normal3);
  return mesh;
}

} // namespace nanairo
