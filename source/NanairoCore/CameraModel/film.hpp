/*!
  \file film.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FILM_HPP
#define NANAIRO_FILM_HPP

// Nanairo 
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Sampling/sample_statistics.hpp"
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
class Film
{
 public:
  //! Create film
  Film(System& system, const SettingNodeBase* settings) noexcept;


  //! Return the aspect ratio of image
  Float aspectRatio() const noexcept;

  //! Return a film coordinate correspond to the given index
  Point2 coordinate(const Index2d& index, const Vector2& jittering) const noexcept;

  //! Clear film buffers
  void clear() noexcept;

  //! Return the image height
  uint heightResolution() const noexcept;

  //! Return the image resolution
  Index2d imageResolution() const noexcept;

  //! Return the sample statistics
  SampleStatistics& sampleStatistics() noexcept;

  //! Return the sample statistics
  const SampleStatistics& sampleStatistics() const noexcept;

  //! Return the image width
  uint widthResolution() const noexcept;

 private:
  //! Initialize
  void initialize(System& system, const SettingNodeBase* settings) noexcept;


  SampleStatistics sample_statistics_;
};

//! \} Core 

} // namespace nanairo

#include "film-inl.hpp"

#endif // NANAIRO_FILM_HPP
