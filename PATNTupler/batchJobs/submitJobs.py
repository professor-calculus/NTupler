#!/usr/bin/python

import subprocess
import argparse
import os

parser = argparse.ArgumentParser()

parser.add_argument('--inputFiles', action='store', dest='inputList',
                    help='Text file with a list of root files')
parser.add_argument('--dataLumi', action='store', dest='dataLumi',default=1.0,
                    type = float, help='equivalent Lumi in Data to match in MC')
parser.add_argument('--crossSec', action='store', dest='crossSec',default=1.0,
                    type = float, help='MC cross section')
parser.add_argument('--numGenerated', action='store', dest='numGen', default=1.0,
                    type = float, help='Number of MC events generated')
parser.add_argument('--MC', action='store_true', dest='isMC',
                    help='Is the data sample MC')
parser.add_argument('--filesPerJob', action='store', dest='filesPerJob_par',
                    type=int, default =10, help='Number of root files per job')
parser.add_argument('--analyser', action='store', dest='analyser_par',
                    default='EMuAnalyser',help='EDAnalyser to run')
parser.add_argument('--mode', action='store', dest='jobType_par',
                    default='TTBAR',help='Channel that we are running on')
parser.add_argument('--globalTag', action='store', dest='gt_par',
                    default='MCRUN2_74_V9A',help='Global tag')
parser.add_argument('--json', action='store', dest='json_par',
                    default='del.txt', help='Good lumi json file to use')
parser.add_argument('--outputEmuVars', action='store_true', dest='dumpeMuVars_par',
                   default=False, help='Write out the full emu vars to ntuple')
parser.add_argument('--muonPt', action='store', dest='muonPt_par',
                    type=float, default =40.0, help='Muon pt cut')
parser.add_argument('--elecEt', action='store', dest='elecEt_par',
                    type=float, default =40.0, help='Electron Et cut')
results = parser.parse_args()

subConJobFilename = "../tmp/subCondorJobs_"+results.jobType_par+".sh"
subJobs = open(subConJobFilename,"w")

inputListFile = open(results.inputList)
fileList = inputListFile.readlines()

#calc MC weight

weight = 1.0

if (results.isMC):
    weight = ( results.dataLumi * results.crossSec)/ results.numGen


#Copy json file to config area
if results.json_par != "del.txt" :
    subprocess.Popen(["cp", "../json/"+results.json_par, "../cfg/."], stdout=subprocess.PIPE)

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

#print filesperJobList

inputconfigFile = open("../plugins/conFigTemplate.py") #tweak this with an option to be plugins or src
configFileList = inputconfigFile.readlines()


#generate the config file for each job
jobNum = 0
for jobList in filesperJobList:
    py_config = "../cfg/"+results.jobType_par+"_"+str(jobNum)+"_cfg.py"
    rootFileBaseName = results.jobType_par+"_"+str(jobNum)+".root"
    rootFileToScratch = ""
    outRootFile = ""    
    outFile = open(py_config,"w")
    for line in configFileList:
        line = line.replace("ANALYSER_INSERTEDHERE",results.analyser_par)        
        line = line.replace("MCWEIGHT_INSERTEDHERE", str(weight))
        line = line.replace("GLOBALTAG_INSERTEDHERE", results.gt_par)
        line = line.replace("GOODLUMILIST_INSERTEDHERE", results.json_par)
        line = line.replace("MUONPTCUT_INSERTEDHERE", str(results.muonPt_par))
        line = line.replace("ELECTRONETCUT_INSERTEDHERE", str(results.elecEt_par))
        if ("ISTHISMCORNOT_INSERTEDHERE" in line):
            if (results.isMC):
                trueOrFalse = "True"
                line = line.replace("ISTHISMCORNOT_INSERTEDHERE",trueOrFalse)
            else:
                trueOrFalse = "False"
                line = line.replace("ISTHISMCORNOT_INSERTEDHERE",trueOrFalse)
        if ("OUTPUTEMUVARS_INSERTEDHERE" in line):
            if (results.dumpeMuVars_par):
                trueOrFalse2 = "True"
                line = line.replace("OUTPUTEMUVARS_INSERTEDHERE",trueOrFalse2)
            else:
                trueOrFalse2 = "False"
                line = line.replace("OUTPUTEMUVARS_INSERTEDHERE",trueOrFalse2)
        if ("LISTOFFILESINSERTEDHERE" in line):
            listOfFiles = ""
            stringGlue = ""
            filenum = 0
            rangeOfNames = []
            for fileString in jobList:
                if (filenum == 0):
                    stringGlue = "'"
                    fli = fileString.split("/")
                    #rootFileBaseName = fli[-4]+"_"+fli[-3]+"_"+fli[-1].rstrip()
                    rangeOfNames.append(fli[-4]+"_"+fli[-3]+"_"+fli[-1].rstrip())
                else:
                    stringGlue = ",'"
                    fli = fileString.split("/")
                    tmpString = fli[-4]+"_"+fli[-3]+"_"+fli[-1].rstrip()
                    tmpString = tmpString.replace(".root","")
                    rangeOfNames.append(tmpString)
                    #rootFileBaseName = tmpString+"_"+rootFileBaseName
                filenum = filenum + 1
                fileString = fileString.rstrip()
                listOfFiles = listOfFiles + stringGlue +fileString+"'"
            if (len(rangeOfNames) == 1):
                rootFileBaseName = rangeOfNames[0]
            else:
                rootFileBaseName = rangeOfNames[-1]+"_"+rangeOfNames[0]
            line = line.replace("'LISTOFFILESINSERTEDHERE'",listOfFiles)
        if ("OUTPUTFILENAME_INSERTEDHERE" in line):
            rootFileToScratch = "/scratch/"+rootFileBaseName
            outRootFile = "../output/"+rootFileBaseName 
            line = line.replace("OUTPUTFILENAME_INSERTEDHERE", rootFileToScratch)
        outFile.write(line)

#Generate the shell script which will run the job

    shellFileName = "../tmp/"+results.jobType_par+"_"+str(jobNum)+".sh"
    shellJob = open(shellFileName,"w")

    cmd = "#!/bin/bash\n"
    cmd += "export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch/\n"
    cmd += "source $VO_CMS_SW_DIR/cmsset_default.sh\n"
    cmd += "cd /opt/ppd/month/olaiya/CMS/analysis/CMSSW_7_4_8_patch1/src/\n"
    cmd += "eval `scramv1 runtime -sh`\n"
    cmd += "cd /opt/ppd/month/olaiya/CMS/analysis/CMSSW_7_4_8_patch1/src/NTupler/PATNTupler/cfg\n\n"
    cmd += "cmsRun "+ py_config+"\n"
    cmd += "mv "+rootFileToScratch+" "+outRootFile+"\n"

    shellJob.write(cmd)
    os.chmod(shellFileName, 0755)

#Generate condor config files to submit jobs
    jobid = results.jobType_par+"_"+str(jobNum)
    cFileName = "../tmp/"+jobid+".condor"
    condorFile = open(cFileName,"w")
    condorTemplate = open("template.condor")

    condLineList = condorTemplate.readlines()
    for condLine in condLineList:
        condLine = condLine.replace("REPLACE_WITH_JOBID",jobid)
        condorFile.write(condLine)

    jobNum = jobNum + 1
    os.chmod(cFileName,0755)
    subJobs.write("condor_submit "+cFileName+"\n")
    os.chmod(subConJobFilename,0755)
    
