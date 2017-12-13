# file: keyword.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015-2017 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


# Gtt Nanairo keywords
function(getNanairoKeywords nanairo_keyword_list)
  set(keyword_list 
      # Nanairo files
      nanairoFileFormat "nana"
      sceneBackupFileName "settings.nana"
      sceneBinaryFileName "settings.nanabin"
      previewDir "Preview"

      name "Name"
      # General
      type "Type"
      enabled "Enabled"
      value "Value"
      width "Width"
      height "Height"
      delta "Delta"
      epsilon "Epsilon"

      # Scene
      scene "Scene"
      sceneName "SceneName"

      # System
      system "System"
      numOfThreads "NumOfThreads"
      randomSeed "RandomSeed"
      terminationCycle "TerminationCycle"
      terminationTime "TerminationTime"
      imageResolution "ImageResolution"
#      enableToSaveSpectraImage "EnableToSaveSpectraImage"
      power2CycleSaving "Power2CycleSaving"
      savingIntervalTime "SavingIntervalTime"
      savingIntervalCycle "SavingIntervalCycle"

      # Color
      color "Color"
      color1 "Color1"
      color2 "Color2"
      colorMode "ColorMode"
          rgb "RGB"
          spectra "Spectra"
      wavelengthSampling "WavelengthSampling"
          regularSampling "Regular sampling"
          randomSampling "Random sampling"
          stratifiedSampling "Stratified sampling"
          lightsBasedSampling "Lights based sampling"
      colorSpace "ColorSpace"
          sRgbD65 "sRGB (D65)"
          sRgbD50 "sRGB (D50)"
          adobeRgbD65 "Adobe RGB (D65)"
          adobeRgbD50 "Adobe RGB (D50)"
      gamma "Gamma"
      toneMapping "ToneMapping"
          exposure "Exposure"
          reinhard "Reinhard"
          filmic "Filmic"
          uncharted2Filmic "Uncharted2 filmic"

      # Rendering
      renderingMethod "RenderingMethod"
          pathTracing "PathTracing"
          lightTracing "LightTracing"
          probabilisticPpm "ProbabilisticPPM"
      rayCastEpsilon "RayCastEpsilon"
      russianRoulette "RussianRoulette"
          rouletteMaxReflectance "Reflectance (Max)"
          rouletteAverageReflectance "Reflectance (Average)"
          roulettePathLength "Path length"
      pathLength "PathLength"
      lightPathLightSampler "LightPathLightSampler"
      eyePathLightSampler "EyePathLightSampler"
          uniformLightSampler "UniformLightSampler"
          powerWeightedLightSampler "PowerWeightedLightSampler"
          contributionWeightedLightSampler "ContributionWeightedLightSampler"
              lightSamplerMaxSurfaceSplit "LightSamplerMaxSurfaceSplit"
              lightSamplerNumOfPhotons "LightSamplerNumOfPhotons"
      # Probabilistic PPM
      numOfPhotons "NumOfPhotons"
      photonSearchRadius "PhotonSearchRadius"
      radiusReductionRate "RadiusReductionRate"
      kNearestNeighbor "KNearestNeighbor"
      kernelType "KernelType"
          noKernel "NoKernel"
          coneKernel "ConeKernel"
          perlinKernel "PerlinKernel"

      # BVH
      bvh "Bvh"
          binaryRadixTreeBvh "BinaryRadixTreeBvh"
          agglomerativeTreeletRestructuringBvh "AgglomerativeTreeletRestructuringBvh"
              treeletSize "TreeletSize"
              optimizationLoopCount "OptimizationLoopCount"

      # Texture
      textureModel "TextureModel"
          valueTexture "ValueTexture"
          unicolorTexture "UnicolorTexture"
          checkerboardTexture "CheckerboardTexture"
          imageTexture "ImageTexture"
              imageFilePath "ImageFilePath"

      # SurfaceModel
      surfaceModel "SurfaceModel"
          smoothDiffuseSurface "SmoothDiffuseSurface"
              reflectanceIndex "ReflectanceIndex"
          smoothDielectricSurface "SmoothDielectricSurface"
              outerRefractiveIndex "OuterRefractiveIndex"
              innerRefractiveIndex "InnerRefractiveIndex"
              innerExtinction "InnerExtinction"
          smoothConductorSurface "SmoothConductorSurface"
          roughDielectricSurface "RoughDielectricSurface"
              roughnessIndex "RoughnessIndex"
          roughConductorSurface "RoughConductorSurface"
          layeredDiffuseSurface "LayeredDiffuseSurface"
          clothSurface "ClothSurface"
              fabricRefractiveIndex "FabricRefractiveIndex"
              isotropicScatteringCoefficient "IsotropicScatteringCoefficient"
              surfaceReflectanceGaussianWidth "SurfaceReflectanceGaussianWidth"
              volumeReflectanceGaussianWidth "VolumeReflectanceGaussianWidth"
              bandwidthParameter "bandwidthParameter"

      # EmitterModel
      emitterModel "EmitterModel"
          nonDirectionalEmitter "NonDirectionalEmitter"
              emissiveColorIndex "EmissiveColorIndex"
              radiantExitance "RadiantExitance"

      # Object
      object "Object"
          cameraObject "CameraObject"
          singleObject "SingleObject"
          groupObject "GroupObject"
      groupLevel "GroupLevel"
      # Camera
      cameraType "CameraType"
          pinholeCamera "PinholeCamera"
      jittering "Jittering"
      # PinholeCamera
      angleOfView "AngleOfView"
      # SingleObject
      shapeType "ShapeType"
          planeObject "PlaneObject"
          meshObject "MeshObject"
      objectFilePath "ObjectFilePath"
      smoothing "Smoothing"
      surfaceIndex "SurfaceIndex"
      isEmissiveObject "IsEmissiveObject"
      emitterIndex "EmitterIndex"
      # Transformation
      transformation "Transformation"
          translation "Translation"
              xAxis "X Axis"
              yAxis "Y Axis"
              zAxis "Z Axis"
          scaling "Scaling"
          rotation "Rotation"
              axis "Axis"
              angle "Angle"
              unit "Unit"
              degreeUnit "Degree"
              radianUnit "Radian"
      )
  if(Z_MAC)
    list(APPEND nanairo_keyword_list ctrlKey "Meta")
  else()
    list(APPEND nanairo_keyword_list ctrlKey "Ctrl")
  endif()
  set(${nanairo_keyword_list} ${keyword_list} PARENT_SCOPE)
endfunction(getNanairoKeywords)
