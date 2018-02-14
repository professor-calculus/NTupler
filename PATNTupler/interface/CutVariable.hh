#ifndef CUTVARIABLE_HH
#define CUTVARIABLE_HH

class CutVariable{

public:
    // constructor
    CutVariable(const std::string&, const std::vector<std::string>&);
	
	std::string GetCutVariableName(const std::string&);

private:
	std::string defaultVariableName;
	std::vector<std::string> listOfSystematics;
};

#endif