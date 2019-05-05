/*!
  \file face.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FACE_HPP
#define NANAIRO_FACE_HPP

// Standard C++ library
#include <array>
#include <limits>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class Face
{
 public:
  //! Make a invalid face
  Face() noexcept;

  //! Make a triangular face
  Face(const uint32 v0, const uint32 v1, const uint32 v2) noexcept;

  //! Make a quadrangular face
  Face(const uint32 v0, const uint32 v1, const uint32 v2, const uint32 v3) noexcept;


  //! Check if the face is quadrangular
  bool isQuadrangular() const noexcept;

  //! Check if the face is triangular
  bool isTriangular() const noexcept;

  //! Check if the face has vnormals
  bool hasVnormal() const noexcept;

  //! Check if the face has uv
  bool hasVuv() const noexcept;

  //! Return the null index
  static constexpr uint32 nullIndex() noexcept
  {
    return std::numeric_limits<uint32>::max();
  }

  //! Return the vertex indices of quadrangle
  const std::array<uint32, 4>& quadrangleVertexIndices() const noexcept;

  //! Return the vnormal indices of quadrangle
  const std::array<uint32, 4>& quadrangleVnormalIndices() const noexcept;

  //! Return the vuv indices of quadrangle
  const std::array<uint32, 4>& quadrangleVuvIndices() const noexcept;

  //! Set the smoothing flag
  void setSmoothing(const bool flag) noexcept;

  //! Return the smoothing flag
  bool smoothing() const noexcept;

  //! Set the vertex indices of triangule
  void setVertexIndices(const uint32 v0,
                        const uint32 v1,
                        const uint32 v2) noexcept;

  //! Set the vertex indices of quadrangle
  void setVertexIndices(const uint32 v0,
                        const uint32 v1,
                        const uint32 v2,
                        const uint32 v3) noexcept;

  //! Set the vnormal indices of triangule
  void setVnormalIndices(const uint32 n0,
                         const uint32 n1,
                         const uint32 n2) noexcept;

  //! Set the vnormal indices of quadrangle
  void setVnormalIndices(const uint32 n0,
                         const uint32 n1,
                         const uint32 n2,
                         const uint32 n3) noexcept;

  //! Set the vnormal indices of triangule
  void setVuvIndices(const uint32 t0,
                     const uint32 t1,
                     const uint32 t2) noexcept;

  //! Set the vnormal indices of quadrangle
  void setVuvIndices(const uint32 t0,
                     const uint32 t1,
                     const uint32 t2,
                     const uint32 t3) noexcept;

  //! Return the vertex indices of triangule
  const std::array<uint32, 3>& triangleVertexIndices() const noexcept;

  //! Return the vnormal indices of triangule
  const std::array<uint32, 3>& triangleVnormalIndices() const noexcept;

  //! Return the vuv indices of triangule
  const std::array<uint32, 3>& triangleVuvIndices() const noexcept;

 private:
  std::array<uint32, 4> vertex_indices_;
  std::array<uint32, 4> vnormal_indices_;
  std::array<uint32, 4> vuv_indices_;
  uint8 smoothing_;
  std::array<uint8, 3> padding_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_FACE_HPP
