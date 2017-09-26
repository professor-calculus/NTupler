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
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"

// PLOTS EFFICIENCY AS A FUNCTION OF OFFLINE HT
int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_09_25/triggerEff/testing/"; // where we are going to save the output plots (should include the samples name, and any important features)

    // TWO: plot histogram settings
    TH1D hTemplate("hTemplate", ";offline H_{T} (GeV);efficiency", 100, 0, 5000);
    // std::vector<double> customBinning = {-1.0, -0.6, 0.2, 0.3, 0.6, 0.8, 1.0};
    // TH1D hTemplate("hTemplate", ";offline H_{T} (GeV);efficiency", customBinning.size()-1, &(customBinning)[0]);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_triggerEff.cc %s/%s__plotting_triggerEff.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<PlotEntry> plotVec;


    // THREE: make effiencies
    PlotEntry plot_ratio1 = PlotEntry("Run2016G", hTemplate, "ht");
    plot_ratio1.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_25_CMSSW_8_0_29_dbtV4_wMu/data/SingleMuon_Run2016G-03Feb2017-v1/flatTree.root", "muon_maxPt < 100", "trgDecision==1");
    plotVec.push_back(plot_ratio1);

    PlotEntry plot_ratio2 = PlotEntry("Run2016H", hTemplate, "ht");
    plot_ratio2.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_25_CMSSW_8_0_29_dbtV4_wMu/data/SingleMuon_Run2016H-03Feb2017_ver2-v1/flatTree.root", "muon_maxPt < 100", "trgDecision==1");
    plotVec.push_back(plot_ratio2);


    // FOUR: plot aesthetics
    Plotter plot = Plotter(plotVec); // individual plots
    plot.AddLegend(0.45, 0.88, 0.70, 0.87); // top right (extra wide 2)
    plot.AddLatex("Data 2016 Period H");


    std::string saveName = "trigEff";
    std::string outputFile = outputDir + "/" + saveName + ".pdf";
    plot.Save(outputFile.c_str());
    return 0;
}