/*!
  \file scene_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SCENE_SETTING_NODE_HPP
#define NANAIRO_SCENE_SETTING_NODE_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <string_view>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/system.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class SceneSettingNode : public SettingNodeBase
{
 public:
  //!
  SceneSettingNode(const SettingNodeBase* parent = nullptr) noexcept;

  //!
  ~SceneSettingNode() noexcept override;


  //! Return the bvh setting node
  SettingNodeBase* bvhSettingNode() noexcept;

  //! Return the bvh setting node
  const SettingNodeBase* bvhSettingNode() const noexcept;

  //! Return the camera setting node
  SettingNodeBase* cameraSettingNode() noexcept;

  //! Return the camera setting node
  const SettingNodeBase* cameraSettingNode() const noexcept;

  //! Return the memory resource for data allocation
  zisc::pmr::memory_resource* dataResource() const noexcept override;

  //! Return the emitter model setting node
  SettingNodeBase* emitterModelSettingNode() noexcept;

  //! Return the emitter model setting node
  const SettingNodeBase* emitterModelSettingNode() const noexcept;

  //! Initialize a scene node
  void initialize() noexcept override;

  //! Return the node type
  static SettingNodeType nodeType() noexcept;

  //! Return the object setting node
  SettingNodeBase* objectSettingNode() noexcept;

  //! Return the object setting node
  const SettingNodeBase* objectSettingNode() const noexcept;

  //! Read the setting data from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Return the rendering method setting node
  SettingNodeBase* renderingMethodSettingNode() noexcept;

  //! Return the rendering method setting node
  const SettingNodeBase* renderingMethodSettingNode() const noexcept;

  //! Return the scene name
  std::string_view sceneName() const noexcept;

  //! Set the scene name
  void setSceneName(const std::string_view& scene_name) noexcept;

  //! Return the surface model setting node
  SettingNodeBase* surfaceModelSettingNode() noexcept;

  //! Return the surface model setting node
  const SettingNodeBase* surfaceModelSettingNode() const noexcept;

  //! Return the system setting node
  SettingNodeBase* systemSettingNode() noexcept;

  //! Return the system setting node
  const SettingNodeBase* systemSettingNode() const noexcept;

  //! Return the texture model setting node
  SettingNodeBase* textureModelSettingNode() noexcept;

  //! Return the texture model setting node
  const SettingNodeBase* textureModelSettingNode() const noexcept;

  //! Return the setting node type
  SettingNodeType type() const noexcept override;

  //! Return the memory resource for temporary works
  zisc::pmr::memory_resource* workResource() const noexcept override;

  //! Write the setting data to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  System::MemoryManager data_resource_;
  System::MemoryManager work_resource_;
  std::mutex data_mutex_;
  std::mutex work_mutex_;
  zisc::pmr::string scene_name_;
  zisc::UniqueMemoryPointer<SettingNodeBase> system_setting_node_;
  zisc::UniqueMemoryPointer<SettingNodeBase> rendering_method_setting_node_;
  zisc::UniqueMemoryPointer<SettingNodeBase> texture_model_setting_node_;
  zisc::UniqueMemoryPointer<SettingNodeBase> surface_model_setting_node_;
  zisc::UniqueMemoryPointer<SettingNodeBase> emitter_model_setting_node_;
  zisc::UniqueMemoryPointer<SettingNodeBase> camera_setting_node_;
  zisc::UniqueMemoryPointer<SettingNodeBase> object_setting_node_;
  zisc::UniqueMemoryPointer<SettingNodeBase> bvh_setting_node_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SCENE_SETTING_NODE_HPP
