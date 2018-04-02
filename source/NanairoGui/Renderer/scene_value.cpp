/*!
  \file scene_value.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "scene_value.hpp"
// Standard C++ library
#include <array>
#include <memory>
#include <tuple>
#include <vector>
// Qt
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QTextStream>
// Zisc
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/csv.hpp"
#include "zisc/unit.hpp"
// Nanairo
#include "obj_loader.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/color_space.hpp"
#include "NanairoCore/Color/rgba_32.hpp"
#include "NanairoCore/Color/ldr_image.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Material/EmitterModel/emitter_model.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/Setting/bvh_setting_node.hpp"
#include "NanairoCore/Setting/camera_setting_node.hpp"
#include "NanairoCore/Setting/emitter_setting_node.hpp"
#include "NanairoCore/Setting/group_object_setting_node.hpp"
#include "NanairoCore/Setting/material_setting_node.hpp"
#include "NanairoCore/Setting/object_model_setting_node.hpp"
#include "NanairoCore/Setting/rendering_method_setting_node.hpp"
#include "NanairoCore/Setting/scene_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/single_object_setting_node.hpp"
#include "NanairoCore/Setting/spectra_setting_node.hpp"
#include "NanairoCore/Setting/surface_setting_node.hpp"
#include "NanairoCore/Setting/system_setting_node.hpp"
#include "NanairoCore/Setting/texture_setting_node.hpp"
#include "NanairoCore/Setting/transformation_setting_node.hpp"
#include "NanairoCore/Shape/shape.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"
#include "NanairoGui/keyword.hpp"

namespace nanairo {

/*!
  */
std::unique_ptr<SettingNodeBase> SceneValue::toSetting(
    const QJsonObject& value) noexcept
{
  std::unique_ptr<SettingNodeBase> setting = std::make_unique<SceneSettingNode>();
  toSceneSetting(value, setting.get());
  return setting;
}

/*!
  */
void SceneValue::toBvhSetting(const QJsonObject& value,
                              SettingNodeBase* setting) noexcept
{
  auto bvh_setting = castNode<BvhSettingNode>(setting);
  // Attributes
  const auto bvh_value = toObject(value, keyword::bvh);
  {
    const auto bvh_type = toString(bvh_value, keyword::type);
    const BvhType bvh = (bvh_type == keyword::binaryRadixTreeBvh)
        ? BvhType::kBinaryRadixTree
        : BvhType::kAgglomerativeTreeletRestructuring;
    bvh_setting->setBvhType(bvh);
  }
  switch (bvh_setting->bvhType()) {
   case BvhType::kAgglomerativeTreeletRestructuring: {
    auto& parameters = bvh_setting->agglomerativeTreeletRestructuringParameters();
    {
      parameters.treelet_size_ = toInt<uint32>(bvh_value, keyword::treeletSize);
    }
    {
      parameters.optimization_loop_ = toInt<uint32>(bvh_value,
                                                    keyword::optimizationLoopCount);
    }
    break;
   }
   case BvhType::kBinaryRadixTree:
   default:
    break;
  }
}

/*!
  */
void SceneValue::toCameraSetting(const QJsonArray& value,
                                 SettingNodeBase* setting) noexcept
{
  auto object_setting = castNode<ObjectModelSettingNode>(setting);
  const auto camera_value = toObject(value[0]);
  ZISC_ASSERT(toString(camera_value, keyword::type) == keyword::cameraObject,
              "The object value isn't camera object.");

  auto s = object_setting->setObject(ObjectType::kCamera);
  toObjectModelSetting(camera_value, setting);

  auto camera_setting = castNode<CameraSettingNode>(s);
  // Attributes
  {
    const auto camera_type = toString(camera_value, keyword::cameraType);
    const CameraType type =
        (camera_type == keyword::pinholeCamera)
            ? CameraType::kPinhole
            : CameraType::kThinLens;
    camera_setting->setCameraType(type);
  }
  {
    const auto jittering = toBool(camera_value, keyword::jittering);
    camera_setting->setJittering(jittering);
  }
  switch (camera_setting->cameraType()) {
   case CameraType::kPinhole: {
    auto& parameters = camera_setting->pinholeCameraParameters();
    {
      const auto angle_of_view = toFloat<double>(camera_value, keyword::angleOfView);
      parameters.angle_of_view_ = angle_of_view;
    }
    break;
   }
   case CameraType::kThinLens:
   default:
    break;
  }
}

/*!
  */
void SceneValue::toEmitterModelSetting(const QJsonObject& value,
                                       SettingNodeBase* setting) noexcept
{
  auto material_setting = castNode<EmitterModelSettingNode>(setting);
  // Attributes
  const auto emitter_list = toArray(value, keyword::emitterModel);
  for (int i = 0; i < emitter_list.size(); ++i) {
    auto emitter_setting =
        castNode<EmitterSettingNode>(material_setting->addMaterial());
    const auto emitter_value = toObject(emitter_list[i]);
    {
      const auto emitter_type = toString(emitter_value, keyword::type);
      const EmitterType type = EmitterType::kNonDirectional;
      emitter_setting->setEmitterType(type);
    }
    {
      const auto name = toString(emitter_value, keyword::name);
      emitter_setting->setName(name.toStdString());
    }

    switch (emitter_setting->emitterType()) {
     case EmitterType::kNonDirectional: {
      auto& parameters = emitter_setting->nonDirectionalEmitterParameters();
      {
        const auto radiant_exitance = toFloat<double>(emitter_value,
                                                      keyword::radiantExitance);
        parameters.radiant_exitance_ = radiant_exitance;
      }
      {
        const auto color_index = toInt<uint32>(emitter_value,
                                               keyword::emissiveColorIndex);
        parameters.color_index_ = color_index;
      }
      break;
     }
     default: {
      zisc::raiseError("Invalid emitter type is specified.");
      break;
     }
    }
  }
}

/*!
  */
void SceneValue::toObjectModelSetting(const QJsonObject& object_value,
                                      SettingNodeBase* setting) noexcept
{
  auto object_setting = castNode<ObjectModelSettingNode>(setting);
  // Attributes
  {
    const auto name = toString(object_value, keyword::name);
    object_setting->setName(name.toStdString());
  }
  {
    const auto visibility = toBool(object_value, keyword::enabled);
    object_setting->setVisibility(visibility);
  }
  // Transformation
  const auto transformation_list = toArray(object_value, keyword::transformation);
  for (int i = 0; i < transformation_list.size(); ++i) {
    const auto transformation_value = toObject(transformation_list[i]);
    const auto transformation_type = toString(transformation_value, keyword::type);
    TransformationType type = TransformationType::kTranslation;
    double x = 0.0,
           y = 0.0,
           z = 0.0;
    if (transformation_type == keyword::translation) {
      type = TransformationType::kTranslation;
      const auto value = toArray(transformation_value, keyword::value);
      x = toFloat<double>(value[0]);
      y = toFloat<double>(value[1]);
      z = toFloat<double>(value[2]);
    }
    else if (transformation_type == keyword::scaling) {
      type = TransformationType::kScaling;
      const auto value = toArray(transformation_value, keyword::value);
      x = toFloat<double>(value[0]);
      y = toFloat<double>(value[1]);
      z = toFloat<double>(value[2]);
    }
    else {
      type = TransformationType::kRotation;
      double angle = toFloat<double>(transformation_value, keyword::angle);
      const auto unit = toString(transformation_value, keyword::unit);
      if (unit == keyword::degreeUnit)
        angle = zisc::toRadian(angle);
      const auto axis = toString(transformation_value, keyword::axis);
      if (axis == keyword::xAxis)
        x = angle;
      else if (axis == keyword::yAxis)
        y = angle;
      else
        z = angle;
    }
    auto s = object_setting->addTransformation(type, x, y, z);
    {
      auto transformation_setting = castNode<TransformationSettingNode>(s);
      const auto enabled = toBool(transformation_value, keyword::enabled);
      transformation_setting->setEnabled(enabled);
    }
  }
}

/*!
  */
void SceneValue::toObjectSetting(const QJsonArray& value,
                                 SettingNodeBase* setting) noexcept
{
  std::vector<SettingNodeBase*> group_list;
  group_list.reserve(16);

  // World group
  {
    auto object_setting = castNode<ObjectModelSettingNode>(setting);
    auto group_value = toObject(value[1]);
    ZISC_ASSERT(toString(group_value, keyword::type) == keyword::groupObject,
                "The object value isn't group object.");
    ZISC_ASSERT(toInt<int>(group_value, keyword::groupLevel) == 0,
                "The world group level isn't 0.");

    auto s = object_setting->setObject(ObjectType::kGroup);
    group_list.push_back(s);
    toObjectModelSetting(group_value, setting);
  }

  int group_level = 1;
  for (int i = 2; i < value.size(); ++i) {
    auto object_value = toObject(value[i]);
    // Check group level
    {
      const auto level = toInt<int>(object_value, keyword::groupLevel);
      ZISC_ASSERT(level <= group_level, "The invalid object level is found.");
      if (level < group_level) {
        const int level_diff = group_level - level;
        for (int j = 0; j < level_diff; ++j) {
          group_list.pop_back();
          --group_level;
        }
      }
    }
    ZISC_ASSERT(0 < group_list.size(), "The group list is empty.");
    auto group_setting = castNode<GroupObjectSettingNode>(group_list.back());
    auto child_object_setting =
        castNode<ObjectModelSettingNode>(group_setting->addObject());

    const auto object_type = toString(object_value, keyword::type);
    if (object_type == keyword::singleObject) {
      auto s = child_object_setting->setObject(ObjectType::kSingle);
      toSingleObjectSetting(object_value, s);
    }
    else if (object_type == keyword::groupObject) {
      auto s = child_object_setting->setObject(ObjectType::kGroup);
      group_list.push_back(s);
      ++group_level;
    }
    else {
      zisc::raiseError("Invalid object type is specified.");
    }
    toObjectModelSetting(object_value, child_object_setting);
  }
}

/*!
  */
void SceneValue::toRenderingMethodSetting(const QJsonObject& value,
                                          SettingNodeBase* setting) noexcept
{
  auto method_setting = castNode<RenderingMethodSettingNode>(setting);
  // Attributes
  const auto method_value = toObject(value, keyword::renderingMethod);
  {
    const auto epsilon = toFloat<double>(method_value, keyword::rayCastEpsilon);
    method_setting->setRayCastEpsilon(epsilon);
  }
  {
    const auto roulette_type = toString(method_value, keyword::russianRoulette);
    const RouletteType roulette =
        (roulette_type == keyword::rouletteMaxReflectance)
            ? RouletteType::kMaxWeight :
        (roulette_type == keyword::rouletteAverageReflectance)
            ? RouletteType::kAverageWeight
            : RouletteType::kPathLength;
    method_setting->setRouletteType(roulette);
  }
  {
    const auto path_length = toInt<uint32>(method_value, keyword::pathLength);
    method_setting->setRoulettePathLength(path_length);
  }
  {
    const auto rendering_method = toString(method_value, keyword::type);
    const RenderingMethodType method =
        (rendering_method == keyword::pathTracing)
            ? RenderingMethodType::kPathTracing :
        (rendering_method == keyword::lightTracing)
            ? RenderingMethodType::kLightTracing
            : RenderingMethodType::kProbabilisticPpm;
    method_setting->setMethodType(method);
  }

  auto getLightSourceSamplerType = [](const QString& light_sampler)
  {
    const LightSourceSamplerType sampler_type =
        (light_sampler == keyword::uniformLightSampler)
            ? LightSourceSamplerType::kUniform :
        (light_sampler == keyword::powerWeightedLightSampler)
            ? LightSourceSamplerType::kPowerWeighted
            : LightSourceSamplerType::kContributionWeighted;
    return sampler_type;
  };

  switch (method_setting->methodType()) {
   case RenderingMethodType::kPathTracing: {
    auto& parameters = method_setting->pathTracingParameters();
    {
      const auto light_sampler = toString(method_value, keyword::eyePathLightSampler);
      const auto sampler_type = getLightSourceSamplerType(light_sampler);
      parameters.eye_path_light_sampler_type_ = sampler_type;
    }
    break;
   }
   case RenderingMethodType::kLightTracing: {
    auto& parameters = method_setting->lightTracingParameters();
    {
      const auto light_sampler = toString(method_value, keyword::lightPathLightSampler);
      const auto sampler_type = getLightSourceSamplerType(light_sampler);
      parameters.light_path_light_sampler_type_ = sampler_type;
    }
    break;
   }
   case RenderingMethodType::kProbabilisticPpm: {
    auto& parameters = method_setting->probabilisticPpmParameters();
    {
      const uint32 num_of_photons = toInt<uint32>(method_value,
                                                  keyword::numOfPhotons);
      parameters.num_of_photons_ = num_of_photons;
    }
    {
      const uint32 k_nearest_neighbor = toInt<uint32>(method_value,
                                                      keyword::kNearestNeighbor);
      parameters.k_nearest_neighbor_ = k_nearest_neighbor;
    }
    {
      const double photon_search_radius = toFloat<double>(method_value,
                                                          keyword::photonSearchRadius);
      parameters.photon_search_radius_ = photon_search_radius;
    }
    {
      const double radius_reduction_rate = toFloat<double>(method_value,
                                                           keyword::radiusReductionRate);
      parameters.radius_reduction_rate_ = radius_reduction_rate;
    }
    {
      const auto light_sampler = toString(method_value, keyword::lightPathLightSampler);
      const auto sampler_type = getLightSourceSamplerType(light_sampler);
      parameters.light_path_light_sampler_type_ = sampler_type;
    }
    break;
   }
   default:
    break;
  }
}

/*!
  */
void SceneValue::toSceneSetting(const QJsonObject& value,
                                SettingNodeBase* setting) noexcept
{
  auto scene_setting = castNode<SceneSettingNode>(setting);
  // Attributes
  const auto scene_value = toObject(value, keyword::scene);
  {
    const auto scene_name = toString(scene_value, keyword::sceneName);
    scene_setting->setSceneName(scene_name.toStdString());
  }
  // Child nodes
  toSystemSetting(value, scene_setting->systemSettingNode());
  toRenderingMethodSetting(value, scene_setting->renderingMethodSettingNode());
  toTextureModelSetting(value, scene_setting->textureModelSettingNode());
  toSurfaceModelSetting(value, scene_setting->surfaceModelSettingNode());
  toEmitterModelSetting(value, scene_setting->emitterModelSettingNode());
  toBvhSetting(value, scene_setting->bvhSettingNode());

  const auto object_list = toArray(value, keyword::object);
  toCameraSetting(object_list, scene_setting->cameraSettingNode());
  toObjectSetting(object_list, scene_setting->objectSettingNode());
}

/*!
  */
void SceneValue::toSingleObjectSetting(const QJsonObject& object_value,
                                       SettingNodeBase* setting) noexcept
{
  auto object_setting = castNode<SingleObjectSettingNode>(setting);
  // Attributes
  {
    const auto shape_type = toString(object_value, keyword::shapeType);
    const ShapeType type =
        (shape_type == keyword::planeObject)
            ? ShapeType::kPlane
            : ShapeType::kMesh;
    object_setting->setShapeType(type);
  }
  {
    const auto surface_index = toInt<uint32>(object_value, keyword::surfaceIndex);
    object_setting->setSurfaceIndex(surface_index);
  }
  {
    const auto emitter_index = toInt<uint32>(object_value, keyword::emitterIndex);
    object_setting->setEmitterIndex(emitter_index);
  }
  {
    const auto is_emissive_object = toBool(object_value, keyword::isEmissiveObject);
    object_setting->setEmissive(is_emissive_object);
  }
  switch (object_setting->shapeType()) {
   case ShapeType::kMesh: {
    auto& parameters = object_setting->meshParameters();
    {
      const auto smoothing = toBool(object_value, keyword::smoothing);
      parameters.smoothing_ = (smoothing) ? kTrue : kFalse;
    }
    {
      const auto object_file_path = toString(object_value, keyword::objectFilePath);
      // Open a object file
      const QFileInfo file_info{object_file_path};
      if (!file_info.exists())
        qFatal("File '%s' doesn't exists.", qUtf8Printable(object_file_path));
      QFile object_file{object_file_path};
      if (!object_file.open(QIODevice::ReadOnly | QIODevice::Text))
        qFatal("File '%s' open failed.", qUtf8Printable(object_file_path));

      const auto suffix = file_info.suffix();
      switch (keyword::Fnv1aHash32::hash(suffix)) {
       case zisc::Fnv1aHash32::hash("obj"): {
        QTextStream obj_stream{&object_file};
        ObjLoader::parse(obj_stream,
                         &parameters.face_list_,
                         &parameters.vertex_list_,
                         &parameters.vnormal_list_,
                         &parameters.vuv_list_);
        break;
       }
       default: {
        qFatal("MeshError: '%s' isn't supported object format",
               qUtf8Printable(object_file_path));
       }
      }
    }
    break;
   }
   case ShapeType::kPlane:
   default:
    break;
  }
}

/*!
  */
void SceneValue::toSpectraSetting(const QJsonObject& spectra_value,
                                  SettingNodeBase* setting) noexcept
{
  auto spectra_setting = castNode<SpectraSettingNode>(setting);
  {
    const auto color_mode = toString(spectra_value, keyword::colorMode);
    const ColorRepresentationType type = (color_mode == keyword::rgb)
        ? ColorRepresentationType::kRgb
        : ColorRepresentationType::kSpectra; 
    spectra_setting->setRepresentationType(type);
  }
  switch (spectra_setting->representationType()) {
   case ColorRepresentationType::kRgb: {
    auto& parameters = spectra_setting->rgbParameters();
    {
      const auto rgb = toArray(spectra_value, keyword::value);
      {
        const double r = toFloat<double>(rgb[0]);
        parameters.rgb_[0] = r;
      }
      {
        const double g = toFloat<double>(rgb[1]);
        parameters.rgb_[1] = g;
      }
      {
        const double b = toFloat<double>(rgb[2]);
        parameters.rgb_[2] = b;
      }
    }
    break;
   }
   case ColorRepresentationType::kSpectra: {
    auto& parameters = spectra_setting->spectraParameters();
    {
      const auto spectra_file_path = toString(spectra_value, keyword::value);
      // Open spectra file
      {
        const QFileInfo file_info{spectra_file_path};
        if (!file_info.exists())
          qFatal("File '%s' doesn't exists.", qUtf8Printable(spectra_file_path));
      }
      QFile spectra_file{spectra_file_path};
      if (!spectra_file.open(QIODevice::ReadOnly | QIODevice::Text))
        qFatal("File '%s' open failed.", qUtf8Printable(spectra_file_path));

      QTextStream csv_text{&spectra_file};
      // Remove headers
      csv_text.readLine();
      csv_text.readLine();
      // Load distribution
      zisc::Csv<double, double> csv;
      for (auto line = csv_text.readLine(); !line.isNull(); line = csv_text.readLine())
        csv.append(line.toStdString());
      // Set distribution
      parameters.spectra_.clear();
      parameters.spectra_.reserve(csv.rowSize());
      for (uint i = 0; i < csv.rowSize(); ++i) {
        const auto& record = csv.record(i);
        parameters.spectra_.emplace_back(std::get<0>(record), std::get<1>(record));
      }
    }
    break;
   }
   default:
    break;
  }
}

/*!
  */
void SceneValue::toSurfaceModelSetting(const QJsonObject& value,
                                       SettingNodeBase* setting) noexcept
{
  auto material_setting = castNode<SurfaceModelSettingNode>(setting);
  // Attributes
  const auto surface_list = toArray(value, keyword::surfaceModel);
  for (int i = 0; i < surface_list.size(); ++i) {
    auto surface_setting =
        castNode<SurfaceSettingNode>(material_setting->addMaterial());
    const auto surface_value = toObject(surface_list[i]);
    {
      const auto surface_type = toString(surface_value, keyword::type);
      const SurfaceType type =
          (surface_type == keyword::smoothDiffuseSurface)
              ? SurfaceType::kSmoothDiffuse :
          (surface_type == keyword::smoothDielectricSurface)
              ? SurfaceType::kSmoothDielectric :
          (surface_type == keyword::smoothConductorSurface)
              ? SurfaceType::kSmoothConductor :
          (surface_type == keyword::roughDielectricSurface)
              ? SurfaceType::kRoughDielectric :
          (surface_type == keyword::roughConductorSurface)
              ? SurfaceType::kRoughConductor :
          (surface_type == keyword::layeredDiffuseSurface)
              ? SurfaceType::kLayeredDiffuse
              : SurfaceType::kCloth;
      surface_setting->setSurfaceType(type);
    }
    {
      const auto name = toString(surface_value, keyword::name);
      surface_setting->setName(name.toStdString());
    }

    switch (surface_setting->surfaceType()) {
     case SurfaceType::kSmoothDiffuse: {
      auto& parameters = surface_setting->smoothDiffuseParameters();
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::reflectanceIndex);
        parameters.reflectance_index_ = index;
      }
      break;
     }
     case SurfaceType::kSmoothDielectric: {
      auto& parameters = surface_setting->smoothDielectricParameters();
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::outerRefractiveIndex);
        parameters.outer_refractive_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::innerRefractiveIndex);
        parameters.inner_refractive_index_ = index;
      }
      break;
     }
     case SurfaceType::kSmoothConductor: {
      auto& parameters = surface_setting->smoothConductorParameters();
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::outerRefractiveIndex);
        parameters.outer_refractive_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::innerRefractiveIndex);
        parameters.inner_refractive_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::innerExtinction);
        parameters.inner_extinction_index_ = index;
      }
      break;
     }
     case SurfaceType::kRoughDielectric: {
      auto& parameters = surface_setting->roughDielectricParameters();
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::outerRefractiveIndex);
        parameters.outer_refractive_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::innerRefractiveIndex);
        parameters.inner_refractive_index_ = index;
      }
      {
        const bool anisotropic = toBool(surface_value,
                                        keyword::anisotropic);
        parameters.anisotropic_ = (anisotropic) ? kTrue : kFalse;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::roughnessXIndex);
        parameters.roughness_x_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::roughnessYIndex);
        parameters.roughness_y_index_ = index;
      }
      break;
     }
     case SurfaceType::kRoughConductor: {
      auto& parameters = surface_setting->roughConductorParameters();
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::outerRefractiveIndex);
        parameters.outer_refractive_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::innerRefractiveIndex);
        parameters.inner_refractive_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::innerExtinction);
        parameters.inner_extinction_index_ = index;
      }
      {
        const bool anisotropic = toBool(surface_value,
                                        keyword::anisotropic);
        parameters.anisotropic_ = (anisotropic) ? kTrue : kFalse;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::roughnessXIndex);
        parameters.roughness_x_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::roughnessYIndex);
        parameters.roughness_y_index_ = index;
      }
      break;
     }
     case SurfaceType::kLayeredDiffuse: {
      auto& parameters = surface_setting->layeredDiffuseParameters();
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::outerRefractiveIndex);
        parameters.outer_refractive_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::innerRefractiveIndex);
        parameters.inner_refractive_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::reflectanceIndex);
        parameters.reflectance_index_ = index;
      }
      {
        const bool anisotropic = toBool(surface_value,
                                        keyword::anisotropic);
        parameters.anisotropic_ = (anisotropic) ? kTrue : kFalse;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::roughnessXIndex);
        parameters.roughness_x_index_ = index;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::roughnessYIndex);
        parameters.roughness_y_index_ = index;
      }
      break;
     }
     case SurfaceType::kCloth: {
      auto& parameters = surface_setting->clothParameters();
      {
        const auto refractive_index = toFloat<double>(surface_value,
                                                keyword::fabricRefractiveIndex);
        parameters.coefficients_[0] = refractive_index;

        const auto isotropic_scattering = toFloat<double>(surface_value,
                                       keyword::isotropicScatteringCoefficient);
        parameters.coefficients_[1] = isotropic_scattering;

        const auto surface_gaussian_width = toFloat<double>(surface_value,
                                      keyword::surfaceReflectanceGaussianWidth);
        parameters.coefficients_[2] = surface_gaussian_width;

        const auto volume_gaussian_width = toFloat<double>(surface_value,
                                       keyword::volumeReflectanceGaussianWidth);
        parameters.coefficients_[3] = volume_gaussian_width;

        const auto bandwidth = toFloat<double>(surface_value,
                                               keyword::bandwidthParameter);
        parameters.coefficients_[4] = bandwidth;
      }
      {
        const auto index = toInt<uint32>(surface_value,
                                         keyword::reflectanceIndex);
        parameters.reflectance_index_ = index;
      }
      break;
     }
     default: {
      zisc::raiseError("Invalid surface type is specified.");
      break;
     }
    }
  }
}


/*!
  */
void SceneValue::toSystemSetting(const QJsonObject& value,
                                 SettingNodeBase* setting) noexcept
{
  auto system_setting = castNode<SystemSettingNode>(setting);
  // Attributes
  const auto system_value = toObject(value, keyword::system);
  {
    const auto num_of_threads = toInt<uint32>(system_value,
                                              keyword::numOfThreads);
    system_setting->setNumOfThreads(num_of_threads);
  }
  {
    const auto random_seed = toInt<uint32>(system_value,
                                           keyword::randomSeed);
    system_setting->setRandomSeed(random_seed);
  }
  {
    const uint32 termination_time = toInt<uint32>(system_value,
                                                  keyword::terminationTime);
    system_setting->setTerminationTime(termination_time);
  }
  {
    const auto termination_cycle = toInt<uint32>(system_value,
                                                 keyword::terminationCycle);
    system_setting->setTerminationCycle(termination_cycle);
  }
  {
    //! \todo rename keyword
    const auto saving_interval_time = toInt<uint32>(system_value,
                                                    keyword::savingIntervalTime);
    system_setting->setSavingIntervalTime(saving_interval_time);
  }
  {
    const uint32 saving_interval_cycle = toInt<uint32>(system_value,
                                                       keyword::savingIntervalCycle);
    system_setting->setSavingIntervalCycle(saving_interval_cycle);
  }
  {
    const auto image_resolution = toArray(system_value,
                                          keyword::imageResolution);
    const auto width = toInt<uint32>(image_resolution[0]);
    const auto height = toInt<uint32>(image_resolution[1]);
    system_setting->setImageResolution(width, height);
  }
  {
    const auto power2_cycle_saving = toBool(system_value,
                                            keyword::power2CycleSaving);
    system_setting->setPower2CycleSaving(power2_cycle_saving);
  }

  const auto color_value = toObject(value, keyword::color);
  {
    const auto color_mode = toString(color_value, keyword::colorMode);
    const RenderingColorMode mode = 
        (color_mode == keyword::rgb) ? RenderingColorMode::kRgb
                                     : RenderingColorMode::kSpectra;
    system_setting->setColorMode(mode);
  }
  {
    const auto wavelength_sampler_type = toString(color_value,
                                                  keyword::wavelengthSampling);
    const WavelengthSamplerType sampler_type =
        (wavelength_sampler_type == keyword::regularSampling)
            ? WavelengthSamplerType::kRegular :
        (wavelength_sampler_type == keyword::randomSampling)
            ? WavelengthSamplerType::kRandom
            : WavelengthSamplerType::kStratified;
    system_setting->setWavelengthSamplerType(sampler_type);
  }
  {
    const auto color_space = toString(color_value,
                                      keyword::colorSpace);
    const ColorSpaceType space =
        (color_space == keyword::sRgbD65)
            ? ColorSpaceType::kSRgbD65 :
        (color_space == keyword::sRgbD50)
            ? ColorSpaceType::kSRgbD50 :
        (color_space == keyword::adobeRgbD65)
            ? ColorSpaceType::kAdobeRgbD65
            : ColorSpaceType::kAdobeRgbD50;
    system_setting->setColorSpace(space);
  }
  {
    const auto gamma = toFloat<double>(color_value, keyword::gamma);
    system_setting->setGammaCorrection(gamma);
  }
  {
    const auto tone_mapping_type = toString(color_value, keyword::toneMapping);
    const ToneMappingType tone_mapping =
        (tone_mapping_type == keyword::reinhard)
            ? ToneMappingType::kReinhard :
        (tone_mapping_type == keyword::filmic)
            ? ToneMappingType::kFilmic
            : ToneMappingType::kUncharted2Filmic;
    system_setting->setToneMappingType(tone_mapping);
  }
  {
    const auto exposure = toFloat<double>(color_value, keyword::exposure);
    system_setting->setExposure(exposure);
  }
}

/*!
  */
void SceneValue::toTextureModelSetting(const QJsonObject& value,
                                       SettingNodeBase* setting) noexcept
{
  auto material_setting = castNode<TextureModelSettingNode>(setting);
  // Attributes
  const auto texture_list = toArray(value, keyword::textureModel);
  for (int i = 0; i < texture_list.size(); ++i) {
    auto texture_setting =
        castNode<TextureSettingNode>(material_setting->addMaterial());
    const auto texture_value = toObject(texture_list[i]);
    {
      const auto texture_type = toString(texture_value, keyword::type);
      const TextureType type =
          (texture_type == keyword::valueTexture)
              ? TextureType::kValue :
          (texture_type == keyword::unicolorTexture)
              ? TextureType::kUnicolor :
          (texture_type == keyword::checkerboardTexture)
              ? TextureType::kCheckerboard
              : TextureType::kImage;
      texture_setting->setTextureType(type);
    }
    {
      const auto name = toString(texture_value, keyword::name);
      texture_setting->setName(name.toStdString());
    }

    switch (texture_setting->textureType()) {
     case TextureType::kValue: {
      auto& parameters = texture_setting->valueTextureParameters();
      {
        const auto v = toFloat<double>(texture_value, keyword::value);
        parameters.value_ = v;
      }
      break;
     }
     case TextureType::kUnicolor: {
      auto& parameters = texture_setting->unicolorTextureParameters();
      {
        const auto color_value = toObject(texture_value, keyword::color);
        toSpectraSetting(color_value, parameters.color_.get());
      }
      break;
     }
     case TextureType::kCheckerboard: {
      auto& parameters = texture_setting->checkerboardTextureParameters();
      {
        const auto color_value = toObject(texture_value, keyword::color1);
        toSpectraSetting(color_value, parameters.color_[0].get());
      }
      {
        const auto color_value = toObject(texture_value, keyword::color2);
        toSpectraSetting(color_value, parameters.color_[1].get());
      }
      {
        const auto resolution = toArray(texture_value, keyword::imageResolution);
        parameters.resolution_[0] = toInt<uint32>(resolution[0]);
        parameters.resolution_[1] = toInt<uint32>(resolution[1]);
      }
      break;
     }
     case TextureType::kImage: {
      auto& parameters = texture_setting->imageTextureParameters();
      {
        // Get the image path
        const auto image_file_path = toString(texture_value, keyword::imageFilePath);
        {
          const QFileInfo file_info{image_file_path};
          if (!file_info.exists())
            qFatal("File '%s' doesn't exists.", qUtf8Printable(image_file_path));
        }
        // Load the image
        QImage image{image_file_path};
        if (image.isNull())
          qFatal("File '%s' open failed.", qUtf8Printable(image_file_path));
        // Copy the image to parameter
        parameters.image_.setResolution(zisc::cast<uint>(image.width()),
                                        zisc::cast<uint>(image.height()));
        for (uint y = 0; y < parameters.image_.heightResolution(); ++y) {
          for (uint x = 0; x < parameters.image_.widthResolution(); ++x) {
            auto& rgba32 = parameters.image_.get(x, y);
            rgba32.setRowData(image.pixelColor(x, y).rgba());
          }
        }
      }
      break;
     }
     default: {
      zisc::raiseError("Invalid texture type is specified.");
      break;
     }
    }
  }
}

} // namespace nanairo
