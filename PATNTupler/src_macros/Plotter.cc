//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <fstream>
#include <sys/stat.h>
#include <map>

//ROOT HEADERS
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TLine.h>
#include <TCanvas.h>
#include <THStack.h>

//RAL PARTICLE HEADERS
#include "../interface/PlotEntry.hh"
#include "../interface/PlotEntry2D.hh"
#include "../interface/Plotter.hh"
#include "../interface/MassRegionCuts.hh"


//--------constructor---------//
Plotter::Plotter(std::vector<PlotEntry> histoIndiDummy) :
leg(0),
addLatex(false),
useLogY(false),
useLogZ(false),
plotWithErrorsIndi(false),
plotWithErrorsStack(false),
tdrStyle(TDRStyle())
{
	if (histoIndiDummy.empty()) std::cout << "Plotter WARNING: no plot entries handed to plotter!" << std::endl;
	else histoIndi = histoIndiDummy;

	std::vector<PlotEntry> histoStackEmpty;
	histoStack = histoStackEmpty;

	for (size_t iIndi = 0; iIndi != histoIndi.size(); ++iIndi){
		histoIndi[iIndi].GetHistogram()->SetLineColor(SetColor_stark(iIndi));
		histoIndi[iIndi].GetHistogram()->SetLineWidth(2);
		histoIndi[iIndi].GetHistogram()->GetXaxis()->SetTitleSize(0.05); // can't get this to work via tstyle
		histoIndi[iIndi].GetHistogram()->GetXaxis()->SetLabelSize(0.04);
		histoIndi[iIndi].GetHistogram()->GetYaxis()->SetTitleSize(0.05);
		histoIndi[iIndi].GetHistogram()->GetYaxis()->SetLabelSize(0.04);
	}
}

Plotter::Plotter(std::vector<PlotEntry> histoIndiDummy, std::vector<PlotEntry> histoStackDummy) :
leg(0),
addLatex(false),
useLogY(false),
useLogZ(false),
plotWithErrorsIndi(false),
plotWithErrorsStack(false),
tdrStyle(TDRStyle())
{
	if (histoIndiDummy.empty()) std::cout << "Plotter Message: first constructor argument std::vector<PlotEntry> is empty" << std::endl;
	else histoIndi = histoIndiDummy;
	
	if (histoStackDummy.empty()) std::cout << "Plotter Message: second constructor argument std::vector<PlotEntry> is empty" << std::endl;
	else histoStack = histoStackDummy;

	if (histoStackDummy.empty() && histoIndiDummy.empty()) std::cout << "Plotter WARNING: no plot entries handed to plotter!" << std::endl;
	
	for (size_t iIndi = 0; iIndi != histoIndi.size(); ++iIndi){
		histoIndi[iIndi].GetHistogram()->SetLineColor(SetColor_stark(iIndi));
		histoIndi[iIndi].GetHistogram()->SetLineWidth(2);
		histoIndi[iIndi].GetHistogram()->GetXaxis()->SetTitleSize(0.05); // can't get this to work via tstyle
		histoIndi[iIndi].GetHistogram()->GetXaxis()->SetLabelSize(0.04);
		histoIndi[iIndi].GetHistogram()->GetYaxis()->SetTitleSize(0.05);
		histoIndi[iIndi].GetHistogram()->GetYaxis()->SetLabelSize(0.04);
	}

	for (size_t iStack = 0; iStack != histoStack.size(); ++iStack){
		histoStack[iStack].GetHistogram()->SetFillColor(SetColor_mellow(iStack+2, histoStack.size()));	
		histoStack[iStack].GetHistogram()->SetLineWidth(0.0);
		histoStack[iStack].GetHistogram()->GetXaxis()->SetTitleSize(0.05); // can't get this to work via tstyle
		histoStack[iStack].GetHistogram()->GetXaxis()->SetLabelSize(0.04);
		histoStack[iStack].GetHistogram()->GetYaxis()->SetTitleSize(0.05);
		histoStack[iStack].GetHistogram()->GetYaxis()->SetLabelSize(0.04);
	}
}

Plotter::Plotter(std::vector<PlotEntry2D> dummyHistos2D) :
leg(0),
addLatex(false),
useLogY(false),
useLogZ(false),
plotWithErrorsIndi(false),
plotWithErrorsStack(false),
tdrStyle(TDRStyle())
{
	if (dummyHistos2D.empty()) std::cout << "Plotter Message: constructor argument std::vector<PlotEntry2D> is empty" << std::endl;
	else histos2D = dummyHistos2D;

	for (size_t iHistos2D = 0; iHistos2D != histos2D.size(); ++iHistos2D){
		histos2D[iHistos2D].GetHistogram()->GetXaxis()->SetTitleSize(0.05); // can't get this to work via tstyle
		histos2D[iHistos2D].GetHistogram()->GetXaxis()->SetLabelSize(0.04);
		histos2D[iHistos2D].GetHistogram()->GetYaxis()->SetTitleSize(0.05);
		histos2D[iHistos2D].GetHistogram()->GetYaxis()->SetLabelSize(0.04);
	}
}

Plotter::Plotter(std::vector<TH1D*> th1IndiDummy) :
leg(0),
addLatex(false),
useLogY(false),
useLogZ(false),
plotWithErrorsIndi(false),
plotWithErrorsStack(false),
tdrStyle(TDRStyle())
{
	if (th1IndiDummy.empty()) std::cout << "Plotter Message: no histograms handed to plotter!" << std::endl;
	else th1Indi = th1IndiDummy;

	for (size_t iTh1I = 0; iTh1I != th1Indi.size(); ++iTh1I){
		th1Indi[iTh1I]->SetLineColor(SetColor_stark(iTh1I));
		th1Indi[iTh1I]->SetLineWidth(2);
		th1Indi[iTh1I]->GetXaxis()->SetTitleSize(0.05); // can't get this to work via tstyle
		th1Indi[iTh1I]->GetXaxis()->SetLabelSize(0.04);
		th1Indi[iTh1I]->GetYaxis()->SetTitleSize(0.05);
		th1Indi[iTh1I]->GetYaxis()->SetLabelSize(0.04);
	}
}

Plotter::Plotter(std::vector<TH1D*> th1IndiDummy, std::vector<TH1D*> th1StackDummy) :
leg(0),
addLatex(false),
useLogY(false),
useLogZ(false),
plotWithErrorsIndi(false),
plotWithErrorsStack(false),
tdrStyle(TDRStyle())
{
	if (th1IndiDummy.empty()) std::cout << "Plotter Message: first constructor argument std::vector<TH1D*> is empty" << std::endl;
	else th1Indi = th1IndiDummy;
	if (th1StackDummy.empty()) std::cout << "Plotter Message: first constructor argument std::vector<TH1D*> is empty" << std::endl;
	else th1Stack = th1StackDummy;

	for (size_t iTh1I = 0; iTh1I != th1Indi.size(); ++iTh1I){
		th1Indi[iTh1I]->SetLineColor(SetColor_stark(iTh1I));
		th1Indi[iTh1I]->SetLineWidth(2);
		th1Indi[iTh1I]->GetXaxis()->SetTitleSize(0.05); // can't get this to work via tstyle
		th1Indi[iTh1I]->GetXaxis()->SetLabelSize(0.04);
		th1Indi[iTh1I]->GetYaxis()->SetTitleSize(0.05);
		th1Indi[iTh1I]->GetYaxis()->SetLabelSize(0.04);
	}
	for (size_t iTh1S = 0; iTh1S != th1Stack.size(); ++iTh1S){
		th1Stack[iTh1S]->SetFillColor(SetColor_mellow(iTh1S+2, th1Stack.size()));
		th1Stack[iTh1S]->SetLineWidth(0.0);
		th1Stack[iTh1S]->GetXaxis()->SetTitleSize(0.05); // can't get this to work via tstyle
		th1Stack[iTh1S]->GetXaxis()->SetLabelSize(0.04);
		th1Stack[iTh1S]->GetYaxis()->SetTitleSize(0.05);
		th1Stack[iTh1S]->GetYaxis()->SetLabelSize(0.04);
	}
}


//-----------public-----------//
void Plotter::AddLegend(TLegend * legDummy)
{
	leg = legDummy;
	leg->SetBorderSize(0);
	for (std::vector<PlotEntry>::const_iterator iStack = histoStack.begin(); iStack != histoStack.end(); ++iStack)
		leg->AddEntry(iStack->GetHistogram(), iStack->GetPlotEntryName().c_str(), "f");

	for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi)
		leg->AddEntry(iIndi->GetHistogram(), iIndi->GetPlotEntryName().c_str(), "L");	

	return;
}

void Plotter::AddLegend(const double& x1, const double& x2, const double& y1, const double& y2, const double& textSize)
{
	leg = new TLegend();
    leg->SetX1NDC(x1);
    leg->SetX2NDC(x2);
	leg->SetY1NDC(y1);
    leg->SetY2NDC(y2);
	leg->SetTextSize(textSize);
	leg->SetBorderSize(0);
	for (std::vector<PlotEntry>::const_iterator iStack = histoStack.begin(); iStack != histoStack.end(); ++iStack)
		leg->AddEntry(iStack->GetHistogram(), iStack->GetPlotEntryName().c_str(), "f");

	for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi)
		leg->AddEntry(iIndi->GetHistogram(), iIndi->GetPlotEntryName().c_str(), "L");	

	return;
}

void Plotter::AddLegend(const std::vector<std::string>& legendNames, const double& x1, const double& x2, const double& y1, const double& y2, const double& textSize)
{
	if (legendNames.size() != (th1Indi.size() + th1Stack.size()) ){
		std::cout << "The legend you provided does not have the correct number of strings to match th1Indi+th1Stack" << std::endl;
		return;
	}
	leg = new TLegend();
    leg->SetX1NDC(x1);
    leg->SetX2NDC(x2);
	leg->SetY1NDC(y1);
    leg->SetY2NDC(y2);
	leg->SetTextSize(textSize);
	leg->SetBorderSize(0);
	for (size_t i = 0; i < legendNames.size(); ++i){
		if (i < th1Indi.size()) leg->AddEntry(th1Indi[i], legendNames[i].c_str(), "L");
		else leg->AddEntry(th1Stack[i-th1Indi.size()], legendNames[i].c_str(), "f");
	}
	return;
}

void Plotter::AddLatex(const double& lumiValueDummy, const std::string& lhsStringAfterCMSDummy)
{
	addLatex = true;
	lumiLabel = Form("%.1f fb^{-1} (13 TeV)", lumiValueDummy);
	lhsStringAfterCMS = lhsStringAfterCMSDummy;

	return;
}

void Plotter::AddLatex(const std::string& lhsStringAfterCMSDummy)
{
	addLatex = true;
	lumiLabel = "(13 TeV)";
	lhsStringAfterCMS = lhsStringAfterCMSDummy;

	return;
}


TStyle * Plotter::GetTStyle() {return tdrStyle;} // get it, to edit it


void Plotter::SetLogY(){
	useLogY = true;
	return;
}


void Plotter::SetLogZ(){
	useLogZ = true;
	return;
}


void Plotter::SetErrors(const std::string& errorInfo){

	if (errorInfo == "only_stack"){
		plotWithErrorsStack = true;
		return;
	}
	else{		
		plotWithErrorsIndi = true;
		for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi){
			
			iIndi->GetHistogram()->SetMarkerStyle(21);
			iIndi->GetHistogram()->SetMarkerSize(0.2);
			iIndi->GetHistogram()->SetLineWidth(3);
			for (int iBin = 0; iBin < iIndi->GetHistogram()->GetNbinsX()+2; ++iBin){
				iIndi->GetHistogram()->SetBinError(iBin, sqrt(iIndi->GetStatErrorSquaredVector()[iBin]));
			}
		}

		for (size_t iTh1I = 0; iTh1I != th1Indi.size(); ++iTh1I){
			
			th1Indi[iTh1I]->SetMarkerStyle(21);
			th1Indi[iTh1I]->SetMarkerSize(0.2);
			th1Indi[iTh1I]->SetLineWidth(3);
		}
	}
	if (errorInfo != "only_indi") plotWithErrorsStack = true;
}
	

void Plotter::Save(const std::string& saveName){

	if (histoStack.empty() && histoIndi.empty()){
		std::cout << "Plotter::Save @@@ Exiting without saving... no histos @@@" << std::endl;
		return;
	}

	tdrStyle->cd();
	TCanvas * c = new TCanvas("c","c");
	if (useLogY) gPad->SetLogy();

	std::string hsTitles = ""; // NB, can't set title for THStack after construction
	if (!histoStack.empty()) hsTitles = Form("%s;%s;%s", histoStack[0].GetHistogram()->GetTitle(), histoStack[0].GetHistogram()->GetXaxis()->GetTitle(), histoStack[0].GetHistogram()->GetYaxis()->GetTitle());
	THStack * hs = new THStack("hs", hsTitles.c_str());
	for (std::vector<PlotEntry>::const_iterator iStack = histoStack.begin(); iStack != histoStack.end(); ++iStack)
		hs->Add(iStack->GetHistogram());

	/////////////////////////////////////////////////////////////////////////////////////////////
	// find max and min (don't want zero values for min, they won't work properly for logY plots)
	// NB this currently might not do logY minimums correctly for stacks
	double max = 0.0;
	double min = 0.0; // for logY plots
	bool setMin = false;
	for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi){
		
		// MAX from histoIndi
		if (plotWithErrorsIndi == false){
			if (iIndi->GetHistogram()->GetMaximum() > max || iIndi == histoIndi.begin()) max = iIndi->GetHistogram()->GetMaximum();
		}
		else{
			for (int i=1; i != iIndi->GetHistogram()->GetNbinsX()+1; ++i){
				if (iIndi->GetHistogram()->GetBinContent(i) + sqrt(iIndi->GetStatErrorSquaredVector()[i]) > max || (iIndi == histoIndi.begin() && i==1) ) max = iIndi->GetHistogram()->GetBinContent(i) + sqrt(iIndi->GetStatErrorSquaredVector()[i]);
			}
		}

		// nonzero MIN from histoIndi
		double histoMinNonZero = 0.0; // lowest non zero value of the histogram
		bool setHistoMinNonZero = false;
		// first calculate without error bars
		for (int i=1; i != iIndi->GetHistogram()->GetNbinsX()+1; ++i)
			if (iIndi->GetHistogram()->GetBinContent(i) > 0 && (iIndi->GetHistogram()->GetBinContent(i) < histoMinNonZero || setHistoMinNonZero == false) ){
				histoMinNonZero = iIndi->GetHistogram()->GetBinContent(i);
				setHistoMinNonZero = true;
			}
		if (setHistoMinNonZero == true && (histoMinNonZero < min || setMin == false)){
			min = histoMinNonZero;
			setMin = true;
		}
		// if (plotWithErrorsStack == true){ // CURRENTLY NOT USING ERRORS BARS IN THE MIN EVALUATION
		// 	for (int i=1; i != iIndi->GetHistogram()->GetNbinsX()+1; ++i)
		// 		if ( (iIndi->GetHistogram()->GetBinContent(i) - sqrt(iIndi->GetStatErrorSquaredVector()[i]) > 0) && ( (iIndi->GetHistogram()->GetBinContent(i) - sqrt(iIndi->GetStatErrorSquaredVector()[i]) < histoMinNonZero) || setHistoMinNonZero == false ) ){
		// 			histoMinNonZero = iIndi->GetHistogram()->GetBinContent(i) - sqrt(iIndi->GetStatErrorSquaredVector()[i]);
		// 			setHistoMinNonZero = true;
		// 		}
		// 	if (setHistoMinNonZero == true && (histoMinNonZero < min || setMin == false)){
		// 		min = histoMinNonZero;
		// 		setMin = true;
		// 	}
		// }
	} // closes loop through histoIndi elements

	if (!histoStack.empty()){

		TH1D *histoStackClone = (TH1D*)histoStack[0].GetHistogram()->Clone();
		std::vector<double> histoStackCloneStatErrSqrdVec(histoStackClone->GetNbinsX()+2, 0.0);
		for (size_t iStack = 0; iStack != histoStack.size(); ++iStack){
			if (iStack != 0) histoStackClone->Add(histoStack[iStack].GetHistogram());
			for (int iBin = 0; iBin < histoStackClone->GetNbinsX()+2; ++iBin) histoStackCloneStatErrSqrdVec[iBin] += histoStack[iStack].GetStatErrorSquaredVector()[iBin];
		}
		for (int iBin = 0; iBin < histoStackClone->GetNbinsX()+2; ++iBin) histoStackClone->SetBinError(iBin, sqrt(histoStackCloneStatErrSqrdVec[iBin]));

		// MAX from histoStack
		if (plotWithErrorsStack == false){
			if (histoStackClone->GetMaximum() > max) max = histoStackClone->GetMaximum();
		}
		else{
			for (int i=1; i != histoStackClone->GetNbinsX()+1; ++i){
				if (histoStackClone->GetBinContent(i) + histoStackClone->GetBinError(i) > max) max = histoStackClone->GetBinContent(i) + histoStackClone->GetBinError(i);
			}
		}

		// nonzero MIN from histoStack
		double histoMinNonZero = 0.0; // lowest non zero value of the histogram
		bool setHistoMinNonZero = false; // have we found a non zero min value?
		for (int i=1; i != histoStackClone->GetNbinsX()+1; ++i){
			if (histoStackClone->GetBinContent(i) > 0 && (histoStackClone->GetBinContent(i) < histoMinNonZero || setHistoMinNonZero == false) ){
				histoMinNonZero = histoStackClone->GetBinContent(i);
				setHistoMinNonZero = true;
			}
			if (setHistoMinNonZero == true && (histoMinNonZero < min || setMin == false)){
				min = histoMinNonZero;
				setMin = true;
			}
		}
	} // closes 'if' histoStack has entries

	/////////////////////////////////////////////////////////////////////////////////////////////

	// set histo max and min and draw
	double initialMax = 0.0; // use to reset the histo max and min to what it initially was
	double initialMin = 0.0;

	if (!histoIndi.empty() && !histoStack.empty()){
		initialMax = histoIndi[0].GetHistogram()->GetMaximum();
		initialMin = histoIndi[0].GetHistogram()->GetMinimum();
		if (useLogY == false){
			histoIndi[0].GetHistogram()->SetMaximum(1.05 * max);
			histoIndi[0].GetHistogram()->SetMinimum(0);
		}
		else{
			histoIndi[0].GetHistogram()->SetMaximum(2.00 * max);
			histoIndi[0].GetHistogram()->SetMinimum(0.50 * min);
		}
		histoIndi[0].GetHistogram()->Draw();
		hs->Draw("same");
		for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi)
			if (plotWithErrorsIndi == false) iIndi->GetHistogram()->Draw("same");
			else iIndi->GetHistogram()->Draw("same P");
		if (plotWithErrorsStack){
			TH1D *histoStackClone = (TH1D*)histoStack[0].GetHistogram()->Clone();
			histoStackClone->SetFillColor(kBlack);
			histoStackClone->SetFillStyle(3004);
			std::vector<double> histoStackCloneStatErrSqrdVec(histoStackClone->GetNbinsX()+2, 0.0);
			for (size_t iStack = 0; iStack != histoStack.size(); ++iStack){
				if (iStack != 0) histoStackClone->Add(histoStack[iStack].GetHistogram());
				for (int iBin = 0; iBin < histoStackClone->GetNbinsX()+2; ++iBin) histoStackCloneStatErrSqrdVec[iBin] += histoStack[iStack].GetStatErrorSquaredVector()[iBin];
			}
			for (int iBin = 0; iBin < histoStackClone->GetNbinsX()+2; ++iBin) histoStackClone->SetBinError(iBin, sqrt(histoStackCloneStatErrSqrdVec[iBin]));
			histoStackClone->Draw("same, E2");
		}
	}

	else if (!histoIndi.empty() && histoStack.empty()){
		initialMax = histoIndi[0].GetHistogram()->GetMaximum();
		initialMin = histoIndi[0].GetHistogram()->GetMinimum();
		if (useLogY == false){
			histoIndi[0].GetHistogram()->SetMaximum(1.05 * max);
			histoIndi[0].GetHistogram()->SetMinimum(0);
		}
		else{
			histoIndi[0].GetHistogram()->SetMaximum(2.00 * max);
			histoIndi[0].GetHistogram()->SetMinimum(0.50 * min);
		}
		for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi)
			if (plotWithErrorsIndi == false) iIndi->GetHistogram()->Draw("same");
			else iIndi->GetHistogram()->Draw("same P");
	}

	else if (histoIndi.empty() && !histoStack.empty()){
		initialMax = histoStack[0].GetHistogram()->GetMaximum();
		initialMin = histoStack[0].GetHistogram()->GetMinimum();
		if (useLogY == false){
			histoStack[0].GetHistogram()->SetMaximum(1.05 * max);
			histoStack[0].GetHistogram()->SetMinimum(0);
		}
		else{
			histoStack[0].GetHistogram()->SetMaximum(2.00 * max);
			histoStack[0].GetHistogram()->SetMinimum(0.50 * min);
		}
		histoStack[0].GetHistogram()->Draw();
		hs->Draw("same");
		if (plotWithErrorsStack){
			TH1D *histoStackClone = (TH1D*)histoStack[0].GetHistogram()->Clone();
			histoStackClone->SetFillColor(kBlack);
			histoStackClone->SetFillStyle(3004);
			std::vector<double> histoStackCloneStatErrSqrdVec(histoStackClone->GetNbinsX()+2, 0.0);
			for (size_t iStack = 0; iStack != histoStack.size(); ++iStack){
				if (iStack != 0) histoStackClone->Add(histoStack[iStack].GetHistogram());
				for (int iBin = 0; iBin < histoStackClone->GetNbinsX()+2; ++iBin) histoStackCloneStatErrSqrdVec[iBin] += histoStack[iStack].GetStatErrorSquaredVector()[iBin];
			}
			for (int iBin = 0; iBin < histoStackClone->GetNbinsX()+2; ++iBin) histoStackClone->SetBinError(iBin, sqrt(histoStackCloneStatErrSqrdVec[iBin]));
			histoStackClone->Draw("same, E2");
		}
	}

	if (addLatex) DrawLatex();
	if (leg != NULL) leg->Draw("same");
	gPad->RedrawAxis();
	c->SaveAs(saveName.c_str());
	c->Close();
	// reset the max values of histograms that we altered
	if (!histoIndi.empty()){
		histoIndi[0].GetHistogram()->SetMaximum(initialMax);
		histoIndi[0].GetHistogram()->SetMinimum(initialMin);
	}
	else {
		histoStack[0].GetHistogram()->SetMaximum(initialMax);
		histoStack[0].GetHistogram()->SetMinimum(initialMin);
	}
	std::cout << std::endl;
	return;
} // closes function Save


void Plotter::SaveSpec01(const std::string& saveName, const std::vector<std::string> htBins){

	if (th1Indi.empty() && th1Stack.empty()){
		std::cout << "Plotter::SaveSpec01 @@@ Exiting without saving... no histos @@@" << std::endl;
		return;
	}

	tdrStyle->cd();
	TCanvas * c = new TCanvas("c","c");
	if (useLogY) gPad->SetLogy();

	std::string hsTitles = ""; // NB, can't set title for THStack after construction
	if (!th1Stack.empty()) hsTitles = Form("%s;%s;%s", th1Stack[0]->GetTitle(), th1Stack[0]->GetXaxis()->GetTitle(), th1Stack[0]->GetYaxis()->GetTitle());	
	THStack * hs = new THStack("hs", hsTitles.c_str());
	for (size_t iTh1S = 0; iTh1S != th1Stack.size(); ++iTh1S)
		hs->Add(th1Stack[iTh1S], "HIST");

	/////////////////////////////////////////////////////////////////////////////////////////////
	// find max and min (don't want zero values for min, they won't work properly for logY plots)
	double max = 0.0;
	double min = 0.0; // for logY plots only
	bool setMin = false;

	for (size_t iTh1I = 0; iTh1I != th1Indi.size(); ++iTh1I){
		
		// MAX from th1Indi
		if (plotWithErrorsIndi == false){
			if (th1Indi[iTh1I]->GetMaximum() > max || iTh1I == 0) max = th1Indi[iTh1I]->GetMaximum();
		}
		else{
			for (int i=1; i != th1Indi[iTh1I]->GetNbinsX()+1; ++i)
				if ( (th1Indi[iTh1I]->GetBinContent(i) + th1Indi[iTh1I]->GetBinError(i) > max) || (iTh1I == 0 && i==1) ) max = th1Indi[iTh1I]->GetBinContent(i) + th1Indi[iTh1I]->GetBinError(i);	
		}
		
		// nonzero MIN from th1Indi
		double histoMinNonZero = 0.0; // lowest non zero value of the histogram
		bool setHistoMinNonZero = false; // have we found a non zero min value?
		// first calculate without error bars
		for (int i=1; i != th1Indi[iTh1I]->GetNbinsX()+1; ++i)
			if (th1Indi[iTh1I]->GetBinContent(i) > 0 && (th1Indi[iTh1I]->GetBinContent(i) < histoMinNonZero || setHistoMinNonZero == false) ){
				histoMinNonZero = th1Indi[iTh1I]->GetBinContent(i);
				setHistoMinNonZero = true;
			}
		if (setHistoMinNonZero == true && (histoMinNonZero < min || setMin == false)){
			min = histoMinNonZero;
			setMin = true;
		}
		// if (plotWithErrorsStack == true){ // CURRENTLY NOT USING ERRORS BARS IN THE MIN EVALUATION
		// 	for (int i=1; i != th1Indi[iTh1I]->GetNbinsX()+1; ++i)
		// 		if ( (th1Indi[iTh1I]->GetBinContent(i) - th1Indi[iTh1I]->GetBinError(i) > 0) && ( (th1Indi[iTh1I]->GetBinContent(i) - th1Indi[iTh1I]->GetBinError(i) < histoMinNonZero) || setHistoMinNonZero == false ) ){
		// 			histoMinNonZero = th1Indi[iTh1I]->GetBinContent(i) - th1Indi[iTh1I]->GetBinError(i);
		// 			setHistoMinNonZero = true;
		// 		}
		// 	if (setHistoMinNonZero == true && (histoMinNonZero < min || setMin == false)){
		// 		min = histoMinNonZero;
		// 		setMin = true;
		// 	}
		// }
	} // closes loop through th1Indi elements

	if (!th1Stack.empty()){

		TH1D *histoStackClone = (TH1D*)th1Stack[0]->Clone();
		for (size_t iTh1S = 1; iTh1S != th1Stack.size(); ++iTh1S) histoStackClone->Add(th1Stack[iTh1S]);

		// MAX from th1Stack
		if (plotWithErrorsStack == false){
			if (histoStackClone->GetMaximum() > max) max = histoStackClone->GetMaximum();
		}
		else{
			for (int i=1; i != histoStackClone->GetNbinsX()+1; ++i){
				if (histoStackClone->GetBinContent(i) + histoStackClone->GetBinError(i) > max) max = histoStackClone->GetBinContent(i) + histoStackClone->GetBinError(i);
			}
		}

		// nonzero MIN from th1Stack
		double histoMinNonZero = 0.0; // lowest non zero value of the histogram
		bool setHistoMinNonZero = false; // have we found a non zero min value?
		for (int i=1; i != histoStackClone->GetNbinsX()+1; ++i){
			if (histoStackClone->GetBinContent(i) > 0 && (histoStackClone->GetBinContent(i) < histoMinNonZero || setHistoMinNonZero == false) ){
				histoMinNonZero = histoStackClone->GetBinContent(i);
				setHistoMinNonZero = true;
			}
			if (setHistoMinNonZero == true && (histoMinNonZero < min || setMin == false)){
				min = histoMinNonZero;
				setMin = true;
			}
		}
	} // closes 'if' th1Stack has entries

	/////////////////////////////////////////////////////////////////////////////////////////////

	// set histo max and min and draw
	double initialMax = 0.0; // use to reset the histo max and min to what it initially was
	double initialMin = 0.0;

	if (!th1Indi.empty() && !th1Stack.empty()){
		initialMax = th1Indi[0]->GetMaximum();
		initialMin = th1Indi[0]->GetMinimum();
		if (useLogY == false){
			th1Indi[0]->SetMaximum(1.15 * max);
			th1Indi[0]->SetMinimum(0);
		}
		else{
			th1Indi[0]->SetMaximum(2.00 * max);
			th1Indi[0]->SetMinimum(0.50 * min);
		}
		if (!plotWithErrorsIndi) th1Indi[0]->Draw("HIST");
		else th1Indi[0]->Draw("P");
		hs->Draw("same");
		for (size_t iTh1I = 0; iTh1I != th1Indi.size(); ++iTh1I)
			if (plotWithErrorsIndi == false) th1Indi[iTh1I]->Draw("HIST, same");
			else th1Indi[iTh1I]->Draw("same, P");
		if (plotWithErrorsStack){
			TH1D *histoStackClone = (TH1D*)th1Stack[0]->Clone();
			histoStackClone->SetFillColor(kBlack);
			histoStackClone->SetFillStyle(3004);
			for (size_t iTh1S = 1; iTh1S != th1Stack.size(); ++iTh1S) histoStackClone->Add(th1Stack[iTh1S]);
			histoStackClone->Draw("same, E2");
		}

	}

	else if (!th1Indi.empty() && th1Stack.empty()){
		initialMax = th1Indi[0]->GetMaximum();
		initialMin = th1Indi[0]->GetMinimum();
		if (useLogY == false){
			th1Indi[0]->SetMaximum(1.15 * max);
			th1Indi[0]->SetMinimum(0);
		}
		else{
			th1Indi[0]->SetMaximum(2.00 * max);
			th1Indi[0]->SetMinimum(0.50 * min);
		}
		for (size_t iTh1I = 0; iTh1I != th1Indi.size(); ++iTh1I)
			if (plotWithErrorsIndi == false) th1Indi[iTh1I]->Draw("HIST, same");
			else th1Indi[iTh1I]->Draw("same, P");
	}

	else if (th1Indi.empty() && !th1Stack.empty()){
		initialMax = th1Stack[0]->GetMaximum();
		initialMin = th1Stack[0]->GetMinimum();
		if (useLogY == false){
			th1Stack[0]->SetMaximum(1.15 * max);
			th1Stack[0]->SetMinimum(0);
		}
		else{
			th1Stack[0]->SetMaximum(2.00 * max);
			th1Stack[0]->SetMinimum(0.50 * min);
		}
		th1Stack[0]->Draw("HIST");
		hs->Draw("same");
		if (plotWithErrorsStack){
			TH1D *histoStackClone = (TH1D*)th1Stack[0]->Clone();
			histoStackClone->SetFillColor(kBlack);
			histoStackClone->SetFillStyle(3004);
			for (size_t iTh1S = 1; iTh1S != th1Stack.size(); ++iTh1S) histoStackClone->Add(th1Stack[iTh1S]);
			histoStackClone->Draw("same, E2");
		}
	}

	if (addLatex) DrawLatex();
	if (leg != NULL) leg->Draw("same");
	gPad->RedrawAxis();

	/////////////////////////////////////////////////////////////////////////////////////////////

	// Add the HT division Lines
	c->Update();
	unsigned int numberOfBins = 0;
	if (!th1Indi.empty()) numberOfBins = th1Indi[0]->GetNbinsX();
	else numberOfBins = th1Stack[0]->GetNbinsX();
	unsigned int binsPerDivision = numberOfBins / htBins.size();
	for (unsigned int c = 0; c < numberOfBins; c = c + binsPerDivision){

		if (c != 0){
			TLine * line = new TLine(c, 0, c, max); // xmin, ymin, xmax, ymax
			line->SetLineStyle(2);
			line->SetLineWidth(3);
			line->Draw();
		}
		TLatex * latexHT = new TLatex();
	    latexHT->SetTextFont(42);
	    latexHT->SetTextAlign(11); // align from left
	    latexHT->DrawLatex(c+1, 1.05 * max, htBins[c/binsPerDivision].c_str());
	}

	/////////////////////////////////////////////////////////////////////////////////////////////

	c->SaveAs(saveName.c_str());
	c->Close();
	// reset the max values of histograms that we altered
	if (!th1Indi.empty()){
		th1Indi[0]->SetMaximum(initialMax);
		th1Indi[0]->SetMinimum(initialMin);
	}
	else {
		th1Stack[0]->SetMaximum(initialMax);
		th1Stack[0]->SetMinimum(initialMin);
	}
	std::cout << std::endl;
	return;
} // closes function SaveSpec01


void Plotter::Save2D(const std::string& saveName){

	if (histos2D.empty()){
		std::cout << "Plotter::Save2D @@@ Exiting without saving... no 2D histos @@@" << std::endl;
		return;
	}

	tdrStyle->cd();
	double default_PadRightMargin = tdrStyle->GetPadRightMargin();
	double default_PadLeftMargin = tdrStyle->GetPadLeftMargin();
	// std::cout << default_PadLeftMargin << " " << default_PadRightMargin << std::endl;
	tdrStyle->SetPadRightMargin(0.11);
	tdrStyle->SetPadLeftMargin(0.11);

	TCanvas * c = new TCanvas("c","c");
	if (useLogZ) gPad->SetLogz();

	for (std::vector<PlotEntry2D>::const_iterator iHistos2D = histos2D.begin(); iHistos2D != histos2D.end(); ++iHistos2D){
		iHistos2D->GetHistogram()->SetEntries(1);
		iHistos2D->GetHistogram()->Draw("colz, same");
		// iHistos2D->GetHistogram()->Draw("colz, same, text");
	}

	if (addLatex) DrawLatex();
	c->SaveAs(saveName.c_str());
	c->Close();
	
	tdrStyle->SetPadRightMargin(default_PadRightMargin);
	tdrStyle->SetPadRightMargin(default_PadLeftMargin);
	std::cout << std::endl;
	return;
}


void Plotter::Save2D(const std::string& saveName, const MassRegionCuts& MassCutsObject){

	if (histos2D.empty()){
		std::cout << "Plotter::Save2D @@@ Exiting without saving... no 2D histos @@@" << std::endl;
		return;
	}

	tdrStyle->cd();
	double default_PadRightMargin = tdrStyle->GetPadRightMargin();
	double default_PadLeftMargin = tdrStyle->GetPadLeftMargin();
	// std::cout << default_PadLeftMargin << " " << default_PadRightMargin << std::endl;
	tdrStyle->SetPadRightMargin(0.11);
	tdrStyle->SetPadLeftMargin(0.11);

	TCanvas * c = new TCanvas("c","c");
	if (useLogZ) gPad->SetLogz();

	for (std::vector<PlotEntry2D>::const_iterator iHistos2D = histos2D.begin(); iHistos2D != histos2D.end(); ++iHistos2D){
		iHistos2D->GetHistogram()->SetEntries(1);
		iHistos2D->GetHistogram()->Draw("colz, same");
		// iHistos2D->GetHistogram()->Draw("colz, same, text");
	}

	if (addLatex) DrawLatex();

	// **************************
	// NOW ADD THE MASS CUT LINES
	// **************************
	c->Update();
	
	double S1_Node1 = MassCutsObject.Get_S1_Node1();
	double S1_Node2 = MassCutsObject.Get_S1_Node2();
	double SMAX_Node1 = MassCutsObject.Get_SMAX_Node1();
	double SMAX_Node2 = MassCutsObject.Get_SMAX_Node2();
	std::vector<double> SN_Nodes = MassCutsObject.Get_SN_Nodes();

	double gradientUpperSignalLine = (SMAX_Node1 - S1_Node1) / (SMAX_Node2 - S1_Node2);
	double gradientLowerSignalLine = 1 / gradientUpperSignalLine;
	double upperBand_x1 = S1_Node2 - 0.5 * (S1_Node1 - S1_Node2);
	double upperBand_y1 = S1_Node1 + 0.5 * (S1_Node1 - S1_Node2);
	double upperBand_x2 = SMAX_Node2 - 0.5 * (SMAX_Node1 - SMAX_Node2);
	double upperBand_y2 = SMAX_Node1 + 0.5 * (SMAX_Node1 - SMAX_Node2);
	double gradientUpperBand = (upperBand_y2 - upperBand_y1) / (upperBand_x2 - upperBand_x1);
	double gradientDownerBand = 1 / gradientUpperBand;

	// work out the coords for 'upper' corner of upper segement 1
	double yValue_S1UpperLeft = SN_Nodes[0];
	double xValue_S1UpperLeft = gradientLowerSignalLine * (SN_Nodes[0] - S1_Node1) + S1_Node2;
	double upperBand_x1U = xValue_S1UpperLeft - 0.5 * (yValue_S1UpperLeft - xValue_S1UpperLeft);
	double upperBand_y1U = yValue_S1UpperLeft + 0.5 * (yValue_S1UpperLeft - xValue_S1UpperLeft);
	TLine *line_U1Cap = new TLine(S1_Node2, S1_Node1, upperBand_x1U, upperBand_y1U); // xmin, ymin, xmax, ymax
	line_U1Cap->SetLineStyle(2);
	line_U1Cap->SetLineWidth(3);
	line_U1Cap->Draw();
	TLine *line_D1Cap = new TLine(S1_Node1, S1_Node2, upperBand_y1U, upperBand_x1U); // xmin, ymin, xmax, ymax
	line_D1Cap->SetLineStyle(2);
	line_D1Cap->SetLineWidth(3);
	line_D1Cap->Draw();	

	// bottom diagnol line for signal regions
	TLine *line_signalBottom = new TLine(S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2); // xmin, ymin, xmax, ymax
	line_signalBottom->SetLineStyle(2);
	line_signalBottom->SetLineWidth(3);
	line_signalBottom->Draw();

	// top diagnol line for signal regions
	TLine *line_signalTop = new TLine(S1_Node2, S1_Node1, SMAX_Node2, SMAX_Node1); // xmin, ymin, xmax, ymax
	line_signalTop->SetLineStyle(2);
	line_signalTop->SetLineWidth(3);
	line_signalTop->Draw();

	// caps the topRight segment of final signal region and U,D wings
	TLine *line_signalSegmentTop = new TLine(upperBand_x2, upperBand_y2, upperBand_y2, upperBand_x2); // xmin, ymin, xmax, ymax
	line_signalSegmentTop->SetLineStyle(2);
	line_signalSegmentTop->SetLineWidth(3);
	line_signalSegmentTop->Draw();

	// caps the bottomLeft segment of first signal region and U,D wings
	TLine *line_downerBottomLeft = new TLine(S1_Node1, S1_Node2, S1_Node2, S1_Node1); // xmin, ymin, xmax, ymax
	line_downerBottomLeft->SetLineStyle(2);
	line_downerBottomLeft->SetLineWidth(3);
	line_downerBottomLeft->Draw();

	// top line of upper band
	TLine *line_upperBand = new TLine(upperBand_x1U, upperBand_y1U, upperBand_x2, upperBand_y2);
	line_upperBand->SetLineStyle(2);
	line_upperBand->SetLineWidth(3);
	line_upperBand->Draw();

	// bottom line of downer band
	TLine *line_downBand = new TLine(upperBand_y1U, upperBand_x1U, upperBand_y2, upperBand_x2);
	line_downBand->SetLineStyle(2);
	line_downBand->SetLineWidth(3);
	line_downBand->Draw();

	// does the line segments...
	for (size_t i=0; i!=SN_Nodes.size(); ++i){
		
		double yValueSignalLower = gradientLowerSignalLine * (SN_Nodes[i] - S1_Node1) + S1_Node2;
		double xValueDownerLower = (yValueSignalLower - upperBand_x1 + gradientDownerBand * upperBand_y1 + SN_Nodes[i]) / (gradientDownerBand + 1);
		double yValueDownerLower = gradientDownerBand * (xValueDownerLower - upperBand_y1) + upperBand_x1;

		TLine *line_signalSegment = new TLine(xValueDownerLower, yValueDownerLower, yValueDownerLower, xValueDownerLower); // xmin, ymin, xmax, ymax
		line_signalSegment->SetLineStyle(2);
		line_signalSegment->SetLineWidth(3);
		line_signalSegment->Draw();
	}
	// **************************
	// **************************
	// **************************

	c->SaveAs(saveName.c_str());
	c->Close();
	
	tdrStyle->SetPadRightMargin(default_PadRightMargin);
	tdrStyle->SetPadRightMargin(default_PadLeftMargin);
	std::cout << std::endl;
	return;
}


//-----------private----------//
void Plotter::DrawLatex()
{
	TLatex * latex = new TLatex();
    latex->SetNDC();
    latex->SetTextFont(42);
    
    latex->SetTextAlign(11); // align from left
    latex->DrawLatex(0.15,0.92,Form("#bf{CMS} %s", lhsStringAfterCMS.c_str()));

    latex->SetTextAlign(31); // align from right
	latex->DrawLatex(0.92,0.92,lumiLabel.c_str());

	return;
}


int Plotter::SetColor_mellow(int position, const int& maxColors)
{
	gStyle->SetPalette(55); // sets what sort of colours we will use	
	double modifier = 0.00; // modifier is an offset in the colour spectrum
	double colorIndex;
	int colour = 1;
	double fraction = (double)(position)/(double)(maxColors);
	// double fraction = (double)(maxColors) - (double)(position)/(double)(maxColors) - 1.0;
	if (position < 0 || maxColors < 0 ) colour = 1;
	else
	{
		if (position > maxColors) position = position % maxColors;
	    colorIndex = (fraction + modifier) * gStyle->GetNumberOfColors();
	    colour = gStyle->GetColorPalette(colorIndex);
	}
	return colour;
}


int Plotter::SetColor_stark(const int& index)
{
	if (index==0) return kRed;
	if (index==1) return kBlue;
	if (index==2) return kGreen+1;
	// if (index==2) return kMagenta;
	if (index==3) return kOrange+1;
	// if (index==3) return kBlack;
	if (index==4) return kMagenta-4;
	if (index==5) return kCyan+1;
	else return kBlack;
}


TStyle * Plotter::TDRStyle()
{
	TStyle * tdrStyle = new TStyle("tdrStyle","");
	//tdrStyle->SetPalette(palette);

	// For the canvas:
	tdrStyle->SetCanvasBorderMode(0);
	tdrStyle->SetCanvasColor(kWhite);
	tdrStyle->SetCanvasDefH(600); //Height of canvas
	tdrStyle->SetCanvasDefW(800); //Width of canvas
	tdrStyle->SetCanvasDefX(0);   //Position on screen
	tdrStyle->SetCanvasDefY(0);

	// For the Pad:
	tdrStyle->SetPadBorderMode(0);
	// tdrStyle->SetPadBorderSize(Width_t size = 1);
	tdrStyle->SetPadColor(kWhite);
	tdrStyle->SetPadGridX(false);
	tdrStyle->SetPadGridY(false);
	tdrStyle->SetGridColor(0);
	tdrStyle->SetGridStyle(3);
	tdrStyle->SetGridWidth(1);
	tdrStyle->SetPadGridX(false);
	tdrStyle->SetPadGridY(false);

	// For the frame:
	tdrStyle->SetFrameBorderMode(0);
	tdrStyle->SetFrameBorderSize(1);
	tdrStyle->SetFrameFillColor(0);
	tdrStyle->SetFrameFillStyle(0);
	tdrStyle->SetFrameLineColor(1);
	tdrStyle->SetFrameLineStyle(1);
	tdrStyle->SetFrameLineWidth(1);

	// For the histo:
	// tdrStyle->SetHistFillColor(1);
	// tdrStyle->SetHistFillStyle(0);
	tdrStyle->SetHistLineColor(1);
	tdrStyle->SetHistLineStyle(0);
	tdrStyle->SetHistLineWidth(1);
	// tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
	// tdrStyle->SetNumberContours(Int_t number = 20);

	// tdrStyle->SetEndErrorSize(2);
	// tdrStyle->SetErrorMarker(20);
	// tdrStyle->SetErrorX(0.);
	// tdrStyle->SetMarkerStyle(20);

	//For the legend
	tdrStyle->SetLegendBorderSize(0);
	tdrStyle->SetLegendFillColor(0);
	tdrStyle->SetLegendFont(42);
	tdrStyle->SetLegendTextSize(0.05);

	//For the fit/function:
	tdrStyle->SetOptFit(0);
	tdrStyle->SetFitFormat("5.4g");
	tdrStyle->SetFuncColor(2);
	tdrStyle->SetFuncStyle(1);
	tdrStyle->SetFuncWidth(1);

	//For the date:
	tdrStyle->SetOptDate(0);
	// tdrStyle->SetDateX(Float_t x = 0.01);
	// tdrStyle->SetDateY(Float_t y = 0.01);

	// For the statistics box:
	tdrStyle->SetOptFile(0);
	tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
	tdrStyle->SetStatColor(kWhite);
	tdrStyle->SetStatFont(42);
	tdrStyle->SetStatFontSize(0.025);
	tdrStyle->SetStatTextColor(1);
	tdrStyle->SetStatFormat("6.4g");
	tdrStyle->SetStatBorderSize(1);
	tdrStyle->SetStatH(0.1);
	tdrStyle->SetStatW(0.15);
	// tdrStyle->SetStatStyle(Style_t style = 1001);
	// tdrStyle->SetStatX(Float_t x = 0);
	// tdrStyle->SetStatY(Float_t y = 0);

	// Margins:
	//tdrStyle->SetPadTopMargin(0.05);
	tdrStyle->SetPadTopMargin(0.10);
	tdrStyle->SetPadBottomMargin(0.13);
	// tdrStyle->SetPadLeftMargin(0.16);
	tdrStyle->SetPadLeftMargin(0.14);
	// tdrStyle->SetPadRightMargin(0.02);
	tdrStyle->SetPadRightMargin(0.07);
	// tdrStyle->SetPadRightMargin(0.10); // only really want to be changing this for the scatters

	// For the Global title:
	tdrStyle->SetOptTitle(0);
	tdrStyle->SetTitleFont(42);
	tdrStyle->SetTitleColor(1);
	tdrStyle->SetTitleTextColor(1);
	tdrStyle->SetTitleFillColor(10);
	tdrStyle->SetTitleFontSize(0.05);
	// tdrStyle->SetTitleH(0); // Set the height of the title box
	// tdrStyle->SetTitleW(0); // Set the width of the title box
	// tdrStyle->SetTitleX(0); // Set the position of the title box
	// tdrStyle->SetTitleY(0.985); // Set the position of the title box
	// tdrStyle->SetTitleStyle(Style_t style = 1001);
	// tdrStyle->SetTitleBorderSize(2);

	// For the axis titles:
	tdrStyle->SetTitleColor(1, "XYZ");
	tdrStyle->SetTitleFont(42, "XYZ");
	tdrStyle->SetTitleSize(0.06, "XYZ");
	// tdrStyle->SetTitleXSize(0.06); // Another way to set the size?
	tdrStyle->SetTitleXOffset(0.95);//EDITFROM 0.9
	tdrStyle->SetTitleYOffset(1.25);
	// tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

	// For the axis labels:
	tdrStyle->SetLabelColor(1, "XYZ");
	tdrStyle->SetLabelFont(42, "XYZ");
	tdrStyle->SetLabelOffset(0.007, "XYZ");
	// tdrStyle->SetLabelSize(0.05, "XYZ");
	tdrStyle->SetLabelSize(0.04, "XYZ");

	// For the axis:
	tdrStyle->SetAxisColor(1, "XYZ");
	tdrStyle->SetStripDecimals(kTRUE);
	tdrStyle->SetTickLength(0.03, "XYZ");
	tdrStyle->SetNdivisions(510, "XYZ");
	tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
	tdrStyle->SetPadTickY(1);

	// Change for log plots:
	tdrStyle->SetOptLogx(0);
	tdrStyle->SetOptLogy(0);
	tdrStyle->SetOptLogz(0);

	// Postscript options:
	tdrStyle->SetPaperSize(20.,20.);
	// tdrStyle->SetLineScalePS(Float_t scale = 3);
	// tdrStyle->SetLineStyleString(Int_t i, const char* text);
	// tdrStyle->SetHeaderPS(const char* header);
	// tdrStyle->SetTitlePS(const char* pstitle);

	// tdrStyle->SetBarOffset(Float_t baroff = 0.5);
	// tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
	// tdrStyle->SetPaintTextFormat(const char* format = "g");
	// tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
	// tdrStyle->SetTimeOffset(Double_t toffset);
	// tdrStyle->SetHistMinimumZero(kTRUE);

	//tdrStyle->cd();
	return tdrStyle;
} // closes function 'TDRStyle'