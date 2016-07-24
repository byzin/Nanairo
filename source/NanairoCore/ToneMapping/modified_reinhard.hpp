/*!
  \file modified_reinhard.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MODIFIED_REINHARD_HPP
#define NANAIRO_MODIFIED_REINHARD_HPP

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "tone_mapping_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/yxy_color.hpp"

// Forward declaration
class QImage;
class QJsonObject;

namespace nanairo {

// Forward declaration
class HdrImage;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class ModifiedReinhard : public ToneMappingMethod
{
 public:
  //! Initialize reinhard method
  ModifiedReinhard(const System& system, const QJsonObject& settings) noexcept;


  //! Apply tone mapping
  void toneMap(System& system,
               const HdrImage& hdr_image, 
               QImage& ldr_image) noexcept override;

 private:
  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;


  std::vector<YxyColor> pixel_luminance_;
};

//! \} Core 

} // namespace nanairo

#endif // NANAIRO_MODIFIED_REINHARD_HPP
