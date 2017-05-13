/*!
  \file single_object_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SINGLE_OBJECT_SETTING_NODE_HPP
#define NANAIRO_SINGLE_OBJECT_SETTING_NODE_HPP

// Standard C++ library
#include <array>
#include <istream>
#include <limits>
#include <memory>
#include <ostream>
#include <vector>
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/face.hpp"
#include "NanairoCore/Shape/shape.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! Mesh parameters
struct MeshParameters : public NodeParameters
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  std::vector<Face> face_list_;
  std::vector<std::array<double, 3>> vertex_list_;
  std::vector<std::array<double, 3>> vnormal_list_;
  std::vector<std::array<double, 2>> vuv_list_;
  uint8 smoothing_ = SettingNodeBase::kFalse;
};

/*!
  */
class SingleObjectSettingNode : public SettingNodeBase
{
 public:
  //! Return the emitter index of the object
  uint32 emitterIndex() const noexcept;

  //! Initialize a object
  virtual void initialize() noexcept override;

  //! Check if the object is emissive
  bool isEmissiveObject() const noexcept;

  //! Return the mesh parameters
  MeshParameters& meshParameters() noexcept;

  //! Return the mesh parameters
  const MeshParameters& meshParameters() const noexcept;

  //! Read the setting data from the stream
  virtual void readData(std::istream* data_stream) noexcept override;

  //! Set the emissive flag
  void setEmissive(const bool emissive) noexcept;

  //! Set the emitter index
  void setEmitterIndex(const uint32 index) noexcept;

  //! Set the shape type
  void setShapeType(const ShapeType type) noexcept;

  //! Set the surface index
  void setSurfaceIndex(const uint32 index) noexcept;

  //! Return the shape type
  ShapeType shapeType() const noexcept;

  //! Return the surface index of the object
  uint32 surfaceIndex() const noexcept;

  //! Return the node type
  SettingNodeType type() const noexcept override;

  //! Write the string data to the stream
  void writeData(std::ostream* data_stream) const noexcept override;


  static constexpr uint32 kInvalidIndex = std::numeric_limits<uint32>::max();

 private:
  std::unique_ptr<NodeParameters> parameters_;
  ShapeType shape_type_;
  uint32 surface_index_,
         emitter_index_;
  uint8 is_emissive_object_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SINGLE_OBJECT_SETTING_NODE_HPP
