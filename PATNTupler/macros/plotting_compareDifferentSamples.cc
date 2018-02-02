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
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_01_08/1d_dbtDiscrim_V2/DATA/specialQCDCutsV2_coarseLoose/"; // where we are going to save the output plots (should include the samples name, and any important features)
    // std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_01_08/qcdDomRegionV2/OffMaxOffMax_fJ300_ht1500to2500_noAK4/COARSE_mH90_noMassCut/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params, each combination = new plot
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"DIAG_UP", "Loose"} }; // Top Diagnol Corner Crossing Axis at...--> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Loose","Off","Loose"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Loose","Off","Loose"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,2500} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: plot histogram settings
    double luminosity = 35.867; // 2016 DATASET (data plots should not be using this object)
    std::string varToPlot = "fatJetA_doubleBtagDiscrim";
    // std::string varToPlot = "fatJetA_softDropMass";
    // std::string varToPlot = "fatJetA_p4.Pt()";
    // std::string varToPlot = "slimJetA_p4.Pt()";
    // std::string varToPlot = "ht";
    std::vector<double> customBinning = {-1.0, 0.3, 1.0};
    // TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", 20, -1, 1);
    TH1D hTemplate("hTemplate", ";fatJetA_doubleBtagDiscrim;a.u.", customBinning.size()-1, &(customBinning)[0]);
    // TH1D hTemplate("hTemplate", ";fatJetA SoftDropMass (GeV);events / bin", 50, 0, 200);
    // TH1D hTemplate("hTemplate", ";H_{T} (GeV);a.u.", 80, 0, 8000);
    // TH1D hTemplate("hTemplate", ";fatJetA p_{T} (GeV);a.u.", 60, 0, 3000);
    // TH1D hTemplate("hTemplate", ";leadingAK4Jet p_{T} (GeV);a.u.", 60, 0, 3000);



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
                    
                    std::string dbtCut = "";
                    if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP")
                        dbtCut = Form("fatJetA_doubleBtagDiscrim >= (-1.0 * fatJetB_doubleBtagDiscrim + 1.0 + %f) ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]) );
                    if (cut2_ak8Dbt[iCut2].size() == 4)
                        dbtCut = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]) );
                    std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    // cutToApply += " && fatJetA_softDropMass > 45 && fatJetA_softDropMass < 50";


      
                    // FOUR: samples to use

                    // std::string cutToApplyB = "70 < fatJetB_softDropMass && fatJetB_softDropMass < 110 && " + cutToApply;
                    // PlotEntry ZJets_B = PlotEntry("70 < fatJetB Mass < 110 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // ZJets_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/ZJets_HT600toInf/flatTree.root", cutToApplyB.c_str(), 5.67);
                    // ZJets_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ZJets_B);

                    // PlotEntry WJets = PlotEntry("WJets", hTemplate, varToPlot.c_str(), luminosity);
                    // WJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/WJets_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);
                    // // WJets.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(WJets);

                    // PlotEntry ZJets = PlotEntry("ZJets", hTemplate, varToPlot.c_str(), luminosity);
                    // ZJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/ZJets_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);
                    // // ZJets.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ZJets);

                    // PlotEntry ttbarNLO = PlotEntry("TTJets", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarNLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO_ht1500plus/flatTree.root", cutToApply.c_str(), 831.76);
                    // // ttbarNLO.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ttbarNLO);

                    // PlotEntry ttbarNLO = PlotEntry("TTJets", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarNLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO_ht1500plus/flatTree.root", cutToApply.c_str(), 831.76);
                    // // ttbarNLO.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(ttbarNLO);

                    // PlotEntry QCD = PlotEntry("QCD", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500_ht1500plus/flatTree.root", cutToApply.c_str(), 1206);
                    // QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000_ht1500plus/flatTree.root", cutToApply.c_str(), 120.4);
                    // QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf_ht1500plus/flatTree.root", cutToApply.c_str(), 25.25);
                    // // QCD.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD);


                    // std::string cutToApplyA = "10 <= fatJetB_softDropMass && " + cutToApply;
                    // PlotEntry QCD_A = PlotEntry("fatJetB Mass > 10 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500_ht1500plus/flatTree.root", cutToApplyA.c_str(), 1206);
                    // QCD_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000_ht1500plus/flatTree.root", cutToApplyA.c_str(), 120.4);
                    // QCD_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf_ht1500plus/flatTree.root", cutToApplyA.c_str(), 25.25);
                    // QCD_A.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_A);

                    // std::string cutToApplyB = "120 < fatJetB_softDropMass && fatJetB_softDropMass < 200 && " + cutToApply;
                    // PlotEntry QCD_B = PlotEntry("120 < fatJetB Mass < 200 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500_ht1500plus/flatTree.root", cutToApplyB.c_str(), 1206);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000_ht1500plus/flatTree.root", cutToApplyB.c_str(), 120.4);
                    // QCD_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf_ht1500plus/flatTree.root", cutToApplyB.c_str(), 25.25);
                    // QCD_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_B);

                    // std::string cutToApplyC = "80 < fatJetB_softDropMass && fatJetB_softDropMass < 90 && " + cutToApply;
                    // PlotEntry QCD_C = PlotEntry("80 < fatJetB Mass < 90 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500_ht1500plus/flatTree.root", cutToApplyC.c_str(), 1206);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000_ht1500plus/flatTree.root", cutToApplyC.c_str(), 120.4);
                    // QCD_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf_ht1500plus/flatTree.root", cutToApplyC.c_str(), 25.25);
                    // QCD_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_C);

                    // std::string cutToApplyD = "120 < fatJetB_softDropMass && fatJetB_softDropMass < 130 && " + cutToApply;
                    // PlotEntry QCD_D = PlotEntry("120 < fatJetB Mass < 130 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500_ht1500plus/flatTree.root", cutToApplyD.c_str(), 1206);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000_ht1500plus/flatTree.root", cutToApplyD.c_str(), 120.4);
                    // QCD_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf_ht1500plus/flatTree.root", cutToApplyD.c_str(), 25.25);
                    // QCD_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_D);





                    // std::string cutToApplyA = "150 <= fatJetB_softDropMass && fatJetB_softDropMass < 200 && " + cutToApply;
                    // PlotEntry TTJets_A = PlotEntry("150 < fatJetB Mass < 200 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // TTJets_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO_ht1500plus/flatTree.root", cutToApplyA.c_str(), 831.76);
                    // TTJets_A.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(TTJets_A);

                    // std::string cutToApplyB = "40 < fatJetB_softDropMass && fatJetB_softDropMass < 50 && " + cutToApply;
                    // PlotEntry TTJets_B = PlotEntry("40 < fatJetB Mass < 50 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // TTJets_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO_ht1500plus/flatTree.root", cutToApplyB.c_str(), 831.76);
                    // TTJets_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(TTJets_B);

                    // std::string cutToApplyC = "80 < fatJetB_softDropMass && fatJetB_softDropMass < 90 && " + cutToApply;
                    // PlotEntry TTJets_C = PlotEntry("80 < fatJetB Mass < 90 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // TTJets_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO_ht1500plus/flatTree.root", cutToApplyC.c_str(), 831.76);
                    // TTJets_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(TTJets_C);

                    // std::string cutToApplyD = "120 < fatJetB_softDropMass && fatJetB_softDropMass < 130 && " + cutToApply;
                    // PlotEntry TTJets_D = PlotEntry("120 < fatJetB Mass < 130 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // TTJets_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO_ht1500plus/flatTree.root", cutToApplyD.c_str(), 831.76);
                    // TTJets_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(TTJets_D);








                    // std::string cutToApplyE = "15 < fatJetA_softDropMass && fatJetA_softDropMass < 20 && " + cutToApply;
                    // PlotEntry QCD_E = PlotEntry("15 < Mass < 20 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_E.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500/flatTree.root", cutToApplyE.c_str(), 1206);
                    // QCD_E.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000/flatTree.root", cutToApplyE.c_str(), 120.4);
                    // QCD_E.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf/flatTree.root", cutToApplyE.c_str(), 25.25);
                    // QCD_E.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_E);

                    // std::string cutToApplyF = "25 < fatJetA_softDropMass && fatJetA_softDropMass < 30 && " + cutToApply;
                    // PlotEntry QCD_F = PlotEntry("25 < Mass < 30 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_F.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500/flatTree.root", cutToApplyF.c_str(), 1206);
                    // QCD_F.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000/flatTree.root", cutToApplyF.c_str(), 120.4);
                    // QCD_F.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf/flatTree.root", cutToApplyF.c_str(), 25.25);
                    // QCD_F.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_F);

                    // std::string cutToApplyG = "35 < fatJetA_softDropMass && fatJetA_softDropMass < 40 && " + cutToApply;
                    // PlotEntry QCD_G = PlotEntry("35 < Mass < 40 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_G.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500/flatTree.root", cutToApplyG.c_str(), 1206);
                    // QCD_G.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000/flatTree.root", cutToApplyG.c_str(), 120.4);
                    // QCD_G.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf/flatTree.root", cutToApplyG.c_str(), 25.25);
                    // QCD_G.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_G);

                    // std::string cutToApplyH = "45 < fatJetA_softDropMass && fatJetA_softDropMass < 50 && " + cutToApply;
                    // PlotEntry QCD_H = PlotEntry("45 < Mass < 50 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_H.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500/flatTree.root", cutToApplyH.c_str(), 1206);
                    // QCD_H.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000/flatTree.root", cutToApplyH.c_str(), 120.4);
                    // QCD_H.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf/flatTree.root", cutToApplyH.c_str(), 25.25);
                    // QCD_H.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_H);


                    // PlotEntry DATA = PlotEntry("DATA", hTemplate, varToPlot.c_str());
                    // DATA.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApply.c_str());
                    // // DATA.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA);

                    std::string cutToApplyA = "15 < fatJetA_softDropMass && fatJetA_softDropMass < 20 && " + cutToApply;
                    PlotEntry DATA_A = PlotEntry("15 < Mass < 20 GeV", hTemplate, varToPlot.c_str());
                    DATA_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApplyA.c_str());
                    DATA_A.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(DATA_A);

                    std::string cutToApplyB = "25 < fatJetA_softDropMass && fatJetA_softDropMass < 30 && " + cutToApply;
                    PlotEntry DATA_B = PlotEntry("25 < Mass < 30 GeV", hTemplate, varToPlot.c_str());
                    DATA_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApplyB.c_str());
                    DATA_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(DATA_B);

                    std::string cutToApplyC = "35 < fatJetA_softDropMass && fatJetA_softDropMass < 40 && " + cutToApply;
                    PlotEntry DATA_C = PlotEntry("35 < Mass < 40 GeV", hTemplate, varToPlot.c_str());
                    DATA_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApplyC.c_str());
                    DATA_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(DATA_C);

                    std::string cutToApplyD = "45 < fatJetA_softDropMass && fatJetA_softDropMass < 50 && " + cutToApply;
                    PlotEntry DATA_D = PlotEntry("45 < Mass < 50 GeV", hTemplate, varToPlot.c_str());
                    DATA_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApplyD.c_str());
                    DATA_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(DATA_D);

                    // std::string cutToApplyA = "0 < fatJetA_softDropMass && fatJetA_softDropMass < 5 && " + cutToApply;
                    // PlotEntry DATA_A = PlotEntry("0 < Mass < 5 GeV", hTemplate, varToPlot.c_str());
                    // DATA_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApplyA.c_str());
                    // DATA_A.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA_A);

                    // std::string cutToApplyB = "40 < fatJetA_softDropMass && fatJetA_softDropMass < 50 && " + cutToApply;
                    // PlotEntry DATA_B = PlotEntry("40 < Mass < 50 GeV", hTemplate, varToPlot.c_str());
                    // DATA_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApplyB.c_str());
                    // DATA_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA_B);

                    // std::string cutToApplyC = "80 < fatJetA_softDropMass && fatJetA_softDropMass < 90 && " + cutToApply;
                    // PlotEntry DATA_C = PlotEntry("80 < Mass < 90 GeV", hTemplate, varToPlot.c_str());
                    // DATA_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApplyC.c_str());
                    // DATA_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA_C);

                    // std::string cutToApplyD = "120 < fatJetA_softDropMass && fatJetA_softDropMass < 130 && " + cutToApply;
                    // PlotEntry DATA_D = PlotEntry("120 < Mass < 130 GeV", hTemplate, varToPlot.c_str());
                    // DATA_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApplyD.c_str());
                    // DATA_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA_D);



                    // PlotEntry mH90_mSusy1200 = PlotEntry("mH90_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.4951000*0.85*0.85);
                    // // mH90_mSusy1200.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec2.push_back(mH90_mSusy1200);

                    // PlotEntry mH90_mSusy1600 = PlotEntry("mH90_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.0603900*0.85*0.85);
                    // // mH90_mSusy1600.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec2.push_back(mH90_mSusy1600);






                    // FIVE: plot aesthetics
                    Plotter plot = Plotter(plotEntryVec);
                    // Plotter plot = Plotter(plotEntryVec2);
                    // Plotter plot = Plotter(plotEntryVec2, plotEntryVec);
                    // Plotter plot = Plotter({}, plotEntryVec);
                    // plot.AddLegend(0.74, 0.88, 0.77, 0.87); // top right (thin 2)
                    // plot.AddLegend(0.65, 0.88, 0.77, 0.87); // top right (wide 2)
                    // plot.AddLegend(0.50, 0.88, 0.73, 0.87); // top right (extra wide 2)
                    // plot.AddLegend(0.74, 0.88, 0.70, 0.87); // top right (thin 4)
                    // plot.AddLegend(0.60, 0.88, 0.70, 0.87); // top right (wide 4)
                    // plot.AddLegend(0.50, 0.88, 0.70, 0.87); // top right (extra wide 4)
                    // plot.AddLegend2Cols(0, 0.43, 0.89, 0.77, 0.87, 0.03); // 2 COLUMNS
                    plot.AddLegend(0.17, 0.45, 0.17, 0.37); // bottom left (wide 4)
                    // plot.AddLegend(0.55, 0.88, 0.17, 0.37); // bottom right (wide 4)
                    // plot.AddLegend(0.17, 0.35, 0.70, 0.87); // top left (wide 4)

                    // plot.AddLatex(); // if you HAVE normalised
                    // plot.AddLatex(luminosity); // if you HAVE NOT normalised
                    // plot.AddLatex("#it{Data} 2016"); // normalised DATA
                    plot.AddLatex(luminosity, "#it{Data} 2016"); // DATA
                    
                    // plot.AddRatioBox(0.1, 1.9, "ratio", true);
                    // plot.AddRatioBox(0.91, 1.09);
                    // plot.AddRatioBox();
                    plot.SetErrors();




                    std::string saveName = varToPlot;
                    if (cut2_ak8Dbt.size()>1) saveName += "__dbtVary";
                    else{
                        if (cut2_ak8Dbt[0].size() == 4) saveName += "__dbt" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1] + "And" + cut2_ak8Dbt[0][2] + cut2_ak8Dbt[0][3];
                        if (cut2_ak8Dbt[0].size() == 2 && cut2_ak8Dbt[0][0] == "DIAG_UP") saveName += "__dbtDiagUp" + cut2_ak8Dbt[0][1];
                    }
                    saveName += Form("_ak8pt%d", cut3_ak8Pt[iCut3]);
                    if (cut4_ht[iCut4][1]==99999) saveName += Form("_ht%dplus",cut4_ht[iCut4][0]);
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