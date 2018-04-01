/*!
  \file filmic.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FILMIC_HPP
#define NANAIRO_FILMIC_HPP

// Nanairo
#include "tone_mapping_operator.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Filmic : public ToneMappingOperator
{
 public:
  //! Initialize reinhard method
  Filmic(const System& system, const SettingNodeBase* settings) noexcept;


 private:
  //! Apply a filmic tonemap curve
  Float tonemap(const Float x) const noexcept override;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_FILMIC_HPP
