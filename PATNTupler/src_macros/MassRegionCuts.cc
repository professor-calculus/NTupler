//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <fstream>
#include <sys/stat.h>
#include <map>

//ROOT HEADERS
#include <TString.h>

//RAL PARTICLE HEADERS
#include "../interface/MassRegionCuts.hh"


//--------constructor---------//
MassRegionCuts::MassRegionCuts(const std::string& MassRegionCutNameDummy, const double& S1_Node1Dummy, const double& S1_Node2Dummy, const double& SMAX_Node1Dummy, const double& SMAX_Node2Dummy, const std::vector<double>& SN_NodesDummy, const std::string& fatJetA_massDummy, const std::string& fatJetB_massDummy, const double& sideBandScaleFactorDummy) :
MassRegionCutName(MassRegionCutNameDummy),
sideBandScaleFactor(sideBandScaleFactorDummy),
fatJetA_mass(fatJetA_massDummy),
fatJetB_mass(fatJetB_massDummy),
S1_Node1(S1_Node1Dummy),
S1_Node2(S1_Node2Dummy),
SMAX_Node1(SMAX_Node1Dummy),
SMAX_Node2(SMAX_Node2Dummy),
SN_Nodes(SN_NodesDummy)
{
	make_cuts();
}


//-----------public-----------//
std::string MassRegionCuts::GetName() const {return MassRegionCutName;}
double MassRegionCuts::Get_S1_Node2() const {return S1_Node2;}
double MassRegionCuts::Get_S1_Node1() const {return S1_Node1;}
double MassRegionCuts::Get_SMAX_Node1() const {return SMAX_Node1;}
double MassRegionCuts::Get_SMAX_Node2() const {return SMAX_Node2;}
double MassRegionCuts::Get_sideBandScaleFactor() const {return sideBandScaleFactor;}
std::vector<double> MassRegionCuts::Get_SN_Nodes() const {return SN_Nodes;}
std::vector<std::string> MassRegionCuts::Get_S_Cuts() const {return S_Cuts;}
std::vector<std::string> MassRegionCuts::Get_U_Cuts() const {return U_Cuts;}
std::vector<std::string> MassRegionCuts::Get_D_Cuts() const {return D_Cuts;}
std::vector<std::string> MassRegionCuts::GetAllCuts() const
{
	std::vector<std::string> allCuts;
	for (size_t i=0; i!=S_Cuts.size(); ++i) allCuts.push_back(S_Cuts[i]);
	for (size_t i=0; i!=U_Cuts.size(); ++i) allCuts.push_back(U_Cuts[i]);
	for (size_t i=0; i!=D_Cuts.size(); ++i) allCuts.push_back(D_Cuts[i]);
	return allCuts;
}


//-----------private----------//
double MassRegionCuts::yValue(const double& xValue, const double& gradient, const double& xCoord, const double& yCoord) const
{
	return gradient * (xValue - xCoord) + yCoord;
}


void MassRegionCuts::make_cuts()
{
	if (SN_Nodes.empty()){
		std::cout << "cannot make mass_cuts with an empty SN_Nodes object. Soz..." << std::endl;
		return;
	}

	// S cuts
	double gradientUpperSignalLine = (SMAX_Node1 - S1_Node1) / (SMAX_Node2 - S1_Node2);
	double gradientLowerSignalLine = 1 / gradientUpperSignalLine;
	std::string upperSignalLine = Form("(%f * (%s - %f) + %f)", gradientUpperSignalLine, fatJetA_mass.c_str(), S1_Node2, S1_Node1);
	std::string lowerSignalLine = Form("(%f * (%s - %f) + %f)", gradientLowerSignalLine, fatJetA_mass.c_str(), S1_Node1, S1_Node2);

	std::string baseLineCutString = fatJetB_mass + " < " + upperSignalLine;
	baseLineCutString += " && " + fatJetB_mass + " >= " + lowerSignalLine;

	std::vector<std::string> additionalSignalRegionCuts(SN_Nodes.size()+1, baseLineCutString);
	std::string initialDiagnolSegmentLine = Form("(-1 * (%s - %f) + %f)", fatJetA_mass.c_str(), S1_Node1, S1_Node2);
	additionalSignalRegionCuts[0] += Form(" && %s >= %s", fatJetB_mass.c_str(), initialDiagnolSegmentLine.c_str());
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node1, S1_Node2);
		std::string diagnolSegmentLine = Form("(-1 * (%s - %f) + %f)", fatJetA_mass.c_str(), SN_Nodes[i], correspondingYValue);
		additionalSignalRegionCuts[i] += Form(" && %s < %s", fatJetB_mass.c_str(), diagnolSegmentLine.c_str());
		additionalSignalRegionCuts[i+1] += Form(" && %s >= %s", fatJetB_mass.c_str(), diagnolSegmentLine.c_str());
	}
	std::string finalDiagnolSegmentLine = Form("(-1 * (%s - %f) + %f)", fatJetA_mass.c_str(), SMAX_Node1, SMAX_Node2);
	additionalSignalRegionCuts[SN_Nodes.size()] += Form(" && %s < %s", fatJetB_mass.c_str(), finalDiagnolSegmentLine.c_str());
	S_Cuts = additionalSignalRegionCuts;

	// U cuts
	double upperBand_x1 = S1_Node2 - sideBandScaleFactor * (S1_Node1 - S1_Node2);
	double upperBand_y1 = S1_Node1 + sideBandScaleFactor * (S1_Node1 - S1_Node2);
	double upperBand_x2 = SMAX_Node2 - sideBandScaleFactor * (SMAX_Node1 - SMAX_Node2);
	double upperBand_y2 = SMAX_Node1 + sideBandScaleFactor * (SMAX_Node1 - SMAX_Node2);
	double gradientUpperBand = (upperBand_y2 - upperBand_y1) / (upperBand_x2 - upperBand_x1);
	std::string upperBandLine = Form("(%f * (%s - %f) + %f)", gradientUpperBand, fatJetA_mass.c_str(), upperBand_x1, upperBand_y1);
	
	std::string upperBandBaseLineCuts = fatJetB_mass + " < " + upperBandLine;
	upperBandBaseLineCuts += " && " + fatJetB_mass + " >= " + upperSignalLine;

	// work out the coords for upper corner of upper segement 1
	double yValue_S1UpperLeft = SN_Nodes[0];
	double xValue_S1UpperLeft = yValue(SN_Nodes[0], gradientLowerSignalLine, S1_Node1, S1_Node2);
	double upperBand_x1U = xValue_S1UpperLeft - sideBandScaleFactor * (yValue_S1UpperLeft - xValue_S1UpperLeft);
	double upperBand_y1U = yValue_S1UpperLeft + sideBandScaleFactor * (yValue_S1UpperLeft - xValue_S1UpperLeft);
	double gradientUpperSegment1LowerBound = (S1_Node1 - upperBand_y1U) / (S1_Node2 - upperBand_x1U);

	std::vector<std::string> additionalUpperBandCuts(SN_Nodes.size()+1, upperBandBaseLineCuts);
	if (gradientUpperSegment1LowerBound > 0) additionalUpperBandCuts[0] += Form(" && %s< (%f * (%s - %f) + %f)", fatJetB_mass.c_str(), gradientUpperSegment1LowerBound, fatJetA_mass.c_str(), S1_Node2, S1_Node1);
	else if (gradientUpperSegment1LowerBound < 0) additionalUpperBandCuts[0] += Form(" && %s>= (%f * (%s - %f) + %f)", fatJetB_mass.c_str(), gradientUpperSegment1LowerBound, fatJetA_mass.c_str(), S1_Node2, S1_Node1);
	else{
		std::cout << "WARNING: gradient of U1 lower bounding segment is undefined, setting at 999999.0" << std::endl;
		gradientUpperSignalLine = 999999.0;
		additionalUpperBandCuts[0] += Form(" && %s< (%f * (%s - %f) + %f)", fatJetB_mass.c_str(), gradientUpperSegment1LowerBound, fatJetA_mass.c_str(), S1_Node2, S1_Node1);
	}
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node1, S1_Node2);
		std::string diagnolSegmentLine = Form("(-1 * (%s - %f) + %f)", fatJetA_mass.c_str(), SN_Nodes[i], correspondingYValue);
		additionalUpperBandCuts[i] += Form(" && %s < %s", fatJetB_mass.c_str(), diagnolSegmentLine.c_str());
		additionalUpperBandCuts[i+1] += Form(" && %s >= %s", fatJetB_mass.c_str(), diagnolSegmentLine.c_str());
	}
	additionalUpperBandCuts[SN_Nodes.size()] += Form(" && %s < %s", fatJetB_mass.c_str(), finalDiagnolSegmentLine.c_str());
	U_Cuts = additionalUpperBandCuts;

	// D cuts
	std::string lowerBandLine = Form("(%f * (%s - %f) + %f)", 1/gradientUpperBand, fatJetA_mass.c_str(), upperBand_y1, upperBand_x1);

	std::string lowerBandBaseLineCuts = fatJetB_mass + " >= " + lowerBandLine;
	if (gradientUpperBand < 0) lowerBandBaseLineCuts = fatJetB_mass + " <= " + lowerBandLine;
	lowerBandBaseLineCuts +=  " && " + fatJetB_mass + " < " + lowerSignalLine;

	double gradientDownerSegment1LowerBound = 1 / gradientUpperSegment1LowerBound;

	std::vector<std::string> additionalLowerBandCuts(SN_Nodes.size()+1, lowerBandBaseLineCuts);
	additionalLowerBandCuts[0] += Form(" && %s>= (%f * (%s - %f) + %f)", fatJetB_mass.c_str(), gradientDownerSegment1LowerBound, fatJetA_mass.c_str(), S1_Node1, S1_Node2);
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node1, S1_Node2);
		std::string diagnolSegmentLine = Form("(-1 * (%s - %f) + %f)", fatJetA_mass.c_str(), SN_Nodes[i], correspondingYValue);
		additionalLowerBandCuts[i] += Form(" && %s < %s", fatJetB_mass.c_str(), diagnolSegmentLine.c_str());
		additionalLowerBandCuts[i+1] += Form(" && %s >= %s", fatJetB_mass.c_str(), diagnolSegmentLine.c_str());
	}
	additionalLowerBandCuts[SN_Nodes.size()] += Form(" && %s < %s", fatJetB_mass.c_str(), finalDiagnolSegmentLine.c_str());
	D_Cuts = additionalLowerBandCuts;

	return;
}