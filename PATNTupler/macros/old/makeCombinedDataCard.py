import os

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/combinedWorkspaces/makeCombinedDataCard.py

# TODO: add other systematic errors

###########################################################################################################
############## USER INPUTS ################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

outputFile = "/opt/ppd/scratch/xap79297/CMSSW_8_0_29/src/NTupler/PATNTupler/combinedWorkspaces/Data_JetHt2016_goldenJson_NOAK4.txt"

numberOfBinsPerHisto = 10
htBins = ["ht1500to2500", "ht2500to3500", "ht3500toInf"]
processVec = ["mH70_mSusy2000_NOAK4", "QCD", "TTJets_NOAK4", "ZJets_NOAK4", "WJets_NOAK4"] # first element is for the signal sample, the rest is background (note that QCD never has the NOAK4 label)
rootFileInputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_09_28_CMSSW_8_0_29_dbtV4/MassCutsV04/histosForCombined/Data_JetHt2016_goldenJson_NOAK4/mH70_mSusy2000_NOAK4/"

###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

numberOfChannels = len(htBins)
numberOfBackgrounds = len(processVec) - 1
listofDashes = "-" * 20

def writeBlock(stringToWrite, f):
	
	charactersPerBlock = 25
	f.write("%s" % stringToWrite)

	if ( len(stringToWrite) < charactersPerBlock ):
		emptySpace = " " * (charactersPerBlock - len(stringToWrite))
		f.write("%s" % emptySpace)
	else:
		print "WARNING: the string " + stringToWrite + " is longer than then block size"
		f.write(" ")
		
	return


f = open("%s" % outputFile, 'w')

f.write("imax %d\n" % numberOfChannels)
f.write("jmax %d\n" % numberOfBackgrounds)
f.write("kmax *\n") # it might be a good idea to put a concrete value here in future

f.write("%s\n" % listofDashes)

for htBin in htBins:
	rootFile = "combineTH1D_" + htBin + ".root"
	rootFile = os.path.join(rootFileInputDir, rootFile)
	f.write("shapes * %s %s $PROCESS $PROCESS_$SYSTEMATIC\n" % (htBin, rootFile))

f.write("%s\n" % listofDashes)

writeBlock("bin", f)
for htBin in htBins:
	writeBlock(htBin, f)
f.write("\n")
writeBlock("observation", f)	
for htBin in htBins:
	writeBlock("-1", f)

f.write("\n%s\n" % listofDashes)

writeBlock("bin", f)
for htBin in htBins:
	for process in processVec:
		writeBlock(htBin, f)
f.write("\n")
writeBlock("process", f)
for htBin in htBins:
	for process in processVec:
		writeBlock(process, f)
f.write("\n")
writeBlock("process", f)
for htBin in htBins:
	for i in range (0, len(processVec)):
		writeBlock(str(i), f)
f.write("\n")
writeBlock("rate", f)
for htBin in htBins:
	for i in range (0, len(processVec)):
		writeBlock("-1", f)

f.write("\n%s\n" % listofDashes)

for iChan in range(0, numberOfChannels):
	for iPro in range(1, numberOfBackgrounds+1):
		for i in range(1, numberOfBinsPerHisto+1):
			writeBlock("bin%dStat shapeN2" % i, f)
			for j in range(0, numberOfChannels * (numberOfBackgrounds+1) ):
				
				if (j == iChan * (numberOfBackgrounds+1) + iPro):
					writeBlock("1.0", f)
				else:
					writeBlock("-", f)
			f.write("\n")
		f.write("\n")
	f.write("\n")

f.close()