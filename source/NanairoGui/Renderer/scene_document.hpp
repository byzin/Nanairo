/*!
  \file scene_document.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SCENE_DOCUMENT_HPP
#define NANAIRO_SCENE_DOCUMENT_HPP

// Qt
#include <QVariant>

// Forward declaration
class QJsonArray;
class QJsonObject;
class QJsonValue;
class QString;
class QUrl;

namespace nanairo {

//! \addtogroup Gui
//! \{

/*!
  */
class SceneDocument
{
 public:
  //! Check if a file is Nanairo scene file
  static bool isSceneDocument(const QString& file_path,
                              QString& error_message) noexcept;

  //! Load a scene document
  static bool loadDocument(const QUrl& file_url,
                           QVariant& node,
                           QString& error_message) noexcept;

  //! Load a scene document
  static bool loadDocument(const QString& file_path,
                           QJsonObject& node,
                           QString& error_message) noexcept;

  //! Save a scene document
  static bool saveDocument(const QUrl& file_url,
                           const QVariant& node,
                           QString& error_message) noexcept;

  //! Save a scene document
  static bool saveDocument(const QString& file_path,
                           const QJsonObject& node,
                           QString& error_message) noexcept;

 private:
  //! Convert a URL to a file path
  static QString toFilePath(const QUrl& url) noexcept;

  //! Convert JsonArray to List 
  static QVariant toVariant(const QJsonArray& node) noexcept;

  //! Convert JsonObject to Map
  static QVariant toVariant(const QJsonObject& node) noexcept;

  //! Convert JsonValue to Variant
  static QVariant toVariant(const QJsonValue& node) noexcept;
};

//! \}

} // namespace nanairo

#endif // NANAIRO_SCENE_DOCUMENT_HPP
