/*!
  \file tone_mapping_operator.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TONE_MAPPING_OPERATOR_HPP
#define NANAIRO_TONE_MAPPING_OPERATOR_HPP

// Qt
#include <QColor>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QImage;
class QJsonObject;

namespace nanairo {

// Forward declaration
class HdrImage;
class System;

//! \addtogroup Core
//! \{

/*!
  \brief The interface of tone mapping class.
  \details
  No detailed.
  */
class ToneMappingOperator
{
 public:
  //! Initialize the method
  ToneMappingOperator(const System& system, const QJsonObject& settings) noexcept;

  //! Finalize the method
  virtual ~ToneMappingOperator() noexcept;


  //! Return the exposure 
  Float exposure() const noexcept;

  //! Return the inverse gamma value
  Float inverseGamma() const noexcept;

  //! Apply a tonemapping operator
  void map(System& system,
           const HdrImage& hdr_image,
           QImage* ldr_image) const noexcept;

 protected:
  //! Apply a tonemap curve
  virtual Float tonemap(const Float x) const noexcept = 0;

 private:
  //! Return the pixel array of LDR image
  QRgb* getPixelArray(QImage* ldr_image) const noexcept;

  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;


  Float inverse_gamma_;
  Float exposure_;
};

//! Make tonemapping method
UniquePointer<ToneMappingOperator> makeToneMappingOperator(
    const System& system,
    const QJsonObject& settings) noexcept;

//! \} Core

} // namespace nanairo

#include "tone_mapping_operator-inl.hpp"

#endif // NANAIRO_TONE_MAPPING_OPERATOR_HPP
