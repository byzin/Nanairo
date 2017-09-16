/*!
  \file test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "test.hpp"
// Standard C++ library
#include <memory>
// Qt
#include <QByteArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QtGlobal>
// GoogleTest
#include "gtest/gtest.h"
// Nanairo
#include "NanairoCore/system.hpp"

/*!
  \details
  No detailed.
  */
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

namespace  {

QByteArray makeTestSystemJson(const int image_width,
                              const int image_height,
                              const bool is_rgb_rendering)
{
  // Open system json
  const auto json_path = QStringLiteral(":/test/test_system.json");
  QFile json_file{json_path};
  const bool is_open = json_file.open(QIODevice::ReadOnly | QIODevice::Text);
  if (!is_open) {
    qCritical("Test resource file \"%s\" open failed.", qUtf8Printable(json_path));
  }

  //
  QString json_text{json_file.readAll()};

  // Replacement
  {
    const auto width = QString::number(image_width);
    const auto height = QString::number(image_height);
    const auto color = is_rgb_rendering ? QStringLiteral("RGB")
                                        : QStringLiteral("Spectra");
    json_text = json_text.arg(width, height, color);
  }
  return json_text.toLatin1();
}

} // namespace

/*!
  \details
  No detailed.
  */
std::unique_ptr<nanairo::System> makeTestSystem(const int image_width,
                                                const int image_height,
                                                const bool is_rgb_rendering)
{
  QJsonObject settings;
  {
    const auto json_text = ::makeTestSystemJson(image_width,
                                                image_height,
                                                is_rgb_rendering);
    QJsonParseError parse_result;
    const auto system_document = QJsonDocument::fromJson(json_text, &parse_result);
    if (parse_result.error != QJsonParseError::NoError) {
      qFatal("Parsing test system json failed.");
    }
    settings = system_document.object();
  }
  return std::make_unique<nanairo::System>(settings);
}
