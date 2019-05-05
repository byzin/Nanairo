/*!
  \file group_object_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "object_model_setting_node.hpp"
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
GroupObjectSettingNode::GroupObjectSettingNode(
    const SettingNodeBase* parent) noexcept :
        SettingNodeBase(parent),
        object_list_{dataResource()},
        object_body_list_{dataResource()}
{
}

/*!
  */
SettingNodeBase* GroupObjectSettingNode::addObject() noexcept
{
  object_body_list_.emplace_back(
      zisc::UniqueMemoryPointer<ObjectModelSettingNode>::make(dataResource(), this));
  auto s = object_body_list_.back().get();
  s->initialize();
  object_list_.emplace_back(s);
  return s;
}

/*!
  */
void GroupObjectSettingNode::initialize() noexcept
{
  object_list_.clear();
  object_body_list_.clear();
}

/*!
  */
SettingNodeType GroupObjectSettingNode::nodeType() noexcept
{
  return SettingNodeType::kGroupObject;
}

/*!
  */
void GroupObjectSettingNode::readData(std::istream* data_stream) noexcept
{
  uint32 size = 0;
  zisc::read(&size, data_stream);
  object_list_.reserve(size);
  object_body_list_.reserve(size);
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
zisc::pmr::vector<SettingNodeBase*>& GroupObjectSettingNode::objectList() noexcept
{
  return object_list_;
}

/*!
  */
const zisc::pmr::vector<SettingNodeBase*>& GroupObjectSettingNode::objectList()
    const noexcept
{
  return object_list_;
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
    const auto& object_list = objectList();
    const uint32 size = zisc::cast<uint32>(object_list.size());
    zisc::write(&size, data_stream);
    for (const auto object : object_list)
      object->writeData(data_stream);
  }
}

} // namespace nanairo
