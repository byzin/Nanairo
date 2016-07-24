/*!
  \file reinhard.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_REINHARD_HPP
#define NANAIRO_REINHARD_HPP

// Nanairo
#include "tone_mapping_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

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
class Reinhard : public ToneMappingMethod
{
 public:
  //! Initialize reinhard method
  Reinhard(const System& system, const QJsonObject& settings) noexcept;


  //! Apply tone mapping
  void toneMap(System& system,
               const HdrImage& hdr_image, 
               QImage& ldr_image) noexcept override;
};

//! \} Core 

} // namespace nanairo

#endif // NANAIRO_REINHARD_HPP
