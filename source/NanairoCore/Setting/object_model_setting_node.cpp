/*!
  \file object_model_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "object_model_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <ostream>
#include <string>
#include <vector>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
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
SettingNodeBase* ObjectModelSettingNode::addTransformation(
    const TransformationType type,
    const double x,
    const double y,
    const double z) noexcept
{
  transformation_list_.emplace_back(new TransformationSettingNode{});
  auto t = zisc::cast<TransformationSettingNode*>(transformation_list_.back().get());
  t->setTransformationType(type);
  t->setValue(x, y, z);
  return t;
}

/*!
  */
void ObjectModelSettingNode::initialize() noexcept
{
  setName("Object");
  setVisibility(true);
  transformation_list_.clear();
  setObject(ObjectType::kSingle);
}

/*!
  */
void ObjectModelSettingNode::readData(std::istream* data_stream) noexcept
{
  setName(readString(data_stream));
  zisc::read(&visibility_, data_stream);
  // Transformation
  {
    uint32 num_of_transformation;
    zisc::read(&num_of_transformation, data_stream);
    transformation_list_.reserve(num_of_transformation);
    for (uint i = 0; i < num_of_transformation; ++i) {
      auto type = readType(data_stream);
      ZISC_ASSERT(type == SettingNodeType::kTransformation, "Invalid header.");
      if (type == SettingNodeType::kTransformation) {
        auto t = addTransformation(TransformationType::kTranslation, 0.0, 0.0, 0.0);
        t->readData(data_stream);
      }
    }
  }
  // Object
  //! \todo Object reading
}

/*!
  */
const std::string& ObjectModelSettingNode::name() const noexcept
{
  return name_;
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
void ObjectModelSettingNode::setName(const std::string& name) noexcept
{
  name_ = name;
}

/*!
  */
void ObjectModelSettingNode::setName(std::string&& name) noexcept
{
  name_ = std::move(name);
}

/*!
  */
SettingNodeBase* ObjectModelSettingNode::setObject(const ObjectType type) noexcept
{
  switch (type) {
   case ObjectType::kCamera: {
    object_setting_node_.reset(new CameraSettingNode{});
    break;
   }
   case ObjectType::kGroup: {
    object_setting_node_.reset(new GroupObjectSettingNode{});
    break;
   }
   case ObjectType::kSingle: {
    object_setting_node_.reset(new SingleObjectSettingNode{});
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
std::vector<SettingNodeBase*> ObjectModelSettingNode::transformationList() noexcept
{
  std::vector<SettingNodeBase*> transformation_list;
  transformation_list.resize(transformation_list_.size());
  for (uint i = 0; i < transformation_list_.size(); ++i)
    transformation_list[i] = transformation_list_[i].get();
  return transformation_list;
}

/*!
  */
std::vector<const SettingNodeBase*> ObjectModelSettingNode::transformationList()
    const noexcept
{
  std::vector<const SettingNodeBase*> transformation_list;
  transformation_list.resize(transformation_list_.size());
  for (uint i = 0; i < transformation_list_.size(); ++i)
    transformation_list[i] = transformation_list_[i].get();
  return transformation_list;
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
void ObjectModelSettingNode::writeData(std::ostream* /* data_stream */) const noexcept
{
}

} // namespace nanairo
