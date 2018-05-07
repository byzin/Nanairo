/*!
  \file morton_code_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <algorithm>
#include <array>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/simple_memory_resource.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/DataStructure/morton_code.hpp"

TEST(MortonCodeTest, SplitTest)
{
  using nanairo::MortonCode;

  std::array<nanairo::uint64, 8> code_list{{
      0b00001,
      0b00010,
      0b00100,
      0b00101,
      0b10011,
      0b11000,
      0b11001,
      0b11110}};

  auto work_resource = zisc::SimpleMemoryResource::sharedResource();
  zisc::pmr::vector<MortonCode> morton_code_list{
      decltype(morton_code_list)::allocator_type{work_resource}};
  {
    morton_code_list.reserve(code_list.size());
    for (const auto code : code_list)
      morton_code_list.emplace_back(nullptr, code);
  }

  auto first = morton_code_list.begin();
  auto begin = first;
  auto end = morton_code_list.end();

  ASSERT_TRUE(std::is_sorted(begin, end)) <<
      "The morton code list must be sorted.";

  // First splitting
  constexpr nanairo::uint key_bit = 5;
  auto position = MortonCode::findSplit(key_bit, begin, end);
  ASSERT_EQ(first + 4, position) << "The func 'findSplit' is wrong.";

  // Left
  {
    constexpr nanairo::uint key_bit1 = 3;
    auto begin1 = begin;
    auto end1 = position;
    auto position1 = MortonCode::findSplit(key_bit1, begin1, end1);
    ASSERT_EQ(first + 2, position1) << "The func 'findSplit' is wrong.";

    {
      constexpr nanairo::uint key_bit2 = 2;
      auto begin2 = begin1;
      auto end2 = position1;
      auto position2 = MortonCode::findSplit(key_bit2, begin2, end2);
      ASSERT_EQ(first + 1, position2) << "The func 'findSplit' is wrong.";
    }
    {
      constexpr nanairo::uint key_bit2 = 1;
      auto begin2 = position1;
      auto end2 = end1;
      auto position2 = MortonCode::findSplit(key_bit2, begin2, end2);
      ASSERT_EQ(first + 3, position2) << "The func 'findSplit' is wrong.";
    }
  }
  // Right
  {
    constexpr nanairo::uint key_bit1 = 4;
    auto begin1 = position;
    auto end1 = end;
    auto position1 = MortonCode::findSplit(key_bit1, begin1, end1);
    ASSERT_EQ(first + 5, position1) << "The func 'findSplit' is wrong.";

    {
      constexpr nanairo::uint key_bit2 = 3;
      auto begin2 = position1;
      auto end2 = end1;
      auto position2 = MortonCode::findSplit(key_bit2, begin2, end2);
      ASSERT_EQ(first + 7, position2) << "The func 'findSplit' is wrong.";

      {
        constexpr nanairo::uint key_bit3 = 1;
        auto begin3 = begin2;
        auto end3 = position2;
        auto position3 = MortonCode::findSplit(key_bit3, begin3, end3);
        ASSERT_EQ(first + 6, position3) << "The func 'findSplit' is wrong.";
      }
    }
  }
}
