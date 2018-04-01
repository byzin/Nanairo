# file: spectral_transport.py

# Import system plugins
import math
import os
import sys
import time

# Import custom plugins
def printPluginError(plugin_name, package_name):
    sys.stderr.write("'{}' is required.".format(plugin_name))
    sys.stderr.write(" Please install the package using 'pip install {}'.\n".format(package_name))

kHavePlugins = True
# NumPy
try:
    import numpy as np
except:
    printPluginError("NumPy", "numpy")
    kHavePlugins = False
# SciPy
try:
    import scipy
except:
    printPluginError("SciPy", "scipy")
    kHavePlugins = False
# Colour Science
try:
    import colour.plotting as clr
    import colour.recovery as rec
    import colour
except:
    printPluginError("Colour Science", "colour-science")
    kHavePlugins = False
# Matplotlib
try:
    import matplotlib.pyplot as plt
    from matplotlib.path import Path
except:
    printPluginError("Matplotlib", "matplotlib")
    kHavePlugins = False

# Global variables
kLicenseString = '\
/*!\n\
  Copyright (c) 2015-2018 Sho Ikeda\n\
  This software is released under the MIT License.\n\
  http://opensource.org/licenses/mit-license.php\n\
  */\n'
kShortestWavelength = 380
kLongestWavelength = 780
kWavelengthResolution = 10
assert(((kLongestWavelength - kShortestWavelength) % kWavelengthResolution) == 0)
kGridResolution = (6, 4)
kCmfXBarPath = "xyz_cmf/cie_sco_2degree_xbar.csv"
kCmfYBarPath = "xyz_cmf/cie_sco_2degree_ybar.csv"
kCmfZBarPath = "xyz_cmf/cie_sco_2degree_zbar.csv"


# ------------------------------------------------------------------------------
# Return the num of wavelengths.
# ------------------------------------------------------------------------------
def spectraSize():
    return (kLongestWavelength - kShortestWavelength) // kWavelengthResolution + 1


# ------------------------------------------------------------------------------
# Color matching functions.
# ------------------------------------------------------------------------------
class Cmf:
    x_bar = []
    y_bar = []
    z_bar = []

    @staticmethod
    def getWavelength(index):
        assert(0 <= index < spectraSize())
        return kShortestWavelength + index * kWavelengthResolution

    @classmethod
    def initCmf(cls):
        w, x_bar = np.loadtxt(kCmfXBarPath, delimiter=',', skiprows=2, unpack=True)
        _, y_bar = np.loadtxt(kCmfYBarPath, delimiter=',', skiprows=2, unpack=True)
        _, z_bar = np.loadtxt(kCmfZBarPath, delimiter=',', skiprows=2, unpack=True)
        assert(w[0] <= kShortestWavelength)
        assert(kLongestWavelength <= w[-1])
        for i in range(spectraSize()):
            wavelength = Cmf.getWavelength(i)
            cls.x_bar.append(np.interp(wavelength, w, x_bar))
            cls.y_bar.append(np.interp(wavelength, w, y_bar))
            cls.z_bar.append(np.interp(wavelength, w, z_bar))

    @classmethod
    def getX(cls, index):
        assert(0 <= index < spectraSize())
        return cls.x_bar[index]

    @classmethod
    def getY(cls, index):
        assert(0 <= index < spectraSize())
        return cls.y_bar[index]

    @classmethod
    def getZ(cls, index):
        assert(0 <= index < spectraSize())
        return cls.z_bar[index]

    @staticmethod
    def toXyz(spectra):
        '''As CIE instructs, we integrate using simple summation.'''
        assert(len(spectra) == spectraSize())
        xyz = [0.0, 0.0, 0.0]
        for i in range(spectraSize()):
            s = spectra[i]
            xyz[0] += s * Cmf.getX(i)
            xyz[1] += s * Cmf.getY(i)
            xyz[2] += s * Cmf.getZ(i)
        d_lambda = float(kWavelengthResolution)
        return [v * d_lambda for v in xyz]

    @staticmethod
    def getEEWhite():
        ee_white = [1.0] * spectraSize()
        return Cmf.toXyz(ee_white)

# ------------------------------------------------------------------------------
# Transform between color spaces.
# ------------------------------------------------------------------------------
class Transform:
    # --------------------------------------------------------------------------
    # Homogenize/dehomogenize vectors.
    # --------------------------------------------------------------------------
    @staticmethod
    def hom(v2):
        assert(len(v2) >= 2)
        return np.matrix([[v2[0]], [v2[1]], [1]])

    @staticmethod
    def dehom(v3):
        assert((v3.shape[0] == 3 and v3.shape[1] == 1)
            or (v3.shape[0] == 1 and v3.shape[1] == 3))
        v = v3.flatten().tolist()[0]
        return [v[0] / v[2], v[1] / v[2]]

    # --------------------------------------------------------------------------
    # Convert from xyy to xyz and back.
    # --------------------------------------------------------------------------
    @staticmethod
    def toXyzFromXyy(xyy):
        return (xyy[0] * xyy[2] / xyy[1],
                xyy[2],
                (1 - xyy[0] - xyy[1]) * xyy[2] / xyy[1])

    @staticmethod
    def toXyyFromXyz(xyz):
        s = sum(xyz)
        return (xyz[0] / s, xyz[1] / s, xyz[1])

    # --------------------------------------------------------------------------
    # Convert from xy to xy* and back.
    # --------------------------------------------------------------------------
    mat_xystar_to_xy = None
    mat_xy_to_xystar = None

    @classmethod
    def initXystar(cls):
        '''xy* is a color space where the line between blue and red is horizontal.
           Also, equal-energy white is the origin.
           xy* depends only on the color matching functions used.'''
        num_bins = spectraSize()

        # Pure blue.
        s = [0.0] * num_bins
        s[0] = 1.0
        xy0 = cls.toXyyFromXyz(Cmf.toXyz(s))

        # Pure red.
        s = [0.0] * num_bins
        s[-1] = 1.0
        xy1 = cls.toXyyFromXyz(Cmf.toXyz(s))

        d = np.array(xy1[:2]) - np.array(xy0[:2])
        d /= math.sqrt(np.vdot(d, d))

        # Translation to make ee-white (in xy) the origin.
        T = np.matrix([[ 1.0, 0.0, -1/3],
                       [ 0.0, 1.0, -1/3],
                       [ 0.0, 0.0,  1.0]])
        # Rotation to make purple line horizontal.
        R = np.matrix([[ d[0], d[1], 0.0],
                       [-d[1], d[0], 0.0],
                       [  0.0,  0.0, 1.0]]) 

        cls.mat_xy_to_xystar = np.dot(R, T)
        cls.mat_xystar_to_xy = cls.mat_xy_to_xystar.getI()

    @classmethod
    def toXystarFromXy(cls, xy):
        return cls.dehom(np.dot(cls.mat_xy_to_xystar, cls.hom(xy)))

    @classmethod
    def toXyFromXystar(cls, xystar):
        return cls.dehom(np.dot(cls.mat_xystar_to_xy, cls.hom(xystar)))

    # --------------------------------------------------------------------------
    # Convert from xy to uv and back.
    # --------------------------------------------------------------------------
    mat_uv_to_xystar = None
    mat_xystar_to_uv = None
    mat_uv_to_xy     = None
    mat_xy_to_uv     = None

    @classmethod
    def initUv(cls, xystar_bbox, grid_res):
        '''uv is derived from xy* by transforming grid points to integer coordinates.
           uv depends on xy* and the grid used.'''

        # Translate xystar bounding box min to origin.
        T = np.matrix([[1.0, 0.0, -xystar_bbox[0][0]],
                       [0.0, 1.0, -xystar_bbox[0][1]],
                       [0.0, 0.0,                1.0]])

        # Scale so that one grid cell has unit size.
        w = xystar_bbox[1][0] - xystar_bbox[0][0]
        h = xystar_bbox[1][1] - xystar_bbox[0][1]
        S = np.matrix([[grid_res[0] / w, 0.0, 0.0],
                       [0.0, grid_res[1] / h, 0.0],
                       [0.0, 0.0, 1.0]])

        cls.mat_xystar_to_uv = np.dot(S, T)
        cls.mat_uv_to_xystar = cls.mat_xystar_to_uv.getI()
        cls.mat_xy_to_uv     = np.dot(cls.mat_xystar_to_uv, cls.mat_xy_to_xystar)
        cls.mat_uv_to_xy     = cls.mat_xy_to_uv.getI()

    @classmethod
    def toUvFromXy(cls, xy):
        return cls.dehom(np.dot(cls.mat_xy_to_uv, cls.hom(xy)))

    @classmethod
    def toXyFromUv(cls, uv):
        return cls.dehom(np.dot(cls.mat_uv_to_xy, cls.hom(uv)))

    @classmethod
    def toUvFromXystar(cls, xystar):
        return cls.dehom(np.dot(cls.mat_xystar_to_uv, cls.hom(xystar)))

    @classmethod
    def toXystarFromUv(cls, uv):
        return cls.dehom(np.dot(cls.mat_uv_to_xystar, cls.hom(uv)))

# ------------------------------------------------------------------------------
# Given a color in XYZ, determine a smooth spectrum that corresponds to that 
# color.
# ------------------------------------------------------------------------------
def findSpectrum(xyz):
    from scipy.optimize import minimize

    # As an objective, we use a similar roughness term as Smits did.
    def objective(S):
        roughness = 0
        for i in range(len(S)-1):
            roughness += (S[i] - S[i+1]) ** 2
        # Note: We found much better convergence with the square term!
        # roughness = math.sqrt(roughness)
        return roughness

    num_bins = spectraSize()
    x0       = [1] * num_bins

    # Constraint: Match XYZ values.
    cnstr = {
        'type': 'eq',
        'fun': lambda s: (np.array(Cmf.toXyz(s)) - xyz)
    }

    # We want positive spectra.
    bnds = [(0, 1000)] * num_bins

    res = minimize(objective, x0, method='SLSQP', constraints=cnstr,
                   bounds=bnds, options={"maxiter": 2000, "ftol": 1e-10})
    if not res.success:
        err_message = 'Error for xyz={} after {} iterations: {}'.format(xyz, res.nit, res.message)
        return ([0] * num_bins, True, err_message)
    else:
        # The result may contain some very tiny negative values due 
        # to numerical issues. Clamp those to 0.
        return ([max(x, 0) for x in res.x], False, "")


# ------------------------------------------------------------------------------
# Get the boundary of the horseshoe as a path in xy*.
# ------------------------------------------------------------------------------
def makeHorseshoePath():
    verts = []
    codes = []

    d_lambda = float(kWavelengthResolution)
    for i in range(spectraSize()):
        xyz = [0.0, 0.0, 0.0]
        xyz[0] = d_lambda * Cmf.getX(i)
        xyz[1] = d_lambda * Cmf.getY(i)
        xyz[2] = d_lambda * Cmf.getZ(i)
        xyy = Transform.toXyyFromXyz(xyz)
        xystar = Transform.toXystarFromXy(xyy[:2])
        verts.append(xystar)
        codes.append(Path.LINETO)
    codes[0] = Path.MOVETO
    codes.append(Path.CLOSEPOLY)

    vx = [x for (x, y) in verts]
    vy = [y for (x, y) in verts]
    bbox = [ (min(vx), min(vy)), (max(vx), max(vy)) ]

    verts.append((0,0))
    return (Path(verts, codes), bbox)


# ------------------------------------------------------------------------------
# Grid data structures.
# ------------------------------------------------------------------------------
class DataPoint:
    def __init__(self):
        self.xystar             = (0, 0)
        self.uv                 = (0, 0)
        self.Y                  = 0
        self.spectrum           = [0]
        self.M                  = 0
        self.inside             = False
        self.equal_energy_white = False
        self.broken             = False

    def updateUv(self):
        self.uv = Transform.toUvFromXystar(self.xystar)

class GridCell:
    def __init__(self):
        self.indices   = []
        self.triangles = []
        self.inside    = True

# binary search to find intersection
def findIntersection(p0, p1, i0, i1, clip_path):
    delta = p1 - p0
    if np.linalg.norm(delta) < 0.0001:
        # Points are very close, terminate.
        # Move new intersection slightly into the gamut.
        delta *= 0.998
        if i0:
            return p1 - delta
        else:
            return p0 + delta

    p01 = 0.5 * (p0 + p1)
    i01 = clip_path.contains_point(p01)
    if i0 != i01:
        return findIntersection(p0, p01, i0, i01, clip_path)
    elif i1 != i01:
        return findIntersection(p01, p1, i01, i1, clip_path)
    else:
        print ("something wrong here")
        return p01

def clipEdge(d0, d1, clip_path):
    from operator import xor
    if not xor(d0.inside, d1.inside):
        return (False, None)

    p0 = np.array(d0.xystar)
    p1 = np.array(d1.xystar)
    p  = findIntersection(p0, p1, d0.inside, d1.inside, clip_path)

    data_point        = DataPoint()
    data_point.xystar = p
    data_point.inside = True

    return (True, data_point)

def generateXystarGrid(scale):
    print("Generating clip path ...")
    clip_path, bbox = makeHorseshoePath()

    # We know that xy(1/3, 1/3) = xystar(0, 0) must be a grid point.
    # subdivide the rectangle between that and the purest red regularly with res.
    # Note: This can be freely chosen, but we found 6,4 to be a reasonable value.
    res          = kGridResolution
    white_xystar = [0.0, 0.0]
    step_x       = abs(white_xystar[0] - bbox[1][0]) / res[0]
    step_y       = abs(white_xystar[1] - bbox[0][1]) / res[1]

    # Find bbox top left corner so that the whole diagram is contained.
    add_x = int(math.ceil(abs(white_xystar[0] - bbox[0][0]) / step_x))
    add_y = int(math.ceil(abs(bbox[1][1] - white_xystar[1]) / step_y))

    # The index of white - we will set this spectrum to equal energy white.
    white_idx = (add_x, res[1])

    grid_res = (res[0] + add_x, res[1] + add_y)
    bbox = [
        # min
        (white_xystar[0]- step_x * add_x, bbox[0][1]),
        # max
        (bbox[1][0], white_xystar[1] + step_y * add_y)
    ]

    grid        = [GridCell() for i in range(grid_res[0] * grid_res[1])]
    data_points = []

    # Generate grid points.
    print(" Generating grid points in xy* ...")
    for (x,y) in [(x,y) for y in range(grid_res[1]+1) for x in range(grid_res[0]+1)]:
        data_point        = DataPoint()
        data_point.xystar = (bbox[0][0] + step_x * x, bbox[0][1] + step_y * y)

        if (x, y) == white_idx:
            # Numerically, we want the white point to be at xy = (1/3, 1/3).
            delta = np.array(data_point.xystar) - np.array(white_xystar)
            assert(np.dot(delta, delta) < 1e-7)
            data_point.equal_energy_white = True

        # Clip on horseshoe.
        if clip_path.contains_point(data_point.xystar) \
            or (x > 0 and y == 0): # Special case for purple line.
            data_point.inside = True

        new_idx = len(data_points)
        data_points.append(data_point)

        # Add new index to this all four adjacent cells.
        for (cx, cy) in [(x-dx, y-dy) for dy in range(2) for dx in range(2)]:
            if cx >= 0 and cx < grid_res[0] and cy >= 0 and cy < grid_res[1]:
                cell = grid[cy * grid_res[0] + cx]
                cell.indices.append(new_idx)
                cell.inside = cell.inside and data_point.inside

    # Clip grid cells against horseshoe.
    print(" Clipping cells to xy gamut ...")
    for (x, y) in [(x, y) for x in range(grid_res[0]) for y in range(grid_res[1])]:
        cell = grid[y * grid_res[0] + x]

        # No need to clip cells that are completely inside.
        if cell.inside:
            continue

        # We clip the two outgoing edges of each point:
        #
        # d2
        #  .
        # d0 . d1
        # Note: We assume here that data_points was generated as a regular
        #       grid in row major order.
        d0 = data_points[(y+0)*(grid_res[0]+1)+(x+0)]
        d1 = data_points[(y+0)*(grid_res[0]+1)+(x+1)]
        d2 = data_points[(y+1)*(grid_res[0]+1)+(x+0)]

        (clipped_h, p_h) = clipEdge(d0, d1, clip_path)
        if clipped_h:
            new_idx = len(data_points)
            data_points.append(p_h)
            cell.indices.append(new_idx)
            if y > 0:
                grid[(y-1) * grid_res[0] + x].indices.append(new_idx)

        (clipped_v, p_v) = clipEdge(d0, d2, clip_path)
        if clipped_v:
            new_idx = len(data_points)
            data_points.append(p_v)
            cell.indices.append(new_idx)
            if x > 0:
                grid[y * grid_res[0] + x - 1].indices.append(new_idx)

    # Compact grid points (throw away points that are not inside).
    print(" Compacting grid ...")
    new_data_points = []
    new_indices = []
    prefix = 0
    for data_point in data_points:
        if data_point.inside:
            new_indices.append(prefix)
            new_data_points.append(data_point)
            prefix += 1
        else:
            new_indices.append(-1)
    data_points = new_data_points

    for gridcell in grid:
        new_cell_indices = []
        for index in range(len(gridcell.indices)):
            old_index = gridcell.indices[index]
            if new_indices[old_index] >= 0:
                new_cell_indices.append(new_indices[old_index])
        gridcell.indices = new_cell_indices[:]

    # Scale points down towards white point to avoid singular spectra.
    for data_point in data_points:
        data_point.xystar = [v * scale for v in data_point.xystar]

    bbox[0] = [v * scale for v in bbox[0]]
    bbox[1] = [v * scale for v in bbox[1]]

    return data_points, grid, grid_res, bbox

# ------------------------------------------------------------------------------
# Compute spectra for all data points.
# ------------------------------------------------------------------------------
def computeDataSpectra(data_point):
    xy = Transform.toXyFromUv(data_point.uv)

    # Set luminance to y. This means that X+Y+Z = 1, 
    # since y = Y / (X+Y+Z) = y / (X+Y+Z).
    xyY = [xy[0], xy[1], xy[1]]
    xyz = Transform.toXyzFromXyy(xyY)

    spectrum = []
    broken   = False

    if data_point.equal_energy_white:
        # Since we want X=Y=Z=1/3 (so that X+Y+Z=1), the equal-energy white 
        # spectrum we want is 1/(3 int(x)) for x color matching function.
        spectrum = [1 / (3 * Cmf.getEEWhite()[0])] * spectraSize()
    else:
        spectrum, broken, message = findSpectrum(xyz)

        if broken:
            print("Couldn't find a spectrum for uv=({uv[0]},{uv[1]})".format(uv=data_point.uv))
            print(message)

    xyz = Cmf.toXyz(spectrum)
    sum = xyz[0] + xyz[1] + xyz[2]
    if sum < 0.99 or 1.01 < sum:
        print('Invalid brightness {} for uv=({uv[0]},{uv[1]})'.format(sum, uv=data_point.uv))

    return (spectrum, broken)

# ------------------------------------------------------------------------------

def computeSpectra(data_points):
    print('Computing spectra ...')
    # TODO: Parallelize for multiplatform
    num_done = 0
    data_size = len(data_points)

    def printStatus():
        print('\033[2K\r{} done, {} total: '.format(num_done, data_size), end='')

    printStatus()
    for data in data_points:
        spectrum, broken = computeDataSpectra(data)
        data.spectrum = spectrum
        data.broken = broken
        # Print status
        num_done = num_done + 1
        printStatus()

# ------------------------------------------------------------------------------
# Write spectral data
# ------------------------------------------------------------------------------
def writeSpectralTransportParameters(data_points, grid, grid_res, filename):
#def writeSpectralTransportParameters(filename):
    with open(filename, 'w') as f:
        # Header
        include_guard_name = \
            'NANAIRO_SPECTRAL_TRANSPORT_PARAMETERS_{0}_{1}_{2}_HPP'.format(
                kShortestWavelength,
                kLongestWavelength,
                kWavelengthResolution)
        f.write(kLicenseString)
        f.write('\n')
        f.write('#ifndef {0}\n'.format(include_guard_name))
        f.write('#define {0}\n'.format(include_guard_name))
        f.write('\n')
        f.write('// Zisc\n')
        f.write('#include "zisc/array.hpp"\n')
        f.write('#include "zisc/arithmetic_array.hpp"\n')
        f.write('#include "zisc/matrix.hpp"\n')
        f.write('// Nanairo\n')
        f.write('#include "NanairoCore/nanairo_core_config.hpp"\n')
        f.write('#include "NanairoCore/Color/color.hpp"\n')
        f.write('\n')
        f.write('namespace nanairo {\n')
        f.write('\n')
        f.write('namespace spectral_transport {\n')
        f.write('\n')

        # Body
        f.write('static constexpr uint kGridResolution[] = {{ {}, {} }};\n'.format(grid_res[0], grid_res[1]))
        f.write('\n')
        f.write('static constexpr zisc::Matrix<Float, 2, 3> kToXystarFromXy{\n')
        f.write('    {m[0]}, {m[1]}, {m[2]},\n    {m[3]}, {m[4]}, {m[5]} }};\n'.format(m=Transform.mat_xy_to_xystar[:2,:].flatten().tolist()[0]))
        f.write('static constexpr zisc::Matrix<Float, 2, 3> kToUvFromXy{\n')
        f.write('    {m[0]}, {m[1]}, {m[2]},\n    {m[3]}, {m[4]}, {m[5]} }};\n'.format(m=Transform.mat_xy_to_uv[:2,:].flatten().tolist()[0]))
        f.write('\n')
        f.write('static constexpr Float kInvEqualEnergyReflectance = {};\n'.format(max(Cmf.getEEWhite())))
        f.write('\n')
        # Grid cell
        max_num_idx = 0
        for cell in grid:
            if max_num_idx < len(cell.indices):
                max_num_idx = len(cell.indices)
        f.write('// Grid cells. Laid out in row-major format.\n')
        f.write('struct GridCell {\n')
        f.write('  zisc::Array<int, {}> indices_;\n'.format(max_num_idx))
        f.write('  uint num_points_;\n')
        f.write('  bool inside_;\n')
        f.write('};\n')
        f.write('\n')

        f.write('static constexpr GridCell kGrid[] = {\n')
        cell_strings = []
        for (x, y) in [(x,y) for y in range(grid_res[1]) for x in range(grid_res[0])]:
            cell = grid[y * grid_res[0] + x]
            # pad cell indices with -1.
            padded_indices = cell.indices[:] + [-1] * (max_num_idx - len(cell.indices))
            num_inside = len(cell.indices)
            if 0 < num_inside:
                idx_str = ', '.join(map(str, padded_indices))
                if cell.inside and num_inside == 4:
                    cell_strings.append('  {{ {{ {} }}, {}, true }}'.format(idx_str, num_inside))
                else:
                    cell_strings.append('  {{ {{ {} }}, {}, false }}'.format(idx_str, num_inside))
            else:
                cell_strings.append('  {{ {{ {} }}, 0, false }}'.format(', '.join(['-1'] * max_num_idx)))
        f.write(',\n'.join(cell_strings))
        f.write('\n};\n')
        f.write('\n')
        # Data point
        f.write('// Grid data points.\n')
        f.write('struct DataPoint {\n')
        f.write('  zisc::ArithmeticArray<Float, {}> spectra_;\n'.format(spectraSize()))
        f.write('  Color<2> xystar_;\n')
        f.write('  Color<2> uv_;\n')
        f.write('};\n')
        f.write('\n')

        data_point_strings = []
        for p in data_points:
            spec_str = ', '.join(map(str, list(p.spectrum)))
            data_point_strings.append('  {{ {{ {spec} }}, {{ {p.xystar[0]}, {p.xystar[1]} }}, {{ {p.uv[0]}, {p.uv[1]} }} }}'.format(p=p, spec=spec_str))
        f.write('static constexpr DataPoint kDataPoints[] = {\n')
        f.write(',\n'.join(data_point_strings))
        f.write('\n};\n')
        f.write('\n')

        # Footer
        f.write('} // namespace spectral_transport\n')
        f.write('\n')
        f.write('} // namespace nanairo\n')
        f.write('\n')
        f.write('#endif // {0}\n'.format(include_guard_name))

# ------------------------------------------------------------------------------
# We need to triangulate along the spectral locus, since our regular grid
# cannot properly capture this edge.
# ------------------------------------------------------------------------------
def createTriangleFans(grid):
    print("generating triangle fans...")
    for cell in grid:
        num_points = len(cell.indices)
        # skip trivial inner cells (full quad interpolation)\n",
        if len(cell.indices) == 4 and cell.inside:
            # these could be sorted here, too. but turns out we always get them in scanline order
            # so we will know exactly how to treat them in the exported c code.
            continue

        # triangulate hard cases (irregular quads + n-gons, 5-gons in practice)
        if num_points > 0:
            # used for delaunay or plotting:\n",
            points = np.array([data_points[cell.indices[i]].xystar for i in range(num_points)])
            centroid = (sum(points[:,0])/num_points, sum(points[:,1])/num_points)
            dp = DataPoint()
            dp.xystar = centroid
            dp.updateUv()
            index = len(data_points)
            data_points.append(dp)

            # create triangle fan:
            pts = [(points[i], i, cell.indices[i], math.atan2((points[i]-centroid)[1], (points[i]-centroid)[0])) for i in range(num_points)]
            pts = sorted(pts, key=lambda pt: pt[3])
            # print('sorted {}'.format([pts[i][2] for i in range(num_points)]))
            cell.indices = [index] + [pts[i][2] for i in range(num_points)]
            # print('indices: {}'.format(cell.indices))
            num_points = num_points + 1;
            # do that again with the new sort order:
            # points = np.array([data_points[cell.indices[i]].xystar for i in range(num_points)])
            # now try triangle fan again with right pivot
            cell.triangles = [[0, i+1, i+2] for i in range(len(cell.indices)-2)]

if __name__ == "__main__":
    # Parse command line options.
    import argparse
    parser = argparse.ArgumentParser(description='Generate spectrum_grid.h')
    parser.add_argument('-s', '--scale', metavar='SCALE', type=float,
        default=0.97,
        dest='scale',
        help='Scale grid points toward the EE white point using this factor. Defaults to 0.97.')
    parser.add_argument('-sw', '--shortestW', metavar='SHORTESTW', type=int,
        default=kShortestWavelength,
        dest='shortest_w',
        help='Shortest wavelength.')
    parser.add_argument('-lw', '--longestW', metavar='LONGESTW', type=int,
        default=kLongestWavelength,
        dest='longest_w',
        help='Longest wavelength.')
    parser.add_argument('-r', '--resolution', metavar='RESOLUTION', type=int,
        default=kWavelengthResolution,
        dest='w_resolution',
        help='Wavelength resolution.')

    args = parser.parse_args()

    if not kHavePlugins:
        sys.exit(1)

    kShortestWavelength = args.shortest_w
    kLongestWavelength = args.longest_w
    kWavelengthResolution = args.w_resolution

    # Init xystar.
    Cmf.initCmf()
    Transform.initXystar()

    # Generate the grid.
    data_points, grid, grid_res, xystar_bbox = generateXystarGrid(args.scale)

    # Init uv.
    Transform.initUv(xystar_bbox, grid_res)
    for dp in data_points:
        dp.updateUv()

    createTriangleFans(grid)

    # Compute spectra and store in spectrum_data.h
    output_file_name = 'spectral_transport_parameters_{0}_{1}_{2}.hpp'.format(
        kShortestWavelength,
        kLongestWavelength,
        kWavelengthResolution)
    computeSpectra(data_points)
    writeSpectralTransportParameters(data_points, grid, grid_res, output_file_name)
