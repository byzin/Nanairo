/*!
  \file triangle_mesh.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "triangle_mesh.hpp"
// Standard C++ library
#include <cstdint>
#include <fstream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
// Qt
#include <QFileInfo>
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/string.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "flat_mesh.hpp"
#include "geometry.hpp"
//#include "normal_smoothed_mesh.hpp"
#include "smoothed_mesh.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Utility/floating_point.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo  {

//! Make a smoothed mesh
TriangleMesh* makeSmoothedMesh(const Point3& v0,
                               const Point3& v1, 
                               const Point3& v2,
                               const Vector3& n0, 
                               const Vector3& n1, 
                               const Vector3& n2);

/*!
  \details
  No detailed.
  */
bool calculateBarycentricCoordinate(const Ray& ray,
                                    const Point3& vertex,
                                    const Vector3* edge,
                                    Float* barycentric,
                                    Float* t)
{
  const auto direction = ray.origin() - vertex;
  const auto q = zisc::cross(direction, edge[0]);
  const auto p = zisc::cross(ray.direction(), edge[1]);
  const Float inverse_determinant = 1.0 / zisc::dot(p, edge[0]);

  // The time that tye ray intersects with this triangle
  const Float time = inverse_determinant * zisc::dot(q, edge[1]);
  if (time <= 0.0)
    return false;

  const Float u = inverse_determinant * zisc::dot(p, direction);
  if (u < 0.0 || 1.0 < u)
    return false;
  const Float v = inverse_determinant * zisc::dot(q, ray.direction());
  if (v < 0.0 || 1.0 < v)
    return false;
  if ((u + v) > 1.0)
    return false;

  barycentric[0] = u;
  barycentric[1] = v;
  barycentric[2] = 1.0 - (u + v);
  *t = time;
  return true;
}

/*!
  \details
  No detailed.
  */
TriangleMesh* makeWavefrontMesh(
    const MeshType mesh_type,
    const std::vector<const Point3*>& vertex_list,
    const std::vector<const Point2*>& texture_coordinate_list,
    const std::vector<const Vector3*>& normal_list,
    const bool has_texture_coordinate,
    const bool has_vertex_normal,
    const bool smoothing,
    std::istringstream& buffer)

{
  char delimiter;
  uint vertex_index[3];
  uint texture_index[3];
  uint normal_index[3];
  if (has_texture_coordinate && has_vertex_normal) {
    for (uint index = 0; index < 3; ++index) {
      buffer >> vertex_index[index] >> delimiter
             >> texture_index[index] >> delimiter
             >> normal_index[index];
    }
  }
  else if (!has_texture_coordinate && has_vertex_normal) {
    for (uint index = 0; index < 3; ++index)
      buffer >> vertex_index[index] >> delimiter >> delimiter >> normal_index[index];
  }
  else if (has_texture_coordinate && !has_vertex_normal) {
    for (uint index = 0; index < 3; ++index)
      buffer >> vertex_index[index] >> delimiter >> texture_index[index];
  }
  else if (!has_texture_coordinate && !has_vertex_normal) {
    for (uint index = 0; index < 3; ++index)
      buffer >> vertex_index[index];
  }
  TriangleMesh* mesh = nullptr;
  if (mesh_type == MeshType::Flat || !smoothing) {
    mesh = new FlatMesh{*(vertex_list[vertex_index[0] - 1]),
                        *(vertex_list[vertex_index[1] - 1]),
                        *(vertex_list[vertex_index[2] - 1])};
  }
  else {
    mesh = makeSmoothedMesh(*(vertex_list[vertex_index[0] - 1]),
                            *(vertex_list[vertex_index[1] - 1]),
                            *(vertex_list[vertex_index[2] - 1]),
                            *(normal_list[normal_index[0] - 1]),
                            *(normal_list[normal_index[1] - 1]),
                            *(normal_list[normal_index[2] - 1]));
  }
  if (has_texture_coordinate) {
    mesh->setTextureCoordinate(*(texture_coordinate_list[texture_index[0] - 1]),
                               *(texture_coordinate_list[texture_index[1] - 1]),
                               *(texture_coordinate_list[texture_index[2] - 1]));
  }
  return mesh;
}

/*!
  \details
  No detailed.
  */
TriangleMesh* makeSmoothedMesh(const Point3& v0,
                               const Point3& v1, 
                               const Point3& v2,
                               const Vector3& n0, 
                               const Vector3& n1, 
                               const Vector3& n2)
{
  TriangleMesh* mesh = nullptr;

  const auto c0 = zisc::dot(n0, n1);
  const auto c1 = zisc::dot(n1, n2);
  const auto c2 = zisc::dot(n0, n2);
  constexpr Float threshold = 0.9999;
  // If the mesh is mostly the same as flat, make a flat mesh instead of the smoothed.
  if ((threshold < c0) && (threshold < c1) && (threshold < c2))
    mesh = new FlatMesh{v0, v1, v2};
  else
    mesh = new SmoothedMesh{v0, v1, v2, n0, n1, n2};
  return mesh;
}

/*!
  \details
  No detailed.
  */
std::vector<UniquePointer<Geometry>> makeMeshesFromWavefront(
    const QString& object_file_path,
    const MeshType mesh_type)
{
  using zisc::toHash32;

  std::ifstream object_file{object_file_path.toStdString()};

  std::string keyword;
  std::string value;
  bool smoothing = false;
  bool has_texture_coordinate = false,
       has_vertex_normal = false;
  bool first_mesh = true;
  uint line_number = 1;
  std::list<Point3> vertexes;
  std::list<Vector3> normals;
  std::list<Point2> texture_coordinates;
  std::vector<const Point3*> vertex_list;
  std::vector<const Point2*> texture_coordinate_list;
  std::vector<const Vector3*> normal_list;
  std::list<Geometry*> meshes;
  Float x,
        y,
        z;

  for (std::string line; std::getline(object_file, line); ++line_number) {
    // Skip empty or comment line
    line = zisc::trimmed(line);
    if (line == "" || line[0] == '#')
      continue;

    std::istringstream buffer{line};
    buffer >> keyword;
    switch (toHash32(keyword.c_str())) {
     // Material name
     case toHash32("mtllib"):
       break;
     case toHash32("usemtl"):
       break;
     // Group name
     case toHash32("g"):
       break;
     // Object name
     case toHash32("o"):
       break;
     // Vertex point
     case toHash32("v"):
      buffer >> x >> y >> z;
      vertexes.emplace_back(x, y, z);
      break;
     // Vertex normal
     case toHash32("vn"):
      has_vertex_normal = true;
      buffer >> x >> y >> z;
      normals.emplace_back(Vector3{x, y, z}.normalized());
      break;
     // Texture-coordinate
     case toHash32("vt"):
      has_texture_coordinate = true;
      buffer >> x >> y;
      texture_coordinates.emplace_back(x, 1.0 - y);
      break;
     // Smoothing
     case toHash32("s"):
      buffer >> value;
      smoothing = (value == "1");
      break;
     case toHash32("f"):
      if (first_mesh) {
        // Make vertex reference list
        vertex_list.reserve(vertexes.size());
        for (const auto& vertex : vertexes)
          vertex_list.emplace_back(&vertex);
        // Make vertex normal reference list
        normal_list.reserve(normals.size());
        for (const auto& normal : normals)
          normal_list.emplace_back(&normal);
        // Make texture coordinate reference list
        texture_coordinate_list.reserve(texture_coordinates.size());
        for (const auto& coordinate : texture_coordinates)
          texture_coordinate_list.emplace_back(&coordinate);
        first_mesh = false;
      }
      meshes.emplace_back(makeWavefrontMesh(
            mesh_type, vertex_list, texture_coordinate_list, normal_list,
            has_texture_coordinate, has_vertex_normal, smoothing, buffer));
      break;
     default:
      zisc::raiseError("MeshError: In \"", object_file_path.toStdString(),
                       "\" line ", line_number, ": unsupported syntax.");
      break;
    }
  }
  std::vector<UniquePointer<Geometry>> mesh_list;
  mesh_list.reserve(meshes.size());
  for (auto* mesh : meshes)
    mesh_list.emplace_back(mesh);
  return mesh_list;
}

/*!
  \details
  No detailed.
  */
std::vector<UniquePointer<Geometry>> makeMeshes(const SceneSettings& settings,
                                                const QString& prefix)
{
  auto key = prefix + "/" + keyword::objectFilePath;
  const auto object_file_path = settings.stringValue(key);

  key = prefix + "/" + keyword::smoothing;
  const bool smoothing = settings.booleanValue(key);
  const auto mesh_type = smoothing ? MeshType::Smoothed : MeshType::Flat;

  std::vector<UniquePointer<Geometry>> mesh_list;

  const QFileInfo file_info{object_file_path};
  const auto suffix = file_info.suffix();
  if (suffix == "obj") {
    mesh_list = makeMeshesFromWavefront(object_file_path, mesh_type);
  }
  else {
    zisc::raiseError("TriangleMeshError: \"", object_file_path.toStdString(), 
                     "\" is not supported format.");
  }
  return mesh_list;
}

} // namespace nanairo 
