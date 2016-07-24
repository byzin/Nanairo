/*!
  \file scene_value.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */  

#ifndef NANAIRO_SCENE_VALUE_HPP
#define NANAIRO_SCENE_VALUE_HPP

// Qt
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

// Forward declaration
class QString;

namespace nanairo {

//! \addtogroup Core
//! \{

//! Return a array value
QJsonArray arrayValue(const QJsonObject& object, const QString& key) noexcept;

//! Return a array value
QJsonArray arrayValue(const QJsonValue& value) noexcept;

//! Return a boolean value
bool boolValue(const QJsonObject& object, const QString& key) noexcept;

//! Return a boolean value
bool boolValue(const QJsonValue& value) noexcept;

//! Return a float value
template <typename FloatingPoint>
FloatingPoint floatValue(const QJsonObject& object, const QString& key) noexcept;

//! Return a float value
template <typename FloatingPoint>
FloatingPoint floatValue(const QJsonValue& value) noexcept;

//! Return a integer value
template <typename Integer>
Integer intValue(const QJsonObject& object, const QString& key) noexcept;

//! Return a integer value
template <typename Integer>
Integer intValue(const QJsonValue& value) noexcept;

//! Return a object value
QJsonObject objectValue(const QJsonObject& object, const QString& key) noexcept;

//! Return a object value
QJsonObject objectValue(const QJsonValue& value) noexcept;

//! Return a string value
QString stringValue(const QJsonObject& object, const QString& key) noexcept;

//! Return a string value
QString stringValue(const QJsonValue& value) noexcept;

//! \}

} // namespace nanairo

#include "scene_value-inl.hpp"

#endif // NANAIRO_SCENE_VALUE_HPP
