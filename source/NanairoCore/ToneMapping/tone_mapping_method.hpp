/*!
  \file tone_mapping_method.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_TONE_MAPPING_METHOD_HPP_
#define _NANAIRO_TONE_MAPPING_METHOD_HPP_

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
  ToneMappingMethod(const System& system, const SceneSettings& settings);

  //! Finalize the method
  virtual ~ToneMappingMethod();


  //! Return the pixel array of LDR image
  static QRgb* getPixelArray(QImage& ldr_image);

  //! Return the inverse gamma value
  Float inverseGamma() const;

  //! Apply tonemapping
  virtual void toneMap(System& system,
                       const HdrImage& hdr_image, 
                       QImage& ldr_image) = 0;

  //! Return the xyz to rgb matrix
  const Matrix3x3& toRgbMatrix() const;

  //! Return the rgb to xyz matrix
  const Matrix3x3& toXyzMatrix() const;

 protected:
  //! Return the white point
  YxyColor getWhitePoint(const std::vector<YxyColor>& pixel_luminance) const;

  //! Set pixel bright
  void setPixelLuminance(System& system,
                         const HdrImage& hdr_image,
                         std::vector<YxyColor>& pixel_luminance) const;

 private:
  //! Initialize
  void initialize(const System& system, const SceneSettings& settings);


  Matrix3x3 to_rgb_matrix_;
  Matrix3x3 to_xyz_matrix_;
  Float inverse_gamma_;
};

//! Make tonemapping method
UniquePointer<ToneMappingMethod> makeToneMappingMethod(const System& system,
                                                       const SceneSettings& settings);

//! \} Core 

} // namespace nanairo

#include "tone_mapping_method-inl.hpp"

#endif // _NANAIRO_TONE_MAPPING_METHOD_HPP_
