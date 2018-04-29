/*!
  \file bvh_building_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "bvh_building_node.hpp"
// Standard C++ library
#include <iterator>
#include <vector>
#include <utility>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
Aabb combineBoundingBoxes(
    zisc::pmr::vector<BvhBuildingNode>::const_iterator begin,
    zisc::pmr::vector<BvhBuildingNode>::const_iterator end) noexcept
{
  auto i = begin;
  auto min_point = i->boundingBox().minPoint().data();
  auto max_point = i->boundingBox().maxPoint().data();
  ++i;
  while (i != end) {
    min_point = zisc::minElements(min_point, i->boundingBox().minPoint().data());
    max_point = zisc::maxElements(max_point, i->boundingBox().maxPoint().data());
    ++i;
  }
  return Aabb{Point3{min_point}, Point3{max_point}};
}

} // namespace nanairo
