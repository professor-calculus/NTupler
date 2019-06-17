import os
import sys
sys.path.insert(0, '../batchJobs')
from SignalMonteCarloDict import *


# run with $ python quickSignalSubmit_flatTrees.py <yearOfRun> from main directory

# make sure that in your submitCondorNtupleJobs_VJ.py...
# inputFileListPath and outputDirectory contain NAMEXYZ


if len(sys.argv) != 2:
    print "You have not provided a valid argument. It should be"
    print "$ python quickSignalSubmit_filelists.py <yearOfRun>"
    print "Exiting..."
    exit()

if sys.argv[1] == "2016":
    keys = bkgSamples16.keys()
    for i in range (0, len(keys)):

        susyNameList = keys[i]
        
        susyNameOut = susyNameList # FOR ORDINARY production
        # susyNameOut = susyNameList.replace("Susy", "Squark") # FOR SQUARK-SQUARK production only

    	os.system("cp submitCondorNtupleJobs_VJ.py TEMP_submitCondorNtupleJobs_VJ.py")
        os.system("sed -i 's:NAMEXYZ.list:%s.list:g' TEMP_submitCondorNtupleJobs_VJ.py" % susyNameList)
        os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_submitCondorNtupleJobs_VJ.py" % susyNameOut)
    	# os.system("cat TEMP_submitCondorNtupleJobs_VJ.py") # for testing
    	os.system("python TEMP_submitCondorNtupleJobs_VJ.py") # for running
    	os.system("rm TEMP_submitCondorNtupleJobs_VJ.py")
    exit()

elif sys.argv[1] == "2017":
    keys = bkgSamples17.keys()
    for i in range (0, len(keys)):

        susyNameList = keys[i]

        susyNameOut = susyNameList # FOR ORDINARY production
        # susyNameOut = susyNameList.replace("Susy", "Squark") # FOR SQUARK-SQUARK production only

        os.system("cp submitCondorNtupleJobs_VJ.py TEMP_submitCondorNtupleJobs_VJ.py")
        os.system("sed -i 's:NAMEXYZ.list:%s.list:g' TEMP_submitCondorNtupleJobs_VJ.py" % susyNameList)
        os.system("sed -i 's:NAMEXYZ:%s:g' TEMP_submitCondorNtupleJobs_VJ.py" % susyNameOut)
        # os.system("cat TEMP_submitCondorNtupleJobs_VJ.py") # for testing
        os.system("python TEMP_submitCondorNtupleJobs_VJ.py") # for running
        os.system("rm TEMP_submitCondorNtupleJobs_VJ.py")
    exit()

else:
    print "You have not provided a valid yearOfRun."
    print "Must be 2016 or 2017."
    print "Exiting..."
    exit()    
