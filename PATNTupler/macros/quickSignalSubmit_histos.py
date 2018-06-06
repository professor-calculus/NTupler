import os
import sys
sys.path.insert(0, '../batchJobs')
from SignalMonteCarloDict import *


# run with $ python quickSignalSubmit_histos.py <yearOfRun> from macros directory

# make sure that in your histos_create.cc...
# you use SLIMXYZ in outputdir
# you use NAMEXYZ for the flatTree location
# you use 987654321.0 for the cross section

# check that histos_create.cc actually compiles beforehand
# probs best to close the file histos_create.cc before running !!!


if len(sys.argv) != 2:
    print "You have not provided a valid argument. It should be"
    print "$ python quickSignalSubmit_filelists.py <yearOfRun>"
    print "Exiting..."
    exit()

if sys.argv[1] == "2016":
    keys = signalSamples16.keys()

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
        os.system("sed -i 's:987654321.0:%s:g' histos_create.cc" % signalSamples16[keys[i]][0])
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
    exit()

elif sys.argv[1] == "2017":
    keys = signalSamples17.keys()

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
        os.system("sed -i 's:987654321.0:%s:g' histos_create.cc" % signalSamples17[keys[i]][0])
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
    exit()

else:
    print "You have not provided a valid yearOfRun."
    print "Must be 2016 or 2017."
    print "Exiting..."
    exit()    