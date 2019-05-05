/*!
  \file triangle_mesh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "flat_triangle.hpp"
#include "shape.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
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
zisc::pmr::vector<zisc::UniqueMemoryPointer<Shape>> TriangleMesh::makeMeshes(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto object_settings = castNode<SingleObjectSettingNode>(settings);

  auto data_resource = &system.dataMemoryManager();
  auto work_resource = settings->workResource();

  const auto& parameters = object_settings->meshParameters();
  zisc::pmr::vector<zisc::UniqueMemoryPointer<Shape>> mesh_list{work_resource};
  mesh_list.reserve(parameters.face_list_.size());

  //! \todo Add smoothed mesh
  //! \todo Add quadrangle mesh
  for (const auto& face : parameters.face_list_) {
    const auto vertices = getVertices(parameters, face);
    // Skip invisible mesh
    if (FlatTriangle::calcSurfaceArea(vertices[0], vertices[1], vertices[2]) <= 0.0)
      continue;
    auto mesh = zisc::UniqueMemoryPointer<FlatTriangle>::make(data_resource,
                                                              vertices[0],
                                                              vertices[1],
                                                              vertices[2]);
    if (face.hasVuv()) {
      const auto vuvs = getUvs(parameters, face);
      mesh->setUv(vuvs[0], vuvs[1], vuvs[2]);
    }
    mesh_list.emplace_back(std::move(mesh));
  }
  return mesh_list;
}

/*!
  */
std::array<Point3, 3> TriangleMesh::getVertices(
    const MeshParameters& parameters,
    const Face& face) noexcept
{
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
  return vertices;
}

/*!
  */
std::array<Point2, 3> TriangleMesh::getUvs(const MeshParameters& parameters,
                                           const Face& face) noexcept
{
  std::array<Point2, 3> uvs;
  const auto& uv_indices = face.triangleVuvIndices();
  for (uint i = 0; i < 3; ++i) {
    auto& uv = uvs[i];
    const auto& uv_data = parameters.vuv_list_[uv_indices[i]];
    for (uint axis = 0; axis < 2; ++axis)
      uv[axis] = zisc::cast<Float>(uv_data[axis]);
  }
  return uvs;
}

} // namespace nanairo
