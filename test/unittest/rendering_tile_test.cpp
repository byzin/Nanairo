/*!
  \file rendering_tile_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/point.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/rendering_tile.hpp"

TEST(RenderingTileTest, TileIterationTest)
{
  using nanairo::uint;
  using P = nanairo::Index2d;

  constexpr uint width = 5;
  constexpr uint height = 10;
  constexpr uint num_of_pixels = width * height;

  const nanairo::Index2d begin_pixel{5, 10};
  const nanairo::Index2d end_pixel{begin_pixel[0] + width, begin_pixel[1] + height};

  nanairo::RenderingTile tile{begin_pixel, end_pixel};

  ASSERT_EQ(width, tile.widthResolution())
      << "The initialization of the tile is failed.";
  ASSERT_EQ(height, tile.heightResolution())
      << "The initialization of the tile is failed.";
  ASSERT_EQ(num_of_pixels, tile.numOfPixels())
      << "The initialization of the tile is failed.";

/*!
  ( 5, 10) ( 6, 10) ( 7, 10) ( 8, 10) ( 9, 10)
  ( 5, 11) ( 6, 11) ( 7, 11) ( 8, 11) ( 9, 11)
  ( 5, 12) ( 6, 12) ( 7, 12) ( 8, 12) ( 9, 12)
  ( 5, 13) ( 6, 13) ( 7, 13) ( 8, 13) ( 9, 13)
  ( 5, 14) ( 6, 14) ( 7, 14) ( 8, 14) ( 9, 14)
  ( 5, 15) ( 6, 15) ( 7, 15) ( 8, 15) ( 9, 15)
  ( 5, 16) ( 6, 16) ( 7, 16) ( 8, 16) ( 9, 16)
  ( 5, 17) ( 6, 17) ( 7, 17) ( 8, 17) ( 9, 17)
  ( 5, 18) ( 6, 18) ( 7, 18) ( 8, 18) ( 9, 18)
  ( 5, 19) ( 6, 19) ( 7, 19) ( 8, 19) ( 9, 19)
  */

  const std::array<nanairo::Index2d, num_of_pixels> pixel_array{{
    P{5, 10},
    P{6, 10}, P{5, 11},
    P{7, 10}, P{6, 11}, P{5, 12},
    P{8, 10}, P{7, 11}, P{6, 12}, P{5, 13},
    P{9, 10}, P{8, 11}, P{7, 12}, P{6, 13}, P{5, 14},
    P{9, 11}, P{8, 12}, P{7, 13}, P{6, 14}, P{5, 15},
    P{9, 12}, P{8, 13}, P{7, 14}, P{6, 15}, P{5, 16},
    P{9, 13}, P{8, 14}, P{7, 15}, P{6, 16}, P{5, 17},
    P{9, 14}, P{8, 15}, P{7, 16}, P{6, 17}, P{5, 18},
    P{9, 15}, P{8, 16}, P{7, 17}, P{6, 18}, P{5, 19},
    P{9, 16}, P{8, 17}, P{7, 18}, P{6, 19},
    P{9, 17}, P{8, 18}, P{7, 19},
    P{9, 18}, P{8, 19},
    P{9, 19}}};

  for (uint i = 0; i < num_of_pixels; ++i) {
    const auto& expected = pixel_array[i];
    const auto& p = tile.current();
    ASSERT_EQ(expected.data(), p.data()) << "The next function is wrong.";
    tile.next();
  }
}

TEST(RenderingTileTest, OneLineTest)
{
  using nanairo::uint;
  using P = nanairo::Index2d;

  {
    constexpr uint width = 5;
    constexpr uint height = 1;
    constexpr uint num_of_pixels = width * height;

    const nanairo::Index2d begin_pixel{0, 0};
    const nanairo::Index2d end_pixel{begin_pixel[0] + width,
                                     begin_pixel[1] + height};

    nanairo::RenderingTile tile{begin_pixel, end_pixel};

    const std::array<nanairo::Index2d, num_of_pixels> pixel_array{{
      P{0, 0}, P{1, 0}, P{2, 0}, P{3, 0}, P{4, 0}}};

    for (uint i = 0; i < num_of_pixels; ++i) {
      const auto& expected = pixel_array[i];
      const auto& p = tile.current();
      ASSERT_EQ(expected.data(), p.data()) << "The next function is wrong.";
      tile.next();
    }
  }
  {
    constexpr uint width = 1;
    constexpr uint height = 5;
    constexpr uint num_of_pixels = width * height;

    const nanairo::Index2d begin_pixel{0, 0};
    const nanairo::Index2d end_pixel{begin_pixel[0] + width,
                                     begin_pixel[1] + height};

    nanairo::RenderingTile tile{begin_pixel, end_pixel};

    const std::array<nanairo::Index2d, num_of_pixels> pixel_array{{
      P{0, 0}, P{0, 1}, P{0, 2}, P{0, 3}, P{0, 4}}};

    for (uint i = 0; i < num_of_pixels; ++i) {
      const auto& expected = pixel_array[i];
      const auto& p = tile.current();
      ASSERT_EQ(expected.data(), p.data()) << "The next function is wrong.";
      tile.next();
    }
  }
}

TEST(RenderingTileTest, OnePixelTest)
{
  using nanairo::uint;
  using P = nanairo::Index2d;

  constexpr uint width = 1;
  constexpr uint height = 1;
  constexpr uint num_of_pixels = width * height;

  const nanairo::Index2d begin_pixel{0, 0};
  const nanairo::Index2d end_pixel{begin_pixel[0] + width,
                                   begin_pixel[1] + height};

  nanairo::RenderingTile tile{begin_pixel, end_pixel};

  const std::array<nanairo::Index2d, num_of_pixels> pixel_array{{P{0, 0}}};

  for (uint i = 0; i < num_of_pixels; ++i) {
    const auto& expected = pixel_array[i];
    const auto& p = tile.current();
    ASSERT_EQ(expected.data(), p.data()) << "The next function is wrong.";
    tile.next();
  }
}
