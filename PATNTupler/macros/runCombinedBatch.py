import os
import sys
import re
import time

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/runCombinedBatch.py
# FROM THE LOCATION WHERE YOU WISH TO GET THE OUTPUT
# runs on all mHXX_mSusyYYYY directories.

#############################
#############################
#############################
### ## # USER INPUTS # ## ###

# A COMMA SEPARATED LIST OF NUISANCES THAT YOU WISH TO FREEZE.

nuisancesToFreeze = "" # >>> no freezing a thing
# nuisancesToFreeze = "SigDbtTag2016,SigDbtTag2017"
# nuisancesToFreeze = "TtDbtTag2016,TtDbtTag2017"
# nuisancesToFreeze = "XS_TTJets2016,XS_TTJets0L2017,XS_TTJets1L2017,XS_TTJets2L2017"
# nuisancesToFreeze = "luminosity2016,luminosity2017"
# nuisancesToFreeze = "jecAK4Unc2016,jecAK4Unc2017"
# nuisancesToFreeze = "jerAK4Unc2016,jerAK4Unc2017"
# nuisancesToFreeze = "jecAK8Unc2016,jecAK8Unc2017"
# nuisancesToFreeze = "jerAK8Unc2016,jerAK8Unc2017"
# nuisancesToFreeze = "jmsUnc2016,jmsUnc2017"
# nuisancesToFreeze = "jmrUnc2016,jmrUnc2017"
# nuisancesToFreeze = "isr2016,isr2017"

#############################
#############################
#############################
#############################

cmsswBase = os.popen("echo $CMSSW_BASE", "r").readline()
cmsswBase = cmsswBase.rstrip()

inputDir = os.getcwd()

signalDirs = os.popen("ls %s" % inputDir, "r").readlines()

timeStamp = str(time.localtime().tm_year) + "_" + str(time.localtime().tm_mon) + "_" + str(time.localtime().tm_mday) + "_"
timeStamp += str(time.localtime().tm_hour) + "_" + str(time.localtime().tm_min) + "_" + str(time.localtime().tm_sec)
batchDir = "batchFiles_" + timeStamp
os.mkdir(batchDir)

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

    f = open("%s/%s" % (batchDir,condorFileName), 'w')
    f.write("Universe                = vanilla\n")
    f.write("Executable              = %s/%s\n" % (batchDir,jobFileName) )
    f.write("Log                     = /opt/ppd/scratch/xap79297/jobLogs/combined/%s__%s.log\n" % (timeStamp,signalDir) )
    f.write("Output                  = /opt/ppd/scratch/xap79297/jobLogs/combined/%s__%s.out\n" % (timeStamp,signalDir) )
    f.write("Error                   = /opt/ppd/scratch/xap79297/jobLogs/combined/%s__%s.err\n" % (timeStamp,signalDir) )
    f.write("Request_memory          = 1 GB\n")
    f.write("should_transfer_files   = YES\n")
    f.write("when_to_transfer_output = ON_EXIT_OR_EVICT\n")
    f.write("#\n")
    f.write("Getenv                  = True\n")
    f.write("Queue\n")
    f.close()

    g = open("%s/%s" % (batchDir,jobFileName), 'w')
    g.write("#!/bin/bash\n")
    g.write("cd %s\n" % cmsswBase)
    g.write("eval `scramv1 runtime -sh`\n")
    g.write("cd %s\n" % inputDir)
    if (len(nuisancesToFreeze) == 0):
        g.write("combine -M AsymptoticLimits --mass %s --keyword-value mSusy=%s %s\n" % (higgsMass, susyMass, fileToUse) )
    else:
        g.write("combine -M AsymptoticLimits --freezeParameter %s --mass %s --keyword-value mSusy=%s %s\n" % (nuisancesToFreeze, higgsMass, susyMass, fileToUse) )
    g.close()
    os.chmod("%s/%s" % (batchDir,jobFileName), 0755)

    # print "condor_submit %s/%s" % (batchDir,condorFileName)
    os.system("condor_submit %s/%s" % (batchDir,condorFileName) )
    print ""
    print ""