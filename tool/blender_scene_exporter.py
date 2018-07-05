#
# \file blender_scene_exporter.py
# \author Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

import argparse
import bpy
import copy
import math
import json
import os

def makeArgParser():
  parser = argparse.ArgumentParser(
      description="Export a blender scene setting for Nanairo")

  parser.add_argument(
      '--scene',
      action='store',
      nargs=1,
      default="Scene",
      type=str,
      help="A scene name to be exported.")

  parser.add_argument(
      '--camera',
      action='store',
      nargs=1,
      default="Camera",
      type=str,
      help="An active camera name.")

  parser.add_argument(
      '--resourcedir',
      action='store',
      nargs=1,
      default="resources/scene",
      type=str,
      help="Resource directory")

  parser.add_argument(
      '--meshincludetransformation',
      action='store_true',
      help="Mesh data includes transformation info.")

  return parser 

# We assume that a blend file has only one scene

def getBlenderScene(args):
  assert bpy.data.scenes.find(args.scene) != -1, \
      "Blender doesn't have '{0}' scene.".format(args.scene)
  return bpy.data.scenes[args.scene]

def checkBlenderHasObject(args, obj_name):
  scene = getBlenderScene(args)
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
  print("Scene name: {0}.".format(scene_name))
  scene_data["SceneName"] = scene_name

  return scene_data

def getSystemSettings(args):
  scene_data = dict()

  scene = getBlenderScene(args)

  scene_data["NumOfThreads"] = 4
  scene_data["SamplerType"] = "Correlated Multi-Jittered"
  scene_data["SamplerSeed"] = scene.cycles.seed
  scene_data["ImageResolution"] = [scene.render.resolution_x,
                                   scene.render.resolution_y]
  scene_data["SavingIntervalCycle"] = 0
  scene_data["SavingIntervalTime"] = 3600000
  scene_data["TerminationCycle"] = scene.cycles.samples
  scene_data["TerminationTime"] = 0
  scene_data["Power2CycleSaving"] = True

  return scene_data

def getColorSettings(args):
  scene_data = dict()

  scene = getBlenderScene(args)

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

def takeTransformationSettings(args, obj, is_camera_object = False):
  transformation_list = list()
  if args.meshincludetransformation and (not is_camera_object):
    return transformation_list

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

def calcNanairoAngleOfView(args, angle_of_view):
  scene = getBlenderScene(args)

  tan_phi = scene.render.resolution_y / scene.render.resolution_x
  tan_theta = math.tan(0.5 * angle_of_view) * math.sqrt(1.0 + tan_phi * tan_phi)
  theta = 2.0 * math.atan(tan_theta)
  return theta 

def getCameraSettings(args):
  camera_data = dict()

  scene = getBlenderScene(args)
  camera_obj = scene.camera

  camera_data["GroupLevel"] = 0
  camera_data["Type"] = "CameraObject"
  camera_data["Enabled"] = True
  camera_data["Name"] = "Camera"
  camera_data["CameraType"] = "PinholeCamera"
  camera_data["AngleOfView"] = \
      toNanaFloat(math.degrees(calcNanairoAngleOfView(args, camera_obj.data.angle)))
  camera_data["Jittering"] = True
  camera_data["Transformation"] = takeTransformationSettings(args, camera_obj, True)

  return camera_data 

def makeWorldSettings():
  world_data = dict()

  world_data["GroupLevel"] = 0
  world_data["Type"] = "GroupObject"
  world_data["Enabled"] = True
  world_data["Name"] = "World"
  world_data["Transformation"] = list()

  return world_data

def calcVolumeOfBoundBox(obj):
  bound = obj.bound_box
  min_point = bound[0]
  max_point = bound[-1]
  x = math.fabs(max_point[0] - min_point[0])
  y = math.fabs(max_point[1] - min_point[1])
  z = math.fabs(max_point[2] - min_point[2])
  volume = 2.0 * (x * y + x * z + y * z)
  return volume

def hasBody(args, obj):
  has_body = (obj.type != 'EMPTY') and (0.0 < calcVolumeOfBoundBox(obj))
  if has_body:
    if obj.type == 'MESH':
      has_body = 0 < len(obj.data.polygons)
    elif obj.type == 'CURVE': 
      scene = getBlenderScene(args)
      mesh = obj.to_mesh(scene, True, 'RENDER')
      has_body = 0 < len(mesh.polygons)

  return has_body

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

def setSceneObjectSettings(args, object_list, obj, depth = 1):
  num_of_objects = 0
  if not (obj.type in {'MESH', 'CURVE', 'EMPTY'}):
    return num_of_objects

  obj_data = dict()

  has_body = hasBody(args, obj)
  has_child = 0 < len(obj.children)
  if (not has_body) and (not has_child):
    return num_of_objects

  obj_data["GroupLevel"] = depth
  obj_data["Type"] = "GroupObject" if has_child else "SingleObject"
  obj_data["Enabled"] = not obj.hide_render
  obj_data["Name"] = obj.name
  obj_data["Transformation"] = takeTransformationSettings(args, obj)

  # Single object settings
  if has_body:
    if has_child:
      object_list.append(obj_data)
      obj_data = copy.deepcopy(obj_data)
      obj_data["GroupLevel"] = depth + 1
      obj_data["Type"] = "SingleObject"
      obj_data["Transformation"] = list()

    has_material = obj.active_material != None
    assert has_material, "Error: object '{0}' hasn't material.".format(obj.name)

    obj_file_name = obj.name + ".obj"
    exportObject(obj, obj_file_name)

    obj_data["ShapeType"] = "MeshObject"
    obj_data["ObjectFilePath"] = args.resourcedir + "/" + getBlendFileName() + "/" + obj_file_name
    obj_data["Smoothing"] = False
    surface_index, emitter_index, is_emissive_object = getMaterialInfo(obj)
    obj_data["SurfaceIndex"] = surface_index
    obj_data["EmitterIndex"] = emitter_index
    obj_data["IsEmissiveObject"] = is_emissive_object
    num_of_objects += 1

  object_list.append(obj_data)

  for child_obj in obj.children:
    num_of_objects += setSceneObjectSettings(args, object_list, child_obj, depth + 1)

  if num_of_objects == 0:
    del object_list[-1]
  elif has_child:
    num_of_objects += 1

  return num_of_objects

def getObjectSettings(args):
  object_list = list()

  object_list.append(getCameraSettings(args))
  object_list.append(makeWorldSettings())

  # Process a top level objects
  scene = getBlenderScene(args)
  for obj in scene.objects:
    if obj.parent == None:
      setSceneObjectSettings(args, object_list, obj)

  return object_list 

def getBvhSettings():
  scene_data = dict()

  scene_data["Type"] = "BinaryRadixTreeBvh"

  return scene_data

def getSceneSettings(args):
  scene_data = dict()

  scene_data["Scene"] = getTagSettings()
  scene_data["System"] = getSystemSettings(args)
  scene_data["Color"] = getColorSettings(args)
  scene_data["RenderingMethod"] = getRenderingMethodSettings()
  scene_data["TextureModel"] = getTextureModelSettings()
  scene_data["SurfaceModel"] = getSurfaceModelSettings()
  scene_data["EmitterModel"] = getEmitterModelSettings()
  scene_data["Object"] = getObjectSettings(args)
  scene_data["Bvh"] = getBvhSettings()

  return scene_data

# Encode a scene data into JSON and save as a nana file
def exportSceneSettingsAsNana(scene_data):
  file_name = getBlendFileName() + ".nana"
  nana_file = open(file_name, 'w')
  json.dump(scene_data, nana_file, allow_nan=False, indent=4)

def exportImages(args):
  scene = getBlenderScene(args)
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
      assert False, "Error: image '{0}' is unsupported file format '{1}".format(image.name, image.file_format)
    if path[-len(extension):] != extension:
      path = path + extension
    image.filepath_raw = path
    image.save()

if __name__ == "__main__":
  arg_parser = makeArgParser()
  args, unknown = arg_parser.parse_known_args()

  bpy.ops.object.select_all(action='DESELECT')
  scene_data = getSceneSettings(args)
  exportSceneSettingsAsNana(scene_data)
  exportImages(args)
