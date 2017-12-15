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

// GET TABLES REQUIRED FOR TTJETS MC VALIDATION

int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_10_02/TEST_E/MassCutsQxyV01/data/fullCutsNoAK4PT_tagloose/"; // where we are going to save the output plots (should include the samples name + binning maybe)



    // TWO: set the cut params.
    // std::vector<std::string> cut2_ak8Dbt = {"Med2","Max","Med2","Max"}; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    std::vector<std::string> cut2_ak8Dbt = {"Loose","Max","Loose","Max"}; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<std::string> cut2_ak8Dbt = {"Off","Loose","Off","Loose"}; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    int cut3_ak8Pt = 300;
    std::vector<int> cut4_ht = {1500,2500}; // HT BIN
    std::vector<int> cut5_ak4Pt = {-1,-1}; // 1st element for leading pt, 2nd element for secondary pt



    // THREE: set the mass regions (use a KEYNAME and keep track of what means what!!!)
    double S1_Node1 = 182.0;
    double S1_Node2 = 138.0;
    double SMAX_Node1 = 200.0;
    double SMAX_Node2 = 170.0;
    std::vector<double> SN_Nodes = {182.0};
    MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsQxyV01", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes);



    // FOUR: set up the histogram template
    double luminosity = 35.867; // 2016 DATASET (DATA PLOTS SHOULD NOT BE USING THIS OBJECT)
    TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetB_SoftDrop_Mass (GeV)", 300, 0, 300, 300, 0, 300);
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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/table_QXY.cc %s/%s__table_QXY.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/table_QXY.cc %s/%s__table_QXY.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "table_QXY", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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
    // check that the MassRegionCuts object is only 1 true element
    size_t totalNumberOfCuts = MassCutsObject.GetAllCuts().size();
    if (totalNumberOfCuts != 6){
        std::cout << "Have not provided the right kind of MassRegionCuts object for this study...Exiting..." << std::endl;
        return 1;
    }


    std::string varToPlot = varYAxis + ":" + varXAxis;
    std::string varToPlotSaveName = varYAxis + "VS" + varXAxis;
    std::string cutToApplyBase = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[3]), cut3_ak8Pt, cut3_ak8Pt, cut4_ht[0], cut4_ht[1], cut5_ak4Pt[0], cut5_ak4Pt[1]);

    // Create the table of info
    std::ofstream table;
    std::string tableSaveName = "fatJet2dDist_" + MassCutsObject.GetName();
    tableSaveName += "__dbt" + cut2_ak8Dbt[0] + cut2_ak8Dbt[1] + "And" + cut2_ak8Dbt[2] + cut2_ak8Dbt[3];
    tableSaveName += Form("_ak8pt%d", cut3_ak8Pt);
    if (cut4_ht[1]==99999) tableSaveName += Form("_ht%dplus",cut4_ht[0]);
    else tableSaveName += Form("_ht%dto%d",cut4_ht[0],cut4_ht[1]);
    tableSaveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0], cut5_ak4Pt[1]);
    tableSaveName += Form("_lumi%.0f", luminosity);
    tableSaveName += ".csv";
    table.open(Form("%s/%s",outputDir.c_str(),tableSaveName.c_str()));
    table << "luminosity: " << luminosity << "fb^-1\n";
    table << "Mass Key: " << MassCutsObject.GetName() << "\n";
    table << "Q_Cut " << MassCutsObject.Get_S_Cuts()[1] << "\n";
    table << "X_Cut " << MassCutsObject.Get_U_Cuts()[1] << "\n";
    table << "Y_Cut " << MassCutsObject.Get_D_Cuts()[1] << "\n";
    table << "\n";
    table << "Base Line Cuts: " << cutToApplyBase << "\n";
    table << "CUT_TYPE, COUNT, STAT_ERROR\n";

    // loop through the different mass regions
    for (size_t iMassRegion = 0; iMassRegion!=totalNumberOfCuts; ++iMassRegion){

        if (iMassRegion % 2 == 0) continue; // Have to do this due to how we are using a MassRegionCuts object

        std::string cutToApply;
        cutToApply = cutToApplyBase + " && " + MassCutsObject.GetAllCuts()[iMassRegion];
        PlotEntry2D plotEntry = PlotEntry2D("plotEntry", hTemplate, varToPlot.c_str(), luminosity);
        std::string saveName = varToPlotSaveName + "_" + MassCutsObject.GetName();
        std::string cutType;
        if (iMassRegion < MassCutsObject.Get_S_Cuts().size() ) cutType = "Qcut";
        else if (iMassRegion < 2 * MassCutsObject.Get_S_Cuts().size() ) cutType = "Xcut";
        else if (iMassRegion < 3 * MassCutsObject.Get_S_Cuts().size() ) cutType = "Ycut";
        saveName += "_" + cutType;
        saveName += "__dbt" + cut2_ak8Dbt[0] + cut2_ak8Dbt[1] + "And" + cut2_ak8Dbt[2] + cut2_ak8Dbt[3];
        saveName += Form("_ak8pt%d", cut3_ak8Pt);
        if (cut4_ht[1]==99999) saveName += Form("_ht%dplus",cut4_ht[0]);
        else saveName += Form("_ht%dto%d",cut4_ht[0],cut4_ht[1]);
        saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0], cut5_ak4Pt[1]);
        std::string outputFile = outputDir + "/" + saveName + ".pdf";



        // FIVE: sample info
        // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
        // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
        // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);

        // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
        // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/ZJets_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);
        // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/WJets_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);

        plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHt_Run2016_goldenJson/flatTree.root", cutToApply.c_str());



        // SIX: plot & table aesthetics
        Plotter plot2d = Plotter({plotEntry});
        plot2d.AddLatex(luminosity);
        table << cutType << "," << plotEntry.GetNumberOfEventsAfterCuts() << "," << plotEntry.GetNumberOfEventsAfterCutsStatError() << "\n";
        plot2d.Save2D(outputFile.c_str(), MassCutsObject); // with extra lines



    } // closes loop through different mass configurations
    table.close();
    return 0;
}