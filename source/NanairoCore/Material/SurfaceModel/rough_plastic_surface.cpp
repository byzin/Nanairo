///*!
//  \file rough_plastic_surface.cpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#include "rough_plastic_surface.hpp"
//// Standard C++ library
//#include <cstddef>
//#include <utility>
//#include <vector>
//// Qt
//#include <QString>
//// Nanairo
//#include "fresnel.hpp"
//#include "surface_model.hpp"
//#include "NanairoCommon/keyword.hpp"
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/Color/spectral_distribution.hpp"
//#include "NanairoCore/Utility/floating_point.hpp"
//#include "NanairoCore/Utility/scene_settings.hpp"
//
//namespace nanairo {
//
///*!
//  \details
//  No detailed.
//  */
//RoughPlasticSurface::RoughPlasticSurface(
//    const SceneSettings& settings,
//    const QString& prefix,
//    const std::vector<const Texture*>& texture_list)
//{
//  initialize(settings, prefix, texture_list);
//}
//
///*!
//  \details
//  No detailed.
//  */
//std::size_t RoughPlasticSurface::surfaceSize() const
//{
//  return sizeof(diffuse_color_) + sizeof(roughness_) + sizeof(eta_);
//}
//
///*!
//  \details
//  No detailed.
//  */
//SurfaceType RoughPlasticSurface::type() const
//{
//  return SurfaceType::RoughPlastic;
//}
//
///*!
//  \details
//  No detailed.
//  */
//void RoughPlasticSurface::initialize(
//    const SceneSettings& settings,
//    const QString& prefix,
//    const std::vector<const Texture*>& texture_list)
//{
//  const auto p = prefix + "/" + keyword::roughPlasticSurface;
//
//  auto key = p + "/" + keyword::diffuseColorIndex;
//  diffuse_color_ = getTexture(settings, key, texture_list);
//
//  key = p + "/" + keyword::roughnessIndex;
//  roughness_ = getTexture(settings, key, texture_list);
//
//  key = p + "/" + keyword::outerRefractiveIndex;
//  const auto n1 = makeSpectra(settings, key);
//
//  key = p + "/" + keyword::innerRefractiveIndex;
//  const auto n2 = makeSpectra(settings, key);
//
//  eta_ = n2 / n1;
//  NANAIRO_CHECK_VALID_FLOAT(eta_);
//  NANAIRO_CHECK_NON_ZERO_FLOAT(eta_);
//}
//
//} // namespace nanairo
