/*!
  \file nanairo.js
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */


.pragma library

// Definitions

// Nanairo files
var nanairoFileFormat = "@nanairoFileFormat@";
var sceneBackupFileName = "@sceneBackupFileName@";
var previewDir = "@previewDir@";
// General
var name = "@name@";
var type = "@type@";
var enabled = "@enabled@";
var value = "@value@";
var width = "@width@";
var height = "@height@";
var delta = "@delta@";
var epsilon = "@epsilon@";
// Scene
var scene = "@scene@";
var sceneName = "@sceneName@";
// System
var system = "@system@";
var numOfThreads = "@numOfThreads@";
var randomSeed = "@randomSeed@";
var terminationCycle = "@terminationCycle@";
var imageResolution = "@imageResolution@";
var power2CycleSaving = "@power2CycleSaving@";
var savingInterval = "@savingInterval@";
// Color
var color = "@color@";
var color1 = "@color1@";
var color2 = "@color2@";
var colorMode = "@colorMode@";
    var rgb = "@rgb@";
    var spectra = "@spectra@";
var standardObserver = "@standardObserver@";
    var cie2Deg = "@cie2Deg@";
    var cie10Deg = "@cie10Deg@";
var standardIllumination = "@standardIllumination@";
    var cieD65 = "@cieD65@";
    var cieA = "@cieA@";
var wavelengthSampling = "@wavelengthSampling@";
    var regularSampling = "@regularSampling@";
    var randomSampling = "@randomSampling@";
    var stratifiedSampling = "@stratifiedSampling@";
    var lightsBasedSampling = "@lightsBasedSampling@";
var colorSpace = "@colorSpace@";
    var sRgbD65 = "@sRgbD65@";
    var sRgbD50 = "@sRgbD50@";
    var adobeRgbD65 = "@adobeRgbD65@";
    var adobeRgbD50 = "@adobeRgbD50@";
var gamma = "@gamma@";
var toneMapping = "@toneMapping@";
    var exposure = "@exposure@";
    var reinhard = "@reinhard@";
    var modifiedReinhard = "@modifiedReinhard@";
    var filmic = "@filmic@";
    var uncharted2Filmic = "@uncharted2Filmic@";
// Rendering
var renderingMethod = "@renderingMethod@";
    var pathTracing = "@pathTracing@";
    var lightTracing = "@lightTracing@";
    var probabilisticPpm = "@probabilisticPpm@";
var rayCastEpsilon = "@rayCastEpsilon@";
var russianRoulette = "@russianRoulette@";
    var rouletteMaxReflectance = "@rouletteMaxReflectance@";
    var rouletteAverageReflectance = "@rouletteAverageReflectance@";
    var roulettePathLength = "@roulettePathLength@";
var pathLength = "@pathLength@";
// Probabilistic PPM
var numOfPhotons = "@numOfPhotons@";
var photonSearchRadius = "@photonSearchRadius@";
var radiusReductionRate = "@radiusReductionRate@";
var kNearestNeighbor = "@kNearestNeighbor@";
var glossyPhotonMap = "@glossyPhotonMap@";
var kernelType = "@kernelType@";
    var noKernel = "@noKernel@";
    var coneKernel = "@coneKernel@";
    var perlinKernel = "@perlinKernel@";
// BVH
var bvh = "@bvh@";
    var binaryRadixTreeBvh = "@binaryRadixTreeBvh@";
    var approximateAgglomerativeClusteringBvh = "@approximateAgglomerativeClusteringBvh@";
    var agglomerativeTreeletRestructuringBvh = "@agglomerativeTreeletRestructuringBvh@";
        var treeletSize = "@treeletSize@";
        var optimizationLoopCount = "@optimizationLoopCount@";
// Texture
var textureModel = "@textureModel@";
    var valueTexture = "@valueTexture@";
    var unicolorTexture = "@unicolorTexture@";
    var checkerboardTexture = "@checkerboardTexture@";
    var imageTexture = "@imageTexture@";
        var imageFilePath = "@imageFilePath@";
// SurfaceModel
var surfaceModel = "@surfaceModel@";
    var smoothDiffuseSurface = "@smoothDiffuseSurface@";
        var reflectanceIndex = "@reflectanceIndex@";
    var smoothDielectricSurface = "@smoothDielectricSurface@";
        var outerRefractiveIndex = "@outerRefractiveIndex@";
        var innerRefractiveIndex = "@innerRefractiveIndex@";
        var innerExtinction = "@innerExtinction@";
    var smoothConductorSurface = "@smoothConductorSurface@";
    var roughDielectricSurface = "@roughDielectricSurface@";
        var roughnessIndex = "@roughnessIndex@";
    var roughConductorSurface = "@roughConductorSurface@";
    var layeredDiffuseSurface = "@layeredDiffuseSurface@";
    var clothSurface = "@clothSurface@";
        var fabricRefractiveIndex = "@fabricRefractiveIndex@";
        var isotropicScatteringCoefficient = "@isotropicScatteringCoefficient@";
        var surfaceReflectanceGaussianWidth = "@surfaceReflectanceGaussianWidth@";
        var volumeReflectanceGaussianWidth = "@volumeReflectanceGaussianWidth@";
        var bandwidthParameter = "@bandwidthParameter@";
// EmitterModel
var emitterModel = "@emitterModel@";
    var nonDirectionalEmitter = "@nonDirectionalEmitter@";
        var emissiveColorIndex = "@emissiveColorIndex@";
        var radiantExitance = "@radiantExitance@";
// Object
var object = "@object@";
    var cameraObject = "@cameraObject@";
    var singleObject = "@singleObject@";
    var groupObject = "@groupObject@";
var groupLevel = "@groupLevel@";
// Camera
var cameraType = "@cameraType@";
    var pinholeCamera = "@pinholeCamera@";
var jittering = "@jittering@";
// PinholeCamera
var angleOfView = "@angleOfView@";
// SingleObject
var shapeType = "@shapeType@";
    var planeObject = "@planeObject@";
    var meshObject = "@meshObject@";
var objectFilePath = "@objectFilePath@";
var smoothing = "@smoothing@";
var surfaceIndex = "@surfaceIndex@";
var isEmissiveObject = "@isEmissiveObject@";
var emitterIndex = "@emitterIndex@";
// Transformation
var transformation = "@transformation@";
    var xAxis = "@xAxis@";
    var yAxis = "@yAxis@";
    var zAxis = "@zAxis@";
    var translation = "@translation@";
    var scaling = "@scaling@";
    var rotation = "@rotation@";
        var axis = "@axis@";
        var angle = "@angle@";
        var unit = "@unit@";
        var degreeUnit = "@degreeUnit@";
        var radianUnit = "@radianUnit@";
// Other
var ctrlKey = "@ctrlKey@";
var expanded = "expanded";
var visible = "visible";

// Global variables

// Window
var mainWindowWidth = 960;
var mainWindowHeight = 540;
var defaultBlockWidth = 160;
var defaultBlockHeight = 64;
var backgroundColor = "#EBEBEB";
var title = "Nanairo";
// Preset scenes
var defaultScene = "DefaultScene";
var cornellBoxScene = "CornellBox";
var jensenCornellBoxScene = "JensenCornellBox";
var causticsTestScene = "CausticsTest";
var dispersionTestScene = "DispersionTest";
var veachMisTestScene = "VeachMisTest";
var veachBdptTestScene = "VeachBdptTest";
var materialTestScene = "MaterialTest";
var smoothDiffuseTestScene = "SurfaceTest-SmoothDiffuse";
var smoothDielectricTestScene = "SurfaceTest-SmoothDielectric";
var smoothConductorTestScene = "SurfaceTest-SmoothConductor";
var roughDielectricTestScene = "SurfaceTest-RoughDielectric";
var roughConductorTestScene = "SurfaceTest-RoughConductor";
var layeredDiffuseTestScene = "SurfaceTest-LayeredDiffuse";
var clothTestScene = "SurfaceTest-Cloth";
var colorCheckerSpectrumScene = "ColorChecker-Spectrum";
var colorCheckerSRgbScene = "ColorChecker-sRGB";
var raytracingCamp4Scene = "RaytracingCamp4";
// Tab item
var tabItemWidth = 960;
var tabItemHeight = 494;
// Setting block
var itemOffset = 4;
var blockOffset = 16;
var defaultComboboxWidth = 120;
var defaultComboboxHeight = 16;
var defaultSpinboxWidth = 120;
var defaultSpinboxHeight = 20;
var defaultButtonWidth = 50;
var defaultButtonHeight = 16;
var defaultCheckboxheight = 16;
var defaultSwitchWidth = 120;;
var defaultSwitchHeight = 16;
var defaultTextFieldWidth = 140;
var defaultTextFieldHeight = 20;
var defaultColorViewWidth = 140;
var defaultColorViewHeight = 20;
// System
var integer32Max = 2147483647;
// Color
var defaultDarkerScale = 1.2;
var defaultLighterScale = 0.83;
// RenderingMethod
var renderingTabItemWidth = 768;
var renderingTabItemHeight = 494;
// BVH
var bvhTabItemWidth = renderingTabItemWidth;
var bvhTabItemHeight = renderingTabItemHeight;
// Object
var objectTreeWidth = 256;
var objectTreeItemMargin = 2;
var objectTreeItemWidth = objectTreeWidth - 2 * objectTreeItemMargin;
var objectTreeItemHeight = 16;
var objectTreeItemButtonSize = 16;
var treeViewMargin = 8;
var objectTreeViewWidth = objectTreeWidth + 2 * treeViewMargin;
var objectTreeViewHeight = tabItemHeight;
var objectSettingViewWidth = tabItemWidth - objectTreeViewWidth;
var objectSettingViewHeight = tabItemHeight;
var defaultImageButtonSize = 24;
// Object item
var defaultIndentWidth = 4;
var objectSettingTabWidth = 688;
var objectSettingTabHeight = 468;
var cameraTabItemWidth = 496;
var cameraTabItemHeight = 468;
// Transformation
var transformationListWidth = 444;
var transformationListMargin = treeViewMargin;
var transformationItemMargin = 2;
var transformationItemWidth = transformationListWidth - 2 * transformationItemMargin;
var transformationItemHeight = 48;
var transformationMaxValue = 10000.0;
// Material
var materialListWidth = 160;
var materialViewMargin = 8;
var materialListItemMargin = 2;
var materialListItemWidth = materialListWidth - 2 * materialListItemMargin;
var materialListItemHeight = 16;
var materialListViewWidth = materialListWidth + 2 * materialViewMargin;
var materialSettingViewWidth = 592;
var materialSettingViewHeight = 494;

// Global functions

function mergeDataList(dataList) {
  var sceneData = {};
  for (var i = 0; i < dataList.length; ++i) {
    var data = dataList[i];
    for (var key in data) {
      sceneData[key] = data[key];
    }
  }
  return sceneData;
}
