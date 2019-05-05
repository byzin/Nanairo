/*!
  \file object_model_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "object_model_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "camera_setting_node.hpp"
#include "group_object_setting_node.hpp"
#include "single_object_setting_node.hpp"
#include "setting_node_base.hpp"
#include "transformation_setting_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"

namespace nanairo {

/*!
  */
ObjectModelSettingNode::ObjectModelSettingNode(const SettingNodeBase* parent) noexcept :
    SettingNodeBase(parent),
    name_{dataResource()},
    transformation_list_{dataResource()},
    transformation_body_list_{dataResource()}
{
}

/*!
  */
SettingNodeBase* ObjectModelSettingNode::addTransformation(
    const TransformationType type,
    const double x,
    const double y,
    const double z) noexcept
{
  transformation_body_list_.emplace_back(
      zisc::UniqueMemoryPointer<TransformationSettingNode>::make(dataResource(),
                                                                 this));
  auto t = zisc::cast<TransformationSettingNode*>(transformation_body_list_.back().get());
  t->setTransformationType(type);
  t->setValue(x, y, z);
  transformation_list_.emplace_back(t);
  return t;
}

/*!
  */
void ObjectModelSettingNode::initialize() noexcept
{
  setName("Object");
  setVisibility(true);
  transformation_list_.clear();
  transformation_body_list_.clear();
  setObject(ObjectType::kSingle);
}

/*!
  */
std::string_view ObjectModelSettingNode::name() const noexcept
{
  return std::string_view{name_};
}

/*!
  */
SettingNodeType ObjectModelSettingNode::nodeType() noexcept
{
  return SettingNodeType::kObjectModel;
}

/*!
  */
SettingNodeBase* ObjectModelSettingNode::objectSettingNode() noexcept
{
  return object_setting_node_.get();
}

/*!
  */
const SettingNodeBase* ObjectModelSettingNode::objectSettingNode() const noexcept
{
  return object_setting_node_.get();
}

/*!
  */
void ObjectModelSettingNode::readData(std::istream* data_stream) noexcept
{
  // Properties
  {
    name_ = readString(data_stream);
  }
  zisc::read(&visibility_, data_stream);

  // Transformation
  {
    uint32 num_of_transformations;
    zisc::read(&num_of_transformations, data_stream);
    transformation_list_.reserve(num_of_transformations);
    transformation_body_list_.reserve(num_of_transformations);
    for (uint i = 0; i < num_of_transformations; ++i) {
      SettingNodeType type;
      zisc::read(&type, data_stream);
      ZISC_ASSERT(type == SettingNodeType::kTransformation, "The stream header is wrong.");
      auto t = addTransformation(TransformationType::kTranslation, 0.0, 0.0, 0.0);
      t->readData(data_stream);
    }
  }

  // Object
  {
    SettingNodeType type;
    zisc::read(&type, data_stream);
    ZISC_ASSERT((type == SettingNodeType::kCameraObject) ||
                (type == SettingNodeType::kGroupObject) ||
                (type == SettingNodeType::kSingleObject),
                "The invalid object type is specified.");
    const ObjectType object_type =
        (type == SettingNodeType::kCameraObject) ? ObjectType::kCamera :
        (type == SettingNodeType::kGroupObject)  ? ObjectType::kGroup
                                                 : ObjectType::kSingle;
    auto object = setObject(object_type);
    object->readData(data_stream);
  }
}

/*!
  */
void ObjectModelSettingNode::setName(const std::string_view& name) noexcept
{
  name_ = name;
}

/*!
  */
SettingNodeBase* ObjectModelSettingNode::setObject(const ObjectType type) noexcept
{
  switch (type) {
   case ObjectType::kCamera: {
    object_setting_node_ =
        zisc::UniqueMemoryPointer<CameraSettingNode>::make(dataResource(), this);
    break;
   }
   case ObjectType::kGroup: {
    object_setting_node_ =
        zisc::UniqueMemoryPointer<GroupObjectSettingNode>::make(dataResource(), this);
    break;
   }
   case ObjectType::kSingle: {
    object_setting_node_ =
        zisc::UniqueMemoryPointer<SingleObjectSettingNode>::make(dataResource(), this);
    break;
   }
   default: {
    zisc::raiseError("Invalid object type is specified.");
    break;
   }
  }
  object_setting_node_->initialize();
  return objectSettingNode();
}

/*!
  */
void ObjectModelSettingNode::setVisibility(const bool visibility) noexcept
{
  visibility_ = visibility ? kTrue : kFalse;
}

/*!
  */
zisc::pmr::vector<SettingNodeBase*>& ObjectModelSettingNode::transformationList() noexcept
{
  return transformation_list_;
}

/*!
  */
const zisc::pmr::vector<SettingNodeBase*>& ObjectModelSettingNode::transformationList()
    const noexcept
{
  return transformation_list_;
}

/*!
  */
SettingNodeType ObjectModelSettingNode::type() const noexcept
{
  return SettingNodeType::kObjectModel;
}

/*!
  */
bool ObjectModelSettingNode::visibility() const noexcept
{
  return (visibility_ == kTrue);
}

/*!
  */
void ObjectModelSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);

  // Properties
  writeString(name(), data_stream);
  zisc::write(&visibility_, data_stream);

  // Transformations
  {
    const auto& transformation_list = transformationList();
    const uint32 num_of_transformations = zisc::cast<uint32>(transformation_list.size());
    zisc::write(&num_of_transformations, data_stream);
    for (const auto transformation : transformation_list)
      transformation->writeData(data_stream);
  }

  // Object
  {
    const auto object_settings = objectSettingNode();
    object_settings->writeData(data_stream);
  }
}

} // namespace nanairo
