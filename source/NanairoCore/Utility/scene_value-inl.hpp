/*!
  \file scene_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SCENE_VALUE_INL_HPP
#define NANAIRO_SCENE_VALUE_INL_HPP

#include "scene_value.hpp"
// Standard C++ library
#include <cmath>
// Qt
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
// Zisc
#include "zisc/error.hpp"
#include "zisc/type_traits.hpp"
#include "zisc/utility.hpp"

namespace nanairo {

namespace inner {

//! Return a json value
QJsonValue value(const QJsonObject& object, const QString& key) noexcept;

/*!
  */
inline
QJsonValue value(const QJsonObject& object, const QString& key) noexcept
{
  ZISC_ASSERT(object.contains(key), 
              "Object doesn't have the item: ", key.toStdString());
  return object[key];
}

} // namespace inner

/*!
  */
inline
QJsonArray arrayValue(const QJsonObject& object, const QString& key) noexcept
{
  const auto value = inner::value(object, key);
  ZISC_ASSERT(value.isArray(), "The value \"", key.toStdString(), "\" isn't array.");
  return arrayValue(value);
}

/*!
  */
inline
QJsonArray arrayValue(const QJsonValue& value) noexcept
{
  ZISC_ASSERT(value.isArray(), "The value isn't array.");
  return value.toArray();
}


/*!
  */
inline
bool boolValue(const QJsonObject& object, const QString& key) noexcept
{
  const auto value = inner::value(object, key);
  ZISC_ASSERT(value.isBool(), "The value \"", key.toStdString(), "\" isn't boolean.");
  return boolValue(value);
}

/*!
  */
inline
bool boolValue(const QJsonValue& value) noexcept
{
  ZISC_ASSERT(value.isBool(), "The value isn't boolean.");
  return value.toBool();
}

/*!
  */
template <typename FloatingPoint> inline
FloatingPoint floatValue(const QJsonObject& object, const QString& key) noexcept
{
  static_assert(zisc::kIsFloat<FloatingPoint>, "FloatingPoint isn't float.");
  const auto value = inner::value(object, key);
  ZISC_ASSERT(value.isDouble(), "The value \"", key.toStdString(), "\" isn't float.");
  return floatValue<FloatingPoint>(value);
}

/*!
  */
template <typename FloatingPoint> inline
FloatingPoint floatValue(const QJsonValue& value) noexcept
{
  static_assert(zisc::kIsFloat<FloatingPoint>, "FloatingPoint isn't float.");
  ZISC_ASSERT(value.isDouble(), "The value isn't float.");
  return zisc::cast<FloatingPoint>(value.toDouble());
}

/*!
  */
template <typename Integer> inline
Integer intValue(const QJsonObject& object, const QString& key) noexcept
{
  static_assert(zisc::kIsInteger<Integer>, "Integer isn't integer.");
  const auto value = inner::value(object, key);
  ZISC_ASSERT(value.isDouble(), "The value \"", key.toStdString(), "\" isn't integer.");
  return intValue<Integer>(value);
}

/*!
  */
template <typename Integer> inline
Integer intValue(const QJsonValue& value) noexcept
{
  static_assert(zisc::kIsInteger<Integer>, "Integer isn't integer.");
  ZISC_ASSERT(value.isDouble(), "The value isn't integer.");
  return zisc::cast<Integer>(std::lround(value.toDouble()));
}

/*!
  */
inline
QJsonObject objectValue(const QJsonObject& object, const QString& key) noexcept
{
  const auto value = inner::value(object, key);
  ZISC_ASSERT(value.isObject(), "The value \"", key.toStdString(), "\" isn't object.");
  return objectValue(value);
}

/*!
  */
inline
QJsonObject objectValue(const QJsonValue& value) noexcept
{
  ZISC_ASSERT(value.isObject(), "The value isn't object.");
  return value.toObject();
}

/*!
  */
inline
QString stringValue(const QJsonObject& object, const QString& key) noexcept
{
  const auto value = inner::value(object, key);
  ZISC_ASSERT(value.isString(), "The value \"", key.toStdString(), "\" isn't string.");
  return stringValue(value);
}

/*!
  */
inline
QString stringValue(const QJsonValue& value) noexcept
{
  ZISC_ASSERT(value.isString(), "The value isn't string.");
  return value.toString();
}

} // namespace nanairo

#endif // NANAIRO_SCENE_VALUE_INL_HPP
