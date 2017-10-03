/*!
  \file spectra_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRA_SETTING_NODE_HPP
#define NANAIRO_SPECTRA_SETTING_NODE_HPP

// Standard C++ library
#include <array>
#include <istream>
#include <memory>
#include <ostream>
#include <tuple>
#include <vector>
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! RGB parameters
struct RgbParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  std::array<double, 3> rgb_{{1.0, 1.0, 1.0}};
};

//! Spectra parameters
struct SpectraParameters : public NodeParameterBase
{
  //! Initialize the spectra
  SpectraParameters() noexcept;

  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  std::vector<std::tuple<double, double>> spectra_;
};

/*!
  */
class SpectraSettingNode : public SettingNodeBase
{
 public:
  //! Initialize the setting
  void initialize() noexcept override;

  //! Read the setting from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Return the color representation type
  ColorRepresentationType representationType() const noexcept;

  //! Return the RGB parameters
  RgbParameters& rgbParameters() noexcept;

  //! Return the RGB parameters
  const RgbParameters& rgbParameters() const noexcept;

  //! Set the color type
  void setRepresentationType(const ColorRepresentationType type) noexcept;

  //! Return the spectra parameters
  SpectraParameters& spectraParameters() noexcept;

  //! Return the spectra parameters
  const SpectraParameters& spectraParameters() const noexcept;

  //! Return the node type
  SettingNodeType type() const noexcept override;

  //! Write the setting to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  std::unique_ptr<NodeParameterBase> parameters_;
  ColorRepresentationType color_type_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SPECTRA_SETTING_NODE_HPP
