///*!
//  \file rough_plastic_surface.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2016 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef _NANAIRO_ROUGH_PLASTIC_SURFACE_HPP_
//#define _NANAIRO_ROUGH_PLASTIC_SURFACE_HPP_
//
//// Standard C++ library
//#include <vector>
//// Nanairo
//#include "surface_model.hpp"
//#include "NanairoCore/nanairo_core_config.hpp"
//#include "NanairoCore/Color/spectral_distribution.hpp"
//#include "NanairoCore/Utility/unique_pointer.hpp"
//
////! Forward declaration
//class QString;
//
//namespace nanairo {
//
////! Forward declaration
//class IntersectionInfo;
//class Sampler;
//class SceneSettings;
//template <uint> class ShaderModel;
//class Texture;
//template <uint> class WavelengthSamples;
//
////! \addtogroup Core
////! \{
//
///*!
//  \details
//  No detailed.
//  */
//class RoughPlasticSurface : public SurfaceModel
//{
// public:
//  template <uint kSampleSize>
//  using ShaderPointer = SurfaceModel::ShaderPointer<kSampleSize>;
//
//
//  //! Create a rough plastic surface
//  RoughPlasticSurface(const SceneSettings& settings, 
//                      const QString& prefix,
//                      const std::vector<const Texture*>& texture_list);
//
//
//  //! Make a ggx plastic BRDF
//  template <uint kSampleSize>
//  ShaderPointer<kSampleSize> makeGgxPlasticBrdf(
//      const Vector3* vin,
//      const IntersectionInfo& intersection,
//      const WavelengthSamples<kSampleSize>& wavelengths,
//      Sampler& sampler,
//      MemoryPool& memory_pool) const;
//
//  //! Return the surface model size
//  std::size_t surfaceSize() const override;
//
//  //! Return the rough plastic type
//  SurfaceType type() const override;
//
// private:
//  //! Initialize
//  void initialize(const SceneSettings& settings, 
//                  const QString& prefix,
//                  const std::vector<const Texture*>& texture_list);
//
//
//  const Texture* diffuse_color_;
//  const Texture* roughness_;
//  SpectralDistribution eta_; //!< Interior exterior ratio of refractive index
//};
//
////! \}
//
//} // namespace nanairo
//
//#include "rough_plastic_surface-inl.hpp"
//
//#endif // _NANAIRO_ROUGH_PLASTIC_SURFACE_HPP_
