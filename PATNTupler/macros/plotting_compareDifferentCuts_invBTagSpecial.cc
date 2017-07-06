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
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"

int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_06_22/qcdShape_invertBTag/fullCutsHt1500to2500v2/"; // where we are going to save the output plots (should include the samples name + binning maybe)


    // TWO: set of cut params for the plot (will loop through the combinations)
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Off","Max","Off","Max"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    // std::vector<int> cut3_ak8Pt = {-1};
    // std::vector<std::vector<int>> cut4_ht = { {-1,13000} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    // std::vector<std::vector<int>> cut5_ak4Pt = { {-1,-1} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)

    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Max","Med","Max"}, {"Off","Tight","Off","Med"}, {"Off","Tight","Loose","Med"}, {"Loose","Tight","Off","Med"}, {"Loose","Tight","Loose","Med"}, {"Med","Tight","Off","Med"}, {"Med","Tight","Loose","Med"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Max","Med","Max"}, {"Tight","Max","Off","Med"}, {"Off","Tight","Off","Med"}, {"Off","Loose","Off","Loose"} }; // 4 elements in sub-vector: 1st for leading DBT min, 2nd for leading DBT max, 3rd for secondary DBT min, 4th for secondary DBT max --> "Off", "Loose", "Med", "Tight", "Max"
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,2500} }; // these are HT bins, not just cuts (NB: use 13000 for a maximum)
    std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading pt, 2nd for seconary pt)


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // THREE: plot histogram settings
    double luminosity = 50.0; // note that this value doesn't matter IF you normalise later

    // TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
    TH1F hTemplate("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);events / bin", 50, 0, 200);
    std::string varToPlot = "fatJetA_softDropMass";
    
    // TH1F hTemplate("hTemplate", ";SecondaryBDiscFatJet_SoftDrop_Mass (GeV);a.u.", 50, 0, 200);
    // std::string varToPlot = "fatJetB_softDropMass";

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_compareDifferentCuts_invBTagSpecial.cc %s/%s__plotting_compareDifferentCuts_invBTagSpecial.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    std::vector<PlotEntry> plotEntryVec;
    std::vector<PlotEntry> plotEntryVecBackground;
    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::string cutToApply = Form("fatJetA_doubleBtagDiscrim>%f && fatJetA_doubleBtagDiscrim<%f && fatJetB_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim<%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][2]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][3]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);

                    // for a given cut criteria creates the legend name
                    std::string legendName = "";
                    if (cut2_ak8Dbt.size()>1) legendName += "DBT " + cut2_ak8Dbt[iCut2][0] + "-" + cut2_ak8Dbt[iCut2][1] + ":" + cut2_ak8Dbt[iCut2][2]+ "-" + cut2_ak8Dbt[iCut2][3];
                    if (cut3_ak8Pt.size()>1){
                        if (legendName.size() == 0) legendName += Form("AK8PT %d", cut3_ak8Pt[iCut3]);
                        else legendName += Form("_AK8PT %d", cut3_ak8Pt[iCut3]);
                    }
                    if (cut4_ht.size()>1){
                        if (legendName.size() == 0){
                        if (cut4_ht[iCut4][1]==13000) legendName += Form("HT %d+", cut4_ht[iCut4][0]);
                        else legendName += Form("HT %d-%d", cut4_ht[iCut4][0], cut4_ht[iCut4][1]);
                    }
                    else if (cut4_ht[iCut4][1]==13000) legendName += Form("_HT %d+", cut4_ht[iCut4][0]);
                        else legendName += Form("_HT %d-%d", cut4_ht[iCut4][0], cut4_ht[iCut4][1]);
                    }
                    if (cut5_ak4Pt.size()>1){
                        if (legendName.size() == 0) legendName += Form("AK4PT %d:%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                        else legendName += Form("_AK4PT %d:%d", cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    }                    
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    // FOUR: sample info
                    PlotEntry plotElement = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), luminosity); // note that the luminosity value doesn't matter IF we will normalise later
                    // PlotEntry plotElement = PlotEntry("ttbar+jets", hTemplate, varToPlot.c_str(), luminosity); // OPTIONAL: can manually set the legendName

                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply.c_str()), 831.76);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 2.6665344485, false);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 1.0980821984, false);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.1987479092, false);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply.c_str()), 0.002368412585, false);

                    // USE FOR SHAPE ONLY - not for counting number of events
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", cutToApply.c_str(), 2.6665344485);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", cutToApply.c_str(), 1.0980821984);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", cutToApply.c_str(), 0.1987479092);
                    // plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", cutToApply.c_str(), 0.002368412585);

                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT700to1000/flatTree.root", cutToApply.c_str(), 6802);
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1000to1500/flatTree.root", cutToApply.c_str(), 1206);
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT1500to2000/flatTree.root", cutToApply.c_str(), 120.4);
                    plotElement.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/QCD_HT2000toInf/flatTree.root", cutToApply.c_str(), 25.25);

                    plotElement.NormalisePlot();
                    plotEntryVec.push_back(plotElement);
                    // plotEntryVecBackground.push_back(plotElement); // if creating a background sample

                    // PlotEntry mH30_mSusy800 = PlotEntry("mH30_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // mH30_mSusy800.NormalisePlot();
                    // plotEntryVec.push_back(mH30_mSusy800);

                    // PlotEntry mH50_mSusy800 = PlotEntry("mH50_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH50_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // mH50_mSusy800.NormalisePlot();
                    // plotEntryVec.push_back(mH50_mSusy800);

                    // PlotEntry mH70_mSusy800 = PlotEntry("mH70_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // mH70_mSusy800.NormalisePlot();
                    // plotEntryVec.push_back(mH70_mSusy800);

                    // PlotEntry mH90_mSusy800 = PlotEntry("mH90_mSusy800", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy800.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 6.466*0.85*0.85);
                    // // mH90_mSusy800.NormalisePlot();
                    // plotEntryVec.push_back(mH90_mSusy800);

                    // PlotEntry mH30_mSusy1200 = PlotEntry("mH30_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                    // // mH30_mSusy1200.NormalisePlot();
                    // plotEntryVec.push_back(mH30_mSusy1200);

                    // PlotEntry mH50_mSusy1200 = PlotEntry("mH50_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // mH50_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                    // // mH50_mSusy1200.NormalisePlot();
                    // plotEntryVec.push_back(mH50_mSusy1200);

                    // PlotEntry mH70_mSusy1200 = PlotEntry("mH70_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                    // // mH70_mSusy1200.NormalisePlot();
                    // plotEntryVec.push_back(mH70_mSusy1200);

                    // PlotEntry mH90_mSusy1200 = PlotEntry("mH90_mSusy1200", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy1200.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.494*0.85*0.85);
                    // // mH90_mSusy1200.NormalisePlot();
                    // plotEntryVec.push_back(mH90_mSusy1200);

                    // PlotEntry mH30_mSusy1600 = PlotEntry("mH30_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);
                    // mH30_mSusy1600.NormalisePlot();
                    // plotEntryVec.push_back(mH30_mSusy1600);

                    // PlotEntry mH50_mSusy1600 = PlotEntry("mH50_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // mH50_mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);
                    // mH50_mSusy1600.NormalisePlot();
                    // plotEntryVec.push_back(mH50_mSusy1600);

                    // PlotEntry mH70_mSusy1600 = PlotEntry("mH70_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);
                    // mH70_mSusy1600.NormalisePlot();
                    // plotEntryVec.push_back(mH70_mSusy1600);

                    // PlotEntry mH90_mSusy1600 = PlotEntry("mH90_mSusy1600", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy1600.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.060*0.85*0.85);
                    // mH90_mSusy1600.NormalisePlot();
                    // plotEntryVec.push_back(mH90_mSusy1600);

                    // PlotEntry mH30_mSusy2000 = PlotEntry("mH30_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH30_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // // mH30_mSusy2000.NormalisePlot();
                    // plotEntryVec.push_back(mH30_mSusy2000);

                    // PlotEntry mH50_mSusy2000 = PlotEntry("mH50_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH50_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // // mH50_mSusy2000.NormalisePlot();
                    // plotEntryVec.push_back(mH50_mSusy2000);

                    // PlotEntry mH70_mSusy2000 = PlotEntry("mH70_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH70_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // // mH70_mSusy2000.NormalisePlot();
                    // plotEntryVec.push_back(mH70_mSusy2000);

                    // PlotEntry mH90_mSusy2000 = PlotEntry("mH90_mSusy2000", hTemplate, varToPlot.c_str(), luminosity);
                    // mH90_mSusy2000.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root", cutToApply.c_str(), 0.009*0.85*0.85);
                    // // mH90_mSusy2000.NormalisePlot();
                    // plotEntryVec.push_back(mH90_mSusy2000);

                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////////////////
                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // FIVE: plot aesthetics
    Plotter plot = Plotter(plotEntryVec); // if doing signal OR background
    // Plotter plot = Plotter(plotEntryVec,plotEntryVecBackground); // if doing signal AND background
    // plot.AddLegend(0.45, 0.88, 0.77, 0.87); // top right (extra wide 2)
    // plot.AddLegend(0.45, 0.88, 0.70, 0.87); // top right (extra wide 4)
    plot.AddLegend(0.45, 0.88, 0.62, 0.87); // top right (extra wide 6)
    // plot.AddLegend(0.65, 0.88, 0.77, 0.87); // top right (wide 2)
    // plot.AddLegend(0.65, 0.88, 0.70, 0.87); // top right (wide 4)
    // plot.AddLegend(0.65, 0.88, 0.62, 0.87); // top right (wide 6)
    plot.AddLatex();
    // plot.AddLatex(luminosity);
    plot.SetErrors();
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    std::string saveName = varToPlot;
    if (cut2_ak8Dbt.size()>1) saveName += "__dbtVary";
    else saveName += "__dbt" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1] + "And" + cut2_ak8Dbt[0][2] + cut2_ak8Dbt[0][3];
    if (cut3_ak8Pt.size()>1) saveName += "_ak8ptVary";
    else saveName += Form("_ak8pt%d", cut3_ak8Pt[0]);
    if (cut4_ht.size()>1) saveName += "_htVary";
    else if (cut4_ht[0][1]==13000) saveName += Form("_ht%dplus",cut4_ht[0][0]);
        else saveName += Form("_ht%dto%d",cut4_ht[0][0],cut4_ht[0][1]);
    if (cut5_ak4Pt.size()>1) saveName += "_ak4ptVary";
    else saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0][0], cut5_ak4Pt[0][1]);
    // saveName = ""; // OPTIONAL: can manually create the saveName
    std::string outputFile = outputDir + "/" + saveName + ".pdf";

	plot.Save(outputFile.c_str());
    plot.SetLogY();
    outputFile = outputDir + "/" + saveName + "__logY.pdf";
    plot.Save(outputFile.c_str());

	return 0;
}