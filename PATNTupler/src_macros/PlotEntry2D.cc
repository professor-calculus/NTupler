//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <fstream>
#include <sys/stat.h>

//ROOT HEADERS
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>

//RAL PARTICLE HEADERS
#include "../interface/PlotEntry2D.hh"


//--------constructor---------//
PlotEntry2D::PlotEntry2D(const std::string& plotEntryNameDummy, const TH2D& hTemplate, const std::string& variablesToPlotDummy) :
	plotEntryName(plotEntryNameDummy),
	luminosity(0.0),
	variablesToPlot(variablesToPlotDummy),
	hNull(hTemplate),
	numberOfEventsBeforeCuts(0.0),
	numberOfEventsAfterCuts_StatErrorSquared(0.0)
{
	Int_t nBinsX = hTemplate.GetNbinsX();
	Int_t nBinsY = hTemplate.GetNbinsY();
	hTotal = new TH2D("hTotal", Form("%s;%s;%s", hTemplate.GetTitle(), hTemplate.GetXaxis()->GetTitle(), hTemplate.GetYaxis()->GetTitle()), nBinsX, hTemplate.GetXaxis()->GetBinLowEdge(1), hTemplate.GetXaxis()->GetBinLowEdge(nBinsX+1), nBinsY, hTemplate.GetYaxis()->GetBinLowEdge(1), hTemplate.GetYaxis()->GetBinLowEdge(nBinsY+1));
}

PlotEntry2D::PlotEntry2D(const std::string& plotEntryNameDummy, const TH2D& hTemplate, const std::string& variablesToPlotDummy, const double& luminosityDummy) :
	plotEntryName(plotEntryNameDummy),
	luminosity(luminosityDummy),
	variablesToPlot(variablesToPlotDummy),
	hNull(hTemplate),
	numberOfEventsBeforeCuts(0.0),
	numberOfEventsAfterCuts_StatErrorSquared(0.0)
{
	Int_t nBinsX = hTemplate.GetNbinsX();
	Int_t nBinsY = hTemplate.GetNbinsY();
	hTotal = new TH2D("hTotal", Form("%s;%s;%s", hTemplate.GetTitle(), hTemplate.GetXaxis()->GetTitle(), hTemplate.GetYaxis()->GetTitle()), nBinsX, hTemplate.GetXaxis()->GetBinLowEdge(1), hTemplate.GetXaxis()->GetBinLowEdge(nBinsX+1), nBinsY, hTemplate.GetYaxis()->GetBinLowEdge(1), hTemplate.GetYaxis()->GetBinLowEdge(nBinsY+1));
}


//-----------public-----------//
void PlotEntry2D::AddInput(const std::string& flatTreeAddress, const std::string& selectionCut)
{
	TFile * f = TFile::Open(flatTreeAddress.c_str());
    TTree * evT = (TTree*)f->Get("eventCountTree");
    Int_t evTEntries = (Int_t)evT->GetEntries();
	UInt_t nEvtsRunOverForInputEntry;
	evT->SetBranchAddress("nEvtsRunOver",&nEvtsRunOverForInputEntry);        
    for (Int_t ievT=0; ievT<evTEntries; ++ievT){
        evT->GetEntry(ievT);
        numberOfEventsBeforeCuts += nEvtsRunOverForInputEntry;
    }

	TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");
	TH2D hContainer = hNull; // make a copy of the empty histogram to fill with TTreeDraw
	hContainer.SetName("hContainer");
	std::string drawStringA = Form("%s>>hContainer", variablesToPlot.c_str());
	std::string drawStringB = Form("%s", selectionCut.c_str());
    std::cout << "Filling for TTree: " << flatTreeAddress << std::endl;
	std::cout << "Variables used: " << variablesToPlot << std::endl;
	if (!selectionCut.empty()) std::cout << "Cut applied: " << drawStringB << std::endl;
	else std::cout << "NB: no cut applied" << std::endl;
	std::cout << "NB: no event weighting (for data)" << std::endl;
	T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");
	for (int iBin = 0; iBin < hContainer.GetNbinsX()+2; ++iBin){
		for (int jBin = 0; jBin < hContainer.GetNbinsY()+2; ++jBin){
			Int_t binLabel = hContainer.GetBin(iBin,jBin);
			hTotal->AddBinContent(binLabel, hContainer.GetBinContent(binLabel));
			numberOfEventsAfterCuts_StatErrorSquared += hContainer.GetBinContent(binLabel);
		}	
	}
	std::cout << std::endl;
}

void PlotEntry2D::AddInput(const std::string& flatTreeAddress, const std::string& selectionCut, const double& crossSection, const bool& addNumEventsBeforeCutsToTotal)
{
	TFile * f = TFile::Open(flatTreeAddress.c_str());
    TTree * evT = (TTree*)f->Get("eventCountTree");
    Int_t evTEntries = (Int_t)evT->GetEntries();
	UInt_t nEvtsRunOverForInputEntry;
	evT->SetBranchAddress("nEvtsRunOver",&nEvtsRunOverForInputEntry);        
    UInt_t nEvtsRunOverForInputTotal = 0;
    for (Int_t ievT=0; ievT<evTEntries; ++ievT){
        evT->GetEntry(ievT);
        nEvtsRunOverForInputTotal += nEvtsRunOverForInputEntry;
    }
    if (addNumEventsBeforeCutsToTotal) numberOfEventsBeforeCuts += 1000.0 * crossSection * luminosity;
    double eventWeighting = 1000.0 * crossSection * luminosity / nEvtsRunOverForInputTotal;

	TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");
	TH2D hContainer = hNull; // make a copy of the empty histogram to fill with TTreeDraw
	hContainer.SetName("hContainer");
	std::string drawStringA = Form("%s>>hContainer", variablesToPlot.c_str());
	std::string drawStringB;
	if (!selectionCut.empty()) drawStringB = Form("%.15f*(%s)", eventWeighting, selectionCut.c_str());
	else drawStringB = Form("%.15f", eventWeighting);
    std::cout << "Filling for TTree: " << flatTreeAddress << std::endl;
	std::cout << "Variables used: " << variablesToPlot << std::endl;
	if (!selectionCut.empty()) std::cout << "Event Weighting * Cut applied: " << drawStringB << std::endl;
	else std::cout << "Event Weighting: " << drawStringB << "\nNB: no cut applied." << std::endl;
	T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");
	for (int iBin = 0; iBin < hContainer.GetNbinsX()+2; ++iBin){
		for (int jBin = 0; jBin < hContainer.GetNbinsY()+2; ++jBin){
			Int_t binLabel = hContainer.GetBin(iBin,jBin);
			hTotal->AddBinContent(binLabel, hContainer.GetBinContent(binLabel));
			numberOfEventsAfterCuts_StatErrorSquared += hContainer.GetBinContent(binLabel) * eventWeighting;
		}	
	}
	std::cout << std::endl;
}

std::string PlotEntry2D::GetPlotEntryName() const {return plotEntryName;}

TH2D * PlotEntry2D::GetHistogram() const {return hTotal;}

double PlotEntry2D::GetNumberOfEventsBeforeCuts() const {return numberOfEventsBeforeCuts;}

double PlotEntry2D::GetNumberOfEventsAfterCuts() const
{
	double numberOfEventsAfterCuts = 0.0;
	for (int iBin = 0; iBin < hTotal->GetNbinsX()+2; ++iBin){
		for (int jBin = 0; jBin < hTotal->GetNbinsY()+2; ++jBin){
		
			numberOfEventsAfterCuts += hTotal->GetBinContent(iBin,jBin);
		}	
	}
	return numberOfEventsAfterCuts;
}

double PlotEntry2D::GetNumberOfEventsAfterCutsStatError() const
{
	return sqrt(numberOfEventsAfterCuts_StatErrorSquared);
}

void PlotEntry2D::NormalisePlot()
{
	double numberOfEventsAfterCuts = 0.0;
	for (int iBin = 0; iBin < hTotal->GetNbinsX()+2; ++iBin){
		for (int jBin = 0; jBin < hTotal->GetNbinsY()+2; ++jBin){
			numberOfEventsAfterCuts += hTotal->GetBinContent(iBin,jBin);
		}	
	}
	hTotal->Scale(1/numberOfEventsAfterCuts);
}

//-----------private----------//