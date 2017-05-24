#ifndef DOUBLEBTAGWPS_H
#define DOUBLEBTAGWPS_H


namespace DoubleBTagWPs{

	double dbtNameToDouble(const std::string& dtbString){

	    double dbtDouble;
	    if (dtbString == "Off") dbtDouble = -100.0;
	    else if (dtbString == "Loose") dbtDouble = 0.3;
	    else if (dtbString == "Med") dbtDouble = 0.6;
	    else if (dtbString == "Tight") dbtDouble = 0.9;
	    else{
	        std::cout << "Have not specified the DBT string correctly\nExiting..." << std::endl;
	        exit(EXIT_FAILURE);
	    }
	    return dbtDouble;
	}

} // closes namespace 'DoubleBTagWPs'
#endif