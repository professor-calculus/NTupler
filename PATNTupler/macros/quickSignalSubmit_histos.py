import os
import sys

sys.path.insert(0, '../batchJobs')
from SignalMonteCarloDict import *


# run with $ python quickSignalSubmit_histos.py from macros directory

# make sure that in your histos_create.cc...
# you use SLIMXYZ in outputdir
# you use NAMEXYZ for the flatTree location
# you use 987654321.0 for the cross section

# check that histos_create.cc actually compiles beforehand
# probs best to close the file histos_create.cc before running !!!


keys = signalSamples.keys()

if len(keys) > 0:
    os.system("cp histos_create.cc INITIALCOPY_histos_create.cc")
    os.system("rm histos_create histos_create.o")

for i in range (0, len(keys)):

    print "* * * * * * * * * * * * * * * *"
    print "submitting histo_create job for " + keys[i]
    print "* * * * * * * * * * * * * * * *"
    os.system("cp INITIALCOPY_histos_create.cc histos_create.cc")
    os.system("sed -i 's:SLIMXYZ:%s:g' histos_create.cc" % slimSignalName(keys[i]))
    os.system("sed -i 's:NAMEXYZ:%s:g' histos_create.cc" % keys[i])
    os.system("sed -i 's:987654321.0:%s:g' histos_create.cc" % signalSamples[keys[i]][0])
    # os.system("cat histos_create.cc") # for testing
    os.system("gmake histos_create")
    os.system("./histos_create batch")
    print
    print
    print
    print
    print

os.system("cp INITIALCOPY_histos_create.cc histos_create.cc")
os.system("rm INITIALCOPY_histos_create.cc")
os.system("rm histos_create histos_create.o")