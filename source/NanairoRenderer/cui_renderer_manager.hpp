/*!
  \file cui_renderer_manager.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CUI_RENDERER_MANAGER_HPP
#define NANAIRO_CUI_RENDERER_MANAGER_HPP

// Qt
#include <QString>

namespace nanairo {

// Forward declaration
class SceneRendererBase;

/*!
  \details
  No detailed.
  */
class CuiRendererManager
{
 public:
  //! Initialize the renderer manager
  CuiRendererManager(const QString& scene_file_path) noexcept;

  
  //! Render the image
  void render() noexcept;

 private:
  //! Get the current time string
  QString getCurrentTime() const noexcept;

  //! Initialize the renderer manager
  void initialize() noexcept;

  //!
  void setRenderer(const SceneRendererBase* renderer) noexcept;


  QString scene_file_path_;
};

} // namespace nanairo

#endif // NANAIRO_CUI_RENDERER_MANAGER_HPP
