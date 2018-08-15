/*!
  \file cui_renderer_manager.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CUI_RENDERER_MANAGER_HPP
#define NANAIRO_CUI_RENDERER_MANAGER_HPP

// Standard C++ library
#include <fstream>
#include <memory>
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

  //! Return an output path of rendered images
  const QString& outputPath() const noexcept;

  //! Set an output path of rendered images
  void setOutputPath(const QString& output_path) noexcept;

 protected:
  //! Backup scene files
  void backupSceneFiles(const QJsonObject& scene_value,
                        const SettingNodeBase& scene_settings,
                        const QString& output_dir,
                        QString* error_message) const noexcept;

  //! Make a directory for saving images
  QString makeOutputDir(const SettingNodeBase& scene_settings) const noexcept;

  //! Prepare renderer with the scene for rendering
  bool prepareForRendering(const SettingNodeBase& scene_settings,
                           CuiRenderer* renderer,
                           QString* error_message) const noexcept;

 private:
  //! Return the current time string
  QString getCurrentTime() const noexcept;

  //! Initialize the renderer manager
  void initialize() noexcept;


  QString output_path_;
  bool is_saving_scene_binary_enabled_;
};

//! \} Gui

} // namespace nanairo

#endif // NANAIRO_CUI_RENDERER_MANAGER_HPP
