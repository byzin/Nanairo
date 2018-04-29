/*!
  \file texture_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TEXTURE_SETTING_NODE_HPP
#define NANAIRO_TEXTURE_SETTING_NODE_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! ValueTexture parameters
struct ValueTextureParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  double value_ = 0.8;
};

//! UnicolorTexture parameters
struct UnicolorTextureParameters : public NodeParameterBase
{
  //! Initialize parameters
  UnicolorTextureParameters(const SettingNodeBase* parent) noexcept;

  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  zisc::UniqueMemoryPointer<SettingNodeBase> color_;
};

//! CheckerboardTexture parameters
struct CheckerboardTextureParameters : public NodeParameterBase
{
  //! Initialize parameters
  CheckerboardTextureParameters(const SettingNodeBase* parent) noexcept;

  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  std::array<zisc::UniqueMemoryPointer<SettingNodeBase>, 2> color_;
  std::array<uint32, 2> resolution_{{10, 10}};
};

//! ImageTexture parameters
struct ImageTextureParameters : public NodeParameterBase
{
  //! Initialize parameters
  ImageTextureParameters(zisc::pmr::memory_resource* data_resource) noexcept;

  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  LdrImage image_;
};

/*!
  */
class TextureSettingNode : public SettingNodeBase
{
 public:
  //! Create a texture settings
  TextureSettingNode(const SettingNodeBase* parent) noexcept;


  //! Return the checkerboard texture parameters
  CheckerboardTextureParameters& checkerboardTextureParameters() noexcept;

  //! Return the checkerboard texture parameters
  const CheckerboardTextureParameters& checkerboardTextureParameters() const noexcept;

  //! Return the image texture parameters
  ImageTextureParameters& imageTextureParameters() noexcept;

  //! Return the image texture parameters
  const ImageTextureParameters& imageTextureParameters() const noexcept;

  //! Initialize the texture setting
  void initialize() noexcept override;

  //! Return the name of the texture
  std::string_view name() const noexcept;

  //! Return the node type
  static SettingNodeType nodeType() noexcept;

  //! Read the texture setting from the data stream
  void readData(std::istream* data_stream) noexcept override;

  //! Set the name of the texture
  void setName(const std::string_view& name) noexcept;

  //! Set the texture type
  void setTextureType(const TextureType type) noexcept;

  //! Return the texture type
  TextureType textureType() const noexcept;

  //! Return the node type
  SettingNodeType type() const noexcept override;

  //! Return the unicolor texture parameters
  UnicolorTextureParameters& unicolorTextureParameters() noexcept;

  //! Return the unicolor texture parameters
  const UnicolorTextureParameters& unicolorTextureParameters() const noexcept;

  //! Return the value texture parameter
  ValueTextureParameters& valueTextureParameters() noexcept;

  //! Return the value texture parameter
  const ValueTextureParameters& valueTextureParameters() const noexcept;

  //! Write the texture setting to the data stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  zisc::UniqueMemoryPointer<NodeParameterBase> parameters_;
  zisc::pmr::string name_;
  TextureType texture_type_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_TEXTURE_SETTING_NODE_HPP
