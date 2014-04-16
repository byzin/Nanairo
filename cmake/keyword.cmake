# file: keyword.cmake
# author: Sho Ikeda
# 
# Copyright (c) 2015 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


cmake_minimum_required(VERSION 3.0)


# Functions and macros

# Set reflect keywords
macro(setNanairoKeywords)
  set(nanairo_keyword_list 
  # Value keywords
  # System
  png "PNG"
  bmp "BMP"
  ppm "PPM"
  openClSingleGpu "Single GPU"
  openClMultipleGpus "Multiple GPUs"
  openClCpu "CPU"
  # Color
  cie2Deg "CIE 2deg color matching function"
  cie10Deg "CIE 10deg color matching function"
  cieD65 "CIE D65"
  cieA "CIE A"
  regularSampling "Regular sampling"
  randomSampling "Random sampling"
  stratifiedSampling "Stratified sampling"
  lightsBasedImportanceSampling "Lights based importance sampling"
  sRgbD65 "sRGB (D65)"
  sRgbD50 "sRGB (D50)"
  adobeRgbD65 "Adobe RGB (D65)"
  adobeRgbD50 "Adobe RGB (D50)"
  reinhard "Reinhard"
  modifiedReinhard "Modified reinhard"
  filmic "Filmic"
  uncharted2Filmic "Uncharted2 filmic"
  # Rendering
  pathTracing "PathTracing"
  lightTracing "LightTracing"
  probabilisticPpm "ProbabilisticPPM"
  rouletteMaxReflectance "Reflectance (Max)"
  rouletteAverageReflectance "Reflectance (Average)"
  roulettePathLength "Path length"
  # BVH
  binaryRadixTreeBvh "BinaryRadixTreeBvh"
  approximateAgglomerativeClusteringBvh "ApproximateAgglomerativeClusteringBvh"
  agglomerativeTreeletRestructuringBvh "AgglomerativeTreeletRestructuringBvh"
  # Texture
  valueTexture "ValueTexture"
  unicolorTexture "UnicolorTexture"
  checkerboardTexture "CheckerboardTexture"
  imageTexture "ImageTexture"
  # SurfaceModel
  smoothDiffuseSurface "SmoothDiffuseSurface"
  smoothDielectricSurface "SmoothDielectricSurface"
  smoothConductorSurface "SmoothConductorSurface"
  roughDielectricSurface "RoughDielectricSurface"
  roughConductorSurface "RoughConductorSurface"
  roughPlasticSurface "RoughPlasticSurface"
  # EmitterModel
  nonDirectionalEmitter "NonDirectionalEmitter"
  # Object
  cameraObject "CameraObject"
  singleObject "SingleObject"
  groupObject "GroupObject"
  # Camera
  pinholeCamera "PinholeCamera"
  # Single object
  planeObject "PlaneObject"
  meshObject "MeshObject"
  # Transformation
  translation "Translation"
  scaling "Scaling"
  rotation "Rotation"
  # Rotation
  x "X"
  y "Y"
  z "Z"
  degreeUnit "Degree"
  radianUnit "Radian"

  # Setting keywords
  # General
  nanairoFileFormat "nana"
  name "Name"
  count "Count"
  type "Type"
  delta "Delta"
  epsilon "Epsilon"
  value "Value"
  width "Width"
  height "Height"
  # System
  system "System"
  sceneName "SceneName"
  numOfThreads "NumOfThreads"
  randomSeed "RandomSeed"
  terminationPass "TerminationPass"
  imageWidthResolution "ImageWidthResolution"
  imageHeightResolution "ImageHeightResolution"
  enableToSaveSpectraImage "EnableToSaveSpectraImage"
  ldrImageFormat "LdrImageFormat"
  power2Saving "Power2Saving"
  savingInterval "SavingInterval"
  openClRendering "OpenClRendering"
  openClType "OpenClType"
  # Color
  color "Color"
  rgbRendering "RgbRendering"
  standardObserver "StandardObserver"
  standardIllumination "StandardIllumination"
  wavelengthSampling "WavelengthSampling"
  colorSpace "ColorSpace"
  gamma "Gamma"
  toneMapping "ToneMapping"
  # Rendering
  renderingMethod "RenderingMethod"
  renderingMethodType "RenderingMethodType"
  rayCastEpsilon "RayCastEpsilon"
  russianRoulette "RussianRoulette"
  pathLength "PathLength"
  # Probabilistic PPM
  numOfPhotons "NumOfPhotons"
  photonSearchRadius "PhotonSearchRadius"
  radiusReductionRate "RadiusReductionRate"
  kNearestNeighbor "KNearestNeighbor"
  glossyPhotonMap "GlossyPhotonMap"
  kernelType "KernelType"
  # Photon map kernel
  noKernel "NoKernel"
  coneKernel "ConeKernel"
  perlinKernel "PerlinKernel"
  # BVH
  bvh "Bvh"
  bvhType "BvhType"
  treeletSize "TreeletSize"
  optimizationLoopCount "OptimizationLoopCount"
  # Color palette
  isRgbMode "IsRgbMode"
  rgbColor "RgbColor"
  spectraFilePath "SpectraFilePath"
  # Texture
  texture "Texture"
  textureColor "TextureColor"
  imageFilePath "ImageFilePath"
  # SurfaceModel
  surfaceModel "SurfaceModel"
  reflectanceIndex "ReflectanceIndex"
  outerRefractiveIndex "OuterRefractiveIndex"
  innerRefractiveIndex "InnerRefractiveIndex"
  innerExtinction "InnerExtinction"
  diffuseColorIndex "DiffuseColorIndex"
  roughnessIndex "RoughnessIndex"
  # EmitterModel
  emitterModel "EmitterModel"
  radiantExitance "RadiantExitance"
  # Object
  object "Object"
  treeLevel "treeLevel"
  visibility "Visibility"
  # Camera
  cameraType "CameraType"
  jittering "Jittering"
  # PinholeCamera
  angleOfView "AngleOfView"
  # SingleObject
  geometryType "GeometryType"
  objectFilePath "ObjectFilePath"
  smoothing "Smoothing"
  surfaceIndex "SurfaceIndex"
  isEmissiveObject "IsEmissiveObject"
  emitterIndex "EmitterIndex"
  # Transformation
  transformation "Transformation"
  axis "Axis"
  angle "Angle"
  unit "Unit"
  active "Active"
  )
  if (z_mac)
    list(APPEND nanairo_keyword_list ctrlKey "Meta")
  else()
    list(APPEND nanairo_keyword_list ctrlKey "Ctrl")
  endif()
endmacro(setNanairoKeywords)
