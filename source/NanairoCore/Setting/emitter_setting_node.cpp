/*!
  \file emitter_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "emitter_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"

namespace nanairo {

/*!
  */
void NonDirectionalEmitterParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&radiant_exitance_, data_stream);
  zisc::read(&color_index_, data_stream);
}

/*!
  */
void NonDirectionalEmitterParameters::writeData(std::ostream* data_stream)
    const noexcept
{
  zisc::write(&radiant_exitance_, data_stream);
  zisc::write(&color_index_, data_stream);
}

/*!
  */
EmitterSettingNode::EmitterSettingNode(const SettingNodeBase* parent) noexcept :
    SettingNodeBase(parent),
    name_{dataResource()}
{
}

/*!
  */
EmitterType EmitterSettingNode::emitterType() const noexcept
{
  return emitter_type_;
}

/*!
  */
void EmitterSettingNode::initialize() noexcept
{
  setName("EmitterModel");
  setEmitterType(EmitterType::kNonDirectional);
}

/*!
  */
std::string_view EmitterSettingNode::name() const noexcept
{
  return std::string_view{name_};
}

/*!
  */
SettingNodeType EmitterSettingNode::nodeType() noexcept
{
  return SettingNodeType::kEmitter;
}

/*!
  */
NonDirectionalEmitterParameters&
EmitterSettingNode::nonDirectionalEmitterParameters() noexcept
{
  ZISC_ASSERT(emitterType() == EmitterType::kNonDirectional,
              "Invalid emitter type is specified.");
  auto parameters = zisc::cast<NonDirectionalEmitterParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const NonDirectionalEmitterParameters&
EmitterSettingNode::nonDirectionalEmitterParameters() const noexcept
{
  ZISC_ASSERT(emitterType() == EmitterType::kNonDirectional,
              "Invalid emitter type is specified.");
  auto parameters = zisc::cast<const NonDirectionalEmitterParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
void EmitterSettingNode::readData(std::istream* data_stream) noexcept
{
  {
    name_= readString(data_stream);
  }
  {
    zisc::read(&emitter_type_, data_stream);
    setEmitterType(emitter_type_);
  }
  if (parameters_)
    parameters_->readData(data_stream);
}

/*!
  */
void EmitterSettingNode::setEmitterType(const EmitterType type) noexcept
{
  emitter_type_ = type;
  // Initialize parameters
  parameters_.reset();
  switch (emitter_type_) {
   case EmitterType::kNonDirectional: {
    parameters_ =
        zisc::UniqueMemoryPointer<NonDirectionalEmitterParameters>::make(dataResource());
    break;
   }
   default:
    break;
  }
}

/*!
  */
void EmitterSettingNode::setName(const std::string_view& name) noexcept
{
  name_ = name;
}

/*!
  */
SettingNodeType EmitterSettingNode::type() const noexcept
{
  return SettingNodeType::kEmitter;
}

/*!
  */
void EmitterSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);
  writeString(name(), data_stream);
  zisc::write(&emitter_type_, data_stream);
  if (parameters_)
    parameters_->writeData(data_stream);
}

} // namespace nanairo
