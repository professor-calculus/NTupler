//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <fstream>
#include <sys/stat.h>

//ROOT HEADERS
#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TLegend.h>

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
	TH1D hTemplate("hTemplate", "insert_title;insert_xaxis;insert_yaxis", 50, 0, 150); // a histogram template for a given PlotEntry object, it will use the same credentials
	std::string varToPlot = "fatJetA_softDropMass"; // a flatTree variable name for a PlotEntry object to draw (NB: 2d plots are not currently enabled)
	std::string cutToApply = "fatJetA_doubleBtagDiscrim>0.9 && fatJetB_doubleBtagDiscrim>0.6 && fatJetA_p4.Pt()>300.0 && fatJetB_p4.Pt()>300.0 && ht>=1500.0 && ht<2500.0 && slimJetA_p4.Pt()>250.0 && slimJetB_p4.Pt()>250.0"; // a cut for the PlotEntry object to apply when cutting 
	double luminosity = 50.0; // the luminosity, in fb^-1, for a PlotEntry object to use in weighting plots (for MC samples)

	// constructor:
	// the first argument defines the 'name' of the PlotEntry object (eg used in legends)
	// NB: for data the last argument can be omitted as an event weighting is not needed
	PlotEntry mH30_mSusy2000 = PlotEntry("mH30_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
	
	// adding an input
	// the first argument is the path to the flatTree
	// the last argument is the sample XS (pb), this should be omitted for data to avoid calculating a weighting
	// info about what is happening will be printed when the command is executed
	mH30_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
	
	// access to members of PlotEntry
	std::cout << "Examples of how to access PlotEntry members:" << std::endl;
	std::cout << "Get the histo and extract info (nbinsX) " << mH30_mSusy2000.GetHistogram()->GetNbinsX() <<std::endl; // can get the histogram then do normal histogram commands
	std::cout << "Get the xaxis title " << mH30_mSusy2000.GetHistogram()->GetXaxis()->GetTitle() << std::endl; // IMPORTANT - this includes changing the histogram!
	std::cout << "Number of events before cuts " << mH30_mSusy2000.GetNumberOfEventsBeforeCuts() << std::endl; // useful for cut flow tables
	std::cout << "Number of events after cuts " << mH30_mSusy2000.GetNumberOfEventsAfterCuts() << std::endl; // useful for cut flow tables
	std::cout << "Name of PlotEntry object " << mH30_mSusy2000.GetPlotEntryName() << std::endl;
	std::cout << std::endl;

	// when all entries added, put the PlotEntry object into the vector
	histoIndi.push_back(mH30_mSusy2000);

	// below: just doing the same again and adding to the histoIndi object
	PlotEntry mH50_mSusy2000 = PlotEntry("mH50_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
	mH50_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
	histoIndi.push_back(mH50_mSusy2000);

	PlotEntry mH70_mSusy2000 = PlotEntry("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
	mH70_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
	histoIndi.push_back(mH70_mSusy2000);

	PlotEntry mH90_mSusy2000 = PlotEntry("mH90_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
	mH90_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
	histoIndi.push_back(mH90_mSusy2000);

	// adding an input: further info
	// multiple inputs can be defined, the results are added together
	// NOTE the 'false' argument stops the AddInput incrementing PlotEntry::numberOfEventsBeforeCuts variable (used to prevent double counting when stitching samples together)
	PlotEntry ttbar = PlotEntry("ttbar+jets", hTemplate, varToPlot.c_str(), luminosity);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply.c_str()), 831.76);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 2.6665344485, false);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 1.0980821984, false);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.1987479092, false);
	ttbar.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.002368412585, false);
	std::cout << "num_ttbar before cuts " << ttbar.GetNumberOfEventsBeforeCuts() << std::endl;
	std::cout << "num_ttbar after cuts " << ttbar.GetNumberOfEventsAfterCuts() << std::endl;
	std::cout << std::endl;
	histoStack.push_back(ttbar);

	PlotEntry ZJets = PlotEntry("Z+jets", hTemplate, varToPlot.c_str(), luminosity);
	ZJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/ZJetsToQQ_HT600toInf/flatTree.root", cutToApply.c_str(), 52.79);
	histoStack.push_back(ZJets);

	PlotEntry WJets = PlotEntry("W+jets", hTemplate, varToPlot.c_str(), luminosity);
	WJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/WJetsToQQ_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);
	histoStack.push_back(WJets);

	// won't include in plots, but just to double check the event counting
	PlotEntry WWJets = PlotEntry("WW+jets", hTemplate, varToPlot.c_str(), luminosity);
	WWJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/WWTo4Q/flatTree.root", cutToApply.c_str(), 51.723);
	WWJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/WWTo1L1Nu2Q/flatTree.root", cutToApply.c_str(), 49.997); // note that no longer providing the 'false' argument as we want PlotEntry::numberOfEventsBeforeCuts to be summed
	std::cout << "num_WW before cuts " << WWJets.GetNumberOfEventsBeforeCuts() << std::endl;
	std::cout << "num_WW after cuts " << WWJets.GetNumberOfEventsAfterCuts() << std::endl;
	std::cout << std::endl;	

	// okay, now how to handle these objects with the plotter class

	std::cout << "Example of how to plot individual and stacked histos" << std::endl;
	// EXAMPLE 1: individual plots and stacked plots
	// constructor
	Plotter plot_ex1 = Plotter(histoIndi, histoStack);
	// NB: can also add legend by passing it a TLegend* template as the argument
	// x1, x2, y1, y2
	plot_ex1.AddLegend(0.6, 0.8, 0.6, 0.8, 0.05); // the last value is the textSize, if no arg given 0.04 is used
	plot_ex1.AddLatex(luminosity, "#it{AdditionalTag}"); // the last value is the tag after 'cms', if no arg given simulation WIP is used
	plot_ex1.GetTStyle()->SetOptDate(1); // you can ask for the TStyle object and change things if desired
	plot_ex1.Save("out_ex1.pdf"); // to save the plot

	// COPY TEST - expect the same result 
	std::cout << "COPY TEST" << std::endl;
	Plotter plot_ex1_copy = Plotter(histoIndi, histoStack);
	// NB: can also add legend by passing it a TLegend* template as the argument
	// x1, x2, y1, y2
	plot_ex1_copy.AddLegend(0.6, 0.8, 0.6, 0.8, 0.05); // the last value is the textSize, if no arg given 0.04 is used
	plot_ex1_copy.AddLatex(luminosity, "#it{AdditionalTag}"); // the last value is the tag after 'cms', if no arg given simulation WIP is used
	plot_ex1_copy.GetTStyle()->SetOptDate(1); // you can ask for the TStyle object and change things if desired
	plot_ex1_copy.Save("out_ex1_copy.pdf"); // to save the plot

	std::cout << "Example of how to plot individual histos only" << std::endl;
	// EXAMPLE 2: individual plots only
	// constructor, only using one argument 
	Plotter plot_ex2 = Plotter(histoIndi);
	TLegend * legDummy = new TLegend(0.6, 0.6, 0.8, 0.8); //NB, follows x1,y1,x2,y2 (different convention to me!)
	plot_ex2.AddLegend(legDummy); // use a legend template
	plot_ex2.AddLatex(luminosity); // no second arg given, will use default
	plot_ex2.Save("out_ex2.pdf"); // to save the plot

	std::cout << "Example of how to plot stacked histos only" << std::endl;
	// EXAMPLE 3: stacked plots only
	// constructor, have to give an empty first arguement
	Plotter plot_ex3 = Plotter(histoEmpty, histoStack);
	TLegend * legDummy2 = new TLegend(0.1, 0.6, 0.8, 0.8); // note the use of a different legend, this is because they are passed as pointers!
	plot_ex3.AddLegend(legDummy2);
	plot_ex3.AddLatex(luminosity, "#it{AdditionalTag}");
	plot_ex3.Save("out_ex3.pdf"); // to save the plot

	// NB: will get warning/error messages if you only supply empty vector inputs (it will not seg fault, he says)

	return 0;
}