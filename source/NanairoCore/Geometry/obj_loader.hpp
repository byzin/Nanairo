/*!
  \file obj_loader.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_OBJ_LOADER_HPP
#define NANAIRO_OBJ_LOADER_HPP

// Standard C++ library
#include <tuple>
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/triangle_mesh.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QTextStream;

namespace nanairo {

// Forward declaration
class Geometry;

//! \addtogroup Core
//! \{

/*!
  */
class ObjLoader
{
 public:
  //! Parse a obj text and make a obj model
  static std::vector<UniquePointer<Geometry>> parse(
      QTextStream& obj_stream,
      const MeshType type) noexcept;

 private:
  //! Prevent from making a obj loader instance
  ObjLoader() noexcept;
  ObjLoader(const ObjLoader&) = delete;
  ObjLoader& operator=(const ObjLoader&) = delete;


  //! Count the number of meshes
  static std::tuple<uint, uint, uint, uint> countNumOfMeshes(
      QTextStream& obj_stream) noexcept;

  //! Make a triangle mesh
  static UniquePointer<Geometry> makeMesh(
      QTextStream& face_line,
      const std::vector<Point3>& vertices,
      const std::vector<Vector3>& vnormals,
      const std::vector<Point2>& vtextures,
      const MeshType mesh_type,
      const bool smoothing) noexcept;

  //! Make triangle meshes
  static std::vector<UniquePointer<Geometry>> makeMeshes(
      QTextStream& obj_stream,
      const std::tuple<uint, uint, uint, uint>& mesh_info,
      const MeshType type) noexcept;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_OBJ_LOADER_HPP
