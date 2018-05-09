/*!
  \file cui_renderer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CUI_RENDERER_HPP
#define NANAIRO_CUI_RENDERER_HPP

// Standard C++ library
#include <string>
// Qt
#include <QImage>
// Nanairo
#include "simple_renderer.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Gui
//! \{

/*!
  */
class CuiRenderer : public SimpleRenderer
{
 public:
  //! Create a renderer
  CuiRenderer() noexcept;


  //! Return the LDR image for Qt
  QImage& ldrImageHelper() noexcept;

  //! Return the LDR image for Qt
  const QImage& ldrImageHelper() const noexcept;

  //! Set the image body
  void setImage(QImage* image) noexcept;

 protected:
  //! Output LDR image
  void outputLdrImage(const std::string& output_path,
                      const uint64 cycle) noexcept override;

 private:
  //! Initialize the renderer
  void initialize() noexcept;


  QImage* ldr_image_helper_;
};

//! \} Gui

} // namespace nanairo

#endif // NANAIRO_CUI_RENDERER_HPP
