/*!
  \file cui_renderer_manager.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CUI_RENDERER_MANAGER_HPP
#define NANAIRO_CUI_RENDERER_MANAGER_HPP

// Standard C++ library
#include <tuple>
// Qt
#include <QJsonObject>
#include <QScopedPointer>
// Nanairo
#include "NanairoRenderer/scene_renderer_base.hpp"

// Forward declaration
class QString;

namespace nanairo {

// Forward declaration
class RendererParameter;

/*!
  \details
  No detailed.
  */
class CuiRendererManager
{
 public:
  //! Initialize the renderer manager
  CuiRendererManager(const RendererParameter& parameters) noexcept;


  //! Invoke renderer function
  void invokeRenderer() noexcept;

  //! Check if the renderer is runnable
  bool isRunnable() const noexcept;
  
 private:
  //! Generate a output dir
  std::tuple<bool, QString> generateOutputDir() const noexcept;

  //! Get the current time string
  QString getCurrentTime() const noexcept;

  //! Initialize the renderer manager
  void initialize(const RendererParameter& parameters) noexcept;

  //!
  void setRenderer() noexcept;


  QScopedPointer<SceneRendererBase> renderer_;
  QJsonObject settings_;
  bool is_runnable_;
};

} // namespace nanairo

#endif // NANAIRO_CUI_RENDERER_MANAGER_HPP
