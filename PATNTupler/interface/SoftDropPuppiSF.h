#ifndef SOFTDROPPUPPISF_H
#define SOFTDROPPUPPISF_H

#include "SoftDropPuppiSF.h"

// https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetWtagging

namespace SoftDropPuppiSF{

	double get_jetMassScaleSF() {return 1.0;}
	double get_jetMassScaleSFUp() {return 1.0 + 0.0094;}
	double get_jetMassScaleSFDown() {return 1.0 - 0.0094;}
	double get_jetMassResolutionSF() {return 1.0;}
	double get_jetMassResolutionSFUp() {return 1.0 + 0.20;}
	double get_jetMassResolutionSFDown() {return 1.0 - 0.20;}
	double get_mcMassResolution() {return 10.1;}

} // closes namespace 'SoftDropPuppiSF'
#endif