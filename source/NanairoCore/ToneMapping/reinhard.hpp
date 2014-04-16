/*!
  \file reinhard.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_REINHARD_HPP_
#define _NANAIRO_REINHARD_HPP_

// Nanairo
#include "tone_mapping_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

// Forward declaration
class QImage;

namespace nanairo {

// Forward declaration
class HdrImage;
class SceneSettings;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Reinhard : public ToneMappingMethod
{
 public:
  //! Initialize reinhard method
  Reinhard(const System& system, const SceneSettings& settings);


  //! Apply tone mapping
  void toneMap(System& system,
               const HdrImage& hdr_image, 
               QImage& ldr_image) override;
};

//! \} Core 

} // namespace nanairo

#endif // _NANAIRO_REINHARD_HPP_
