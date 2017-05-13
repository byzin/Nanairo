/*!
  \file obj_loader.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "obj_loader.hpp"
// Standard C++ library
#include <array>
#include <vector>
#include <utility>
// Qt
#include <QString>
#include <QTextStream>
#include <QtGlobal>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/face.hpp"
#include "NanairoGui/keyword.hpp"

namespace nanairo  {

/*!
  */
void ObjLoader::parse(
    QTextStream& obj_stream,
    std::vector<Face>* face_list,
    std::vector<std::array<double, 3>>* vertex_list,
    std::vector<std::array<double, 3>>* vnormal_list,
    std::vector<std::array<double, 2>>* vuv_list) noexcept
{
  ZISC_ASSERT(obj_stream.device() != 0, "The obj_stream isn't initialized.");
  ZISC_ASSERT(face_list != nullptr, "The face_list is null.");
  ZISC_ASSERT(vertex_list != nullptr, "The vertex_list is null.");
  ZISC_ASSERT(vnormal_list != nullptr, "The vnormal_list is null.");
  ZISC_ASSERT(vuv_list != nullptr, "The vuv_list is null.");
  face_list->clear();
  vertex_list->clear();
  vnormal_list->clear();
  vuv_list->clear();

  // Count elements
  obj_stream.seek(0);
  const auto counts = countNumOfMeshes(obj_stream);
  if (counts[0] == 0)
    qFatal("The num of faces is 0.");
  face_list->reserve(counts[0]);
  vertex_list->reserve(counts[1]);
  vnormal_list->reserve(counts[2]);
  vuv_list->reserve(counts[3]);

  // Make meshes
  obj_stream.seek(0);
  loadMesh(obj_stream, face_list, vertex_list, vnormal_list, vuv_list);
}

/*!
  */
std::array<uint, 4> ObjLoader::countNumOfMeshes(QTextStream& obj_stream) noexcept
{
  using keyword::toHash32;
  using zisc::toHash32;

  uint num_of_faces = 0;
  uint num_of_vertices = 0;
  uint num_of_vnormals = 0; // Vertex normals
  uint num_of_vuv = 0; // Vertex texture uv
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
      ++num_of_vuv;
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
  return std::array<uint, 4>{{num_of_faces,
                              num_of_vertices,
                              num_of_vnormals,
                              num_of_vuv}};
}

/*!
  */
void ObjLoader::loadFace(QTextStream& face_line,
                         const bool has_vnormal,
                         const bool has_vuv,
                         const bool smoothing,
                         Face* face) noexcept
{
  char delimiter;
  uint vertex_indices[3];
  uint vnormal_indices[3];
  uint vuv_indices[3];
  for (uint i = 0; i < 3; ++i) {
    if (has_vnormal && has_vuv) {
      face_line >> vertex_indices[i] >> delimiter
                >> vuv_indices[i] >> delimiter
                >> vnormal_indices[i];
    }
    else if (has_vnormal && !has_vuv) {
      face_line >> vertex_indices[i] >> delimiter >> delimiter
                >> vnormal_indices[i];
    }
    else if (!has_vnormal && has_vuv) {
      face_line >> vertex_indices[i] >> delimiter >> vuv_indices[i];
    }
    else {
      face_line >> vertex_indices[i];
    }
  }

  face->setVertexIndices(zisc::cast<uint32>(vertex_indices[0] - 1),
                         zisc::cast<uint32>(vertex_indices[1] - 1),
                         zisc::cast<uint32>(vertex_indices[2] - 1));

  face->setSmoothing(smoothing);
  if (has_vnormal) {
    face->setVnormalIndices(zisc::cast<uint32>(vnormal_indices[0] - 1),
                            zisc::cast<uint32>(vnormal_indices[1] - 1),
                            zisc::cast<uint32>(vnormal_indices[2] - 1));
  }
  if (has_vuv) {
    face->setVuvIndices(zisc::cast<uint32>(vuv_indices[0] - 1),
                        zisc::cast<uint32>(vuv_indices[1] - 1),
                        zisc::cast<uint32>(vuv_indices[2] - 1));
  }
}

/*!
  */
void ObjLoader::loadMesh(QTextStream& obj_stream,
                         std::vector<Face>* face_list,
                         std::vector<std::array<double, 3>>* vertex_list,
                         std::vector<std::array<double, 3>>* vnormal_list,
                         std::vector<std::array<double, 2>>* vuv_list) noexcept
{
  using keyword::toHash32;
  using zisc::toHash32;

  bool smoothing = false;
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
      double v0 = 0.0,
             v1 = 0.0,
             v2 = 0.0;
      buffer >> v0 >> v1 >> v2;
      vertex_list->emplace_back(std::array<double, 3>{{v0, v1, v2}});
      break;
     }
     case toHash32("vn"): {
      double n0 = 0.0,
             n1 = 0.0,
             n2 = 0.0;
      buffer >> n0 >> n1 >> n2;
      vnormal_list->emplace_back(std::array<double, 3>{{n0, n1, n2}});
      break;
     }
     case toHash32("vt"): {
      double u = 0.0,
             v = 0.0;
      buffer >> u >> v;
      u = zisc::clamp(u, 0.0, 1.0);
      v = 1.0 - zisc::clamp(v, 0.0, 1.0);
      vuv_list->emplace_back(std::array<double, 2>{{u, v}});
      break;
     }
     case toHash32("f"): {
      const bool has_vnormal = (0 < vnormal_list->size());
      const bool has_vuv = (0 < vuv_list->size());
      face_list->emplace_back();
      auto& face = face_list->back();
      loadFace(buffer, has_vnormal, has_vuv, smoothing, &face);
      break;
     }
     default:
      break;
    }
  }
}

} // namespace nanairo
