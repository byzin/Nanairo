/*!
  \file gui_engine.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "gui_engine.hpp"
// Qt
#include <QUrl>
#include <QString>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>
// Nanairo
#include "Utility/gui_renderer_manager.hpp"
#include "Utility/rendered_image_provider.hpp"

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
  manager_.setRenderedImageProvider(image_provider_);
  engine_.rootContext()->setContextProperty(QLatin1String("nanairoManager"),
                                            &manager_);
  engine_.addImageProvider("renderedImage", image_provider_);
}

} // namespace nanairo
