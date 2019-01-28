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
# nuisancesToFreeze = "signalPdfBOTH"
# nuisancesToFreeze = "isrReweightBOTH"
# nuisancesToFreeze = "SigDbtTag2016,SigDbtTag2017"
# nuisancesToFreeze = "TtDbtTag2016,TtDbtTag2017"
# nuisancesToFreeze = "XS_TTJets2016,XS_TTJets0L2017,XS_TTJets1L2017,XS_TTJets2L2017"
# nuisancesToFreeze = "XS_ZJets2016,XS_ZJets2017"
# nuisancesToFreeze = "XS_WJets2016,XS_WJets2017"
# nuisancesToFreeze = "luminosity2016,luminosity2017"
# nuisancesToFreeze = "jecAKXUncBOTH"
# nuisancesToFreeze = "jerAKXUnc2016,jerAKXUnc2017"
# nuisancesToFreeze = "jmsUncBOTH"
# nuisancesToFreeze = "jmrUncBOTH"
# nuisancesToFreeze = "prefire2016,prefire2017"

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

    if signalDir[0:1] != "P":
        continue

    susyMass = signalDir[3:7]
    ratio = signalDir[10:]

    print(susyMass, ratio)

    if ratio == 'R0p99':
        lspMass = '1'
    elif ratio == 'R0p555':
        lspMass = '100'
    elif ratio == 'R0p384':
        lspMass = '200'
    else:
        lspMass = -1
        exit(1)

    fileToUseTXT = os.path.join(inputDir,signalDir,"allbins.txt")
    fileToUseROOT = os.path.join(inputDir,signalDir,"allbins.root")
    
    print "RUNNING COMBINE FOR " + susyMass + "squark, " + lspMass + "LSP"
    print "nuisance to freeze: " + nuisancesToFreeze

    condorFileName = signalDir + ".condor"
    jobFileName = signalDir + ".sh"

    f = open("%s/%s" % (batchDir,condorFileName), 'w')
    f.write("Universe                = vanilla\n")
    f.write("Executable              = %s/%s\n" % (batchDir,jobFileName) )
    f.write("Log                     = /opt/ppd/scratch/titterton/jobLogs/combined/%s__%s.log\n" % (timeStamp,signalDir) )
    f.write("Output                  = /opt/ppd/scratch/titterton/jobLogs/combined/%s__%s.out\n" % (timeStamp,signalDir) )
    f.write("Error                   = /opt/ppd/scratch/titterton/jobLogs/combined/%s__%s.err\n" % (timeStamp,signalDir) )
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
    g.write("text2workspace.py %s\n" % fileToUseTXT)
    if (len(nuisancesToFreeze) == 0):
        g.write("combine -M AsymptoticLimits --mass %s --keyword-value mSusy=%s %s\n" % (lspMass, susyMass, fileToUseROOT) )
    else:
        g.write("combine -M AsymptoticLimits --freezeParameter %s --mass %s --keyword-value mSusy=%s %s\n" % (nuisancesToFreeze, lspMass, susyMass, fileToUseROOT) )
    g.close()
    os.chmod("%s/%s" % (batchDir,jobFileName), 0755)

    # print "condor_submit %s/%s" % (batchDir,condorFileName)
    os.system("condor_submit %s/%s" % (batchDir,condorFileName) )
    print ""
    print ""
