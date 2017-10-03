/*!
  \file bvh_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "bvh_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "setting_node_base.hpp"

namespace nanairo {

/*!
  */
void ApproximateAgglomerativeClusteringParameters::readData(
    std::istream* data_stream) noexcept
{
  zisc::read(&epsilon_, data_stream);
  zisc::read(&delta_, data_stream);
}

/*!
  */
void ApproximateAgglomerativeClusteringParameters::writeData(
    std::ostream* data_stream) const noexcept
{
  zisc::write(&epsilon_, data_stream);
  zisc::write(&delta_, data_stream);
}

/*!
  */
void AgglomerativeTreeletRestructuringParameters::readData(
    std::istream* data_stream) noexcept
{
  zisc::read(&treelet_size_, data_stream);
  zisc::read(&optimization_loop_, data_stream);
}

/*!
  */
void AgglomerativeTreeletRestructuringParameters::writeData(
    std::ostream* data_stream) const noexcept
{
  zisc::write(&treelet_size_, data_stream);
  zisc::write(&optimization_loop_, data_stream);
}

/*!
  */
ApproximateAgglomerativeClusteringParameters&
BvhSettingNode::approximateAgglomerativeClusteringParameters() noexcept
{
  ZISC_ASSERT(bvhType() == BvhType::kApproximateAgglomerativeClustering,
              "Invalid BVH type is specified.");
  auto parameter = zisc::cast<ApproximateAgglomerativeClusteringParameters*>(
      parameters_.get());
  return *parameter;
}

/*!
  */
const ApproximateAgglomerativeClusteringParameters&
BvhSettingNode::approximateAgglomerativeClusteringParameters() const noexcept
{
  ZISC_ASSERT(bvhType() == BvhType::kApproximateAgglomerativeClustering,
              "Invalid BVH type is specified.");
  auto parameter = zisc::cast<const ApproximateAgglomerativeClusteringParameters*>(
      parameters_.get());
  return *parameter;
}

/*!
  */
AgglomerativeTreeletRestructuringParameters&
BvhSettingNode::agglomerativeTreeletRestructuringParameters() noexcept
{
  ZISC_ASSERT(bvhType() == BvhType::kAgglomerativeTreeletRestructuring,
              "Invalid BVH type is specified.");
  auto parameter = zisc::cast<AgglomerativeTreeletRestructuringParameters*>(
      parameters_.get());
  return *parameter;
}

/*!
  */
const AgglomerativeTreeletRestructuringParameters&
BvhSettingNode::agglomerativeTreeletRestructuringParameters() const noexcept
{
  ZISC_ASSERT(bvhType() == BvhType::kAgglomerativeTreeletRestructuring,
              "Invalid BVH type is specified.");
  auto parameter = zisc::cast<const AgglomerativeTreeletRestructuringParameters*>(
      parameters_.get());
  return *parameter;
}

/*!
  */
BvhType BvhSettingNode::bvhType() const noexcept
{
  return bvh_type_;
}

/*!
  */
void BvhSettingNode::initialize() noexcept
{
  setBvhType(BvhType::kBinaryRadixTree);
}

/*!
  */
void BvhSettingNode::readData(std::istream* data_stream) noexcept
{
  {
    zisc::read(&bvh_type_, data_stream);
    setBvhType(bvh_type_);
  }
  if (parameters_)
    parameters_->readData(data_stream);
}

/*!
  */
void BvhSettingNode::setBvhType(const BvhType type) noexcept
{
  bvh_type_ = type;
  // Initialize parameters
  parameters_.reset(nullptr);
  switch (bvh_type_) {
   case BvhType::kApproximateAgglomerativeClustering: {
    parameters_ = std::make_unique<ApproximateAgglomerativeClusteringParameters>();
    break;
   }
   case BvhType::kAgglomerativeTreeletRestructuring: {
    parameters_ = std::make_unique<AgglomerativeTreeletRestructuringParameters>();
    break;
   }
   case BvhType::kBinaryRadixTree:
   default:
    break;
  }
}

/*!
  */
SettingNodeType BvhSettingNode::type() const noexcept
{
  return SettingNodeType::kBvh;
}

/*!
  */
void BvhSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);
  // Write properties
  zisc::write(&bvh_type_, data_stream);
  if (parameters_)
    parameters_->writeData(data_stream);
}

} // namespace nanairo
