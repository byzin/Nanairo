/*!
  \file definitions.js
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

.pragma library

// Definitions

// Nanairo
var projectName = "@PROJECT_NAME@";
var nanairoFileFormat = "@nanairoFileFormat@";

// Menu
var ctrlKey = "@ctrlKey@";

// General
var name = "@name@";
var type = "@type@";
var enabled = "@enabled@";
var value = "@value@";
var width = "@width@";
var height = "@height@";
var delta = "@delta@";
var epsilon = "@epsilon@";

// Tag
var scene = "@scene@";
    var sceneName = "@sceneName@";

// System
var system = "@system@";
var numOfThreads = "@numOfThreads@";
var randomSeed = "@randomSeed@";
var terminationCycle = "@terminationCycle@";
var terminationTime = "@terminationTime@";
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
var wavelengthSampling = "@wavelengthSampling@";
    var regularSampling = "@regularSampling@";
    var randomSampling = "@randomSampling@";
    var stratifiedSampling = "@stratifiedSampling@";
var colorSpace = "@colorSpace@";
    var sRgbD65 = "@sRgbD65@";
    var sRgbD50 = "@sRgbD50@";
    var adobeRgbD65 = "@adobeRgbD65@";
    var adobeRgbD50 = "@adobeRgbD50@";
var gamma = "@gamma@";
var toneMapping = "@toneMapping@";
    var exposure = "@exposure@";
    var reinhard = "@reinhard@";
    var filmic = "@filmic@";
    var uncharted2Filmic = "@uncharted2Filmic@";

// Rendering method
var renderingMethod = "@renderingMethod@";
    var pathTracing = "@pathTracing@";
    var lightTracing = "@lightTracing@";
    var probabilisticPpm = "@probabilisticPpm@";
        var numOfPhotons = "@numOfPhotons@";
        var photonSearchRadius = "@photonSearchRadius@";
        var radiusReductionRate = "@radiusReductionRate@";
        var kNearestNeighbor = "@kNearestNeighbor@";
        var kernelType = "@kernelType@";
            var noKernel = "@noKernel@";
            var coneKernel = "@coneKernel@";
            var perlinKernel = "@perlinKernel@";
var rayCastEpsilon = "@rayCastEpsilon@";
var russianRoulette = "@russianRoulette@";
    var rouletteMaxReflectance = "@rouletteMaxReflectance@";
    var rouletteAverageReflectance = "@rouletteAverageReflectance@";
    var roulettePathLength = "@roulettePathLength@";
        var pathLength = "@pathLength@";
var lightPathLightSampler = "@lightPathLightSampler@";
var eyePathLightSampler = "@eyePathLightSampler@";
    var uniformLightSampler = "@uniformLightSampler@";
    var powerWeightedLightSampler = "@powerWeightedLightSampler@";
    var contributionWeightedLightSampler = "@contributionWeightedLightSampler@";

// Texture
var textureModel = "@textureModel@";
    var valueTexture = "@valueTexture@";
    var unicolorTexture = "@unicolorTexture@";
    var checkerboardTexture = "@checkerboardTexture@";
    var imageTexture = "@imageTexture@";
        var imageFilePath = "@imageFilePath@";

// Surface
var surfaceModel = "@surfaceModel@";
    var smoothDiffuseSurface = "@smoothDiffuseSurface@";
        var reflectanceIndex = "@reflectanceIndex@";
    var smoothDielectricSurface = "@smoothDielectricSurface@";
        var outerRefractiveIndex = "@outerRefractiveIndex@";
        var innerRefractiveIndex = "@innerRefractiveIndex@";
    var smoothConductorSurface = "@smoothConductorSurface@";
        var innerExtinction = "@innerExtinction@";
    var roughDielectricSurface = "@roughDielectricSurface@";
        var roughnessIndex = "@roughnessIndex@";
    var roughConductorSurface = "@roughConductorSurface@";
    var layeredDiffuseSurface = "@layeredDiffuseSurface@";

// Emitter
var emitterModel = "@emitterModel@";
    var nonDirectionalEmitter = "@nonDirectionalEmitter@";
        var emissiveColorIndex = "@emissiveColorIndex@";
        var radiantExitance = "@radiantExitance@";

// Object
// object type
var object = "@object@";
    var cameraObject = "@cameraObject@";
    var singleObject = "@singleObject@";
    var groupObject = "@groupObject@";
var groupLevel = "@groupLevel@";

// Camera
var cameraType = "@cameraType@";
    var pinholeCamera = "@pinholeCamera@";
var jittering = "@jittering@";

// Pinhole camera
var angleOfView = "@angleOfView@";

// Single object
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
    var translation = "@translation@";
    var scaling = "@scaling@";
    var rotation = "@rotation@";
    var axis = "@axis@";
        var xAxis = "@xAxis@";
        var yAxis = "@yAxis@";
        var zAxis = "@zAxis@";
    var angle = "@angle@";
    var unit = "@unit@";
        var degreeUnit = "@degreeUnit@";
        var radianUnit = "@radianUnit@";

// BVH
var bvh = "@bvh@";
    var binaryRadixTreeBvh = "@binaryRadixTreeBvh@";
    var agglomerativeTreeletRestructuringBvh = "@agglomerativeTreeletRestructuringBvh@";
        var treeletSize = "@treeletSize@";
        var optimizationLoopCount = "@optimizationLoopCount@";

// Global variables

var defaultItemSpace = 4;
var defaultBlockSize = 16;
var defaultButtonHeight = 32;
var defaultImageButtonSize = 24;
var defaultDarkerScale = 1.2;
var defaultTextFontSize = 12;
var defaultSettingItemWidth = 128;
var defaultSettingItemHeight = 24;
var defaultSettingItemColumnInterval = 64;

// Window
var mainWindowWidth = 960;
var mainWindowHeight = 540;
var defaultMenuHeight = 24;
var defaultTabHeight = 32;
var settingViewWidth = mainWindowWidth;
var settingViewHeight = mainWindowHeight - (defaultMenuHeight + defaultTabHeight);

// Preset scenes
var defaultScene = "DefaultScene";
var cornellBoxScene = "CornellBox";
var jensenCornellBoxScene = "JensenCornellBox";
var causticsTestScene = "CausticsTest";
var dispersionTestScene = "DispersionTest";
var veachMisTestScene = "VeachMisTest";
var veachBdptTestScene = "VeachBdptTest";
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
var privateRoomScene = "PrivateRoom";

// Object
var objectTreeWidth = 256;
var objectInfoSettingViewWidth = mainWindowWidth - (objectTreeWidth + 3 * defaultItemSpace);

// ListModel
// Object model
var modelGroupLevelKey = "grouplevel";
var modelEnabledKey = "enabled";
var modelExpandedKey = "expanded";
var modelVisibleIndexKey = "visibleindex";
var modelNameKey = "name";
var modelTypeKey = "type";
// Transformation model
var modelTransformationKey = "transformation";
var modelAxisKey = "axis";
var modelXAxisKey = "x";
var modelYAxisKey = "y";
var modelZAxisKey = "z";
var modelAngleKey = "angle";
var modelUnitKey = "unit";

// SpinBox
var intMax = Math.pow(2, 31) - 1;

// Functions

function getProperty(item, propertyName) {
  var isUndefined = typeof(item[propertyName]) == "undefined";
  console.assert(!isUndefined, "The item doesn't have '" + propertyName + "' property.");
  return item[propertyName];
}

function setProperty(item, propertyName, value) {
  var isUndefined = typeof(item[propertyName]) == "undefined";
  item[propertyName] = value;
}

function copyObject(item) {
  var newItem = {};
  for (var key in item)
    newItem[key] = item[key];
  return newItem;
}

function copyColor(color) {
  var newColor = Qt.rgba(color.r, color.g, color.b, color.a);
  return newColor;
}

function isInBounds(x, lower, upper) {
  var result = (lower <= x) && (x < upper);
  return result;
}
