//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <map>
#include <fstream>
#include <sys/stat.h>

//ROOT HEADERS
#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TFile.h>

//RAL PARTICLE HEADERS
#include "PlotEntry2D.hh"
#include "Plotter.hh"
#include "MassRegionCuts.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "MacrosOnCondor.h"

//  CREATE THE HISTOGRAMS THAT WE USE TO DO ANALYSIS IN 1D

void CreateHistograms(std::map<std::string,TH1D*>& h_, const std::vector<std::vector<std::string>>& cut2_ak8Dbt, const std::vector<std::vector<int>>& cut4_ht, const unsigned int& numberOfSegments)
{
    unsigned int numberOfBins = cut4_ht.size() * numberOfSegments;

    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        
        std::string histogramName = "";
        if (cut2_ak8Dbt[iCut2].size() == 4) histogramName += "dbt" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "And" + cut2_ak8Dbt[iCut2][2] + cut2_ak8Dbt[iCut2][3];
        if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP") histogramName += "dbtDiagUp" + cut2_ak8Dbt[iCut2][1];

        h_[Form("S_%s", histogramName.c_str())] = new TH1D(Form("S_%s", histogramName.c_str()), ";Search Region Bin Number;Events", numberOfBins, 0, numberOfBins);
        h_[Form("U_%s", histogramName.c_str())] = new TH1D(Form("U_%s", histogramName.c_str()), ";Search Region Bin Number;Events", numberOfBins, 0, numberOfBins);
        h_[Form("D_%s", histogramName.c_str())] = new TH1D(Form("D_%s", histogramName.c_str()), ";Search Region Bin Number;Events", numberOfBins, 0, numberOfBins);
        
    } // closes loop through DBT configurations
}


void WriteHistograms(const std::map<std::string,TH1D*> & h_, const std::string& outputFile)
{
   TFile * outFile = new TFile(outputFile.c_str(),"RECREATE");
   for ( auto & h : h_ ){
        h.second->Write();
    }
   outFile -> Close();
   delete outFile;
}


int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2018_01_11_CMSSW_8_0_29_dbtV4/MassCutsV05/WJets/controls/"; // where we are going to save the output plots (should include the samples name + binning maybe)



    // TWO: set the cut params.
    std::vector<std::vector<std::string>> cut2_ak8Dbt;
    // cut2_ak8Dbt.push_back({"DIAG_UP", "Loose"}); // TAG: Top Diagnol Corner Crossing Axis at...--> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // cut2_ak8Dbt.push_back({"Off","Loose","Off","Loose"}); // ANTI: 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    cut2_ak8Dbt.push_back({"Loose","Med2","Off","IDBTCv21"}); // CONTROL_A: 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    cut2_ak8Dbt.push_back({"Off","IDBTCv21","Loose","Med2"}); // CONTROL_B: 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    int cut3_ak8Pt = 300;
    std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,99999} }; // HT BIN
    std::vector<int> cut5_ak4Pt = {300,-1}; // 1st element for leading pt, 2nd element for secondary pt
    // std::vector<int> cut5_ak4Pt = {-1,-1}; // 1st element for leading pt, 2nd element for secondary pt



    // THREE: set the mass regions (use a KEYNAME and keep track of what means what!!!!)
    double S1_Node1 = 33.0;
    double S1_Node2 = 17.6;
    double SMAX_Node1 = 168.5;
    double SMAX_Node2 = 115.04;
    std::vector<double> SN_Nodes = {42.5, 52.9, 64.2, 76.4, 89.5, 103.5, 118.4, 134.2, 150.9};
    MassRegionCuts MassCutsObject = MassRegionCuts("MassCutsV05", S1_Node1, S1_Node2, SMAX_Node1, SMAX_Node2, SN_Nodes);



    // FOUR: set up the luminosity
    double luminosity = 35.867; // 2016 DATASET



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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_create.cc %s/%s__histos_create.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_create.cc %s/%s__histos_create.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "histos_create", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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
    unsigned int numberOfSegments = MassCutsObject.Get_SN_Nodes().size() + 1;
    std::map<std::string, TH1D*> h_;
    CreateHistograms(h_, cut2_ak8Dbt, cut4_ht, numberOfSegments);

    unsigned int numberOfCutsForCodeTodo = (MassCutsObject.Get_SN_Nodes().size() + 1) * cut4_ht.size() * cut2_ak8Dbt.size() * 3;
    unsigned int counter = 1;
    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iMassRegion = 0; iMassRegion!=3*numberOfSegments; ++iMassRegion){ // REGULAR METHOD
        // for (size_t iMassRegion = numberOfSegments; iMassRegion!=3*numberOfSegments; ++iMassRegion){ // TO AVOID LOOKING IN THE S REGION!!! (DATA)

            std::string histogramName = "";
            if (cut2_ak8Dbt[iCut2].size() == 4) histogramName += "dbt" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "And" + cut2_ak8Dbt[iCut2][2] + cut2_ak8Dbt[iCut2][3];
            if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP") histogramName += "dbtDiagUp" + cut2_ak8Dbt[iCut2][1];

            if (iMassRegion < numberOfSegments) histogramName = "S_" + histogramName;
            else if (iMassRegion < 2*numberOfSegments) histogramName = "U_" + histogramName;
            else histogramName = "D_" + histogramName;

            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){

                std::string dbtCut = "";
                if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP")
                    dbtCut = Form("fatJetA_doubleBtagDiscrim >= (-1.0 * fatJetB_doubleBtagDiscrim + 1.0 + %f) ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]) );
                if (cut2_ak8Dbt[iCut2].size() == 4)
                    dbtCut = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]) );
                std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", dbtCut.c_str(), cut3_ak8Pt, cut3_ak8Pt, cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[0], cut5_ak4Pt[1]);

                cutToApply += " && " + MassCutsObject.GetAllCuts()[iMassRegion];
                TH2D hTemplate = TH2D("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);fatJetB_SoftDrop_Mass (GeV)", 400, 0, 200, 400, 0, 200);
                std::string varToPlot = "fatJetB_softDropMass:fatJetA_softDropMass";
                PlotEntry2D plotEntry = PlotEntry2D("plotEntry", hTemplate, varToPlot.c_str(), luminosity);

                //////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////////////////////////
                // FIVE: sample info
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500_ht1500plus/flatTree.root", cutToApply.c_str(), 1206);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000_ht1500plus/flatTree.root", cutToApply.c_str(), 120.4);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf_ht1500plus/flatTree.root", cutToApply.c_str(), 25.25);

                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO_ht1500plus/flatTree.root", cutToApply.c_str(), 831.76);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/ZJets_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);
                plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/WJets_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);

                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH30p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH50p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);

                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH30p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH50p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH70p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);

                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/mH70p0_mSusy2400p0_ratio0p99_splitting0p1_QCUT30/flatTree.root", cutToApply.c_str(), 0.002*0.85*0.85);
                
                // 2016 GOLDEN JSON DATASET
                // plotEntry.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApply.c_str());

                //////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////////////////////////

                int binToFill = iCut4 * numberOfSegments + iMassRegion % numberOfSegments + 1;
                h_[histogramName.c_str()]->SetBinContent(binToFill, plotEntry.GetNumberOfEventsAfterCuts());
                h_[histogramName.c_str()]->SetBinError(binToFill, plotEntry.GetNumberOfEventsAfterCutsStatError());  


                // // this section creates plots to check we trust what is going on
                // std::string plotSaveName = outputDir + "/plot_" + histogramName + "_binNum" + std::to_string(binToFill) + ".pdf";
                // Plotter plot = Plotter({plotEntry});
                // plot.AddLatex(luminosity);
                // plot.Save2D(plotSaveName.c_str(), MassCutsObject); // with the grid
                // plot.Save2D(plotSaveName.c_str()); // without the grid


                std::cout << "DONE " << counter << " of " << numberOfCutsForCodeTodo << " fills" << std::endl;
                std::cout << std::endl;
                counter++;

            } // closes loop through the HT bins
        } // closes loop through S,U,D mass regions
    } // closes loop through DBT configurations

    std::string saveName = MassCutsObject.GetName();
    saveName += Form("_ak8pt%d", cut3_ak8Pt);
    saveName += "_ht";
    for (size_t i = 0; i < cut4_ht.size(); ++i) saveName += Form("%dx", cut4_ht[i][0]);
    saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0], cut5_ak4Pt[1]);
    saveName += Form("_lumi%.0f", luminosity);
    saveName += ".root";
    std::string outputFile = outputDir + "/" + saveName;
    WriteHistograms(h_, outputFile);
    std::cout << "OutputFile is here: " << outputFile << std::endl;
    return 0;
}