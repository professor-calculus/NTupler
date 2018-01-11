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
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_01_08/fatJetAmass_theFITS/fullKinematicCuts_tagAndAntiDBT_mcOnly/ht3500toInf/"; // where we are going to save the output plots (should include the samples name, and any important features)



    // TWO: set of cut params for the plot, will loop through all the combinations and put on a single plot
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Med2","Max","Off","Loose"}, {"Off","Loose","Off","Loose"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"DIAG_UP", "Loose"}, {"Off","Loose","Off","Loose"} }; // Top Diagnol Corner Crossing Axis at...--> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {3500,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {300,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)



    // THREE: plot histogram settings
    double luminosity = 35.867; // 2016 DATASET (DATA PLOTS SHOULD NOT BE USING THIS OBJECT)
    std::string varToPlot = "fatJetA_softDropMass";
    // TH1D hTemplate("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);events / bin", 40, 0, 200);
    TH1D hTemplate("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);a.u.", 40, 0, 200);
    // TH1D hTemplate("hTemplate", ";fatJetA_SoftDrop_Mass (GeV);ratio", 40, 0, 200);



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
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compareDifferentCuts.cc %s/%s__plotting_compareDifferentCuts.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
    }
    else if (runInstructionString == "batch"){
        std::string dirExistCommand = "test -e " + outputDir;
        std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compareDifferentCuts.cc %s/%s__plotting_compareDifferentCuts.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
        MacrosOnCondor::SubmitJob(outputDir.c_str(), "plotting_compareDifferentCuts", "/opt/ppd/scratch/xap79297/jobLogs/macros/");
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

    std::vector<PlotEntry> plotEntryVec_MC;
    // std::vector<PlotEntry> plotEntryVec_Data;
    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::string dbtCut = "";
                    if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP")
                        dbtCut = Form("fatJetA_doubleBtagDiscrim >= (-1.0 * fatJetB_doubleBtagDiscrim + 1.0 + %f) ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]) );
                    if (cut2_ak8Dbt[iCut2].size() == 4)
                        dbtCut = Form("fatJetA_doubleBtagDiscrim>=%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>=%f && fatJetB_doubleBtagDiscrim<%f ", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]) );
                    std::string cutToApply = Form("%s && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", dbtCut.c_str(), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);

                    // for the given cut criteria creates the legend name
                    std::string legendName = "";
                    if (cut2_ak8Dbt.size()>1){
                        if (cut2_ak8Dbt[iCut2].size() == 4) legendName += "DBT " + cut2_ak8Dbt[iCut2][0] + "-" + cut2_ak8Dbt[iCut2][1] + ":" + cut2_ak8Dbt[iCut2][2]+ "-" + cut2_ak8Dbt[iCut2][3];
                        if (cut2_ak8Dbt[iCut2].size() == 2 && cut2_ak8Dbt[iCut2][0] == "DIAG_UP") legendName += "DBTDiagUp" + cut2_ak8Dbt[iCut2][1];
                    }
                    if (cut3_ak8Pt.size()>1){
                        if (legendName.size() == 0) legendName += Form("AK8PT %d", cut3_ak8Pt[iCut3]);
                        else legendName += Form("_AK8PT %d", cut3_ak8Pt[iCut3]);
                    }
                    if (cut4_ht.size()>1){
                        if (legendName.size() == 0){
                        if (cut4_ht[iCut4][1]==99999) legendName += Form("HT %d+", cut4_ht[iCut4][0]);
                        else legendName += Form("HT %d-%d", cut4_ht[iCut4][0], cut4_ht[iCut4][1]);
                    }
                    else if (cut4_ht[iCut4][1]==99999) legendName += Form("_HT %d+", cut4_ht[iCut4][0]);
                        else legendName += Form("_HT %d-%d", cut4_ht[iCut4][0], cut4_ht[iCut4][1]);
                    }
                    if (cut5_ak4Pt.size()>1){
                        if (legendName.size() == 0) legendName += Form("AK4PT %d:%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                        else legendName += Form("_AK4PT %d:%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    }                    

                    // if (iCut2==0) legendName = "0.3 < fatJetA_DBT < 1.0"; // EXTRA HACK ON
                    // if (iCut2==1) legendName = "-1.0 < fatJetA_DBT < 0.3"; // EXTRA HACK ON
                    if (iCut2==0) legendName = "Tag DBT"; // EXTRA HACK ON
                    if (iCut2==1) legendName = "Anti-tag DBT"; // EXTRA HACK ON


                    // FOUR: sample info

                    // DATA
                    // PlotEntry plotElement2 = PlotEntry("Data", hTemplate, varToPlot.c_str()); // NO LUMI
                    // PlotEntry plotElement2 = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str()); // NO LUMI
                    // plotElement2.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/data/JetHT2016_ht1500plus/flatTree.root", cutToApply.c_str());
                    // plotElement2.NormalisePlot(); // OPTIONAL: toggle on or off
                    // if (iCut2 == 0) plotElement2.FitFunction("[0] + [1]/(x-[2]) + [3]/((x-[4])*(x-[4])) + [5]/((x-[6])*(x-[6])*(x-[6])) + [7]/((x-[8])*(x-[8])*(x-[8])*(x-[8]))", 12, 200, {0,0,0,0,0,0,0,0}, 46);
                    // else plotElement2.FitFunction("[0] + [1]/(x-[2]) + [3]/((x-[4])*(x-[4])) + [5]/((x-[6])*(x-[6])*(x-[6])) + [7]/((x-[8])*(x-[8])*(x-[8])*(x-[8]))", 12, 200, {0,0,0,0,0,0,0,0}, 38);

                    // QCD
                    // PlotEntry plotElement = PlotEntry("QCD MC", hTemplate, varToPlot.c_str(), luminosity); // note that the luminosity value doesn't matter IF we will normalise later
                    PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity); // note that the luminosity value doesn't matter IF we will normalise later
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1000to1500_ht1500plus/flatTree.root", cutToApply.c_str(), 1206);
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT1500to2000_ht1500plus/flatTree.root", cutToApply.c_str(), 120.4);
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/QCD_HT2000toInf_ht1500plus/flatTree.root", cutToApply.c_str(), 25.25);
                    plotElement.NormalisePlot(); // OPTIONAL: toggle on or off
                    // if (iCut2 == 0) plotElement.FitFunction("[0] + [1]/(x-[2]) + [3]/((x-[4])*(x-[4])) + [5]/((x-[6])*(x-[6])*(x-[6])) + [7]/((x-[8])*(x-[8])*(x-[8])*(x-[8]))", 13, 200, {0,0,0,0,0,0,0,0}, 46);
                    // else plotElement.FitFunction("[0] + [1]/(x-[2]) + [3]/((x-[4])*(x-[4])) + [5]/((x-[6])*(x-[6])*(x-[6])) + [7]/((x-[8])*(x-[8])*(x-[8])*(x-[8]))", 13, 200, {0,0,0,0,0,0,0,0}, 36);

                    // ZJets
                    // PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/ZJets_HT600toInf/flatTree.root", cutToApply.c_str(), 5.67);
                    // plotElement.NormalisePlot(); // OPTIONAL: toggle on or off

                    // WJets
                    // PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/WJets_HT600toInf/flatTree.root", cutToApply.c_str(), 95.14);
                    // plotElement.NormalisePlot(); // OPTIONAL: toggle on or off

                    // TTJets
                    // PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_09_27_CMSSW_8_0_29_dbtV4/mc/TTJets_NLO_ht1500plus/flatTree.root", cutToApply.c_str(), 831.76);
                    // plotElement.NormalisePlot(); // OPTIONAL: toggle on or off

                    // plotEntryVec_MC.push_back(plotElement2);
                    plotEntryVec_MC.push_back(plotElement);
                    // plotEntryVec_Data.push_back(plotElement2);

                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states



    // FIVE: plot aesthetics
    Plotter plot = Plotter(plotEntryVec_MC);
    // Plotter plot = Plotter(plotEntryVec_Data);
    // // Plotter plot = Plotter({},plotEntryVec); // stack plots

    // // for special ratio of ratio plots
    // Plotter plot = Plotter(plotEntryVec_MC);
    // plot.SetErrors();
    // TH1D * hMC = (TH1D*)plotEntryVec_MC[0].GetHistogram()->Clone();
    // hMC->Divide(plotEntryVec_MC[1].GetHistogram());
    // std::vector<TH1D*> vecMC;
    // vecMC.push_back(hMC);
    // Plotter plotAgainMC = Plotter(vecMC);
    // plotAgainMC.SetErrors();
    // Plotter plot2 = Plotter(plotEntryVec_Data); // individual plots
    // plot2.SetErrors();
    // TH1D * hDATA = (TH1D*)plotEntryVec_Data[0].GetHistogram()->Clone();
    // hDATA->Divide(plotEntryVec_Data[1].GetHistogram());
    // std::vector<TH1D*> vec;
    // vec.push_back(hDATA);
    // Plotter plotAgain = Plotter(vec);
    // plotAgain.SetErrors();
    // std::vector<TH1D*> vecreal = {hDATA, hMC};
    // Plotter therealPlot = Plotter(vecreal);
    // therealPlot.SetErrors();
    // therealPlot.AddRatioBox(0.82, 1.18, "Data / MC", true);
    // therealPlot.AddLegend({"Data", "QCD MC"}, 0.65, 0.88, 0.74, 0.86, 0.06); // top right (wide 2)
    // therealPlot.AddLatex(luminosity, "#it{Data} 2016"); // if you HAVE NOT normalised - maybe...

    // plot.AddLegend(0.65, 0.88, 0.70, 0.87); // top right (wide 4)
    // // plot.AddLegend(0.65, 0.88, 0.62, 0.87); // top right (wide 6)
    plot.AddLegend(0.63, 0.88, 0.70, 0.87, 0.05); // top right (wide 2)
    // plot.AddLegend(0.45, 0.88, 0.70, 0.87, 0.05); // top right (extra wide 2)
    // // plot.AddLegend(0.45, 0.88, 0.70, 0.87); // top right (extra wide 4)
    // // plot.AddLegend(0.45, 0.88, 0.62, 0.87); // top right (extra wide 6)
    plot.AddLatex(); // if you HAVE normalised
    // plot.AddLatex("#it{Data} 2016"); // if you HAVE normalised
    // // plot.AddLatex(luminosity); // if you HAVE NOT normalised
    // plot.AddLatex(luminosity, "#it{Data} 2016"); // if you HAVE NOT normalised - maybe...
    plot.SetErrors();
    // plot.AddRatioBox();
    // plot.AddRatioBox(0.094, 1.06);
    // plot.AddRatioBox("Data / MC");
    // plot.AddRatioBox(0.1, 3.22, "Data / MC");
    // // plot.AddRatioBox(0.4, 2.15);
    // plot.AddRatioBox(0.4, 2.15, "tag / anti");
    plot.AddRatioBox("tag / anti");


    std::string saveName = varToPlot;
    if (cut2_ak8Dbt.size()>1) saveName += "__dbtVary";
    else{
        if (cut2_ak8Dbt[0].size() == 4) saveName += "__dbt" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1] + "And" + cut2_ak8Dbt[0][2] + cut2_ak8Dbt[0][3];
        if (cut2_ak8Dbt[0].size() == 2 && cut2_ak8Dbt[0][0] == "DIAG_UP") saveName += "__dbtDiagUp" + cut2_ak8Dbt[0][1];
    }
    if (cut3_ak8Pt.size()>1) saveName += "_ak8ptVary";
    else saveName += Form("_ak8pt%d", cut3_ak8Pt[0]);
    if (cut4_ht.size()>1) saveName += "_htVary";
    else if (cut4_ht[0][1]==99999) saveName += Form("_ht%dplus",cut4_ht[0][0]);
        else saveName += Form("_ht%dto%d",cut4_ht[0][0],cut4_ht[0][1]);
    if (cut5_ak4Pt.size()>1) saveName += "_ak4ptVary";
    else saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0][0], cut5_ak4Pt[0][1]);
    // saveName += "APPEND_SAVE_NAME"; // OPTIONAL: can manually append the automatic savename
    // saveName = "MANUAL_SAVE_NAME"; // OPTIONAL: can manually overwrite the automatic savename
    std::string outputFile = outputDir + "/" + saveName + ".pdf";
    plot.Save(outputFile.c_str());
    plot.SetLogY();
    outputFile = outputDir + "/" + saveName + "__logY.pdf";
    plot.Save(outputFile.c_str());

    // therealPlot.SaveSpec01(outputFile.c_str(), {});
    // therealPlot.SetLogY();
    // outputFile = outputDir + "/" + saveName + "__logY.pdf";
    // therealPlot.SaveSpec01(outputFile.c_str(), {});

    return 0;
}