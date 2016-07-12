/*!
  \file tone_mapping_method.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TONE_MAPPING_METHOD_HPP
#define NANAIRO_TONE_MAPPING_METHOD_HPP

// Qt
#include <QColor>
// Zisc
#include "zisc/matrix.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/yxy_color.hpp"
#include "NanairoCore/LinearAlgebra/transformation.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

// Forward declaration
class QImage;

namespace nanairo {

// Forward declaration
class HdrImage;
class SceneSettings;
class System;

//! \addtogroup Core 
//! \{

/*!
  \brief The interface of tone mapping class.
  \details
  No detailed.
  */
class ToneMappingMethod
{
 public:
  //! Initialize the method
  ToneMappingMethod(const System& system, const SceneSettings& settings) noexcept;

  //! Finalize the method
  virtual ~ToneMappingMethod() noexcept;


  //! Return the pixel array of LDR image
  static QRgb* getPixelArray(QImage& ldr_image) noexcept;

  //! Return the inverse gamma value
  Float inverseGamma() const noexcept;

  //! Apply tonemapping
  virtual void toneMap(System& system,
                       const HdrImage& hdr_image, 
                       QImage& ldr_image) noexcept = 0;

  //! Return the xyz to rgb matrix
  const Matrix3x3& toRgbMatrix() const noexcept;

  //! Return the rgb to xyz matrix
  const Matrix3x3& toXyzMatrix() const noexcept;

 protected:
  //! Return the white point
  YxyColor getWhitePoint(const std::vector<YxyColor>& pixel_luminance) const noexcept;

  //! Set pixel bright
  void setPixelLuminance(System& system,
                         const HdrImage& hdr_image,
                         std::vector<YxyColor>& pixel_luminance) const noexcept;

 private:
  //! Initialize
  void initialize(const System& system, const SceneSettings& settings) noexcept;


  Matrix3x3 to_rgb_matrix_;
  Matrix3x3 to_xyz_matrix_;
  Float inverse_gamma_;
};

//! Make tonemapping method
UniquePointer<ToneMappingMethod> makeToneMappingMethod(const System& system,
                                                       const SceneSettings& settings) noexcept;

//! \} Core 

} // namespace nanairo

#include "tone_mapping_method-inl.hpp"

#endif // NANAIRO_TONE_MAPPING_METHOD_HPP
