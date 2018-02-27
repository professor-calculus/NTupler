#ifndef DOUBLEBTAGSF_H
#define DOUBLEBTAGSF_H

#include "DoubleBTagWPs.h"

// Enter the fatJet's info and get the DBT SF info back
// info from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco
// Custom = Med1 SF's, if need be increase errors to span Loose & Med2 SF's

namespace DoubleBTagSF{


	// void editLooseSfSignal(std::vector<double>& looseScaleFactorVec, const double& fatJet_pt){

	// 	if (fatJet_pt < 250.0){
	// 		looseScaleFactorVec[0] *= (0.96 - 2 * 0.02);
	// 		looseScaleFactorVec[1] *= (0.96);
	// 		looseScaleFactorVec[2] *= (0.96 + 2 * 0.03);
	// 	}
	// 	else if (fatJet_pt < 350.0){
	// 		looseScaleFactorVec[0] *= (0.96 - 0.02);
	// 		looseScaleFactorVec[1] *= (0.96);
	// 		looseScaleFactorVec[2] *= (0.96 + 0.03);
	// 	}
	// 	else if (fatJet_pt < 430.0){
	// 		looseScaleFactorVec[0] *= (1.00 - 0.03);
	// 		looseScaleFactorVec[1] *= (1.00);
	// 		looseScaleFactorVec[2] *= (1.00 + 0.04);
	// 	}
	// 	else if (fatJet_pt < 840.0){
	// 		looseScaleFactorVec[0] *= (1.01 - 0.04);
	// 		looseScaleFactorVec[1] *= (1.01);
	// 		looseScaleFactorVec[2] *= (1.01 + 0.02);
	// 	}
	// 	else{
	// 		looseScaleFactorVec[0] *= (1.01 - 2 * 0.04);
	// 		looseScaleFactorVec[1] *= (1.01);
	// 		looseScaleFactorVec[2] *= (1.01 + 2 * 0.02);
	// 	}
	// } // closes function 'editLooseSfSignal'


	// void editLooseSfTTJets(std::vector<double>& looseScaleFactorVec, const double& fatJet_pt){

	// 	if (fatJet_pt < 250.0){
	// 		looseScaleFactorVec[0] *= (1.044 - 2 * 0.028);
	// 		looseScaleFactorVec[1] *= (1.044);
	// 		looseScaleFactorVec[2] *= (1.044 + 2 * 0.028);
	// 	}
	// 	else if (fatJet_pt < 350.0){
	// 		looseScaleFactorVec[0] *= (1.044 - 0.028);
	// 		looseScaleFactorVec[1] *= (1.044);
	// 		looseScaleFactorVec[2] *= (1.044 + 0.028);
	// 	}
	// 	else if (fatJet_pt < 430.0){
	// 		looseScaleFactorVec[0] *= (1.074 - 0.052);
	// 		looseScaleFactorVec[1] *= (1.074);
	// 		looseScaleFactorVec[2] *= (1.074 + 0.052);
	// 	}
	// 	else if (fatJet_pt < 700.0){
	// 		looseScaleFactorVec[0] *= (1.119 - 0.079);
	// 		looseScaleFactorVec[1] *= (1.119);
	// 		looseScaleFactorVec[2] *= (1.119 + 0.079);
	// 	}
	// 	else{
	// 		looseScaleFactorVec[0] *= (1.119 - 2 * 0.079);
	// 		looseScaleFactorVec[1] *= (1.119);
	// 		looseScaleFactorVec[2] *= (1.119 + 2 * 0.079);
	// 	}
	// } // closes function 'editLooseSfTTJets'


	void editCustomSfSignal(std::vector<double>& customScaleFactorVec, const double& fatJet_pt){

		if (fatJet_pt < 250.0){
			customScaleFactorVec[0] *= 0.89;
			customScaleFactorVec[1] *= 0.93;
			customScaleFactorVec[2] *= 0.99;
		}
		else if (fatJet_pt < 350.0){
			customScaleFactorVec[0] *= 0.91;
			customScaleFactorVec[1] *= 0.93;
			customScaleFactorVec[2] *= 0.96;
		}
		else if (fatJet_pt < 430.0){
			customScaleFactorVec[0] *= 0.98;
			customScaleFactorVec[1] *= 1.01;
			customScaleFactorVec[2] *= 1.04;
		}
		else if (fatJet_pt < 840.0){
			customScaleFactorVec[0] *= 0.92;
			customScaleFactorVec[1] *= 0.99;
			customScaleFactorVec[2] *= 1.01;
		}
		else{
			customScaleFactorVec[0] *= 0.91;
			customScaleFactorVec[1] *= 0.99;
			customScaleFactorVec[2] *= 1.03;
		}
	} // closes function 'editCustomSfSignal'


	void editCustomSfTTJets(std::vector<double>& customScaleFactorVec, const double& fatJet_pt){

		if (fatJet_pt < 250.0){
			customScaleFactorVec[0] *= 0.961;
			customScaleFactorVec[1] *= 1.029;
			customScaleFactorVec[2] *= 1.097;
		}
		else if (fatJet_pt < 350.0){
			customScaleFactorVec[0] *= 0.995;
			customScaleFactorVec[1] *= 1.029;
			customScaleFactorVec[2] *= 1.063;
		}
		else if (fatJet_pt < 700.0){
			customScaleFactorVec[0] *= 1.074;
			customScaleFactorVec[1] *= 1.156;
			customScaleFactorVec[2] *= 1.220;
		}
		else{
			customScaleFactorVec[0] *= 1.028;
			customScaleFactorVec[1] *= 1.156;
			customScaleFactorVec[2] *= 1.284;
		}
	} // closes function 'editCustomSfTTJets'


	// std::vector<double> GetLooseScaleFactors(const std::string& sampleType, const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt){

	// 	std::vector<double> looseScaleFactorVec = {1.0, 1.0, 1.0}; // dbtLooseDown, dbtLooseNominal, dbtLooseUp  
	// 	double dbt_LooseWP = DoubleBTagWPs::dbtNameToDouble("Loose");
	// 	if (sampleType != "SIGNAL" && sampleType != "TTJETS") return looseScaleFactorVec;
	// 	if (fatJetA_dbt < dbt_LooseWP || fatJetB_dbt < dbt_LooseWP) return looseScaleFactorVec;

	// 	if (sampleType == "SIGNAL"){
	// 		editLooseSfSignal(looseScaleFactorVec, fatJetA_pt);
	// 		editLooseSfSignal(looseScaleFactorVec, fatJetB_pt);
	// 	}

	// 	if (sampleType == "TTJETS"){
	// 		editLooseSfTTJets(looseScaleFactorVec, fatJetA_pt);
	// 		editLooseSfTTJets(looseScaleFactorVec, fatJetB_pt);
	// 	}

	// 	return looseScaleFactorVec;

	// } // closes function 'GetLooseScaleFactors'


	std::vector<double> GetCustomScaleFactors(const std::string& sampleType, const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt){

		std::vector<double> customScaleFactorVec = {1.0, 1.0, 1.0}; // dbtCustomDown, dbtCustomNominal, dbtCustomUp  

		if (sampleType != "SIGNAL" && sampleType != "TTJETS") return customScaleFactorVec;

		double dbtLooseWP = DoubleBTagWPs::dbtNameToDouble("Loose");
		if ( fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + dbtLooseWP) ) return customScaleFactorVec;

		if (sampleType == "SIGNAL"){
			editCustomSfSignal(customScaleFactorVec, fatJetA_pt);
			editCustomSfSignal(customScaleFactorVec, fatJetB_pt);
		}

		if (sampleType == "TTJETS"){
			editCustomSfTTJets(customScaleFactorVec, fatJetA_pt);
			editCustomSfTTJets(customScaleFactorVec, fatJetB_pt);
		}

		return customScaleFactorVec;

	} // closes function 'GetCustomScaleFactors'


} // closes namespace 'DoubleBTagSF'
#endif