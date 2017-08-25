#ifndef DOUBLEBTAGWPS_H
#define DOUBLEBTAGWPS_H


namespace DoubleBTagWPs{

	double dbtNameToDouble(const std::string& dbtString){

	    double dbtDouble;
	    if (dbtString == "Off") dbtDouble = -100.0;
	    else if (dbtString == "Loose") dbtDouble = 0.3;
	    else if (dbtString == "Med1") dbtDouble = 0.6;
	    else if (dbtString == "Med2") dbtDouble = 0.8;
	    else if (dbtString == "Tight") dbtDouble = 0.9;
	    else if (dbtString == "Max") dbtDouble = 100.0;
	    // else if (dbtString == "IDBTCv1") dbtDouble = -0.35;
	    // else if (dbtString == "IDBTCv2") dbtDouble = -0.60;
		else if (dbtString == "IDBTCv10") dbtDouble = 0.40;
		else if (dbtString == "IDBTCv11") dbtDouble = -0.80;
	    else{
	        std::cout << "Have not specified the DBT string correctly\nExiting..." << std::endl;
	        exit(EXIT_FAILURE);
	    }
	    return dbtDouble;
	}

} // closes namespace 'DoubleBTagWPs'
#endif