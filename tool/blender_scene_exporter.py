#
# \file blender_scene_exporter.py
# \author Sho Ikeda
#
# Copyright (c) 2015-2019 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

import argparse
import bpy
import copy
import math
import multiprocessing 
import concurrent.futures
import json
import os

def clamp(x, minval, maxval):
  result = min(max(x, minval), maxval)
  return result

class InfoType:
  kStatus = 0
  kWarning = 1
  kError = 2
  kDebug = 3

def printInfo(t, message):
  type_string = ["Status ",
                 "Warning",
                 "Error  ",
                 "Debug  "]
  m = "[{0}] {1}".format(type_string[t], message)
  print(m)

def makeArgParser():
  parser = argparse.ArgumentParser(
      description="Export a blender scene setting for Nanairo")

  parser.add_argument(
      '--nanascene',
      action='store',
      nargs=1,
      default="",
      type=str,
      help="A scene name to be exported.")

  parser.add_argument(
      '--nanaresourcedir',
      action='store',
      nargs=1,
      default=os.path.join("resources", "scene"),
      type=str,
      help="The relative resource path that resources are exported into.")

  parser.add_argument(
      '--nananotransformation',
      action='store_true',
      help="Prevent to separate transformation info from object data.")

  parser.add_argument(
      '--nananoresource',
      action='store_true',
      help="Prevent to export resources.")


  parser.add_argument(
      '--nanathreads',
      action='store',
      nargs=1,
      default=multiprocessing.cpu_count(),
      type=int,
      help="The number of threads that are used for exporting.")

  return parser 

def getBlendFileName():
  file_name = bpy.path.display_name_from_filepath(bpy.data.filepath)
  return file_name

class NanairoSettings:
  def __init__(self, args):
    # Arguments
    self.scene_name_ = args.nanascene
    self.separate_transformation_ = not args.nananotransformation
    self.export_resources_ = not args.nananoresource
    self.resource_dir_ = os.path.join(args.nanaresourcedir, getBlendFileName())
    # Scene data
    self.scene_data_ = dict()
    # Object data
    self.object_data_list_ = dict()
    # Resources
    self.object_list_ = list()
    self.image_list_ = list()

def getBlendScene(scene_name):
  assert bpy.data.scenes.find(scene_name) != -1, \
      "Blender doesn't have \"{0}\" scene.".format(scene_name)
  return bpy.data.scenes[scene_name]

def checkBlenderRenderEngine(engine_name):
  supported_engines = ['CYCLES']
  assert engine_name in supported_engines, \
         "The render engine '{0}' isn't supported.".format(engine_name)

def toNanaFloat(value, round_digits = 4):
  v = round(value, round_digits)
  return v

def parseTagSettings(settings, executor):
  scene_data = dict()

  # Get scene name
  scene_name = getBlendFileName()
  printInfo(InfoType.kDebug, "Scene name: \"{0}\".".format(scene_name))
  scene_data["SceneName"] = scene_name

  return scene_data

def calcImageResolution(scene):
  resolution_scale = scene.render.resolution_percentage / 100.0
  resolutions = [int(scene.render.resolution_x * resolution_scale),
                 int(scene.render.resolution_y * resolution_scale)]
  resolutions[0] = clamp(resolutions[0], 256, scene.render.resolution_x)
  resolutions[1] = clamp(resolutions[1], 256, scene.render.resolution_x)
  return resolutions

def parseSystemSettings(settings, executor):
  scene_data = dict()

  scene = getBlendScene(settings.scene_name_)
  checkBlenderRenderEngine(scene.render.engine)
  printInfo(InfoType.kDebug, "Blender render engine: '{0}'.".format(scene.render.engine))

  scene_data["NumOfThreads"] = 4
  scene_data["SamplerType"] = "Correlated Multi-Jittered"
  scene_data["SamplerSeed"] = scene.cycles.seed
  resolutions = calcImageResolution(scene)
  scene_data["ImageResolution"] = resolutions
  printInfo(InfoType.kDebug, "Render resolution: {0} x {1}.".format(resolutions[0],
                                                                    resolutions[1]))
  scene_data["SavingIntervalCycle"] = 0
  scene_data["SavingIntervalTime"] = 3600000
  num_of_cycles = scene.cycles.samples
  num_of_cycles = max(num_of_cycles, 1)
  scene_data["TerminationCycle"] = num_of_cycles
  printInfo(InfoType.kDebug, "Num of render cycles: {0}.".format(num_of_cycles));
  scene_data["TerminationTime"] = 0
  scene_data["Power2CycleSaving"] = True

  return scene_data

def parseColorSettings(settings, executor):
  scene_data = dict()

  scene = getBlendScene(settings.scene_name_)

  scene_data["ColorMode"] = "RGB"
  scene_data["ColorSpace"] = "sRGB (D65)"
  scene_data["Exposure"] = toNanaFloat(scene.cycles.film_exposure)
  scene_data["Gamma"] = toNanaFloat(2.2)
  scene_data["ToneMapping"] = "Reinhard"
  scene_data["WavelengthSampling"] = "Regular sampling"

  # Denoiser settings
  scene_data["EnableDenoising"] = False
  scene_data["DenoiserType"] = "BayesianCollaborativeDenoiser"
  scene_data["HistogramBins"] = 16
  scene_data["HistogramDistanceThreshold"] = toNanaFloat(0.75)
  scene_data["PatchRadius"] = 1
  scene_data["SearchWindowRadius"] = 6
  scene_data["NumberOfScales"] = 2

  return scene_data

def parseRenderingMethodSettings(settings, executor):
  scene_data = dict()

  # Path tracing
  scene_data["Type"] = "PathTracing"
  scene_data["EyePathLightSampler"] = "PowerWeightedLightSampler"
  scene_data["RussianRoulette"] = "Reflectance (Max)"
  scene_data["PathLength"] = 3
  raycast_epsilon = toNanaFloat(1e-06, 7)
  scene_data["RayCastEpsilon"] = raycast_epsilon
  printInfo(InfoType.kDebug, "Raycast epsilon: {0}.".format(raycast_epsilon))

  return scene_data

def getDefaultTextureSetting():
  texture_data = dict()

  texture_data["Name"] = "DefaultTexture"
  texture_data["Type"] = "ValueTexture"
  texture_data["Value"] = toNanaFloat(0.8)

  return texture_data

def parseTextureModelSettings(settings, executor):
  texture_info_list = list()
  texture_model_list = list()

  # Default texture
  texture_model_list.append(dict())
  texture_model_list[0]["Name"] = "DefaultTexture"
  texture_model_list[0]["Type"] = "ValueTexture"
  texture_model_list[0]["Value"] = toNanaFloat(0.8)
  texture_info_list.append(0.8)

  # Image textures
  image_dir = os.path.join(settings.resource_dir_, "images")
  for image in bpy.data.images:
    if image.type in ['IMAGE', 'UV_TEST']:
      settings.image_list_.append(image)
      if image.is_float:
        # \todo Save a float image
        print("## " + image.name + " isn't saved.")
      else:
        texture_data = dict()
        image_name = os.path.splitext(image.name)[0]
        texture_data["Name"] = image_name
        texture_data["Type"] = "ImageTexture"
        texture_data["ImageFilePath"] = os.path.join(image_dir, image_name + ".png")
        texture_info_list.append(image_name)
        texture_model_list.append(texture_data)

  return (texture_info_list, texture_model_list)

class MaterialInfo:
  def __init__(self, material):
    self.material_ = material
    self.surface_index_ = 0
    self.emitter_index_ = 0
    self.has_emitter_ = False

def parseCyclesMaterialSettings(settings, material_info, texture_info_list, texture_model_list, surface_model_list, emitter_model_list, executor):
  material = material_info.material_

  if not material.use_nodes:
    printInfo(InfoType.kWarning,
              "Material '{0}' doesn't have node tree.".format(material.name))
    surface_data = dict()
    surface_data["Name"] = material.name
    surface_data["Type"] = "SmoothDiffuseSurface"
    surface_data["ReflectanceIndex"] = 0
    material_info.surface_index_ = len(surface_model_list)
    surface_model_list.append(surface_data)
    return

  root_node = material.node_tree.get_output_node('CYCLES')

  surface_root = []
  for link in root_node.inputs[0].links:
    surface_root.append(link.from_node)
  assert len(surface_root) == 1, "The size of surface root is wrong."

  volume_root = []
  for link in root_node.inputs[1].links:
    volume_root.append(link.from_node)
  assert len(volume_root) <= 1, "The size of volume root is wrong."

  # Parse surface
  node_tree = [surface_root[0]]
  surface_data = dict()
  surface_data["Name"] = material.name
  surface_data["Type"] = "SmoothDiffuseSurface"
  surface_data["ReflectanceIndex"] = 0
  material_info.surface_index_ = len(surface_model_list)
  surface_model_list.append(surface_data)

  # Parse surface emitter
  node_tree = [surface_root[0]]
  while node_tree:
    shader_node = node_tree.pop()

    if isinstance(shader_node, bpy.types.ShaderNodeEmission):
      emitter_data = dict()
      emitter_data["Name"] = material.name
      emitter_data["Type"] = "NonDirectionalEmitter"
      emitter_data["EmissiveColorIndex"] = 0
      emitter_data["RadiantExitance"] = toNanaFloat(128.0)
      material_info.emitter_index_ = len(emitter_model_list)
      material_info.has_emitter_ = True
      emitter_model_list.append(emitter_data)
      break

    for child_node in shader_node.inputs:
      for link in child_node.links:
        n = link.from_node
        node_tree.append(n)

def parseMaterialSettings(settings, texture_info_list, texture_model_list, executor):
  scene = getBlendScene(settings.scene_name_)

  material_info_list = list()
  surface_model_list = list()
  emitter_model_list = list()

  # Default surface
  surface_model_list.append(dict())
  surface_model_list[0]["Name"] = "DefaultSurface"
  surface_model_list[0]["Type"] = "SmoothDiffuseSurface"
  surface_model_list[0]["ReflectanceIndex"] = 0

  # Default emitter
  emitter_model_list.append(dict())
  emitter_model_list[0]["Name"] = "DefaultEmitter"
  emitter_model_list[0]["Type"] = "NonDirectionalEmitter"
  emitter_model_list[0]["EmissiveColorIndex"] = 0
  emitter_model_list[0]["RadiantExitance"] = toNanaFloat(128.0)

  for material in bpy.data.materials:
    material_info = MaterialInfo(material)
    if scene.render.engine == 'CYCLES':
      parseCyclesMaterialSettings(settings, material_info, texture_info_list, texture_model_list, surface_model_list, emitter_model_list, executor)
    material_info_list.append(material_info)

  return (material_info_list, surface_model_list, emitter_model_list)

class Object:
  def __init__(self, settings, obj, index):
    self.obj_ = obj
    self.id_ = index[0]
    index[0] = index[0] + 1
    # Check modifiers
    removed_modifiers = list()
    for modifier in obj.modifiers:
      if not modifier.show_render:
        removed_modifiers.append(modifier)
    for modifier in removed_modifiers:
      obj.modifiers.remove(modifier)
    # Transformation
    self.separate_transformation_ = settings.separate_transformation_
    if self.separate_transformation_:
      modifier_types = ['MIRROR']
      for modifier in obj.modifiers:
        self.separate_transformation_ = modifier.type not in modifier_types
        if not self.separate_transformation_:
          break
    # Instancing
    can_be_instanced = self.separate_transformation_ and \
        (len(obj.modifiers) == 0) and \
        obj.data
    if can_be_instanced and (obj.data.name in settings.object_data_list_):
      self.is_instance_ = True
      self.original_ = settings.object_data_list_[obj.data.name]
    else:
      if obj.data:
        settings.object_data_list_[obj.data.name] = self
      self.is_instance_ = False
    # Children
    self.children_ = list()
    for child_obj in obj.children:
      self.children_.append(Object(settings, child_obj, index))
    # Instancer
    if obj.instance_type == 'COLLECTION':
      scene = getBlendScene(settings.scene_name_)
      view_collection = bpy.context.view_layer.active_layer_collection.collection
      for child_obj in obj.instance_collection.objects:
        new_obj = child_obj.copy()
        view_collection.objects.link(new_obj)
        new_obj.parent = obj
        self.children_.append(Object(settings, new_obj, index))

  def isCamera(self):
    return self.obj_.type == 'CAMERA'

  def hasChild(self):
    return 0 < len(self.children_)

  def calcVolumeOfBoundBox(self):
    bound = self.obj_.bound_box
    min_point = [bound[0][0], bound[0][1], bound[0][2]]
    max_point = [bound[0][0], bound[0][1], bound[0][2]]
    for p in bound:
      for i in range(3):
        min_point[i] = min(min_point[i], p[i])
        max_point[i] = max(max_point[i], p[i])
    x = math.fabs(max_point[0] - min_point[0])
    y = math.fabs(max_point[1] - min_point[1])
    z = math.fabs(max_point[2] - min_point[2])
    volume = 2.0 * (x * y + x * z + y * z)
    return volume

  def hasBody(self):
    epsilon = 1.0e-10
    has_body = epsilon < self.calcVolumeOfBoundBox()
    if has_body:
      obj = self.obj_
      if obj.type == 'MESH':
        has_body = 0 < len(obj.data.polygons)
        if not has_body:
          printInfo(InfoType.kWarning,
                    "Object '{0}' doesn't have polygon.".format(obj.name))
      elif obj.type == 'ARMATURE':
        printInfo(InfoType.kWarning, "Object '{0}' is an armature.".format(obj.name))
        has_body = False
      elif obj.type == 'LATTICE':
        printInfo(InfoType.kWarning, "Object '{0}' is a lattice.".format(obj.name))
        has_body = False
      else:
        data = obj.to_mesh(bpy.context.depsgraph, True)
        has_body = 0 < len(data.polygons)

    return has_body

  def takeTransformation(self):
    transformation_list = list()
    if (not self.separate_transformation_) and (not self.isCamera()):
      return transformation_list

    obj = self.obj_
    # Scaling
    scale = [toNanaFloat(obj.scale[0]),
             toNanaFloat(obj.scale[1]),
             toNanaFloat(obj.scale[2])]
    if (scale[0] != 1.0) or (scale[1] != 1.0) or (scale[2] != 1.0):
      scaling_setting = dict()
      scaling_setting["Type"] = "Scaling"
      scaling_setting["Enabled"] = True
      scaling_setting["Value"] = scale
      if not self.isCamera():
        transformation_list.append(scaling_setting)
      # Clear scaling info
      obj.scale = [1.0, 1.0, 1.0]

    # Rotation
    supported_rotation_mode = ['XYZ', 'XZY', 'YXZ', 'YZX', 'ZXY', 'ZYX']
    assert obj.rotation_mode in supported_rotation_mode, \
           "The rotation mode'{0}' isn't supported.".format(obj.rotation_mode)
    for axis in obj.rotation_mode:
      axis_index = ord(axis) - ord('X')
      axis_name = axis + " Axis"
      angle = math.degrees(obj.rotation_euler[axis_index])
      if self.isCamera() and (axis == 'X'):
        # Assume that the rotation order of camera is 'XYZ'
        angle = angle - 90.0
      angle = toNanaFloat(angle)
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
    location = [toNanaFloat(obj.location[0]),
                toNanaFloat(obj.location[1]),
                toNanaFloat(obj.location[2])]
    if (location[0] != 0.0) or (location[1] != 0.0) or (location[2] != 0.0):
      translation_setting = dict()
      translation_setting["Type"] = "Translation"
      translation_setting["Enabled"] = True
      translation_setting["Value"] = location
      transformation_list.append(translation_setting)
      # Clear translation info
      obj.location = [0.0, 0.0, 0.0]

    return transformation_list

def calcNanaAngleOfView(scene, angle_of_view):
  resolutions = calcImageResolution(scene)
  tan_phi = resolutions[1] / resolutions[0]
  tan_theta = math.tan(0.5 * angle_of_view) * math.sqrt(1.0 + tan_phi * tan_phi)
  theta = 2.0 * math.atan(tan_theta)
  return theta 

def parseCameraSettings(settings, executor):
  camera_data = dict()

  scene = getBlendScene(settings.scene_name_)
  index = [0]
  obj = Object(settings, scene.camera, index)

  camera_data["GroupLevel"] = 0
  camera_data["Type"] = "CameraObject"
  camera_data["Enabled"] = True
  camera_data["Name"] = "Camera"
  camera_data["CameraType"] = "PinholeCamera"
  camera_data["AngleOfView"] = \
      toNanaFloat(math.degrees(calcNanaAngleOfView(scene, obj.obj_.data.angle)))
  camera_data["Jittering"] = True
  camera_data["Transformation"] = obj.takeTransformation()

  return camera_data 

def makeWorldSettings():
  world_data = dict()

  world_data["GroupLevel"] = 0
  world_data["Type"] = "GroupObject"
  world_data["Enabled"] = True
  world_data["Name"] = "World"
  world_data["Transformation"] = list()

  return world_data

def parseSceneObjectSettings(settings, scene, material_info_list, object_list, obj, depth = 1):
  obj_data = dict()

  num_of_objects = 0
  has_body = obj.hasBody()
  has_child = obj.hasChild()
  if (not has_body) and (not has_child):
    printInfo(InfoType.kWarning, "Object '{0}' is skipped.".format(obj.obj_.name))
    return num_of_objects

  obj_data["GroupLevel"] = depth
  obj_data["Type"] = "GroupObject" if has_child else "SingleObject"
  obj_data["Enabled"] = not obj.obj_.hide_render
  obj_data["Name"] = obj.obj_.name
  obj_data["Transformation"] = obj.takeTransformation()

  # Single object settings
  if has_body:
    if has_child:
      object_list.append(obj_data)
      obj_data = copy.deepcopy(obj_data)
      obj_data["GroupLevel"] = depth + 1
      obj_data["Type"] = "SingleObject"
      obj_data["Transformation"] = list()

    material_info = MaterialInfo(None)
    if obj.obj_.active_material != None:
      material_index = bpy.data.materials.find(obj.obj_.active_material.name)
      material_info = material_info_list[material_index]
    else:
      printInfo(InfoType.kWarning, 
                "Object '{0}' doesn't have material.".format(obj.obj_.name))

    obj_file_name = obj.obj_.name + ".obj"
    obj_file_path = os.path.join(settings.resource_dir_, "objects", obj_file_name)
    if obj.is_instance_:
      printInfo(InfoType.kWarning,
                "Object '{0}' is an instance.".format(obj.obj_.name))
    settings.object_list_.append(obj.obj_)

    obj_data["ShapeType"] = "MeshObject"
    obj_data["ObjectFilePath"] = obj_file_path
    obj_data["Smoothing"] = False
    obj_data["SurfaceIndex"] = material_info.surface_index_
    obj_data["EmitterIndex"] = material_info.emitter_index_
    obj_data["IsEmissiveObject"] = material_info.has_emitter_
    num_of_objects += 1

  object_list.append(obj_data)

  for child_obj in obj.children_:
    num_of_objects += parseSceneObjectSettings(settings, scene, material_info_list, object_list, child_obj, depth + 1)

  if num_of_objects == 0:
    printInfo(InfoType.kWarning, "Object '{0}' is skipped!".format(obj.obj_.name))
    del object_list[-1]
  elif has_child:
    num_of_objects += 1

  return num_of_objects

def parseObjectSettings(settings, material_info_list, executor):
  object_list = list()

  object_list.append(parseCameraSettings(settings, executor))
  object_list.append(makeWorldSettings())

  # Process a top level objects
  scene = getBlendScene(settings.scene_name_)
  obj_info_list = list()
  index = [2] # 0: camera, 1: world
  for obj in scene.objects:
    if obj.parent == None:
      obj_info_list.append(Object(settings, obj, index))

  for obj in obj_info_list:
    parseSceneObjectSettings(settings, scene, material_info_list, object_list, obj)

  return object_list 

def parseBvhSettings(settings, executor):
  scene_data = dict()

  scene_data["Type"] = "BinaryRadixTreeBvh"

  return scene_data

def parseSceneSettings(settings, executor):
  settings.scene_data_["Scene"] = parseTagSettings(settings, executor)
  settings.scene_data_["System"] = parseSystemSettings(settings, executor)
  settings.scene_data_["Color"] = parseColorSettings(settings, executor)
  settings.scene_data_["RenderingMethod"] = parseRenderingMethodSettings(settings, executor)
  texture_info_list, texture_model_list = parseTextureModelSettings(settings, executor)
  material_info_list, surface_model_list, emitter_model_list = parseMaterialSettings(settings, texture_info_list, texture_model_list, executor)
  settings.scene_data_["TextureModel"] = texture_model_list
  settings.scene_data_["SurfaceModel"] = surface_model_list
  settings.scene_data_["EmitterModel"] = emitter_model_list
  settings.scene_data_["Object"] = parseObjectSettings(settings, material_info_list, executor)
  settings.scene_data_["Bvh"] = parseBvhSettings(settings, executor)

# Encode a scene data into JSON and save as a nana file
def exportSceneSettingsAsNana(settings):
  file_name = getBlendFileName() + ".nana"
  nana_file = open(file_name, 'w')
  json.dump(settings.scene_data_, nana_file, allow_nan=False, indent=4)

def exportObject(obj, object_dir):
  obj.select_set(True)

  bpy.ops.export_scene.obj( \
      filepath = os.path.join(object_dir, obj.name + ".obj"), \
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

  obj.select_set(False)

def exportImage(scene, image, resource_dir):
  if image.is_float:
    # \todo Save a float image
    print("## " + image.name + " isn't saved.")
  else:
    # Save an image as PNG
    image_name = os.path.splitext(image.name)[0] + ".png"
    file_path = os.path.join(resource_dir, image_name)
    printInfo(InfoType.kDebug, "Saving image: '{0}'.".format(image_name))
    image.save_render(file_path, scene=scene)

def exportResources(settings, executor):
  if not settings.export_resources_:
    return

  scene = getBlendScene(settings.scene_name_)
  resource_dir = os.path.join(os.getcwd(), settings.resource_dir_)

  # Object
  object_dir = os.path.join(resource_dir, "objects")
  os.makedirs(object_dir, exist_ok=True)
  printInfo(InfoType.kStatus, "Object dir: '{0}'.".format(object_dir))
  for obj in settings.object_list_:
    exportObject(obj, object_dir)
  # Image
  image_dir = os.path.join(resource_dir, "images")
  os.makedirs(image_dir, exist_ok=True)
  printInfo(InfoType.kStatus, "Image dir: '{0}'.".format(image_dir))
  for image in settings.image_list_:
    exportImage(scene, image, image_dir)

def main():
  # Process command line arguments
  arg_parser = makeArgParser()
  args, unknown = arg_parser.parse_known_args()
  if args.nanascene == "":
    # We assume that a blend file has only one scene
    args.nanascene = bpy.data.scenes[0].name
  args.nanathreads = max(args.nanathreads, 1)
  printInfo(InfoType.kDebug, "Num of threads: {0}.".format(args.nanathreads))

  # Initialize the exporter
  bpy.ops.object.select_all(action='DESELECT')
  settings = NanairoSettings(args)
  os.makedirs(settings.resource_dir_, exist_ok=True)
  printInfo(InfoType.kStatus, "Resource dir: '{0}'.".format(settings.resource_dir_))
  executor = concurrent.futures.ThreadPoolExecutor(max_workers=args.nanathreads)

  # Export a scene data
  parseSceneSettings(settings, executor)
  exportSceneSettingsAsNana(settings)
  exportResources(settings, executor)

if __name__ == "__main__":
  main()
