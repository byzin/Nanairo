/*!
  \file renderer_parameter.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_RENDERER_PARAMETER_HPP
#define NANAIRO_RENDERER_PARAMETER_HPP

// Qt
#include <QString>

namespace nanairo {

//! \addtogroup Renderer
//! \{

enum class RendererManagerType
{
  kCui,
  kGui
};

/*!
  */
class RendererParameter
{
 public:
  //! Initialize renderer parameters
  RendererParameter() noexcept;


  //! Return the renderer manager type
  RendererManagerType managerType() const noexcept;

  //! Return the scene file path
  const QString& sceneFilePath() const noexcept;

  //! Set a renderer manager type
  void setManagerType(const RendererManagerType& type) noexcept;

  //! Set a scene file path
  void setSceneFilePath(const QString& scene_file_path) noexcept;

 private:
  QString scene_file_path_;
  RendererManagerType manager_type_;
};

//! \}

} // namespace nanairo

#endif // NANAIRO_RENDERER_PARAMETER_HPP
