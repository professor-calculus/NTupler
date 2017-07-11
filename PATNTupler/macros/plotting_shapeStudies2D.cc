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

// 2D SCATTER. NORMALISE AND SUBTRACT (A - B) OR DIVIDE (A / B)
int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_10/plotting_QCDShapeVariations/preSel_ht1500to2500/ak4ptVariation/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params, element1 = A, element2 = B
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"}, {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max (options--> "Off", "Loose", "Med", "Tight", "Max")
    std::vector<int> cut3_ak8Pt = {-1, -1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,13000}, {-1,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1}, {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Loose","Off","Loose"}, {"Med","Max","Med","Max"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max (options--> "Off", "Loose", "Med", "Tight", "Max")
    // std::vector<int> cut3_ak8Pt = {-1, 300};
    // std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {2500,3500}, {3500,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut4_ht = { {1500,2500}, {1500,2500} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1}, {250,250} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: histogram settings
    double luminosity = 50.0;
    
    // TH2D hTemplate = TH2D("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);SecondaryBDiscFatJet_SoftDrop_Mass (GeV)", 400, 0, 200, 400, 0, 200);
    // TH2D hTemplate = TH2D("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);SecondaryBDiscFatJet_SoftDrop_Mass (GeV)", 200, 0, 200, 200, 0, 200);
    // TH2D hTemplate = TH2D("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);SecondaryBDiscFatJet_SoftDrop_Mass (GeV)", 100, 0, 200, 100, 0, 200);
    // TH2D hTemplate = TH2D("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);SecondaryBDiscFatJet_SoftDrop_Mass (GeV)", 50, 0, 200, 50, 0, 200);
    TH2D hTemplate = TH2D("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);SecondaryBDiscFatJet_SoftDrop_Mass (GeV)", 25, 0, 200, 25, 0, 200);
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
    
    if (cut2_ak8Dbt.size() != 2 || cut2_ak8Dbt.size() != cut3_ak8Pt.size() || cut3_ak8Pt.size() != cut4_ht.size() || cut4_ht.size() != cut5_ak4Pt.size()){
        std::cout << "stepXlabels not size=2 or not all same size, exiting..." << std::endl;
        return 1;
    }

    std::string runInstructionString(argv[1]);
    
    if (runInstructionString == "local"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_shapeStudies2D.cc %s/%s__plotting_shapeStudies2D.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_shapeStudies2D.cc %s/%s__plotting_shapeStudies2D.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_shapeStudies2D", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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
    std::vector<PlotEntry2D> plotEntryVec;
    for (size_t iCut = 0; iCut < 2; ++iCut){
        std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut][3]), cut3_ak8Pt[iCut], cut3_ak8Pt[iCut], cut4_ht[iCut][0], cut4_ht[iCut][1], cut5_ak4Pt[iCut][0], cut5_ak4Pt[iCut][1]);



        // FOUR: samples to use
        PlotEntry2D plot2d = PlotEntry2D("QCD", hTemplate, varToPlot.c_str(), luminosity);
        plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT700to1000/flatTree.root", cutToApply.c_str(), 6802);
        plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
        plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
        plot2d.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);
        plot2d.NormalisePlot();
        plotEntryVec.push_back(plot2d);


    
    } // closes loop through iCut

    std::string saveName = varToPlotSaveName;
    if (cut2_ak8Dbt[0] == cut2_ak8Dbt[1]) saveName += "__dbtConst" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1] + "And" + cut2_ak8Dbt[0][2] + cut2_ak8Dbt[0][3];
    else saveName += "__dbtVaryA" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1] + "And" + cut2_ak8Dbt[0][2] + cut2_ak8Dbt[0][3] + "B" + cut2_ak8Dbt[1][0] + cut2_ak8Dbt[1][1] + "And" + cut2_ak8Dbt[1][2] + cut2_ak8Dbt[1][3];
    if (cut3_ak8Pt[0] == cut3_ak8Pt[1]) saveName += Form("_ak8ptConst%d", cut3_ak8Pt[0]);
    else saveName += Form("_ak8ptVaryA%dB%d", cut3_ak8Pt[0], cut3_ak8Pt[1]);
    if (cut4_ht[0] == cut4_ht[1]) saveName += Form("_htConst%dto%d", cut4_ht[0][0], cut4_ht[0][1]);
    else saveName += Form("_htVaryA%dto%dB%dto%d", cut4_ht[0][0], cut4_ht[0][1], cut4_ht[1][0], cut4_ht[1][1]);
    if (cut5_ak4Pt[0] == cut5_ak4Pt[1]) saveName += Form("_ak4ptConst%dn%d", cut5_ak4Pt[0][0], cut5_ak4Pt[0][1]);
    else saveName += Form("_ak4ptVaryA%dn%dB%dn%d", cut5_ak4Pt[0][0], cut5_ak4Pt[0][1], cut5_ak4Pt[1][0], cut5_ak4Pt[1][1]);

    plotEntryVec[0].GetHistogram()->Add(plotEntryVec[1].GetHistogram(), -1);
    std::string saveName2 = "AminusB__" + saveName;
    Plotter plot2 = Plotter({plotEntryVec[0]});
    plot2.AddLatex();
    std::string outputFile2 = outputDir + "/" + saveName2 + ".pdf";
    plot2.Save2D(outputFile2.c_str());
    plotEntryVec[0].GetHistogram()->Add(plotEntryVec[1].GetHistogram()); // undo the change that you have done

    plotEntryVec[0].GetHistogram()->Divide(plotEntryVec[1].GetHistogram());
    std::string saveName1 = "AoverB__" + saveName;
    Plotter plot1 = Plotter({plotEntryVec[0]});
    plot1.AddLatex();
    std::string outputFile1 = outputDir + "/" + saveName1 + ".pdf";
    plot1.Save2D(outputFile1.c_str());

	return 0;
}