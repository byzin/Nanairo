/*!
  \file shape.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "plane.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/single_object_setting_node.hpp"

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
zisc::pmr::vector<zisc::UniqueMemoryPointer<Shape>> Shape::makeShape(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto object_settings = castNode<SingleObjectSettingNode>(settings);

  auto data_resource = &system.dataMemoryManager();
  auto work_resource = settings->workResource();
  zisc::pmr::vector<zisc::UniqueMemoryPointer<Shape>> shape_list{work_resource};
  switch (object_settings->shapeType()) {
   case ShapeType::kPlane: {
    auto s = zisc::UniqueMemoryPointer<Plane>::make(data_resource);
    shape_list.emplace_back(std::move(s));
    break;
   }
   case ShapeType::kMesh: {
    shape_list = TriangleMesh::makeMeshes(system, settings);
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
