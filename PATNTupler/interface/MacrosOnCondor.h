#ifndef MACROSONCONDOR_H
#define MACROSONCONDOR_H

#include "TimeStamp.h"
#include "SystemCommands.h"

namespace MacrosOnCondor{

	void CreateCondorFile(const std::string& outputDir, const std::string& executable, const std::string& logBaseDir)
	{
		std::string dateStamp = TimeStamp::GetTimeStamp();
		
		std::string logDir = logBaseDir + "/" + executable + "__" + dateStamp;
		std::system( Form("mkdir %s", logDir.c_str()) );

		// put the executable in a unique tmp location
		std::string pathToMacrosExe = SystemCommands::GetCmsswBase();
		pathToMacrosExe += "/src/NTupler/PATNTupler/macros/";
		pathToMacrosExe += executable;

		std::string pathToTmpExe = SystemCommands::GetCmsswBase();
		pathToTmpExe += "/src/NTupler/PATNTupler/tmp/";
		pathToTmpExe += "macroExe_" + dateStamp + "/";
		std::system( Form("mkdir %s", pathToTmpExe.c_str()) );
		std::system( Form("cp %s %s", pathToMacrosExe.c_str(), pathToTmpExe.c_str()) );
		pathToTmpExe += executable;

		std::ofstream condorFile;
		condorFile.open( Form("%s/job.condor", outputDir.c_str()) );
		condorFile << "Universe               = vanilla\n";
		condorFile << "Executable             = job.sh\n";
		condorFile << Form("Log                    = %s/job.log\n", logDir.c_str());
		condorFile << Form("Output                 = %s/job.out\n", logDir.c_str());
		condorFile << Form("Error                  = %s/job.err\n", logDir.c_str());
		// condorFile << "Request_memory         = 1 GB\n";
		condorFile << "Request_memory         = 10 GB\n";
		condorFile << "Transfer_input_files   = job.sh, " << pathToTmpExe << "\n";
		condorFile << "\n";
		condorFile << "should_transfer_files	= YES\n";
		condorFile << "when_to_transfer_output	= ON_EXIT_OR_EVICT\n";
		condorFile << "\n";
		condorFile << "Getenv                 = True\n";
		condorFile << "Queue\n";
		condorFile.close();

	    return;
	} // closes function "CreateCondorFile"


	void CreateExeFile(const std::string& outputDir, const std::string& executable)
	{
		std::ofstream exeFile;
		exeFile.open( Form("%s/job.sh", outputDir.c_str()) );
		exeFile << "#!/bin/bash\n";
		exeFile << Form("./%s batchRUN\n", executable.c_str());
		exeFile << "cp *.pdf *.csv *.root ../../.\n";
		exeFile.close();

	    return;
	} // closes function "CreateExeFile"


	void CreateSubmitFile(const std::string& outputDir)
	{
		std::string currentDir = SystemCommands::GetPwd();

		std::string pathToTmpFile = SystemCommands::GetCmsswBase();
		pathToTmpFile += "/src/NTupler/PATNTupler/tmp/submitTheMacroJobToCondor.sh"; 

		std::ofstream submitFile;
		submitFile.open( pathToTmpFile.c_str() );
		submitFile << "#!/bin/bash\n";
		submitFile << Form("cd %s\n", outputDir.c_str());
		submitFile << "condor_submit job.condor\n";
		submitFile << Form("cd %s\n", currentDir.c_str());
		submitFile.close();		

		return;
	} // closes function "CreateSubmitFile"


	void SubmitJob(const std::string& outputDir, const std::string& executable, const std::string& logBaseDir) // executable wrt 'macros' dir
	{
		CreateCondorFile(outputDir.c_str(), executable.c_str(), logBaseDir.c_str());
		CreateExeFile(outputDir.c_str(), executable.c_str());
		CreateSubmitFile(outputDir.c_str());

		std::string pathToTmpFile = SystemCommands::GetCmsswBase();
		pathToTmpFile += "/src/NTupler/PATNTupler/tmp/submitTheMacroJobToCondor.sh"; 

		std::system(Form("source %s", pathToTmpFile.c_str()));
		std::system(Form("rm %s", pathToTmpFile.c_str()));

	} // closes function "SubmitJob"


} // closes namespace 'MacrosOnCondor'
#endif