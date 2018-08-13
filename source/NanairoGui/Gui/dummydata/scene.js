/*!
  \file scene.js
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

.pragma library

function getCornellBoxScene() {
  var sceneData =
{
    "@bvh@": {
        "@type@": "@binaryRadixTreeBvh@"
    },
    "@color@": {
        "@colorMode@": "@spectra@",
        "@colorSpace@": "@sRgbD50@",
        "@exposure@": 2,
        "@gamma@": 2.4,
        "@toneMapping@": "@filmic@",
        "@wavelengthSampling@": "@randomSampling@",
        "@enableDenoising@": true,
        "@denoiserType@": "@bayesianCollaborativeDenoiser@",
        "@histogramBins@": 10,
        "@histogramDistanceThreshold@": 2.0,
        "@patchRadius@": 3,
        "@searchWindowRadius@": 13,
        "@numberOfScales@":5 
    },
    "@emitterModel@": [
        {
            "@emissiveColorIndex@": 3,
            "@name@": "Emitter",
            "@radiantExitance@": 512,
            "@type@": "@nonDirectionalEmitter@"
        }
    ],
    "@object@": [
        {
            "@angleOfView@": 60,
            "@cameraType@": "@pinholeCamera@",
            "@enabled@": true,
            "@groupLevel@": 0,
            "@jittering@": true,
            "@name@": "@camera@",
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        0,
                        -5.8,
                        0
                    ]
                }
            ],
            "@type@": "@cameraObject@"
        },
        {
            "@enabled@": true,
            "@groupLevel@": 0,
            "@name@": "@world@",
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        0,
                        0,
                        0
                    ]
                }
            ],
            "@type@": "@groupObject@"
        },
        {
            "@emitterIndex@": 0,
            "@enabled@": true,
            "@groupLevel@": 1,
            "@isEmissiveObject@": false,
            "@name@": "Floor",
            "@shapeType@": "@planeObject@",
            "@surfaceIndex@": 0,
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        3,
                        4.5,
                        1
                    ]
                },
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        0,
                        0.75,
                        -1.5
                    ]
                }
            ],
            "@type@": "@singleObject@"
        },
        {
            "@emitterIndex@": 0,
            "@enabled@": true,
            "@groupLevel@": 1,
            "@isEmissiveObject@": false,
            "@name@": "LeftWall",
            "@shapeType@": "@planeObject@",
            "@surfaceIndex@": 2,
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        3,
                        4.5,
                        1
                    ]
                },
                {
                    "@angle@": 90,
                    "@axis@": "@yAxis@",
                    "@enabled@": true,
                    "@type@": "@rotation@",
                    "@unit@": "@degreeUnit@"
                },
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        -1.5,
                        0.75,
                        0
                    ]
                }
            ],
            "@type@": "@singleObject@"
        },
        {
            "@emitterIndex@": 0,
            "@enabled@": true,
            "@groupLevel@": 1,
            "@isEmissiveObject@": false,
            "@name@": "RightWall",
            "@shapeType@": "@planeObject@",
            "@surfaceIndex@": 1,
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        3,
                        4.5,
                        1
                    ]
                },
                {
                    "@angle@": -90,
                    "@axis@": "@yAxis@",
                    "@enabled@": true,
                    "@type@": "@rotation@",
                    "@unit@": "@degreeUnit@"
                },
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        1.5,
                        0.75,
                        0
                    ]
                }
            ],
            "@type@": "@singleObject@"
        },
        {
            "@emitterIndex@": 0,
            "@enabled@": true,
            "@groupLevel@": 1,
            "@isEmissiveObject@": false,
            "@name@": "BackWall",
            "@shapeType@": "@planeObject@",
            "@surfaceIndex@": 0,
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        3,
                        3,
                        1
                    ]
                },
                {
                    "@angle@": 90,
                    "@axis@": "@xAxis@",
                    "@enabled@": true,
                    "@type@": "@rotation@",
                    "@unit@": "@degreeUnit@"
                },
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        0,
                        3,
                        0
                    ]
                }
            ],
            "@type@": "@singleObject@"
        },
        {
            "@emitterIndex@": 0,
            "@enabled@": true,
            "@groupLevel@": 1,
            "@isEmissiveObject@": false,
            "@name@": "Ceiling",
            "@shapeType@": "@planeObject@",
            "@surfaceIndex@": 0,
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        3,
                        4.5,
                        1
                    ]
                },
                {
                    "@angle@": 180,
                    "@axis@": "@xAxis@",
                    "@enabled@": true,
                    "@type@": "@rotation@",
                    "@unit@": "@degreeUnit@"
                },
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        0,
                        0.75,
                        1.5
                    ]
                }
            ],
            "@type@": "@singleObject@"
        },
        {
            "@emitterIndex@": 0,
            "@enabled@": true,
            "@groupLevel@": 1,
            "@isEmissiveObject@": false,
            "@name@": "TallBlock",
            "@objectFilePath@": "resources/models/Primitives/Cube.obj",
            "@shapeType@": "@meshObject@",
            "@smoothing@": false,
            "@surfaceIndex@": 0,
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        1,
                        1,
                        2
                    ]
                },
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        0.9,
                        0.9,
                        0.9
                    ]
                },
                {
                    "@angle@": 15,
                    "@axis@": "@zAxis@",
                    "@enabled@": true,
                    "@type@": "@rotation@",
                    "@unit@": "@degreeUnit@"
                },
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        -0.5,
                        1,
                        -1.5
                    ]
                }
            ],
            "@type@": "@singleObject@"
        },
        {
            "@emitterIndex@": 0,
            "@enabled@": true,
            "@groupLevel@": 1,
            "@isEmissiveObject@": false,
            "@name@": "ShortBlock",
            "@objectFilePath@": "resources/models/Primitives/Cube.obj",
            "@shapeType@": "@meshObject@",
            "@smoothing@": false,
            "@surfaceIndex@": 0,
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        0.9,
                        0.9,
                        0.9
                    ]
                },
                {
                    "@angle@": -15,
                    "@axis@": "@zAxis@",
                    "@enabled@": true,
                    "@type@": "@rotation@",
                    "@unit@": "@degreeUnit@"
                },
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        0.5,
                        -0.4,
                        -1.5
                    ]
                }
            ],
            "@type@": "@singleObject@"
        },
        {
            "@emitterIndex@": 0,
            "@enabled@": true,
            "@groupLevel@": 1,
            "@isEmissiveObject@": true,
            "@name@": "Light",
            "@shapeType@": "@planeObject@",
            "@surfaceIndex@": 0,
            "@transformation@": [
                {
                    "@enabled@": true,
                    "@type@": "@scaling@",
                    "@value@": [
                        0.75,
                        0.75,
                        1
                    ]
                },
                {
                    "@angle@": 180,
                    "@axis@": "@xAxis@",
                    "@enabled@": true,
                    "@type@": "@rotation@",
                    "@unit@": "@degreeUnit@"
                },
                {
                    "@enabled@": true,
                    "@type@": "@translation@",
                    "@value@": [
                        0,
                        0.75,
                        1.49
                    ]
                }
            ],
            "@type@": "@singleObject@"
        }
    ],
    "@renderingMethod@": {
        "@eyePathLightSampler@": "@powerWeightedLightSampler@",
        "@pathLength@": 3,
        "@rayCastEpsilon@": 1e-07,
        "@russianRoulette@": "@rouletteMaxReflectance@",
        "@type@": "@pathTracing@"
    },
    "@scene@": {
        "@sceneName@": "CornellBox"
    },
    "@surfaceModel@": [
        {
            "@name@": "WhiteWallSurface",
            "@reflectanceIndex@": 0,
            "@type@": "@smoothDiffuseSurface@"
        },
        {
            "@name@": "GreenWallSurface",
            "@reflectanceIndex@": 1,
            "@type@": "@smoothDiffuseSurface@"
        },
        {
            "@name@": "RedWallSurface",
            "@reflectanceIndex@": 2,
            "@type@": "@smoothDiffuseSurface@"
        }
    ],
    "@system@": {
        "@imageResolution@": [
            1280,
            720 
        ],
        "@numOfThreads@": 4,
        "@power2CycleSaving@": true,
        "@samplerType@": "@cmjSampler@",
        "@samplerSeed@": 123456789,
        "@savingIntervalCycle@": 1,
        "@savingIntervalTime@": 10000,
        "@terminationCycle@": 1024,
        "@terminationTime@": 1 
    },
    "@textureModel@": [
        {
            "@color@": {
                "@colorMode@": "@spectra@",
                "@value@": "resources/spectrum/reflectance/cornell_box_white.csv"
            },
            "@name@": "WhiteTexture",
            "@type@": "@unicolorTexture@"
        },
        {
            "@color@": {
                "@colorMode@": "@spectra@",
                "@value@": "resources/spectrum/reflectance/cornell_box_green.csv"
            },
            "@name@": "GreenTexture",
            "@type@": "@unicolorTexture@"
        },
        {
            "@color@": {
                "@colorMode@": "@spectra@",
                "@value@": "resources/spectrum/reflectance/cornell_box_red.csv"
            },
            "@name@": "RedTexture",
            "@type@": "@unicolorTexture@"
        },
        {
            "@color@": {
                "@colorMode@": "@spectra@",
                "@value@": "resources/spectrum/illuminant/cornell_box_light.csv"
            },
            "@name@": "EmitterTexture",
            "@type@": "@unicolorTexture@"
        }
    ]
};
  return sceneData;
}
