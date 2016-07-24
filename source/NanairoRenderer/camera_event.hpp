/*!
  \file camera_event.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
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
  void addEvent(const int transformation_event_type,
                const int axis_event_type,
                const int value) noexcept;

  //! Clear the event
  void clear() noexcept;

  //! Check if the event occured
  bool isEventOccured() const noexcept;

  //! Check if the horizontal translation event occured
  bool isHorizontalTranslationEventOccured() const noexcept;

  //! Check if the vertical translation event occured
  bool isVerticalTranslationEventOccured() const noexcept;

  //! Check if the rotation event occured
  bool isRotationEventOccured() const noexcept;

  //! Flush the horizontal translation event
  Vector2 flushHorizontalTranslationEvent() noexcept;

  //! Flush the rotation event 
  Vector2 flushRotationEvent() noexcept;

  //! Flush the vertical translation event
  Vector2 flushVerticalTranslationEvent() noexcept;

  //! Return the horizontal translation event
  Vector2& horizontalTranslationEvent() noexcept;

  //! Return the horizontal translation event
  const Vector2& horizontalTranslationEvent() const noexcept;

  //! Return the rotation event
  Vector2& rotationEvent() noexcept;

  //! Return the rotation event
  const Vector2& rotationEvent() const noexcept;

  //! Return the vertical translation event
  Vector2& verticalTranslationEvent() noexcept;

  //! Return the vertical translation event
  const Vector2& verticalTranslationEvent() const noexcept;

 private:
  //!
  template <int kTransformationType>
  Vector2 flushTransformationEvent() noexcept;


  static constexpr int kHorizontalTranslationEvent = 0;
  static constexpr int kVerticalTranslationEvent = 1;
  static constexpr int kRotationEvent = 2;
  static constexpr int kXAxis = 0;
  static constexpr int kYAxis = 1;
  static constexpr Float kHorizontalTranslationEventScale = 0.005;
  static constexpr Float kVerticalTranslationEventScale = 0.01;
  static constexpr Float kRotationEventScale = 0.005;


  Vector2 event_list_[3];
};

//! \} Renderer

} // namespace nanairo

#include "camera_event-inl.hpp"

#endif // NANAIRO_CAMERA_EVENT_HPP
