import os

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/batchJobs/setoffJobs.py

###########################################################################################################
############## USER INPUTS ################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

motherDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/" # has to be the full path
subDirs = ""; # will use dirs that match <motherDir>/<subDirs> (subDirs only needs to be a subset of desired directory)

###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

cmsswBase = os.popen("echo $CMSSW_BASE", "r").readline()
cmsswBase = cmsswBase.rstrip()

f = open("%s/src/NTupler/PATNTupler/batchJobs/tmpSetoffJobs.sh" % cmsswBase, 'w')
f.write("#!/bin/bash\n")
f.write("# RUN WITH > source $CMSSW_BASE/src/NTupler/PATNTupler/batchJobs/tmpSetoffJobs.sh\n")

projectsToSubmit = os.popen("ls %s" % motherDir, "r").readlines()
for project in projectsToSubmit:

	project = project.rstrip()
	projectPath = os.path.join(motherDir,project,"tmp")
	if os.path.isdir(projectPath) and subDirs in project:

		f.write("cd %s\n" % projectPath)
		# f.write("pwd\n") # for testing
		f.write("./submitCondorJobs_nTupAnaNMSSM.sh\n")
		f.write("echo\n")
		f.write("echo\n")
		f.write("echo\n")
		f.write("echo\n")
		f.write("echo\n")

f.close()
os.chmod("%s/src/NTupler/PATNTupler/batchJobs/tmpSetoffJobs.sh" % cmsswBase, 0755)
os.system("%s/src/NTupler/PATNTupler/batchJobs/tmpSetoffJobs.sh" % cmsswBase)