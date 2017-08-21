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

// COMPARE AMOUNT OF SIGNAL IN DIFFERENT 2*DBT REGIONS
int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_14/Signal/DBT_Regions/mH90_mSusy2000/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params, each combination = new plot
    // std::vector<int> cut3_ak8Pt = {-1};
    std::vector<std::vector<int>> cut4_ht = { {-1,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    std::vector<int> cut3_ak8Pt = {300};
    // std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    std::vector<std::vector<std::string>> cut2_ak8Dbt; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    cut2_ak8Dbt.push_back({"Off","Max","Off","Max"}); // this should always be ALL DBT space
    cut2_ak8Dbt.push_back({"Med2","Max","Med2","Max"});
    cut2_ak8Dbt.push_back({"Off","Loose","Off","Loose"});
    cut2_ak8Dbt.push_back({"Med2","Max","Off","Loose"});
    // cut2_ak8Dbt.push_back({"Off","Loose","Med2","Max"});
    cut2_ak8Dbt.push_back({"Med2","Max","Off","IDBTCv1"});
    cut2_ak8Dbt.push_back({"Off","Loose","Off","IDBTCv1"});



    // THREE: plot histogram settings
    double luminosity = 50.0; // note that this value doesn't matter IF you normalise later

    TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 400, -1, 1, 400, -1, 1);
    // TH2D hTemplate = TH2D("hTemplate", ";fatJetA DBT_Score;fatJetB DBT_Score", 50, -1, 1, 50, -1, 1);
    std::string varXAxis = "fatJetA_doubleBtagDiscrim";
    std::string varYAxis = "fatJetB_doubleBtagDiscrim";



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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/table_dbtRegionSignalContamination.cc %s/%s__table_dbtRegionSignalContamination.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/table_dbtRegionSignalContamination.cc %s/%s__table_dbtRegionSignalContamination.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "table_dbtRegionSignalContamination", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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

    std::ofstream table;
    table.open( Form("%s/eventCount_%.0finvFb.csv", outputDir.c_str(), luminosity) );
    table << "Luminosity = " << luminosity << "fb^-1," << "\n";
    table << "Cuts Applied, Number of Events, Fraction of all DBT space\n";

    double numberEventsAllDbtSpace = 0.0;
    for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
        for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
            for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){
                for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){

                    std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
      


                    // FOUR: sample to use
                    // PlotEntry2D plotEle = PlotEntry2D("mH30_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // plotEle.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);

                    // PlotEntry2D plotEle = PlotEntry2D("mH50_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // plotEle.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);

                    // PlotEntry2D plotEle = PlotEntry2D("mH70_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // plotEle.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);

                    // PlotEntry2D plotEle = PlotEntry2D("mH90_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // plotEle.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);

                    // PlotEntry2D plotEle = PlotEntry2D("mH30_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // plotEle.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

                    // PlotEntry2D plotEle = PlotEntry2D("mH50_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // plotEle.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

                    // PlotEntry2D plotEle = PlotEntry2D("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // plotEle.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

                    PlotEntry2D plotEle = PlotEntry2D("mH90_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    plotEle.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_07_14_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);




                    // FIVE: plot&table aesthetics
                    if (iCut2==0) numberEventsAllDbtSpace = plotEle.GetNumberOfEventsAfterCuts();
                    table << cutToApply << ", ";
                    table << plotEle.GetNumberOfEventsAfterCuts() << ", ";
                    table << plotEle.GetNumberOfEventsAfterCuts() / numberEventsAllDbtSpace << "\n";

                    Plotter plot = Plotter({plotEle});
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
                    // plot.SetLogZ();
                    // outputFile = outputDir + "/" + saveName + "__logY.pdf";
                    // plot.Save2D(outputFile.c_str());



                } // closes loop through cut 2 states
            } // closes loop through cut 5 states
        } // closes loop through cut 4 states
    } // closes loop through cut 3 states
    table.close();
	return 0;
}