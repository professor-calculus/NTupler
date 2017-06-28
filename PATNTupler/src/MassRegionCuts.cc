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
	make_OnS_cuts();
}


//-----------public-----------//
std::string MassRegionCuts::GetName() const {return MassRegionCutName;}
std::string MassRegionCuts::Get_V_Cuts() const {return V_Cuts;}
std::string MassRegionCuts::Get_H_Cuts() const {return H_Cuts;}
std::string MassRegionCuts::Get_K_Cuts() const {return K_Cuts;}
std::string MassRegionCuts::Get_O_Cuts() const {return O_Cuts;}
std::vector<std::string> MassRegionCuts::Get_S_Cuts() const {return S_Cuts;}
std::vector<std::string> MassRegionCuts::GetAllCuts() const
{
	std::vector<std::string> allCuts;
	allCuts.push_back(V_Cuts);
	allCuts.push_back(H_Cuts);
	allCuts.push_back(K_Cuts);
	allCuts.push_back(O_Cuts);
	for (size_t i=0; i!=S_Cuts.size(); ++i) allCuts.push_back(S_Cuts[i]);
	return allCuts;
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

void MassRegionCuts::make_OnS_cuts()
{
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

	O_Cuts = "((fatJetB_softDropMass >= " + upperSignalLine + Form(" && fatJetA_softDropMass>=%f)", VHK_bandSize);
	O_Cuts += " || (fatJetB_softDropMass < " + lowerSignalLine + Form(" && fatJetB_softDropMass>=%f)", VHK_bandSize);
	O_Cuts += Form(" || (fatJetB_softDropMass > %s))", finalDiagnolSegmentLine.c_str());

	return;
}