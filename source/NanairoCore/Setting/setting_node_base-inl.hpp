/*!
  \file setting_node_base-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SETTING_NODE_BASE_INL_HPP
#define NANAIRO_SETTING_NODE_BASE_INL_HPP

#include "setting_node_base.hpp"
// Standard C++ library
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"

namespace nanairo {

/*!
  */
inline
bool SettingNodeBase::isRoot() const noexcept
{
  const bool is_root = parent() == nullptr;
  return is_root;
}

/*!
  */
inline
const SettingNodeBase* SettingNodeBase::parent() const noexcept
{
  return parent_;
}

/*!
  */
inline
const SettingNodeBase* SettingNodeBase::root() const noexcept
{
  const SettingNodeBase* r = isRoot() ? this : parent()->root();
  return r;
}

/*!
  */
template <typename NodeType> inline
NodeType* castNode(SettingNodeBase* node) noexcept
{
  static_assert(std::is_base_of<SettingNodeBase, NodeType>::value,
                "The NodeType isn't derived from SettingNodeBase.");
  ZISC_ASSERT(node != nullptr, "The setting node is null.");
  ZISC_ASSERT(node->type() == NodeType::nodeType(),
              "Wrong setting node is specified.");
  return zisc::cast<NodeType*>(node);
}

/*!
  */
template <typename NodeType> inline
const NodeType* castNode(const SettingNodeBase* node) noexcept
{
  static_assert(std::is_base_of<SettingNodeBase, NodeType>::value,
                "The NodeType isn't derived from SettingNodeBase.");
  ZISC_ASSERT(node != nullptr, "The setting node is null.");
  ZISC_ASSERT(node->type() == NodeType::nodeType(),
              "Wrong setting node is specified.");
  return zisc::cast<const NodeType*>(node);
}

} // namespace nanairo

#endif // NANAIRO_SETTING_NODE_BASE_INL_HPP
