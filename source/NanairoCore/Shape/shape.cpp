/*!
  \file shape.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "shape.hpp"
// Standard C++ library
#include <cstddef>
#include <vector>
#include <utility>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "plane.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"
#include "NanairoCore/Utility/scene_value.hpp"

namespace nanairo {

/*!
  */
Shape::~Shape() noexcept
{
}

/*!
  \details
  No detailed.
  */
void Shape::setSurfaceArea(const Float surface_area) noexcept
{
  ZISC_ASSERT(0.0 < surface_area, "The surface area of the shape is minus.");
  surface_area_ = surface_area;
}

/*!
  \details
  No detailed.
  */
std::vector<UniquePointer<Shape>> Shape::makeShape(
    const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  std::vector<UniquePointer<Shape>> shape_list;
  const auto type = SceneValue::toString(settings, keyword::shapeType);
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::planeObject): {
    shape_list.emplace_back(new Plane{});
    break;
   }
   case toHash32(keyword::meshObject): {
    shape_list = TriangleMesh::makeMeshes(settings);
    break;
   }
   default: {
    zisc::raiseError("ShapeError: Unsupported type is specified.");
    break;
   }
  }
  return shape_list;
}

} // namespace nanairo
