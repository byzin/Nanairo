/*!
  \file gui_engine.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_engine.hpp"
// Qt
#include <QFont>
#include <QGuiApplication>
#include <QUrl>
#include <QString>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>
// Nanairo
#include "rendered_image_provider.hpp"
#include "Renderer/gui_renderer_manager.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
GuiEngine::GuiEngine() noexcept :
    image_provider_{new RenderedImageProvider}
{
  initialize();
}

/*!
  \details
  No detailed.
  */
void GuiEngine::load(const QUrl& url) noexcept
{
  engine_.load(url);
}

/*!
  \details
  No detailed.
  */
void GuiEngine::initialize() noexcept
{
  initializeFont();
  manager_.setRenderedImageProvider(image_provider_);
  engine_.rootContext()->setContextProperty(QLatin1String("nanairoManager"),
                                            &manager_);
  engine_.addImageProvider("renderedImage", image_provider_);
}

/*!
  */
void GuiEngine::initializeFont() noexcept
{
  auto font = QGuiApplication::font();

#ifdef Z_LINUX
  font.setPixelSize(12);
#endif // Z_LINUX

  QGuiApplication::setFont(font);
}

} // namespace nanairo
