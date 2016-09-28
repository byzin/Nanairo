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

/*!
  */
class SceneValue
{
 public:
  //! Return a array value
  static QJsonArray toArray(const QJsonObject& object,
                            const QString& key) noexcept;

  //! Return a array value
  static QJsonArray toArray(const QJsonValue& value) noexcept;

  //! Return a boolean value
  static bool toBool(const QJsonObject& object,
                     const QString& key) noexcept;

  //! Return a boolean value
  static bool toBool(const QJsonValue& value) noexcept;

  //! Return a float value
  template <typename FloatingPoint>
  static FloatingPoint toFloat(const QJsonObject& object,
                               const QString& key) noexcept;

  //! Return a float value
  template <typename FloatingPoint>
  static FloatingPoint toFloat(const QJsonValue& value) noexcept;

  //! Return a integer value
  template <typename Integer>
  static Integer toInt(const QJsonObject& object,
                       const QString& key) noexcept;

  //! Return a integer value
  template <typename Integer>
  static Integer toInt(const QJsonValue& value) noexcept;

  //! Return a object value
  static QJsonObject toObject(const QJsonObject& object,
                              const QString& key) noexcept;

  //! Return a object value
  static QJsonObject toObject(const QJsonValue& value) noexcept;

  //! Return a string value
  static QString toString(const QJsonObject& object,
                          const QString& key) noexcept;

  //! Return a string value
  static QString toString(const QJsonValue& value) noexcept;

 private:
  // Prevent instance
  SceneValue() noexcept;

  //! Return a json value
  static QJsonValue getValue(const QJsonObject& object,
                             const QString& key) noexcept;
};

//! \}

} // namespace nanairo

#include "scene_value-inl.hpp"

#endif // NANAIRO_SCENE_VALUE_HPP
