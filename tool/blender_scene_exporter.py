#
# \file blender_scene_exporter.py
# \author Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

import bpy
import copy
import math
import json
import os

# Constant values
kResourceDir = "resources/scene"
kSceneName = "Scene"
kCameraName = "Camera"

# We assume that a blend file has only one scene

def getBlenderScene():
  assert bpy.data.scenes.find(kSceneName) != -1, \
      "Blender doesn't have '" + kSceneName + "' scene."
  return bpy.data.scenes[kSceneName]

def checkBlenderHasObject(obj_name):
  scene = getBlenderScene()
  assert obj_name in scene.objects, "'" + obj_name + "' object not found."

def toNanaFloat(value, round_digits = 4):
  v = round(value, round_digits)
  return v

def getBlendFileName():
  file_name = bpy.path.display_name_from_filepath(bpy.data.filepath)
  return file_name

def getTagSettings():
  scene_data = dict()

  # Get scene name
  scene_name = getBlendFileName()
  print("Scene name: " + scene_name)
  scene_data["SceneName"] = scene_name

  return scene_data

def getSystemSettings():
  scene_data = dict()

  scene = getBlenderScene()

  scene_data["NumOfThreads"] = 4
  scene_data["RandomSeed"] = scene.cycles.seed
  scene_data["ImageResolution"] = [scene.render.resolution_x,
                                   scene.render.resolution_y]
  scene_data["SavingIntervalCycle"] = 0
  scene_data["SavingIntervalTime"] = 3600000
  scene_data["TerminationCycle"] = scene.cycles.samples
  scene_data["TerminationTime"] = 0
  scene_data["Power2CycleSaving"] = True

  return scene_data

def getColorSettings():
  scene_data = dict()

  scene = getBlenderScene()

  scene_data["ColorMode"] = "RGB"
  scene_data["ColorSpace"] = "sRGB (D65)"
  scene_data["Exposure"] = toNanaFloat(scene.cycles.film_exposure)
  scene_data["Gamma"] = toNanaFloat(2.2)
  scene_data["ToneMapping"] = "Reinhard"
  scene_data["WavelengthSampling"] = "Regular sampling"

  return scene_data

def getRenderingMethodSettings():
  scene_data = dict()

  # Path tracing
  scene_data["Type"] = "PathTracing"
  scene_data["EyePathLightSampler"] = "PowerWeightedLightSampler"
  scene_data["RussianRoulette"] = "Reflectance (Max)"
  scene_data["PathLength"] = 3
  scene_data["RayCastEpsilon"] = toNanaFloat(1e-07, 7)

  return scene_data

def getTextureModelSettings():
  texture_data = dict() 

  texture_data["Name"] = "DefaultTexture"
  texture_data["Type"] = "ValueTexture"
  texture_data["Value"] = toNanaFloat(0.25)

  return [texture_data]

def getSurfaceModelSettings():
  surface_list = list()

  for material in bpy.data.materials:
    surface_data = dict()
    surface_data["Name"] = material.name
    surface_data["Type"] = "SmoothDiffuseSurface"
    surface_data["ReflectanceIndex"] = 0
    surface_list.append(surface_data)

  return surface_list

def getEmitterModelSettings():
  emitter_data = dict()

  emitter_data["Name"] = "DefaultEmitter"
  emitter_data["Type"] = "NonDirectionalEmitter"
  emitter_data["EmissiveColorIndex"] = 0
  emitter_data["RadiantExitance"] = toNanaFloat(128.0)

  return [emitter_data]

def takeTransformationSettings(obj, is_camera_object = False):
  transformation_list = list()

  # Scaling
  scale = [toNanaFloat(obj.scale[0]), toNanaFloat(obj.scale[1]), toNanaFloat(obj.scale[2])]
  if (scale[0] != 1.0) or (scale[1] != 1.0) or (scale[2] != 1.0):
    scaling_setting = dict()
    scaling_setting["Type"] = "Scaling"
    scaling_setting["Enabled"] = True
    scaling_setting["Value"] = scale
    if not is_camera_object:
      transformation_list.append(scaling_setting)
    # Clear scaling info
    obj.scale = [1.0, 1.0, 1.0]

  # Rotation
  for axis in obj.rotation_euler.order:
    axis_index = ord(axis) - ord('X')
    axis_name = axis + " Axis"
    angle = toNanaFloat(math.degrees(obj.rotation_euler[axis_index]))
    if is_camera_object and (axis == 'X'):
      # Assume that the rotation order of camera is 'XYZ'
      angle = angle - 90.0
    if angle != 0.0:
      rotation_setting = dict()
      rotation_setting["Type"] = "Rotation"
      rotation_setting["Enabled"] = True
      rotation_setting["Axis"] = axis_name
      rotation_setting["Angle"] = angle
      rotation_setting["Unit"] = "Degree"
      transformation_list.append(rotation_setting)
      # Clear rotation info
      obj.rotation_euler[axis_index] = 0.0

  # Transformation
  location = [toNanaFloat(obj.location[0]), toNanaFloat(obj.location[1]), toNanaFloat(obj.location[2])]
  if (location[0] != 0.0) or (location[1] != 0.0) or (location[2] != 0.0):
    translation_setting = dict()
    translation_setting["Type"] = "Translation"
    translation_setting["Enabled"] = True
    translation_setting["Value"] = location
    transformation_list.append(translation_setting)
    # Clear translation info
    obj.location = [0.0, 0.0, 0.0]

  return transformation_list

def calcNanairoAngleOfView(angle_of_view):
  scene = getBlenderScene()

  tan_phi = scene.render.resolution_y / scene.render.resolution_x
  tan_theta = math.tan(0.5 * angle_of_view) * math.sqrt(1.0 + tan_phi * tan_phi)
  theta = 2.0 * math.atan(tan_theta)
  return theta 

def getCameraSettings():
  camera_data = dict()

  scene = getBlenderScene()
  camera_obj = scene.camera

  camera_data["GroupLevel"] = 0
  camera_data["Type"] = "CameraObject"
  camera_data["Enabled"] = True
  camera_data["Name"] = "Camera"
  camera_data["CameraType"] = "PinholeCamera"
  camera_data["AngleOfView"] = \
      toNanaFloat(math.degrees(calcNanairoAngleOfView(camera_obj.data.angle)))
  camera_data["Jittering"] = True
  camera_data["Transformation"] = takeTransformationSettings(camera_obj, True)

  return camera_data 

def makeWorldSettings():
  world_data = dict()

  world_data["GroupLevel"] = 0
  world_data["Type"] = "GroupObject"
  world_data["Enabled"] = True
  world_data["Name"] = "World"
  world_data["Transformation"] = list()

  return world_data

def exportObject(obj, obj_file_name):
  obj.select = True

  bpy.ops.export_scene.obj( \
      filepath = obj_file_name, \
      check_existing = True, \
      axis_forward = 'Y', \
      axis_up = 'Z', \
      use_selection = True, \
      use_animation = False, \
      use_mesh_modifiers = True, \
      use_edges = False, \
      use_smooth_groups = False, \
      use_smooth_groups_bitflags = False, \
      use_normals = True, \
      use_uvs = True, \
      use_materials = False, \
      use_triangles = True, \
      use_nurbs = False, \
      use_vertex_groups = False, \
      use_blen_objects = True, \
      group_by_object = False, \
      group_by_material = False, \
      keep_vertex_order = True, \
      global_scale = 1.0, \
      path_mode = 'AUTO')

  obj.select = False

def getMaterialInfo(obj):
  material_index = bpy.data.materials.find(obj.active_material.name)
  return (material_index, 0, False)

def setSceneObjectSettings(object_list, obj, depth = 1):
  if not (obj.type in {'MESH', 'EMPTY'}):
    return 

  obj_data = dict()

  has_child = 0 < len(obj.children)

  obj_data["GroupLevel"] = depth
  obj_data["Type"] = "GroupObject" if has_child else "SingleObject"
  obj_data["Enabled"] = not obj.hide_render
  obj_data["Name"] = obj.name
  obj_data["Transformation"] = takeTransformationSettings(obj)

  # Single object settings
  if obj.type == 'MESH':
    if has_child:
      object_list.append(obj_data)
      obj_data = copy.deepcopy(obj_data)
      obj_data["GroupLevel"] = depth + 1
      obj_data["Type"] = "SingleObject"
      obj_data["Transformation"] = list()

    if (0 < len(obj.data.polygons)) and (obj.active_material != None):
      obj_file_name = obj.name + ".obj"
      exportObject(obj, obj_file_name)

      obj_data["ShapeType"] = "MeshObject"
      obj_data["ObjectFilePath"] = kResourceDir + "/" + getBlendFileName() + "/" + obj_file_name
      obj_data["Smoothing"] = False
      surface_index, emitter_index, is_emissive_object = getMaterialInfo(obj)
      obj_data["SurfaceIndex"] = surface_index
      obj_data["EmitterIndex"] = emitter_index
      obj_data["IsEmissiveObject"] = is_emissive_object
    else:
      obj_data = None

  if obj_data != None:
    object_list.append(obj_data)

  for child_obj in obj.children:
    setSceneObjectSettings(object_list, child_obj, depth + 1)

def getObjectSettings():
  object_list = list()

  object_list.append(getCameraSettings())
  object_list.append(makeWorldSettings())

  # Process a top level objects
  scene = getBlenderScene()
  for obj in scene.objects:
    if obj.parent == None:
      setSceneObjectSettings(object_list, obj)

  return object_list 

def getBvhSettings():
  scene_data = dict()

  scene_data["Type"] = "BinaryRadixTreeBvh"

  return scene_data

def getSceneSettings():
  scene_data = dict()

  scene_data["Scene"] = getTagSettings()
  scene_data["System"] = getSystemSettings()
  scene_data["Color"] = getColorSettings()
  scene_data["RenderingMethod"] = getRenderingMethodSettings()
  scene_data["TextureModel"] = getTextureModelSettings()
  scene_data["SurfaceModel"] = getSurfaceModelSettings()
  scene_data["EmitterModel"] = getEmitterModelSettings()
  scene_data["Object"] = getObjectSettings()
  scene_data["Bvh"] = getBvhSettings()

  return scene_data

# Encode a scene data into JSON and save as a nana file
def exportSceneSettingsAsNana(scene_data):
  file_name = getBlendFileName() + ".nana"
  nana_file = open(file_name, 'w')
  json.dump(scene_data, nana_file, allow_nan=False, indent=4)

def exportImages():
  scene = getBlenderScene()
  for image in bpy.data.images:
    if image.type != 'IMAGE':
      continue
    path = os.path.join(os.getcwd(), image.name)
    if image.file_format == 'PNG':
      extension = ".png"
    elif image.file_format == 'JPEG':
      extension = ".jpg"
    elif image.file_format == 'TARGA':
      extension = ".tga"
    else:
      assert False, image.name + ": unsupported file format '" + image.file_format + "'"
    if path[-len(extension):] != extension:
      path = path + extension
    image.filepath_raw = path
    image.save()

if __name__ == "__main__":
  bpy.ops.object.select_all(action='DESELECT')
  scene_data = getSceneSettings()
  exportSceneSettingsAsNana(scene_data)
  exportImages()
