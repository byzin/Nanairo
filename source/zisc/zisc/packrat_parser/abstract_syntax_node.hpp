/*!
  \file abstract_syntax_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_ABSTRACT_SYNTAX_NODE_HPP
#define ZISC_ABSTRACT_SYNTAX_NODE_HPP

// Standard C++ library
#include <string>
#include <vector>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

// Forward declaration
class AbstractSyntaxInternalNode;
class AbstractSyntaxLeafNode;

/*!
  */
class AbstractSyntaxNode
{
 public:
  //!
  virtual ~AbstractSyntaxNode();
  

  //! Return the expression ID
  uint expressionId() const noexcept;

  //! Check if this node is internal node
  virtual bool isInternal() const noexcept;

  //! Check if this node is leaf node
  virtual bool isLeaf() const noexcept;

  //! Return this pointer as internal node class
  AbstractSyntaxInternalNode* treatAsInternalNode() noexcept;

  //! Return this pointer as internal node class
  const AbstractSyntaxInternalNode* treatAsInternalNode() const noexcept;

  //! Return this pointer as leaf node class
  AbstractSyntaxLeafNode* treatAsLeafNode() noexcept;

  //! Return this pointer as leaf node class
  const AbstractSyntaxLeafNode* treatAsLeafNode() const noexcept;

 protected:
  //!
  AbstractSyntaxNode(const uint expression_id) noexcept;

 private:
  uint expression_id_;
};

/*!
  */
class AbstractSyntaxInternalNode : public AbstractSyntaxNode
{
 public:
  //!
  AbstractSyntaxInternalNode(const uint expression_id) noexcept;


  //! Add a syntax node
  void appendChildNode(const AbstractSyntaxNode* node) noexcept;

  //! Return the child node by the index
  const AbstractSyntaxNode* getChildNode(const uint index) const noexcept;

  //! Check if this node is internal node
  bool isInternal() const noexcept override;

  //! Return the number of child nodes
  uint numOfChildNodes() const noexcept;

 private:
  std::vector<const AbstractSyntaxNode*> child_node_list_;
};

/*!
  */
class AbstractSyntaxLeafNode : public AbstractSyntaxNode
{
 public:
  //!
  AbstractSyntaxLeafNode(const uint expression_id, 
                         const std::string& symbol) noexcept;

  //!
  AbstractSyntaxLeafNode(const uint expression_id, 
                         std::string&& symbol) noexcept;


  //! Check if this node is leaf node
  bool isLeaf() const noexcept override;

  //!
  const std::string& symbol() const noexcept;

 private:
  std::string symbol_;
};

} // namespace zisc

#include "abstract_syntax_node-inl.hpp"

#endif // ZISC_ABSTRACT_SYNTAX_NODE_HPP
