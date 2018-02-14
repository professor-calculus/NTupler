//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <fstream>
#include <sys/stat.h>
#include <map>

//ROOT HEADERS

//RAL PARTICLE HEADERS
#include "../interface/CutVariable.hh"


//--------constructor---------//
CutVariable::CutVariable(const std::string& defaultVariableNameDummy, const std::vector<std::string>& listOfSystematicsDummy) :
defaultVariableName(defaultVariableNameDummy),
listOfSystematics(listOfSystematicsDummy)
{
}


//-----------public-----------//
std::string CutVariable::GetCutVariableName(const std::string& appendSystematic)
{
	if (appendSystematic == "NO_SYS") return defaultVariableName;

	bool doesVariableGetAppended = false;
	for (auto systematic : listOfSystematics){
		if (systematic == appendSystematic){
			doesVariableGetAppended = true;
			break;
		}
	}

	if (doesVariableGetAppended == false) return defaultVariableName;
	
	else{
		std::string editVariableNameFront = defaultVariableName;
		std::string editVariableNameBack = "";

		for (size_t iCh = 0; iCh < editVariableNameFront.size(); ++iCh){
			if (editVariableNameFront[iCh] == '.') {
				editVariableNameBack = editVariableNameFront.substr(iCh);
				editVariableNameFront = editVariableNameFront.substr(0, iCh);
			}
		}
		return editVariableNameFront + "_" + appendSystematic + editVariableNameBack;
	}

}


//-----------private----------//