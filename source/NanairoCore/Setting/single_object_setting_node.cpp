/*!
  \file single_object_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "single_object_setting_node.hpp"
// Standard C++ library
#include <array>
#include <istream>
#include <memory>
#include <ostream>
#include <vector>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/face.hpp"
#include "NanairoCore/Shape/shape.hpp"

namespace nanairo {

/*!
  */
void MeshParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&smoothing_, data_stream);
  // Read mesh information
  // Face
  {
    constexpr uint face_size = sizeof(face_list_[0]);
    uint32 num_of_faces = 0;
    zisc::read(&num_of_faces, data_stream);
    if (0 < num_of_faces) {
      face_list_.resize(num_of_faces);
      zisc::read(&face_list_[0], data_stream, face_size * num_of_faces);
    }
  }
  // Vertex
  {
    constexpr uint vertex_size = sizeof(vertex_list_[0]);
    static_assert(vertex_size == sizeof(double) * 3, "The size of vertex is wrong.");
    uint32 num_of_vertice = 0;
    zisc::read(&num_of_vertice, data_stream);
    if (0 < num_of_vertice) {
      vertex_list_.resize(num_of_vertice);
      zisc::read(&vertex_list_[0], data_stream, vertex_size * num_of_vertice);
    }
  }
  // Normal
  {
    constexpr uint normal_size = sizeof(vnormal_list_[0]);
    static_assert(normal_size == sizeof(double) * 3, "The size of normal is wrong.");
    uint32 num_of_normals = 0;
    zisc::read(&num_of_normals, data_stream);
    if (0 < num_of_normals) {
      vnormal_list_.resize(num_of_normals);
      zisc::read(&vnormal_list_[0], data_stream, normal_size * num_of_normals);
    }
  }
  // Texture UV
  {
    constexpr uint uv_size = sizeof(vuv_list_[0]);
    static_assert(uv_size == sizeof(double) * 2, "The size of uv is wrong.");
    uint32 num_of_uv = 0;
    zisc::read(&num_of_uv, data_stream);
    if (0 < num_of_uv) {
      vuv_list_.resize(num_of_uv);
      zisc::read(&vuv_list_[0], data_stream, uv_size * num_of_uv);
    }
  }
}

/*!
  */
void MeshParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&smoothing_, data_stream);
  // Face
  {
    constexpr uint face_size = sizeof(face_list_[0]);
    const uint32 num_of_faces = zisc::cast<uint32>(face_list_.size());
    zisc::write(&num_of_faces, data_stream);
    if (0 < num_of_faces)
      zisc::write(&face_list_[0], data_stream, face_size * num_of_faces);
  }
  // Vertex
  {
    constexpr uint vertex_size = sizeof(vertex_list_[0]);
    static_assert(vertex_size == sizeof(double) * 3, "The size of vertex is wrong.");
    const uint32 num_of_vertice = zisc::cast<uint32>(vertex_list_.size());
    zisc::write(&num_of_vertice, data_stream);
    if (0 < num_of_vertice)
      zisc::write(&vertex_list_[0], data_stream, vertex_size * num_of_vertice);
  }
  // Normal
  {
    constexpr uint normal_size = sizeof(vnormal_list_[0]);
    static_assert(normal_size == sizeof(double) * 3, "The size of normal is wrong.");
    const uint32 num_of_normals = zisc::cast<uint32>(vnormal_list_.size());
    zisc::write(&num_of_normals, data_stream);
    if (0 < num_of_normals)
      zisc::write(&vnormal_list_[0], data_stream, normal_size * num_of_normals);
  }
  // Texture
  {
    constexpr uint uv_size = sizeof(vuv_list_[0]);
    static_assert(uv_size == sizeof(double) * 2, "The size of uv is wrong.");
    const uint32 num_of_uv = zisc::cast<uint32>(vuv_list_.size());
    zisc::write(&num_of_uv, data_stream);
    if (0 < num_of_uv)
      zisc::write(&vuv_list_[0], data_stream, uv_size * num_of_uv);
  }
}

/*!
  */
uint32 SingleObjectSettingNode::emitterIndex() const noexcept
{
  return emitter_index_;
}

/*!
  */
void SingleObjectSettingNode::initialize() noexcept
{
  setShapeType(ShapeType::kPlane);
  setEmissive(false);
  setSurfaceIndex(0);
  setEmitterIndex(0);
}

/*!
  */
bool SingleObjectSettingNode::isEmissiveObject() const noexcept
{
  return is_emissive_object_ == kTrue;
}

/*!
  */
MeshParameters& SingleObjectSettingNode::meshParameters() noexcept
{
  ZISC_ASSERT(shapeType() == ShapeType::kMesh,
              "Invalid shape type is specified.");
  auto parameter = zisc::cast<MeshParameters*>(parameters_.get());
  return *parameter;
}

/*!
  */
const MeshParameters& SingleObjectSettingNode::meshParameters() const noexcept
{
  ZISC_ASSERT(shapeType() == ShapeType::kMesh,
              "Invalid shape type is specified.");
  auto parameter = zisc::cast<const MeshParameters*>(parameters_.get());
  return *parameter;
}

/*!
  */
void SingleObjectSettingNode::readData(std::istream* data_stream) noexcept
{
  {
    zisc::read(&shape_type_, data_stream);
    setShapeType(shape_type_);
  }
  zisc::read(&is_emissive_object_, data_stream);
  zisc::read(&surface_index_, data_stream);
  zisc::read(&emitter_index_, data_stream);
  if (parameters_)
    parameters_->readData(data_stream);
}

/*!
  */
void SingleObjectSettingNode::setEmissive(const bool emissive) noexcept
{
  is_emissive_object_ = emissive ? kTrue : kFalse;
}

/*!
  */
void SingleObjectSettingNode::setEmitterIndex(const uint32 index) noexcept
{
  emitter_index_ = index;
}

/*!
  */
void SingleObjectSettingNode::setShapeType(const ShapeType type) noexcept
{
  shape_type_ = type;
  // Initialize parameters
  parameters_.reset(nullptr);
  switch (shape_type_) {
   case ShapeType::kMesh: {
    parameters_ = std::make_unique<MeshParameters>();
    break;
   }
   case ShapeType::kPlane:
   default:
    break;
  }
}

/*!
  */
void SingleObjectSettingNode::setSurfaceIndex(const uint32 index) noexcept
{
  surface_index_ = index;
}

/*!
  */
ShapeType SingleObjectSettingNode::shapeType() const noexcept
{
  return shape_type_;
}

/*!
  */
uint32 SingleObjectSettingNode::surfaceIndex() const noexcept
{
  return surface_index_;
}

/*!
  */
SettingNodeType SingleObjectSettingNode::type() const noexcept
{
  return SettingNodeType::kSingleObject;
}

/*!
  */
void SingleObjectSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);
  // Write properties
  zisc::write(&shape_type_, data_stream);
  zisc::write(&is_emissive_object_, data_stream);
  zisc::write(&surface_index_, data_stream);
  zisc::write(&emitter_index_, data_stream);
  if (parameters_)
    parameters_->writeData(data_stream);
}

} // namespace nanairo
