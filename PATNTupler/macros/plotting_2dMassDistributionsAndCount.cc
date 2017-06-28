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
#include "PlotEntry2D.hh"
#include "Plotter.hh"
#include "MassRegionCuts.hh"
#include "DoubleBTagWPs.h"

int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_06_27/massScatter/doHelpDrawLines/"; // where we are going to save the output plots (should include the samples name + binning maybe)


    // TWO: set the cut params.
    std::vector<std::string> cut2_ak8Dbt = {"Tight","Max","Med","Max"}; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    // std::vector<std::string> cut2_ak8Dbt = {"Off","Max","Off","Med"}; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    // std::vector<std::string> cut2_ak8Dbt = {"Off","Loose","Off","Loose"}; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    // std::vector<std::string> cut2_ak8Dbt = {"Off","Max","Off","Max"}; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    int cut3_ak8Pt = 300;
    // int cut3_ak8Pt = -1;
    std::vector<int> cut4_ht = {1500,2500}; // HT BIN
    // std::vector<int> cut4_ht = {-1,13000}; // HT BIN
    std::vector<int> cut5_ak4Pt = {250,250}; // 1st element for leading pt, 2nd element for secondary pt
    // std::vector<int> cut5_ak4Pt = {-1,-1}; // 1st element for leading pt, 2nd element for secondary pt


    // THREE: set the mass regions (use a KEYNAME and keep track of what means what!!!)
    double VHK_bandSize = 10.0;
    double S1_Node = 20.0;
    double SMAX_Node1 = 110.0;
    double SMAX_Node2 = 80.0;
    std::vector<double> SN_Nodes = {40.0, 60.0, 80.0};
    MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsVTEST", VHK_bandSize, S1_Node, SMAX_Node1, SMAX_Node2, SN_Nodes);    
    // // EXTRACTION: method1
    // std::cout << "Name of MassRegions Object: " << MassCutsObject.GetName() << std::endl;
    // std::cout << "V_Cuts: " << MassCutsObject.Get_V_Cuts() << std::endl;
    // std::cout << "H_Cuts: " << MassCutsObject.Get_H_Cuts() << std::endl;
    // std::cout << "K_Cuts: " << MassCutsObject.Get_K_Cuts() << std::endl;
    // std::cout << "O_Cuts: " << MassCutsObject.Get_O_Cuts() << std::endl;
    // for (size_t i = 0; i!=MassCutsObject.Get_S_Cuts().size(); ++i) std::cout << "S_Cuts(" << i+1 << "): " << MassCutsObject.Get_S_Cuts()[i] << std::endl;
    // std::cout << std::endl;
    // // EXTRACTION: method2
    // std::cout << "Name of MassRegions Object: " << MassCutsObject.GetName() << std::endl;
    // std::cout << "V_Cuts: " << MassCutsObject.GetAllCuts()[0] << std::endl;
    // std::cout << "H_Cuts: " << MassCutsObject.GetAllCuts()[1] << std::endl;
    // std::cout << "K_Cuts: " << MassCutsObject.GetAllCuts()[2] << std::endl;
    // std::cout << "O_Cuts: " << MassCutsObject.GetAllCuts()[3] << std::endl;
    // for (size_t i = 3; i!=MassCutsObject.GetAllCuts().size(); ++i) std::cout << "S_Cuts(" << i-3 << "): " << MassCutsObject.GetAllCuts()[i] << std::endl;
    // std::cout << std::endl;


    // FOUR: set up the histogram template
    TH2F hTemplate = TH2F("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);SecondaryBDiscFatJet_SoftDrop_Mass (GeV)", 400, 0, 140, 400, 0, 140);
    std::string varToPlot = "fatJetB_softDropMass:fatJetA_softDropMass";
    double luminosity = 50.0;


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::string cutToApplyBase = Form("fatJetA_doubleBtagDiscrim>%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[3]), cut3_ak8Pt, cut3_ak8Pt, cut4_ht[0], cut4_ht[1], cut5_ak4Pt[0], cut5_ak4Pt[1]);

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
    table << "V_Cuts: " << MassCutsObject.Get_V_Cuts() << "\n";
    table << "H_Cuts: " << MassCutsObject.Get_H_Cuts() << "\n";
    table << "K_Cuts: " << MassCutsObject.Get_K_Cuts() << "\n";
    table << "O_Cuts: " << MassCutsObject.Get_O_Cuts() << "\n";
    for (size_t i = 0; i!=MassCutsObject.Get_S_Cuts().size(); ++i) table << "S_Cuts(" << i+1 << "): " << MassCutsObject.Get_S_Cuts()[i] << "\n";
    table << "\n";
    table << "Base Line Cuts: " << cutToApplyBase << "\n";
    table << "CUT_TYPE, COUNT, STAT_ERROR\n";

    for (size_t iMassRegion = 0; iMassRegion!=MassCutsObject.GetAllCuts().size()+1; ++iMassRegion){

        std::string cutToApply;
        if (iMassRegion != MassCutsObject.GetAllCuts().size()) cutToApply = cutToApplyBase + " && " + MassCutsObject.GetAllCuts()[iMassRegion];
        else cutToApply = cutToApplyBase;
        PlotEntry2D plotEntry = PlotEntry2D("plotEntry", hTemplate, varToPlot.c_str(), luminosity);
        std::string saveName = "fatJet2dDist_" + MassCutsObject.GetName();
        std::string cutType;
        if (iMassRegion == 0) cutType = "Vcut";
        else if (iMassRegion == 1) cutType = "Hcut";
        else if (iMassRegion == 2) cutType = "Kcut";
        else if (iMassRegion == 3) cutType = "Ocut";
        else if (iMassRegion == MassCutsObject.GetAllCuts().size()) cutType = "ALL";
        else cutType = "Scut" + std::to_string(iMassRegion-3);
        saveName += "_" + cutType;
        saveName += "__dbt" + cut2_ak8Dbt[0] + cut2_ak8Dbt[1] + "And" + cut2_ak8Dbt[2] + cut2_ak8Dbt[3];
        saveName += Form("_ak8pt%d", cut3_ak8Pt);
        if (cut4_ht[1]==13000) saveName += Form("_ht%dplus",cut4_ht[0]);
        else saveName += Form("_ht%dto%d",cut4_ht[0],cut4_ht[1]);
        saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0], cut5_ak4Pt[1]);
        std::string outputFile = outputDir + "/" + saveName + ".pdf";

        // FOUR: sample info
        plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
        
        // FIVE: plot aesthetics
        Plotter plot = Plotter({plotEntry});
        plot.AddLatex(luminosity);
        // plot.SetLogZ();
        
        table << cutType << "," << plotEntry.GetNumberOfEventsAfterCuts() << "," << plotEntry.GetNumberOfEventsAfterCutsStatError() << "\n";
        plot.Save2D(outputFile.c_str());

    } // closes loop through different mass configurations
    table.close();
    return 0;
}