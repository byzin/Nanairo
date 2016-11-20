# file: keyword.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015-2016 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

# Gtt Nanairo keywords
function(getNanairoKeywords nanairo_keyword_list)
  set(keyword_list 
      # Nanairo files
      nanairoFileFormat "nana"
      sceneBackupFileName "settings.nana"
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
      imageResolution "ImageResolution"
#      enableToSaveSpectraImage "EnableToSaveSpectraImage"
      power2CycleSaving "Power2CycleSaving"
      savingInterval "SavingInterval"

      # Color
      color "Color"
      color1 "Color1"
      color2 "Color2"
      colorMode "ColorMode"
          rgb "RGB"
          spectra "Spectra"
      standardObserver "StandardObserver"
          cie2Deg "CIE 2deg color matching function"
          cie10Deg "CIE 10deg color matching function"
      standardIllumination "StandardIllumination"
          cieD65 "CIE D65"
          cieA "CIE A"
      wavelengthSampling "WavelengthSampling"
          regularSampling "Regular sampling"
          randomSampling "Random sampling"
          stratifiedSampling "Stratified sampling"
          lightsBasedImportanceSampling "Lights based importance sampling"
      colorSpace "ColorSpace"
          sRgbD65 "sRGB (D65)"
          sRgbD50 "sRGB (D50)"
          adobeRgbD65 "Adobe RGB (D65)"
          adobeRgbD50 "Adobe RGB (D50)"
      gamma "Gamma"
      toneMapping "ToneMapping"
          exposure "Exposure"
          reinhard "Reinhard"
          modifiedReinhard "Modified reinhard"
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
      # Probabilistic PPM
      numOfPhotons "NumOfPhotons"
      photonSearchRadius "PhotonSearchRadius"
      radiusReductionRate "RadiusReductionRate"
      kNearestNeighbor "KNearestNeighbor"
      glossyPhotonMap "GlossyPhotonMap"
      kernelType "KernelType"
          noKernel "NoKernel"
          coneKernel "ConeKernel"
          perlinKernel "PerlinKernel"

      # BVH
      bvh "Bvh"
          binaryRadixTreeBvh "BinaryRadixTreeBvh"
          approximateAgglomerativeClusteringBvh "ApproximateAgglomerativeClusteringBvh"
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
  if(Z_IS_MAC)
    list(APPEND nanairo_keyword_list ctrlKey "Meta")
  else()
    list(APPEND nanairo_keyword_list ctrlKey "Ctrl")
  endif()
  set(${nanairo_keyword_list} ${keyword_list} PARENT_SCOPE)
endfunction(getNanairoKeywords)
