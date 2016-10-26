/*!
  \file nanairo_common_config.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_NANAIRO_COMMON_CONFIG_HPP
#define NANAIRO_NANAIRO_COMMON_CONFIG_HPP

// Standard C++ library
#include <string>

namespace nanairo {

//! \addtogroup Common
//! \{

/*!
  \brief This class contains information used in common by other modules
  */
class CommonConfig
{
 public:
  //! Return the application name
  static std::string applicationName() noexcept;

  //! Return the version string of the application
  static std::string versionString() noexcept;

  //! Return the major number of the version
  static constexpr int versionMajor() noexcept;

  //! Return the minor number of the version
  static constexpr int versionMinor() noexcept;

  //! Return the minor number of the version
  static constexpr int versionPatch() noexcept;
};

//! \} Common

} // namespace nanairo

#include "NanairoCommon/nanairo_common_config-inl.hpp"

#endif // NANAIRO_NANAIRO_COMMON_CONFIG_HPP
