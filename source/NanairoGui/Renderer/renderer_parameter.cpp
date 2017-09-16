/*!
  \file renderer_parameter.cpp
  \author Sho Ikeda
  
  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "renderer_parameter.hpp"
// Qt
#include <QString>

namespace nanairo {

/*!
  */
RendererParameter::RendererParameter() noexcept
    : manager_type_{RendererManagerType::kGui}
{
}

/*!
  */
RendererManagerType RendererParameter::managerType() const noexcept
{
  return manager_type_;
}

/*!
  */
const QString& RendererParameter::sceneFilePath() const noexcept
{
  return scene_file_path_;
}

/*!
  */
void RendererParameter::setManagerType(const RendererManagerType& type) noexcept
{
  manager_type_ = type;
}

/*!
  */
void RendererParameter::setSceneFilePath(const QString& scene_file_path) noexcept
{
  scene_file_path_ = scene_file_path;
}

} // namespace nanairo
