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
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_08_03/2017_94X/fatJetAmass_theFITS/fullKinematicCuts_antiDBT/ht1500to2500/"; // where we are going to save the output plots (should include the samples name, and any important features)




    // TWO: set of cut params for the plot, will loop through all the combinations and put on a single plot
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"}, {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for fatJetA min, 2nd for fatJetA max, 3rd for fatJetB min, 4th for fatJetB max --> "Off", "Loose", "Med1", "Med2", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,99999} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"DIAG_UP", "Loose"} };
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Loose","Off","Loose"} };
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,2500} }; // these are HT bins, not just cuts (NB: use 99999 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {300,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)




    // THREE: plot histogram settings
    
    // double luminosity = 35.867; // 2016 DATASET (data plots should not be using this object)
    double luminosity = 41.370; // 2017 DATASET
    
    std::string varToPlot = "fatJetA_softDropMassPuppi";
    // std::string varToPlot = "fatJetA_doubleBtagDiscrim";
    // std::string varToPlot = "fatJetA_p4.Pt()";
    // std::string varToPlot = "slimJetA_p4.Pt()";
    // std::string varToPlot = "ht";
    
    TH1D hTemplate("hTemplate", ";fatJetA SoftDropMass (GeV);fraction of events / bin", 88, 0, 220);
    // TH1D hTemplate("hTemplate", ";fatJetA SoftDropMass (GeV);ratio", 40, 0, 200);
    // TH1D hTemplate("hTemplate", ";fatJetA doubleBtagDiscriminator;fraction of events / bin", 40, -1, 1);
    // TH1D hTemplate("hTemplate", ";fatJetA p_{T} (GeV);fraction of events / bin", 40, 0, 3000);



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
    std::vector<PlotEntry> plotEntryVec_Data;
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

                    // if (iCut2==0) legendName = "0.3 < fatJetA DBT < 1.0"; // EXTRA HACK ON
                    // if (iCut2==1) legendName = "-1.0 < fatJetA DBT < 0.3"; // EXTRA HACK ON






                    // FOUR: sample info

                    // DATA
                    PlotEntry plotElementA = PlotEntry("Data", hTemplate, varToPlot.c_str()); // NO LUMI
                    // PlotEntry plotElementA = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str()); // NO LUMI
                    
                    // plotElementA.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data16/JetHT_Run2016Total_ht1499plus/flatTree.root", cutToApply.c_str());
                    plotElementA.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/data17/JetHT_Run2017TOTAL_ht1499plus/flatTree.root", cutToApply.c_str());
                    
                    plotElementA.NormalisePlot(); // OPTIONAL: toggle on or off
                    if (iCut2 == 0) plotElementA.FitFunction("[0] + [1]/(x-[2]) + [3]/((x-[4])*(x-[4])) + [5]/((x-[6])*(x-[6])*(x-[6])) + [7]/((x-[8])*(x-[8])*(x-[8])*(x-[8])) + [9]*(x-[10]) + [11]*(x-[12])*(x-[12]) + [13]*(x-[14])*(x-[14])*(x-[14]) + [15]*(x-[16])*(x-[16])*(x-[16])*(x-[16])", 13, 220, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 46);
                    // else plotElementA.FitFunction("[0] + [1]/(x-[2]) + [3]/((x-[4])*(x-[4])) + [5]/((x-[6])*(x-[6])*(x-[6])) + [7]/((x-[8])*(x-[8])*(x-[8])*(x-[8])) + [9]*(x-[10]) + [11]*(x-[12])*(x-[12]) + [13]*(x-[14])*(x-[14])*(x-[14]) + [15]*(x-[16])*(x-[16])*(x-[16])*(x-[16])", 13, 220, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 38);
                    

                    // QCD
                    PlotEntry plotElementB = PlotEntry("QCD MC", hTemplate, varToPlot.c_str(), luminosity); // note that the luminosity value doesn't matter IF we will normalise later
                    // PlotEntry plotElementB = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity); // note that the luminosity value doesn't matter IF we will normalise later
                    
                    // plotElementB.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc16/QCD_HT1000to1500_ht1499plus/flatTree.root", cutToApply.c_str(), 1206);
                    // plotElementB.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc16/QCD_HT1500to2000_ht1499plus/flatTree.root", cutToApply.c_str(), 120.4);
                    // plotElementB.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc16/QCD_HT2000toInf_ht1499plus/flatTree.root", cutToApply.c_str(), 25.25);
                    plotElementB.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc17_WMS/QCD_HT1000to1500_ht1499plus/flatTree.root", cutToApply.c_str(), 1005);
                    plotElementB.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc17_WMS/QCD_HT1500to2000_ht1499plus/flatTree.root", cutToApply.c_str(), 101.8);
                    plotElementB.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_08_03/mc17_WMS/QCD_HT2000toInf_ht1499plus/flatTree.root", cutToApply.c_str(), 20.54);
                    
                    plotElementB.NormalisePlot(); // OPTIONAL: toggle on or off
                    // if (iCut2 == 0) plotElementB.FitFunction("[0] + [1]/(x-[2]) + [3]/((x-[4])*(x-[4])) + [5]/((x-[6])*(x-[6])*(x-[6])) + [7]/((x-[8])*(x-[8])*(x-[8])*(x-[8])) + [9]*(x-[10]) + [11]*(x-[12])*(x-[12]) + [13]*(x-[14])*(x-[14])*(x-[14]) + [15]*(x-[16])*(x-[16])*(x-[16])*(x-[16])", 13, 220, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 46);
                    plotElementB.FitFunction("[0] + [1]/(x-[2]) + [3]/((x-[4])*(x-[4])) + [5]/((x-[6])*(x-[6])*(x-[6])) + [7]/((x-[8])*(x-[8])*(x-[8])*(x-[8])) + [9]*(x-[10]) + [11]*(x-[12])*(x-[12]) + [13]*(x-[14])*(x-[14])*(x-[14]) + [15]*(x-[16])*(x-[16])*(x-[16])*(x-[16])", 13, 220, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 38);


                    // STILL ON OLD 80X VERSION AS DON"T KNOW WHEN I WILL NEED THEM
                    // ZJets
                    // PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_04_11/mc/ZJets_ht1200plus/flatTree.root", cutToApply.c_str(), 52.79);
                    // plotElement.NormalisePlot(); // OPTIONAL: toggle on or off

                    // WJets
                    // PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_04_11/mc/WJets_ht1200plus/flatTree.root", cutToApply.c_str(), 95.14);
                    // plotElement.NormalisePlot(); // OPTIONAL: toggle on or off

                    // TTJets
                    // PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_04_11/mc/TTJets_ht1200plus/flatTree.root", cutToApply.c_str(), 831.76);
                    // plotElement.NormalisePlot(); // OPTIONAL: toggle on or off

                    // Signal
                    // PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2018_04_11/mc/mH125p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.0091050*0.85*0.85);
                    // plotElement.NormalisePlot();





                    plotEntryVec_MC.push_back(plotElementA);
                    plotEntryVec_MC.push_back(plotElementB);
                    // plotEntryVec_MC.push_back(plotElement);
                    // plotEntryVec_Data.push_back(plotElementB);


                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states





    // FIVE: plot aesthetics
    Plotter plot = Plotter(plotEntryVec_MC);
    // Plotter plot = Plotter(plotEntryVec_Data);
    // // Plotter plot = Plotter({},plotEntryVec); // stack plots

    // for special ratio of ratio plots
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
    // therealPlot.AddLatex(luminosity, "#it{Preliminary}"); // DATA

    // plot.AddLegend(0.65, 0.88, 0.70, 0.87); // top right (wide 4)
    // // plot.AddLegend(0.65, 0.88, 0.62, 0.87); // top right (wide 6)
    plot.AddLegend(0.63, 0.88, 0.70, 0.87, 0.05); // top right (wide 2)
    // plot.AddLegend(0.20, 0.50, 0.70, 0.87, 0.05); // top left (wide 2)
    // plot.AddLegend(0.45, 0.88, 0.70, 0.87, 0.05); // top right (extra wide 2)
    // // plot.AddLegend(0.45, 0.88, 0.70, 0.87); // top right (extra wide 4)
    // // plot.AddLegend(0.45, 0.88, 0.62, 0.87); // top right (extra wide 6)

    // plot.AddLatex(); // simulation only - normalised
    // plot.AddLatex(luminosity); // simulation only - NOT normalised
    plot.AddLatex(luminosity, "#it{Preliminary}"); // DATA
    
    plot.SetErrors();

    plot.AddRatioBox();
    // plot.AddRatioBox(0.094, 1.06);
    // plot.AddRatioBox("Data / MC");
    // plot.AddRatioBox(0.05, 3.10, "Data / MC");
    // plot.AddRatioBox(0.4, 2.15);
    // plot.AddRatioBox(0.4, 2.15, "tag / anti");
    // plot.AddRatioBox("tag / anti");

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