import os
import sys
sys.path.insert(0, '../batchJobs')
from SignalMonteCarloDict import *


# run with $ python quickSignalSubmit_crab.py <yearOfRun> from plugins directory

# make sure that in your crabConfig_nTupleProd_VJ.py...
# uniqueName = 'NAMEXYZ_CMSSW8029wDBTV4_9b0ff8' (or similar)
# config.Data.inputDataset = 'DATASETXYZ'


if len(sys.argv) != 2:
    print "You have not provided a valid argument. It should be"
    print "$ python quickSignalSubmit_filelists.py <yearOfRun>"
    print "Exiting..."
    exit()

if sys.argv[1] == "2016":
	keys = signalSamples16.keys()
	for i in range (0, len(keys)):

		os.system("cp crabConfig_nTupleProd_VJ.py TEMP_crabConfig_nTupleProd_VJ.py") # 80X
		os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_crabConfig_nTupleProd_VJ.py" % keys[i])
		os.system("sed -i 's:DATASETXYZ:%s:g' TEMP_crabConfig_nTupleProd_VJ.py" % signalSamples16[keys[i]][1])
		# os.system("cat TEMP_crabConfig_nTupleProd_VJ.py") # for testing
		os.system("crab submit -c TEMP_crabConfig_nTupleProd_VJ.py") # for running
		os.system("rm TEMP_crabConfig_nTupleProd_VJ.py")
		print
		print
		print
	exit()

if sys.argv[1] == "2017":
	keys = signalSamples17.keys()
	for i in range (0, len(keys)):

		os.system("cp crabConfig_nTupleProd2017_VJ.py TEMP_crabConfig_nTupleProd_VJ.py") # 94X
		os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_crabConfig_nTupleProd_VJ.py" % keys[i])
		os.system("sed -i 's:DATASETXYZ:%s:g' TEMP_crabConfig_nTupleProd_VJ.py" % signalSamples17[keys[i]][1])
		# os.system("cat TEMP_crabConfig_nTupleProd_VJ.py") # for testing
		os.system("crab submit -c TEMP_crabConfig_nTupleProd_VJ.py") # for running
		os.system("rm TEMP_crabConfig_nTupleProd_VJ.py")
		print
		print
		print
	exit()

else:
    print "You have not provided a valid yearOfRun."
    print "Must be 2016 or 2017."
    print "Exiting..."
    exit() 