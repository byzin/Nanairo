/*!
  \file film.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FILM_HPP
#define NANAIRO_FILM_HPP

// Nanairo 
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectra_image_interface.hpp"
#include "NanairoCore/LinearAlgebra/point.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

// Forward declaration
class Sampler;
class SceneSettings;
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
  Film(const System& system, const SceneSettings& settings) noexcept;


  //! Return the aspect ratio of image
  Float aspectRatio() const noexcept;

  //! Sample film coordinate randomly
  Point2 coordinate(const uint x, const uint y, const Vector2& jittering) const noexcept;

  //! Return image height
  uint heightResolution() const noexcept;

  //! Return the inverse aspect ratio
  Float inverseAspectRatio() const noexcept;

  //! Return spectra pixel buffer
  SpectraImageInterface& spectraBuffer() noexcept;

  //! Return spectra pixel buffer
  const SpectraImageInterface& spectraBuffer() const noexcept;

  //! Return image width resolution
  uint widthResolution() const noexcept;

 private:
  //! Initialize
  void initialize(const System& system, const SceneSettings& settings) noexcept;


  UniquePointer<SpectraImageInterface> spectra_buffer_; //!< The buffer of spectra
  Float inverse_width_,
        inverse_height_;
};

//! \} Core 

} // namespace nanairo

#include "film-inl.hpp"

#endif // NANAIRO_FILM_HPP
