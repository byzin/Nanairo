/*!
  \file gui_scene_settings.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_settings.hpp"
// Qt
#include <QColor>
#include <QDir>
#include <QObject>
#include <QRegularExpression>
#include <QSettings>
#include <QScopedPointer>
#include <QString>
#include <QtGlobal>
#include <QUrl>
#include <QVariant>

#ifdef NANAIRO_DEBUG
#include <QDebug>
#define NANAIRO_CHECK_KEY(key) checkKey(key)

/*!
  \details
  No detailed.
  */
void SceneSettings::checkKey(const QString& key) const noexcept
{
  if (!settings_->contains(key))
    qDebug() << "key \"" << key << "\" has no setting.";
}

#else // NANAIRO_DEBUG
#define NANAIRO_CHECK_KEY(key)
#endif // NANAIRO_DEBUG

namespace nanairo {

/*!
  \details
  No detailed.
  */
SceneSettings::SceneSettings()
{
}

/*!
  \details
  No detailed.
  */
void SceneSettings::clear()
{
  settings_->clear();
}

/*!
  \details
  No detailed.
  */
void SceneSettings::close()
{
  settings_.reset();
}

/*!
  \details
  No detailed.
  */
bool SceneSettings::booleanValue(const QString& key, const bool default_value) const
{
  NANAIRO_CHECK_KEY(key);
  return settings_->value(key, QVariant{default_value}).toBool();
}

/*!
  \details
  No detailed.
  */
QColor SceneSettings::colorValue(const QString& key, const QColor& default_value) const
{
  NANAIRO_CHECK_KEY(key + "/R");
  NANAIRO_CHECK_KEY(key + "/G");
  NANAIRO_CHECK_KEY(key + "/B");
  const int red = settings_->value(key + "/R", default_value.red()).toInt();
  const int green = settings_->value(key + "/G", default_value.green()).toInt();
  const int blue = settings_->value(key + "/B", default_value.blue()).toInt();
  return QColor{red, green, blue};
}

/*!
  \details
  No detailed.
  */
int SceneSettings::intValue(const QString& key, const int default_value) const
{
  NANAIRO_CHECK_KEY(key);
  return settings_->value(key, QVariant{default_value}).toInt();
}

/*!
  \details
  No detailed.
  */
void SceneSettings::open(const QUrl& file_path)
{
  settings_.reset(new QSettings{file_path.toLocalFile(), QSettings::IniFormat});
}

/*!
  \details
  No detailed.
  */
void SceneSettings::open(const QString& file_path)
{
  settings_.reset(new QSettings{file_path, QSettings::IniFormat});
}

/*!
  \details
  No detailed.
  */
qreal SceneSettings::realValue(const QString& key, const qreal default_value) const
{
  NANAIRO_CHECK_KEY(key);
  return settings_->value(key, QVariant{default_value}).toReal();
}

/*!
  \details
  No detailed.
  */
QString SceneSettings::stringValue(const QString& key, 
                                   const QString& default_value) const
{
  NANAIRO_CHECK_KEY(key);
  return settings_->value(key, QVariant{default_value}).toString();
}

/*!
  \details
  No detailed.
  */
QUrl SceneSettings::urlValue(const QString& key, const QUrl& default_value) const
{
  NANAIRO_CHECK_KEY(key);
  const auto path = settings_->value(key, QVariant{default_value.toLocalFile()});
  QDir current_dir;
  auto absolute_path = current_dir.absoluteFilePath(path.toString());
  absolute_path = QDir::cleanPath(absolute_path);
  return QUrl::fromLocalFile(absolute_path);
}

/*!
  \details
  No detailed.
  */
void SceneSettings::setBooleanValue(const QString& key, const bool value)
{
  settings_->setValue(key, QVariant{value});
}

/*!
  \details
  No detailed.
  */
void SceneSettings::setColorValue(const QString& key, const QColor& value)
{
  settings_->setValue(key + "/R", value.red());
  settings_->setValue(key + "/G", value.green());
  settings_->setValue(key + "/B", value.blue());
}

/*!
  \details
  No detailed.
  */
void SceneSettings::setIntValue(const QString& key, const int value)
{
  settings_->setValue(key, QVariant{value});
}

/*!
  \details
  No detailed.
  */
void SceneSettings::setRealValue(const QString& key, const qreal value)
{
  settings_->setValue(key, QVariant{value});
}

/*!
  \details
  No detailed.
  */
void SceneSettings::setStringValue(const QString& key, const QString& value)
{
  settings_->setValue(key, QVariant{value});
}

/*!
  \details
  No detailed.
  */
void SceneSettings::setUrlValue(const QString& key, const QUrl& value)
{
  QDir current_dir;
  const auto path = current_dir.relativeFilePath(value.toLocalFile());
  settings_->setValue(key, QVariant{path});
}

} // namespace nanairo
