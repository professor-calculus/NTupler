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
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_06_13/ttbarShape/loWithoutTheInclusiveBitNoLheCut_sample_ht1500to2500_secondaryFatJet/"; // where we are going to save the output plots (should include the samples name + binning maybe)


    // TWO: set of cut params for the plot (will loop through the combinations)
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Off"} }; // "Off", Loose", "Med", or "Tight" (2 elements in sub-vector, 1st for leading DBT, 2nd for secondary DBT)
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Med"} }; // "Off", Loose", "Med", or "Tight" (2 elements in sub-vector, 1st for leading DBT, 2nd for secondary DBT)
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,2500} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // THREE: plot histogram settings
	// TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
	// std::string varToPlot = "fatJetA_softDropMass";
    
    TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
    std::string varToPlot = "fatJetB_softDropMass";

    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_doubleBtagDiscrim;a.u.", 50, -1.0, 1.0);
    // std::string varToPlot = "fatJetA_doubleBtagDiscrim";

    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_doubleBtagDiscrim;a.u.", 50, -1.0, 1.0);
    // std::string varToPlot = "fatJetB_doubleBtagDiscrim";

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

    std::vector<PlotEntry> plotEntryVec;
    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

					std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);

                    // for a given cut criteria creates the legend name
                    std::string legendName = "";
                    if (cut2_ak8Dbt.size()>1) legendName += "DBT " + cut2_ak8Dbt[iCut2][0] + ":" + cut2_ak8Dbt[iCut2][1];
                    if (cut3_ak8Pt.size()>1){
                        if (legendName.size() == 0) legendName += Form("AK8PT %d", cut3_ak8Pt[iCut3]);
                        else legendName += Form("_AK8PT %d", cut3_ak8Pt[iCut3]);
                    }
                    if (cut4_ht.size()>1){
                        if (legendName.size() == 0){
                        if (cut4_ht[iCut4][1]==13000) legendName += Form("HT %d+", cut4_ht[iCut4][0]);
                        else legendName += Form("HT %d-%d", cut4_ht[iCut4][0], cut4_ht[iCut4][1]);
                    }
                    else if (cut4_ht[iCut4][1]==13000) legendName += Form("_HT %d+", cut4_ht[iCut4][0]);
                        else legendName += Form("_HT %d-%d", cut4_ht[iCut4][0], cut4_ht[iCut4][1]);
                    }
                    if (cut5_ak4Pt.size()>1){
                        if (legendName.size() == 0) legendName += Form("AK4PT %d:%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                        else legendName += Form("_AK4PT %d:%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    }                    
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    // FOUR: sample info
                    double luminosity = 50.0; // note that this value doesn't matter IF you normalise later
                    PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity); // note that the luminosity value doesn't matter as we will normalise later

                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply.c_str()), 831.76);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 2.6665344485, false);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 1.0980821984, false);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.1987479092, false);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.002368412585, false);

                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", cutToApply.c_str(), 2.6665344485, false);
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", cutToApply.c_str(), 1.0980821984, false);
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", cutToApply.c_str(), 0.1987479092, false);
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", cutToApply.c_str(), 0.002368412585, false);

                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);

                    plotElement.NormalisePlot();
                    plotEntryVec.push_back(plotElement);
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states

    // DOING IN MANUALLY (would comment out the loops above)
    // std::string cutToApply1 = "ht>=2500 && ht<3500";
    // PlotEntry plotElement1 = PlotEntry("Pre-Sel", hTemplate, varToPlot.c_str(), 50);
    // plotElement1.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", cutToApply1.c_str(), 2.6665344485, false);
    // plotElement1.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", cutToApply1.c_str(), 1.0980821984, false);
    // plotElement1.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", cutToApply1.c_str(), 0.1987479092, false);
    // plotElement1.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", cutToApply1.c_str(), 0.002368412585, false);
    // plotElement1.NormalisePlot();
    // plotEntryVec.push_back(plotElement1);
    // std::string cutToApply2 = "fatJetA_doubleBtagDiscrim>0.9 && fatJetB_doubleBtagDiscrim>0.6 && ht>=2500 && ht<3500";
    // PlotEntry plotElement2 = PlotEntry("+ DBT Tight:Med", hTemplate, varToPlot.c_str(), 50);
    // plotElement2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", cutToApply2.c_str(), 2.6665344485, false);
    // plotElement2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", cutToApply2.c_str(), 1.0980821984, false);
    // plotElement2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", cutToApply2.c_str(), 0.1987479092, false);
    // plotElement2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", cutToApply2.c_str(), 0.002368412585, false);
    // plotElement2.NormalisePlot();
    // plotEntryVec.push_back(plotElement2);
    // std::string cutToApply3 = "fatJetA_doubleBtagDiscrim>0.9 && fatJetB_doubleBtagDiscrim>0.6 && fatJetA_p4.Pt()>300 && fatJetB_p4.Pt()>300 && ht>=2500 && ht<3500";
    // PlotEntry plotElement3 = PlotEntry("+ AK8PT 300", hTemplate, varToPlot.c_str(), 50);
    // plotElement3.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", cutToApply3.c_str(), 2.6665344485, false);
    // plotElement3.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", cutToApply3.c_str(), 1.0980821984, false);
    // plotElement3.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", cutToApply3.c_str(), 0.1987479092, false);
    // plotElement3.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", cutToApply3.c_str(), 0.002368412585, false);
    // plotElement3.NormalisePlot();
    // plotEntryVec.push_back(plotElement3);
    // std::string cutToApply4 = "fatJetA_doubleBtagDiscrim>0.9 && fatJetB_doubleBtagDiscrim>0.6 && fatJetA_p4.Pt()>300 && fatJetB_p4.Pt()>300 && ht>=2500 && ht<3500 && slimJetA_p4.Pt()>250 && slimJetB_p4.Pt()>250";
    // PlotEntry plotElement4 = PlotEntry("+ AK4PT 250:250", hTemplate, varToPlot.c_str(), 50);
    // plotElement4.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", cutToApply4.c_str(), 2.6665344485, false);
    // plotElement4.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", cutToApply4.c_str(), 1.0980821984, false);
    // plotElement4.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", cutToApply4.c_str(), 0.1987479092, false);
    // plotElement4.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", cutToApply4.c_str(), 0.002368412585, false);
    // plotElement4.NormalisePlot();
    // plotEntryVec.push_back(plotElement4);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // FIVE: plot aesthetics
    Plotter plot = Plotter(plotEntryVec);
    // plot.AddLegend(0.74, 0.88, 0.77, 0.87); // top right (thin 2)
    // plot.AddLegend(0.68, 0.88, 0.77, 0.87); // top right (wide 2)
    // plot.AddLegend(0.65, 0.88, 0.70, 0.87); // top right (wide 4)
    plot.AddLegend(0.65, 0.88, 0.62, 0.87); // top right (wide 6)
    // plot.AddLegend(0.50, 0.88, 0.70, 0.87); // top right (extra wide 4)
    // plot.AddLegend(0.20, 0.35, 0.77, 0.87); // top left (2)
    // plot.AddLegend(0.20, 0.35, 0.25, 0.43); // bottom left
    plot.AddLatex();
    plot.SetErrors();
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    std::string saveName = varToPlot;
    if (cut2_ak8Dbt.size()>1) saveName += "__dbtVary";
    else saveName += "__dbt" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1];
    if (cut3_ak8Pt.size()>1) saveName += "_ak8ptVary";
    else saveName += Form("_ak8pt%d", cut3_ak8Pt[0]);
    if (cut4_ht.size()>1) saveName += "_htVary";
    else if (cut4_ht[0][1]==13000) saveName += Form("_ht%dplus",cut4_ht[0][0]);
        else saveName += Form("_ht%dto%d",cut4_ht[0][0],cut4_ht[0][1]);
    if (cut5_ak4Pt.size()>1) saveName += "_ak4ptVary";
    else saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0][0], cut5_ak4Pt[0][1]);
    std::string outputFile = outputDir + "/" + saveName + ".pdf";

	plot.Save(outputFile.c_str());
    plot.SetLogY();
    outputFile = outputDir + "/" + saveName + "__logY.pdf";
    plot.Save(outputFile.c_str());

	return 0;
}