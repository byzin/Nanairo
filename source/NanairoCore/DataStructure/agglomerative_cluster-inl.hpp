/*!
  \file agglomerative_cluster-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_AGGLOMERATIVE_CLUSTER_INL_HPP
#define NANAIRO_AGGLOMERATIVE_CLUSTER_INL_HPP

#include "agglomerative_cluster.hpp"
// Standard C++ library
#include <array>
#include <limits>
#include <memory>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "aabb.hpp"
#include "bvh_node.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/object.hpp"
#include "NanairoCore/Shape/shape.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
AgglomerativeCluster::AgglomerativeCluster(const BvhNode* leaf_node) noexcept
{
  initializeLeafCluster(leaf_node);
}

/*!
  \details
  No detailed.
  */
inline
AgglomerativeCluster::AgglomerativeCluster(
    std::unique_ptr<AgglomerativeCluster>&& left_child_cluster,
    std::unique_ptr<AgglomerativeCluster>&& right_child_cluster) noexcept
{
  initializeInnerCluster(std::move(left_child_cluster), 
                         std::move(right_child_cluster));
}

/*!
  \details
  No detailed.
  */
inline
AgglomerativeCluster::~AgglomerativeCluster() noexcept
{
  if (isLeaf()) {
    auto& leaf = data_.leaf_;
    delete[] leaf.object_list_;
  }
  else {
    auto& inner = data_.inner_;
    delete inner.left_child_cluster_;
    delete inner.right_child_cluster_;
  }
}

/*!
  \details
  No detailed.
  */
inline
const Aabb& AgglomerativeCluster::boundingBox() const noexcept 
{
  return bounding_box_;
}

/*!
  \details
  No detailed.
  */
inline
Float AgglomerativeCluster::cost() const noexcept
{
  using zisc::cast;
  return cast<Float>(cost_);
}

/*!
  \details
  No detailed.
  */
inline
bool AgglomerativeCluster::isLeaf() const noexcept
{
  return (data_.leaf_.dummy_ == nullptr);
}

/*!
  \details
  No detailed.
  */
inline
const AgglomerativeCluster& AgglomerativeCluster::leftChildCluster() const noexcept
{
  ZISC_ASSERT(!isLeaf(), "This cluster is leaf.");
  return *(data_.inner_.left_child_cluster_);
}

/*!
  \details
  No detailed.
  */
inline
uint AgglomerativeCluster::numOfPrimitives() const noexcept
{
  using zisc::cast;
  return cast<uint>(num_of_objects_);
}

/*!
  \details
  No detailed.
  */
inline
auto AgglomerativeCluster::objectList() const noexcept -> 
    std::array<ObjectPointer, CoreConfig::maxNumOfNodeObjects()>
{
  ZISC_ASSERT(numOfPrimitives() <= CoreConfig::maxNumOfNodeObjects(), "Primitives overflow.");
  std::array<ObjectPointer, CoreConfig::maxNumOfNodeObjects()> object_list; 
  if (isLeaf()) {
    object_list.fill(nullptr);
    const auto& leaf = data_.leaf_;
    for (uint i = 0; i < numOfPrimitives(); ++i)
      object_list[i] = leaf.object_list_[i];
  }
  else {
    object_list = leftChildCluster().objectList();
    const auto object_list_r = rightChildCluster().objectList();
    const auto num_of_objects_l = leftChildCluster().numOfPrimitives();
    const auto num_of_objects_r = rightChildCluster().numOfPrimitives();
    for (uint i = 0; i < num_of_objects_r; ++i)
      object_list[i + num_of_objects_l] = object_list_r[i];
  }
  return object_list;
}

/*!
  \details
  No detailed.
  */
inline
const AgglomerativeCluster& AgglomerativeCluster::rightChildCluster() const noexcept
{
  ZISC_ASSERT(!isLeaf(), "This cluster is leaf.");
  return *(data_.inner_.right_child_cluster_);
}

/*!
  \details
  No detailed.
  */
inline
void AgglomerativeCluster::initializeLeafCluster(const BvhNode* leaf_node) noexcept
{
  using zisc::cast;

  setAsLeaf();
  auto& leaf = data_.leaf_;

  // Initialize the object list
  const auto& object_list = leaf_node->objectList();
  num_of_objects_ = cast<uint32>(leaf_node->numOfObjects());
  leaf.object_list_ = new ObjectPointer[numOfPrimitives()];
  for (uint i = 0; i < numOfPrimitives(); ++i)
    leaf.object_list_[i] = object_list[i];

  // Initialize the bounding box
  bounding_box_ = leaf_node->boundingBox();

  // Set the cost of the cluster
  Float cost = 0.0;
  for (uint i = 0; i < numOfPrimitives(); ++i) {
    const auto* object = leaf.object_list_[i];
    cost += object->shape().getTraversalCost();
  }
  cost_ = cast<float>(cost);
}

/*!
  \details
  No detailed.
  */
inline
void AgglomerativeCluster::initializeInnerCluster(
    std::unique_ptr<AgglomerativeCluster>&& left_child_cluster,
    std::unique_ptr<AgglomerativeCluster>&& right_child_cluster) noexcept
{
  using zisc::cast;

  // Initialize the object list
  const auto num_of_objects_l = left_child_cluster->numOfPrimitives();
  const auto num_of_objects_r = right_child_cluster->numOfPrimitives();
  num_of_objects_ = cast<uint32>(num_of_objects_l + num_of_objects_r);

  // Initialize the bounding box
  bounding_box_ = combine(left_child_cluster->boundingBox(),
                          right_child_cluster->boundingBox());

  // Set the cost of the cluster
  constexpr Float box_cost = rayBoxIntersectionCost();
  const Float surface_area = boundingBox().surfaceArea();
  const Float surface_area_l = left_child_cluster->boundingBox().surfaceArea();
  const Float surface_area_r = right_child_cluster->boundingBox().surfaceArea();
  Float cost = (surface_area_l * (box_cost + left_child_cluster->cost()) +
                surface_area_r * (box_cost + right_child_cluster->cost())) / 
                surface_area;

  // Check the flatting cost of this cluster
  Float flatting_cost = std::numeric_limits<Float>::max();
  if (numOfPrimitives() <= CoreConfig::maxNumOfNodeObjects()) {
    // The objects of the left child cluster
    const auto object_list_l = left_child_cluster->objectList();
    for (uint i = 0; i < num_of_objects_l; ++i) {
      const auto object = object_list_l[i];
      flatting_cost += object->shape().getTraversalCost();
    }
    // The objects of the right child cluster
    const auto object_list_r = right_child_cluster->objectList();
    for (uint i = 0; i < num_of_objects_r; ++i) {
      const auto object = object_list_r[i];
      flatting_cost += object->shape().getTraversalCost();
    }
    if (flatting_cost < cost) {
      auto& leaf = data_.leaf_;
      leaf.object_list_ = new ObjectPointer[numOfPrimitives()];
      for (uint i = 0; i < num_of_objects_l; ++i)
        leaf.object_list_[i] = object_list_l[i];
      for (uint i = 0; i < num_of_objects_r; ++i)
        leaf.object_list_[i + num_of_objects_l] = object_list_r[i];
    }
  }
  if (flatting_cost < cost) {
    setAsLeaf();
    cost = flatting_cost;
  }
  else {
    auto& inner = data_.inner_;
    inner.left_child_cluster_ = left_child_cluster.release();
    inner.right_child_cluster_ = right_child_cluster.release();
  }
}

/*!
  \details
  No detailed.
  */
inline
void AgglomerativeCluster::setAsLeaf() noexcept
{
  data_.leaf_.dummy_ = nullptr;
}

/*!
  \details
  No detailed.
  */
inline
Float getClusterDistance(const AgglomerativeCluster& a, 
                         const AgglomerativeCluster& b) noexcept
{
  return combine(a.boundingBox(), b.boundingBox()).surfaceArea();
}

} // namespace nanairo

#endif // NANAIRO_AGGLOMERATIVE_CLUSTER_INL_HPP
