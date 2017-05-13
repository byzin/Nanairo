/*!
  \file scene_document.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SCENE_DOCUMENT_HPP
#define NANAIRO_SCENE_DOCUMENT_HPP

// Qt
#include <QVariant>

// Forward declaration
class QJsonObject;
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
  static bool checkDocumentInfo(const QString& file_path,
                                QString& error_message) noexcept;

  //! Load a scene document
  static bool loadDocument(const QUrl& file_url,
                           QVariantMap& node,
                           QString& error_message) noexcept;

  //! Load a scene document
  static bool loadDocument(const QString& file_path,
                           QJsonObject& node,
                           QString& error_message) noexcept;

  //! Save a scene document
  static bool saveDocument(const QUrl& file_url,
                           const QVariantMap& node,
                           QString& error_message) noexcept;

  //! Save a scene document
  static bool saveDocument(const QString& file_path,
                           const QJsonObject& node,
                           QString& error_message) noexcept;

 private:
  //! Convert a URL to a file path
  static QString toFilePath(const QUrl& url) noexcept;
};

//! \}

} // namespace nanairo

#endif // NANAIRO_SCENE_DOCUMENT_HPP
