import os
from SignalMonteCarloDict import signalSamples

# run with $ python quickSignalSubmit_filelists.py from batchJobs directory

# make sure that in your makeFileLists.py...
# motherDirs and outputFileList contain NAMEXYZ

keys = signalSamples.keys()
for i in range (0, len(keys)):

	os.system("cp makeFileLists.py TEMP_makeFileLists.py")
	os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_makeFileLists.py" % keys[i])
	# os.system("cat TEMP_makeFileLists.py") # for testing
	os.system("python TEMP_makeFileLists.py") # for running
	os.system("rm TEMP_makeFileLists.py")