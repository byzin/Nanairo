/*!
  \file cui_renderer_manager.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CUI_RENDERER_MANAGER_HPP
#define NANAIRO_CUI_RENDERER_MANAGER_HPP

// Qt
#include <QString>
// Nanairo
#include "NanairoCore/Setting/setting_node_base.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class CuiRenderer;

//! \addtogroup Gui
//! \{

/*!
  \details
  No detailed.
  */
class CuiRendererManager
{
 public:
  //! Initialize the renderer manager
  CuiRendererManager() noexcept;


  //! Enable saving scene binary
  void enableSavingSceneBinary(const bool is_enabled) noexcept;

  //! Invoke rendering with the scene
  void invokeRendering(const QString& scene_file_path) const noexcept;

  //! Check whether is saving scene binary enabled
  bool isSavingSceneBinaryEnabled() const noexcept;

 protected:
  //! Prepare renderer with the scene for rendering
  bool prepareForRendering(const QJsonObject& scene_value,
                           CuiRenderer* renderer,
                           QString* output_dir,
                           QString* error_message) const noexcept;

 private:
  //! Return the current time string
  QString getCurrentTime() const noexcept;

  //! Initialize the renderer manager
  void initialize() noexcept;

  //! Make a directory for saving images
  QString makeOutputDir(const SettingNodeBase* settings) const noexcept;


  bool is_saving_scene_binary_enabled_;
};

//! \} Gui

} // namespace nanairo

#endif // NANAIRO_CUI_RENDERER_MANAGER_HPP
