# fancy limit plotter from Alex Titterton
# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/limits_2d.py

import os
import ROOT
from scipy import interpolate
import numpy as np
import matplotlib
matplotlib.use('agg')
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


#############################
#############################
#############################
### ## # USER INPUTS # ## ###

mSusyVec = [1200, 1600, 2000, 2200, 2400, 2600, 2800]
mHiggsVec = [30, 35, 40, 50, 70, 90, 110, 125]
inputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_01_01/withGluino/allSys/"
outputDir = inputDir + "/a_limitPlot_intp1_noObs/"
# outputDir = inputDir + "/a_limitPlot_intp1_withObs/"

plotObserved = False
plotTitle = '77.5 fb$^{-1}$ (13 TeV)'
# maximally squeeze the z-axis
minMu = -2.10
maxMu = 1.30

#############################
#############################
#############################
#############################

os.system("mkdir -p %s" % outputDir)
os.system("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/limits_2d.py %s/limits_2d_latestCopy.py" % outputDir)

f_2p5 = open("tmpLimits_2p5.txt", 'w')
f_16p0 = open("tmpLimits_16p0.txt", 'w')
f_50p0 = open("tmpLimits_50p0.txt", 'w')
f_84p0 = open("tmpLimits_84p0.txt", 'w')
f_97p5 = open("tmpLimits_97p5.txt", 'w')
f_obs = open("tmpLimits_obs.txt", 'w')

for mSusy in mSusyVec:
    for mHiggs in mHiggsVec:

        rootFile = "higgsCombineTest.AsymptoticLimits.mH" + str(mHiggs) + ".mSusy" + str(mSusy) + ".root"
        rootFile = os.path.join(inputDir, rootFile)
        if os.path.isfile(rootFile) == False:
            print ""
            print "THIS PLOT IS MISSING: mH" + str(mHiggs) + "_mSusy" + str(mSusy)
            print ""
            continue
        f = ROOT.TFile(rootFile)
        T = f.Get("limit")

        T.GetEntry(0)
        f_2p5.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))

        T.GetEntry(1)
        f_16p0.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))

        T.GetEntry(2)
        f_50p0.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))

        T.GetEntry(3)
        f_84p0.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))

        T.GetEntry(4)
        f_97p5.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))
        
        T.GetEntry(5)
        f_obs.write("%d   %d   %f\n" % (mSusy, mHiggs, T.limit))

f_2p5.close()
f_16p0.close()
f_50p0.close()
f_84p0.close()
f_97p5.close()
f_obs.close()

exp_2p5 = np.loadtxt("tmpLimits_2p5.txt")
exp_16p0 = np.loadtxt("tmpLimits_16p0.txt")
exp_50p0 = np.loadtxt("tmpLimits_50p0.txt")
exp_84p0 = np.loadtxt("tmpLimits_84p0.txt")
exp_97p5 = np.loadtxt("tmpLimits_97p5.txt")
obs = np.loadtxt("tmpLimits_obs.txt")

os.system("rm tmpLimits_2p5.txt")
os.system("rm tmpLimits_16p0.txt")
os.system("rm tmpLimits_50p0.txt")
os.system("rm tmpLimits_84p0.txt")
os.system("rm tmpLimits_97p5.txt")
os.system("rm tmpLimits_obs.txt")



def interp(data, method='linear'):
    x = data[:,0]
    y = data[:,1]
    z = data[:,2]
    
    xi = np.linspace(x.min(), x.max(), 100)
    yi = np.linspace(y.min(), y.max(), 100)
    zi = mlab.griddata(x, y, z, xi, yi, interp=method)
    
    return xi, yi, zi

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

# def interp3(data, method='linear'): # currently does not work :(
#     x = data[:,0]
#     y = data[:,1]
#     z = data[:,2]
    
#     xi = np.linspace(x.min(), x.max(), 100)
#     yi = np.linspace(y.min(), y.max(), 100)
#     xi, yi = np.meshgrid(xi,yi)
#     # zi = interpolate.LinearNDInterpolator(x, y, z, method=method)(xi, yi)
    
#     return xi, yi, zi



####################################
# >>> The Linear Interpolation <<< #

xi, yi, zi = interp(exp_50p0, 'linear') # expected grid
xj, yj, zj = interp(exp_50p0, 'linear') # expected line
xj16, yj16, zj16 = interp(exp_16p0, 'linear') # expected line (-1 sigma)
xj84, yj84, zj84 = interp(exp_84p0, 'linear') # expected line (+1 sigma)
xk, yk, zk = 0, 0, 0 # observed line
if (plotObserved):
    xk, yk, zk = interp(obs, 'linear') # observed line

# xi, yi, zi = interp2(exp_50p0, 'linear', 1000) # expected grid
# xj, yj, zj = interp2(exp_50p0, 'linear', 200) # expected line
# xj16, yj16, zj16 = interp2(exp_16p0, 'linear', 200) # expected line (-1 sigma)
# xj84, yj84, zj84 = interp2(exp_84p0, 'linear', 200) # expected line (+1 sigma)
# xk, yk, zk = 0, 0, 0 # observed line
# if (plotObserved):
#     xk, yk, zk = interp2(obs, 'linear', 200) # observed line

####################################


# Plotting & Aesthetics

v = np.logspace(minMu, maxMu, 500) # z axis: min_base10, max_base10, number of samples (squeeze to the point of black spaces)
ticks = [0.00001, 0.0001, 0.001, 0.01, 0.1, 1, 10, 100, 1000, 10000, 100000] # z axis color chart, it should span the above


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

if (plotObserved):
    plt_obs = plt.contour(xk, yk, zk, [1.0], colors='r')
    plt_obs.collections[0].set_label('Observed')
else:
    plt_obs = plt.contour(xj, yj, zj, [1.0], colors='r')
    plt_obs.collections[0].set_label('Observed')
plt_exp = plt.contour(xj, yj, zj, [1.0], colors='k')
plt_exp.collections[0].set_label('Expected')
plt_exp16 = plt.contour(xj16, yj16, zj16, [1.0], colors='k', linestyles='--', label='qwert')
plt_exp16.collections[0].set_label('$\pm1\sigma$')
plt_exp84 = plt.contour(xj84, yj84, zj84, [1.0], colors='k', linestyles='--')
dummy = plt.contourf(xi, yi, zi, levels=v, norm=mcol.LogNorm(vmin=10**minMu, vmax=10**maxMu), cmap=bird)

for d in dummy.collections:
    d.set_edgecolor("face")

plt.xlabel('M$_{SUSY}}$ (GeV)', fontsize=16)
plt.ylabel('M$_{H}$ (GeV)', fontsize=16)
cbar = plt.colorbar()
cbar.set_ticks(ticks)
cbar.set_label('95% CL Upper Limit on $\sigma/\sigma_{theory}$', rotation=90, fontsize=16, labelpad=14)
plt.title('CMS $Preliminary$', loc='left', fontsize=17, fontweight='bold')
plt.title(plotTitle, loc='right', fontsize=17)
plt.legend(loc='upper left')


plt.show()
plt.savefig("%s/limit_plot.pdf" % outputDir)