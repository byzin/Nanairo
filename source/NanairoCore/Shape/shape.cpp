/*!
  \file shape.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "shape.hpp"
// Standard C++ library
#include <memory>
#include <vector>
#include <utility>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "plane.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/single_object_setting_node.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  */
Shape::Shape() noexcept :
    surface_area_{0.0}
{
}

/*!
  */
Shape::~Shape() noexcept
{
}

/*!
  \details
  No detailed.
  */
std::vector<std::unique_ptr<Shape>> Shape::makeShape(
    const SettingNodeBase* settings) noexcept
{
  const auto object_settings = castNode<SingleObjectSettingNode>(settings);

  std::vector<std::unique_ptr<Shape>> shape_list;
  switch (object_settings->shapeType()) {
   case ShapeType::kPlane: {
    shape_list.emplace_back(std::make_unique<Plane>());
    break;
   }
   case ShapeType::kMesh: {
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
  */
void Shape::transform(const Matrix4x4& matrix) noexcept
{
  transformShape(matrix);
  setSurfaceArea(calcSurfaceArea());
}

} // namespace nanairo
