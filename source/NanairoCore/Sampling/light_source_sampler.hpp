/*!
  \file light_source_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_LIGHT_SOURCE_SAMPLER_HPP
#define NANAIRO_LIGHT_SOURCE_SAMPLER_HPP

// Standard C++ library
#include <vector>
// Zisc
#include "zisc/cumulative_distribution_function.hpp"
// Nanairo
#include "NanairoCore/Data/light_source_reference.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class Sampler;

//! \addtogroup Core 
//! \{

/*!
  \details
  No detailed.
  */
class LightSourceSampler
{
 public:
  //! Create a light source sampler
  LightSourceSampler(const std::vector<LightSourceReference>& light_source_list) noexcept;


  //! Sample a light source
  const LightSourceReference& sample(Sampler& sampler) const noexcept;

  //! Return the pdf
  Float getPdf(const Object* light_source) const noexcept;

 private:
  using LightSourceCdf = 
      zisc::CumulativeDistributionFunction<const LightSourceReference*, Float>;


  //! Initialize
  void initialize(const std::vector<LightSourceReference>& light_source_list) noexcept;


  UniquePointer<LightSourceCdf> cdf_;
  Float inverse_total_flux_;
};

//! \} Core 

} // namespace nanairo

#include "light_source_sampler-inl.hpp"

#endif // NANAIRO_LIGHT_SOURCE_SAMPLER_HPP
