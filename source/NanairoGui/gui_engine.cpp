/*!
  \file gui_engine.cpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
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
#include "NanairoCore/Utility/scene_settings.hpp"
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
  manager_.setSceneSettings(&settings_);
  engine_.rootContext()->setContextProperty(QLatin1String("manager"), &manager_);
  engine_.rootContext()->setContextProperty(QLatin1String("scene_settings"), 
                                            &settings_);
  engine_.addImageProvider("rendered_image", image_provider_);
}

} // namespace nanairo
