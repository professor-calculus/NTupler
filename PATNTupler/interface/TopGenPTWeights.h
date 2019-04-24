#ifndef TOGENPTWEIGHTS_H
#define TOPGENWEIGHTS_H

#include <TMath.h>

// get ISR weighting for a given number of ISR jets
// TODO: fill in 2017 factors

namespace TopGenPTWeights{


	double GetPowhegWeight(const float& top1_pT, const float& top2_pT){

		double sf1_, sf2_;

        sf1_ = TMath::Exp( 0.0615 - (0.0005*top1_pT) );
        sf2_ = TMath::Exp( 0.0615 - (0.0005*top2_pT) );
        
        double powhegWeight = TMath:Sqrt(sf1_ * sf2_);
        return powhegWeight;

	} // closes function GetWeight


} // closes namespace 'TopGenPTWeights'
#endif