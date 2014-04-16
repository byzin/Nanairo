/*!
  \file camera_event.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_CAMERA_EVENT_HPP_
#define _NANAIRO_CAMERA_EVENT_HPP_

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
  CameraEvent();


  //! Add event
  void addEvent(const int buttons, const int x, const int y, const int wheel);

  //! Clear the event
  void clear();

  //! Return the distance value
  const Vector2& distance() const;

  //! Check if event occured
  bool isEventOccured() const;

  //! Check if the distance event occured
  bool isDistanceEventOccured() const;

  //! Check if the rotation event occured
  bool isRotationEventOccured() const;

  //! Check if the translation event occured
  bool isTranslationEventOccured() const;

  //! Return the rotation value
  const Vector2& rotation() const;

  //! Return the translation
  const Vector2& translation() const;

 private:
  Vector2 translation_,
          distance_,
          rotation_;
};

//! \} Renderer
//
} // namespace nanairo

#include "camera_event-inl.hpp"

#endif // _NANAIRO_CAMERA_EVENT_HPP_
