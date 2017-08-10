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
#include "MassRegionCuts.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "MacrosOnCondor.h"

// PLOT 2D MASS DISTRIBUTIONS IN CERTAIN REGIONS AND COUNT EVENTS
int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_19/QCD/MassCutsV03/fullCutsNOAK4PT_HT1500to2500/signal/"; // where we are going to save the output plots (should include the samples name + binning maybe)



    // TWO: set the cut params.
    std::vector<std::string> cut2_ak8Dbt = {"Med2","Max","Med2","Max"}; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::string> cut2_ak8Dbt = {"Off","Loose","Off","Loose"}; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::string> cut2_ak8Dbt = {"Med2","Max","Off","IDBTCv1"}; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::string> cut2_ak8Dbt = {"Off","IDBTCv1","Med2","Max"}; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    int cut3_ak8Pt = 300;
    // int cut3_ak8Pt = -1;
    std::vector<int> cut4_ht = {1500,2500}; // HT BIN
    // std::vector<int> cut4_ht = {2500,3500}; // HT BIN
    // std::vector<int> cut4_ht = {3500,13000}; // HT BIN
    // std::vector<int> cut4_ht = {-1,13000}; // HT BIN
    // std::vector<int> cut5_ak4Pt = {250,250}; // 1st element for leading pt, 2nd element for secondary pt
    std::vector<int> cut5_ak4Pt = {-1,-1}; // 1st element for leading pt, 2nd element for secondary pt



    // THREE: set the mass regions (use a KEYNAME and keep track of what means what!!!)
    double S1_Node1 = 31.5;
    double S1_Node2 = 16.5;
    double SMAX_Node1 = 117.0;
    double SMAX_Node2 = 78.0;
    std::vector<double> SN_Nodes = {40.6, 50.7, 61.8, 73.9, 87.0, 101.6};
    MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsV03", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes);    



    // FOUR: set up the histogram template
    double luminosity = 50.0;

    TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetB_SoftDrop_Mass (GeV)", 400, 0, 200, 400, 0, 200);
    std::string varXAxis = "fatJetA_softDropMass";
    std::string varYAxis = "fatJetB_softDropMass";



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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_2dMassDistributionsAndCount.cc %s/%s__plotting_2dMassDistributionsAndCount.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_2dMassDistributionsAndCount.cc %s/%s__plotting_2dMassDistributionsAndCount.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_2dMassDistributionsAndCount", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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
    std::string cutToApplyBase = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[3]), cut3_ak8Pt, cut3_ak8Pt, cut4_ht[0], cut4_ht[1], cut5_ak4Pt[0], cut5_ak4Pt[1]);

    // Create the table of info
    std::ofstream table;
    std::string tableSaveName = "fatJet2dDist_" + MassCutsObject.GetName();
    tableSaveName += "__dbt" + cut2_ak8Dbt[0] + cut2_ak8Dbt[1] + "And" + cut2_ak8Dbt[2] + cut2_ak8Dbt[3];
    tableSaveName += Form("_ak8pt%d", cut3_ak8Pt);
    if (cut4_ht[1]==13000) tableSaveName += Form("_ht%dplus",cut4_ht[0]);
    else tableSaveName += Form("_ht%dto%d",cut4_ht[0],cut4_ht[1]);
    tableSaveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0], cut5_ak4Pt[1]);
    tableSaveName += ".csv";
    table.open(Form("%s/%s",outputDir.c_str(),tableSaveName.c_str()));
    table << "Mass Key: " << MassCutsObject.GetName() << "\n";
    for (size_t i = 0; i!=MassCutsObject.Get_S_Cuts().size(); ++i) table << "S_Cuts(" << i+1 << "): " << MassCutsObject.Get_S_Cuts()[i] << "\n";
    for (size_t i = 0; i!=MassCutsObject.Get_U_Cuts().size(); ++i) table << "U_Cuts(" << i+1 << "): " << MassCutsObject.Get_U_Cuts()[i] << "\n";
    for (size_t i = 0; i!=MassCutsObject.Get_D_Cuts().size(); ++i) table << "D_Cuts(" << i+1 << "): " << MassCutsObject.Get_D_Cuts()[i] << "\n";
    table << "\n";
    table << "Base Line Cuts: " << cutToApplyBase << "\n";
    table << "CUT_TYPE, COUNT, STAT_ERROR\n";

    // loop through the different mass regions
    for (size_t iMassRegion = 0; iMassRegion!=MassCutsObject.GetAllCuts().size()+1; ++iMassRegion){

        std::string cutToApply;
        if (iMassRegion != MassCutsObject.GetAllCuts().size()) cutToApply = cutToApplyBase + " && " + MassCutsObject.GetAllCuts()[iMassRegion];
        else cutToApply = cutToApplyBase;
        PlotEntry2D plotEntry = PlotEntry2D("plotEntry", hTemplate, varToPlot.c_str(), luminosity);
        std::string saveName = varToPlotSaveName + "_" + MassCutsObject.GetName();
        std::string cutType;
        if (iMassRegion == MassCutsObject.GetAllCuts().size()) cutType = "ALL";
        else if (iMassRegion < MassCutsObject.Get_S_Cuts().size() ) cutType = "Scut" + std::to_string(iMassRegion+1);
        else if (iMassRegion < MassCutsObject.Get_S_Cuts().size() + MassCutsObject.Get_U_Cuts().size() ) cutType = "Ucut" + std::to_string(iMassRegion +1 - MassCutsObject.Get_S_Cuts().size());
        else if (iMassRegion < MassCutsObject.Get_S_Cuts().size() + MassCutsObject.Get_U_Cuts().size() + MassCutsObject.Get_D_Cuts().size() ) cutType = "Dcut" + std::to_string(iMassRegion +1 - MassCutsObject.Get_S_Cuts().size() -  MassCutsObject.Get_U_Cuts().size());
        saveName += "_" + cutType;
        saveName += "__dbt" + cut2_ak8Dbt[0] + cut2_ak8Dbt[1] + "And" + cut2_ak8Dbt[2] + cut2_ak8Dbt[3];
        saveName += Form("_ak8pt%d", cut3_ak8Pt);
        if (cut4_ht[1]==13000) saveName += Form("_ht%dplus",cut4_ht[0]);
        else saveName += Form("_ht%dto%d",cut4_ht[0],cut4_ht[1]);
        saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0], cut5_ak4Pt[1]);
        std::string outputFile = outputDir + "/" + saveName + ".pdf";

        if (true){ // use to select which cuts we want to use
        // if (iMassRegion == MassCutsObject.GetAllCuts().size()){ // use to select which cuts we want to use


            
            // FIVE: sample info
            plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206); // ONLY NEED TO USE FOR HT1500-2500 BIN
            plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4); // DON"T NEED TO USE FOR HT3500+ BIN
            plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);

            // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
            // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
            // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
            // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

            // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
            // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH50p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
            // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
            // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);

            // SIX: plot & table aesthetics
            Plotter plot = Plotter({plotEntry});
            plot.AddLatex(luminosity);
            // plot.SetLogZ();
            table << cutType << "," << plotEntry.GetNumberOfEventsAfterCuts() << "," << plotEntry.GetNumberOfEventsAfterCutsStatError() << "\n";
            // plot.Save2D(outputFile.c_str()); // no extra lines
            plot.Save2D(outputFile.c_str(), MassCutsObject); // with extra lines



        } // closes 'if' we select the cuts we want to do! 
    } // closes loop through different mass configurations
    table.close();
    return 0;
} 