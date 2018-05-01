import os
import sys
sys.path.insert(0, '../batchJobs')
from SignalMonteCarloDict import signalSamples

# run with $ python quickSignalSubmit_crab.py from plugins directory

# make sure that in your crabConfig_nTupleProd_VJ.py...
# uniqueName = 'NAMEXYZ_CMSSW8029wDBTV4_9b0ff8' (or similar)
# config.Data.inputDataset = 'DATASETXYZ'

keys = signalSamples.keys()
for i in range (0, len(keys)):

	os.system("cp crabConfig_nTupleProd_VJ.py TEMP_crabConfig_nTupleProd_VJ.py")
	os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_crabConfig_nTupleProd_VJ.py" % keys[i])
	os.system("sed -i 's:DATASETXYZ:%s:g' TEMP_crabConfig_nTupleProd_VJ.py" % signalSamples[keys[i]])
	# os.system("cat TEMP_crabConfig_nTupleProd_VJ.py") # for testing
	os.system("crab submit -c TEMP_crabConfig_nTupleProd_VJ.py") # for running
	os.system("rm TEMP_crabConfig_nTupleProd_VJ.py")
	print
	print
	print