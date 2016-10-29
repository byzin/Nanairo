/*!
  \file obj_loader.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "obj_loader.hpp"
// Standard C++ library
#include <tuple>
#include <vector>
#include <utility>
// Qt
#include <QTextStream>
#include <QtGlobal>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "flat_mesh.hpp"
#include "smoothed_mesh.hpp"
#include "shape.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo  {

/*!
  */
std::vector<UniquePointer<Shape>> ObjLoader::parse(QTextStream& obj_stream,
                                                      const MeshType type) noexcept
{
  ZISC_ASSERT(obj_stream.device() != 0, "The obj stream isn't initialized.");
  // Count the num of meshes
  obj_stream.seek(0);
  const auto result = countNumOfMeshes(obj_stream);
  const auto num_of_faces = std::get<3>(result);
  if (num_of_faces == 0)
    qFatal("The num of faces is 0.");
  // Make meshes
  obj_stream.seek(0);
  return makeMeshes(obj_stream, result, type);
}

/*!
  */
std::tuple<uint, uint, uint, uint> ObjLoader::countNumOfMeshes(
    QTextStream& obj_stream) noexcept
{
  using keyword::toHash32;
  using zisc::toHash32;

  uint num_of_vertices = 0;
  uint num_of_vnormals = 0; // Vertex normals
  uint num_of_vtextures = 0; // Vertex texture coordinates
  uint num_of_faces = 0;
  QString value;
  for (QString line; obj_stream.readLineInto(&line);) {
    // Skip a empty line
    if (line.isEmpty())
      continue;

    // Check a keyword
    QTextStream buffer{&line, QIODevice::ReadOnly | QIODevice::Text};
    buffer >> value;
    // Count
    switch (toHash32(value)) {
     case toHash32("v"): {
      ++num_of_vertices;
      break;
     }
     case toHash32("vn"): {
      ++num_of_vnormals;
      break;
     }
     case toHash32("vt"): {
      ++num_of_vtextures;
      break;
     }
     case toHash32("f"): {
      ++num_of_faces;
      break;
     }
     default:
      break;
    }
  }
  return std::make_tuple(num_of_vertices,
                         num_of_vnormals,
                         num_of_vtextures,
                         num_of_faces);
}

/*!
  */
UniquePointer<Shape> ObjLoader::makeMesh(
    QTextStream& face_line,
    const std::vector<Point3>& vertices,
    const std::vector<Vector3>& vnormals,
    const std::vector<Point2>& vtextures,
    const MeshType mesh_type,
    const bool smoothing) noexcept
{
  const bool has_vnormal = (0 < vnormals.size());
  const bool has_vtexture = (0 < vtextures.size());
  char delimiter;
  uint vertex_index[3];
  uint vnormal_index[3];
  uint vtexture_index[3];
  for (uint i = 0; i < 3; ++i) {
    if (has_vnormal && has_vtexture) {
      face_line >> vertex_index[i] >> delimiter
                >> vtexture_index[i] >> delimiter
                >> vnormal_index[i];
    }
    else if (has_vnormal && !has_vtexture) {
      face_line >> vertex_index[i] >> delimiter >> delimiter
                >> vnormal_index[i];
    }
    else if (!has_vnormal && has_vtexture) {
      face_line >> vertex_index[i] >> delimiter >> vtexture_index[i];
    }
    else {
      face_line >> vertex_index[i];
    }
  }
  UniquePointer<Shape> mesh;
  if (mesh_type == MeshType::Flat || !smoothing) {
    mesh.reset(new FlatMesh{vertices[vertex_index[0] - 1],
                            vertices[vertex_index[1] - 1],
                            vertices[vertex_index[2] - 1]});
  }
  else {
    mesh = TriangleMesh::makeSmoothedMesh(vertices[vertex_index[0] - 1],
                                          vertices[vertex_index[1] - 1],
                                          vertices[vertex_index[2] - 1],
                                          vnormals[vnormal_index[0] - 1],
                                          vnormals[vnormal_index[1] - 1],
                                          vnormals[vnormal_index[2] - 1]);
  }
  if (has_vtexture) {
    auto triangle = zisc::cast<TriangleMesh*>(mesh.get());
    triangle->setTextureCoordinate(vtextures[vtexture_index[0] - 1],
                                   vtextures[vtexture_index[1] - 1],
                                   vtextures[vtexture_index[2] - 1]);
  }
  return mesh;
}

/*!
  */
std::vector<UniquePointer<Shape>> ObjLoader::makeMeshes(
    QTextStream& obj_stream,
    const std::tuple<uint, uint, uint, uint>& mesh_info,
    const MeshType type) noexcept
{
  using keyword::toHash32;
  using zisc::toHash32;

  // Vertices
  const uint num_of_vertices = std::get<0>(mesh_info);
  std::vector<Point3> vertices;
  vertices.reserve(num_of_vertices);
  // Vertex normals
  const uint num_of_vnormals = std::get<1>(mesh_info);
  std::vector<Vector3> vnormals;
  vnormals.reserve(num_of_vnormals);
  // Vertex texture coordinates
  const uint num_of_vtextures = std::get<2>(mesh_info);
  std::vector<Point2> vtextures;
  vtextures.reserve(num_of_vtextures);
  // Triangle faces
  const uint num_of_faces = std::get<3>(mesh_info);
  std::vector<UniquePointer<Shape>> meshes;
  meshes.reserve(num_of_faces);
  // Variables for parsing
  bool smoothing = false;
  Float x,
        y,
        z;

  QString value;
  for (QString line; obj_stream.readLineInto(&line);) {
    // Skip a empty line
    if (line.isEmpty())
      continue;

    // Check a keyword
    QTextStream buffer{&line, QIODevice::ReadOnly | QIODevice::Text};
    buffer >> value;
    // Count
    switch (toHash32(value)) {
     case toHash32("s"): {
      buffer >> value;
      smoothing = (value == "1");
      break;
     }
     case toHash32("v"): {
      buffer >> x >> y >> z;
      vertices.emplace_back(x, y, z);
      break;
     }
     case toHash32("vn"): {
      buffer >> x >> y >> z;
      vnormals.emplace_back(Vector3{x, y, z}.normalized());
      break;
     }
     case toHash32("vt"): {
      buffer >> x >> y;
      x = zisc::clamp(x, 0.0, 1.0);
      y = zisc::clamp(y, 0.0, 1.0);
      vtextures.emplace_back(x, 1.0 - y);
      break;
     }
     case toHash32("f"): {
      auto mesh = makeMesh(buffer, vertices, vnormals, vtextures, type, smoothing);
      meshes.emplace_back(std::move(mesh));
      break;
     }
     default:
      break;
    }
  }
  return meshes;
}

} // namespace nanairo
