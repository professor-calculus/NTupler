import os
import sys
import re

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/runCombined.py
# FROM THE LOCATION WHERE YOU WISH TO GET THE OUTPUT
# runs on all mHXX_mSusyYYYY directories.

#############################
#############################
#############################
### ## # USER INPUTS # ## ###

# the combined process you desire
combinedMethod = "AsymptoticLimits"

#############################
#############################
#############################
#############################

inputDir = os.getcwd()

signalDirs = os.popen("ls %s" % inputDir, "r").readlines()

for signalDir in signalDirs:

	# if (signalDir[0:4] != "mH40"):
	# 	continue

	signalDir = signalDir.rstrip()
	c1 = -1
	c2 = -1
	c3 = len(signalDir)

	if signalDir[0:2] != "mH":
		continue

	for i in range(2, len(signalDir)):
		if (signalDir[i] == "_"):
			c1 = i
			break

	for i in range(c1, len(signalDir)):
		if (signalDir[i:i+6] == "_mSusy"):
			c2 = i + 6
			break

	for i in range(c2, len(signalDir)):
		if (signalDir[i].isdigit() == False):
			c3 = i
			break

	if (c1 == -1 or c2 == -1):
		continue

	higgsMass = signalDir[2:c1]
	susyMass = signalDir[c2:c3]
	fileToUse = os.path.join(inputDir,signalDir,"allbins.root")

	
	print "RUNNING COMBINE FOR mH" + higgsMass + " mSusy" + susyMass
	# print "combine -M %s --mass %s --keyword-value mSusy=%s %s" % (combinedMethod, higgsMass, susyMass, fileToUse) # for testing
	os.system("combine -M %s --mass %s --keyword-value mSusy=%s %s" % (combinedMethod, higgsMass, susyMass, fileToUse))
	print ""
	print ""
