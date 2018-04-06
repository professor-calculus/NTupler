#ifndef ISRWEIGHTS_H
#define ISRWEIGHTS_H

// get ISR weighting for a given number of ISR jets

namespace IsrWeights{


	double GetWeight(const unsigned int& nISR){

		std::vector<double> weightVec;

		weightVec.push_back(1.000); // this is for 0 jets
		weightVec.push_back(0.920); // etc.
		weightVec.push_back(0.821);
		weightVec.push_back(0.715);
		weightVec.push_back(0.662);
		weightVec.push_back(0.561);
		weightVec.push_back(0.511); // this is for 6+ jets

		if (nISR >= weightVec.size()){
			std::cout << "\nWARNING: nISR index greater that weightVec entries... Returing dummy value of -777.77\n" << std::endl;
			return -777.77;
		}

		return weightVec[nISR];

	} // closes function GetWeight


} // closes namespace 'IsrWeights'
#endif