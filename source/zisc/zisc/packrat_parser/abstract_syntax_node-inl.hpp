/*!
  \file abstract_syntax_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_ABSTRACT_SYNTAX_NODE_INL_HPP
#define ZISC_ABSTRACT_SYNTAX_NODE_INL_HPP

#include "abstract_syntax_node.hpp"
// Standard C++ library
#include <string>
#include <vector>
#include <utility>
// Zisc
#include "../error.hpp"
#include "../utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

// AbstractSyntaxNode

/*!
  */
inline
uint AbstractSyntaxNode::expressionId() const noexcept
{
  return expression_id_;
}

/*!
  */
inline
AbstractSyntaxInternalNode* AbstractSyntaxNode::treatAsInternalNode() noexcept
{
  ZISC_ASSERT(isInternal(), "The node is not internal node.");
  return treatAs<AbstractSyntaxInternalNode*>(this);
}

/*!
  */
inline
const AbstractSyntaxInternalNode* AbstractSyntaxNode::treatAsInternalNode() 
    const noexcept
{
  ZISC_ASSERT(isInternal(), "The node is not internal node.");
  return treatAs<const AbstractSyntaxInternalNode*>(this);
}

/*!
  */
inline
AbstractSyntaxLeafNode* AbstractSyntaxNode::treatAsLeafNode() noexcept
{
  ZISC_ASSERT(isLeaf(), "The node is not leaf node.");
  return treatAs<AbstractSyntaxLeafNode*>(this);
}

/*!
  */
inline
const AbstractSyntaxLeafNode* AbstractSyntaxNode::treatAsLeafNode() const noexcept
{
  ZISC_ASSERT(isLeaf(), "The node is not leaf node.");
  return treatAs<const AbstractSyntaxLeafNode*>(this);
}

/*!
  */
inline
AbstractSyntaxNode::AbstractSyntaxNode(const uint expression_id) noexcept :
    expression_id_{expression_id}
{
}

// AbstractSyntaxInternalNode

/*!
  */
inline
AbstractSyntaxInternalNode::AbstractSyntaxInternalNode(
    const uint expression_id) noexcept :
        AbstractSyntaxNode(expression_id)
{
}

/*!
  */
inline
void AbstractSyntaxInternalNode::appendChildNode(
    const AbstractSyntaxNode* node) noexcept
{
  child_node_list_.push_back(node);
}

/*!
  */
inline
const AbstractSyntaxNode* AbstractSyntaxInternalNode::getChildNode(
    const uint index) const noexcept
{
  ZISC_ASSERT(index < numOfChildNodes(), "The index is out of range: ", index);
  return child_node_list_[index];
}

/*!
  */
inline
uint AbstractSyntaxInternalNode::numOfChildNodes() const noexcept
{
  return cast<uint>(child_node_list_.size());
}

/*!
  */
inline
AbstractSyntaxLeafNode::AbstractSyntaxLeafNode(const uint expression_id,
                                               const std::string& symbol) noexcept :
    AbstractSyntaxNode(expression_id),
    symbol_{symbol}
{
}

/*!
  */
inline
AbstractSyntaxLeafNode::AbstractSyntaxLeafNode(const uint expression_id,
                                               std::string&& symbol) noexcept :
    AbstractSyntaxNode(expression_id),
    symbol_{std::move(symbol)}
{
}

/*!
  */
inline
const std::string& AbstractSyntaxLeafNode::symbol() const noexcept
{
  return symbol_;
}

} // namespace zisc

#endif // ZISC_ABSTRACT_SYNTAX_NODE_INL_HPP
