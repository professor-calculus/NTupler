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

int main(){

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //// U S E R * I N P U T S ///////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/testABCD/"; // where we are going to save the output plots

    // TWO: set the cut params. Will loop through all combinations. Note that the DBT is inverted by requiring the leading DBT < cut2_ak8Dbt[0]
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Med"} };// "Off", Loose", "Med", or "Tight" (2 elements in sub-vector, 1st for leading DBT)
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>> cut4_ht = { {1500,2500} }; // these are HT bins, not just cuts
    std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading pt)

    // THREE: choose mass points which define the set of ABCD grids (should contain at least four elements)
    // first element: lower bound
    // second element: 1st abcd mass point
    // third element: 2nd abcd mass point
    // etc.
    // last element: upper bound 
    std::vector<double> abcdMassSet = {0.0, 30.0, 60.0, 90.0, 200.0};

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //// E N D * O F * I N P U T S ///////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());

    std::string varToPlot = "fatJetA_softDropMass";
    double dummyLuminosity = 50.0; // note that the luminosity value doesn't matter as we will later take a ratio, it just needs to be consistent

    // Loop through the specified cut combinations
    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::string legendName = "";
                    if (cut2_ak8Dbt.size()>1) legendName += "DBT " + cut2_ak8Dbt[iCut2][0] + ":" + cut2_ak8Dbt[iCut2][1];
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

                    std::string cutToApplyBase = Form("fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d", cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]);
                    std::string cutToApply_wDBT = Form("%s && fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f", cutToApplyBase.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][1]));; // uses the DBT cut --> regions A (signal) and C (mass sidebands)
                    std::string cutToApply_invDBT = Form("%s && fatJetA_doubleBtagDiscrim<%f", cutToApplyBase.c_str(), DoubleBTagWPs::dbtNameToDouble(cut2_ak8Dbt[iCut2][0]));; // inverts the DBT cut --> regions B and D (mass sidebands)
                    
                    TH1F hTemplate = TH1F("hTemplate", ";LeadingBDiscFatJet_SoftDrop_Mass (GeV);DBT Pass / DBT Fail", abcdMassSet.size()-1, &(abcdMassSet)[0]);

                    // NB: could loop through the mass space and make all actual ABCDs

                    
                    PlotEntry ttbar_wDBT = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), dummyLuminosity); 
                    ttbar_wDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply_wDBT.c_str()), 831.76);
                    ttbar_wDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply_wDBT.c_str()), 2.6665344485, false);
                    ttbar_wDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply_wDBT.c_str()), 1.0980821984, false);
                    ttbar_wDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply_wDBT.c_str()), 0.1987479092, false);
                    ttbar_wDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply_wDBT.c_str()), 0.002368412585, false);

                    PlotEntry ttbar_invDBT = PlotEntry(legendName.c_str(), hTemplate, varToPlot.c_str(), dummyLuminosity); 
                    ttbar_invDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_inclusiveHt/flatTree.root", Form("%s && lheHT<700.0",cutToApply_invDBT.c_str()), 831.76);
                    ttbar_invDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT600to800/flatTree.root", Form("%s && lheHT>=700.0",cutToApply_invDBT.c_str()), 2.6665344485, false);
                    ttbar_invDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT800to1200/flatTree.root", Form("%s && lheHT>=700.0",cutToApply_invDBT.c_str()), 1.0980821984, false);
                    ttbar_invDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT1200to2500/flatTree.root", Form("%s && lheHT>=700.0",cutToApply_invDBT.c_str()), 0.1987479092, false);
                    ttbar_invDBT.AddInput("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/TTJets_HT2500toInf/flatTree.root", Form("%s && lheHT>=700.0",cutToApply_invDBT.c_str()), 0.002368412585, false);

                    PlotEntry ttbar_div = PlotEntry(legendName.c_str(), ttbar_wDBT, ttbar_invDBT);
                    Plotter plot_ttbar = Plotter({ttbar_div});
                    plot_ttbar.AddLatex();
                    plot_ttbar.SetErrors();
                    // std::string outputFile = outputDir + "/" + saveName + ".pdf";
                    std::string outputFile = outputDir + "/testing.pdf";
                    plot_ttbar.Save();

                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states

 //    Plotter plot = Plotter(setOfTtbar);
 //    plot.AddLegend(0.74, 0.88, 0.77, 0.87);
 //    plot.AddLatex();
 //    plot.SetErrors();

 //    std::string saveName = varToPlot;
 //    if (cut2_ak8Dbt.size()>1) saveName += "__dbtVary";
 //    else saveName += "__dbt" + cut2_ak8Dbt[0][0] + cut2_ak8Dbt[0][1];
 //    if (cut3_ak8Pt.size()>1) saveName += "_ak8ptVary";
 //    else saveName += Form("_ak8pt%d", cut3_ak8Pt[0]);
 //    if (cut4_ht.size()>1) saveName += "_htVary";
 //    else if (cut4_ht[0][1]==13000) saveName += Form("_ht%dplus",cut4_ht[0][0]);
 //        else saveName += Form("_ht%dto%d",cut4_ht[0][0],cut4_ht[0][1]);
 //    if (cut5_ak4Pt.size()>1) saveName += "_ak4ptVary";
 //    else saveName += Form("_ak4pt%dn%d", cut5_ak4Pt[0][0], cut5_ak4Pt[0][1]);
 //    std::string outputFile = outputDir + "/" + saveName + ".pdf";

	// plot.Save(outputFile.c_str());


	return 0;
}