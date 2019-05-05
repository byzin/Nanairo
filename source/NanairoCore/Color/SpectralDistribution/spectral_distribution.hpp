/*!
  \file spectral_distribution.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRAL_DISTRIBUTION_HPP
#define NANAIRO_SPECTRAL_DISTRIBUTION_HPP

// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/xyz_color.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class System;

//! \addtogroup Core
//! \{

/*!
  */
class SpectralDistribution
{
 public:
  using RepresentationType = RenderingColorMode;
  using SpectralDistributionPointer = zisc::UniqueMemoryPointer<SpectralDistribution>;


  //! Finalize a spectral distribution
  virtual ~SpectralDistribution() noexcept;


  //! Return the point of the distribution by the index
  Float operator[](const uint index) const noexcept;


  //! Add a intensity by an index
  virtual void add(const uint index, const Float intensity) noexcept = 0;

  //! Clamp all elements to the range [min, max]
  void clampAll(const Float minimum, const Float maximum) noexcept;

  //! Apply gamma correction
  void correctGamma(const Float gamma) noexcept;

  //! Fill all elements by the value
  void fill(const Float value) noexcept;

  //! Return the element by the index
  virtual Float get(const uint index) const noexcept = 0;

  //! Return the element by the wavelength
  Float getByWavelength(const uint16 wavelength) const noexcept;

  //! Get the index correspond to the given wavelength
  virtual uint getIndex(const uint16 wavelength) const noexcept = 0;

  //! Get the wavelength correspond to the given index
  virtual uint16 getWavelength(const uint index) const noexcept = 0;

  //! Check if the distribution has inf value
  bool hasInf() const noexcept;

  //! Check if the distribution has nan value
  bool hasNan() const noexcept;

  //! Check if the distribution has negative value
  bool hasNegative() const noexcept;

  //! Check if the distribution has the specified value
  bool hasValue(const Float value) const noexcept;

  //! Check if all components are betwenn [ \p lower , \p upper )
  bool isAllInBounds(const Float lower, const Float upper) const noexcept;

  //! Check if all components are betwenn [ \p lower , \p upper ]
  bool isAllInClosedBounds(const Float lower, const Float upper) const noexcept;

  //! Check if all components are zero
  bool isAllZero() const noexcept;

  //! Make a distribution initialized by 0
  static SpectralDistributionPointer makeDistribution(
      const RepresentationType type,
      zisc::pmr::memory_resource* mem_resource,
      const bool is_compensated = false) noexcept;

  //! Make a spectral distribution
  static SpectralDistributionPointer makeDistribution(
      const System& system,
      const SettingNodeBase* settings,
      zisc::pmr::memory_resource* mem_resource,
      const bool is_compensated = false) noexcept;

  //! Return the largest element
  Float max() const noexcept;

  //! Return the smallest element
  Float min() const noexcept;

  //! Make a spectra
  virtual void load(const System& system,
                    const SettingNodeBase* settings) noexcept = 0;

  //! Normalize a distribution
  void normalize() noexcept;

  //! Return the representation type of a distribution
  virtual RepresentationType representationType() const noexcept = 0;

  //! Set value by the index
  virtual void set(const uint index, const Float intensity) noexcept = 0;

  //! Set value by the wavelength
  void setByWavelength(const uint16 wavelength, const Float intensity) noexcept;

  //! Convert the given distribution as color and set to this
  virtual void setColor(const System& system,
                        const SpectralDistribution& other,
                        zisc::pmr::memory_resource* work_resource) noexcept = 0;

  //! Convert the given distribution and set to this
  virtual void setData(const SpectralDistribution& other) noexcept = 0;

  //! Return the size of a distribution
  virtual uint size() const noexcept = 0;

  //! Return the sum of intensities
  Float sum() const noexcept;

  //! Return the sum of intensities with compensation
  Float compensatedSum() const noexcept;

  //! Return the xyz color converted from the spectra to
  virtual XyzColor toXyzForEmitter(const System& system) const noexcept = 0;

  //! Return the xyz color converted from the spectra to
  virtual XyzColor toXyzForReflector(const System& system) const noexcept = 0;
};

//! \} Core

} // namespace nanairo

#include "spectral_distribution-inl.hpp"

#endif // NANAIRO_SPECTRAL_DISTRIBUTION_HPP
