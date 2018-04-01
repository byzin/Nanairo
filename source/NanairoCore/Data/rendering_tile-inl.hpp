/*!
  \file rendering_tile_inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
  const bool is_last = isRightEnd(current_) && isBottomEnd(current_);
  if (!is_last) {
    if (isLeftEnd(current_) || isBottomEnd(current_)) {
      backToLineStart(&current_);
      if (isRightEnd(current_))
        moveToBottom(&current_);
      else
        moveToRight(&current_);
    }
    else {
      moveToLeftBottom(&current_);
    }
  }
  return current();
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

/*!
  */
inline
void RenderingTile::backToLineStart(Index2d* pixel) noexcept
{
  ZISC_ASSERT(pixel != nullptr, "The pixel is null.");
  const auto& b = begin();
  const auto& e = end();

  const auto d1 = (e[0] - 1) - (*pixel)[0];
  const auto d2 = (*pixel)[1] - b[1];

  const auto d = zisc::min(d1, d2);

  (*pixel)[0] = (*pixel)[0] + d;
  (*pixel)[1] = (*pixel)[1] - d;
}

/*!
  */
inline
bool RenderingTile::isBottomEnd(const Index2d& pixel) const noexcept
{
  const auto& e = end();
  ZISC_ASSERT(pixel[1] < e[1], "The pixel is out of range.");
  const bool result = pixel[1] == (e[1] - 1);
  return result;
}

/*!
  */
inline
bool RenderingTile::isLeftEnd(const Index2d& pixel) const noexcept
{
  const auto& b = begin();
  ZISC_ASSERT(b[0] <= pixel[0], "The pixel is out of range.");
  const bool result = pixel[0] == b[0];
  return result;
}

/*!
  */
inline
bool RenderingTile::isRightEnd(const Index2d& pixel) const noexcept
{
  const auto& e = end();
  ZISC_ASSERT(pixel[0] < e[0], "The pixel is out of range.");
  const bool result = pixel[0] == (e[0] - 1);
  return result;
}

/*!
  */
inline
bool RenderingTile::isTopEnd(const Index2d& pixel) const noexcept
{
  const auto& b = begin();
  ZISC_ASSERT(b[1] <= pixel[1], "The pixel is out of range.");
  const bool result = pixel[1] == b[1];
  return result;
}

/*!
  */
inline
void RenderingTile::moveToBottom(Index2d* pixel) noexcept
{
  ZISC_ASSERT(pixel != nullptr, "The pixel is null.");
  ++(*pixel)[1];
}

/*!
  */
inline
void RenderingTile::moveToLeftBottom(Index2d* pixel) noexcept
{
  ZISC_ASSERT(pixel != nullptr, "The pixel is null.");
  --(*pixel)[0];
  ++(*pixel)[1];
}

/*!
  */
inline
void RenderingTile::moveToRight(Index2d* pixel) noexcept
{
  ZISC_ASSERT(pixel != nullptr, "The pixel is null.");
  ++(*pixel)[0];
}

} // namespace nanairo

#endif // NANAIRO_RENDERING_TILE_INL_HPP
