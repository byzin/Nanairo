/*!
  \file emitter_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_EMITTER_SETTING_NODE_HPP
#define NANAIRO_EMITTER_SETTING_NODE_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <string>
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! NonDirectional emitter
struct NonDirectionalEmitterParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  double radiant_exitance_ = 128.0;
  uint32 color_index_ = 0;
};

/*!
  */
class EmitterSettingNode : public SettingNodeBase
{
 public:
  //! Return the emitter type
  EmitterType emitterType() const noexcept;

  //! Initialize a emitter setting
  void initialize() noexcept override;

  //! Return the emitter name
  const std::string& name() const noexcept;

  //! Return the non directional emitter parameters
  NonDirectionalEmitterParameters& nonDirectionalEmitterParameters() noexcept;

  //! Return the non directional emitter parameters
  const NonDirectionalEmitterParameters& nonDirectionalEmitterParameters()
      const noexcept;

  //! Read the emitter setting from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Set the emitter type
  void setEmitterType(const EmitterType type) noexcept;

  //! Set the emitter name
  void setName(const std::string& name) noexcept;

  //! Set the emitter name
  void setName(std::string&& name) noexcept;

  //! Return the node type
  SettingNodeType type() const noexcept override;

  //! Write the emitter setting to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  std::unique_ptr<NodeParameterBase> parameters_;
  std::string name_;
  EmitterType emitter_type_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_EMITTER_SETTING_NODE_HPP
