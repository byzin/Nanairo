/*!
  \file uncharted2_filmic.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNCHARTED2_FILMIC_HPP
#define NANAIRO_UNCHARTED2_FILMIC_HPP

// Standard C++ library
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
class Uncharted2Filmic : public ToneMappingMethod
{
 public:
  //! Initialize reinhard method
  Uncharted2Filmic(const System& system, const QJsonObject& settings) noexcept;


  //! Apply tone mapping
  void toneMap(System& system,
               const HdrImage& hdr_image, 
               QImage& ldr_image) noexcept override;

 private:
  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;

  //! Apply uncharted2 tone map
  Float uncharted2ToneMap(const Float x) const noexcept;


  std::vector<YxyColor> pixel_luminance_;
};

//! \} Core 

} // namespace nanairo

#endif // NANAIRO_UNCHARTED2_FILMIC_HPP
