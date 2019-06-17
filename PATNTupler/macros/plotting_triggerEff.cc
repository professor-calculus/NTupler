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
    const std::string outputDir = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/plots_2019_05_20/2017_94X/triggerStudiesV2/singleMuon2017/normalBinning_muonMaxPtLessThan200/"; // where we are going to save the output plots (should include the samples name, and any important features)

    // TWO: plot histogram settings
    // TH1D hTemplate("hTemplate", ";offline H_{T} (GeV);efficiency", 100, 0, 5000);
    std::vector<double> customBinning;
    // customBinning = {0, 1500, 10000};
    for(double binLowerEdge=  0.0; binLowerEdge< 1500.0; binLowerEdge+= 50.0) customBinning.push_back(binLowerEdge);
    // for(double binLowerEdge=  1200.0; binLowerEdge< 1500.0; binLowerEdge+= 50.0) customBinning.push_back(binLowerEdge);
    for(double binLowerEdge=  1500.0; binLowerEdge< 2000.0; binLowerEdge+= 100.0) customBinning.push_back(binLowerEdge);
    for(double binLowerEdge=  2000.0; binLowerEdge< 2800.0; binLowerEdge+= 200.0) customBinning.push_back(binLowerEdge);
    for(double binLowerEdge=  2800.0; binLowerEdge< 4000.0; binLowerEdge+= 400.0) customBinning.push_back(binLowerEdge);
    for(double binLowerEdge=  4000.0; binLowerEdge< 5000.1; binLowerEdge+= 500.0) customBinning.push_back(binLowerEdge);
    TH1D hTemplate("hTemplate", ";offline H_{T} (GeV);efficiency", customBinning.size()-1, &(customBinning)[0]);

    // THREE: luminosity (if you need it)
    // const double luminosity = 35.867; // 2016 DATASET
    const double luminosity = 41.370; // 2017 DATASET

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

    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // THREE: make effiencies
    //PlotEntry plot_ratio1 = PlotEntry("Run2016", hTemplate, "ht");
    //plot_ratio1.AddInputEfficiency("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_singleMu_2019_01_08/data/SingleMuon_Run2016TOTAL/flatTree.root", "muon_maxPt < 200.0 && (fatJetA_p4.Pt() != fatJetB_p4.Pt())", "trgDecision==1"); // flatTree, commonCut, numeratorCut
    //plotVec.push_back(plot_ratio1);

    // PlotEntry plot_ratio2 = PlotEntry("Run2016H", hTemplate, "ht");
    // plot_ratio2.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_04_11/data/SingleMuon_Run2016H-03Feb2017_bothVersionsCombined/flatTree.root", "", "trgDecision==1");
    // plotVec.push_back(plot_ratio2);

    // PlotEntry plot_ratio3 = PlotEntry("TTJets", hTemplate, "ht");
    // plot_ratio3.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_04_11/mc/TTJets_NLO/flatTree.root", "", "trgDecision==1"); // flatTree, commonCut, numeratorCut
    // plotVec.push_back(plot_ratio3);

    // PlotEntry plot_ratio4 = PlotEntry("ZJets", hTemplate, "ht");
    // plot_ratio4.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_04_11/mc/ZJets_HT600toInf/flatTree.root", "", "trgDecision==1"); // flatTree, commonCut, numeratorCut
    // plotVec.push_back(plot_ratio4);

    // PlotEntry plot_ratio5 = PlotEntry("WJets", hTemplate, "ht");
    // plot_ratio5.AddInputEfficiency("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_04_11/mc/WJets_HT600toInf/flatTree.root", "", "trgDecision==1"); // flatTree, commonCut, numeratorCut
    // plotVec.push_back(plot_ratio5);

    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.

    PlotEntry plot_ratio6 = PlotEntry("Run2017", hTemplate, "ht");
    plot_ratio6.AddInputEfficiency("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_singleMu17_2019_01_08/data/SingleMuon_Run2017TOTAL/flatTree.root", "muon_maxPt < 200.0 && (fatJetA_p4.Pt() != fatJetB_p4.Pt())", "trgDecision==1"); // flatTree, commonCut, numeratorCut
    plotVec.push_back(plot_ratio6);

    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.

    // FOUR: plot aesthetics
    Plotter plot = Plotter(plotVec);
    // plot.AddLegend(0.70, 0.88, 0.25, 0.40); // top right (extra wide 2)
    // plot.AddLatex(); // FOR SIMULATION
    plot.AddLatex(luminosity, "#it{Preliminary}"); // FOR DATA

    std::string saveName = "trigEff";
    std::string outputFile = outputDir + "/" + saveName + ".pdf";
    plot.Save(outputFile.c_str());
    return 0;
}
