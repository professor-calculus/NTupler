#ifndef DOUBLEBTAGSF_H
#define DOUBLEBTAGSF_H

#include "DoubleBTagWPs.h"

// Enter the fatJet's info and get the DBT SF info back
// info from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco

namespace DoubleBTagSF{


	void editLooseSignal(std::vector<double>& looseScaleFactorVec, const double& fatJet_pt){

		if (fatJet_pt < 250.0){
			looseScaleFactorVec[0] *= (0.96 - 2 * 0.02);
			looseScaleFactorVec[1] *= (0.96);
			looseScaleFactorVec[2] *= (0.96 + 2 * 0.03);
		}
		else if (fatJet_pt < 350.0){
			looseScaleFactorVec[0] *= (0.96 - 0.02);
			looseScaleFactorVec[1] *= (0.96);
			looseScaleFactorVec[2] *= (0.96 + 0.03);
		}
		else if (fatJet_pt < 430.0){
			looseScaleFactorVec[0] *= (1.00 - 0.03);
			looseScaleFactorVec[1] *= (1.00);
			looseScaleFactorVec[2] *= (1.00 + 0.04);
		}
		else if (fatJet_pt < 840.0){
			looseScaleFactorVec[0] *= (1.01 - 0.04);
			looseScaleFactorVec[1] *= (1.01);
			looseScaleFactorVec[2] *= (1.01 + 0.02);
		}
		else{
			looseScaleFactorVec[0] *= (1.01 - 2 * 0.04);
			looseScaleFactorVec[1] *= (1.01);
			looseScaleFactorVec[2] *= (1.01 + 2 * 0.02);
		}
	} // closes function 'editLooseSignal'


	void editLooseTTJets(std::vector<double>& looseScaleFactorVec, const double& fatJet_pt){

		if (fatJet_pt < 250.0){
			looseScaleFactorVec[0] *= (1.044 - 2 * 0.028);
			looseScaleFactorVec[1] *= (1.044);
			looseScaleFactorVec[2] *= (1.044 + 2 * 0.028);
		}
		else if (fatJet_pt < 350.0){
			looseScaleFactorVec[0] *= (1.044 - 0.028);
			looseScaleFactorVec[1] *= (1.044);
			looseScaleFactorVec[2] *= (1.044 + 0.028);
		}
		else if (fatJet_pt < 430.0){
			looseScaleFactorVec[0] *= (1.074 - 0.052);
			looseScaleFactorVec[1] *= (1.074);
			looseScaleFactorVec[2] *= (1.074 + 0.052);
		}
		else if (fatJet_pt < 700.0){
			looseScaleFactorVec[0] *= (1.119 - 0.079);
			looseScaleFactorVec[1] *= (1.119);
			looseScaleFactorVec[2] *= (1.119 + 0.079);
		}
		else{
			looseScaleFactorVec[0] *= (1.119 - 2 * 0.079);
			looseScaleFactorVec[1] *= (1.119);
			looseScaleFactorVec[2] *= (1.119 + 2 * 0.079);
		}
	} // closes function 'editLooseTTJets'


	std::vector<double> GetLooseScaleFactors(const std::string& sampleType, const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt){

		std::vector<double> looseScaleFactorVec = {1.0, 1.0, 1.0}; // dbtLooseDown, dbtLooseNominal, dbtLooseUp  
		double dbt_LooseWP = DoubleBTagWPs::dbtNameToDouble("Loose");
		if (sampleType != "SIGNAL" && sampleType != "TTJETS") return looseScaleFactorVec;
		if (fatJetA_dbt < dbt_LooseWP || fatJetB_dbt < dbt_LooseWP) return looseScaleFactorVec;

		if (sampleType == "SIGNAL"){
			editLooseSignal(looseScaleFactorVec, fatJetA_pt);
			editLooseSignal(looseScaleFactorVec, fatJetB_pt);
		}

		if (sampleType == "TTJETS"){
			editLooseTTJets(looseScaleFactorVec, fatJetA_pt);
			editLooseTTJets(looseScaleFactorVec, fatJetB_pt);
		}

		return looseScaleFactorVec;

	} // closes function 'GetLooseScaleFactors'


} // closes namespace 'DoubleBTagSF'
#endif