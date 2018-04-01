/*!
  \file scene_value.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */  

#ifndef NANAIRO_SCENE_VALUE_HPP
#define NANAIRO_SCENE_VALUE_HPP

// Standard C++ library
#include <memory>
// Qt
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
// Nanairo
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

//! \addtogroup Gui
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

  //! Convert json scene to setting
  static std::unique_ptr<SettingNodeBase> toSetting(
      const QJsonObject& value) noexcept;

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

  //! Convert json bvh to bvh setting
  static void toBvhSetting(const QJsonObject& value,
                           SettingNodeBase* setting) noexcept;

  //! Convert json camera to camera setting
  static void toCameraSetting(const QJsonArray& value,
                              SettingNodeBase* setting) noexcept;

  //! Convert json emitter to emitter setting
  static void toEmitterModelSetting(const QJsonObject& value,
                                    SettingNodeBase* setting) noexcept;

  //! Convert json object to object setting
  static void toObjectModelSetting(const QJsonObject& object_value,
                                   SettingNodeBase* setting) noexcept;

  //! Convert json object to object setting
  static void toObjectSetting(const QJsonArray& value,
                              SettingNodeBase* setting) noexcept;

  //! Convert json rendering method to rendering method setting
  static void toRenderingMethodSetting(const QJsonObject& value,
                                       SettingNodeBase* setting) noexcept;

  //! Convert json scene to scene setting
  static void toSceneSetting(const QJsonObject& value,
                             SettingNodeBase* setting) noexcept;

  //! Convert json single object to single object setting
  static void toSingleObjectSetting(const QJsonObject& object_value,
                                    SettingNodeBase* setting) noexcept;

  //! Convert json spectra object to spectra setting
  static void toSpectraSetting(const QJsonObject& spectra_value,
                               SettingNodeBase* setting) noexcept;

  //! Convert json surface to surface setting
  static void toSurfaceModelSetting(const QJsonObject& value,
                                    SettingNodeBase* setting) noexcept;

  //! Convert json system to system setting
  static void toSystemSetting(const QJsonObject& value,
                              SettingNodeBase* setting) noexcept;

  //! Convert json texture to texture setting
  static void toTextureModelSetting(const QJsonObject& value,
                                    SettingNodeBase* setting) noexcept;
};

//! \}

} // namespace nanairo

#include "scene_value-inl.hpp"

#endif // NANAIRO_SCENE_VALUE_HPP
