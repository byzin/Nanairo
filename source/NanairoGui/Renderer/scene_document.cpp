/*!
  \file scene_document.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_document.hpp"
// Qt
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrl>
#include <QVariant>
#include <QtGlobal>
// Zisc
#include "zisc/error.hpp"
#include "NanairoGui/keyword.hpp"

namespace nanairo {

/*!
  */
bool SceneDocument::isSceneDocument(const QString& file_path,
                                    QString& error_message) noexcept
{
  const QFileInfo file_info{file_path};
  if (!file_info.exists()) {
    error_message = QStringLiteral("File does't exist: ") + file_path;
    return false;
  }
  if (file_info.suffix() != keyword::nanairoFileFormat) {
    error_message = QStringLiteral("File isn't Nanairo scene file: ") + file_path;
    return false;
  }
  return true;
}

/*!
  */
bool SceneDocument::loadDocument(const QUrl& file_url,
                                 QVariant& node,
                                 QString& error_message) noexcept
{
  QJsonObject object;
  const auto file_path = toFilePath(file_url);
  const bool result = loadDocument(file_path, object, error_message);
  if (result)
    node = toVariant(object);
  return result;
}

/*!
  */
bool SceneDocument::loadDocument(const QString& file_path,
                                 QJsonObject& node,
                                 QString& error_message) noexcept
{
  if (!isSceneDocument(file_path, error_message))
    return false;

  // Open a scene document
  QFile json_file{file_path};
  const bool is_open = json_file.open(QIODevice::ReadOnly | QIODevice::Text);
  if (!is_open) {
    error_message = QStringLiteral("File cann't open: ") + file_path;
    return false;
  }

  // Parse a scene document
  QJsonParseError parse_result;
  auto&& document = QJsonDocument::fromJson(json_file.readAll(), &parse_result);
  const bool is_no_error = (parse_result.error == QJsonParseError::NoError);
  if (!is_no_error) {
    error_message = QStringLiteral("Scenen parse failed: ") +
                    parse_result.errorString();
    return false;
  }

  node = document.object();
  return true;
}

/*!
  */
bool SceneDocument::saveDocument(const QUrl& file_url,
                                 const QVariant& node,
                                 QString& error_message) noexcept
{
  const auto object = QJsonObject::fromVariantMap(node.toMap());
  const auto file_path = toFilePath(file_url);
  return saveDocument(file_path, object, error_message);
}

/*!
  */
bool SceneDocument::saveDocument(const QString& file_path,
                                 const QJsonObject& node,
                                 QString& error_message) noexcept
{
  // Open a document
  QFile json_file{file_path};
  const bool is_open = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
  if (!is_open) {
    error_message = QStringLiteral("Document cann't save as: ") + file_path;
    return false;
  }

  // Save a document
  QJsonDocument document{node};
  json_file.write(document.toJson());

  return true;
}

/*!
  */
QString SceneDocument::toFilePath(const QUrl& file_url) noexcept
{
  return file_url.isLocalFile() 
      ? file_url.toLocalFile() // Local file
      : ":" + file_url.toString(QUrl::RemoveScheme); // Resource file
}

/*!
  */
QVariant SceneDocument::toVariant(const QJsonValue& value) noexcept
{
  QVariant node;
  switch (value.type()) {
   case QJsonValue::Null: {
    zisc::raiseError("The json value is null.");
    break;
   }
   case QJsonValue::Bool:
   case QJsonValue::Double:
   case QJsonValue::String: {
    node = value.toVariant();
    break;
   }
   case QJsonValue::Array: {
    node = toVariant(value.toArray());
    break;
   }
   case QJsonValue::Object: {
    node = toVariant(value.toObject());
    break;
   }
   case QJsonValue::Undefined: {
    zisc::raiseError("The json value is undefined.");
    break;
   }
   default: {
    zisc::raiseError("Undefined json value type is specified.");
    break;
   }
  }
  return node;
}

/*!
  */
QVariant SceneDocument::toVariant(const QJsonArray& array) noexcept
{
  QVariantList node;
  node.reserve(array.size());
  for (const auto& element : array) {
    const auto value = toVariant(element);
    node.append(value);
  }
  return QVariant::fromValue(node);
}

/*!
  */
QVariant SceneDocument::toVariant(const QJsonObject& object) noexcept
{
  QVariantMap node;
  const auto keys = object.keys();
  for (const auto& key : keys) {
    const auto value = toVariant(object[key]);
    node.insert(key, value);
  }
  return QVariant::fromValue(node);
}

} // namespace nanairo
