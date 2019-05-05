/*!
  \file rendering_tile_inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RENDERING_TILE_INL_HPP
#define NANAIRO_RENDERING_TILE_INL_HPP

#include "rendering_tile.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/point.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
RenderingTile::RenderingTile(const Index2d& begin, const Index2d& end) noexcept :
    begin_{begin},
    end_{end},
    current_{begin}
{
}

/*!
  */
inline
const Index2d& RenderingTile::begin() const noexcept
{
  return begin_;
}

/*!
  */
inline
const Index2d& RenderingTile::current() const noexcept
{
  return current_;
}

/*!
  */
inline
const Index2d& RenderingTile::end() const noexcept
{
  return end_;
}

/*!
  */
inline
uint RenderingTile::getIndex(const Index2d& pixel) const noexcept
{
  const uint x = pixel[0] - begin_[0];
  const uint y = pixel[1] - begin_[1];
  const uint index = x + widthResolution() * y;
  return index;
}

/*!
  */
inline
uint RenderingTile::heightResolution() const noexcept
{
  const uint h = zisc::cast<uint>(end_[1] - begin_[1]);
  return h;
}

/*!
  */
inline
const Index2d& RenderingTile::next() noexcept
{
  const uint row = current_[1] - begin_[1];
  if (zisc::isOdd(row)) {
    if (current_[0] == begin_[0])
      ++current_[1];
    else
      --current_[0];
  }
  else {
    if (current_[0] == (end_[0] - 1))
      ++current_[1];
    else
      ++current_[0];
  }
  return current_;
}

/*!
  */
inline
uint RenderingTile::numOfPixels() const noexcept
{
  const uint num_of_pixels = widthResolution() * heightResolution();
  return num_of_pixels;
}

/*!
  */
inline
void RenderingTile::reset() noexcept
{
  current_ = begin();
}

/*!
  */
inline
Index2d RenderingTile::resolution() const noexcept
{
  const auto w = widthResolution();
  const auto h = heightResolution();
  const Index2d r{w, h};
  return r;
}

/*!
  */
inline
uint RenderingTile::widthResolution() const noexcept
{
  const uint w = zisc::cast<uint>(end_[0] - begin_[0]);
  return w;
}

/*!
  */
inline
void RenderingTile::initialize() noexcept
{
  ZISC_ASSERT(begin_[0] < end_[0],
              "The end pixel index is less than or equal begin.");
  ZISC_ASSERT(begin_[1] < end_[1],
              "The end pixel index is less than or equal begin.");
}

} // namespace nanairo

#endif // NANAIRO_RENDERING_TILE_INL_HPP
