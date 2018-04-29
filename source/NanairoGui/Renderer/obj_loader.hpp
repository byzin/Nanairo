/*!
  \file obj_loader.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_OBJ_LOADER_HPP
#define NANAIRO_OBJ_LOADER_HPP

// Standard C++ library
#include <array>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/face.hpp"

// Forward declaration
class QTextStream;

namespace nanairo {

//! \addtogroup Gui
//! \{

/*!
  */
class ObjLoader
{
 public:
  //! Parse a obj text and make a obj model
  static void parse(QTextStream& obj_stream,
                    zisc::pmr::vector<Face>* face_list,
                    zisc::pmr::vector<std::array<double, 3>>* vertex_list,
                    zisc::pmr::vector<std::array<double, 3>>* vnormal_list,
                    zisc::pmr::vector<std::array<double, 2>>* vuv_list) noexcept;

 private:
  //! Prevent from making a obj loader instance
  ObjLoader() noexcept;
  ObjLoader(const ObjLoader&) = delete;
  ObjLoader& operator=(const ObjLoader&) = delete;


  //! Count the number of meshes
  static std::array<uint, 4> countNumOfMeshes(QTextStream& obj_stream) noexcept;

  //! Load the face data
  static void loadFace(QTextStream& face_line,
                       const bool has_vnormal,
                       const bool has_vuv,
                       const bool smoothing,
                       Face* face) noexcept;

  //! Load the mesh data
  static void loadMesh(QTextStream& obj_stream,
                       zisc::pmr::vector<Face>* face_list,
                       zisc::pmr::vector<std::array<double, 3>>* vertex_list,
                       zisc::pmr::vector<std::array<double, 3>>* vnormal_list,
                       zisc::pmr::vector<std::array<double, 2>>* vuv_list) noexcept;
};

//! \} Gui

} // namespace nanairo

#endif // NANAIRO_OBJ_LOADER_HPP
