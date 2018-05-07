/*!
  \file film.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FILM_HPP
#define NANAIRO_FILM_HPP

// Zisc
#include "zisc/point.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo 
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectra_image_interface.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class Sampler;
class System;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class Film
{
 public:
  //! Create film
  Film(System& system, const SettingNodeBase* settings) noexcept;


  //! Return the aspect ratio of image
  Float aspectRatio() const noexcept;

  //! Sample film coordinate randomly
  Point2 coordinate(const Index2d& index, const Vector2& jittering) const noexcept;

  //! Clear film buffers
  void clear() noexcept;

  //! Return image height
  uint heightResolution() const noexcept;

  //! Return the inverse aspect ratio
  Float inverseAspectRatio() const noexcept;

  //! Return the image resolution
  const Index2d& imageResolution() const noexcept;

  //! Return spectra pixel buffer
  SpectraImageInterface& spectraBuffer() noexcept;

  //! Return spectra pixel buffer
  const SpectraImageInterface& spectraBuffer() const noexcept;

  //! Return image width resolution
  uint widthResolution() const noexcept;

 private:
  //! Initialize
  void initialize(System& system, const SettingNodeBase* settings) noexcept;

  //! Return the inverse resolution
  const Point2& inverseResolution() const noexcept;


  zisc::UniqueMemoryPointer<SpectraImageInterface> spectra_buffer_; //!< The buffer of spectra
  Point2 inverse_resolution_;
};

//! \} Core 

} // namespace nanairo

#include "film-inl.hpp"

#endif // NANAIRO_FILM_HPP
