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
#include "Plotter.hh"

// a script showing various features of the plotting classes I have developed
// TIP: one could easily put this stuff inside some kind of loop

int main(){

	// the following block of objects, when filled, will be passed to the plotter class
	std::vector<PlotEntry> histoIndi; // in here we will put PlotEntry objects for histograms that we want plotted individually (eg signal MC)
	std::vector<PlotEntry> histoStack; // in here we will put PlotEntry objects for histograms that we want plotted in stacks (eg background MC)
	std::vector<PlotEntry> histoEmpty; // this is an example of an empty version

	// the following block defines objects used in PlotEntry::PlotEntry and PlotEntry::AddInput
	TH1F hTemplate("hTemplate", "insert_title;insert_xaxis;insert_yaxis", 50, 0, 150); // a histogram template for a given PlotEntry object, it will use the same credentials
	std::string varToPlot = "fatJetA_softDropMass"; // a flatTree variable name for a PlotEntry object to draw (NB: 2d plots are not currently enabled)
	std::string cutToApply = "ht>2000"; // a cut for the PlotEntry object to apply when cutting 
	double luminosity = 50.0; // the luminosity, in fb^-1, for a PlotEntry object to use in weighting plots (for MC samples)

	// constructor:
	// the first argument defines the 'name' of the PlotEntry object (eg used in legends)
	// NB: for data the last argument can be omitted as an event weighting is not needed
	PlotEntry mH30_mSusy1200 = PlotEntry("mH30_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
	
	// adding an input
	// the first argument is the path to the flatTree
	// the last argument is the sample XS (pb), this should be omitted for data to avoid calculating a weighting
	// info about what is happening will be printed when the command is executed
	mH30_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
	
	// access to members of PlotEntry
	std::cout << "Examples of how to access PlotEntry members:" << std::endl;
	std::cout << "Get the histo and extract info (nbinsX) " << mH30_mSusy1200.GetHistogram()->GetNBinsX() <<std::endl; // can get the histogram then do normal histogram commands
	std::cout << "Number of events before cuts " << mH30_mSusy1200.GetNumberOfEventsBeforeCuts() << std::endl; // useful for cut flow tables
	std::cout << "Number of events after cuts " << mH30_mSusy1200.GetNumberOfEventsAfterCuts() << std::endl; // useful for cut flow tables
	std::cout << "Name of PlotEntry object " << mH30_mSusy1200.GetPlotEntryName() << std::endl;
	std::cout << std::endl;

	// when all entries added, put the PlotEntry object into the vector
	histoIndi.push_back(mH30_mSusy1200);

	// below: just doing the same again and adding to the histoIndi object
	PlotEntry mH50_mSusy1200 = PlotEntry("mH50_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
	mH50_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
	histoIndi.push_back(mH50_mSusy1200);

	PlotEntry mH70_mSusy1200 = PlotEntry("mH70_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
	mH70_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
	histoIndi.push_back(mH70_mSusy1200);

	PlotEntry mH90_mSusy1200 = PlotEntry("mH90_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
	mH90_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
	histoIndi.push_back(mH90_mSusy1200);

	// adding an input: further info
	// multiple inputs can be defined, the results are added together
	PlotEntry ttbar = PlotEntry("ttbar+jets", hTemplate, varToPlot.c_str(), luminosity);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700",cutToApply.c_str()), 831.76);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700",cutToApply.c_str()), 2.6665344485);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700",cutToApply.c_str()), 1.0980821984);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700",cutToApply.c_str()), 0.1987479092);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700",cutToApply.c_str()), 0.002368412585);
	histoStack.push_back(ttbar);

	PlotEntry ZJets = PlotEntry("Z+jets", hTemplate, varToPlot.c_str(), luminosity);
	ZJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/ZJetsToQQ_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);
	histoStack.push_back(ZJets);

	PlotEntry WJets = PlotEntry("W+jets", hTemplate, varToPlot.c_str(), luminosity);
	WJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/WJetsToQQ_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);
	histoStack.push_back(WJets);


	// okay, now how to handle these objects with the plotter class

	std::cout << "Example of how to plot individual and stacked histos" << std::endl;
	// EXAMPLE 1: individual plots and stacked plots
	// constructor
	Plotter plot_ex1 = Plotter(histoIndi, histoStack);
	

	// plot.AddLegend(0.6, 0.8, 0.6, 0.8);
	plot.AddLegend(0.6, 0.8, 0.6, 0.8, 0.8);
	// plot.AddLatex(333.333, "yippe");
	plot.AddLatex(333.333);
	plot.GetTStyle()->SetOptDate(1);
	plot.Save("out.pdf");



	// Plotter plot = Plotter(histoEmpty);

	// LOOP THROUGH CUT SPACE, maybe need extension for ttbar and stuff
	// MAKE PLOT_ENTRY OBJECTS AND DO PLOT, SAVE

	return 0;
}