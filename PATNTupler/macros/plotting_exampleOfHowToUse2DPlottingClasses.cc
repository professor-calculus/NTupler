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
#include "PlotEntry2D.hh"
#include "Plotter.hh"
#include "DoubleBTagWPs.h"

int main(){

    TH2F hTemplate = TH2F("hTemplate", ";dummy x-axis;dummy y-axis", 400, 0, 200, 400, 0, 200); // can't do the vector binning for 2d
    std::string varToPlot = "fatJetB_softDropMass:fatJetA_softDropMass"; // 2d object to plot: <Y_Axis Variable>:<X_AxisVariable> (it will warn you if this is not a legit string)
    double luminosity = 50.0;

    PlotEntry2D mH30mSusy1600 = PlotEntry2D("legendName", hTemplate, varToPlot.c_str(), luminosity); // nb: don't really use the legend name in 2d
    mH30mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", "", 0.060 * 0.85 * 0.85);
    std::cout << "mH30mSusy1600 num events after cuts: " << mH30mSusy1600.GetNumberOfEventsAfterCuts() << std::endl;
    std::cout << "mH30mSusy1600 num events after cuts stat error: " << mH30mSusy1600.GetNumberOfEventsAfterCutsStatError() << std::endl; // no longer bin by bin like in the 1d case

    PlotEntry2D mH90mSusy1600 = PlotEntry2D("legendName", hTemplate, varToPlot.c_str(), luminosity);
    mH90mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", "", 0.060 * 0.85 * 0.85);
    std::cout << "mH90mSusy1600 num events after cuts: " << mH90mSusy1600.GetNumberOfEventsAfterCuts() << std::endl;
    std::cout << "mH90mSusy1600 num events after cuts stat error: " << mH90mSusy1600.GetNumberOfEventsAfterCutsStatError() << std::endl;

    Plotter plot = Plotter({mH30mSusy1600, mH90mSusy1600}); // this draw them on the same plot (ie contributions are NOT summed together)
    plot.SetLogZ();
    plot.AddLatex(luminosity, "testing123");
    plot.Save2D("/opt/ppd/scratch/xap79297/test.pdf");

	return 0;
}