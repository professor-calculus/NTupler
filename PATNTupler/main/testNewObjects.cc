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
#include <TString.h>
#include <TLorentzVector.h> 
#include <TH1F.h>
#include <TH2F.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <THStack.h>

//RAL PARTICLE HEADERS
#include "PlotEntry.hh"
#include "PlottingTools.h"

int main(){

	std::vector<PlotEntry> histoStack;
	std::vector<PlotEntry> histoIndi;

	TH1F hTemplate("hTemplate", "insert_title;insert_xaxis;insert_yaxis", 100, 0, 100);
	std::string varToPlot = "fatJetA_softDropMass";
	std::string cutToApply = "ht>2000";
	double luminosity = 50.0; // fb^-1

	PlotEntry mH30_mSusy800 = PlotEntry("mH30_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
	mH30_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
	histoIndi.push_back(mH30_mSusy800);
	std::cout << mH30_mSusy800.GetNumberOfEventsBeforeCuts() << std::endl;
	std::cout << mH30_mSusy800.GetNumberOfEventsAfterCuts() << std::endl;
	std::cout << mH30_mSusy800.GetPlotEntryName() << std::endl;


	PlotEntry mH50_mSusy800 = PlotEntry("mH50_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
	mH50_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
	histoIndi.push_back(mH50_mSusy800);

	PlotEntry mH70_mSusy800 = PlotEntry("mH70_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
	mH70_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
	histoIndi.push_back(mH70_mSusy800);

	PlotEntry mH90_mSusy800 = PlotEntry("mH90_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
	mH90_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
	histoIndi.push_back(mH90_mSusy800);


	// LOOP THROUGH CUT SPACE, maybe need extension for ttbar and stuff
	// MAKE PLOT_ENTRY OBJECTS AND DO PLOT, SAVE



	// PLOTTER CLASS
	// add histoStack
	// add histoIndi
	// ratio box
	// save()

	TCanvas * c = new TCanvas("c","c");
	mH30_mSusy800.GetHistogram()->Draw();
	mH50_mSusy800.GetHistogram()->Draw("same");
	mH70_mSusy800.GetHistogram()->Draw("same");
	mH90_mSusy800.GetHistogram()->Draw("same");
	c->SaveAs("out.pdf");
	std::cout << std::endl;


	return 0;
}