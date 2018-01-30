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
MassRegionCuts::MassRegionCuts(const std::string& MassRegionCutNameDummy, const double& S1_Node1Dummy, const double& S1_Node2Dummy, const double& SMAX_Node1Dummy, const double& SMAX_Node2Dummy, const std::vector<double>& SN_NodesDummy) :
MassRegionCutName(MassRegionCutNameDummy),
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
	std::string upperSignalLine = Form("(%f * (fatJetA_softDropMass - %f) + %f)", gradientUpperSignalLine, S1_Node2, S1_Node1);
	std::string lowerSignalLine = Form("(%f * (fatJetA_softDropMass - %f) + %f)", gradientLowerSignalLine, S1_Node1, S1_Node2);

	std::string baseLineCutString = "fatJetB_softDropMass < " + upperSignalLine;
	baseLineCutString += " && fatJetB_softDropMass >= " + lowerSignalLine;

	std::vector<std::string> additionalSignalRegionCuts(SN_Nodes.size()+1, baseLineCutString);
	std::string initialDiagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", S1_Node1, S1_Node2);
	additionalSignalRegionCuts[0] += Form(" && fatJetB_softDropMass >= %s", initialDiagnolSegmentLine.c_str());
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node1, S1_Node2);
		std::string diagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", SN_Nodes[i], correspondingYValue);
		additionalSignalRegionCuts[i] += Form(" && fatJetB_softDropMass < %s", diagnolSegmentLine.c_str());
		additionalSignalRegionCuts[i+1] += Form(" && fatJetB_softDropMass >= %s", diagnolSegmentLine.c_str());
	}
	std::string finalDiagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", SMAX_Node1, SMAX_Node2);
	additionalSignalRegionCuts[SN_Nodes.size()] += Form(" && fatJetB_softDropMass < %s", finalDiagnolSegmentLine.c_str());
	S_Cuts = additionalSignalRegionCuts;

	// U cuts
	double upperBand_x1 = S1_Node2 - 0.5 * (S1_Node1 - S1_Node2);
	double upperBand_y1 = S1_Node1 + 0.5 * (S1_Node1 - S1_Node2);
	double upperBand_x2 = SMAX_Node2 - 0.5 * (SMAX_Node1 - SMAX_Node2);
	double upperBand_y2 = SMAX_Node1 + 0.5 * (SMAX_Node1 - SMAX_Node2);
	double gradientUpperBand = (upperBand_y2 - upperBand_y1) / (upperBand_x2 - upperBand_x1);
	std::string upperBandLine = Form("(%f * (fatJetA_softDropMass - %f) + %f)", gradientUpperBand, upperBand_x1, upperBand_y1);
	
	std::string upperBandBaseLineCuts = "fatJetB_softDropMass < " + upperBandLine;
	upperBandBaseLineCuts += " && fatJetB_softDropMass >= " + upperSignalLine;

	// work out the coords for upper corner of upper segement 1
	double yValue_S1UpperLeft = SN_Nodes[0];
	double xValue_S1UpperLeft = yValue(SN_Nodes[0], gradientLowerSignalLine, S1_Node1, S1_Node2);
	double upperBand_x1U = xValue_S1UpperLeft - 0.5 * (yValue_S1UpperLeft - xValue_S1UpperLeft);
	double upperBand_y1U = yValue_S1UpperLeft + 0.5 * (yValue_S1UpperLeft - xValue_S1UpperLeft);
	double gradientUpperSegment1LowerBound = (S1_Node1 - upperBand_y1U) / (S1_Node2 - upperBand_x1U);

	std::vector<std::string> additionalUpperBandCuts(SN_Nodes.size()+1, upperBandBaseLineCuts);
	if (gradientUpperSegment1LowerBound > 0) additionalUpperBandCuts[0] += Form(" && fatJetB_softDropMass< (%f * (fatJetA_softDropMass - %f) + %f)", gradientUpperSegment1LowerBound, S1_Node2, S1_Node1);
	else if (gradientUpperSegment1LowerBound < 0) additionalUpperBandCuts[0] += Form(" && fatJetB_softDropMass>= (%f * (fatJetA_softDropMass - %f) + %f)", gradientUpperSegment1LowerBound, S1_Node2, S1_Node1);
	else{
		std::cout << "WARNING: gradient of U1 lower bounding segment is undefined, setting at 999999.0" << std::endl;
		gradientUpperSignalLine = 999999.0;
		additionalUpperBandCuts[0] += Form(" && fatJetB_softDropMass< (%f * (fatJetA_softDropMass - %f) + %f)", gradientUpperSegment1LowerBound, S1_Node2, S1_Node1);
	}
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node1, S1_Node2);
		std::string diagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", SN_Nodes[i], correspondingYValue);
		additionalUpperBandCuts[i] += Form(" && fatJetB_softDropMass < %s", diagnolSegmentLine.c_str());
		additionalUpperBandCuts[i+1] += Form(" && fatJetB_softDropMass >= %s", diagnolSegmentLine.c_str());
	}
	additionalUpperBandCuts[SN_Nodes.size()] += Form(" && fatJetB_softDropMass < %s", finalDiagnolSegmentLine.c_str());
	U_Cuts = additionalUpperBandCuts;

	// D cuts
	std::string lowerBandLine = Form("(%f * (fatJetA_softDropMass - %f) + %f)", 1/gradientUpperBand, upperBand_y1, upperBand_x1);

	std::string lowerBandBaseLineCuts = "fatJetB_softDropMass >= " + lowerBandLine;
	if (gradientUpperBand < 0) lowerBandBaseLineCuts = "fatJetB_softDropMass <= " + lowerBandLine;
	lowerBandBaseLineCuts +=  " && fatJetB_softDropMass < " + lowerSignalLine;

	double gradientDownerSegment1LowerBound = 1 / gradientUpperSegment1LowerBound;

	std::vector<std::string> additionalLowerBandCuts(SN_Nodes.size()+1, lowerBandBaseLineCuts);
	additionalLowerBandCuts[0] += Form(" && fatJetB_softDropMass>= (%f * (fatJetA_softDropMass - %f) + %f)", gradientDownerSegment1LowerBound, S1_Node1, S1_Node2);
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node1, S1_Node2);
		std::string diagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", SN_Nodes[i], correspondingYValue);
		additionalLowerBandCuts[i] += Form(" && fatJetB_softDropMass < %s", diagnolSegmentLine.c_str());
		additionalLowerBandCuts[i+1] += Form(" && fatJetB_softDropMass >= %s", diagnolSegmentLine.c_str());
	}
	additionalLowerBandCuts[SN_Nodes.size()] += Form(" && fatJetB_softDropMass < %s", finalDiagnolSegmentLine.c_str());
	D_Cuts = additionalLowerBandCuts;

	return;
}