/*!
  \file geometry.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "geometry.hpp"
// Standard C++ library
#include <cstddef>
#include <vector>
#include <utility>
// Qt
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
#include "NanairoCore/Utility/scene_settings.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
void Geometry::setSurfaceArea(const Float surface_area)
{
  ZISC_ASSERT(0.0 < surface_area, 
              "The surface area of the geometry must be greater than 0.");
  surface_area_ = surface_area;
}

/*!
  \details
  No detailed.
  */
std::vector<UniquePointer<Geometry>> makeGeometry(const SceneSettings& settings,
                                                  const QString& prefix)
{
  using zisc::toHash32;

  std::vector<UniquePointer<Geometry>> geometry_list;

  const auto type = settings.stringValue(prefix + "/" + keyword::geometryType);
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::planeObject):
    geometry_list.emplace_back(new Plane{});
    break;
   case toHash32(keyword::meshObject):
    geometry_list = makeMeshes(settings, prefix);
    break;
   default:
    zisc::raiseError("GeometryError: Unsupported type is specified.");
  }
  return geometry_list;
}

} // namespace nanairo
