/*!
  \file camera_event-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CAMERA_EVENT_INL_HPP
#define NANAIRO_CAMERA_EVENT_INL_HPP

#include "camera_event.hpp"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
CameraEvent::CameraEvent() noexcept
{
  clear();
}

/*!
  */
inline
void CameraEvent::addEvent(const int transformation_event_type,
                           const int axis_event_type,
                           const int value) noexcept
{
  auto& event = event_list_[transformation_event_type];
  const auto scale = 
      (transformation_event_type == kHorizontalTranslationEvent)
          ? kHorizontalTranslationEventScale
    : (transformation_event_type == kVerticalTranslationEvent)
          ? kVerticalTranslationEventScale
          : kRotationEventScale;
  const Float v = scale * zisc::cast<Float>(value);
  event[axis_event_type] += v;
}

/*!
  \details
  No detailed.
  */
inline
void CameraEvent::clear() noexcept
{
  for (auto& event : event_list_)
    event = Vector2{0.0, 0.0};
}

/*!
  */
inline
Vector2 CameraEvent::flushHorizontalTranslationEvent() noexcept
{
  return flushTransformationEvent<kHorizontalTranslationEvent>();
}

/*!
  */
inline
Vector2 CameraEvent::flushRotationEvent() noexcept
{
  return flushTransformationEvent<kRotationEvent>();
}

/*!
  */
inline
Vector2 CameraEvent::flushVerticalTranslationEvent() noexcept
{
  return flushTransformationEvent<kVerticalTranslationEvent>();
}

/*!
  */
inline
bool CameraEvent::isEventOccured() const noexcept
{
  return isHorizontalTranslationEventOccured() ||
         isRotationEventOccured() ||
         isVerticalTranslationEventOccured();
}

/*!
  \details
  No detailed.
  */
inline
bool CameraEvent::isHorizontalTranslationEventOccured() const noexcept
{
  const auto& horizontal_translation = horizontalTranslationEvent();
  return (horizontal_translation[0] != 0.0) || (horizontal_translation[1] != 0.0);
}

/*!
  \details
  No detailed.
  */
inline
bool CameraEvent::isRotationEventOccured() const noexcept
{
  const auto& rotation = rotationEvent();
  return (rotation[0] != 0.0) || (rotation[1] != 0.0);
}

/*!
  \details
  No detailed.
  */
inline
bool CameraEvent::isVerticalTranslationEventOccured() const noexcept
{
  const auto& vertical_translation = verticalTranslationEvent();
  return (vertical_translation[0] != 0.0) || (vertical_translation[1] != 0.0);
}

/*!
  \details
  No detailed.
  */
inline
Vector2& CameraEvent::horizontalTranslationEvent() noexcept
{
  return event_list_[kHorizontalTranslationEvent];
}

/*!
  \details
  No detailed.
  */
inline
const Vector2& CameraEvent::horizontalTranslationEvent() const noexcept
{
  return event_list_[kHorizontalTranslationEvent];
}

/*!
  \details
  No detailed.
  */
inline
Vector2& CameraEvent::rotationEvent() noexcept
{
  return event_list_[kRotationEvent];
}

/*!
  \details
  No detailed.
  */
inline
const Vector2& CameraEvent::rotationEvent() const noexcept
{
  return event_list_[kRotationEvent];
}

/*!
  \details
  No detailed.
  */
inline
Vector2& CameraEvent::verticalTranslationEvent() noexcept
{
  return event_list_[kVerticalTranslationEvent];
}

/*!
  \details
  No detailed.
  */
inline
const Vector2& CameraEvent::verticalTranslationEvent() const noexcept
{
  return event_list_[kVerticalTranslationEvent];
}

/*!
  */
template <int kTransformationType> inline
Vector2 CameraEvent::flushTransformationEvent() noexcept
{
  auto& transformation = event_list_[kTransformationType];
  const auto event = transformation;
  transformation[0] = 0.0;
  transformation[1] = 0.0;
  return event;
}

} // namespace nanairo

#endif // _NANAIRO_CAMERA_EVENT_INL_HPP_
