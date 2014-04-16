/*!
  \file scene_settings.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_SCENE_SETTINGS_HPP_
#define _NANAIRO_SCENE_SETTINGS_HPP_

// Qt
#include <QColor>
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QSettings>
#include <QUrl>
#include <QtGlobal>

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class SceneSettings : public QObject
{
  Q_OBJECT

 public:
  //! Initialize the scene setting
  SceneSettings();


  //! Clear all settings
  Q_INVOKABLE void clear();

  //! Close a setting file
  Q_INVOKABLE void close();

  //! Return the bool value for setting key
  Q_INVOKABLE bool booleanValue(const QString& key, 
                                const bool default_value = false) const;

  //! Return the color value for setting key
  Q_INVOKABLE QColor colorValue(const QString& key, 
                                const QColor& default_value = QColor{}) const;

  //! Return the integer value for setting key
  Q_INVOKABLE int intValue(const QString& key, 
                           const int default_value = 0) const;

  //! Open a setting file
  Q_INVOKABLE void open(const QUrl& file_path);

  //! Open a setting file
  Q_INVOKABLE void open(const QString& file_path);

  //! Return the real value for setting key
  Q_INVOKABLE qreal realValue(const QString& key, 
                              const qreal default_value = 0.0) const;

  //! Return the string value for setting key
  Q_INVOKABLE QString stringValue(const QString& key, 
                                  const QString& default_value = QString{}) const;

  //! Return the URL value for setting key
  Q_INVOKABLE QUrl urlValue(const QString& key, 
                            const QUrl& default_value = QUrl{}) const;

  //! Set the boolean value of setting key
  Q_INVOKABLE void setBooleanValue(const QString& key, const bool value);

  //! Set the color value of setting key
  Q_INVOKABLE void setColorValue(const QString& key, const QColor& value);

  //! Set the integer value of setting key
  Q_INVOKABLE void setIntValue(const QString& key, const int value);

  //! Set the real value of setting key
  Q_INVOKABLE void setRealValue(const QString& key, const qreal value);

  //! Set the string value of setting key
  Q_INVOKABLE void setStringValue(const QString& key, const QString& value);

  //! Set the URL value of setting key
  Q_INVOKABLE void setUrlValue(const QString& key, const QUrl& value);

 private:
  //! Check if the value corresponding to the key is exists
  void checkKey(const QString& key) const;


  QScopedPointer<QSettings> settings_;
};

//! \} Core

} // namespace nanairo

#endif // _NANAIRO_SCENE_SETTINGS_HPP_
