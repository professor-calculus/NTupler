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

// COMPARE A SET OF DIFFERENT CUTS ON A CHOSEN DATA SET VARIABLE
int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_05/testingMacros/testing_lheHTSTitch2/"; // where we are going to save the output plots (should include the samples name + binning maybe)



    // TWO: plot histogram settings
    double luminosity = 50.0; // note that this value doesn't matter IF you normalise later    
    // std::string varToPlot = "ht";
    // TH1D hTemplate("hTemplate", ";H_{T} (GeV);events / bin", 100, 0, 4000);
    std::string varToPlot = "lheHT";
    TH1D hTemplate("hTemplate", ";LHE H_{T} (GeV);events / bin", 100, 0, 4000);



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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_checkTtbarHtStitching.cc %s/%s__plotting_checkTtbarHtStitching.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_checkTtbarHtStitching.cc %s/%s__plotting_checkTtbarHtStitching.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_checkTtbarHtStitching", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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

    for (double lheHT = 700.0; lheHT < 900.1; lheHT = lheHT + 100.0){

        std::vector<PlotEntry> plotEntryVec;
        std::vector<PlotEntry> plotEntryStack;



        // FOUR: sample info
        PlotEntry ttbar_incNOCUT = PlotEntry("Inclusive", hTemplate, varToPlot.c_str(), luminosity);
        ttbar_incNOCUT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", "", 831.76);
        plotEntryVec.push_back(ttbar_incNOCUT);

        PlotEntry ttbar_ht2500toInf = PlotEntry( Form("HT2500toInf (lheHT>%.0f)", lheHT), hTemplate, varToPlot.c_str(), luminosity);
        ttbar_ht2500toInf.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("lheHT>=%f", lheHT), 0.002368412585);
        plotEntryStack.push_back(ttbar_ht2500toInf);

        PlotEntry ttbar_ht1200to2500 = PlotEntry( Form("HT1200to2500 (lheHT>%.0f)", lheHT), hTemplate, varToPlot.c_str(), luminosity);
        ttbar_ht1200to2500.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("lheHT>=%f", lheHT), 0.1987479092);
        plotEntryStack.push_back(ttbar_ht1200to2500);

        PlotEntry ttbar_ht800to1200 = PlotEntry( Form("HT800to1200 (lheHT>%.0f)", lheHT), hTemplate, varToPlot.c_str(), luminosity);
        ttbar_ht800to1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("lheHT>=%f", lheHT), 1.0980821984);
        plotEntryStack.push_back(ttbar_ht800to1200);

        PlotEntry ttbar_ht600to800 = PlotEntry( Form("HT600to800 (lheHT>%.0f)", lheHT), hTemplate, varToPlot.c_str(), luminosity);
        ttbar_ht600to800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("lheHT>=%f", lheHT), 2.6665344485);
        plotEntryStack.push_back(ttbar_ht600to800);

        PlotEntry ttbar_inc = PlotEntry( Form("Inclusive (lheHT<%.0f)", lheHT), hTemplate, varToPlot.c_str(), luminosity);
        ttbar_inc.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("lheHT<%f", lheHT), 831.76);
        plotEntryStack.push_back(ttbar_inc);



        // FIVE: plot aesthetics
        Plotter plot = Plotter(plotEntryVec, plotEntryStack);
        plot.AddLegend(0.55, 0.88, 0.62, 0.87); // top right (wide 6)
        plot.AddLatex(luminosity);
        plot.SetLogY();
        std::string outputFile = outputDir + "/ttbar_" + varToPlot + "_lheHtCut" + Form("%.0f", lheHT) + ".pdf"; 
        plot.Save(outputFile.c_str());



    } // closes loop through different lheHT cut values
	return 0;
}