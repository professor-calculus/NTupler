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
#include "DoubleBTagWPs.h"

int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/compareDifferentSamples/ttbar/"; // where we are going to save the output plots (should include the samples name)

    // TWO: set of cut params, each combination = new plot
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Off"} }; // "Off", Loose", "Med", or "Tight" (2 elements in sub-vector, 1st for leading DBT, 2nd for secondary DBT)
    std::vector<int> cut3_ak8Pt = {-1};
    std::vector<std::vector<int>> cut4_ht = { {-1,13000} }; // these are HT bins, not just cuts
    std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt)

    // THREE: plot histogram settings
	// TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
	// std::string varToPlot = "fatJetA_softDropMass";
    
    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
    // std::string varToPlot = "fatJetB_softDropMass";

    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_doubleBtagDiscrim;a.u.", 25, -1.0, 1.0);
    // std::string varToPlot = "fatJetA_doubleBtagDiscrim";

    TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_doubleBtagDiscrim;a.u.", 25, -1.0, 1.0);
    std::string varToPlot = "fatJetB_doubleBtagDiscrim";

    // TH1F hTemplate("hTemplate", ";H_{T} (GeV);a.u.", 50, 0, 7000);
    // std::string varToPlot = "ht";

    // TH1F hTemplate("hTemplate", ";SecondaryPtAK4Jet p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // std::string varToPlot = "slimJetB_p4.Pt()";

    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // std::string varToPlot = "fatJetA_p4.Pt()";

    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // std::string varToPlot = "fatJetB_p4.Pt()";

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());

    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::vector<PlotEntry> plotEntryVec;
					std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);

                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////       
                    // FOUR: samples to use
                    PlotEntry ttbarLO = PlotEntry("ttbar LO", hTemplate, varToPlot.c_str(), 50.0); // note that the luminosity value doesn't matter as we will normalise later
                    ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply.c_str()), 831.76);
                    ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 2.6665344485, false);
                    ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 1.0980821984, false);
                    ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.1987479092, false);
                    ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.002368412585, false);
                    ttbarLO.NormalisePlot();
                    plotEntryVec.push_back(ttbarLO);

                    PlotEntry ttbarNLO = PlotEntry("ttbar NLO", hTemplate, varToPlot.c_str(), 50.0);
                    ttbarNLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_NLO/flatTree.root", Form("%s",cutToApply.c_str()), 831.76);
                    ttbarNLO.NormalisePlot();
                    plotEntryVec.push_back(ttbarNLO);
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    // FIVE: plot aesthetics
                    Plotter plot = Plotter(plotEntryVec);
                    plot.AddLegend(0.74, 0.88, 0.77, 0.87);
                    // plot.AddLegend(0.20, 0.35, 0.77, 0.87);
                    plot.AddLatex();
                    plot.SetErrors();
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////

                    std::string saveName = varToPlot;
                    saveName += "__dbt" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1] + Form("_ak8pt%d", cut3_ak8Pt[0]) + Form("_ht%dplus",cut4_ht[0][0]) + Form("_ak4pt%dn%d", cut5_ak4Pt[0][0], cut5_ak4Pt[0][1]);
                    std::string outputFile = outputDir + "/" + saveName + ".pdf";

                    plot.Save(outputFile.c_str());
                    plot.SetLogY();
                    outputFile = outputDir + "/" + saveName + "__logY.pdf";
                    plot.Save(outputFile.c_str());

                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states
	return 0;
}