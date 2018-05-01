import os
import sys
sys.path.insert(0, '../batchJobs')
from SignalMonteCarloDict import signalSamples

# run with $ python quickSignalSubmit_flatTrees.py from main directory

# make sure that in your submitCondorNtupleJobs_VJ.py...
# inputFileListPath and outputDirectory contain NAMEXYZ

keys = signalSamples.keys()
for i in range (0, len(keys)):

	os.system("cp submitCondorNtupleJobs_VJ.py TEMP_submitCondorNtupleJobs_VJ.py")
	os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_submitCondorNtupleJobs_VJ.py" % keys[i])
	# os.system("cat TEMP_submitCondorNtupleJobs_VJ.py") # for testing
	os.system("python TEMP_submitCondorNtupleJobs_VJ.py") # for running
	os.system("rm TEMP_submitCondorNtupleJobs_VJ.py")