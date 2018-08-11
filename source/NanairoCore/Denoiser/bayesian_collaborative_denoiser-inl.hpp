/*!
  \file bayesian_collaborative_denoiser-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BAYESIAN_COLLABORATIVE_DENOISER_INL_HPP
#define NANAIRO_BAYESIAN_COLLABORATIVE_DENOISER_INL_HPP

#include "bayesian_collaborative_denoiser.hpp"
// Standard C++ library
#include <array>
// Zisc
#include "zisc/point.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
constexpr std::array<Index2d, 9> BayesianCollaborativeDenoiser::getChunkTileOrder()
    noexcept
{
  std::array<Index2d, 9> order{{
      Index2d{1, 1},
      Index2d{0, 1},
      Index2d{0, 0},
      Index2d{1, 0},
      Index2d{2, 0},
      Index2d{2, 1},
      Index2d{2, 2},
      Index2d{1, 2},
      Index2d{0, 2}}};
  return order;
}

/*!
  */
inline
constexpr uint BayesianCollaborativeDenoiser::getCovarianceMatrixSize(
    const uint dimension) noexcept
{
  const uint size = ((dimension + 1) * dimension) >> 1;
  return size;
}

} // namespace nanairo

#endif // NANAIRO_BAYESIAN_COLLABORATIVE_DENOISER_INL_HPP
