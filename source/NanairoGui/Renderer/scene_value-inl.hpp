/*!
  \file scene_value-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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

/*!
  */
inline
QJsonArray SceneValue::toArray(const QJsonObject& object,
                               const QString& key) noexcept
{
  const auto v = getValue(object, key);
  ZISC_ASSERT(v.isArray(), "The value \"", key.toStdString(), "\" isn't array.");
  return toArray(v);
}

/*!
  */
inline
QJsonArray SceneValue::toArray(const QJsonValue& value) noexcept
{
  ZISC_ASSERT(value.isArray(), "The value isn't array.");
  return value.toArray();
}

/*!
  */
inline
bool SceneValue::toBool(const QJsonObject& object,
                        const QString& key) noexcept
{
  const auto v = getValue(object, key);
  ZISC_ASSERT(v.isBool(), "The value \"", key.toStdString(), "\" isn't boolean.");
  return toBool(v);
}

/*!
  */
inline
bool SceneValue::toBool(const QJsonValue& value) noexcept
{
  ZISC_ASSERT(value.isBool(), "The value isn't boolean.");
  return value.toBool();
}

/*!
  */
template <typename FloatingPoint> inline
FloatingPoint SceneValue::toFloat(const QJsonObject& object,
                                  const QString& key) noexcept
{
  static_assert(zisc::kIsFloat<FloatingPoint>, "FloatingPoint isn't float.");
  const auto v = getValue(object, key);
  ZISC_ASSERT(v.isDouble(), "The value \"", key.toStdString(), "\" isn't float.");
  return toFloat<FloatingPoint>(v);
}

/*!
  */
template <typename FloatingPoint> inline
FloatingPoint SceneValue::toFloat(const QJsonValue& value) noexcept
{
  static_assert(zisc::kIsFloat<FloatingPoint>, "FloatingPoint isn't float.");
  ZISC_ASSERT(value.isDouble(), "The value isn't float.");
  return zisc::cast<FloatingPoint>(value.toDouble());
}

/*!
  */
template <typename Integer> inline
Integer SceneValue::toInt(const QJsonObject& object,
                          const QString& key) noexcept
{
  static_assert(zisc::kIsInteger<Integer>, "Integer isn't integer.");
  const auto v = getValue(object, key);
  ZISC_ASSERT(v.isDouble(), "The value \"", key.toStdString(), "\" isn't integer.");
  return toInt<Integer>(v);
}

/*!
  */
template <typename Integer> inline
Integer SceneValue::toInt(const QJsonValue& value) noexcept
{
  static_assert(zisc::kIsInteger<Integer>, "Integer isn't integer.");
  ZISC_ASSERT(value.isDouble(), "The value isn't integer.");
  return zisc::cast<Integer>(std::lround(value.toDouble()));
}

/*!
  */
inline
QJsonObject SceneValue::toObject(const QJsonObject& object,
                                 const QString& key) noexcept
{
  const auto v = getValue(object, key);
  ZISC_ASSERT(v.isObject(), "The value \"", key.toStdString(), "\" isn't object.");
  return toObject(v);
}

/*!
  */
inline
QJsonObject SceneValue::toObject(const QJsonValue& value) noexcept
{
  ZISC_ASSERT(value.isObject(), "The value isn't object.");
  return value.toObject();
}

/*!
  */
inline
QString SceneValue::toString(const QJsonObject& object,
                             const QString& key) noexcept
{
  const auto v = getValue(object, key);
  ZISC_ASSERT(v.isString(), "The value \"", key.toStdString(), "\" isn't string.");
  return toString(v);
}

/*!
  */
inline
QString SceneValue::toString(const QJsonValue& value) noexcept
{
  ZISC_ASSERT(value.isString(), "The value isn't string.");
  return value.toString();
}

/*!
  */
inline
QJsonValue SceneValue::getValue(const QJsonObject& object,
                                const QString& key) noexcept
{
  ZISC_ASSERT(object.contains(key),
              "Object doesn't have the item: ", key.toStdString());
  return object[key];
}

} // namespace nanairo

#endif // NANAIRO_SCENE_VALUE_INL_HPP
