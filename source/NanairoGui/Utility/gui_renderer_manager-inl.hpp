/*!
  \file gui_renderer_manager-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GUI_RENDERER_MANAGER_INL_HPP
#define NANAIRO_GUI_RENDERER_MANAGER_INL_HPP

#include "gui_renderer_manager.hpp"
// Qt
#include <QObject>
#include <QString>

namespace nanairo {

/*!
  \details
  No detailed.
  */
inline
void GuiRendererManager::setMessage(const QString& message) const noexcept
{
  emit outputMessage(message);
}

} // namespace nanairo

#endif // NANAIRO_GUI_RENDERER_MANAGER_INL_HPP
