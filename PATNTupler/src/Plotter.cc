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
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <THStack.h>

//RAL PARTICLE HEADERS
#include "../interface/PlotEntry.hh"
#include "../interface/Plotter.hh"


//--------constructor---------//
Plotter::Plotter(std::vector<PlotEntry> histoIndiDummy) :
leg(0),
addLatex(false),
useLogY(false),
plotWithErrors(false),
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
plotWithErrors(false),
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
		histoStack[iStack].GetHistogram()->SetFillColor(SetColor_mellow(iStack, histoStack.size()));	
		histoStack[iStack].GetHistogram()->SetLineWidth(0.0);
		histoStack[iStack].GetHistogram()->GetXaxis()->SetTitleSize(0.05); // can't get this to work via tstyle
		histoStack[iStack].GetHistogram()->GetXaxis()->SetLabelSize(0.04);
		histoStack[iStack].GetHistogram()->GetYaxis()->SetTitleSize(0.05);
		histoStack[iStack].GetHistogram()->GetYaxis()->SetLabelSize(0.04);
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


void Plotter::SetErrors(){

	// THIS FUNCTION NEEDS EXPANDING - to encapsulate styles more - maybe will require an argument(s)
	plotWithErrors = true;
	
	for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi){
		
		iIndi->GetHistogram()->SetMarkerStyle(21);
		iIndi->GetHistogram()->SetMarkerSize(0.1);
		for (int iBin = 0; iBin < iIndi->GetHistogram()->GetNbinsX()+2; ++iBin){
			iIndi->GetHistogram()->SetBinError(iBin, sqrt(iIndi->GetStatErrorSquaredVector()[iBin]));
		}
	}

	for (std::vector<PlotEntry>::const_iterator iStack = histoStack.begin(); iStack != histoStack.end(); ++iStack){
		for (int iBin = 0; iBin < iStack->GetHistogram()->GetNbinsX()+2; ++iBin){
			iStack->GetHistogram()->SetBinError(iBin, sqrt(iStack->GetStatErrorSquaredVector()[iBin]));
		}
	}
}


void Plotter::Save(const std::string& saveName){

	if (histoStack.empty() && histoIndi.empty()){
		std::cout << "Plotter::Save @@@ Exiting without saving... no histos @@@" << std::endl;
		return;
	}

	tdrStyle->cd();
	TCanvas * c = new TCanvas("c","c");
	if (useLogY) gPad->SetLogy();

	std::string hsTitles = ""; // can't set them later w/o a seg fault
	if (!histoStack.empty()) hsTitles = Form("%s;%s;%s", histoStack[0].GetHistogram()->GetTitle(), histoStack[0].GetHistogram()->GetXaxis()->GetTitle(), histoStack[0].GetHistogram()->GetYaxis()->GetTitle());
	THStack * hs = new THStack("hs", hsTitles.c_str());
	for (std::vector<PlotEntry>::const_iterator iStack = histoStack.begin(); iStack != histoStack.end(); ++iStack)
		hs->Add(iStack->GetHistogram());

	double initialMax = 0.0;
	double max = 0.0;
	for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi)
		if (iIndi->GetHistogram()->GetMaximum() > max) max = iIndi->GetHistogram()->GetMaximum();
	if (!histoStack.empty() && hs->GetMaximum() > max) max = hs->GetMaximum();
	
	if (!histoIndi.empty() && !histoStack.empty()){
		initialMax = histoIndi[0].GetHistogram()->GetMaximum();
		if (useLogY == false){
			histoIndi[0].GetHistogram()->SetMaximum(1.05 * max);
			histoIndi[0].GetHistogram()->SetMinimum(0);
		}
		else histoIndi[0].GetHistogram()->SetMaximum(2.00 * max);
		histoIndi[0].GetHistogram()->Draw();
		hs->Draw("same");
		for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi)
			if (plotWithErrors == false) iIndi->GetHistogram()->Draw("same");
			else iIndi->GetHistogram()->Draw("same P");
	}
	else if (!histoIndi.empty() && histoStack.empty()){
		initialMax = histoIndi[0].GetHistogram()->GetMaximum();
		if (useLogY == false){
			histoIndi[0].GetHistogram()->SetMaximum(1.05 * max);
			histoIndi[0].GetHistogram()->SetMinimum(0);
		}
		else histoIndi[0].GetHistogram()->SetMaximum(2.00 * max);
		for (std::vector<PlotEntry>::const_iterator iIndi = histoIndi.begin(); iIndi != histoIndi.end(); ++iIndi)
			if (plotWithErrors == false) iIndi->GetHistogram()->Draw("same");
			else iIndi->GetHistogram()->Draw("same P");
	}
	else if (histoIndi.empty() && !histoStack.empty()){
		initialMax = histoStack[0].GetHistogram()->GetMaximum();
		if (useLogY == false){
			histoIndi[0].GetHistogram()->SetMaximum(1.05 * max);
			histoIndi[0].GetHistogram()->SetMinimum(0);
		}
		else histoIndi[0].GetHistogram()->SetMaximum(2.00 * max);
		histoStack[0].GetHistogram()->Draw();
		hs->Draw("same");
	}
	if (addLatex) DrawLatex();
	if (leg != NULL) leg->Draw("same");
	gPad->RedrawAxis();
	c->SaveAs(saveName.c_str());
	c->Close();
	// reset the max values of histograms that we altered
	if (!histoIndi.empty()) histoIndi[0].GetHistogram()->SetMaximum(initialMax);
	else histoStack[0].GetHistogram()->SetMaximum(initialMax);
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


int Plotter::SetColor_mellow(const int& position, const int& maxColors)
{
	gStyle->SetPalette(55); // sets what sort of colours we will use	
	double modifier = 0.00; // modifier is an offset in the colour spectrum
	double colorIndex;
	int colour = 1;
	double fraction = (double)(position)/(double)(maxColors);
	if( position > maxColors || position < 0 || maxColors < 0 ) colour = 1;
	else
	{
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
	if (index==3) return kMagenta;
	if (index==4) return kCyan;
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