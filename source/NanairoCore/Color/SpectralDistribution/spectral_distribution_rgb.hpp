/*!
  \file spectral_distribution_rgb.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRAL_DISTRIBUTION_RGB_HPP
#define NANAIRO_SPECTRAL_DISTRIBUTION_RGB_HPP

// Standard C++ library
#include <array>
#include <type_traits>
// Zisc
#include "zisc/compensated_summation.hpp"
// Nanairo
#include "spectral_distribution.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/xyz_color.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class System;

//! \addtogroup Core
//! \{

/*!
  */
template <bool kCompensated>
class SpectralDistributionRgb : public SpectralDistribution
{
 public:
  using DataType = std::conditional_t<kCompensated,
                                      zisc::CompensatedSummation<Float>,
                                      Float>;
  using DistributionType = std::array<DataType, 3>;


  //! Initialize a distribution with 0
  SpectralDistributionRgb() noexcept;

  //! Create a distribution
  SpectralDistributionRgb(const DistributionType& property) noexcept;


  //! Add a intensity by an index
  void add(const uint index, const Float intensity) noexcept override;

  //! Return the element by the index
  Float get(const uint index) const noexcept override;

  //! Get the index correspond to the given wavelength
  uint getIndex(const uint16 wavelength) const noexcept override;

  //! Get the wavelength correspond to the given index
  uint16 getWavelength(const uint index) const noexcept override;

  //! Load a distribution from the given settings
  void load(const System& system,
            const SettingNodeBase* settings) noexcept override;

  //! Return the representation type of spectral data
  RepresentationType representationType() const noexcept override;

  //! Set value by the index
  void set(const uint index, const Float intensity) noexcept override;

  //! Convert the given distritbuion as color and set to this
  void setColor(const System& system,
                const SpectralDistribution& other,
                zisc::pmr::memory_resource* work_resource) noexcept override;

  //! Convert the given distritbuion and set to this
  void setData(const SpectralDistribution& other) noexcept override;

  //! Return the size of a distribution
  uint size() const noexcept override;

  //! Return the xyz color converted from the spectra
  XyzColor toXyzForEmitter(const System& system) const noexcept override;

  //! Return the xyz color converted from the spectra
  XyzColor toXyzForReflector(const System& system) const noexcept override;

 private:
  //!
  void setAsRgbColor(const System& system,
                     const SpectralDistribution& other) noexcept;

  //!
  void setAsRgbData(const SpectralDistribution& other) noexcept;


  DistributionType distribution_;
};

// Type aliases
using RgbDistribution = SpectralDistributionRgb<false>;
using CompensatedRgbDistribution = SpectralDistributionRgb<true>;

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SPECTRAL_DISTRIBUTION_RGB_HPP
