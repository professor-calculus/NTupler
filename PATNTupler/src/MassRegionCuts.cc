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
MassRegionCuts::MassRegionCuts(const std::string& MassRegionCutNameDummy, const double& VHK_bandSizeDummy, const double& S1_NodeDummy, const double& SMAX_Node1Dummy, const double& SMAX_Node2Dummy, const std::vector<double>& SN_NodesDummy) :
MassRegionCutName(MassRegionCutNameDummy),
VHK_bandSize(VHK_bandSizeDummy),
S1_Node(S1_NodeDummy),
SMAX_Node1(SMAX_Node1Dummy),
SMAX_Node2(SMAX_Node2Dummy),
SN_Nodes(SN_NodesDummy)
{
	make_V_Cuts();
	make_H_Cuts();
	make_K_Cuts();
	make_OnSnUnD_cuts();
}


//-----------public-----------//
std::string MassRegionCuts::GetName() const {return MassRegionCutName;}
double MassRegionCuts::Get_VHK_bandSize() const {return VHK_bandSize;}
double MassRegionCuts::Get_S1_Node() const {return S1_Node;}
double MassRegionCuts::Get_SMAX_Node1() const {return SMAX_Node1;}
double MassRegionCuts::Get_SMAX_Node2() const {return SMAX_Node2;}
std::vector<double> MassRegionCuts::Get_SN_Nodes() const {return SN_Nodes;}
std::string MassRegionCuts::Get_V_Cuts() const {return V_Cuts;}
std::string MassRegionCuts::Get_H_Cuts() const {return H_Cuts;}
std::string MassRegionCuts::Get_K_Cuts() const {return K_Cuts;}
std::string MassRegionCuts::Get_O_Cuts() const {return O_Cuts;}
std::vector<std::string> MassRegionCuts::Get_S_Cuts() const {return S_Cuts;}
std::vector<std::string> MassRegionCuts::Get_U_Cuts() const {return U_Cuts;}
std::vector<std::string> MassRegionCuts::Get_D_Cuts() const {return D_Cuts;}
std::vector<std::string> MassRegionCuts::GetAllCuts() const
{
	std::vector<std::string> allCuts;
	// allCuts.push_back(V_Cuts);
	// allCuts.push_back(H_Cuts);
	// allCuts.push_back(K_Cuts);
	// allCuts.push_back(O_Cuts);
	for (size_t i=0; i!=S_Cuts.size(); ++i) allCuts.push_back(S_Cuts[i]);
	for (size_t i=0; i!=U_Cuts.size(); ++i) allCuts.push_back(U_Cuts[i]);
	for (size_t i=0; i!=D_Cuts.size(); ++i) allCuts.push_back(D_Cuts[i]);
	return allCuts;
}

double MassRegionCuts::Get_S1ScaleFactor() const
{
	if (SN_Nodes.empty()){
		std::cout << "Error: Cannot provide valid S1ScaleFactor as SN_Nodes is empty" << std::endl;
		return -999.99;
	}

	double gradientUpperSignalLine = (SMAX_Node1 - S1_Node) / (SMAX_Node2 - VHK_bandSize);
	double gradientLowerSignalLine = 1 / gradientUpperSignalLine;
	double correspondingYValue = yValue(SN_Nodes[0], gradientLowerSignalLine, S1_Node, VHK_bandSize);
	double lengthS1DiagnolTop = sqrt(2) * (SN_Nodes[0] - correspondingYValue);

	double S1TopCentre = 0.5 * (SN_Nodes[0] - correspondingYValue) + correspondingYValue;
	double S1BottomCentre = 0.5 * (S1_Node - VHK_bandSize) + VHK_bandSize;
	double depth = sqrt(2) * (S1TopCentre - S1BottomCentre);

	double area_triangle = 0.5 * (S1_Node - VHK_bandSize) * (S1_Node - VHK_bandSize);
	double area_regular = 0.5 * ( sqrt(2) * (S1_Node - VHK_bandSize) + lengthS1DiagnolTop ) * depth;

	return (area_triangle + area_regular) / area_regular;
}


//-----------private----------//
double MassRegionCuts::yValue(const double& xValue, const double& gradient, const double& xCoord, const double& yCoord) const
{
	return gradient * (xValue - xCoord) + yCoord;
}

void MassRegionCuts::make_V_Cuts()
{
	V_Cuts = Form("fatJetA_softDropMass<%f && fatJetB_softDropMass>=%f", VHK_bandSize, VHK_bandSize);
	return;
}

void MassRegionCuts::make_H_Cuts()
{
	H_Cuts = Form("fatJetA_softDropMass>=%f && fatJetB_softDropMass<%f", VHK_bandSize, VHK_bandSize);
	return;
}

void MassRegionCuts::make_K_Cuts()
{
	K_Cuts = Form("fatJetA_softDropMass<%f && fatJetB_softDropMass<%f", VHK_bandSize, VHK_bandSize);
	return;
}

void MassRegionCuts::make_OnSnUnD_cuts()
{
	// S cuts
	double gradientUpperSignalLine = (SMAX_Node1 - S1_Node) / (SMAX_Node2 - VHK_bandSize);
	double gradientLowerSignalLine = 1 / gradientUpperSignalLine;
	std::string upperSignalLine = Form("(%f * (fatJetA_softDropMass - %f) + %f)", gradientUpperSignalLine, VHK_bandSize, S1_Node);
	std::string lowerSignalLine = Form("(%f * (fatJetA_softDropMass - %f) + %f)", gradientLowerSignalLine, S1_Node, VHK_bandSize);

	std::string baseLineCutString = "fatJetB_softDropMass < " + upperSignalLine;
	baseLineCutString += " && fatJetB_softDropMass >= " + lowerSignalLine;

	std::vector<std::string> additionalSignalRegionCuts(SN_Nodes.size()+1, baseLineCutString);
	additionalSignalRegionCuts[0] += Form(" && fatJetA_softDropMass>=%f && fatJetB_softDropMass>=%f", VHK_bandSize, VHK_bandSize);
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node, VHK_bandSize);
		std::string diagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", SN_Nodes[i], correspondingYValue);
		additionalSignalRegionCuts[i] += Form(" && fatJetB_softDropMass < %s", diagnolSegmentLine.c_str());
		additionalSignalRegionCuts[i+1] += Form(" && fatJetB_softDropMass >= %s", diagnolSegmentLine.c_str());
	}
	std::string finalDiagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", SMAX_Node1, SMAX_Node2);
	additionalSignalRegionCuts[SN_Nodes.size()] += Form(" && fatJetB_softDropMass < %s", finalDiagnolSegmentLine.c_str());
	S_Cuts = additionalSignalRegionCuts;

	// O cuts
	O_Cuts = "((fatJetB_softDropMass >= " + upperSignalLine + Form(" && fatJetA_softDropMass>=%f)", VHK_bandSize);
	O_Cuts += " || (fatJetB_softDropMass < " + lowerSignalLine + Form(" && fatJetB_softDropMass>=%f)", VHK_bandSize);
	O_Cuts += Form(" || (fatJetB_softDropMass > %s && fatJetA_softDropMass>=%f && fatJetB_softDropMass>=%f))", finalDiagnolSegmentLine.c_str(), VHK_bandSize, VHK_bandSize);

	// U cuts
	double upperBand_x1 = VHK_bandSize - 0.5 * (S1_Node - VHK_bandSize);
	double upperBand_y1 = S1_Node + 0.5 * (S1_Node - VHK_bandSize);
	double upperBand_x2 = SMAX_Node2 - 0.5 * (SMAX_Node1 - SMAX_Node2);
	double upperBand_y2 = SMAX_Node1 + 0.5 * (SMAX_Node1 - SMAX_Node2);
	double gradientUpperBand = (upperBand_y2 - upperBand_y1) / (upperBand_x2 - upperBand_x1);
	std::string upperBandLine = Form("(%f * (fatJetA_softDropMass - %f) + %f)", gradientUpperBand, upperBand_x1, upperBand_y1);
	
	std::string upperBandBaseLineCuts = "fatJetB_softDropMass < " + upperBandLine;
	upperBandBaseLineCuts += " && fatJetB_softDropMass >= " + upperSignalLine;

	std::vector<std::string> additionalUpperBandCuts(SN_Nodes.size()+1, upperBandBaseLineCuts);
	additionalUpperBandCuts[0] += Form(" && fatJetB_softDropMass>= (-1 * (fatJetA_softDropMass - %f) + %f)", upperBand_x1, upperBand_y1);
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node, VHK_bandSize);
		std::string diagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", SN_Nodes[i], correspondingYValue);
		additionalUpperBandCuts[i] += Form(" && fatJetB_softDropMass < %s", diagnolSegmentLine.c_str());
		additionalUpperBandCuts[i+1] += Form(" && fatJetB_softDropMass >= %s", diagnolSegmentLine.c_str());
	}
	additionalUpperBandCuts[SN_Nodes.size()] += Form(" && fatJetB_softDropMass < %s", finalDiagnolSegmentLine.c_str());
	U_Cuts = additionalUpperBandCuts;

	// D cuts
	std::string lowerBandLine = Form("(%f * (fatJetA_softDropMass - %f) + %f)", 1/gradientUpperBand, upperBand_y1, upperBand_x1);

	std::string lowerBandBaseLineCuts = "fatJetB_softDropMass >= " + lowerBandLine;
	lowerBandBaseLineCuts +=  " && fatJetB_softDropMass < " + lowerSignalLine;

	std::vector<std::string> additionalLowerBandCuts(SN_Nodes.size()+1, lowerBandBaseLineCuts);
	additionalLowerBandCuts[0] += Form(" && fatJetB_softDropMass>= (-1 * (fatJetA_softDropMass - %f) + %f)", upperBand_x1, upperBand_y1);
	for (size_t i=0; i!=SN_Nodes.size(); ++i){

		double correspondingYValue = yValue(SN_Nodes[i], gradientLowerSignalLine, S1_Node, VHK_bandSize);
		std::string diagnolSegmentLine = Form("(-1 * (fatJetA_softDropMass - %f) + %f)", SN_Nodes[i], correspondingYValue);
		additionalLowerBandCuts[i] += Form(" && fatJetB_softDropMass < %s", diagnolSegmentLine.c_str());
		additionalLowerBandCuts[i+1] += Form(" && fatJetB_softDropMass >= %s", diagnolSegmentLine.c_str());
	}
	additionalLowerBandCuts[SN_Nodes.size()] += Form(" && fatJetB_softDropMass < %s", finalDiagnolSegmentLine.c_str());
	D_Cuts = additionalLowerBandCuts;

	return;
}