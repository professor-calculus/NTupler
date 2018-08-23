import os
import sys
import re

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/runCombinedBatch.py
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

cmsswBase = os.popen("echo $CMSSW_BASE", "r").readline()
cmsswBase = cmsswBase.rstrip()

inputDir = os.getcwd()

signalDirs = os.popen("ls %s" % inputDir, "r").readlines()

for signalDir in signalDirs:

    # IF YOU WANT TO BE SELECTIVE
    # if (signalDir[0:4] != "mH40"):
    #   continue

    # IF YOU WANT TO BE SELECTIVE
    # if (signalDir != "mH40_mSusy2400\n"):
    #   continue

    signalDir = signalDir.rstrip()
    c1 = -1
    c2 = -1
    c3 = len(signalDir)

    if (signalDir[-1] != "0"):
        continue

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

    condorFileName = signalDir + ".condor"
    jobFileName = signalDir + ".sh"
    f = open("%s" % condorFileName, 'w')
    f.write("Universe                = vanilla\n")
    f.write("Executable              = %s\n" % jobFileName)
    f.write("Log                     = /opt/ppd/scratch/xap79297/jobLogs/combined/%s.log\n" % signalDir)
    f.write("Output                  = /opt/ppd/scratch/xap79297/jobLogs/combined/%s.out\n" % signalDir)
    f.write("Error                   = /opt/ppd/scratch/xap79297/jobLogs/combined/%s.err\n" % signalDir)
    f.write("Request_memory          = 1 GB\n")
    f.write("Transfer_input_files    = %s\n" % jobFileName)
    f.write("should_transfer_files   = YES\n")
    f.write("when_to_transfer_output = ON_EXIT_OR_EVICT\n")
    f.write("#\n")
    f.write("Getenv                  = True\n")
    f.write("Queue\n")
    f.close()

    g = open("%s" % jobFileName, 'w')
    g.write("#!/bin/bash\n")
    g.write("cd %s\n" % cmsswBase)
    g.write("eval `scramv1 runtime -sh`\n")
    g.write("cd %s\n" % inputDir)
    g.write("combine -M %s --mass %s --keyword-value mSusy=%s %s\n" % (combinedMethod, higgsMass, susyMass, fileToUse) )
    g.close()
    os.chmod(jobFileName, 0755)

    os.system("condor_submit %s" % condorFileName)
    # print "condor_submit %s" % condorFileName
    print ""
    print ""
