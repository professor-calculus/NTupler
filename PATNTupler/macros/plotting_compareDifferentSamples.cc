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
#include "TimeStamp.h"

// COMPARE VARIABLES FROM DIFFERENT DATASETS AS YOU LOOP THROUGH CUTS
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_05/testingMacros/testing_compareDifferentSamples/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params, each combination = new plot
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max (options--> "Off", "Loose", "Med", "Tight", "Max")
    std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Tight","Off","Med"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max (options--> "Off", "Loose", "Med", "Tight", "Max")
    // std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: plot histogram settings
    double luminosity = 50.0; // note that this value doesn't matter IF you normalise later

    std::string varToPlot = "fatJetA_softDropMass";
    // std::string varToPlot = "fatJetB_softDropMass";
    // std::string varToPlot = "fatJetA_doubleBtagDiscrim";
    // std::string varToPlot = "fatJetB_doubleBtagDiscrim";
    // std::string varToPlot = "ht";
    // std::string varToPlot = "slimJetB_p4.Pt()";
    // std::string varToPlot = "fatJetA_p4.Pt()";
    // std::string varToPlot = "fatJetB_p4.Pt()";
    
    TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);events/bin", 50, 0, 200);
    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_SoftDrop_Mass (GeV);events/bin", 50, 0, 200);
    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_doubleBtagDiscrim;a.u.", 50, -1.0, 1.0);
    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_doubleBtagDiscrim;events/bin", 50, -1.0, 1.0);
    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_doubleBtagDiscrim;a.u.", 50, -1.0, 1.0);
    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_doubleBtagDiscrim;events/bin", 50, -1.0, 1.0);
    // TH1F hTemplate("hTemplate", ";H_{T} (GeV);a.u.", 50, 0, 7000);
    // TH1F hTemplate("hTemplate", ";H_{T} (GeV);events/bin", 50, 0, 7000);
    // TH1F hTemplate("hTemplate", ";SecondaryPtAK4Jet p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // TH1F hTemplate("hTemplate", ";SecondaryPtAK4Jet p_{T} (GeV);events/bin", 50, 0.0, 2000.0);
    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet p_{T} (GeV);events/bin", 50, 0.0, 2000.0);
    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet p_{T} (GeV);events/bin", 50, 0.0, 2000.0);



    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compareDifferentSamples.cc %s/%s__plotting_compareDifferentSamples.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));


    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::vector<PlotEntry> plotEntryVec;
                    std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    // std::string hackOnExtraCuts = "";
                    // cutToApply += hackOnExtraCuts;


      
                    // FOUR: samples to use
                    // PlotEntry ttbarLO = PlotEntry("ttbar LO", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply.c_str()), 831.76);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 2.6665344485, false);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 1.0980821984, false);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.1987479092, false);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.002368412585, false);
                    // ttbarLO.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ttbarLO);

                    // PlotEntry ttbarNLO = PlotEntry("ttbar NLO", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarNLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
                    // ttbarNLO.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ttbarNLO);

                    // PlotEntry QCD = PlotEntry("QCD", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT700to1000/flatTree.root", cutToApply.c_str(), 6802);
                    // QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
                    // QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
                    // QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);
                    // QCD.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD);

                    PlotEntry ZJets = PlotEntry("Z+Jets", hTemplate, varToPlot.c_str(), luminosity);
                    ZJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/ZJetsToQQ_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);
                    ZJets.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(ZJets);

                    PlotEntry WJets = PlotEntry("W+Jets", hTemplate, varToPlot.c_str(), luminosity);
                    WJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/WJetsToQQ_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);
                    WJets.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(WJets);


                    // PlotEntry mH30_mSusy800 = PlotEntry("mH30_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // mH30_mSusy800.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH30_mSusy800);

                    // PlotEntry mH50_mSusy800 = PlotEntry("mH50_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH50_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // mH50_mSusy800.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH50_mSusy800);

                    // PlotEntry mH70_mSusy800 = PlotEntry("mH70_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // mH70_mSusy800.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH70_mSusy800);

                    // PlotEntry mH90_mSusy800 = PlotEntry("mH90_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // mH90_mSusy800.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH90_mSusy800);

                    // PlotEntry mH30_mSusy2000 = PlotEntry("mH30_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // mH30_mSusy2000.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH30_mSusy2000);

                    // PlotEntry mH50_mSusy2000 = PlotEntry("mH50_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH50_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // // mH50_mSusy2000.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH50_mSusy2000);

                    // PlotEntry mH70_mSusy2000 = PlotEntry("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // // mH70_mSusy2000.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH70_mSusy2000);

                    // PlotEntry mH90_mSusy2000 = PlotEntry("mH90_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // mH90_mSusy2000.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH90_mSusy2000);



                    // FIVE: plot aesthetics
                    Plotter plot = Plotter(plotEntryVec);
                    // plot.AddLegend(0.74, 0.88, 0.77, 0.87); // top right (thin 2)
                    plot.AddLegend(0.65, 0.88, 0.77, 0.87); // top right (wide 2)
                    // plot.AddLegend(0.50, 0.88, 0.77, 0.87); // top right (extra wide 2)
                    // plot.AddLegend(0.74, 0.88, 0.70, 0.87); // top right (thin 4)
                    // plot.AddLegend(0.65, 0.88, 0.70, 0.87); // top right (wide 4)
                    // plot.AddLegend(0.50, 0.88, 0.70, 0.87); // top right (extra wide 4)
                    // plot.AddLegend(0.20, 0.35, 0.77, 0.87); // top left (thin 2)
                    plot.AddLatex(); // if you HAVE normalised
                    // plot.AddLatex(luminosity); // if you HAVE NOT normalised
                    plot.SetErrors();



                    std::string saveName = varToPlot;
                    saveName += "__dbt" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "And" + cut2_ak8Dbt[iCut2][2] + cut2_ak8Dbt[iCut2][3];
                    saveName += Form("_ak8pt%d", cut3_ak8Pt[iCut3]);
                    if (cut4_ht[iCut4][1]==13000) saveName += Form("_ht%dplus",cut4_ht[iCut4][0]);
                    else saveName += Form("_ht%dto%d",cut4_ht[iCut4][0],cut4_ht[iCut4][1]);
                    saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    // saveName += "APPEND_SAVE_NAME"; // OPTIONAL: can manually append the automatic savename
                    // saveName = "MANUAL_SAVE_NAME"; // OPTIONAL: can manually overwrite the automatic savename
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