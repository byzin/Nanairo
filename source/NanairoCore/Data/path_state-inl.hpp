/*!
  \file path_state-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PATH_STATE_INL_HPP
#define NANAIRO_PATH_STATE_INL_HPP

#include "path_state.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"

namespace nanairo {

/*!
  */
inline
PathState::PathState() noexcept
{
}

/*!
  */
inline
PathState::PathState(const uint32 s) noexcept : sample_{s}
{
}

/*!
  */
inline
void PathState::incrementLength() noexcept
{
  ++length_;
}

/*!
  */
inline
uint32 PathState::dimension() const noexcept
{
  return dimension_;
}

/*!
  */
inline
uint32 PathState::length() const noexcept
{
  return length_;
}

/*!
  */
inline
uint32 PathState::sample() const noexcept
{
  return sample_;
}

/*!
  */
inline
void PathState::setDimension(const uint32 d) noexcept
{
  dimension_ = d;
}

/*!
  */
inline
void PathState::setDimension(const SampleDimension d) noexcept
{
  dimension_ = zisc::cast<uint32>(d);
}

/*!
  */
inline
void PathState::setLength(const uint32 l) noexcept
{
  length_ = l;
}

/*!
  */
inline
void PathState::setSample(const uint32 s) noexcept
{
  sample_ = s;
}

} // namespace nanairo

#endif // NANAIRO_PATH_STATE_INL_HPP
