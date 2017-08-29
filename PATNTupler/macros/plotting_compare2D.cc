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
#include "PlotEntry2D.hh"
#include "Plotter.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "MacrosOnCondor.h"

// COMPARE VARIABLES AS YOU LOOP THROUGH CUTS (in 2D)

int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_08_14/2dMasses/softDrop/TTJets/fullCuts_NOAK4PT/trailBinningV3/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params, each combination = new plot
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Med2","Max","Med2","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Loose","Off","Loose"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","IDBTCv11","IDBTCv10","Med2"} };
    std::vector<int> cut3_ak8Pt = {300};
    // std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut4_ht = { {1500,2500} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: histogram settings
    double luminosity = 50.0;
    
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_prunedMass (GeV);fatJetB_prunedMass (GeV)", 400, 0, 200, 400, 0, 200);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_prunedMass (GeV);fatJetB_prunedMass (GeV)", 50, 0, 200, 50, 0, 200);
    // std::string varXAxis = "fatJetA_prunedMass";
    // std::string varYAxis = "fatJetB_prunedMass";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetB_SoftDrop_Mass (GeV)", 400, 0, 200, 400, 0, 200);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetB_SoftDrop_Mass (GeV)", 200, 0, 200, 200, 0, 200);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetB_SoftDrop_Mass (GeV)", 100, 0, 200, 100, 0, 200);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetB_SoftDrop_Mass (GeV)", 15, 0, 300, 15, 0, 300);
    TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetB_SoftDrop_Mass (GeV)", 40, 100, 300, 40, 100, 300);
    std::string varXAxis = "fatJetA_softDropMass";
    std::string varYAxis = "fatJetB_softDropMass";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 400, -1, 1, 400, -1, 1);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 200, -1, 1, 200, -1, 1);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 10, -1, 1, 10, -1, 1);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 10, -1, 1.166667, 10, -1, 1); // 10SPECIALA
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 20, -1, 1, 10, -1, 1); // 20SPECIALA
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 10, -1, 1, 20, -1, 1.166667); // 10SPECIALB
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 10, -1, 1, 20, -1, 1); // 20SPECIALB
    // std::string varXAxis = "fatJetA_doubleBtagDiscrim";
    // std::string varYAxis = "fatJetB_doubleBtagDiscrim";
    
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);fatJetA DBT_Score", 400, 0, 1000, 400, -1, 1);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);fatJetA DBT_Score", 400, 0, 2000, 400, -1, 1);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);fatJetA DBT_Score", 50, 0, 2000, 50, -1, 1);
    // std::string varXAxis = "fatJetA_p4.Pt()";
    // std::string varYAxis = "fatJetA_doubleBtagDiscrim";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetA DBT_Score", 400, 0, 200, 400, -1, 1);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetA DBT_Score", 50, 0, 200, 50, -1, 1);
    // std::string varXAxis = "fatJetA_softDropMass";
    // std::string varYAxis = "fatJetA_doubleBtagDiscrim";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetB_SoftDrop_Mass (GeV);fatJetB DBT_Score", 400, 0, 200, 400, -1, 1);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetB_SoftDrop_Mass (GeV);fatJetB DBT_Score", 50, 0, 200, 50, -1, 1);
    // std::string varXAxis = "fatJetB_softDropMass";
    // std::string varYAxis = "fatJetB_doubleBtagDiscrim";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);fatJetB p_{T} (GeV)", 400, 0, 1000, 400, 0, 1000);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);fatJetB p_{T} (GeV)", 400, 0, 2000, 400, 0, 2000);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);fatJetB p_{T} (GeV)", 50, 0, 2000, 50, 0, 2000);
    // std::string varXAxis = "fatJetA_p4.Pt()";
    // std::string varYAxis = "fatJetB_p4.Pt()";

    // TH2D hTemplate = TH2D("hTemplate", ";LeadingPtAK4Jet p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 400, 0, 1000, 400, 0, 1000);
    // TH2D hTemplate = TH2D("hTemplate", ";LeadingPtAK4Jet p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 400, 0, 2000, 400, 0, 2000);
    // TH2D hTemplate = TH2D("hTemplate", ";LeadingPtAK4Jet p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 50, 0, 2000, 50, 0, 2000);
    // std::string varXAxis = "slimJetA_p4.Pt()";
    // std::string varYAxis = "slimJetB_p4.Pt()";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 400, 0, 1000, 400, 0, 1000);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 400, 0, 2000, 400, 0, 2000);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 50, 0, 2000, 50, 0, 2000);
    // std::string varXAxis = "fatJetA_p4.Pt()";
    // std::string varYAxis = "slimJetB_p4.Pt()";

    // TH2D hTemplate = TH2D("hTemplate", ";fatJetB p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 400, 0, 1000, 400, 0, 1000);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetB p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 400, 0, 2000, 400, 0, 2000);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetB p_{T} (GeV);SecondaryPtAK4Jet p_{T} (GeV)", 50, 0, 2000, 50, 0, 2000);
    // std::string varXAxis = "fatJetB_p4.Pt()";
    // std::string varYAxis = "slimJetB_p4.Pt()";

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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compare2D.cc %s/%s__plotting_compare2D.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compare2D.cc %s/%s__plotting_compare2D.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_compare2D", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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
    std::string varToPlot = varYAxis + ":" + varXAxis;
    std::string varToPlotSaveName = varYAxis + "VS" + varXAxis;
    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);


      
                    // FOUR: samples to use
                    // PlotEntry2D plot2d = PlotEntry2D("QCD", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);

                    PlotEntry2D plot2d = PlotEntry2D("TTJets", hTemplate, varToPlot.c_str(), luminosity);
                    plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);

                    // PlotEntry2D plot2d = PlotEntry2D("Z+Jets", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/ZJets_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);

                    // PlotEntry2D plot2d = PlotEntry2D("W+Jets", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/WJets_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);


                    // PlotEntry2D plot2d = PlotEntry2D("mH30_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);

                    // PlotEntry2D plot2d = PlotEntry2D("mH90_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);


                    // PlotEntry2D plot2d = PlotEntry2D("mH30_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);

                    // PlotEntry2D plot2d = PlotEntry2D("mH90_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);


                    // PlotEntry2D plot2d = PlotEntry2D("mH30_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);

                    // PlotEntry2D plot2d = PlotEntry2D("mH50_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH50p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);

                    // PlotEntry2D plot2d = PlotEntry2D("mH70_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);

                    // PlotEntry2D plot2d = PlotEntry2D("mH90_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);


                    // PlotEntry2D plot2d = PlotEntry2D("mH30_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

                    // PlotEntry2D plot2d = PlotEntry2D("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

                    // PlotEntry2D plot2d = PlotEntry2D("mH90_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);



                    // FIVE: plot aesthetics
                    // double S1_Node1 = 31.5;
                    // double S1_Node2 = 16.5;
                    // double SMAX_Node1 = 168.5;
                    // double SMAX_Node2 = 115.04;
                    // std::vector<double> SN_Nodes = {40.7, 50.9, 62.1, 74.3, 87.5, 101.7, 116.9, 133.1, 150.3};
                    // MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsV04", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes);

                    double S1_Node1 = 180;
                    double S1_Node2 = 150;
                    double SMAX_Node1 = 200;
                    double SMAX_Node2 = 180;
                    std::vector<double> SN_Nodes = {180};
                    MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsVXQXY", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes);


                    Plotter plot = Plotter({plot2d});
                    plot.AddLatex(luminosity);
                    std::string saveName = varToPlotSaveName;
                    saveName += "__dbt" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "And" + cut2_ak8Dbt[iCut2][2] + cut2_ak8Dbt[iCut2][3];
                    saveName += Form("_ak8pt%d", cut3_ak8Pt[iCut3]);
                    if (cut4_ht[iCut4][1]==13000) saveName += Form("_ht%dplus",cut4_ht[iCut4][0]);
                    else saveName += Form("_ht%dto%d",cut4_ht[iCut4][0],cut4_ht[iCut4][1]);
                    saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    // saveName += "APPEND_SAVE_NAME"; // OPTIONAL: can manually append the automatic savename
                    // saveName = "MANUAL_SAVE_NAME"; // OPTIONAL: can manually overwrite the automatic savename
                    std::string outputFile = outputDir + "/" + saveName + ".pdf";
                    plot.Save2D(outputFile.c_str());
                    outputFile = outputDir + "/" + saveName + "_withGrid.pdf";
                    plot.Save2D(outputFile.c_str(), MassCutsObject); // if want mass grids on plot
                    plot.SetLogZ(); // toggle ON or OFF
                    outputFile = outputDir + "/" + saveName + "_logZ.pdf";
                    plot.Save2D(outputFile.c_str());
                    outputFile = outputDir + "/" + saveName + "_withGrid_logZ.pdf";
                    plot.Save2D(outputFile.c_str(), MassCutsObject); // if want mass grids on plot



                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states
    return 0;
}