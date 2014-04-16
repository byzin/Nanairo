/*!
  \file cui_renderer_manager.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_CUI_RENDERER_MANAGER_HPP_
#define _NANAIRO_CUI_RENDERER_MANAGER_HPP_

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
  CuiRendererManager(const QString& scene_file_path);

  
  //! Render the image
  void render();

 private:
  //! Get the current time string
  QString getCurrentTime() const;

  //! Initialize the renderer manager
  void initialize();

  //!
  void setRenderer(const SceneRendererBase* renderer);


  QString scene_file_path_;
};

} // namespace nanairo

#endif // _NANAIRO_CUI_RENDERER_MANAGER_HPP_
