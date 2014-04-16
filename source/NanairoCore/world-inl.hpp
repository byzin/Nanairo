/*!
  \file world-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_WORLD_INL_HPP_
#define _NANAIRO_WORLD_INL_HPP_

#include "world.hpp"
// Standard C++ library
#include <vector>
// Nanairo
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"
#include "NanairoCore/Sampling/light_source_sampler.hpp"

namespace nanairo {

/*!
  \details
  No detailed.

  \return BVH
  */
inline
const Bvh& World::bvh() const
{
  return *bvh_;
}

/*!
  \details
  No detailed.
  */
inline
const std::vector<LightSourceReference>& World::lightSourceList() const
{
  return light_source_list_;
}

/*!
  \details
  No detailed.
  */
inline
const LightSourceSampler& World::lightSourceSampler() const
{
  return *light_source_sampler_;
}

///*!
//  \details
//  No detailed.
//  */
//inline
//const std::vector<Object>& World::objectList() const
//{
//  return object_list_;
//}

/*!
  \details
  No detailed.
  */
inline
const LightSourceReference& World::sampleLightSource(Sampler& sampler) const
{
  return light_source_sampler_->sample(sampler);
}

} // namespace nanairo

#endif // _NANAIRO_WORLD_INL_HPP_
