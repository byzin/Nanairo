/*!
  \file group_object_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "group_object_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <vector>
// Zisc
#include "zisc/binary_data.hpp"
// Nanairo
#include "object_model_setting_node.hpp"
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
SettingNodeBase* GroupObjectSettingNode::addObject() noexcept
{
  object_list_.emplace_back(new ObjectModelSettingNode{});
  auto& s = object_list_.back();
  s->initialize();
  return s.get();
}

/*!
  */
void GroupObjectSettingNode::initialize() noexcept
{
  object_list_.clear();
}

/*!
  */
void GroupObjectSettingNode::readData(std::istream* data_stream) noexcept
{
  uint32 size = 0;
  zisc::read(&size, data_stream);
  for (uint i = 0; i < size; ++i) {
  }
}

/*!
  */
std::vector<SettingNodeBase*> GroupObjectSettingNode::objectList() noexcept
{
  std::vector<SettingNodeBase*> object_list;
  object_list.resize(object_list_.size());
  for (uint i = 0; i < object_list_.size(); ++i)
    object_list[i] = object_list_[i].get();
  return object_list;
}

/*!
  */
std::vector<const SettingNodeBase*> GroupObjectSettingNode::objectList()
    const noexcept
{
  std::vector<const SettingNodeBase*> object_list;
  object_list.resize(object_list_.size());
  for (uint i = 0; i < object_list_.size(); ++i)
    object_list[i] = object_list_[i].get();
  return object_list;
}

/*!
  */
SettingNodeType GroupObjectSettingNode::type() const noexcept
{
  return SettingNodeType::kGroupObject;
}

/*!
  */
void GroupObjectSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);
  // Write properties
  const uint32 size = zisc::cast<uint32>(object_list_.size());
  zisc::write(&size, data_stream);
  for (uint i = 0; i < size; ++i)
    object_list_[i]->writeData(data_stream);
}

} // namespace nanairo
