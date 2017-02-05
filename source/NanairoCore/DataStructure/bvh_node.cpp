/*!
  \file bvh_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "bvh_node.hpp"
// Standard C++ library
#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <tuple>
#include <vector>
#include <utility>
// Zisc
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/Geometry/point.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
BvhNode::BvhNode() noexcept :
    parent_index_{nonObjectIndex()},
    left_child_index_{nonObjectIndex()},
    right_child_index_{nonObjectIndex()},
    cost_{std::numeric_limits<float>::max()}
{
  object_list_.fill(nullptr);
}

/*!
  \details
  No detailed.
  */
BvhNode::BvhNode(const Object* object) noexcept :
    bounding_box_{object->shape().boundingBox()},
    parent_index_{BvhNode::nonObjectIndex()},
    left_child_index_{BvhNode::nonObjectIndex()},
    right_child_index_{BvhNode::nonObjectIndex()},
    cost_{std::numeric_limits<float>::max()}
{
  object_list_.fill(nullptr);
  object_list_[0] = object;
}

/*!
  \details
  For the details of this algorithm,
  please see the web page entitled 
  "Morton encoding/decoding through bit interleaving: Implementations"
  http://www.forceflow.be/2013/10/07/morton-encodingdecoding-through-bit-interleaving-implementations/
  */
uint64 calc63bitMortonCode(const Float x, const Float y, const Float z) noexcept
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
  return (expand_bit(z) << 2) | (expand_bit(y) << 1) | expand_bit(x);
}

/*!
  \details
  No detailed.
  */
Aabb combineBoundingBoxs(std::vector<BvhNode>::const_iterator begin,
                         std::vector<BvhNode>::const_iterator end) noexcept
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

/*!
  \details
  No detailed.
  */
Aabb combineBoundingBoxs(NodeIterator begin, NodeIterator end) noexcept
{
  auto i = begin;
  auto min_point = (*i)->boundingBox().minPoint().data();
  auto max_point = (*i)->boundingBox().maxPoint().data();
  ++i;
  while (i != end) {
    min_point = zisc::minElements(min_point, (*i)->boundingBox().minPoint().data());
    max_point = zisc::maxElements(max_point, (*i)->boundingBox().maxPoint().data());
    ++i;
  }
  return Aabb{Point3{min_point}, Point3{max_point}};
}

/*!
  \details
  No detailed.
  */
MortonCodeIterator findSplitPosition(const uint64 bit,
                                     MortonCodeIterator begin,
                                     MortonCodeIterator end) noexcept
{
  const uint64 significant_bit = (zisc::cast<uint64>(1) << (bit - 1));
  const auto is_less = [significant_bit](const MortonCode& a, const uint64& b)
  {
    return (std::get<1>(a) & significant_bit) < b;
  };
  const uint64 threshold = significant_bit - 1;
  return std::lower_bound(begin, end, threshold, is_less);
}

/*!
  \details
  No detailed.
  */
std::vector<MortonCode> makeMortonCodeList(std::vector<BvhNode>& leaf_node_list) noexcept
{
  using zisc::cast;

  const uint num_of_objects = cast<uint>(leaf_node_list.size());

  // Calc the range of scene
  const auto scene_box = combineBoundingBoxs(leaf_node_list.cbegin(),
                                             leaf_node_list.cend());
  const auto& min_point = scene_box.minPoint();
  const auto range = scene_box.maxPoint() - min_point;
  const Float inverse_x = zisc::invert(range[0]);
  const Float inverse_y = zisc::invert(range[1]);
  const Float inverse_z = zisc::invert(range[2]);

  // Sort by the morton code
  std::vector<MortonCode> morton_code_list;
  morton_code_list.reserve(num_of_objects);
  for (auto& node : leaf_node_list) {
    const auto position = node.boundingBox().centroid() - min_point;
    const auto morton_code = calc63bitMortonCode(position[0] * inverse_x,
                                                 position[1] * inverse_y,
                                                 position[2] * inverse_z);
    morton_code_list.emplace_back(&node, morton_code);
  }
  const auto is_less = [](const MortonCode& a, const MortonCode& b)
  {
    return std::get<1>(a) < std::get<1>(b);
  };
  std::sort(morton_code_list.begin(), morton_code_list.end(), is_less);

  return morton_code_list;
}

} // namespace nanairo
