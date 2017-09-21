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
#include "MacrosOnCondor.h"

// COMPARE VARIABLES FROM DIFFERENT DATASETS AS YOU LOOP THROUGH CUTS

int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_08_14/1dDBT/dataValidation/QCD_1000/fullCutsHt1500to2500/fatJetB_dbtLessthan-0p2/signal_contamination/mH30_mSusy1200_mJ20to30/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params, each combination = new plot
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Med2","Max","Med2","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,2500} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: plot histogram settings
    double luminosity = 50.0; // note that this value doesn't matter IF you normalise later

    // std::string varToPlot = "fatJetA_softDropMass";
    // std::string varToPlot = "fatJetB_softDropMass";
    std::string varToPlot = "fatJetA_doubleBtagDiscrim";
    // std::string varToPlot = "fatJetB_doubleBtagDiscrim";
    // std::string varToPlot = "ht";
    // std::string varToPlot = "slimJetB_p4.Pt()";
    // std::string varToPlot = "fatJetA_p4.Pt()";
    // std::string varToPlot = "fatJetB_p4.Pt()";


    // TH1D hTemplate("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
    // TH1D hTemplate("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);events/bin", 50, 0, 200);
    // TH1D hTemplate("hTemplate", ";fatJetB_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
    // TH1D hTemplate("hTemplate", ";fatJetB_SoftDrop_Mass (GeV);events/bin", 50, 0, 200);
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", 50, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;events/bin", 50, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";fatJetB_doubleBtagDiscrim;a.u.", 50, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";fatJetB_doubleBtagDiscrim;events/bin", 50, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";H_{T} (GeV);a.u.", 50, 0, 7000);
    // TH1D hTemplate("hTemplate", ";H_{T} (GeV);events/bin", 6, 1500, 4500);
    // TH1D hTemplate("hTemplate", ";SecondaryPtAK4Jet p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // TH1D hTemplate("hTemplate", ";SecondaryPtAK4Jet p_{T} (GeV);events/bin", 50, 0.0, 2000.0);
    // TH1D hTemplate("hTemplate", ";fatJetA p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // TH1D hTemplate("hTemplate", ";fatJetA p_{T} (GeV);events/bin", 50, 0.0, 2000.0);
    // TH1D hTemplate("hTemplate", ";fatJetB p_{T} (GeV);a.u.", 50, 0.0, 2000.0);
    // TH1D hTemplate("hTemplate", ";fatJetB p_{T} (GeV);events/bin", 50, 0.0, 2000.0);

    // std::vector<double> customBinning = {-1.0, -0.6, 0.2, 0.3, 0.6, 0.8, 1.0};
    // std::vector<double> customBinning = {-1.0, 0.3, 0.6, 0.8, 1.0};
    // std::vector<double> customBinning = {-1.0, -0.4, 0.3, 0.6, 0.8, 1.0};
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", 100, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", 50, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;events/bin", 50, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", 25, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", 20, -1.0, 1.0);
    TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", 10, -1.0, 1.0);
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", customBinning.size()-1, &(customBinning)[0]);



    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    if (argc != 2){
        std::cout << "Haven't provided valid argument, the format should be:" << std::endl;
        std::cout << argv[0] << " <runInstructionString>" << std::endl;
        std::cout << "runInstructionString = 'local' or 'batch'" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }
    
    std::string runInstructionString(argv[1]);
    
    if (runInstructionString == "local"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compareDifferentSamples.cc %s/%s__plotting_compareDifferentSamples.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compareDifferentSamples.cc %s/%s__plotting_compareDifferentSamples.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_compareDifferentSamples", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
        return 0;
    }
    else if (runInstructionString == "batchRUN"){
        outputDir = ".";
    }
    else{
        std::cout << "Haven't provided valid argument, the format should be:" << std::endl;
        std::cout << argv[0] << " <runInstructionString>" << std::endl;
        std::cout << "runInstructionString = 'local' or 'batch'" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::vector<PlotEntry> plotEntryVec;
                    std::vector<PlotEntry> plotEntryVec2;
                    // std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    std::string cutToApply = Form("fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    cutToApply += " && fatJetB_doubleBtagDiscrim < -0.2 ";
                    cutToApply += " && fatJetA_softDropMass > 20 && fatJetA_softDropMass < 30 ";


      
                    // FOUR: samples to use

                    PlotEntry ZJets = PlotEntry("Z+Jets", hTemplate, varToPlot.c_str(), luminosity);
                    ZJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/ZJets_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);
                    // ZJets.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(ZJets);

                    PlotEntry WJets = PlotEntry("W+Jets", hTemplate, varToPlot.c_str(), luminosity);
                    WJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/WJets_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);
                    // WJets.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(WJets);


                    PlotEntry ttbarNLO = PlotEntry("TTJets", hTemplate, varToPlot.c_str(), luminosity);
                    ttbarNLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
                    // ttbarNLO.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(ttbarNLO);

                    // cutToApply = "fatJetA_softDropMass < 10";
                    // PlotEntry ttbarNLO_A = PlotEntry("TTJets_fatJetA", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarNLO_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
                    // ttbarNLO_A.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ttbarNLO_A);

                    // cutToApply = "fatJetA_softDropMass > 10 && fatJetA_softDropMass < 20";
                    // PlotEntry ttbarNLO_B = PlotEntry("TTJets_fatJetA", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarNLO_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
                    // ttbarNLO_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ttbarNLO_B);

                    // cutToApply = "fatJetA_softDropMass > 20 && fatJetA_softDropMass < 30";
                    // PlotEntry ttbarNLO_C = PlotEntry("TTJets_fatJetA", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarNLO_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
                    // ttbarNLO_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ttbarNLO_C);

                    // cutToApply = "fatJetA_softDropMass > 30 && fatJetA_softDropMass < 40";
                    // PlotEntry ttbarNLO_D = PlotEntry("TTJets_fatJetA", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarNLO_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
                    // ttbarNLO_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ttbarNLO_D);



                    PlotEntry QCD = PlotEntry("QCD", hTemplate, varToPlot.c_str(), luminosity);
                    QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
                    QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
                    QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);
                    // QCD.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(QCD);

                    // std::string cutToApplyB = "300 < fatJetA_p4.Pt() && fatJetA_p4.Pt() < 325 && " + cutToApply;
                    // PlotEntry QCD_B = PlotEntry("300 < p_{T} < 325 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // std::string cutToApplyB = "0 <= fatJetA_softDropMass && fatJetA_softDropMass < 5 && " + cutToApply;
                    // PlotEntry QCD_B = PlotEntry("0 < Mass < 5 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApplyB.c_str(), 1206);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApplyB.c_str(), 120.4);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApplyB.c_str(), 25.25);
                    // QCD_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_B);

                    // // std::string cutToApplyC = "400 < fatJetA_p4.Pt() && fatJetA_p4.Pt() < 425 && " + cutToApply;
                    // // PlotEntry QCD_C = PlotEntry("400 < p_{T} < 425 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // std::string cutToApplyC = "40 < fatJetA_softDropMass && fatJetA_softDropMass < 50 && " + cutToApply;
                    // PlotEntry QCD_C = PlotEntry("40 < Mass < 50 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApplyC.c_str(), 1206);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApplyC.c_str(), 120.4);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApplyC.c_str(), 25.25);
                    // QCD_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_C);

                    // // std::string cutToApplyD = "500 < fatJetA_p4.Pt() && fatJetA_p4.Pt() < 525 && " + cutToApply;
                    // // PlotEntry QCD_D = PlotEntry("500 < p_{T} < 525 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // std::string cutToApplyD = "80 < fatJetA_softDropMass && fatJetA_softDropMass < 90 && " + cutToApply;
                    // PlotEntry QCD_D = PlotEntry("80 < Mass < 90 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApplyD.c_str(), 1206);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApplyD.c_str(), 120.4);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApplyD.c_str(), 25.25);
                    // QCD_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_D);

                    // // // std::string cutToApplyE = "900 < fatJetA_p4.Pt() && fatJetA_p4.Pt() < 950 && " + cutToApply;
                    // // // PlotEntry QCD_E = PlotEntry("900 < p_{T} < 950 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // std::string cutToApplyE = "120 < fatJetA_softDropMass && fatJetA_softDropMass < 130 && " + cutToApply;
                    // PlotEntry QCD_E = PlotEntry("120 < Mass < 130 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_E.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApplyE.c_str(), 1206);
                    // QCD_E.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApplyE.c_str(), 120.4);
                    // QCD_E.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApplyE.c_str(), 25.25);
                    // QCD_E.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_E);






                    // std::string cutToApplyB = "15 < fatJetA_softDropMass && fatJetA_softDropMass < 20 && " + cutToApply;
                    // PlotEntry QCD_B = PlotEntry("15 < Mass < 20 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApplyB.c_str(), 1206);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApplyB.c_str(), 120.4);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApplyB.c_str(), 25.25);
                    // QCD_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_B);

                    // std::string cutToApplyB2 = "25 < fatJetA_softDropMass && fatJetA_softDropMass < 30 && " + cutToApply;
                    // PlotEntry QCD_B2 = PlotEntry("25 < Mass < 30 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_B2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApplyB2.c_str(), 1206);
                    // QCD_B2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApplyB2.c_str(), 120.4);
                    // QCD_B2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApplyB2.c_str(), 25.25);
                    // QCD_B2.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_B2);

                    // std::string cutToApplyC = "35 < fatJetA_softDropMass && fatJetA_softDropMass < 40 && " + cutToApply;
                    // PlotEntry QCD_C = PlotEntry("35 < Mass < 40 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApplyC.c_str(), 1206);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApplyC.c_str(), 120.4);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApplyC.c_str(), 25.25);
                    // // QCD_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_C);

                    // std::string cutToApplyD = "45 < fatJetA_softDropMass && fatJetA_softDropMass < 50 && " + cutToApply;
                    // PlotEntry QCD_D = PlotEntry("45 < Mass < 50 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApplyD.c_str(), 1206);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApplyD.c_str(), 120.4);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApplyD.c_str(), 25.25);
                    // // QCD_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_D);




// TTBAR

                    // std::string cutToApplyB = "0 <= fatJetA_softDropMass && fatJetA_softDropMass < 5 && " + cutToApply;
                    // PlotEntry QCD_B = PlotEntry("0 <= Mass < 5 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApplyB.c_str(), 831.76);
                    // QCD_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_B);

                    // std::string cutToApplyC2 = "40 < fatJetA_softDropMass && fatJetA_softDropMass < 50 && " + cutToApply;
                    // PlotEntry QCD_C2 = PlotEntry("40 < Mass < 50 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_C2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApplyC2.c_str(), 831.76);
                    // // QCD_C2.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_C2);

                    // std::string cutToApplyD = "80 < fatJetA_softDropMass && fatJetA_softDropMass < 90 && " + cutToApply;
                    // PlotEntry QCD_D = PlotEntry("80 < Mass < 90 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApplyD.c_str(), 831.76);
                    // QCD_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_D);

                    // std::string cutToApplyE = "120 < fatJetA_softDropMass && fatJetA_softDropMass < 130 && " + cutToApply;
                    // PlotEntry QCD_E = PlotEntry("120 < Mass < 130 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_E.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApplyE.c_str(), 831.76);
                    // QCD_E.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_E);












                    // std::string cutToApplyB = "300 < fatJetA_p4.Pt() && fatJetA_p4.Pt() < 325 && " + cutToApply;
                    // PlotEntry QCD_B = PlotEntry("300 < p_{T} < 325 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // std::string cutToApplyB = "0 <= fatJetA_softDropMass && fatJetA_softDropMass < 5 && " + cutToApply;
                    // PlotEntry QCD_B = PlotEntry("0 <= Mass < 5 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApplyB.c_str(), 6.466*0.85*0.85);
                    // QCD_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_B);

                    // std::string cutToApplyC = "400 < fatJetA_p4.Pt() && fatJetA_p4.Pt() < 425 && " + cutToApply;
                    // PlotEntry QCD_C = PlotEntry("400 < p_{T} < 425 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // std::string cutToApplyC = "40 < fatJetA_softDropMass && fatJetA_softDropMass < 50 && " + cutToApply;
                    // PlotEntry QCD_C = PlotEntry("40 < Mass < 50 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApplyC.c_str(), 6.466*0.85*0.85);
                    // QCD_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_C);

                    // std::string cutToApplyD = "500 < fatJetA_p4.Pt() && fatJetA_p4.Pt() < 525 && " + cutToApply;
                    // PlotEntry QCD_D = PlotEntry("500 < p_{T} < 525 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // std::string cutToApplyD = "40 < fatJetA_softDropMass && fatJetA_softDropMass < 60 && " + cutToApply;
                    // PlotEntry QCD_D = PlotEntry("40 < Mass < 60 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApplyD.c_str(), 6.466*0.85*0.85);
                    // QCD_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_D);

                    // // std::string cutToApplyE = "900 < fatJetA_p4.Pt() && fatJetA_p4.Pt() < 950 && " + cutToApply;
                    // // PlotEntry QCD_E = PlotEntry("900 < p_{T} < 950 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // std::string cutToApplyE = "120 < fatJetA_softDropMass && fatJetA_softDropMass < 130 && " + cutToApply;
                    // PlotEntry QCD_E = PlotEntry("120 < Mass < 130 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_E.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApplyE.c_str(), 6.466*0.85*0.85);
                    // QCD_E.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_E);











                    // PlotEntry mH30_mSusy800 = PlotEntry("mH30_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // mH30_mSusy800.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec2.push_back(mH30_mSusy800);

                    // // PlotEntry mH50_mSusy800 = PlotEntry("mH50_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // // mH50_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // // mH50_mSusy800.NormalisePlot(); // OPTIONAL: toggle on or off
                    // // plotEntryVec.push_back(mH50_mSusy800);

                    // // PlotEntry mH70_mSusy800 = PlotEntry("mH70_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // // mH70_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // // mH70_mSusy800.NormalisePlot(); // OPTIONAL: toggle on or off
                    // // plotEntryVec.push_back(mH70_mSusy800);

                    // PlotEntry mH90_mSusy800 = PlotEntry("mH90_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // mH90_mSusy800.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec2.push_back(mH90_mSusy800);


                    PlotEntry mH30_mSusy1200 = PlotEntry("mH30_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    mH30_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                    // // mH30_mSusy1200.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec2.push_back(mH30_mSusy1200);

                    // PlotEntry mH90_mSusy1200 = PlotEntry("mH90_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                    // // // mH90_mSusy1200.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec2.push_back(mH90_mSusy1200);


                    // PlotEntry mH70_mSusy1600 = PlotEntry("mH70_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);
                    // // mH70_mSusy1600.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH70_mSusy1600);


                    // PlotEntry mH30_mSusy2000 = PlotEntry("mH30_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // mH30_mSusy2000.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH30_mSusy2000);

                    // PlotEntry mH50_mSusy2000 = PlotEntry("mH50_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH50_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // // mH50_mSusy2000.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH50_mSusy2000);

                    // PlotEntry mH70_mSusy2000 = PlotEntry("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // mH70_mSusy2000.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH70_mSusy2000);

                    // PlotEntry mH90_mSusy2000 = PlotEntry("mH90_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // mH90_mSusy2000.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(mH90_mSusy2000);



                    // FIVE: plot aesthetics
                    // Plotter plot = Plotter(plotEntryVec);
                    Plotter plot = Plotter(plotEntryVec2, plotEntryVec);
                    // Plotter plot = Plotter({}, plotEntryVec2);
                    // plot.AddLegend(0.74, 0.88, 0.77, 0.87); // top right (thin 2)
                    // plot.AddLegend(0.65, 0.88, 0.77, 0.87); // top right (wide 2)
                    // plot.AddLegend(0.50, 0.88, 0.77, 0.87); // top right (extra wide 2)

                    // plot.AddLegend(0.57, 0.88, 0.70, 0.87); // top right (thin 4)
                    // plot.AddLegend(0.57, 0.88, 0.16, 0.36); // top right (thin 4)
                    // plot.AddLegend(0.57, 0.88, 0.50, 0.70); // top right (thin 4)
                    // plot.AddLegend(0.17, 0.35, 0.17, 0.37); // top right (thin 4)
                    // plot.AddLegend(0.30, 0.50, 0.27, 0.47); // top right (thin 4)

                    
                    // plot.AddLegend(0.40, 0.88, 0.70, 0.87); // top right (extra wide 4)
                    plot.AddLegend(0.17, 0.35, 0.70, 0.87); // top left (thin 2)
                    plot.AddLatex(); // if you HAVE normalised
                    // plot.AddLatex(luminosity); // if you HAVE NOT normalised
                    // plot.AddRatioBox();
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