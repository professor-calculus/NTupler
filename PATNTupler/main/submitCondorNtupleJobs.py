#!/usr/bin/python

import subprocess
import argparse
import os

parser = argparse.ArgumentParser()

parser.add_argument('--inputFiles', action='store', dest='inputList',
                    help='Text file with a list of root files')
parser.add_argument('--filesPerJob', action='store', dest='filesPerJob_par',
                    type=int, default =10, help='Number of root files per job')
parser.add_argument('--jobName', action='store', dest='jobType_par',
                    default='EE',help='Channel that we are running on')
parser.add_argument('--json', action='store', dest='json_par',
                    default='', help='Good lumi json file to use')
parser.add_argument('--output', action='store', dest='output_par',
                    help='name of the output root file')
results = parser.parse_args()

inputListFile = open(results.inputList)
fileList = inputListFile.readlines()

#Determine the number of jobs to be run. ie how many files per job. Put in a list
numFilesInJob = 1
trueOrFalse =""
tmpList = []
filesperJobList = []
for line in fileList:
    tmpList.append(line)
    if (numFilesInJob == results.filesPerJob_par):
        filesperJobList.append(tmpList)
        numFilesInJob = 1
        tmpList = []
    else:
        numFilesInJob = numFilesInJob + 1

#Add the remaining list
if (len(tmpList) > 0):
    filesperJobList.append(tmpList)

#make directories if they do not exit
for newDir in ["../tmp","../logs", "../deleteDir"]:
    subprocess.Popen(["mkdir", "-p", newDir], stdout=subprocess.PIPE)

subConJobFilename = "../tmp/subCondorJobs_"+results.jobType_par+".sh"
subJobs = open(subConJobFilename,"w")

#generate the executable for each job
jobNum = 0
for jobList in filesperJobList:
    lFile = "../tmp/"+results.jobType_par+"_"+str(jobNum)+".list"
    outFiles = open(lFile,"w")
    for line in jobList:
        outFiles.write(line)

    shellFileName = "../tmp/"+results.jobType_par+"_"+str(jobNum)+".sh"
    shellJob = open(shellFileName,"w")

    rootFile = results.jobType_par+"_"+str(jobNum)+".root"
    rootFileToScratch = "/scratch/"+rootFile
    thisDir = os.environ['PWD']
    baseDir = os.environ['CMSSW_BASE']

    cmd = "#!/bin/bash\n"
    cmd += "export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch/\n"
    cmd += "source $VO_CMS_SW_DIR/cmsset_default.sh\n"
    cmd += "cd "+baseDir+"/src/\n"
    cmd += "eval `scramv1 runtime -sh`\n"
    cmd += "cd "+os.environ['PWD']+"\n"
    cmd += "./nTupAna "+rootFileToScratch+" "+lFile+" "+results.json_par+"\n"
    cmd += "mv "+rootFileToScratch+" "+thisDir+"/"+rootFile+"\n"

    shellJob.write(cmd)
    os.chmod(shellFileName, 0755)

    #Generate condor config files to submit jobs
    jobid = results.jobType_par+"_"+str(jobNum)
    cFileName = "../tmp/"+jobid+".condor"
    condorFile = open(cFileName,"w")
    condorTemplate = open("../batchJobs/template.condor")

    condLineList = condorTemplate.readlines()
    for condLine in condLineList:
        condLine = condLine.replace("REPLACE_WITH_JOBID",jobid)
        condorFile.write(condLine)

    jobNum = jobNum + 1
    os.chmod(cFileName,0755)
    subJobs.write("condor_submit "+cFileName+"\n")
    os.chmod(subConJobFilename,0755)


