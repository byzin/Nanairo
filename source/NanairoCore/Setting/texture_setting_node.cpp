/*!
  \file texture_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "texture_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "spectra_setting_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"

namespace nanairo {

/*!
  */
void ValueTextureParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&value_, data_stream);
}

/*!
  */
void ValueTextureParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&value_, data_stream);
}

/*!
  */
UnicolorTextureParameters::UnicolorTextureParameters(
    const SettingNodeBase* parent) noexcept :
        color_{zisc::UniqueMemoryPointer<SpectraSettingNode>::make(
            parent->dataResource(),
            parent)}
{
  color_->initialize();
}

/*!
  */
void UnicolorTextureParameters::readData(std::istream* data_stream) noexcept
{
  ZISC_ASSERT(color_.get() != nullptr, "The color is null.");

  SettingNodeType type;
  zisc::read(&type, data_stream);
  ZISC_ASSERT(type == SettingNodeType::kSpectra, "The stream header is wrong.");
  color_->readData(data_stream);
}

/*!
  */
void UnicolorTextureParameters::writeData(std::ostream* data_stream) const noexcept
{
  ZISC_ASSERT(color_.get() != nullptr, "The color is null.");
  color_->writeData(data_stream);
}

/*!
  */
CheckerboardTextureParameters::CheckerboardTextureParameters(
    const SettingNodeBase* parent) noexcept :
        color_{{zisc::UniqueMemoryPointer<SpectraSettingNode>::make(
            parent->dataResource(),
            parent),
                zisc::UniqueMemoryPointer<SpectraSettingNode>::make(
            parent->dataResource(),
            parent)}}
{
  color_[0]->initialize();
  color_[1]->initialize();
}

/*!
  */
void CheckerboardTextureParameters::readData(std::istream* data_stream) noexcept
{
  ZISC_ASSERT(color_[0].get() != nullptr, "The color1 is null.");
  ZISC_ASSERT(color_[1].get() != nullptr, "The color2 is null.");

  SettingNodeType type;
  zisc::read(&type, data_stream);
  ZISC_ASSERT(type == SettingNodeType::kSpectra, "The stream header is wrong.");
  color_[0]->readData(data_stream);

  zisc::read(&type, data_stream);
  ZISC_ASSERT(type == SettingNodeType::kSpectra, "The stream header is wrong.");
  color_[1]->readData(data_stream);

  zisc::read(&resolution_[0], data_stream, sizeof(resolution_[0]) * 2);
}

/*!
  */
void CheckerboardTextureParameters::writeData(std::ostream* data_stream)
    const noexcept
{
  ZISC_ASSERT(color_[0].get() != nullptr, "The color1 is null.");
  ZISC_ASSERT(color_[1].get() != nullptr, "The color2 is null.");
  color_[0]->writeData(data_stream);
  color_[1]->writeData(data_stream);
  zisc::write(&resolution_[0], data_stream, sizeof(resolution_[0]) * 2);
}

/*!
  */
ImageTextureParameters::ImageTextureParameters(
    zisc::pmr::memory_resource* data_resource) noexcept :
        image_{1, 1, data_resource}
{
}

/*!
  */
void ImageTextureParameters::readData(std::istream* data_stream) noexcept
{
  {
    std::array<uint32, 2> resolution;
    zisc::read(&resolution[0], data_stream, sizeof(resolution[0]) * 2);
    image_.setResolution(resolution[0], resolution[1]);
  }
  {
    auto& buffer = image_.data();
    zisc::read(&buffer[0], data_stream, sizeof(buffer[0]) * buffer.size());
  }
}

/*!
  */
void ImageTextureParameters::writeData(std::ostream* data_stream) const noexcept
{
  {
    const std::array<uint32, 2> resolution{{
        zisc::cast<uint32>(image_.widthResolution()),
        zisc::cast<uint32>(image_.heightResolution())}};
    zisc::write(&resolution[0], data_stream, sizeof(resolution[0]) * 2);
  }
  {
    const auto& buffer = image_.data();
    zisc::write(&buffer[0], data_stream, sizeof(buffer[0]) * buffer.size());
  }
}

/*!
  */
TextureSettingNode::TextureSettingNode(const SettingNodeBase* parent) noexcept :
    SettingNodeBase(parent),
    name_{dataResource()}
{
}

/*!
  */
CheckerboardTextureParameters&
TextureSettingNode::checkerboardTextureParameters() noexcept
{
  ZISC_ASSERT(textureType() == TextureType::kCheckerboard,
              "Invalid texture type is specified.");
  auto parameters = zisc::cast<CheckerboardTextureParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const CheckerboardTextureParameters&
TextureSettingNode::checkerboardTextureParameters() const noexcept
{
  ZISC_ASSERT(textureType() == TextureType::kCheckerboard,
              "Invalid texture type is specified.");
  const auto parameters =
      zisc::cast<const CheckerboardTextureParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
ImageTextureParameters& TextureSettingNode::imageTextureParameters() noexcept
{
  ZISC_ASSERT(textureType() == TextureType::kImage,
              "Invalid texture type is specified.");
  auto parameters = zisc::cast<ImageTextureParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const ImageTextureParameters& TextureSettingNode::imageTextureParameters()
    const noexcept
{
  ZISC_ASSERT(textureType() == TextureType::kImage,
              "Invalid texture type is specified.");
  const auto parameters =
      zisc::cast<const ImageTextureParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
void TextureSettingNode::initialize() noexcept
{
  setName("TextureModel");
  setTextureType(TextureType::kValue);
}

/*!
  */
std::string_view TextureSettingNode::name() const noexcept
{
  return std::string_view{name_};
}

/*!
  */
SettingNodeType TextureSettingNode::nodeType() noexcept
{
  return SettingNodeType::kTexture;
}

/*!
  */
void TextureSettingNode::readData(std::istream* data_stream) noexcept
{
  {
    name_ = readString(data_stream);
  }
  {
    zisc::read(&texture_type_, data_stream);
    setTextureType(texture_type_);
  }
  if (parameters_)
    parameters_->readData(data_stream);
}

/*!
  */
void TextureSettingNode::setName(const std::string_view& name) noexcept
{
  name_ = name;
}

/*!
  */
void TextureSettingNode::setTextureType(const TextureType type) noexcept
{
  texture_type_ = type;
  // Initialize parameters
  parameters_.reset();
  switch (texture_type_) {
   case TextureType::kValue: {
    parameters_ =
        zisc::UniqueMemoryPointer<ValueTextureParameters>::make(dataResource());
    break;
   }
   case TextureType::kUnicolor: {
    parameters_ =
        zisc::UniqueMemoryPointer<UnicolorTextureParameters>::make(dataResource(),
                                                                   this);
    break;
   }
   case TextureType::kCheckerboard: {
    parameters_ =
        zisc::UniqueMemoryPointer<CheckerboardTextureParameters>::make(dataResource(),
                                                                       this);
    break;
   }
   case TextureType::kImage: {
    parameters_ =
        zisc::UniqueMemoryPointer<ImageTextureParameters>::make(dataResource(),
                                                                dataResource());
    break;
   }
   default:
    break;
  }
}

/*!
  */
TextureType TextureSettingNode::textureType() const noexcept
{
  return texture_type_;
}

/*!
  */
SettingNodeType TextureSettingNode::type() const noexcept
{
  return SettingNodeType::kTexture;
}

/*!
  */
UnicolorTextureParameters&
TextureSettingNode::unicolorTextureParameters() noexcept
{
  ZISC_ASSERT(textureType() == TextureType::kUnicolor,
              "Invalid texture type is specified.");
  auto parameters = zisc::cast<UnicolorTextureParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const UnicolorTextureParameters&
TextureSettingNode::unicolorTextureParameters() const noexcept
{
  ZISC_ASSERT(textureType() == TextureType::kUnicolor,
              "Invalid texture type is specified.");
  auto parameters =
      zisc::cast<const UnicolorTextureParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
ValueTextureParameters& TextureSettingNode::valueTextureParameters() noexcept
{
  ZISC_ASSERT(textureType() == TextureType::kValue,
              "Invalid texture type is specified.");
  auto parameters = zisc::cast<ValueTextureParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const ValueTextureParameters& TextureSettingNode::valueTextureParameters()
    const noexcept
{
  ZISC_ASSERT(textureType() == TextureType::kValue,
              "Invalid texture type is specified.");
  auto parameters = zisc::cast<const ValueTextureParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
void TextureSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);
  writeString(name(), data_stream);
  zisc::write(&texture_type_, data_stream);
  if (parameters_)
    parameters_->writeData(data_stream);
}

} // namespace nanairo
