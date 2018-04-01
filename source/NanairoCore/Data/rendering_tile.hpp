/*!
  \file rendering_tile.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RENDERING_TILE_HPP
#define NANAIRO_RENDERING_TILE_HPP

// Zisc
#include "zisc/point.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class RenderingTile
{
 public:
  //! Make a rendering tile
  RenderingTile(const Index2d& begin, const Index2d& end) noexcept;


  //! Return the begin pixel index
  const Index2d& begin() const noexcept;

  //! Return the current pixel index
  const Index2d& current() const noexcept;

  //! Return the end pixel index
  const Index2d& end() const noexcept;

  //! Return the height resolution
  uint heightResolution() const noexcept;

  //! Next the current pixel index
  const Index2d& next() noexcept;

  //! Return the num of pixels
  uint numOfPixels() const noexcept;

  //! Reset the current pixel index
  void reset() noexcept;

  //! Return the image resolution
  Index2d resolution() const noexcept;

  //! Return the width resolution
  uint widthResolution() const noexcept;

 private:
  //! Initialize
  void initialize() noexcept;

  //! Back the pixel to the line start
  void backToLineStart(Index2d* pixel) noexcept;

  //! Check if the pixel is bottom end
  bool isBottomEnd(const Index2d& pixel) const noexcept;

  //! Check if the pixel is left end
  bool isLeftEnd(const Index2d& pixel) const noexcept;

  //! Check if the pixel is right end
  bool isRightEnd(const Index2d& pixel) const noexcept;

  //! Check if the pixel is top end
  bool isTopEnd(const Index2d& pixel) const noexcept;

  //! Move the pixel to bottom
  void moveToBottom(Index2d* pixel) noexcept;

  //! Move the pixel to left bottom
  void moveToLeftBottom(Index2d* pixel) noexcept;

  //! Move the pixel to right
  void moveToRight(Index2d* pixel) noexcept;


  Index2d begin_,
          end_;
  Index2d current_;
};

//! \} Core

} // namespace nanairo

#include "rendering_tile-inl.hpp"

#endif // NANAIRO_RENDERING_TILE_HPP
