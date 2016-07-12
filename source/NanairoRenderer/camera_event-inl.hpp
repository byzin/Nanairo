/*!
  \file camera_event-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CAMERA_EVENT_INL_HPP
#define NANAIRO_CAMERA_EVENT_INL_HPP

#include "camera_event.hpp"
// Qt
#include <Qt>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
CameraEvent::CameraEvent() noexcept
{
}

/*!
  \details
  No detailed.
  */
inline
void CameraEvent::addEvent(const int buttons, 
                           const int x, 
                           const int y, 
                           const int /* wheel */) noexcept
{
  using zisc::cast;
  constexpr Float tk = 0.005;
  constexpr Float dk = 0.01;
  constexpr Float rk = 0.0005;

  switch (buttons) {
   case Qt::LeftButton:
    rotation_[0] += rk * cast<Float>(x);
    rotation_[1] += rk * cast<Float>(y);
    break;
   case Qt::RightButton:
    distance_[0] += dk * cast<Float>(x);
    distance_[1] += dk * cast<Float>(y);
    break;
   case Qt::MiddleButton:
    translation_[0] += tk * cast<Float>(x);
    translation_[1] += tk * cast<Float>(y);
    break;
   default:
    break;
  }
}

/*!
  \details
  No detailed.
  */
inline
void CameraEvent::clear() noexcept
{
  translation_[0] = 0.0;
  translation_[1] = 0.0;
  distance_[0] = 0.0;
  distance_[1] = 0.0;
  rotation_[0] = 0.0;
  rotation_[1] = 0.0;
}

/*!
  \details
  No detailed.
  */
inline
const Vector2& CameraEvent::distance() const noexcept
{
  return distance_;
}

/*!
  \details
  No detailed.
  */
inline
bool CameraEvent::isEventOccured() const noexcept
{
  return isTranslationEventOccured() ||
         isDistanceEventOccured() ||
         isRotationEventOccured();
}

/*!
  \details
  No detailed.
  */
inline
bool CameraEvent::isDistanceEventOccured() const noexcept
{
  return !((distance_[0] == 0.0) && (distance_[1] == 0.0));
}

/*!
  \details
  No detailed.
  */
inline
bool CameraEvent::isRotationEventOccured() const noexcept
{
  return !((rotation_[0] == 0.0) && (rotation_[1] == 0.0));
}

/*!
  \details
  No detailed.
  */
inline
bool CameraEvent::isTranslationEventOccured() const noexcept
{
  return !((translation_[0] == 0.0) && (translation_[1] == 0.0));
}

/*!
  \details
  No detailed.
  */
inline
const Vector2& CameraEvent::rotation() const noexcept
{
  return rotation_;
}

/*!
  \details
  No detailed.
  */
inline
const Vector2& CameraEvent::translation() const noexcept
{
  return translation_;
}

} // namespace nanairo

#endif // _NANAIRO_CAMERA_EVENT_INL_HPP_
