/*!
  \file filmic.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_FILMIC_HPP_
#define _NANAIRO_FILMIC_HPP_

// Standard C++ library
#include <cstddef>
#include <vector>
// Nanairo
#include "tone_mapping_method.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/yxy_color.hpp"

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
class Filmic : public ToneMappingMethod
{
 public:
  //! Initialize reinhard method
  Filmic(const System& system, const SceneSettings& settings);


  //! Apply tone mapping
  void toneMap(System& system,
               const HdrImage& hdr_image, 
               QImage& ldr_image) override;

 private:
  //! Initialize
  void initialize(const System& system, const SceneSettings& settings);

  //! Apply the filmic tonemap
  Float filmicToneMap(Float x) const;


  std::vector<YxyColor> pixel_luminance_;
};

//! \} Core 

} // namespace nanairo

#endif // _NANAIRO_FILMIC_HPP_
