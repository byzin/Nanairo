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
#include "zisc/utility.hpp"
// Nanairo
#include "object_model_setting_node.hpp"
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
SettingNodeBase* GroupObjectSettingNode::addObject() noexcept
{
  object_list_.emplace_back(std::make_unique<ObjectModelSettingNode>());
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
  object_list_.reserve(size);
  for (uint i = 0; i < size; ++i) {
    SettingNodeType type;
    zisc::read(&type, data_stream);
    ZISC_ASSERT((type == SettingNodeType::kObjectModel),
                "The stream header is wrong.");
    auto object = addObject();
    object->readData(data_stream);
  }
}

/*!
  */
std::vector<SettingNodeBase*>& GroupObjectSettingNode::objectList() noexcept
{
  return *zisc::treatAs<std::vector<SettingNodeBase*>*>(&object_list_);
}

/*!
  */
const std::vector<SettingNodeBase*>& GroupObjectSettingNode::objectList()
    const noexcept
{
  return *zisc::treatAs<const std::vector<SettingNodeBase*>*>(&object_list_);
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
  {
    const auto object_list = objectList();
    const uint32 size = zisc::cast<uint32>(object_list.size());
    zisc::write(&size, data_stream);
    for (const auto object : object_list)
      object->writeData(data_stream);
  }
}

} // namespace nanairo
