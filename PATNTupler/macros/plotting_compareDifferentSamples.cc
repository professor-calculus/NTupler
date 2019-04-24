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
    std::string outputDir = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/plots_MHT_2019_01_15/1D_Variables/TopGenPTCompare/PowHeg_vs_MGLO_HTStitched600/"; // where we are going to save the output plots (should include the samples name, and any important features)




    // TWO: set of cut params, each combination = new plot
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"DIAG_UP", "Loose"} }; // Top Diagnol Corner Crossing Axis at...--> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Loose","Off","Loose"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {300};
    // std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,99999}, {1500,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {300,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)
    // std::vector<std::vector<int>> cut6_mht = { {0,200}, {200,99999} }; // MHT BIN
    // std::vector<std::vector<int>> cut6_lepVeto = { {0,9999}, {0,1} }; // Lepton Veto BIN

    // minimum pre-selection cuts only
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min$
    std::vector<int> cut3_ak8Pt = {0};
    std::vector<std::vector<int>> cut4_ht = { {0,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {0,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)
    std::vector<std::vector<int>> cut6_mht = { {0,99999} }; // MHT BIN
    std::vector<std::vector<int>> cut6_lepVeto = { {0,9999} }; // Lepton Veto BIN

    // THREE: plot histogram settings
    double luminosity = 35.867; // 2016 DATASET (data plots should not be using this object)
    // double luminosity = 41.370; // 2017 DATASET
    
    // std::string varToPlot = "fatJetA_doubleBtagDiscrim";
    // std::string varToPlot = "fatJetA_softDropMassPuppi";
    // std::string varToPlot = "fatJetA_p4.Pt()";
    // std::string varToPlot = "slimJetA_p4.Pt()";
    // std::string varToPlot = "ht";
    // std::string varToPlot = "nPU";
    std::string varToPlot = "top1_pT";

    // std::vector<double> customBinning = {-1.0, 0.3, 1.0};
    // TH1D hTemplate("hTemplate", ";fatJetA doubleBtagDiscriminator;fraction of events / bin", customBinning.size()-1, &(customBinning)[0]);
    // TH1D hTemplate("hTemplate", ";fatJetA doubleBtagDiscriminator;events / bin", 20, -1, 1);
    // TH1D hTemplate("hTemplate", ";fatJetA doubleBtagDiscriminator;fraction of events / bin", 20, -1, 1);
    // TH1D hTemplate("hTemplate", ";fatJetA SoftDropMass (GeV);events / bin", 40, 0, 200);
    // TH1D hTemplate("hTemplate", ";fatJetA SoftDropMass (GeV);events / bin", 10, 0, 200);
    // TH1D hTemplate("hTemplate", ";fatJetA SoftDropMass (GeV);fraction of events / bin", 40, 0, 200);
    // TH1D hTemplate("hTemplate", ";H_{T} (GeV);fraction of events / bin", 90, 0, 9000);
    // TH1D hTemplate("hTemplate", ";fatJetA p_{T} (GeV);fraction of events / bin", 60, 0, 3000);
    // TH1D hTemplate("hTemplate", ";leadingAK4Jet p_{T} (GeV);fraction of events / bin", 60, 0, 3000);
    // TH1D hTemplate("hTemplate", ";nPU;fraction of events / bin", 90, 0, 90);
    // TH1D hTemplate("hTemplate", ";nGluino;fraction of events / bin", 3, 0, 3);
    TH1D hTemplate("hTemplate", ";Top Quark p_{T} (GeV);fraction of events / bin", 60, 0, 3000);




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
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_compareDifferentSamples", "/opt/ppd/scratch/titterton/jobLogs/macros/");
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
                for (size_t iCut6 = 0; iCut6 < cut6_mht.size(); ++iCut6){

                    std::vector<PlotEntry> plotEntryVec;
                    std::vector<PlotEntry> plotEntryVec2;
                    
                    std::string dbtCut = "";
                    if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP")
                        dbtCut = Form("fatJetA_doubleBtagDiscrim >= (-1.0 * fatJetB_doubleBtagDiscrim + 1.0 + %f) ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]) );
                    if (cut2_ak8Dbt[iCut2].size() == 4)
                        dbtCut = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]) );
                    std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && mht>=%d && mht<%d && nrVetoObjects>=%d && nrVetoObjects<%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1], cut6_mht[iCut6][0], cut6_mht[iCut6][1], cut6_lepVeto[iCut6][0], cut6_lepVeto[iCut6][1]);




      
                    // FOUR: samples to use

                    //PlotEntry WJets = PlotEntry("WJets", hTemplate, varToPlot.c_str(), luminosity);
                    //WJets.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/WJets/flatTree.root", cutToApply.c_str(), 95.14);
                    // WJets.NormalisePlot(); // OPTIONAL: toggle on or off
                    //plotEntryVec.push_back(WJets);

                    //PlotEntry ZJets = PlotEntry("ZJets", hTemplate, varToPlot.c_str(), luminosity);
                    //ZJets.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/ZJets/flatTree.root", cutToApply.c_str(), 52.79);
                    // ZJets.NormalisePlot(); // OPTIONAL: toggle on or off
                    //plotEntryVec.push_back(ZJets);

                    //PlotEntry ttbar = PlotEntry("TTJets Powheg", hTemplate, varToPlot.c_str(), luminosity);
                    //ttbar.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08_FULL/mc/TTJets/flatTree.root", cutToApply.c_str(), 831.76);
                    //ttbar.NormalisePlot(); // OPTIONAL: toggle on or off
                    //plotEntryVec.push_back(ttbar);

                    PlotEntry ttbar = PlotEntry("TTJets Powheg", hTemplate, varToPlot.c_str(), luminosity);
                    ttbar.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08_FULL/mc/TTJets/flatTree.root", "", 831.76);
                    //ttbar.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(ttbar);

                    PlotEntry ttbarMG = PlotEntry("TTJets MG Stitched", hTemplate, varToPlot.c_str(), luminosity);
                    ttbarMG.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08_FULL/mc/TTJets_MG/flatTree.root", "lheHT<600.0", 831.76);
                    ttbarMG.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08_FULL/mc/TTJets_HT600to800/flatTree.root", "lheHT>=600.0", 831.76);
                    ttbarMG.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08_FULL/mc/TTJets_HT800to1200/flatTree.root", "lheHT>=600.0", 831.76);
                    ttbarMG.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08_FULL/mc/TTJets_HT1200to2500/flatTree.root", "lheHT>=600.0", 831.76);
                    ttbarMG.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08_FULL/mc/TTJets_HT2500toInf/flatTree.root", "lheHT>=600.0", 831.76);
                    //ttbar.NormalisePlot(); // OPTIONAL: toggle on or off
                    plotEntryVec.push_back(ttbarMG);

                    //PlotEntry QCD = PlotEntry("QCD", hTemplate, varToPlot.c_str(), luminosity);
                    // PlotEntry QCD = PlotEntry("QCD 2016", hTemplate, varToPlot.c_str(), luminosity);
                    // PlotEntry QCD = PlotEntry("No fatJetB Mass Cut", hTemplate, varToPlot.c_str(), luminosity);
                    //QCD.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
                    //QCD.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
                    //QCD.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);
                    // QCD.NormalisePlot(); // OPTIONAL: toggle on or off
                    //plotEntryVec.push_back(QCD);

		    //PlotEntry P1_1800sq_R0p99 = PlotEntry("P1_1800sq_R0p99", hTemplate, varToPlot.c_str(), luminosity);
                    //P1_1800sq_R0p99.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P1_1800sq_R0p99/flatTree.root", cutToApply.c_str(), 0.023290);
		    //plotEntryVec2.push_back(P1_1800sq_R0p99);

                    //PlotEntry P1_1800sq_R0p384 = PlotEntry("P1_1800sq_R0p384", hTemplate, varToPlot.c_str(), luminosity);
                    //P1_1800sq_R0p384.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P1_1800sq_R0p384/flatTree.root", cutToApply.c_str(), 0.023290);
                    //plotEntryVec2.push_back(P1_1800sq_R0p384);

                    //PlotEntry P3_1900sq_R0p99 = PlotEntry("P3_1900sq_R0p99", hTemplate, varToPlot.c_str(), luminosity);
                    //P3_1900sq_R0p99.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P3_1900sq_R0p99/flatTree.root", cutToApply.c_str(), 0.0219);
                    //plotEntryVec2.push_back(P3_1900sq_R0p99);

                    //PlotEntry P3_1900sq_R0p384 = PlotEntry("P3_1900sq_R0p384", hTemplate, varToPlot.c_str(), luminosity);
                    //P3_1900sq_R0p384.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P3_1900sq_R0p384/flatTree.root", cutToApply.c_str(), 0.0219);
                    //plotEntryVec2.push_back(P3_1900sq_R0p384);

                    //PlotEntry P5_1800sq_R0p99 = PlotEntry("P5_1800sq_R0p99", hTemplate, varToPlot.c_str(), luminosity);
                    //P5_1800sq_R0p99.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P5_1800sq_R0p99/flatTree.root", cutToApply.c_str(), 0.023290);
                    //plotEntryVec2.push_back(P5_1800sq_R0p99);

                    //PlotEntry P5_1800sq_R0p384 = PlotEntry("P5_1800sq_R0p384", hTemplate, varToPlot.c_str(), luminosity);
                    //P5_1800sq_R0p384.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P5_1800sq_R0p384/flatTree.root", cutToApply.c_str(), 0.023290);
                    //plotEntryVec2.push_back(P5_1800sq_R0p384);

                    //PlotEntry P6_1800sq_R0p99 = PlotEntry("P6_1800sq_R0p99", hTemplate, varToPlot.c_str(), luminosity);
                    //P6_1800sq_R0p99.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P6_1800sq_R0p99/flatTree.root", cutToApply.c_str(), 0.023290);
                    //plotEntryVec2.push_back(P6_1800sq_R0p99);

                    //PlotEntry P6_1800sq_R0p384 = PlotEntry("P6_1800sq_R0p384", hTemplate, varToPlot.c_str(), luminosity);
                    //P6_1800sq_R0p384.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P6_1800sq_R0p384/flatTree.root", cutToApply.c_str(), 0.023290);
                    //plotEntryVec2.push_back(P6_1800sq_R0p384);

                    //PlotEntry P7_1900sq_R0p99 = PlotEntry("P7_1900sq_R0p99", hTemplate, varToPlot.c_str(), luminosity);
                    //P7_1900sq_R0p99.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P7_1900sq_R0p99/flatTree.root", cutToApply.c_str(), 0.0219);
                    //plotEntryVec2.push_back(P7_1900sq_R0p99);

                    //PlotEntry P7_1900sq_R0p384 = PlotEntry("P7_1900sq_R0p384", hTemplate, varToPlot.c_str(), luminosity);
                    //P7_1900sq_R0p384.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P7_1900sq_R0p384/flatTree.root", cutToApply.c_str(), 0.0219);
                    //plotEntryVec2.push_back(P7_1900sq_R0p384);

                    //PlotEntry P8_1900sq_R0p99 = PlotEntry("P8_1900sq_R0p99", hTemplate, varToPlot.c_str(), luminosity);
                    //P8_1900sq_R0p99.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P8_1900sq_R0p99/flatTree.root", cutToApply.c_str(), 0.0219);
                    //plotEntryVec2.push_back(P8_1900sq_R0p99);

                    //PlotEntry P8_1900sq_R0p384 = PlotEntry("P8_1900sq_R0p384", hTemplate, varToPlot.c_str(), luminosity);
                    //P8_1900sq_R0p384.AddInput("/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/flatTrees_MHT_2019_01_08/mc/P8_1900sq_R0p384/flatTree.root", cutToApply.c_str(), 0.0219);
                    //plotEntryVec2.push_back(P8_1900sq_R0p384);

                    // PlotEntry mH90_mSusy1200 = PlotEntry("mH90_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc16/mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.4951000*0.85*0.85);
                    // plotEntryVec2.push_back(mH90_mSusy1200);

                    // PlotEntry mH90_mSusy1600 = PlotEntry("mH90_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc16/mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.0603900*0.85*0.85);
                    // plotEntryVec2.push_back(mH90_mSusy1600);


                    // std::string cutToApplyA = "20 < fatJetB_softDropMassPuppi && fatJetB_softDropMassPuppi < 50 && " + cutToApply;
                    // PlotEntry QCD_A = PlotEntry("20 < fatJetB SoftDropMass < 50 GeV", hTemplate, varToPlot.c_str(), luminosity);
                    // QCD_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc16/QCD_HT1000to1500_ht1499plus/flatTree.root", cutToApplyA.c_str(), 1206);
                    // QCD_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc16/QCD_HT1500to2000_ht1499plus/flatTree.root", cutToApplyA.c_str(), 120.4);
                    // QCD_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc16/QCD_HT2000toInf_ht1499plus/flatTree.root", cutToApplyA.c_str(), 25.25);
                    // QCD_A.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(QCD_A);

                    // std::string cutToApplyA = "15 < fatJetA_softDropMassPuppi && fatJetA_softDropMassPuppi < 20 && " + cutToApply;
                    // PlotEntry DATA_A = PlotEntry("15 < fatJetA SoftDropMass < 20 GeV", hTemplate, varToPlot.c_str());
                    // DATA_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data16/JetHT_Run2016Total_ht1499plus/flatTree.root", cutToApplyA.c_str());
                    // DATA_A.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data17/JetHT_Run2017TOTAL_ht1499plus/flatTree.root", cutToApplyA.c_str());
                    // DATA_A.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA_A);

                    // std::string cutToApplyB = "25 < fatJetA_softDropMassPuppi && fatJetA_softDropMassPuppi < 30 && " + cutToApply;
                    // PlotEntry DATA_B = PlotEntry("25 < fatJetA SoftDropMass < 30 GeV", hTemplate, varToPlot.c_str());
                    // DATA_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data16/JetHT_Run2016Total_ht1499plus/flatTree.root", cutToApplyB.c_str());
                    // DATA_B.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data17/JetHT_Run2017TOTAL_ht1499plus/flatTree.root", cutToApplyB.c_str());
                    // DATA_B.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA_B);

                    // std::string cutToApplyC = "35 < fatJetA_softDropMassPuppi && fatJetA_softDropMassPuppi < 40 && " + cutToApply;
                    // PlotEntry DATA_C = PlotEntry("35 < fatJetA SoftDropMass < 40 GeV", hTemplate, varToPlot.c_str());
                    // DATA_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data16/JetHT_Run2016Total_ht1499plus/flatTree.root", cutToApplyC.c_str());
                    // DATA_C.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data17/JetHT_Run2017TOTAL_ht1499plus/flatTree.root", cutToApplyC.c_str());
                    // DATA_C.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA_C);

                    // std::string cutToApplyD = "45 < fatJetA_softDropMassPuppi && fatJetA_softDropMassPuppi < 50 && " + cutToApply;
                    // PlotEntry DATA_D = PlotEntry("45 < fatJetA SoftDropMass < 50 GeV", hTemplate, varToPlot.c_str());
                    // DATA_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data16/JetHT_Run2016Total_ht1499plus/flatTree.root", cutToApplyD.c_str());
                    // DATA_D.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data17/JetHT_Run2017TOTAL_ht1499plus/flatTree.root", cutToApplyD.c_str());
                    // DATA_D.NormalisePlot(); // OPTIONAL: toggle on or off
                    // plotEntryVec.push_back(DATA_D);








                    // FIVE: plot aesthetics
                    Plotter plot = Plotter(plotEntryVec);
                    // Plotter plot = Plotter(plotEntryVec2);
                    //Plotter plot = Plotter(plotEntryVec2, plotEntryVec);
                    // Plotter plot = Plotter({}, plotEntryVec);

                    // plot.AddLegend(0.74, 0.88, 0.77, 0.87); // top right (thin 2)
                    // plot.AddLegend(0.65, 0.88, 0.77, 0.87); // top right (wide 2)
                    // plot.AddLegend(0.47, 0.88, 0.73, 0.87); // top right (extra wide 2)
                    // plot.AddLegend(0.28, 0.51, 0.20, 0.35); // bottom centre (extra wide 2)
                    // plot.AddLegend(0.74, 0.88, 0.70, 0.87); // top right (thin 4)
                    // plot.AddLegend(0.60, 0.88, 0.70, 0.87); // top right (wide 4)
                    // plot.AddLegend(0.50, 0.88, 0.70, 0.87); // top right (extra wide 4)
                    // plot.AddLegend(0.33, 0.88, 0.70, 0.87); // top right (extra wide 4)
                    // plot.AddLegend(0.33, 0.88, 0.50, 0.67); // top right (extra wide 4)
                    plot.AddLegend2Cols(0, 0.43, 0.89, 0.77, 0.87, 0.03); // 2 COLUMNS
                    // plot.AddLegend(0.20, 0.60, 0.20, 0.33); // bottom left (wide 2)
                    // plot.AddLegend(0.20, 0.60, 0.17, 0.37); // bottom left (wide 4)
                    // plot.AddLegend(0.55, 0.88, 0.17, 0.37); // bottom right (wide 4)
                    // plot.AddLegend(0.22, 0.44, 0.77, 0.87); // top left (wide 2)
                    // plot.AddLegend(0.22, 0.45, 0.70, 0.87); // top left (wide 4)

                    // plot.AddLatex(); // simulation only - normalised
                    plot.AddLatex(luminosity); // simulation only - NOT normalised
                    // plot.AddLatex(luminosity, "#it{Preliminary}"); // DATA
                    // plot.AddLatex("#it{Preliminary}"); // DATA
                    
                    plot.AddRatioBox(0.1, 2.9, "ratio", true);
                    // plot.AddRatioBox(0.929, 1.109);
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
		    if (cut6_mht[iCut6][1]==99999) saveName += Form("_mht%dplus",cut6_mht[iCut6][0]);
                    else saveName += Form("_mht%dto%d",cut6_mht[iCut6][0],cut6_mht[iCut6][1]);
                    saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    // saveName += "APPEND_SAVE_NAME"; // OPTIONAL: can manually append the automatic savename
                    // saveName = "MANUAL_SAVE_NAME"; // OPTIONAL: can manually overwrite the automatic savename
                    std::string outputFile = outputDir + "/" + saveName + ".pdf";
                    plot.Save(outputFile.c_str());
                    plot.SetLogY();
                    outputFile = outputDir + "/" + saveName + "__logY.pdf";
                    plot.Save(outputFile.c_str());

                } // closes loop through cut 6 states
                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states
    return 0;
}
