#ifndef SOFTDROPPUPPISF_H
#define SOFTDROPPUPPISF_H

#include "SoftDropPuppiSF.h"

// https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetWtagging
// TODO: fill in the 2017 factors

namespace SoftDropPuppiSF{

	double get_jetMassScaleSF(const unsigned int& yearOfRun)
    {
        if (yearOfRun == 2016) return 1.0;
        else if (yearOfRun == 2017) return 1.0;
        else{
            std::cout << "You have not provided a valid yearOfRun for the function SoftDropPuppiSF::get_jetMassScaleSF()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of 987654321.0" << std::endl;
            return 987654321.0;
        }
    }
	
    double get_jetMassScaleSFUp(const unsigned int yearOfRun)
    {
        if (yearOfRun == 2016) return 1.0 + 0.0094;
        else if (yearOfRun == 2017) return 1.0 + 0.0094;
        else{
            std::cout << "You have not provided a valid yearOfRun for the function SoftDropPuppiSF::get_jetMassScaleSFUp()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of 987654321.0" << std::endl;
            return 987654321.0;
        }
    }

	double get_jetMassScaleSFDown(const unsigned int yearOfRun)
    {
        if (yearOfRun == 2016) return 1.0 - 0.0094;
        else if (yearOfRun == 2017) return 1.0 - 0.0094;
        else{
            std::cout << "You have not provided a valid yearOfRun for the function SoftDropPuppiSF::get_jetMassScaleSFDown()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of 987654321.0" << std::endl;
            return 987654321.0;
        }
    }

	double get_jetMassResolutionSF(const unsigned int yearOfRun)
    {
        if (yearOfRun == 2016) return 1.0;
        else if (yearOfRun == 2017) return 1.0;
        else{
            std::cout << "You have not provided a valid yearOfRun for the function SoftDropPuppiSF::get_jetMassResolutionSF()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of 987654321.0" << std::endl;
            return 987654321.0;
        }
    }

	double get_jetMassResolutionSFUp(const unsigned int yearOfRun)
    {
        if (yearOfRun == 2016) return 1.0 + 0.20;
        else if (yearOfRun == 2017) return 1.0 + 0.20;
        else{
            std::cout << "You have not provided a valid yearOfRun for the function SoftDropPuppiSF::get_jetMassResolutionSFUp()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of 987654321.0" << std::endl;
            return 987654321.0;
        }
    }

	double get_jetMassResolutionSFDown(const unsigned int yearOfRun)
    {
        if (yearOfRun == 2016) return 1.0 - 0.20;
        else if (yearOfRun == 2017) return 1.0 - 0.20;
        else{
            std::cout << "You have not provided a valid yearOfRun for the function SoftDropPuppiSF::get_jetMassResolutionSFDown()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of 987654321.0" << std::endl;
            return 987654321.0;
        }
    }

	double get_mcMassResolution(const unsigned int yearOfRun)
    {
        if (yearOfRun == 2016) return 10.1;
        else if (yearOfRun == 2017) return 10.1;
        else{
            std::cout << "You have not provided a valid yearOfRun for the function SoftDropPuppiSF::get_mcMassResolution()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of 987654321.0" << std::endl;
            return 987654321.0;
        }
    }

} // closes namespace 'SoftDropPuppiSF'
#endif