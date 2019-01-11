#!/usr/bin/python

import subprocess
import os
import sys
import datetime

# Instructions:
# 1. Compile the code you wish to run
# 2. Set the USER INPUTS for this script
# 3. Run this script $ python submitCondorNtupleJobs_VJ.py or $ python $CMSSW_BASE/src/NTupler/PATNTupler/main/submitCondorNtupleJobs_VJ.py
# 4. cd into the outputDirectory/tmp and run the condor job (can use batchJobs/setoffJobs.py)

baseDir = os.environ['CMSSW_BASE']
###########################################################################################################
############## USER INPUTS ################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

executable = "nTupAnaNMSSM" # wrt 'main' directory
code = "mainNMSSM.cc" # wrt 'main' directory
inputFileListPath = baseDir + "/src/NTupler/PATNTupler/fileLists/8_0_31/NAMEXYZ.list"
outputDirectory = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/NAMEXYZ/" # has to be the full path
sampleType = "SIGNAL" # choose from SIGNAL, DATA, TTJETS, OTHER_MC
yearOfRun = 2016
filesPerJob = 30
logDirectoryBase = "/opt/ppd/scratch/titterton/jobLogs/flatTrees/"

###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################
###########################################################################################################

if (sampleType != "SIGNAL" and sampleType != "DATA" and sampleType != "TTJETS" and sampleType != "OTHER_MC"):
    print "you have not provided a valid sample type bro, exiting..."
    sys.exit()

if (yearOfRun != 2016 and yearOfRun != 2017):
    print "you have not provided a valid year of run pal, exiting..."
    sys.exit()

if outputDirectory[-1] == "/":
    outputDirectory = outputDirectory[:-1]

dataname = ""
for iChar in range (len(outputDirectory)-1, 0, -1):
    if outputDirectory[iChar] == "/":
        dataname = outputDirectory[iChar+1:]
        break
dataname = dataname + "_" + '{:%Y_%m_%d-%H_%M_%S}'.format(datetime.datetime.now())
logDirectory = os.path.join(logDirectoryBase,dataname)

if len(baseDir) == 0:
    print "You are not in a CMSSW environment"
    print "Exiting..."
    sys.exit()
# if baseDir[0:4] == "/net":
#     baseDir = baseDir[4:]

# Check that the output directory does not already exist
if os.path.isdir(outputDirectory) == True:
    print "The output directory, " + outputDirectory + " already exists"
    print "Exiting..."
    sys.exit()
# Check that the log directory does not already exist
# if os.path.isdir(logDirectory) == True:
#     print "The log directory, " + logDirectory + " already exists"
#     print "Exiting..."
#     sys.exit()
# Check that the fileList exists before continuing
if os.path.exists(inputFileListPath) == False:
    print "The fileList, " + inputFileListPath + " doesn't exist"
    print "Exiting..."
    sys.exit()

# Make the new directory(s)
os.system("mkdir -p %s" % outputDirectory)
os.system("mkdir %s/tmp" % outputDirectory)
os.system("cp %s/src/NTupler/PATNTupler/main/%s %s" % (baseDir,code,outputDirectory))
os.system("cp %s %s" % (inputFileListPath,outputDirectory))
os.system("mkdir -p %s" % logDirectory)


# Create list of files for each job
inputFileList = open(inputFileListPath).readlines()

numFilesInJob = 1
tmpList = []
filesPerJobList = []
for line in inputFileList:
    tmpList.append(line)
    if (numFilesInJob == filesPerJob):
        filesPerJobList.append(tmpList)
        numFilesInJob = 1
        tmpList = []
    else:
        numFilesInJob = numFilesInJob + 1
if (len(tmpList) > 0):
    filesPerJobList.append(tmpList)


# Create a bash script that will submit all the jobs
submitCondorJobsFilename = outputDirectory + "/tmp/submitCondorJobs_" + executable + ".sh"
submitCondorJobs = open(submitCondorJobsFilename,"w")


# Create the executable script for each job
jobNum = 0
for jobList in filesPerJobList:
    
    jobListFileName = outputDirectory + "/tmp/job_" + str(jobNum) + ".list"
    jobListFile = open(jobListFileName,"w")
    for line in jobList:
        jobListFile.write(line)

    shellJobName = outputDirectory + "/tmp/job_" +str(jobNum) + ".sh"
    shellJob = open(shellJobName,"w")

    cmd = "#!/bin/bash\n"
    cmd += baseDir + "/src/NTupler/PATNTupler/main/" + executable + " flatTree_" + str(jobNum) + ".root " + jobListFileName + " " + sampleType + " " + str(yearOfRun) + " batch\n"
    cmd += "if [ $? -eq 0 ]\n"
    cmd += "then\n"
    cmd += "    echo\n"
    cmd += "    echo JOB to ROOT FILE SUCCESSFUL: COPYING FILE BACK TO LOCAL\n"
    cmd += "    # cp flatTree_" + str(jobNum) + ".root ../../.\n" # you don't explicitly need to do the copying so I have commented out this bit
    cmd += "else\n"
    cmd += "    echo\n"
    cmd += "    echo JOB to ROOT FILE UNSUCCESSFUL: NOT COPYING FILE BACK TO LOCAL\n"
    cmd += "    rm flatTree_" + str(jobNum) + ".root\n"
    cmd += "fi\n"

    shellJob.write(cmd)
    os.chmod(shellJobName, 0755)

    # Create htcondor config files for each individual job submission
    condorFileName = outputDirectory + "/tmp/job_" + str(jobNum) + ".condor"
    condorFile = open(condorFileName,"w")
    condorTemplate = open("%s/src/NTupler/PATNTupler/batchJobs/template_VJ.condor" % baseDir)
    condorLineList = condorTemplate.readlines()
    for condorLine in condorLineList:
        jobid = "job_" + str(jobNum)
        condorLine = condorLine.replace("REPLACE_WITH_JOBID",jobid)
        condorLine = condorLine.replace("REPLACE_LOG_DIRECTORY",logDirectory)
        condorFile.write(condorLine)
    os.chmod(condorFileName,0755)

    submitCondorJobs.write("condor_submit job_" + str(jobNum) + ".condor" + "\n")
    jobNum = jobNum + 1

os.chmod(submitCondorJobsFilename,0755)
