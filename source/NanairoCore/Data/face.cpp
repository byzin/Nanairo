/*!
  \file face.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "face.hpp"
// Standard C++ library
#include <array>
#include <limits>
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

Face::Face() noexcept :
    vertex_indices_{{kInvalidIndex, kInvalidIndex, kInvalidIndex, kInvalidIndex}},
    vnormal_indices_{{kInvalidIndex, kInvalidIndex, kInvalidIndex, kInvalidIndex}},
    vuv_indices_{{kInvalidIndex, kInvalidIndex, kInvalidIndex, kInvalidIndex}},
    smoothing_{kFalse},
    padding_{{0, 0, 0}}
{
}

/*!
  */
Face::Face(const uint32 v0, const uint32 v1, const uint32 v2) noexcept :
    vertex_indices_{{v0, v1, v2, kInvalidIndex}},
    vnormal_indices_{{kInvalidIndex, kInvalidIndex, kInvalidIndex, kInvalidIndex}},
    vuv_indices_{{kInvalidIndex, kInvalidIndex, kInvalidIndex, kInvalidIndex}},
    smoothing_{kFalse}
{
  static_assert(sizeof(Face) == sizeof(uint32) * 13, "The size of face is invalid.");
  padding_.fill(0);
}

/*!
  */
Face::Face(const uint32 v0, const uint32 v1, const uint32 v2, const uint32 v3) noexcept :
    vertex_indices_{{v0, v1, v2, v3}},
    vnormal_indices_{{kInvalidIndex, kInvalidIndex, kInvalidIndex, kInvalidIndex}},
    vuv_indices_{{kInvalidIndex, kInvalidIndex, kInvalidIndex, kInvalidIndex}},
    smoothing_{kFalse}
{
  static_assert(sizeof(Face) == sizeof(uint32) * 13, "The size of face is invalid.");
  padding_.fill(0);
}

/*!
  */
bool Face::isQuadrangular() const noexcept
{
  const auto& vertex_indices = quadrangleVertexIndices();
  return vertex_indices[3] != kInvalidIndex;
}

/*!
  */
bool Face::isTriangular() const noexcept
{
  return !isQuadrangular();
}

/*!
  */
bool Face::hasVnormal() const noexcept
{
  const auto& vnormal_indices = quadrangleVnormalIndices();
  return vnormal_indices[0] != kInvalidIndex;
}

/*!
  */
bool Face::hasVuv() const noexcept
{
  const auto& vuv_indices = quadrangleVuvIndices();
  return vuv_indices[0] != kInvalidIndex;
}

/*!
  */
const std::array<uint32, 4>& Face::quadrangleVertexIndices() const noexcept
{
  return vertex_indices_;
}

/*!
  */
const std::array<uint32, 4>& Face::quadrangleVnormalIndices() const noexcept
{
  return vnormal_indices_;
}

/*!
  */
const std::array<uint32, 4>& Face::quadrangleVuvIndices() const noexcept
{
  return vuv_indices_;
}

/*!
  */
void Face::setSmoothing(const bool flag) noexcept
{
  smoothing_ = (flag) ? kTrue : kFalse;
}

/*!
  */
bool Face::smoothing() const noexcept
{
  return (smoothing_ == kTrue);
}

/*!
  */
void Face::setVertexIndices(const uint32 v0,
                            const uint32 v1,
                            const uint32 v2) noexcept
{
  vertex_indices_[0] = v0;
  vertex_indices_[1] = v1;
  vertex_indices_[2] = v2;
  vertex_indices_[3] = kInvalidIndex;
}

/*!
  */
void Face::setVertexIndices(const uint32 v0,
                            const uint32 v1,
                            const uint32 v2,
                            const uint32 v3) noexcept
{
  vertex_indices_[0] = v0;
  vertex_indices_[1] = v1;
  vertex_indices_[2] = v2;
  vertex_indices_[3] = v3;
}

/*!
  */
void Face::setVnormalIndices(const uint32 n0,
                             const uint32 n1,
                             const uint32 n2) noexcept
{
  vnormal_indices_[0] = n0;
  vnormal_indices_[1] = n1;
  vnormal_indices_[2] = n2;
  vnormal_indices_[3] = kInvalidIndex;
}

/*!
  */
void Face::setVnormalIndices(const uint32 n0,
                             const uint32 n1,
                             const uint32 n2,
                             const uint32 n3) noexcept
{
  vnormal_indices_[0] = n0;
  vnormal_indices_[1] = n1;
  vnormal_indices_[2] = n2;
  vnormal_indices_[3] = n3;
}

/*!
  */
void Face::setVuvIndices(const uint32 t0,
                         const uint32 t1,
                         const uint32 t2) noexcept
{
  vuv_indices_[0] = t0;
  vuv_indices_[1] = t1;
  vuv_indices_[2] = t2;
  vuv_indices_[3] = kInvalidIndex;
}

/*!
  */
void Face::setVuvIndices(const uint32 t0,
                         const uint32 t1,
                         const uint32 t2,
                         const uint32 t3) noexcept
{
  vuv_indices_[0] = t0;
  vuv_indices_[1] = t1;
  vuv_indices_[2] = t2;
  vuv_indices_[3] = t3;
}

/*!
  */
const std::array<uint32, 3>& Face::triangleVertexIndices() const noexcept
{
  const auto vertex_indices =
      zisc::treatAs<const std::array<uint32, 3>*>(&vertex_indices_);
  return *vertex_indices;
}

/*!
  */
const std::array<uint32, 3>& Face::triangleVnormalIndices() const noexcept
{
  const auto vnormal_indices =
      zisc::treatAs<const std::array<uint32, 3>*>(&vnormal_indices_);
  return *vnormal_indices;
}

/*!
  */
const std::array<uint32, 3>& Face::triangleVuvIndices() const noexcept
{
  const auto vuv_indices =
      zisc::treatAs<const std::array<uint32, 3>*>(&vuv_indices_);
  return *vuv_indices;
}

} // namespace nanairo
