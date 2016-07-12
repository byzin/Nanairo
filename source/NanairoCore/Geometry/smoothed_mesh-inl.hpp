/*!
  \file smoothed_mesh-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTHED_MESH_INL_HPP
#define NANAIRO_SMOOTHED_MESH_INL_HPP

#include "smoothed_mesh.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

using zisc::treatAs;

/*!
  \details
  No detailed.
  */
inline
Vector3 SmoothedMesh::normal(const Float eta, const Float xi) const noexcept
{
  ZISC_ASSERT((0.0 <= xi) && (xi <= eta), "The xi must be [0, eta]: ", xi);
  ZISC_ASSERT((xi <= eta) && (eta <= 1.0), "The eta must be [xi, 1]: ", eta);
  const auto x1 = c_[1] + c_[3] * xi + 2.0 * c_[4] * eta;
  const auto x2 = c_[2] + c_[3] * eta + 2.0 * c_[5] * xi;
  return zisc::cross(x1, x2).normalized();
}

/*!
  \details
  No detailed.
  */
inline
Point3 SmoothedMesh::point(const Float eta, const Float xi) const noexcept
{
  ZISC_ASSERT((0.0 <= xi) && (xi <= eta), "The xi must be [0, eta]: ", xi);
  ZISC_ASSERT((xi <= eta) && (eta <= 1.0), "The eta must be [xi, 1]: ", eta);
  const auto point = c_[0] + (c_[1] + c_[4] * eta) * eta +
                     (c_[2] + c_[3] * eta + c_[5] * xi) * xi;
  return *treatAs<const Point3*>(&point);
}

} // namespace nanairo

#endif // NANAIRO_SMOOTHED_MESH_INL_HPP
