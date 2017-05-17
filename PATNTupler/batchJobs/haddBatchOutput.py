import os

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/batchJobs/haddBatchOutput.py

###########################################################################################################
############## USER INPUTS ################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

motherDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees/" # has to be the full path
subDirs = ""; # will use dirs that match <motherDir>/<subDirs*> (subDirs only needs to be a subset of desired directory)

###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

projectsToHadd = os.popen("ls %s" % motherDir, "r").readlines()
for project in projectsToHadd:

	project = project.rstrip()
	projectPath = os.path.join(motherDir,project)
	if os.path.isdir(projectPath) and subDirs in project:
	
		rootFiles = os.path.join(projectPath,"tmp","flatTree_*.root") 
		jobFiles = os.path.join(projectPath,"tmp","job_*.sh")
		
		# check that the numberOfRootFiles from jobs = numberOfJobFiles and is > 0 
		numberOfRootFiles = os.popen("ls -l %s | wc -l" % rootFiles).readline()
		numberOfRootFiles = numberOfRootFiles[:-1]
		numberOfJobFiles = os.popen("ls -l %s | wc -l" % jobFiles).readline()
		numberOfJobFiles = numberOfJobFiles[:-1]
		if numberOfRootFiles == numberOfJobFiles and numberOfRootFiles > 0:
			
			combinedRootFile = os.path.join(projectPath,"flatTree.root")
			print "creating hadded ROOT file:"
			print combinedRootFile
			# print "hadd %s %s" % (combinedRootFile,rootFiles) # for testing
			os.system("hadd %s %s" % (combinedRootFile,rootFiles))
			print ""

		else:
			print "***************************"
			print "***************************"
			print "ERROR"
			print "the number of output ROOT files in:\n" + rootFiles + "\ndoes not equal the number of job files... Not hadding..."
			print "***************************"
			print "***************************"
			print ""
		print""