/*!
  \file nanairo_gui_config.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_NANAIRO_GUI_CONFIG_HPP
#define NANAIRO_NANAIRO_GUI_CONFIG_HPP

// Standard C++ library
#include <string>

namespace nanairo {

//! \addtogroup Gui
//! \{

/*!
  */
class GuiConfig
{
 public:
  //! Return the GUI application name
  static std::string applicationName() noexcept;

  //! Get the default fixed font family
  static std::string getDefaultFixedFontFamily() noexcept;

  //! Get the default font family
  static std::string getDefaultFontFamily() noexcept;
};

//! \} Gui

} // namespace nanairo

#include "NanairoGui/nanairo_gui_config-inl.hpp"

#endif // NANAIRO_NANAIRO_GUI_CONFIG_HPP
