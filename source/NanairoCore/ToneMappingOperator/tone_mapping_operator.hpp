/*!
  \file tone_mapping_operator.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TONE_MAPPING_OPERATOR_HPP
#define NANAIRO_TONE_MAPPING_OPERATOR_HPP

// Standard C++ library
#include <memory>
// Zisc
#include "zisc/sip_hash_engine.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class HdrImage;
class LdrImage;
class System;

//! \addtogroup Core
//! \{

enum class ToneMappingType : uint32
{
  kReinhard                   = zisc::SipHash32::hash("Reinhard"),
  kFilmic                     = zisc::SipHash32::hash("Filmic"),
  kUncharted2Filmic           = zisc::SipHash32::hash("Uncharted2Filmic")
};

/*!
  \brief The interface of tone mapping class.
  \details
  No detailed.
  */
class ToneMappingOperator
{
 public:
  //! Initialize the method
  ToneMappingOperator(const System& system, const SettingNodeBase* settings) noexcept;

  //! Finalize the method
  virtual ~ToneMappingOperator() noexcept;


  //! Return the exposure 
  Float exposure() const noexcept;

  //! Return the inverse gamma value
  Float inverseGamma() const noexcept;

  //! Make tonemapping method
  static std::unique_ptr<ToneMappingOperator> makeOperator(
      const System& system,
      const SettingNodeBase* settings) noexcept;

  //! Apply a tonemapping operator
  void map(System& system,
           const HdrImage& hdr_image,
           LdrImage* ldr_image) const noexcept;

 protected:
  //! Apply a tonemap curve
  virtual Float tonemap(const Float x) const noexcept = 0;

 private:
  //! Initialize
  void initialize(const System& system, const SettingNodeBase* settings) noexcept;


  Float inverse_gamma_;
  Float exposure_;
};

//! \} Core

} // namespace nanairo

#include "tone_mapping_operator-inl.hpp"

#endif // NANAIRO_TONE_MAPPING_OPERATOR_HPP
