/*!
  \file morton_code.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "morton_code.hpp"
// Standard C++ library
#include <algorithm>
#include <iterator>
#include <vector>
#include <utility>
// Zisc
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"

namespace nanairo {

/*!
  \details
  For the details of this algorithm,
  please see the web page entitled 
  "Morton encoding/decoding through bit interleaving: Implementations"
  http://www.forceflow.be/2013/10/07/morton-encodingdecoding-through-bit-interleaving-implementations/
  */
uint64 MortonCode::calc63bitCode(const Point3& position) noexcept
{
  using zisc::cast;
  const auto expand_bit = [](Float value)
  {
    constexpr Float expander = cast<Float>((cast<uint64>(1) << 21) - 1);
    value = zisc::clamp(value, 0.0, 1.0);
    uint64 v = cast<uint64>(value * expander);
    v = (v | (v << 32)) & 0x1f00000000ffff;
    v = (v | (v << 16)) & 0x1f0000ff0000ff;
    v = (v | (v <<  8)) & 0x100f00f00f00f00f;
    v = (v | (v <<  4)) & 0x10c30c30c30c30c3;
    v = (v | (v <<  2)) & 0x1249249249249249;
    return v;
  };
  return (expand_bit(position[2]) << 2) |
         (expand_bit(position[1]) << 1) |
          expand_bit(position[0]);
}

/*!
  \details
  No detailed.
  */
auto MortonCode::findSplit(const uint bit,
                           Iterator begin,
                           Iterator end) noexcept -> Iterator
{
  const auto is_less = [](const MortonCode& lhs, const CodeType& rhs)
  {
    return (lhs.code() & rhs) < rhs;
  };
  const CodeType key_bit = (zisc::cast<CodeType>(0b01) << (bit - 1));
  return std::lower_bound(begin, end, key_bit, is_less);
}

/*!
  \details
  No detailed.
  */
std::vector<MortonCode> MortonCode::makeList(
    const std::vector<BvhBuildingNode>& node_list) noexcept
{
  const uint num_of_objects = zisc::cast<uint>(node_list.size());

  // Calc the range of scene
  const auto scene_box = combineBoundingBoxes(node_list.cbegin(), node_list.cend());
  const auto& min_point = scene_box.minPoint();
  const auto range = scene_box.maxPoint() - min_point;
  const Float inverse_x = zisc::invert(range[0]);
  const Float inverse_y = zisc::invert(range[1]);
  const Float inverse_z = zisc::invert(range[2]);

  // Sort by the morton code
  std::vector<MortonCode> morton_code_list;
  morton_code_list.reserve(num_of_objects);
  for (auto& node : node_list) {
    const auto position = node.boundingBox().centroid() - min_point;
    const Point3 normalized_position{position[0] * inverse_x,
                                     position[1] * inverse_y,
                                     position[2] * inverse_z};
    static_assert(sizeof(CodeType) == 8, "The size of code isn't 64bit.");
    const auto morton_code = MortonCode::calc63bitCode(normalized_position);
    morton_code_list.emplace_back(&node, morton_code);
  }
  std::sort(morton_code_list.begin(), morton_code_list.end());

  return morton_code_list;
}

} // namespace nanairo
