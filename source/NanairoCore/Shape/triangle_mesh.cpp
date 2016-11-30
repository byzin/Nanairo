/*!
  \file triangle_mesh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "triangle_mesh.hpp"
// Standard C++ library
#include <string>
#include <vector>
// Qt
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QString>
#include <QTextStream>
#include <QtGlobal>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/string.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "flat_mesh.hpp"
#include "obj_loader.hpp"
#include "shape.hpp"
#include "smoothed_mesh.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo  {

/*!
  \details
  No detailed.
  */
std::vector<UniquePointer<Shape>> TriangleMesh::makeMeshes(
    const QJsonObject& settings) noexcept
{
  using keyword::toHash32;
  using zisc::toHash32;

  std::vector<UniquePointer<Shape>> mesh_list;

  // Initialize object info
  const auto object_file_path = SceneValue::toString(settings,
                                                     keyword::objectFilePath);
  const bool smoothing = SceneValue::toBool(settings, keyword::smoothing);
  const auto mesh_type = smoothing ? MeshType::Smoothed : MeshType::Flat;

  // Open a object file
  const QFileInfo object_file_info{object_file_path};
  if (!object_file_info.exists()) {
    qFatal("TriangleMeshError: File '%s' does not exist",
           qUtf8Printable(object_file_path));
  }
  QFile object_file{object_file_path};
  if (!object_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qFatal("TriangleMeshError: File '%s' open failed.",
           qUtf8Printable(object_file_path));
  }

  const auto suffix = object_file_info.suffix();
  switch (toHash32(suffix)) {
   case toHash32("obj"): {
    QTextStream obj_stream{&object_file};
    mesh_list = ObjLoader::parse(obj_stream, mesh_type);
    break;
   }
   default: {
    qFatal("TriangleMeshError: '%s' isn't supported object format",
           qUtf8Printable(object_file_path));
   }
  }
  return mesh_list;
}

/*!
  \details
  No detailed.
  */
UniquePointer<Shape> TriangleMesh::makeSmoothedMesh(
    const Point3& vertex1,
    const Point3& vertex2,
    const Point3& vertex3,
    const Vector3& normal1,
    const Vector3& normal2,
    const Vector3& normal3) noexcept
{
  TriangleMesh* mesh = nullptr;

  const auto c1 = zisc::dot(normal1, normal2);
  const auto c2 = zisc::dot(normal2, normal3);
  const auto c3 = zisc::dot(normal1, normal3);
  constexpr Float threshold = 0.9999;
  // If the mesh is mostly the same as flat, make a flat mesh instead of the smoothed.
  if ((threshold < c1) && (threshold < c2) && (threshold < c3))
    mesh = new FlatMesh{vertex1, vertex2, vertex3};
  else
    mesh = new SmoothedMesh{vertex1, vertex2, vertex3, normal1, normal2, normal3};
  return UniquePointer<Shape>{mesh};
}

} // namespace nanairo
