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
#include <TH1D.h>
#include <TF1.h>
#include <TEfficiency.h>

//RAL PARTICLE HEADERS
#include "../interface/PlotEntry.hh"


//--------constructor---------//
PlotEntry::PlotEntry(const std::string& plotEntryNameDummy, const TH1D& hTemplate, const std::string& variableToPlotDummy) :
	plotEntryName(plotEntryNameDummy),
	luminosity(0.0),
	variableToPlot(variableToPlotDummy),
	hNull(hTemplate),
	hEff(NULL),
	numberOfEventsBeforeCuts(0.0)
{
	Int_t nBins = hTemplate.GetNbinsX();
	if (hTemplate.GetXaxis()->GetXbins()->GetArray() == NULL) hTotal = new TH1D("hTotal", Form("%s;%s;%s", hTemplate.GetTitle(), hTemplate.GetXaxis()->GetTitle(), hTemplate.GetYaxis()->GetTitle()), nBins, hTemplate.GetBinLowEdge(1), hTemplate.GetBinLowEdge(nBins+1));
	else hTotal = new TH1D("hTotal", Form("%s;%s;%s", hTemplate.GetTitle(), hTemplate.GetXaxis()->GetTitle(), hTemplate.GetYaxis()->GetTitle()), nBins, hTemplate.GetXaxis()->GetXbins()->GetArray());
	std::vector<double> dummyStatError(nBins+2, 0.0);
	statErrorSquared = dummyStatError;
}

PlotEntry::PlotEntry(const std::string& plotEntryNameDummy, const TH1D& hTemplate, const std::string& variableToPlotDummy, const double& luminosityDummy) :
	plotEntryName(plotEntryNameDummy),
	luminosity(luminosityDummy),
	variableToPlot(variableToPlotDummy),
	hNull(hTemplate),
	hEff(NULL),
	numberOfEventsBeforeCuts(0.0)
{
	Int_t nBins = hTemplate.GetNbinsX();
	if (hTemplate.GetXaxis()->GetXbins()->GetArray() == NULL) hTotal = new TH1D("hTotal", Form("%s;%s;%s", hTemplate.GetTitle(), hTemplate.GetXaxis()->GetTitle(), hTemplate.GetYaxis()->GetTitle()), nBins, hTemplate.GetBinLowEdge(1), hTemplate.GetBinLowEdge(nBins+1));
	else hTotal = new TH1D("hTotal", Form("%s;%s;%s", hTemplate.GetTitle(), hTemplate.GetXaxis()->GetTitle(), hTemplate.GetYaxis()->GetTitle()), nBins, hTemplate.GetXaxis()->GetXbins()->GetArray());
	std::vector<double> dummyStatError(nBins+2, 0.0);
	statErrorSquared = dummyStatError;
}


//-----------public-----------//
void PlotEntry::AddInput(const std::string& flatTreeAddress, const std::string& selectionCut)
{
	TFile * f = TFile::Open(flatTreeAddress.c_str());
    TTree * evT = (TTree*)f->Get("eventCountTree");
    Int_t evTEntries = (Int_t)evT->GetEntries();
	UInt_t nEvtsRunOverForInputEntry;
	UInt_t nEvtsAfterPreSelForInputEntry;
	evT->SetBranchAddress("nEvtsRunOver",&nEvtsRunOverForInputEntry);        
	evT->SetBranchAddress("nEvtsPass",&nEvtsAfterPreSelForInputEntry);
    unsigned int numberOfEventsAfterPreSelection = 0;
    for (Int_t ievT=0; ievT<evTEntries; ++ievT){
        evT->GetEntry(ievT);
        numberOfEventsBeforeCuts += nEvtsRunOverForInputEntry;
        numberOfEventsAfterPreSelection += nEvtsAfterPreSelForInputEntry;
    }

	TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");
	if (T->GetEntries() != numberOfEventsAfterPreSelection){
		std::cout << "ERROR: number of events in doubleBFatJetPairTree of " << flatTreeAddress << " does not equal those in the nEvtsPass count. NOT FILLING." << std::endl;
		return;
	}
	TH1D hContainer = hNull; // make a copy of the empty histogram to fill with TTreeDraw
	hContainer.SetName("hContainer");
	std::string drawStringA = Form("%s>>hContainer", variableToPlot.c_str());
	std::string drawStringB = Form("%s", selectionCut.c_str());
    std::cout << "Filling for TTree: " << flatTreeAddress << std::endl;
	std::cout << "Variable used: " << variableToPlot << std::endl;
	if (!selectionCut.empty()) std::cout << "Cut applied: " << drawStringB << std::endl;
	else std::cout << "NB: no cut applied" << std::endl;
	std::cout << "NB: no event weighting (for data)" << std::endl;
	T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");
	for (int iBin = 0; iBin < hContainer.GetNbinsX()+2; ++iBin){
		hTotal->AddBinContent(iBin, hContainer.GetBinContent(iBin));
		statErrorSquared[iBin] += hContainer.GetBinContent(iBin);
	}
	std::cout << std::endl;
}

void PlotEntry::AddInput(const std::string& flatTreeAddress, const std::string& selectionCut, const double& crossSection)
{
	TFile * f = TFile::Open(flatTreeAddress.c_str());
    TTree * evT = (TTree*)f->Get("eventCountTree");
    Int_t evTEntries = (Int_t)evT->GetEntries();
	UInt_t nEvtsRunOverForInputEntry;
	UInt_t nEvtsAfterPreSelForInputEntry;
	evT->SetBranchAddress("nEvtsRunOver",&nEvtsRunOverForInputEntry);
	evT->SetBranchAddress("nEvtsPass",&nEvtsAfterPreSelForInputEntry);
    UInt_t nEvtsRunOverForInputTotal = 0;
    unsigned int numberOfEventsAfterPreSelection = 0;
    for (Int_t ievT=0; ievT<evTEntries; ++ievT){
        evT->GetEntry(ievT);
        nEvtsRunOverForInputTotal += nEvtsRunOverForInputEntry;
        numberOfEventsAfterPreSelection += nEvtsAfterPreSelForInputEntry;
    }
    numberOfEventsBeforeCuts += 1000.0 * crossSection * luminosity;
    double eventWeighting = 1000.0 * crossSection * luminosity / nEvtsRunOverForInputTotal;

	TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");
	if (T->GetEntries() != numberOfEventsAfterPreSelection){
		std::cout << "ERROR: number of events in doubleBFatJetPairTree of " << flatTreeAddress << " does not equal those in the nEvtsPass count. NOT FILLING." << std::endl;
		return;
	}
	TH1D hContainer = hNull; // make a copy of the empty histogram to fill with TTreeDraw
	hContainer.SetName("hContainer");
	std::string drawStringA = Form("%s>>hContainer", variableToPlot.c_str());
	std::string drawStringB;
	if (!selectionCut.empty()) drawStringB = Form("%.15f*(%s)", eventWeighting, selectionCut.c_str());
	else drawStringB = Form("%.15f", eventWeighting);
    std::cout << "Filling for TTree: " << flatTreeAddress << std::endl;
	std::cout << "Variable used: " << variableToPlot << std::endl;
	if (!selectionCut.empty()) std::cout << "Event Weighting * Cut applied: " << drawStringB << std::endl;
	else std::cout << "Event Weighting: " << drawStringB << "\nNB: no cut applied." << std::endl;
	T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");
	for (int iBin = 0; iBin < hContainer.GetNbinsX()+2; ++iBin){
		hTotal->AddBinContent(iBin, hContainer.GetBinContent(iBin));
		statErrorSquared[iBin] += hContainer.GetBinContent(iBin) * eventWeighting;
	}
	std::cout << std::endl;
}

void PlotEntry::AddInputEfficiency(const std::string& flatTreeAddress, const std::string& commonCut, const std::string& numeratorCut)
{
	TFile * f = TFile::Open(flatTreeAddress.c_str());
	TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");
	
	TH1D hDenominator = hNull; // make a copy of the empty histogram to fill with TTreeDraw
	hDenominator.SetName("hDenominator");
	std::string drawStringA = Form("%s>>hDenominator", variableToPlot.c_str());
	std::string drawStringB = Form("%s", commonCut.c_str());
	std::cout << "Filling Denominator from TTree: " << flatTreeAddress << std::endl;
	std::cout << "Variable used: " << variableToPlot << std::endl;
	if (!drawStringB.empty()) std::cout << "Cut applied: " << drawStringB << std::endl;
	else std::cout << "NB: no cut applied" << std::endl;
	std::cout << "NB: no event weighting (DOING A RATIO)" << std::endl;
	T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");	

	TH1D hNumerator = hNull; // make a copy of the empty histogram to fill with TTreeDraw
	hNumerator.SetName("hNumerator");
	drawStringA = Form("%s>>hNumerator", variableToPlot.c_str());
	if (commonCut.empty()) drawStringB = numeratorCut;
	else drawStringB = numeratorCut + " && " + drawStringB;
	std::cout << "Filling Numerator from TTree: " << flatTreeAddress << std::endl;
	std::cout << "Variable used: " << variableToPlot << std::endl;
	if (!drawStringB.empty()) std::cout << "Cut applied: " << drawStringB << std::endl;
	else std::cout << "NB: no cut applied" << std::endl;
	std::cout << "NB: no event weighting (DOING A RATIO)" << std::endl;
	T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");	

	TEfficiency * hEffDummy = new TEfficiency(hNumerator, hDenominator);
	hEff = (TEfficiency*)hEffDummy->Clone();
	for (int iBin = 0; iBin < hNull.GetNbinsX()+2; ++iBin){
		hTotal->AddBinContent(iBin, hEffDummy->GetEfficiency(iBin));
		// we don't set errors here as in general they are asymmetrical for effiencies
	}
	std::cout << std::endl;
}

std::string PlotEntry::GetPlotEntryName() const {return plotEntryName;}

TH1D* PlotEntry::GetHistogram() const {return hTotal;}

TEfficiency* PlotEntry::GetTEff() const {return hEff;}

double PlotEntry::GetNumberOfEventsBeforeCuts() const {return numberOfEventsBeforeCuts;}

double PlotEntry::GetNumberOfEventsAfterCuts() const
{
	double numberOfEventsAfterCuts = 0.0;
	for (int iBin = 0; iBin < hTotal->GetNbinsX()+2; ++iBin) numberOfEventsAfterCuts += hTotal->GetBinContent(iBin);
	return numberOfEventsAfterCuts;
}

// double PlotEntry::GetLuminosity() const {return luminosity;}

std::vector<double> PlotEntry::GetStatErrorSquaredVector() const {return statErrorSquared;}

void PlotEntry::NormalisePlot()
{
	double numberOfEventsAfterCuts = 0.0;
	for (int iBin = 0; iBin < hTotal->GetNbinsX()+2; ++iBin) numberOfEventsAfterCuts += hTotal->GetBinContent(iBin);
	hTotal->Scale(1/numberOfEventsAfterCuts);
	for (size_t iBin = 0; iBin < statErrorSquared.size(); ++iBin) statErrorSquared[iBin] = statErrorSquared[iBin] / (numberOfEventsAfterCuts * numberOfEventsAfterCuts);
}

void PlotEntry::FitFunction(const std::string& functionToFit, const double& minXFit, const double& maxXFit, const std::vector<double>& initialParams, const int& colour)
{
	TF1* f1 = new TF1("f1", functionToFit.c_str(), minXFit, maxXFit);
	f1->SetLineColor(colour);
	for (size_t i = 0; i < initialParams.size(); ++i) f1->SetParameter(i, initialParams[i]);
	hTotal->Fit("f1", "RI");
	hTotal->SetStats(0);
	std::cout << std::endl;
	std::cout << std::endl;
}

//-----------private----------//