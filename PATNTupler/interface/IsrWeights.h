#ifndef ISRWEIGHTS_H
#define ISRWEIGHTS_H

// get ISR weighting for a given number of ISR jets
// TODO: fill in 2017 factors

namespace IsrWeights{


	double GetWeight(const unsigned int& nISR, const unsigned int& yearOfRun){

		if (yearOfRun == 2016){

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
		}

		else if (yearOfRun == 2017){

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
				return -878.78;
			}

			return weightVec[nISR];
		}		

		else{
            std::cout << "You have not provided a valid yearOfRun for the function IsrWeights::GetWeights()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of -987654321.0" << std::endl;
            return -987654321.0;
        }

	} // closes function GetWeight


} // closes namespace 'IsrWeights'
#endif