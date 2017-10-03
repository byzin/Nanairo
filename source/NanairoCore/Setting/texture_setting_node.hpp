/*!
  \file texture_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
  UnicolorTextureParameters() noexcept;

  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  std::unique_ptr<SettingNodeBase> color_;
};

//! CheckerboardTexture parameters
struct CheckerboardTextureParameters : public NodeParameterBase
{
  //! Initialize parameters
  CheckerboardTextureParameters() noexcept;

  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  std::array<std::unique_ptr<SettingNodeBase>, 2> color_;
  std::array<uint32, 2> resolution_;
};

//! ImageTexture parameters
struct ImageTextureParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  LdrImage image_{1, 1};
};

/*!
  */
class TextureSettingNode : public SettingNodeBase
{
 public:
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
  const std::string& name() const noexcept;

  //! Read the texture setting from the data stream
  void readData(std::istream* data_stream) noexcept override;

  //! Set the name of the texture
  void setName(const std::string& name) noexcept;

  //! Set the name of the texture
  void setName(std::string&& name) noexcept;

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
  std::unique_ptr<NodeParameterBase> parameters_;
  std::string name_;
  TextureType texture_type_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_TEXTURE_SETTING_NODE_HPP
