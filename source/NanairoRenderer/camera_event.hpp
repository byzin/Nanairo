/*!
  \file camera_event.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CAMERA_EVENT_HPP
#define NANAIRO_CAMERA_EVENT_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

//! \addtogroup Renderer
//! \{

/*!
  \details
  No detailed.
  */
class CameraEvent
{
 public:
  //! Initialize the event
  CameraEvent() noexcept;


  //! Add event
  void addEvent(const int buttons, const int x, const int y, const int wheel) noexcept;

  //! Clear the event
  void clear() noexcept;

  //! Return the distance value
  const Vector2& distance() const noexcept;

  //! Check if event occured
  bool isEventOccured() const noexcept;

  //! Check if the distance event occured
  bool isDistanceEventOccured() const noexcept;

  //! Check if the rotation event occured
  bool isRotationEventOccured() const noexcept;

  //! Check if the translation event occured
  bool isTranslationEventOccured() const noexcept;

  //! Return the rotation value
  const Vector2& rotation() const noexcept;

  //! Return the translation
  const Vector2& translation() const noexcept;

 private:
  Vector2 translation_,
          distance_,
          rotation_;
};

//! \} Renderer
//
} // namespace nanairo

#include "camera_event-inl.hpp"

#endif // NANAIRO_CAMERA_EVENT_HPP
