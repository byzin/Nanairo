/*!
  \file setting_node_base.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SETTING_NODE_BASE_HPP
#define NANAIRO_SETTING_NODE_BASE_HPP

// Standard C++ library
#include <istream>
#include <ostream>
#include <string>
#include <string_view>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/sip_hash_engine.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! Nanairo setting types
enum class SettingNodeType : uint32
{
  // Root node
  kScene                      = zisc::SipHash32::hash("Scene"),
  // System
  kSystem                     = zisc::SipHash32::hash("System"),
  kColor                      = zisc::SipHash32::hash("Color"),
  kSpectra                    = zisc::SipHash32::hash("Spectra"),
  // Rendering method
  kRenderingMethod            = zisc::SipHash32::hash("RenderingMethod"),
  // Texture model
  kTextureModel               = zisc::SipHash32::hash("TextureModel"),
  kTexture                    = zisc::SipHash32::hash("Texture"),
  // Surface model
  kSurfaceModel               = zisc::SipHash32::hash("SurfaceModel"),
  kSurface                    = zisc::SipHash32::hash("Surface"),
  // Emitter model
  kEmitterModel               = zisc::SipHash32::hash("EmitterModel"),
  kEmitter                    = zisc::SipHash32::hash("Emitter"),
  // Object
  kObjectModel                = zisc::SipHash32::hash("ObjectModel"),
  kCameraObject               = zisc::SipHash32::hash("CameraObject"),
  kGroupObject                = zisc::SipHash32::hash("GroupObject"),
  kSingleObject               = zisc::SipHash32::hash("SingleObject"),
  kTransformation             = zisc::SipHash32::hash("Transformation"),
  // BVH
  kBvh                        = zisc::SipHash32::hash("Bvh"),
};

/*!
  */
class NodeParameterBase
{
 public:
  //!
  virtual ~NodeParameterBase() noexcept;

  //! Read the parameters from the stream
  virtual void readData(std::istream* data_stream) noexcept = 0;

  //! Write the parameters to the stream
  virtual void writeData(std::ostream* data_stream) const noexcept = 0;
};

/*!
  */
class SettingNodeBase
{
 public:
  SettingNodeBase(const SettingNodeBase* parent) noexcept;

  //!
  virtual ~SettingNodeBase() noexcept;


  //! Return the memory resource for data allocation
  virtual zisc::pmr::memory_resource* dataResource() const noexcept;

  //! Initialize a node
  virtual void initialize() noexcept = 0;

  //! Check if this node is a root node
  bool isRoot() const noexcept;

  //! Return the parent node
  const SettingNodeBase* parent() const noexcept;

  //! Read the setting data from the stream
  virtual void readData(std::istream* data_stream) noexcept = 0;

  //! Return the root node of the tree
  const SettingNodeBase* root() const noexcept;

  //! Return the node type
  virtual SettingNodeType type() const noexcept = 0;

  //! Return the memory resource for temporary works
  virtual zisc::pmr::memory_resource* workResource() const noexcept;
 
  //! Write the setting data to the stream
  virtual void writeData(std::ostream* data_stream) const noexcept = 0;

 protected:
  //! Read the string data from the stream
  zisc::pmr::string readString(std::istream* data_stream) const noexcept;

  //! Read the node type from the stream
  SettingNodeType readType(std::istream* data_stream) const noexcept;

  //! Write the string data to the stream
  void writeString(const std::string_view& text,
                   std::ostream* data_stream) const noexcept;

  //! Write the node type to the stream
  void writeType(std::ostream* data_stream) const noexcept;

 private:
  const SettingNodeBase* parent_;
};

//! Cast a node
template <typename NodeType>
NodeType* castNode(SettingNodeBase* node) noexcept;

//! Cast a node
template <typename NodeType>
const NodeType* castNode(const SettingNodeBase* node) noexcept;

} // namespace nanairo

//! \} Core

#include "setting_node_base-inl.hpp"

#endif // NANAIRO_SETTING_NODE_BASE_HPP
