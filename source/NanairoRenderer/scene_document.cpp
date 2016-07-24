/*!
  \file scene_document.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_document.hpp"
// Qt
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QUrl>
#include <QVariantMap>
#include <QtGlobal>
// Zisc
#include "zisc/error.hpp"

namespace nanairo {

/*!
  */
bool SceneDocument::checkDocumentInfo(const QString& file_path,
                                      QString& error_message) noexcept
{
  const QFileInfo file_info{file_path};
  if (!file_info.exists()) {
    error_message = QStringLiteral("File does't exist: ") + file_path;
    return false;
  }
  if (file_info.suffix() != "nana") {
    error_message = QStringLiteral("File isn't Nanairo scene file: ") + file_path;
    return false;
  }
  return true;
}

/*!
  */
bool SceneDocument::loadDocument(const QUrl& file_url,
                                 QVariantMap& node,
                                 QString& error_message) noexcept
{
  QJsonObject object;
  const auto file_path = toFilePath(file_url);
  const bool result = loadDocument(file_path, object, error_message);
  node = object.toVariantMap();
  return result;
}

/*!
  */
bool SceneDocument::loadDocument(const QString& file_path,
                                 QJsonObject& node,
                                 QString& error_message) noexcept
{
  if (!checkDocumentInfo(file_path, error_message))
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
                                 const QVariantMap& node,
                                 QString& error_message) noexcept
{
  const auto object = QJsonObject::fromVariantMap(node);
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

} // namespace nanairo
