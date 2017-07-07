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
#include "Plotter.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "MacrosOnCondor.h"

// CREATE RATIOS FOR DIFFERENT 2*DBT SPACE
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_05/testingMacros/testing_dbtRatio/"; // where we are going to save the output plots (should include the samples name + binning maybe)



    // TWO: set the cut params.
    // cut2_ak8Dbt: first element is the numerator, loop through the other elements to create the denominators. One plot per denominator.
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Max","Med","Max"}, {"Loose","Tight","Loose","Med"}, {"Loose","Tight","Off","Med"}, {"Loose","Tight","Off","Max"}, {"Off","Tight","Off","Med"}, {"Off","Tight","Off","Max"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Max","Med","Max"}, {"Off","Loose","Off","Loose"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    int cut3_ak8Pt = 300;
    std::vector<int> cut4_ht = {1500,2500}; // HT BIN
    std::vector<int> cut5_ak4Pt = {250,250}; // 1st element for leading pt, 2nd element for secondary pt



    // THREE: set up the histogram template 
    std::string varToPlot = "fatJetA_softDropMass";
    // std::vector<double> abcdMassSet = {0.0, 10.0, 30.0, 60.0, 90.0, 200.0};
    // TH1F hTemplate = TH1F("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);DBT Pass / inv DBT", abcdMassSet.size()-1, &(abcdMassSet)[0]);
    // TH1F hTemplate = TH1F("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);DBT Pass / inv DBT", 50, 0, 200);
    // TH1F hTemplate = TH1F("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);DBT Pass / inv DBT", 25, 0, 200);
    TH1F hTemplate = TH1F("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);DBT Pass / inv DBT", 10, 0, 200);



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

    double dummyLuminosity = 50.0; // note that the luminosity value doesn't matter as we will later take a ratio, it just needs to be consistent
    std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[0][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[0][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[0][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[0][3]), cut3_ak8Pt, cut3_ak8Pt, cut4_ht[0], cut4_ht[1], cut5_ak4Pt[0], cut5_ak4Pt[1]);


   
    // FOUR: numerator info
    PlotEntry numerator = PlotEntry("numerator", hTemplate, varToPlot.c_str(), dummyLuminosity);
    
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply.c_str()), 831.76);
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 2.6665344485, false);
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 1.0980821984, false);
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.1987479092, false);
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.002368412585, false);
    
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", cutToApply.c_str(), 2.6665344485, false);
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", cutToApply.c_str(), 1.0980821984, false);
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", cutToApply.c_str(), 0.1987479092, false);
    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", cutToApply.c_str(), 0.002368412585, false);

    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);

    // numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

    numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT700to1000/flatTree.root", cutToApply.c_str(), 6802);
    numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
    numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
    numerator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);




    // Loop through the specified denominator cut combinations
    std::vector<PlotEntry> plotDivVec;
    for (size_t iCut2 = 1; iCut2 < cut2_ak8Dbt.size(); ++iCut2){

        cutToApply = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]), cut3_ak8Pt, cut3_ak8Pt, cut4_ht[0], cut4_ht[1], cut5_ak4Pt[0], cut5_ak4Pt[1]);
        std::string legendName = "invDBT " + cut2_ak8Dbt[iCut2][0] + "-" + cut2_ak8Dbt[iCut2][1] + ":" + cut2_ak8Dbt[iCut2][2]+ "-" + cut2_ak8Dbt[iCut2][3];
        PlotEntry denominator = PlotEntry("denominator", hTemplate, varToPlot.c_str(), dummyLuminosity); 



        // FIVE: sample info
        
        // ttbarLO
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply.c_str()), 831.76);
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 2.6665344485, false);
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 1.0980821984, false);
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.1987479092, false);
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.002368412585, false);
        
        // ttbarLO - WARNING: USE FOR SHAPE ONLY - block below is missing the inclusive snippet
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", cutToApply.c_str(), 2.6665344485);
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", cutToApply.c_str(), 1.0980821984);
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", cutToApply.c_str(), 0.1987479092);
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", cutToApply.c_str(), 0.002368412585);

        // ttbarNLO
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_NLO/flatTree.root", cutToApply.c_str(), 831.76);

        // QCD
        denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT700to1000/flatTree.root", cutToApply.c_str(), 6802);
        denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
        denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
        denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);

        // SIGNAL(s)
        // denominator.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);

        PlotEntry divide = PlotEntry(legendName.c_str(), numerator, denominator);
        plotDivVec.push_back(divide);



    } // closes loop through cut 2 states



    // SIX: plot aesthetics
    Plotter plot = Plotter(plotDivVec);
    // plot.AddLegend(0.65, 0.88, 0.77, 0.87); // top right (wide 2)
    // plot.AddLegend(0.65, 0.88, 0.70, 0.87); // top right (wide 4)
    // plot.AddLegend(0.43, 0.88, 0.77, 0.87); // top right (extra wide 2)
    // plot.AddLegend(0.43, 0.88, 0.70, 0.87); // top right (extra wide 4)
    plot.AddLegend(0.43, 0.88, 0.20, 0.30); // bottom right (extra wide 2)
    // plot.AddLegend(0.20, 0.35, 0.77, 0.87); // top left (2)
    plot.AddLatex();
    plot.SetErrors();



    std::string saveName = varToPlot;
    saveName += "__dbtNumerator" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1] + "And" + cut2_ak8Dbt[0][2] + cut2_ak8Dbt[0][3];
    saveName += Form("_ak8pt%d", cut3_ak8Pt);
    if (cut4_ht[1]==13000) saveName += Form("_ht%dplus",cut4_ht[0]);
        else saveName += Form("_ht%dto%d",cut4_ht[0],cut4_ht[1]);
    saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0], cut5_ak4Pt[1]);
    // saveName += "APPEND_SAVE_NAME"; // OPTIONAL: can manually append the automatic savename
    // saveName = "MANUAL_SAVE_NAME"; // OPTIONAL: can manually overwrite the automatic savename
    std::string outputFile = outputDir + "/" + saveName + ".pdf";
	plot.Save(outputFile.c_str());

	return 0;
}