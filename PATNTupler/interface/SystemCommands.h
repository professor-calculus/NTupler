#ifndef SYSTEMCOMMANDS_H
#define SYSTEMCOMMANDS_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


namespace SystemCommands{

	std::string GetCmsswBase()
	{
	    std::array<char, 128> buffer;
	    std::string result;
	    std::shared_ptr<FILE> pipe(popen("echo $CMSSW_BASE", "r"), pclose);
	    if (!pipe) throw std::runtime_error("popen() failed!");
	    while (!feof(pipe.get())) {
	        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
	            result += buffer.data();
	    }
	    result = result.substr(0, result.size()-1);
	    return result;
	}

	std::string GetPwd()
	{
	    std::array<char, 128> buffer;
	    std::string result;
	    std::shared_ptr<FILE> pipe(popen("pwd", "r"), pclose);
	    if (!pipe) throw std::runtime_error("popen() failed!");
	    while (!feof(pipe.get())) {
	        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
	            result += buffer.data();
	    }
	    result = result.substr(0, result.size()-1);
	    return result;
	}

} // closes namespace 'SystemCommands'
#endif