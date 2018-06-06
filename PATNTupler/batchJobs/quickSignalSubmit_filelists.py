import os
import sys
from SignalMonteCarloDict import *


# run with $ python quickSignalSubmit_filelists.py <yearOfRun> from batchJobs directory

# make sure that in your makeFileLists.py...
# motherDirs and outputFileList contain NAMEXYZ


if len(sys.argv) != 2:
    print "You have not provided a valid argument. It should be"
    print "$ python quickSignalSubmit_filelists.py <yearOfRun>"
    print "Exiting..."
    exit()

if sys.argv[1] == "2016":
    keys = signalSamples16.keys()
    for i in range (0, len(keys)):

    	os.system("cp makeFileLists.py TEMP_makeFileLists.py")
    	os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_makeFileLists.py" % keys[i])
    	# os.system("cat TEMP_makeFileLists.py") # for testing
    	os.system("python TEMP_makeFileLists.py") # for running
    	os.system("rm TEMP_makeFileLists.py")
    exit()

elif sys.argv[1] == "2017":
    keys = signalSamples17.keys()
    for i in range (0, len(keys)):

        os.system("cp makeFileLists.py TEMP_makeFileLists.py")
        os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_makeFileLists.py" % keys[i])
        # os.system("cat TEMP_makeFileLists.py") # for testing
        os.system("python TEMP_makeFileLists.py") # for running
        os.system("rm TEMP_makeFileLists.py")
    exit()

else:
    print "You have not provided a valid yearOfRun."
    print "Must be 2016 or 2017."
    print "Exiting..."
    exit()