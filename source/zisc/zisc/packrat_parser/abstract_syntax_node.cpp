/*!
  \file abstract_syntax_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "abstract_syntax_node.hpp"
// Standard C++ library
#include <memory>
#include <string>
#include <vector>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
AbstractSyntaxNode::~AbstractSyntaxNode()
{
}

/*!
  */
bool AbstractSyntaxNode::isInternal() const noexcept
{
  return false;
}

/*!
  */
bool AbstractSyntaxNode::isLeaf() const noexcept
{
  return false;
}

/*!
  */
bool AbstractSyntaxInternalNode::isInternal() const noexcept
{
  return true;
}

/*!
  */
bool AbstractSyntaxLeafNode::isLeaf() const noexcept
{
  return true;
}

} // namespace zisc
