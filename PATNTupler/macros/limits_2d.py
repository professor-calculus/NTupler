# fancy limit plotter from Alex T
# run with
# $ python limits2d.py -o /path/to/observed.txt -e /path/to/expected.txt -t <whatever title you like>

from scipy import interpolate
import numpy as np
import matplotlib
from matplotlib import rc
matplotlib.rcParams['mathtext.fontset'] = 'custom'
matplotlib.rcParams['mathtext.rm'] = 'Bitstream Vera Sans'
matplotlib.rcParams['mathtext.it'] = 'Bitstream Vera Sans:italic'
matplotlib.rcParams['mathtext.bf'] = 'Bitstream Vera Sans:bold'
matplotlib.rcParams['text.latex.preamble'].append(r'\usepackage{amsmath}')
#matplotlib.rcParams['text.usetex'] = True
#matplotlib.rcParams['text.latex.unicode'] = True
#matplotlib.rcParams['text.dvipnghack'] = True
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.colors as mcol

import sys
import argparse as a

# Get options
parser=a.ArgumentParser(description='NMSSM Limit Plot')
parser.add_argument('-o', '--observed', required=True)
parser.add_argument('-e', '--expected')
parser.add_argument('-t', '--title', default='')
args=parser.parse_args()

obs = np.loadtxt(args.observed)
exp = []
doWeHaveExpected = False
if (args.expected is not None):
    doWeHaveExpected = True
    exp = np.loadtxt(args.expected)

# how many points on the grid
pointsX = np.unique(obs[:,0])
npointsX = pointsX.size
pointsY = np.unique(obs[:,1])
npointsY = pointsY.size
print "Maximal Mass Grid = " + str(npointsX) + "(x-axis) by " + str(npointsY) + "(y-axis)"


# def interp(data, method='linear'):
#     x = data[:,0]
#     y = data[:,1]
#     z = data[:,2]
    
#     xi = np.linspace(x.min(), x.max(), 100)
#     yi = np.linspace(y.min(), y.max(), 100)
#     zi = mlab.griddata(x, y, z, xi, yi, interp=method)
    
#     return xi, yi, zi


# THIS IS THE FUCTION VERSION BJOERN HAD BEEN USING
def interp2(data, method='linear', n_p=10):
    x = data[:,0]
    y = data[:,1]
    z = data[:,2]

    nx = np.unique(x)
    ny = np.unique(y)

    minn = min(len(nx), len(ny))
    
    xi = np.linspace(x.min(), x.max(), n_p)
    yi = np.linspace(y.min(), y.max(), n_p)
    xi, yi = np.meshgrid(xi,yi)
    zi = interpolate.griddata((x, y), z, (xi, yi), method=method)

    #zi = np.clip(zi, None, 6.)
    
    return xi, yi, zi


# def interp3(data, method='linear'):
#     x = data[:,0]
#     y = data[:,1]
#     z = data[:,2]
    
#     xi = np.linspace(x.min(), x.max(), 100)
#     yi = np.linspace(y.min(), y.max(), 100)
#     xi, yi = np.meshgrid(xi,yi)
#     zi = interpolate.LinearNDInterpolator(x, y, z, method=method)(xi, yi)
    
#     return xi, yi, zi



# >>> The Linear Interpolation <<<
xi, yi, zi = interp2(obs, 'linear', 30*npointsX) # expected grid
xj, yj, zj = interp2(obs, 'linear', 3*npointsX) # expected line
xk, yk, zk = 0, 0, 0 # observed line
if (doWeHaveExpected):
    xk, yk, zk = interp2(exp, 'linear', 3*npointsX)



# Plotting & Aesthetics
stops = [0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000]
red   = [0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764]
green = [0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832]
blue  = [0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539]

ered = []
egreen = []
eblue = []
for i, stop in enumerate(stops):
    if i is 0:
        ered.append( (stop, 0., red[i]) )
        egreen.append( (stop, 0., green[i]) )
        eblue.append( (stop, 0., blue[i]) )
    elif i is len(stops)-1:
        ered.append( (stop, red[i], 1.) )
        egreen.append( (stop, green[i], 1.) )
        eblue.append( (stop, blue[i], 1.) )
    else:
        ered.append( (stop, red[i], red[i]) )
        egreen.append( (stop, green[i], green[i]) )
        eblue.append( (stop, blue[i], blue[i]) )
cdict = {'red': ered, 'green': egreen, 'blue': eblue}

bird = mcol.LinearSegmentedColormap('bird', cdict)
bird.set_over("yellow")

v = np.logspace(-2, 3, 500)

if (doWeHaveExpected):
    plt.contour(xk, yk, zk, [1.0], colors='r')
plt.contour(xj, yj, zj, [1.0], colors='k')
plt.contourf(xi, yi, zi, levels=v, norm=mcol.LogNorm(vmin=0.1, vmax=100), cmap='PuBu_r')
plt.xlabel('$M_{SUSY}$ (GeV)')
plt.ylabel('$M_{H}$ (GeV)')
cbar = plt.colorbar()
ticks = [0.01, 0.1, 1, 10, 100, 1000]
cbar.set_ticks(ticks)
cbar.set_ticklabels(ticks)
cbar.set_label(r'Upper Limit $\sigma_{obs}/\sigma_{theory}$ at $95\%$ CL', rotation=90, fontsize=15, labelpad=5)
plt.title(args.title)
plt.show()