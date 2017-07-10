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

// COMPARE SIGNAL AND BACKGROUND FOR DIFFERENT CUTS
int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_05/testingMacros/testing_investigateEventSelectionCuts/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params, each combination = new plot
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max (options--> "Off", "Loose", "Med", "Tight", "Max")
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Max","Med","Max"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max (options--> "Off", "Loose", "Med", "Tight", "Max")
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: plot histogram settings
    double luminosity = 50.0; // note that this value doesn't matter IF you normalise later
    std::string varToPlot = "fatJetA_softDropMass";    
    TH1D hTemplate("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);events / 4 GeV", 50, 0, 200);



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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_investigateEventSelectionCuts.cc %s/%s__plotting_investigateEventSelectionCuts.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_investigateEventSelectionCuts.cc %s/%s__plotting_investigateEventSelectionCuts.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_investigateEventSelectionCuts", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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

    // Create the table
    std::ofstream table;
    table.open(Form("%s/aSignificanceTable_%.0finvFb.csv", outputDir.c_str(), luminosity));
    table << "*SIGNIFICANCE TABLE* significance nDifference (nSignal|nBackground)" << "\n";
    table << "Luminosity = " << luminosity << "fb^-1," << "\n";
    table << "*CUTS USED*,";

    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::vector<PlotEntry> plotEntrySignalVec;
                    std::vector<PlotEntry> plotEntryBackgroundVec;
                    std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);


      
                    // FOUR: samples to use
                    // PlotEntry mH30_mSusy800 = PlotEntry("mH30_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // plotEntrySignalVec.push_back(mH30_mSusy800);

                    // PlotEntry mH50_mSusy800 = PlotEntry("mH50_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH50_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // plotEntrySignalVec.push_back(mH50_mSusy800);

                    // PlotEntry mH70_mSusy800 = PlotEntry("mH70_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // plotEntrySignalVec.push_back(mH70_mSusy800);

                    // PlotEntry mH90_mSusy800 = PlotEntry("mH90_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // plotEntrySignalVec.push_back(mH90_mSusy800);

                    PlotEntry mH30_mSusy2000 = PlotEntry("mH30_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    mH30_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    plotEntrySignalVec.push_back(mH30_mSusy2000);

                    PlotEntry mH50_mSusy2000 = PlotEntry("mH50_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    mH50_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    plotEntrySignalVec.push_back(mH50_mSusy2000);

                    PlotEntry mH70_mSusy2000 = PlotEntry("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    mH70_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    plotEntrySignalVec.push_back(mH70_mSusy2000);

                    PlotEntry mH90_mSusy2000 = PlotEntry("mH90_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    mH90_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    plotEntrySignalVec.push_back(mH90_mSusy2000);

                    PlotEntry QCD = PlotEntry("QCD", hTemplate, varToPlot.c_str(), luminosity);
                    QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT700to1000/flatTree.root", cutToApply.c_str(), 6802);
                    QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
                    QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
                    QCD.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);
                    plotEntryBackgroundVec.push_back(QCD);

                    // PlotEntry ttbarLO = PlotEntry("ttbar LO", hTemplate, varToPlot.c_str(), luminosity);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply.c_str()), 831.76);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 2.6665344485, false);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 1.0980821984, false);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.1987479092, false);
                    // ttbarLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.002368412585, false);
                    // plotEntryBackgroundVec.push_back(ttbarLO);

                    PlotEntry ttbarNLO = PlotEntry("ttbar NLO", hTemplate, varToPlot.c_str(), luminosity);
                    ttbarNLO.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);
                    plotEntryBackgroundVec.push_back(ttbarNLO);

                    PlotEntry ZJets = PlotEntry("Z+Jets", hTemplate, varToPlot.c_str(), luminosity);
                    ZJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/ZJetsToQQ_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);
                    plotEntryBackgroundVec.push_back(ZJets);

                    PlotEntry WJets = PlotEntry("W+Jets", hTemplate, varToPlot.c_str(), luminosity);
                    WJets.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/WJetsToQQ_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);
                    plotEntryBackgroundVec.push_back(WJets);




                    // FIVE: plot aesthetics
                    Plotter plot = Plotter(plotEntrySignalVec, plotEntryBackgroundVec);
                    plot.AddLegend(0.55, 0.88, 0.47, 0.87); // top right (extra wide 8)
                    plot.AddLatex(luminosity);



                    std::string saveName = varToPlot;
                    std::string cutName = "dbt" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "And" + cut2_ak8Dbt[iCut2][2] + cut2_ak8Dbt[iCut2][3];
                    cutName += Form("_ak8pt%d", cut3_ak8Pt[iCut3]);
                    if (cut4_ht[iCut4][1]==13000) cutName += Form("_ht%dplus",cut4_ht[iCut4][0]);
                    else cutName += Form("_ht%dto%d",cut4_ht[iCut4][0],cut4_ht[iCut4][1]);
                    cutName += Form("_ak4pt%dn%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    saveName += "__" + cutName;
                    // saveName += "APPEND_SAVE_NAME"; // OPTIONAL: can manually append the automatic savename
                    // saveName = "MANUAL_SAVE_NAME"; // OPTIONAL: can manually overwrite the automatic savename
                    std::string outputFile = outputDir + "/" + saveName + ".pdf";
                    plot.Save(outputFile.c_str());
                    plot.SetLogY();
                    outputFile = outputDir + "/" + saveName + "__logY.pdf";
                    plot.Save(outputFile.c_str());

                    if (iCut2==0 && iCut3==0 && iCut4==0 && iCut5==0){
                	    for (size_t i = 0; i < plotEntrySignalVec.size(); ++i) table << plotEntrySignalVec[i].GetPlotEntryName().c_str() << ",";
					    table << "\n";
                    }
                    table << Form("%s",cutName.c_str()) << ",";
                    double nBackgroundObserved = 0.0;
                    for (size_t i = 0; i < plotEntryBackgroundVec.size(); ++i) nBackgroundObserved += plotEntryBackgroundVec[i].GetNumberOfEventsAfterCuts();
                    for (size_t i = 0; i < plotEntrySignalVec.size(); ++i){
                        double nSignalObserved = plotEntrySignalVec[i].GetNumberOfEventsAfterCuts();
                        double significance = nSignalObserved / sqrt(nBackgroundObserved);
                        double nDifferenceObserved = nSignalObserved - nBackgroundObserved;
                        table << Form("%.1f & %.1f (%.1f | %.1f),", significance, nDifferenceObserved, nSignalObserved, nBackgroundObserved);
                    }
                    table << "\n";          



                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states
    table.close();
	return 0;
}