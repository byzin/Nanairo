/*!
  \file morton_code-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MORTON_CODE_INL_HPP
#define NANAIRO_MORTON_CODE_INL_HPP

#include "morton_code.hpp"
// Standard C++ library
#include <limits>
// Nanairo
#include "bvh_building_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
MortonCode::MortonCode() noexcept :
    node_{nullptr},
    code_{invalidCode()}
{
}

/*!
  */
inline
MortonCode::MortonCode(const BvhBuildingNode* node, const CodeType code) noexcept :
    node_{node},
    code_{code}
{
}

/*!
  */
inline
MortonCode::MortonCode(MortonCode&& other) noexcept :
    node_{nullptr},
    code_{invalidCode()}
{
  swap(*this, other);
}

/*!
  */
inline
MortonCode& MortonCode::operator=(MortonCode&& other) noexcept
{
  swap(*this, other);
  return *this;
}

/*!
  */
inline
auto MortonCode::code() const noexcept -> CodeType
{
  return code_;
}

/*!
  */
inline
constexpr auto MortonCode::invalidCode() noexcept -> CodeType
{
  constexpr CodeType code = std::numeric_limits<CodeType>::max();
  return code;
}

/*!
  */
inline
const BvhBuildingNode* MortonCode::node() const noexcept
{
  return node_;
}

/*!
  */
inline
void MortonCode::setCode(const CodeType code) noexcept
{
  code_ = code;
}

/*!
  */
inline
void MortonCode::setNode(const BvhBuildingNode* node) noexcept
{
  node_ = node;
}

/*!
  */
inline
bool operator<(const MortonCode& lhs, const MortonCode& rhs) noexcept
{
  return lhs.code() < rhs.code();
}

/*!
  */
inline
void swap(MortonCode& a, MortonCode& b) noexcept
{
  {
    const auto node = a.node();
    a.setNode(b.node());
    b.setNode(node);
  }
  {
    const auto code = a.code();
    a.setCode(b.code());
    b.setCode(code);
  }
}

} // namespace nanairo

#endif // NANAIRO_MORTON_CODE_INL_HPP
