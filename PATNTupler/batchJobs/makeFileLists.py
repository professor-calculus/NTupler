import os

# this script creates the fileLists
# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/batchJobs/makeFileLists.py

def getAllRootFilesBeneathPaths(pathsSoFarVec, pathsToRootFiles):
	
	pathsToNewDirs = []

	for pathSoFar in pathsSoFarVec:

		objectsInPathVec = os.popen("ls %s" % pathSoFar, "r").readlines()
		
		for objectInPath in objectsInPathVec:

			objectInPath = objectInPath.rstrip()

			# here you can avoid paths you do not want to go down
			if (objectInPath == "failed"):
				continue
			
			newPath = os.path.join(pathSoFar,objectInPath)

			if (newPath[-5:] == ".root"):
				pathsToRootFiles.append(newPath)

			if (os.path.isdir(newPath)):
				pathsToNewDirs.append(newPath)

	if (len(pathsToNewDirs) > 0):
		getAllRootFilesBeneathPaths(pathsToNewDirs, pathsToRootFiles)

	return

###########################################################################################################
############## USER INPUTS ################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

# finds all the ROOT files beneath the following set of directories...
motherDirs = ["/pnfs/pp.rl.ac.uk/data/cms/store/user/taylor/ntuples_MC_Apr2018/nmssmSignalCascadeV05_13TeV_NAMEXYZ/"]

# ...and puts the resulting list as stated here (adding on the 'dcap://dcap.pp.rl.ac.uk:22125/' bit)
cmsswBase = os.popen("echo $CMSSW_BASE", "r").readline()
cmsswBase = cmsswBase.rstrip()
outputFileList = cmsswBase + "/src/NTupler/PATNTupler/fileLists/8_0_29_dbtV4_wSys/NAMEXYZ.list"

###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

rootFiles = []
getAllRootFilesBeneathPaths(motherDirs, rootFiles)

f = open("%s" % outputFileList, 'w')
for i in range(0, len(rootFiles)):
	if (i != (len(rootFiles)-1) ):
		f.write("dcap://dcap.pp.rl.ac.uk:22125/%s\n" % rootFiles[i])
	else:
		f.write("dcap://dcap.pp.rl.ac.uk:22125/%s" % rootFiles[i])
f.close()