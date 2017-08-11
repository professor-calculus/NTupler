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

// MAKES PLOTS USING HISTOGRAMS

std::map<std::string,TH1D*> GetHistograms();
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info & luminosity
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_08_10/histosSTILLNEEDPROPERNAME/"; // where we are going to save the output plots (should include the samples name, and any important features)
	double luminosity = 50.0; // NB this is just a label for the plot. It should match the lumi of the histograms.
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_plot.cc %s/%s__histos_plot.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));



    std::map<std::string, TH1D*> h_ = GetHistograms();
    // explanation of terms after h
    // 1. S, U, D --> refers to mass space. predV1, predV2 correspond to predictions of S. 
    // 2. tag, inverted, controlA, controlB --> refers to 2*DBT space
    // 3. sample name on the end



    // TWO: make plot aesthetics and saving
    // std::vector<TH1D*> indiHistoVec = {h_["S_tag_QCD"], h_["predV1_tag_QCD"], h_["predV2_tag_QCD"]};
    std::vector<TH1D*> indiHistoVec = {h_["S_tag_QCDNOAK4"], h_["predV1_tag_QCDNOAK4"], h_["predV2_tag_QCDNOAK4"]};
    // std::vector<TH1D*> indiHistoVec = {h_["S_tag_mH70mSusy1600"], h_["S_tag_mH90mSusy1600"], h_["S_tag_mH70mSusy2000"], h_["S_tag_mH90mSusy2000"]};
    // std::vector<TH1D*> indiHistoVec = {h_["S_tag_mH90mSusy800"], h_["S_tag_mH90mSusy1200"], h_["S_tag_mH90mSusy1600"], h_["S_tag_mH90mSusy2000"]};
    // std::vector<TH1D*> indiHistoVec = {h_["S_tag_mH90mSusy2000"]};
    // std::vector<TH1D*> stackHistoVec = {h_["S_tag_ZJets"], h_["S_tag_TTJets"], h_["S_tag_QCD"]};
    
    Plotter plot = Plotter(indiHistoVec);
    // Plotter plot = Plotter(indiHistoVec, stackHistoVec);
    // Plotter plot = Plotter({}, stackHistoVec);

    std::vector<std::string> legendNames = {"S_tag", "predV1_tag", "predV2_tag"};
    // std::vector<std::string> legendNames = {"mH70mSusy1600", "mH90mSusy1600", "mH70mSusy2000", "mH90mSusy2000", "QCD", "TTJets", "ZJets", "WJets"};
    // std::vector<std::string> legendNames = {"mH90mSusy800", "mH90mSusy1200", "mH90mSusy1600", "mH90mSusy2000", "QCD", "TTJets", "ZJets", "WJets"};
    // std::vector<std::string> legendNames = {"mH90mSusy2000", "ZJets", "TTJets", "QCD"};
    plot.AddLegend(legendNames, 0.68, 0.88, 0.47, 0.81, 0.03);
    
    plot.AddLatex(luminosity);
    // plot.AddRatioBox();
    // plot.AddRatioBox(0,5);
    // plot.SetLogY();
    // plot.SetYValueMin(0.1);
    plot.SetErrors();
    // plot.SetErrors("only_stack");
    // plot.SetErrors("only_indi");

    std::vector<std::string> stringVec = {"HT1500-2500", "HT2500-3500", "HT3500+"};
    std::string plotName = "TESTING6";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);



    return 0;
}





std::map<std::string,TH1D*> GetHistograms()
{
    std::map<std::string, TH1D*> h_;

    // ROOT file w/histos locations
    TFile * f_QCD = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/QCD/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_QCDNOAK4 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/QCD/NOAK4PT/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_TTJets = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/TTJets/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_ZJets = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/ZJets/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_WJets = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/WJets/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    
    TFile * f_mH30mSusy800 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH30_mSusy800/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH30mSusy1200 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH30_mSusy1200/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH30mSusy1600 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH30_mSusy1600/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH30mSusy2000 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH30_mSusy2000/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    
    TFile * f_mH50mSusy800 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH50_mSusy800/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH50mSusy1200 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH50_mSusy1200/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH50mSusy1600 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH50_mSusy1600/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH50mSusy2000 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH50_mSusy2000/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    
    TFile * f_mH70mSusy800 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH70_mSusy800/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH70mSusy1200 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH70_mSusy1200/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH70mSusy1600 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH70_mSusy1600/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH70mSusy2000 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH70_mSusy2000/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    
    TFile * f_mH90mSusy800 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH90_mSusy800/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH90mSusy1200 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH90_mSusy1200/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH90mSusy1600 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH90_mSusy1600/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TFile * f_mH90mSusy2000 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_07_20_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV03/mH90_mSusy2000/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");


    // explanation of terminology
    // 1. S, U, D --> refers to mass space. (predV1, predV2 correspond to predictions of S.)
    // 2. tag, inverted, controlA, controlB --> refers to 2*DBT space
    // 3. sample name on the end


    // QCD
    h_["S_tag_QCD"] = (TH1D*)f_QCD->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_QCD"] = (TH1D*)f_QCD->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_QCD"] = (TH1D*)f_QCD->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_QCD"] = (TH1D*)f_QCD->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_QCD"] = (TH1D*)f_QCD->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_QCD"] = (TH1D*)f_QCD->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_QCD"] = (TH1D*)f_QCD->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_QCD"] = (TH1D*)f_QCD->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_QCD"] = (TH1D*)f_QCD->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_QCD"] = (TH1D*)f_QCD->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_QCD"] = (TH1D*)f_QCD->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_QCD"] = (TH1D*)f_QCD->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_QCD"] = (TH1D*)h_["U_inverted_QCD"]->Clone();
    h_["predV2_inverted_QCD"]->Add(h_["D_inverted_QCD"]);

    h_["predV2_tag_QCD"] = (TH1D*)h_["U_tag_QCD"]->Clone();
    h_["predV2_tag_QCD"]->Add(h_["D_tag_QCD"]);    
    h_["predV1_tag_QCD"] = (TH1D*)h_["predV2_tag_QCD"]->Clone();
    h_["predV1_tag_QCD"]->Multiply(h_["S_inverted_QCD"]);
    h_["predV1_tag_QCD"]->Divide(h_["predV2_inverted_QCD"]);

    h_["predV2_controlA_QCD"] = (TH1D*)h_["U_controlA_QCD"]->Clone();
    h_["predV2_controlA_QCD"]->Add(h_["D_controlA_QCD"]);    
    h_["predV1_controlA_QCD"] = (TH1D*)h_["predV2_controlA_QCD"]->Clone();
    h_["predV1_controlA_QCD"]->Multiply(h_["S_inverted_QCD"]);
    h_["predV1_controlA_QCD"]->Divide(h_["predV2_inverted_QCD"]);

    h_["predV2_controlB_QCD"] = (TH1D*)h_["U_controlB_QCD"]->Clone();
    h_["predV2_controlB_QCD"]->Add(h_["D_controlB_QCD"]);    
    h_["predV1_controlB_QCD"] = (TH1D*)h_["predV2_controlB_QCD"]->Clone();
    h_["predV1_controlB_QCD"]->Multiply(h_["S_inverted_QCD"]);
    h_["predV1_controlB_QCD"]->Divide(h_["predV2_inverted_QCD"]);

    h_["predV2_inverted_QCD"]->SetBinContent(1, 2 * h_["predV2_inverted_QCD"]->GetBinContent(1));
    h_["predV2_inverted_QCD"]->SetBinError(1, 2 * h_["predV2_inverted_QCD"]->GetBinError(1));
    h_["predV2_tag_QCD"]->SetBinContent(1, 2 * h_["predV2_tag_QCD"]->GetBinContent(1));
    h_["predV2_tag_QCD"]->SetBinError(1, 2 * h_["predV2_tag_QCD"]->GetBinError(1));
    h_["predV2_controlA_QCD"]->SetBinContent(1, 2 * h_["predV2_controlA_QCD"]->GetBinContent(1));
    h_["predV2_controlA_QCD"]->SetBinError(1, 2 * h_["predV2_controlA_QCD"]->GetBinError(1));
    h_["predV2_controlB_QCD"]->SetBinContent(1, 2 * h_["predV2_controlB_QCD"]->GetBinContent(1));
    h_["predV2_controlB_QCD"]->SetBinError(1, 2 * h_["predV2_controlB_QCD"]->GetBinError(1));


    // QCDNOAK4
    h_["S_tag_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_QCDNOAK4"] = (TH1D*)f_QCDNOAK4->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_QCDNOAK4"] = (TH1D*)h_["U_inverted_QCDNOAK4"]->Clone();
    h_["predV2_inverted_QCDNOAK4"]->Add(h_["D_inverted_QCDNOAK4"]);

    h_["predV2_tag_QCDNOAK4"] = (TH1D*)h_["U_tag_QCDNOAK4"]->Clone();
    h_["predV2_tag_QCDNOAK4"]->Add(h_["D_tag_QCDNOAK4"]);    
    h_["predV1_tag_QCDNOAK4"] = (TH1D*)h_["predV2_tag_QCDNOAK4"]->Clone();
    h_["predV1_tag_QCDNOAK4"]->Multiply(h_["S_inverted_QCDNOAK4"]);
    h_["predV1_tag_QCDNOAK4"]->Divide(h_["predV2_inverted_QCDNOAK4"]);

    h_["predV2_controlA_QCDNOAK4"] = (TH1D*)h_["U_controlA_QCDNOAK4"]->Clone();
    h_["predV2_controlA_QCDNOAK4"]->Add(h_["D_controlA_QCDNOAK4"]);    
    h_["predV1_controlA_QCDNOAK4"] = (TH1D*)h_["predV2_controlA_QCDNOAK4"]->Clone();
    h_["predV1_controlA_QCDNOAK4"]->Multiply(h_["S_inverted_QCDNOAK4"]);
    h_["predV1_controlA_QCDNOAK4"]->Divide(h_["predV2_inverted_QCDNOAK4"]);

    h_["predV2_controlB_QCDNOAK4"] = (TH1D*)h_["U_controlB_QCDNOAK4"]->Clone();
    h_["predV2_controlB_QCDNOAK4"]->Add(h_["D_controlB_QCDNOAK4"]);    
    h_["predV1_controlB_QCDNOAK4"] = (TH1D*)h_["predV2_controlB_QCDNOAK4"]->Clone();
    h_["predV1_controlB_QCDNOAK4"]->Multiply(h_["S_inverted_QCDNOAK4"]);
    h_["predV1_controlB_QCDNOAK4"]->Divide(h_["predV2_inverted_QCDNOAK4"]);
    
    h_["predV2_inverted_QCDNOAK4"]->SetBinContent(1, 2 * h_["predV2_inverted_QCDNOAK4"]->GetBinContent(1));
    h_["predV2_inverted_QCDNOAK4"]->SetBinError(1, 2 * h_["predV2_inverted_QCDNOAK4"]->GetBinError(1));
    h_["predV2_tag_QCDNOAK4"]->SetBinContent(1, 2 * h_["predV2_tag_QCDNOAK4"]->GetBinContent(1));
    h_["predV2_tag_QCDNOAK4"]->SetBinError(1, 2 * h_["predV2_tag_QCDNOAK4"]->GetBinError(1));
    h_["predV2_controlA_QCDNOAK4"]->SetBinContent(1, 2 * h_["predV2_controlA_QCDNOAK4"]->GetBinContent(1));
    h_["predV2_controlA_QCDNOAK4"]->SetBinError(1, 2 * h_["predV2_controlA_QCDNOAK4"]->GetBinError(1));
    h_["predV2_controlB_QCDNOAK4"]->SetBinContent(1, 2 * h_["predV2_controlB_QCDNOAK4"]->GetBinContent(1));
    h_["predV2_controlB_QCDNOAK4"]->SetBinError(1, 2 * h_["predV2_controlB_QCDNOAK4"]->GetBinError(1));


    // TTJets
    h_["S_tag_TTJets"] = (TH1D*)f_TTJets->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_TTJets"] = (TH1D*)f_TTJets->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_TTJets"] = (TH1D*)f_TTJets->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_TTJets"] = (TH1D*)f_TTJets->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_TTJets"] = (TH1D*)f_TTJets->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_TTJets"] = (TH1D*)f_TTJets->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_TTJets"] = (TH1D*)f_TTJets->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_TTJets"] = (TH1D*)f_TTJets->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_TTJets"] = (TH1D*)f_TTJets->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_TTJets"] = (TH1D*)f_TTJets->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_TTJets"] = (TH1D*)f_TTJets->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_TTJets"] = (TH1D*)f_TTJets->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_TTJets"] = (TH1D*)h_["U_inverted_TTJets"]->Clone();
    h_["predV2_inverted_TTJets"]->Add(h_["D_inverted_TTJets"]);

    h_["predV2_tag_TTJets"] = (TH1D*)h_["U_tag_TTJets"]->Clone();
    h_["predV2_tag_TTJets"]->Add(h_["D_tag_TTJets"]);    
    h_["predV1_tag_TTJets"] = (TH1D*)h_["predV2_tag_TTJets"]->Clone();
    h_["predV1_tag_TTJets"]->Multiply(h_["S_inverted_TTJets"]);
    h_["predV1_tag_TTJets"]->Divide(h_["predV2_inverted_TTJets"]);

    h_["predV2_controlA_TTJets"] = (TH1D*)h_["U_controlA_TTJets"]->Clone();
    h_["predV2_controlA_TTJets"]->Add(h_["D_controlA_TTJets"]);    
    h_["predV1_controlA_TTJets"] = (TH1D*)h_["predV2_controlA_TTJets"]->Clone();
    h_["predV1_controlA_TTJets"]->Multiply(h_["S_inverted_TTJets"]);
    h_["predV1_controlA_TTJets"]->Divide(h_["predV2_inverted_TTJets"]);

    h_["predV2_controlB_TTJets"] = (TH1D*)h_["U_controlB_TTJets"]->Clone();
    h_["predV2_controlB_TTJets"]->Add(h_["D_controlB_TTJets"]);    
    h_["predV1_controlB_TTJets"] = (TH1D*)h_["predV2_controlB_TTJets"]->Clone();
    h_["predV1_controlB_TTJets"]->Multiply(h_["S_inverted_TTJets"]);
    h_["predV1_controlB_TTJets"]->Divide(h_["predV2_inverted_TTJets"]);

    h_["predV2_inverted_TTJets"]->SetBinContent(1, 2 * h_["predV2_inverted_TTJets"]->GetBinContent(1));
    h_["predV2_inverted_TTJets"]->SetBinError(1, 2 * h_["predV2_inverted_TTJets"]->GetBinError(1));
    h_["predV2_tag_TTJets"]->SetBinContent(1, 2 * h_["predV2_tag_TTJets"]->GetBinContent(1));
    h_["predV2_tag_TTJets"]->SetBinError(1, 2 * h_["predV2_tag_TTJets"]->GetBinError(1));
    h_["predV2_controlA_TTJets"]->SetBinContent(1, 2 * h_["predV2_controlA_TTJets"]->GetBinContent(1));
    h_["predV2_controlA_TTJets"]->SetBinError(1, 2 * h_["predV2_controlA_TTJets"]->GetBinError(1));
    h_["predV2_controlB_TTJets"]->SetBinContent(1, 2 * h_["predV2_controlB_TTJets"]->GetBinContent(1));
    h_["predV2_controlB_TTJets"]->SetBinError(1, 2 * h_["predV2_controlB_TTJets"]->GetBinError(1));


    // ZJets
    h_["S_tag_ZJets"] = (TH1D*)f_ZJets->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_ZJets"] = (TH1D*)f_ZJets->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_ZJets"] = (TH1D*)f_ZJets->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_ZJets"] = (TH1D*)f_ZJets->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_ZJets"] = (TH1D*)f_ZJets->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_ZJets"] = (TH1D*)f_ZJets->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_ZJets"] = (TH1D*)f_ZJets->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_ZJets"] = (TH1D*)f_ZJets->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_ZJets"] = (TH1D*)f_ZJets->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_ZJets"] = (TH1D*)f_ZJets->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_ZJets"] = (TH1D*)f_ZJets->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_ZJets"] = (TH1D*)f_ZJets->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_ZJets"] = (TH1D*)h_["U_inverted_ZJets"]->Clone();
    h_["predV2_inverted_ZJets"]->Add(h_["D_inverted_ZJets"]);

    h_["predV2_tag_ZJets"] = (TH1D*)h_["U_tag_ZJets"]->Clone();
    h_["predV2_tag_ZJets"]->Add(h_["D_tag_ZJets"]);    
    h_["predV1_tag_ZJets"] = (TH1D*)h_["predV2_tag_ZJets"]->Clone();
    h_["predV1_tag_ZJets"]->Multiply(h_["S_inverted_ZJets"]);
    h_["predV1_tag_ZJets"]->Divide(h_["predV2_inverted_ZJets"]);

    h_["predV2_controlA_ZJets"] = (TH1D*)h_["U_controlA_ZJets"]->Clone();
    h_["predV2_controlA_ZJets"]->Add(h_["D_controlA_ZJets"]);    
    h_["predV1_controlA_ZJets"] = (TH1D*)h_["predV2_controlA_ZJets"]->Clone();
    h_["predV1_controlA_ZJets"]->Multiply(h_["S_inverted_ZJets"]);
    h_["predV1_controlA_ZJets"]->Divide(h_["predV2_inverted_ZJets"]);

    h_["predV2_controlB_ZJets"] = (TH1D*)h_["U_controlB_ZJets"]->Clone();
    h_["predV2_controlB_ZJets"]->Add(h_["D_controlB_ZJets"]);    
    h_["predV1_controlB_ZJets"] = (TH1D*)h_["predV2_controlB_ZJets"]->Clone();
    h_["predV1_controlB_ZJets"]->Multiply(h_["S_inverted_ZJets"]);
    h_["predV1_controlB_ZJets"]->Divide(h_["predV2_inverted_ZJets"]);

    h_["predV2_inverted_ZJets"]->SetBinContent(1, 2 * h_["predV2_inverted_ZJets"]->GetBinContent(1));
    h_["predV2_inverted_ZJets"]->SetBinError(1, 2 * h_["predV2_inverted_ZJets"]->GetBinError(1));
    h_["predV2_tag_ZJets"]->SetBinContent(1, 2 * h_["predV2_tag_ZJets"]->GetBinContent(1));
    h_["predV2_tag_ZJets"]->SetBinError(1, 2 * h_["predV2_tag_ZJets"]->GetBinError(1));
    h_["predV2_controlA_ZJets"]->SetBinContent(1, 2 * h_["predV2_controlA_ZJets"]->GetBinContent(1));
    h_["predV2_controlA_ZJets"]->SetBinError(1, 2 * h_["predV2_controlA_ZJets"]->GetBinError(1));
    h_["predV2_controlB_ZJets"]->SetBinContent(1, 2 * h_["predV2_controlB_ZJets"]->GetBinContent(1));
    h_["predV2_controlB_ZJets"]->SetBinError(1, 2 * h_["predV2_controlB_ZJets"]->GetBinError(1));


    // WJets
    h_["S_tag_WJets"] = (TH1D*)f_WJets->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_WJets"] = (TH1D*)f_WJets->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_WJets"] = (TH1D*)f_WJets->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_WJets"] = (TH1D*)f_WJets->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_WJets"] = (TH1D*)f_WJets->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_WJets"] = (TH1D*)f_WJets->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_WJets"] = (TH1D*)f_WJets->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_WJets"] = (TH1D*)f_WJets->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_WJets"] = (TH1D*)f_WJets->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_WJets"] = (TH1D*)f_WJets->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_WJets"] = (TH1D*)f_WJets->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_WJets"] = (TH1D*)f_WJets->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_WJets"] = (TH1D*)h_["U_inverted_WJets"]->Clone();
    h_["predV2_inverted_WJets"]->Add(h_["D_inverted_WJets"]);

    h_["predV2_tag_WJets"] = (TH1D*)h_["U_tag_WJets"]->Clone();
    h_["predV2_tag_WJets"]->Add(h_["D_tag_WJets"]);    
    h_["predV1_tag_WJets"] = (TH1D*)h_["predV2_tag_WJets"]->Clone();
    h_["predV1_tag_WJets"]->Multiply(h_["S_inverted_WJets"]);
    h_["predV1_tag_WJets"]->Divide(h_["predV2_inverted_WJets"]);

    h_["predV2_controlA_WJets"] = (TH1D*)h_["U_controlA_WJets"]->Clone();
    h_["predV2_controlA_WJets"]->Add(h_["D_controlA_WJets"]);    
    h_["predV1_controlA_WJets"] = (TH1D*)h_["predV2_controlA_WJets"]->Clone();
    h_["predV1_controlA_WJets"]->Multiply(h_["S_inverted_WJets"]);
    h_["predV1_controlA_WJets"]->Divide(h_["predV2_inverted_WJets"]);

    h_["predV2_controlB_WJets"] = (TH1D*)h_["U_controlB_WJets"]->Clone();
    h_["predV2_controlB_WJets"]->Add(h_["D_controlB_WJets"]);    
    h_["predV1_controlB_WJets"] = (TH1D*)h_["predV2_controlB_WJets"]->Clone();
    h_["predV1_controlB_WJets"]->Multiply(h_["S_inverted_WJets"]);
    h_["predV1_controlB_WJets"]->Divide(h_["predV2_inverted_WJets"]);

    h_["predV2_inverted_WJets"]->SetBinContent(1, 2 * h_["predV2_inverted_WJets"]->GetBinContent(1));
    h_["predV2_inverted_WJets"]->SetBinError(1, 2 * h_["predV2_inverted_WJets"]->GetBinError(1));
    h_["predV2_tag_WJets"]->SetBinContent(1, 2 * h_["predV2_tag_WJets"]->GetBinContent(1));
    h_["predV2_tag_WJets"]->SetBinError(1, 2 * h_["predV2_tag_WJets"]->GetBinError(1));
    h_["predV2_controlA_WJets"]->SetBinContent(1, 2 * h_["predV2_controlA_WJets"]->GetBinContent(1));
    h_["predV2_controlA_WJets"]->SetBinError(1, 2 * h_["predV2_controlA_WJets"]->GetBinError(1));
    h_["predV2_controlB_WJets"]->SetBinContent(1, 2 * h_["predV2_controlB_WJets"]->GetBinContent(1));
    h_["predV2_controlB_WJets"]->SetBinError(1, 2 * h_["predV2_controlB_WJets"]->GetBinError(1));


    // mH30mSusy800
    h_["S_tag_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH30mSusy800"] = (TH1D*)f_mH30mSusy800->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH30mSusy800"] = (TH1D*)h_["U_inverted_mH30mSusy800"]->Clone();
    h_["predV2_inverted_mH30mSusy800"]->Add(h_["D_inverted_mH30mSusy800"]);

    h_["predV2_tag_mH30mSusy800"] = (TH1D*)h_["U_tag_mH30mSusy800"]->Clone();
    h_["predV2_tag_mH30mSusy800"]->Add(h_["D_tag_mH30mSusy800"]);    
    h_["predV1_tag_mH30mSusy800"] = (TH1D*)h_["predV2_tag_mH30mSusy800"]->Clone();
    h_["predV1_tag_mH30mSusy800"]->Multiply(h_["S_inverted_mH30mSusy800"]);
    h_["predV1_tag_mH30mSusy800"]->Divide(h_["predV2_inverted_mH30mSusy800"]);

    h_["predV2_controlA_mH30mSusy800"] = (TH1D*)h_["U_controlA_mH30mSusy800"]->Clone();
    h_["predV2_controlA_mH30mSusy800"]->Add(h_["D_controlA_mH30mSusy800"]);    
    h_["predV1_controlA_mH30mSusy800"] = (TH1D*)h_["predV2_controlA_mH30mSusy800"]->Clone();
    h_["predV1_controlA_mH30mSusy800"]->Multiply(h_["S_inverted_mH30mSusy800"]);
    h_["predV1_controlA_mH30mSusy800"]->Divide(h_["predV2_inverted_mH30mSusy800"]);

    h_["predV2_controlB_mH30mSusy800"] = (TH1D*)h_["U_controlB_mH30mSusy800"]->Clone();
    h_["predV2_controlB_mH30mSusy800"]->Add(h_["D_controlB_mH30mSusy800"]);    
    h_["predV1_controlB_mH30mSusy800"] = (TH1D*)h_["predV2_controlB_mH30mSusy800"]->Clone();
    h_["predV1_controlB_mH30mSusy800"]->Multiply(h_["S_inverted_mH30mSusy800"]);
    h_["predV1_controlB_mH30mSusy800"]->Divide(h_["predV2_inverted_mH30mSusy800"]);

    h_["predV2_inverted_mH30mSusy800"]->SetBinContent(1, 2 * h_["predV2_inverted_mH30mSusy800"]->GetBinContent(1));
    h_["predV2_inverted_mH30mSusy800"]->SetBinError(1, 2 * h_["predV2_inverted_mH30mSusy800"]->GetBinError(1));
    h_["predV2_tag_mH30mSusy800"]->SetBinContent(1, 2 * h_["predV2_tag_mH30mSusy800"]->GetBinContent(1));
    h_["predV2_tag_mH30mSusy800"]->SetBinError(1, 2 * h_["predV2_tag_mH30mSusy800"]->GetBinError(1));
    h_["predV2_controlA_mH30mSusy800"]->SetBinContent(1, 2 * h_["predV2_controlA_mH30mSusy800"]->GetBinContent(1));
    h_["predV2_controlA_mH30mSusy800"]->SetBinError(1, 2 * h_["predV2_controlA_mH30mSusy800"]->GetBinError(1));
    h_["predV2_controlB_mH30mSusy800"]->SetBinContent(1, 2 * h_["predV2_controlB_mH30mSusy800"]->GetBinContent(1));
    h_["predV2_controlB_mH30mSusy800"]->SetBinError(1, 2 * h_["predV2_controlB_mH30mSusy800"]->GetBinError(1));


    // mH30mSusy1200
    h_["S_tag_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH30mSusy1200"] = (TH1D*)f_mH30mSusy1200->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH30mSusy1200"] = (TH1D*)h_["U_inverted_mH30mSusy1200"]->Clone();
    h_["predV2_inverted_mH30mSusy1200"]->Add(h_["D_inverted_mH30mSusy1200"]);

    h_["predV2_tag_mH30mSusy1200"] = (TH1D*)h_["U_tag_mH30mSusy1200"]->Clone();
    h_["predV2_tag_mH30mSusy1200"]->Add(h_["D_tag_mH30mSusy1200"]);    
    h_["predV1_tag_mH30mSusy1200"] = (TH1D*)h_["predV2_tag_mH30mSusy1200"]->Clone();
    h_["predV1_tag_mH30mSusy1200"]->Multiply(h_["S_inverted_mH30mSusy1200"]);
    h_["predV1_tag_mH30mSusy1200"]->Divide(h_["predV2_inverted_mH30mSusy1200"]);

    h_["predV2_controlA_mH30mSusy1200"] = (TH1D*)h_["U_controlA_mH30mSusy1200"]->Clone();
    h_["predV2_controlA_mH30mSusy1200"]->Add(h_["D_controlA_mH30mSusy1200"]);    
    h_["predV1_controlA_mH30mSusy1200"] = (TH1D*)h_["predV2_controlA_mH30mSusy1200"]->Clone();
    h_["predV1_controlA_mH30mSusy1200"]->Multiply(h_["S_inverted_mH30mSusy1200"]);
    h_["predV1_controlA_mH30mSusy1200"]->Divide(h_["predV2_inverted_mH30mSusy1200"]);

    h_["predV2_controlB_mH30mSusy1200"] = (TH1D*)h_["U_controlB_mH30mSusy1200"]->Clone();
    h_["predV2_controlB_mH30mSusy1200"]->Add(h_["D_controlB_mH30mSusy1200"]);    
    h_["predV1_controlB_mH30mSusy1200"] = (TH1D*)h_["predV2_controlB_mH30mSusy1200"]->Clone();
    h_["predV1_controlB_mH30mSusy1200"]->Multiply(h_["S_inverted_mH30mSusy1200"]);
    h_["predV1_controlB_mH30mSusy1200"]->Divide(h_["predV2_inverted_mH30mSusy1200"]);

    h_["predV2_inverted_mH30mSusy1200"]->SetBinContent(1, 2 * h_["predV2_inverted_mH30mSusy1200"]->GetBinContent(1));
    h_["predV2_inverted_mH30mSusy1200"]->SetBinError(1, 2 * h_["predV2_inverted_mH30mSusy1200"]->GetBinError(1));
    h_["predV2_tag_mH30mSusy1200"]->SetBinContent(1, 2 * h_["predV2_tag_mH30mSusy1200"]->GetBinContent(1));
    h_["predV2_tag_mH30mSusy1200"]->SetBinError(1, 2 * h_["predV2_tag_mH30mSusy1200"]->GetBinError(1));
    h_["predV2_controlA_mH30mSusy1200"]->SetBinContent(1, 2 * h_["predV2_controlA_mH30mSusy1200"]->GetBinContent(1));
    h_["predV2_controlA_mH30mSusy1200"]->SetBinError(1, 2 * h_["predV2_controlA_mH30mSusy1200"]->GetBinError(1));
    h_["predV2_controlB_mH30mSusy1200"]->SetBinContent(1, 2 * h_["predV2_controlB_mH30mSusy1200"]->GetBinContent(1));
    h_["predV2_controlB_mH30mSusy1200"]->SetBinError(1, 2 * h_["predV2_controlB_mH30mSusy1200"]->GetBinError(1));


    // mH30mSusy1600
    h_["S_tag_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH30mSusy1600"] = (TH1D*)f_mH30mSusy1600->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH30mSusy1600"] = (TH1D*)h_["U_inverted_mH30mSusy1600"]->Clone();
    h_["predV2_inverted_mH30mSusy1600"]->Add(h_["D_inverted_mH30mSusy1600"]);

    h_["predV2_tag_mH30mSusy1600"] = (TH1D*)h_["U_tag_mH30mSusy1600"]->Clone();
    h_["predV2_tag_mH30mSusy1600"]->Add(h_["D_tag_mH30mSusy1600"]);    
    h_["predV1_tag_mH30mSusy1600"] = (TH1D*)h_["predV2_tag_mH30mSusy1600"]->Clone();
    h_["predV1_tag_mH30mSusy1600"]->Multiply(h_["S_inverted_mH30mSusy1600"]);
    h_["predV1_tag_mH30mSusy1600"]->Divide(h_["predV2_inverted_mH30mSusy1600"]);

    h_["predV2_controlA_mH30mSusy1600"] = (TH1D*)h_["U_controlA_mH30mSusy1600"]->Clone();
    h_["predV2_controlA_mH30mSusy1600"]->Add(h_["D_controlA_mH30mSusy1600"]);    
    h_["predV1_controlA_mH30mSusy1600"] = (TH1D*)h_["predV2_controlA_mH30mSusy1600"]->Clone();
    h_["predV1_controlA_mH30mSusy1600"]->Multiply(h_["S_inverted_mH30mSusy1600"]);
    h_["predV1_controlA_mH30mSusy1600"]->Divide(h_["predV2_inverted_mH30mSusy1600"]);

    h_["predV2_controlB_mH30mSusy1600"] = (TH1D*)h_["U_controlB_mH30mSusy1600"]->Clone();
    h_["predV2_controlB_mH30mSusy1600"]->Add(h_["D_controlB_mH30mSusy1600"]);    
    h_["predV1_controlB_mH30mSusy1600"] = (TH1D*)h_["predV2_controlB_mH30mSusy1600"]->Clone();
    h_["predV1_controlB_mH30mSusy1600"]->Multiply(h_["S_inverted_mH30mSusy1600"]);
    h_["predV1_controlB_mH30mSusy1600"]->Divide(h_["predV2_inverted_mH30mSusy1600"]);

    h_["predV2_inverted_mH30mSusy1600"]->SetBinContent(1, 2 * h_["predV2_inverted_mH30mSusy1600"]->GetBinContent(1));
    h_["predV2_inverted_mH30mSusy1600"]->SetBinError(1, 2 * h_["predV2_inverted_mH30mSusy1600"]->GetBinError(1));
    h_["predV2_tag_mH30mSusy1600"]->SetBinContent(1, 2 * h_["predV2_tag_mH30mSusy1600"]->GetBinContent(1));
    h_["predV2_tag_mH30mSusy1600"]->SetBinError(1, 2 * h_["predV2_tag_mH30mSusy1600"]->GetBinError(1));
    h_["predV2_controlA_mH30mSusy1600"]->SetBinContent(1, 2 * h_["predV2_controlA_mH30mSusy1600"]->GetBinContent(1));
    h_["predV2_controlA_mH30mSusy1600"]->SetBinError(1, 2 * h_["predV2_controlA_mH30mSusy1600"]->GetBinError(1));
    h_["predV2_controlB_mH30mSusy1600"]->SetBinContent(1, 2 * h_["predV2_controlB_mH30mSusy1600"]->GetBinContent(1));
    h_["predV2_controlB_mH30mSusy1600"]->SetBinError(1, 2 * h_["predV2_controlB_mH30mSusy1600"]->GetBinError(1));


    // mH30mSusy2000
    h_["S_tag_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH30mSusy2000"] = (TH1D*)f_mH30mSusy2000->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH30mSusy2000"] = (TH1D*)h_["U_inverted_mH30mSusy2000"]->Clone();
    h_["predV2_inverted_mH30mSusy2000"]->Add(h_["D_inverted_mH30mSusy2000"]);

    h_["predV2_tag_mH30mSusy2000"] = (TH1D*)h_["U_tag_mH30mSusy2000"]->Clone();
    h_["predV2_tag_mH30mSusy2000"]->Add(h_["D_tag_mH30mSusy2000"]);    
    h_["predV1_tag_mH30mSusy2000"] = (TH1D*)h_["predV2_tag_mH30mSusy2000"]->Clone();
    h_["predV1_tag_mH30mSusy2000"]->Multiply(h_["S_inverted_mH30mSusy2000"]);
    h_["predV1_tag_mH30mSusy2000"]->Divide(h_["predV2_inverted_mH30mSusy2000"]);

    h_["predV2_controlA_mH30mSusy2000"] = (TH1D*)h_["U_controlA_mH30mSusy2000"]->Clone();
    h_["predV2_controlA_mH30mSusy2000"]->Add(h_["D_controlA_mH30mSusy2000"]);    
    h_["predV1_controlA_mH30mSusy2000"] = (TH1D*)h_["predV2_controlA_mH30mSusy2000"]->Clone();
    h_["predV1_controlA_mH30mSusy2000"]->Multiply(h_["S_inverted_mH30mSusy2000"]);
    h_["predV1_controlA_mH30mSusy2000"]->Divide(h_["predV2_inverted_mH30mSusy2000"]);

    h_["predV2_controlB_mH30mSusy2000"] = (TH1D*)h_["U_controlB_mH30mSusy2000"]->Clone();
    h_["predV2_controlB_mH30mSusy2000"]->Add(h_["D_controlB_mH30mSusy2000"]);    
    h_["predV1_controlB_mH30mSusy2000"] = (TH1D*)h_["predV2_controlB_mH30mSusy2000"]->Clone();
    h_["predV1_controlB_mH30mSusy2000"]->Multiply(h_["S_inverted_mH30mSusy2000"]);
    h_["predV1_controlB_mH30mSusy2000"]->Divide(h_["predV2_inverted_mH30mSusy2000"]);

    h_["predV2_inverted_mH30mSusy2000"]->SetBinContent(1, 2 * h_["predV2_inverted_mH30mSusy2000"]->GetBinContent(1));
    h_["predV2_inverted_mH30mSusy2000"]->SetBinError(1, 2 * h_["predV2_inverted_mH30mSusy2000"]->GetBinError(1));
    h_["predV2_tag_mH30mSusy2000"]->SetBinContent(1, 2 * h_["predV2_tag_mH30mSusy2000"]->GetBinContent(1));
    h_["predV2_tag_mH30mSusy2000"]->SetBinError(1, 2 * h_["predV2_tag_mH30mSusy2000"]->GetBinError(1));
    h_["predV2_controlA_mH30mSusy2000"]->SetBinContent(1, 2 * h_["predV2_controlA_mH30mSusy2000"]->GetBinContent(1));
    h_["predV2_controlA_mH30mSusy2000"]->SetBinError(1, 2 * h_["predV2_controlA_mH30mSusy2000"]->GetBinError(1));
    h_["predV2_controlB_mH30mSusy2000"]->SetBinContent(1, 2 * h_["predV2_controlB_mH30mSusy2000"]->GetBinContent(1));
    h_["predV2_controlB_mH30mSusy2000"]->SetBinError(1, 2 * h_["predV2_controlB_mH30mSusy2000"]->GetBinError(1));


    // mH50mSusy800
    h_["S_tag_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH50mSusy800"] = (TH1D*)f_mH50mSusy800->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH50mSusy800"] = (TH1D*)h_["U_inverted_mH50mSusy800"]->Clone();
    h_["predV2_inverted_mH50mSusy800"]->Add(h_["D_inverted_mH50mSusy800"]);

    h_["predV2_tag_mH50mSusy800"] = (TH1D*)h_["U_tag_mH50mSusy800"]->Clone();
    h_["predV2_tag_mH50mSusy800"]->Add(h_["D_tag_mH50mSusy800"]);    
    h_["predV1_tag_mH50mSusy800"] = (TH1D*)h_["predV2_tag_mH50mSusy800"]->Clone();
    h_["predV1_tag_mH50mSusy800"]->Multiply(h_["S_inverted_mH50mSusy800"]);
    h_["predV1_tag_mH50mSusy800"]->Divide(h_["predV2_inverted_mH50mSusy800"]);

    h_["predV2_controlA_mH50mSusy800"] = (TH1D*)h_["U_controlA_mH50mSusy800"]->Clone();
    h_["predV2_controlA_mH50mSusy800"]->Add(h_["D_controlA_mH50mSusy800"]);    
    h_["predV1_controlA_mH50mSusy800"] = (TH1D*)h_["predV2_controlA_mH50mSusy800"]->Clone();
    h_["predV1_controlA_mH50mSusy800"]->Multiply(h_["S_inverted_mH50mSusy800"]);
    h_["predV1_controlA_mH50mSusy800"]->Divide(h_["predV2_inverted_mH50mSusy800"]);

    h_["predV2_controlB_mH50mSusy800"] = (TH1D*)h_["U_controlB_mH50mSusy800"]->Clone();
    h_["predV2_controlB_mH50mSusy800"]->Add(h_["D_controlB_mH50mSusy800"]);    
    h_["predV1_controlB_mH50mSusy800"] = (TH1D*)h_["predV2_controlB_mH50mSusy800"]->Clone();
    h_["predV1_controlB_mH50mSusy800"]->Multiply(h_["S_inverted_mH50mSusy800"]);
    h_["predV1_controlB_mH50mSusy800"]->Divide(h_["predV2_inverted_mH50mSusy800"]);

    h_["predV2_inverted_mH50mSusy800"]->SetBinContent(1, 2 * h_["predV2_inverted_mH50mSusy800"]->GetBinContent(1));
    h_["predV2_inverted_mH50mSusy800"]->SetBinError(1, 2 * h_["predV2_inverted_mH50mSusy800"]->GetBinError(1));
    h_["predV2_tag_mH50mSusy800"]->SetBinContent(1, 2 * h_["predV2_tag_mH50mSusy800"]->GetBinContent(1));
    h_["predV2_tag_mH50mSusy800"]->SetBinError(1, 2 * h_["predV2_tag_mH50mSusy800"]->GetBinError(1));
    h_["predV2_controlA_mH50mSusy800"]->SetBinContent(1, 2 * h_["predV2_controlA_mH50mSusy800"]->GetBinContent(1));
    h_["predV2_controlA_mH50mSusy800"]->SetBinError(1, 2 * h_["predV2_controlA_mH50mSusy800"]->GetBinError(1));
    h_["predV2_controlB_mH50mSusy800"]->SetBinContent(1, 2 * h_["predV2_controlB_mH50mSusy800"]->GetBinContent(1));
    h_["predV2_controlB_mH50mSusy800"]->SetBinError(1, 2 * h_["predV2_controlB_mH50mSusy800"]->GetBinError(1));


    // mH50mSusy1200
    h_["S_tag_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH50mSusy1200"] = (TH1D*)f_mH50mSusy1200->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH50mSusy1200"] = (TH1D*)h_["U_inverted_mH50mSusy1200"]->Clone();
    h_["predV2_inverted_mH50mSusy1200"]->Add(h_["D_inverted_mH50mSusy1200"]);

    h_["predV2_tag_mH50mSusy1200"] = (TH1D*)h_["U_tag_mH50mSusy1200"]->Clone();
    h_["predV2_tag_mH50mSusy1200"]->Add(h_["D_tag_mH50mSusy1200"]);    
    h_["predV1_tag_mH50mSusy1200"] = (TH1D*)h_["predV2_tag_mH50mSusy1200"]->Clone();
    h_["predV1_tag_mH50mSusy1200"]->Multiply(h_["S_inverted_mH50mSusy1200"]);
    h_["predV1_tag_mH50mSusy1200"]->Divide(h_["predV2_inverted_mH50mSusy1200"]);

    h_["predV2_controlA_mH50mSusy1200"] = (TH1D*)h_["U_controlA_mH50mSusy1200"]->Clone();
    h_["predV2_controlA_mH50mSusy1200"]->Add(h_["D_controlA_mH50mSusy1200"]);    
    h_["predV1_controlA_mH50mSusy1200"] = (TH1D*)h_["predV2_controlA_mH50mSusy1200"]->Clone();
    h_["predV1_controlA_mH50mSusy1200"]->Multiply(h_["S_inverted_mH50mSusy1200"]);
    h_["predV1_controlA_mH50mSusy1200"]->Divide(h_["predV2_inverted_mH50mSusy1200"]);

    h_["predV2_controlB_mH50mSusy1200"] = (TH1D*)h_["U_controlB_mH50mSusy1200"]->Clone();
    h_["predV2_controlB_mH50mSusy1200"]->Add(h_["D_controlB_mH50mSusy1200"]);    
    h_["predV1_controlB_mH50mSusy1200"] = (TH1D*)h_["predV2_controlB_mH50mSusy1200"]->Clone();
    h_["predV1_controlB_mH50mSusy1200"]->Multiply(h_["S_inverted_mH50mSusy1200"]);
    h_["predV1_controlB_mH50mSusy1200"]->Divide(h_["predV2_inverted_mH50mSusy1200"]);

    h_["predV2_inverted_mH50mSusy1200"]->SetBinContent(1, 2 * h_["predV2_inverted_mH50mSusy1200"]->GetBinContent(1));
    h_["predV2_inverted_mH50mSusy1200"]->SetBinError(1, 2 * h_["predV2_inverted_mH50mSusy1200"]->GetBinError(1));
    h_["predV2_tag_mH50mSusy1200"]->SetBinContent(1, 2 * h_["predV2_tag_mH50mSusy1200"]->GetBinContent(1));
    h_["predV2_tag_mH50mSusy1200"]->SetBinError(1, 2 * h_["predV2_tag_mH50mSusy1200"]->GetBinError(1));
    h_["predV2_controlA_mH50mSusy1200"]->SetBinContent(1, 2 * h_["predV2_controlA_mH50mSusy1200"]->GetBinContent(1));
    h_["predV2_controlA_mH50mSusy1200"]->SetBinError(1, 2 * h_["predV2_controlA_mH50mSusy1200"]->GetBinError(1));
    h_["predV2_controlB_mH50mSusy1200"]->SetBinContent(1, 2 * h_["predV2_controlB_mH50mSusy1200"]->GetBinContent(1));
    h_["predV2_controlB_mH50mSusy1200"]->SetBinError(1, 2 * h_["predV2_controlB_mH50mSusy1200"]->GetBinError(1));


    // mH50mSusy1600
    h_["S_tag_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH50mSusy1600"] = (TH1D*)f_mH50mSusy1600->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH50mSusy1600"] = (TH1D*)h_["U_inverted_mH50mSusy1600"]->Clone();
    h_["predV2_inverted_mH50mSusy1600"]->Add(h_["D_inverted_mH50mSusy1600"]);

    h_["predV2_tag_mH50mSusy1600"] = (TH1D*)h_["U_tag_mH50mSusy1600"]->Clone();
    h_["predV2_tag_mH50mSusy1600"]->Add(h_["D_tag_mH50mSusy1600"]);    
    h_["predV1_tag_mH50mSusy1600"] = (TH1D*)h_["predV2_tag_mH50mSusy1600"]->Clone();
    h_["predV1_tag_mH50mSusy1600"]->Multiply(h_["S_inverted_mH50mSusy1600"]);
    h_["predV1_tag_mH50mSusy1600"]->Divide(h_["predV2_inverted_mH50mSusy1600"]);

    h_["predV2_controlA_mH50mSusy1600"] = (TH1D*)h_["U_controlA_mH50mSusy1600"]->Clone();
    h_["predV2_controlA_mH50mSusy1600"]->Add(h_["D_controlA_mH50mSusy1600"]);    
    h_["predV1_controlA_mH50mSusy1600"] = (TH1D*)h_["predV2_controlA_mH50mSusy1600"]->Clone();
    h_["predV1_controlA_mH50mSusy1600"]->Multiply(h_["S_inverted_mH50mSusy1600"]);
    h_["predV1_controlA_mH50mSusy1600"]->Divide(h_["predV2_inverted_mH50mSusy1600"]);

    h_["predV2_controlB_mH50mSusy1600"] = (TH1D*)h_["U_controlB_mH50mSusy1600"]->Clone();
    h_["predV2_controlB_mH50mSusy1600"]->Add(h_["D_controlB_mH50mSusy1600"]);    
    h_["predV1_controlB_mH50mSusy1600"] = (TH1D*)h_["predV2_controlB_mH50mSusy1600"]->Clone();
    h_["predV1_controlB_mH50mSusy1600"]->Multiply(h_["S_inverted_mH50mSusy1600"]);
    h_["predV1_controlB_mH50mSusy1600"]->Divide(h_["predV2_inverted_mH50mSusy1600"]);

    h_["predV2_inverted_mH50mSusy1600"]->SetBinContent(1, 2 * h_["predV2_inverted_mH50mSusy1600"]->GetBinContent(1));
    h_["predV2_inverted_mH50mSusy1600"]->SetBinError(1, 2 * h_["predV2_inverted_mH50mSusy1600"]->GetBinError(1));
    h_["predV2_tag_mH50mSusy1600"]->SetBinContent(1, 2 * h_["predV2_tag_mH50mSusy1600"]->GetBinContent(1));
    h_["predV2_tag_mH50mSusy1600"]->SetBinError(1, 2 * h_["predV2_tag_mH50mSusy1600"]->GetBinError(1));
    h_["predV2_controlA_mH50mSusy1600"]->SetBinContent(1, 2 * h_["predV2_controlA_mH50mSusy1600"]->GetBinContent(1));
    h_["predV2_controlA_mH50mSusy1600"]->SetBinError(1, 2 * h_["predV2_controlA_mH50mSusy1600"]->GetBinError(1));
    h_["predV2_controlB_mH50mSusy1600"]->SetBinContent(1, 2 * h_["predV2_controlB_mH50mSusy1600"]->GetBinContent(1));
    h_["predV2_controlB_mH50mSusy1600"]->SetBinError(1, 2 * h_["predV2_controlB_mH50mSusy1600"]->GetBinError(1));


    // mH50mSusy2000
    h_["S_tag_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH50mSusy2000"] = (TH1D*)f_mH50mSusy2000->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH50mSusy2000"] = (TH1D*)h_["U_inverted_mH50mSusy2000"]->Clone();
    h_["predV2_inverted_mH50mSusy2000"]->Add(h_["D_inverted_mH50mSusy2000"]);

    h_["predV2_tag_mH50mSusy2000"] = (TH1D*)h_["U_tag_mH50mSusy2000"]->Clone();
    h_["predV2_tag_mH50mSusy2000"]->Add(h_["D_tag_mH50mSusy2000"]);    
    h_["predV1_tag_mH50mSusy2000"] = (TH1D*)h_["predV2_tag_mH50mSusy2000"]->Clone();
    h_["predV1_tag_mH50mSusy2000"]->Multiply(h_["S_inverted_mH50mSusy2000"]);
    h_["predV1_tag_mH50mSusy2000"]->Divide(h_["predV2_inverted_mH50mSusy2000"]);

    h_["predV2_controlA_mH50mSusy2000"] = (TH1D*)h_["U_controlA_mH50mSusy2000"]->Clone();
    h_["predV2_controlA_mH50mSusy2000"]->Add(h_["D_controlA_mH50mSusy2000"]);    
    h_["predV1_controlA_mH50mSusy2000"] = (TH1D*)h_["predV2_controlA_mH50mSusy2000"]->Clone();
    h_["predV1_controlA_mH50mSusy2000"]->Multiply(h_["S_inverted_mH50mSusy2000"]);
    h_["predV1_controlA_mH50mSusy2000"]->Divide(h_["predV2_inverted_mH50mSusy2000"]);

    h_["predV2_controlB_mH50mSusy2000"] = (TH1D*)h_["U_controlB_mH50mSusy2000"]->Clone();
    h_["predV2_controlB_mH50mSusy2000"]->Add(h_["D_controlB_mH50mSusy2000"]);    
    h_["predV1_controlB_mH50mSusy2000"] = (TH1D*)h_["predV2_controlB_mH50mSusy2000"]->Clone();
    h_["predV1_controlB_mH50mSusy2000"]->Multiply(h_["S_inverted_mH50mSusy2000"]);
    h_["predV1_controlB_mH50mSusy2000"]->Divide(h_["predV2_inverted_mH50mSusy2000"]);

    h_["predV2_inverted_mH50mSusy2000"]->SetBinContent(1, 2 * h_["predV2_inverted_mH50mSusy2000"]->GetBinContent(1));
    h_["predV2_inverted_mH50mSusy2000"]->SetBinError(1, 2 * h_["predV2_inverted_mH50mSusy2000"]->GetBinError(1));
    h_["predV2_tag_mH50mSusy2000"]->SetBinContent(1, 2 * h_["predV2_tag_mH50mSusy2000"]->GetBinContent(1));
    h_["predV2_tag_mH50mSusy2000"]->SetBinError(1, 2 * h_["predV2_tag_mH50mSusy2000"]->GetBinError(1));
    h_["predV2_controlA_mH50mSusy2000"]->SetBinContent(1, 2 * h_["predV2_controlA_mH50mSusy2000"]->GetBinContent(1));
    h_["predV2_controlA_mH50mSusy2000"]->SetBinError(1, 2 * h_["predV2_controlA_mH50mSusy2000"]->GetBinError(1));
    h_["predV2_controlB_mH50mSusy2000"]->SetBinContent(1, 2 * h_["predV2_controlB_mH50mSusy2000"]->GetBinContent(1));
    h_["predV2_controlB_mH50mSusy2000"]->SetBinError(1, 2 * h_["predV2_controlB_mH50mSusy2000"]->GetBinError(1));


    // mH70mSusy800
    h_["S_tag_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH70mSusy800"] = (TH1D*)f_mH70mSusy800->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH70mSusy800"] = (TH1D*)h_["U_inverted_mH70mSusy800"]->Clone();
    h_["predV2_inverted_mH70mSusy800"]->Add(h_["D_inverted_mH70mSusy800"]);

    h_["predV2_tag_mH70mSusy800"] = (TH1D*)h_["U_tag_mH70mSusy800"]->Clone();
    h_["predV2_tag_mH70mSusy800"]->Add(h_["D_tag_mH70mSusy800"]);    
    h_["predV1_tag_mH70mSusy800"] = (TH1D*)h_["predV2_tag_mH70mSusy800"]->Clone();
    h_["predV1_tag_mH70mSusy800"]->Multiply(h_["S_inverted_mH70mSusy800"]);
    h_["predV1_tag_mH70mSusy800"]->Divide(h_["predV2_inverted_mH70mSusy800"]);

    h_["predV2_controlA_mH70mSusy800"] = (TH1D*)h_["U_controlA_mH70mSusy800"]->Clone();
    h_["predV2_controlA_mH70mSusy800"]->Add(h_["D_controlA_mH70mSusy800"]);    
    h_["predV1_controlA_mH70mSusy800"] = (TH1D*)h_["predV2_controlA_mH70mSusy800"]->Clone();
    h_["predV1_controlA_mH70mSusy800"]->Multiply(h_["S_inverted_mH70mSusy800"]);
    h_["predV1_controlA_mH70mSusy800"]->Divide(h_["predV2_inverted_mH70mSusy800"]);

    h_["predV2_controlB_mH70mSusy800"] = (TH1D*)h_["U_controlB_mH70mSusy800"]->Clone();
    h_["predV2_controlB_mH70mSusy800"]->Add(h_["D_controlB_mH70mSusy800"]);    
    h_["predV1_controlB_mH70mSusy800"] = (TH1D*)h_["predV2_controlB_mH70mSusy800"]->Clone();
    h_["predV1_controlB_mH70mSusy800"]->Multiply(h_["S_inverted_mH70mSusy800"]);
    h_["predV1_controlB_mH70mSusy800"]->Divide(h_["predV2_inverted_mH70mSusy800"]);

    h_["predV2_inverted_mH70mSusy800"]->SetBinContent(1, 2 * h_["predV2_inverted_mH70mSusy800"]->GetBinContent(1));
    h_["predV2_inverted_mH70mSusy800"]->SetBinError(1, 2 * h_["predV2_inverted_mH70mSusy800"]->GetBinError(1));
    h_["predV2_tag_mH70mSusy800"]->SetBinContent(1, 2 * h_["predV2_tag_mH70mSusy800"]->GetBinContent(1));
    h_["predV2_tag_mH70mSusy800"]->SetBinError(1, 2 * h_["predV2_tag_mH70mSusy800"]->GetBinError(1));
    h_["predV2_controlA_mH70mSusy800"]->SetBinContent(1, 2 * h_["predV2_controlA_mH70mSusy800"]->GetBinContent(1));
    h_["predV2_controlA_mH70mSusy800"]->SetBinError(1, 2 * h_["predV2_controlA_mH70mSusy800"]->GetBinError(1));
    h_["predV2_controlB_mH70mSusy800"]->SetBinContent(1, 2 * h_["predV2_controlB_mH70mSusy800"]->GetBinContent(1));
    h_["predV2_controlB_mH70mSusy800"]->SetBinError(1, 2 * h_["predV2_controlB_mH70mSusy800"]->GetBinError(1));


    // mH70mSusy1200
    h_["S_tag_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH70mSusy1200"] = (TH1D*)f_mH70mSusy1200->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH70mSusy1200"] = (TH1D*)h_["U_inverted_mH70mSusy1200"]->Clone();
    h_["predV2_inverted_mH70mSusy1200"]->Add(h_["D_inverted_mH70mSusy1200"]);

    h_["predV2_tag_mH70mSusy1200"] = (TH1D*)h_["U_tag_mH70mSusy1200"]->Clone();
    h_["predV2_tag_mH70mSusy1200"]->Add(h_["D_tag_mH70mSusy1200"]);    
    h_["predV1_tag_mH70mSusy1200"] = (TH1D*)h_["predV2_tag_mH70mSusy1200"]->Clone();
    h_["predV1_tag_mH70mSusy1200"]->Multiply(h_["S_inverted_mH70mSusy1200"]);
    h_["predV1_tag_mH70mSusy1200"]->Divide(h_["predV2_inverted_mH70mSusy1200"]);

    h_["predV2_controlA_mH70mSusy1200"] = (TH1D*)h_["U_controlA_mH70mSusy1200"]->Clone();
    h_["predV2_controlA_mH70mSusy1200"]->Add(h_["D_controlA_mH70mSusy1200"]);    
    h_["predV1_controlA_mH70mSusy1200"] = (TH1D*)h_["predV2_controlA_mH70mSusy1200"]->Clone();
    h_["predV1_controlA_mH70mSusy1200"]->Multiply(h_["S_inverted_mH70mSusy1200"]);
    h_["predV1_controlA_mH70mSusy1200"]->Divide(h_["predV2_inverted_mH70mSusy1200"]);

    h_["predV2_controlB_mH70mSusy1200"] = (TH1D*)h_["U_controlB_mH70mSusy1200"]->Clone();
    h_["predV2_controlB_mH70mSusy1200"]->Add(h_["D_controlB_mH70mSusy1200"]);    
    h_["predV1_controlB_mH70mSusy1200"] = (TH1D*)h_["predV2_controlB_mH70mSusy1200"]->Clone();
    h_["predV1_controlB_mH70mSusy1200"]->Multiply(h_["S_inverted_mH70mSusy1200"]);
    h_["predV1_controlB_mH70mSusy1200"]->Divide(h_["predV2_inverted_mH70mSusy1200"]);

    h_["predV2_inverted_mH70mSusy1200"]->SetBinContent(1, 2 * h_["predV2_inverted_mH70mSusy1200"]->GetBinContent(1));
    h_["predV2_inverted_mH70mSusy1200"]->SetBinError(1, 2 * h_["predV2_inverted_mH70mSusy1200"]->GetBinError(1));
    h_["predV2_tag_mH70mSusy1200"]->SetBinContent(1, 2 * h_["predV2_tag_mH70mSusy1200"]->GetBinContent(1));
    h_["predV2_tag_mH70mSusy1200"]->SetBinError(1, 2 * h_["predV2_tag_mH70mSusy1200"]->GetBinError(1));
    h_["predV2_controlA_mH70mSusy1200"]->SetBinContent(1, 2 * h_["predV2_controlA_mH70mSusy1200"]->GetBinContent(1));
    h_["predV2_controlA_mH70mSusy1200"]->SetBinError(1, 2 * h_["predV2_controlA_mH70mSusy1200"]->GetBinError(1));
    h_["predV2_controlB_mH70mSusy1200"]->SetBinContent(1, 2 * h_["predV2_controlB_mH70mSusy1200"]->GetBinContent(1));
    h_["predV2_controlB_mH70mSusy1200"]->SetBinError(1, 2 * h_["predV2_controlB_mH70mSusy1200"]->GetBinError(1));


    // mH70mSusy1600
    h_["S_tag_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH70mSusy1600"] = (TH1D*)f_mH70mSusy1600->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH70mSusy1600"] = (TH1D*)h_["U_inverted_mH70mSusy1600"]->Clone();
    h_["predV2_inverted_mH70mSusy1600"]->Add(h_["D_inverted_mH70mSusy1600"]);

    h_["predV2_tag_mH70mSusy1600"] = (TH1D*)h_["U_tag_mH70mSusy1600"]->Clone();
    h_["predV2_tag_mH70mSusy1600"]->Add(h_["D_tag_mH70mSusy1600"]);    
    h_["predV1_tag_mH70mSusy1600"] = (TH1D*)h_["predV2_tag_mH70mSusy1600"]->Clone();
    h_["predV1_tag_mH70mSusy1600"]->Multiply(h_["S_inverted_mH70mSusy1600"]);
    h_["predV1_tag_mH70mSusy1600"]->Divide(h_["predV2_inverted_mH70mSusy1600"]);

    h_["predV2_controlA_mH70mSusy1600"] = (TH1D*)h_["U_controlA_mH70mSusy1600"]->Clone();
    h_["predV2_controlA_mH70mSusy1600"]->Add(h_["D_controlA_mH70mSusy1600"]);    
    h_["predV1_controlA_mH70mSusy1600"] = (TH1D*)h_["predV2_controlA_mH70mSusy1600"]->Clone();
    h_["predV1_controlA_mH70mSusy1600"]->Multiply(h_["S_inverted_mH70mSusy1600"]);
    h_["predV1_controlA_mH70mSusy1600"]->Divide(h_["predV2_inverted_mH70mSusy1600"]);

    h_["predV2_controlB_mH70mSusy1600"] = (TH1D*)h_["U_controlB_mH70mSusy1600"]->Clone();
    h_["predV2_controlB_mH70mSusy1600"]->Add(h_["D_controlB_mH70mSusy1600"]);    
    h_["predV1_controlB_mH70mSusy1600"] = (TH1D*)h_["predV2_controlB_mH70mSusy1600"]->Clone();
    h_["predV1_controlB_mH70mSusy1600"]->Multiply(h_["S_inverted_mH70mSusy1600"]);
    h_["predV1_controlB_mH70mSusy1600"]->Divide(h_["predV2_inverted_mH70mSusy1600"]);

    h_["predV2_inverted_mH70mSusy1600"]->SetBinContent(1, 2 * h_["predV2_inverted_mH70mSusy1600"]->GetBinContent(1));
    h_["predV2_inverted_mH70mSusy1600"]->SetBinError(1, 2 * h_["predV2_inverted_mH70mSusy1600"]->GetBinError(1));
    h_["predV2_tag_mH70mSusy1600"]->SetBinContent(1, 2 * h_["predV2_tag_mH70mSusy1600"]->GetBinContent(1));
    h_["predV2_tag_mH70mSusy1600"]->SetBinError(1, 2 * h_["predV2_tag_mH70mSusy1600"]->GetBinError(1));
    h_["predV2_controlA_mH70mSusy1600"]->SetBinContent(1, 2 * h_["predV2_controlA_mH70mSusy1600"]->GetBinContent(1));
    h_["predV2_controlA_mH70mSusy1600"]->SetBinError(1, 2 * h_["predV2_controlA_mH70mSusy1600"]->GetBinError(1));
    h_["predV2_controlB_mH70mSusy1600"]->SetBinContent(1, 2 * h_["predV2_controlB_mH70mSusy1600"]->GetBinContent(1));
    h_["predV2_controlB_mH70mSusy1600"]->SetBinError(1, 2 * h_["predV2_controlB_mH70mSusy1600"]->GetBinError(1));


    // mH70mSusy2000
    h_["S_tag_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH70mSusy2000"] = (TH1D*)f_mH70mSusy2000->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH70mSusy2000"] = (TH1D*)h_["U_inverted_mH70mSusy2000"]->Clone();
    h_["predV2_inverted_mH70mSusy2000"]->Add(h_["D_inverted_mH70mSusy2000"]);

    h_["predV2_tag_mH70mSusy2000"] = (TH1D*)h_["U_tag_mH70mSusy2000"]->Clone();
    h_["predV2_tag_mH70mSusy2000"]->Add(h_["D_tag_mH70mSusy2000"]);    
    h_["predV1_tag_mH70mSusy2000"] = (TH1D*)h_["predV2_tag_mH70mSusy2000"]->Clone();
    h_["predV1_tag_mH70mSusy2000"]->Multiply(h_["S_inverted_mH70mSusy2000"]);
    h_["predV1_tag_mH70mSusy2000"]->Divide(h_["predV2_inverted_mH70mSusy2000"]);

    h_["predV2_controlA_mH70mSusy2000"] = (TH1D*)h_["U_controlA_mH70mSusy2000"]->Clone();
    h_["predV2_controlA_mH70mSusy2000"]->Add(h_["D_controlA_mH70mSusy2000"]);    
    h_["predV1_controlA_mH70mSusy2000"] = (TH1D*)h_["predV2_controlA_mH70mSusy2000"]->Clone();
    h_["predV1_controlA_mH70mSusy2000"]->Multiply(h_["S_inverted_mH70mSusy2000"]);
    h_["predV1_controlA_mH70mSusy2000"]->Divide(h_["predV2_inverted_mH70mSusy2000"]);

    h_["predV2_controlB_mH70mSusy2000"] = (TH1D*)h_["U_controlB_mH70mSusy2000"]->Clone();
    h_["predV2_controlB_mH70mSusy2000"]->Add(h_["D_controlB_mH70mSusy2000"]);    
    h_["predV1_controlB_mH70mSusy2000"] = (TH1D*)h_["predV2_controlB_mH70mSusy2000"]->Clone();
    h_["predV1_controlB_mH70mSusy2000"]->Multiply(h_["S_inverted_mH70mSusy2000"]);
    h_["predV1_controlB_mH70mSusy2000"]->Divide(h_["predV2_inverted_mH70mSusy2000"]);

    h_["predV2_inverted_mH70mSusy2000"]->SetBinContent(1, 2 * h_["predV2_inverted_mH70mSusy2000"]->GetBinContent(1));
    h_["predV2_inverted_mH70mSusy2000"]->SetBinError(1, 2 * h_["predV2_inverted_mH70mSusy2000"]->GetBinError(1));
    h_["predV2_tag_mH70mSusy2000"]->SetBinContent(1, 2 * h_["predV2_tag_mH70mSusy2000"]->GetBinContent(1));
    h_["predV2_tag_mH70mSusy2000"]->SetBinError(1, 2 * h_["predV2_tag_mH70mSusy2000"]->GetBinError(1));
    h_["predV2_controlA_mH70mSusy2000"]->SetBinContent(1, 2 * h_["predV2_controlA_mH70mSusy2000"]->GetBinContent(1));
    h_["predV2_controlA_mH70mSusy2000"]->SetBinError(1, 2 * h_["predV2_controlA_mH70mSusy2000"]->GetBinError(1));
    h_["predV2_controlB_mH70mSusy2000"]->SetBinContent(1, 2 * h_["predV2_controlB_mH70mSusy2000"]->GetBinContent(1));
    h_["predV2_controlB_mH70mSusy2000"]->SetBinError(1, 2 * h_["predV2_controlB_mH70mSusy2000"]->GetBinError(1));


    // mH90mSusy800
    h_["S_tag_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH90mSusy800"] = (TH1D*)f_mH90mSusy800->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH90mSusy800"] = (TH1D*)h_["U_inverted_mH90mSusy800"]->Clone();
    h_["predV2_inverted_mH90mSusy800"]->Add(h_["D_inverted_mH90mSusy800"]);

    h_["predV2_tag_mH90mSusy800"] = (TH1D*)h_["U_tag_mH90mSusy800"]->Clone();
    h_["predV2_tag_mH90mSusy800"]->Add(h_["D_tag_mH90mSusy800"]);    
    h_["predV1_tag_mH90mSusy800"] = (TH1D*)h_["predV2_tag_mH90mSusy800"]->Clone();
    h_["predV1_tag_mH90mSusy800"]->Multiply(h_["S_inverted_mH90mSusy800"]);
    h_["predV1_tag_mH90mSusy800"]->Divide(h_["predV2_inverted_mH90mSusy800"]);

    h_["predV2_controlA_mH90mSusy800"] = (TH1D*)h_["U_controlA_mH90mSusy800"]->Clone();
    h_["predV2_controlA_mH90mSusy800"]->Add(h_["D_controlA_mH90mSusy800"]);    
    h_["predV1_controlA_mH90mSusy800"] = (TH1D*)h_["predV2_controlA_mH90mSusy800"]->Clone();
    h_["predV1_controlA_mH90mSusy800"]->Multiply(h_["S_inverted_mH90mSusy800"]);
    h_["predV1_controlA_mH90mSusy800"]->Divide(h_["predV2_inverted_mH90mSusy800"]);

    h_["predV2_controlB_mH90mSusy800"] = (TH1D*)h_["U_controlB_mH90mSusy800"]->Clone();
    h_["predV2_controlB_mH90mSusy800"]->Add(h_["D_controlB_mH90mSusy800"]);    
    h_["predV1_controlB_mH90mSusy800"] = (TH1D*)h_["predV2_controlB_mH90mSusy800"]->Clone();
    h_["predV1_controlB_mH90mSusy800"]->Multiply(h_["S_inverted_mH90mSusy800"]);
    h_["predV1_controlB_mH90mSusy800"]->Divide(h_["predV2_inverted_mH90mSusy800"]);

    h_["predV2_inverted_mH90mSusy800"]->SetBinContent(1, 2 * h_["predV2_inverted_mH90mSusy800"]->GetBinContent(1));
    h_["predV2_inverted_mH90mSusy800"]->SetBinError(1, 2 * h_["predV2_inverted_mH90mSusy800"]->GetBinError(1));
    h_["predV2_tag_mH90mSusy800"]->SetBinContent(1, 2 * h_["predV2_tag_mH90mSusy800"]->GetBinContent(1));
    h_["predV2_tag_mH90mSusy800"]->SetBinError(1, 2 * h_["predV2_tag_mH90mSusy800"]->GetBinError(1));
    h_["predV2_controlA_mH90mSusy800"]->SetBinContent(1, 2 * h_["predV2_controlA_mH90mSusy800"]->GetBinContent(1));
    h_["predV2_controlA_mH90mSusy800"]->SetBinError(1, 2 * h_["predV2_controlA_mH90mSusy800"]->GetBinError(1));
    h_["predV2_controlB_mH90mSusy800"]->SetBinContent(1, 2 * h_["predV2_controlB_mH90mSusy800"]->GetBinContent(1));
    h_["predV2_controlB_mH90mSusy800"]->SetBinError(1, 2 * h_["predV2_controlB_mH90mSusy800"]->GetBinError(1));


    // mH90mSusy1200
    h_["S_tag_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH90mSusy1200"] = (TH1D*)f_mH90mSusy1200->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH90mSusy1200"] = (TH1D*)h_["U_inverted_mH90mSusy1200"]->Clone();
    h_["predV2_inverted_mH90mSusy1200"]->Add(h_["D_inverted_mH90mSusy1200"]);

    h_["predV2_tag_mH90mSusy1200"] = (TH1D*)h_["U_tag_mH90mSusy1200"]->Clone();
    h_["predV2_tag_mH90mSusy1200"]->Add(h_["D_tag_mH90mSusy1200"]);    
    h_["predV1_tag_mH90mSusy1200"] = (TH1D*)h_["predV2_tag_mH90mSusy1200"]->Clone();
    h_["predV1_tag_mH90mSusy1200"]->Multiply(h_["S_inverted_mH90mSusy1200"]);
    h_["predV1_tag_mH90mSusy1200"]->Divide(h_["predV2_inverted_mH90mSusy1200"]);

    h_["predV2_controlA_mH90mSusy1200"] = (TH1D*)h_["U_controlA_mH90mSusy1200"]->Clone();
    h_["predV2_controlA_mH90mSusy1200"]->Add(h_["D_controlA_mH90mSusy1200"]);    
    h_["predV1_controlA_mH90mSusy1200"] = (TH1D*)h_["predV2_controlA_mH90mSusy1200"]->Clone();
    h_["predV1_controlA_mH90mSusy1200"]->Multiply(h_["S_inverted_mH90mSusy1200"]);
    h_["predV1_controlA_mH90mSusy1200"]->Divide(h_["predV2_inverted_mH90mSusy1200"]);

    h_["predV2_controlB_mH90mSusy1200"] = (TH1D*)h_["U_controlB_mH90mSusy1200"]->Clone();
    h_["predV2_controlB_mH90mSusy1200"]->Add(h_["D_controlB_mH90mSusy1200"]);    
    h_["predV1_controlB_mH90mSusy1200"] = (TH1D*)h_["predV2_controlB_mH90mSusy1200"]->Clone();
    h_["predV1_controlB_mH90mSusy1200"]->Multiply(h_["S_inverted_mH90mSusy1200"]);
    h_["predV1_controlB_mH90mSusy1200"]->Divide(h_["predV2_inverted_mH90mSusy1200"]);

    h_["predV2_inverted_mH90mSusy1200"]->SetBinContent(1, 2 * h_["predV2_inverted_mH90mSusy1200"]->GetBinContent(1));
    h_["predV2_inverted_mH90mSusy1200"]->SetBinError(1, 2 * h_["predV2_inverted_mH90mSusy1200"]->GetBinError(1));
    h_["predV2_tag_mH90mSusy1200"]->SetBinContent(1, 2 * h_["predV2_tag_mH90mSusy1200"]->GetBinContent(1));
    h_["predV2_tag_mH90mSusy1200"]->SetBinError(1, 2 * h_["predV2_tag_mH90mSusy1200"]->GetBinError(1));
    h_["predV2_controlA_mH90mSusy1200"]->SetBinContent(1, 2 * h_["predV2_controlA_mH90mSusy1200"]->GetBinContent(1));
    h_["predV2_controlA_mH90mSusy1200"]->SetBinError(1, 2 * h_["predV2_controlA_mH90mSusy1200"]->GetBinError(1));
    h_["predV2_controlB_mH90mSusy1200"]->SetBinContent(1, 2 * h_["predV2_controlB_mH90mSusy1200"]->GetBinContent(1));
    h_["predV2_controlB_mH90mSusy1200"]->SetBinError(1, 2 * h_["predV2_controlB_mH90mSusy1200"]->GetBinError(1));


    // mH90mSusy1600
    h_["S_tag_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH90mSusy1600"] = (TH1D*)f_mH90mSusy1600->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH90mSusy1600"] = (TH1D*)h_["U_inverted_mH90mSusy1600"]->Clone();
    h_["predV2_inverted_mH90mSusy1600"]->Add(h_["D_inverted_mH90mSusy1600"]);

    h_["predV2_tag_mH90mSusy1600"] = (TH1D*)h_["U_tag_mH90mSusy1600"]->Clone();
    h_["predV2_tag_mH90mSusy1600"]->Add(h_["D_tag_mH90mSusy1600"]);    
    h_["predV1_tag_mH90mSusy1600"] = (TH1D*)h_["predV2_tag_mH90mSusy1600"]->Clone();
    h_["predV1_tag_mH90mSusy1600"]->Multiply(h_["S_inverted_mH90mSusy1600"]);
    h_["predV1_tag_mH90mSusy1600"]->Divide(h_["predV2_inverted_mH90mSusy1600"]);

    h_["predV2_controlA_mH90mSusy1600"] = (TH1D*)h_["U_controlA_mH90mSusy1600"]->Clone();
    h_["predV2_controlA_mH90mSusy1600"]->Add(h_["D_controlA_mH90mSusy1600"]);    
    h_["predV1_controlA_mH90mSusy1600"] = (TH1D*)h_["predV2_controlA_mH90mSusy1600"]->Clone();
    h_["predV1_controlA_mH90mSusy1600"]->Multiply(h_["S_inverted_mH90mSusy1600"]);
    h_["predV1_controlA_mH90mSusy1600"]->Divide(h_["predV2_inverted_mH90mSusy1600"]);

    h_["predV2_controlB_mH90mSusy1600"] = (TH1D*)h_["U_controlB_mH90mSusy1600"]->Clone();
    h_["predV2_controlB_mH90mSusy1600"]->Add(h_["D_controlB_mH90mSusy1600"]);    
    h_["predV1_controlB_mH90mSusy1600"] = (TH1D*)h_["predV2_controlB_mH90mSusy1600"]->Clone();
    h_["predV1_controlB_mH90mSusy1600"]->Multiply(h_["S_inverted_mH90mSusy1600"]);
    h_["predV1_controlB_mH90mSusy1600"]->Divide(h_["predV2_inverted_mH90mSusy1600"]);

    h_["predV2_inverted_mH90mSusy1600"]->SetBinContent(1, 2 * h_["predV2_inverted_mH90mSusy1600"]->GetBinContent(1));
    h_["predV2_inverted_mH90mSusy1600"]->SetBinError(1, 2 * h_["predV2_inverted_mH90mSusy1600"]->GetBinError(1));
    h_["predV2_tag_mH90mSusy1600"]->SetBinContent(1, 2 * h_["predV2_tag_mH90mSusy1600"]->GetBinContent(1));
    h_["predV2_tag_mH90mSusy1600"]->SetBinError(1, 2 * h_["predV2_tag_mH90mSusy1600"]->GetBinError(1));
    h_["predV2_controlA_mH90mSusy1600"]->SetBinContent(1, 2 * h_["predV2_controlA_mH90mSusy1600"]->GetBinContent(1));
    h_["predV2_controlA_mH90mSusy1600"]->SetBinError(1, 2 * h_["predV2_controlA_mH90mSusy1600"]->GetBinError(1));
    h_["predV2_controlB_mH90mSusy1600"]->SetBinContent(1, 2 * h_["predV2_controlB_mH90mSusy1600"]->GetBinContent(1));
    h_["predV2_controlB_mH90mSusy1600"]->SetBinError(1, 2 * h_["predV2_controlB_mH90mSusy1600"]->GetBinError(1));


    // mH90mSusy2000
    h_["S_tag_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("S_dbtMed2MaxAndMed2Max");
    h_["U_tag_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("U_dbtMed2MaxAndMed2Max");
    h_["D_tag_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("D_dbtMed2MaxAndMed2Max");

    h_["S_inverted_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("S_dbtOffLooseAndOffLoose");
    h_["U_inverted_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("U_dbtOffLooseAndOffLoose");
    h_["D_inverted_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("D_dbtOffLooseAndOffLoose");

    h_["S_controlA_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("S_dbtMed2MaxAndOffIDBTCv1");
    h_["U_controlA_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("U_dbtMed2MaxAndOffIDBTCv1");
    h_["D_controlA_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("D_dbtMed2MaxAndOffIDBTCv1");

    h_["S_controlB_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("S_dbtOffIDBTCv1AndMed2Max");
    h_["U_controlB_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("U_dbtOffIDBTCv1AndMed2Max");
    h_["D_controlB_mH90mSusy2000"] = (TH1D*)f_mH90mSusy2000->Get("D_dbtOffIDBTCv1AndMed2Max");

    h_["predV2_inverted_mH90mSusy2000"] = (TH1D*)h_["U_inverted_mH90mSusy2000"]->Clone();
    h_["predV2_inverted_mH90mSusy2000"]->Add(h_["D_inverted_mH90mSusy2000"]);

    h_["predV2_tag_mH90mSusy2000"] = (TH1D*)h_["U_tag_mH90mSusy2000"]->Clone();
    h_["predV2_tag_mH90mSusy2000"]->Add(h_["D_tag_mH90mSusy2000"]);    
    h_["predV1_tag_mH90mSusy2000"] = (TH1D*)h_["predV2_tag_mH90mSusy2000"]->Clone();
    h_["predV1_tag_mH90mSusy2000"]->Multiply(h_["S_inverted_mH90mSusy2000"]);
    h_["predV1_tag_mH90mSusy2000"]->Divide(h_["predV2_inverted_mH90mSusy2000"]);

    h_["predV2_controlA_mH90mSusy2000"] = (TH1D*)h_["U_controlA_mH90mSusy2000"]->Clone();
    h_["predV2_controlA_mH90mSusy2000"]->Add(h_["D_controlA_mH90mSusy2000"]);    
    h_["predV1_controlA_mH90mSusy2000"] = (TH1D*)h_["predV2_controlA_mH90mSusy2000"]->Clone();
    h_["predV1_controlA_mH90mSusy2000"]->Multiply(h_["S_inverted_mH90mSusy2000"]);
    h_["predV1_controlA_mH90mSusy2000"]->Divide(h_["predV2_inverted_mH90mSusy2000"]);

    h_["predV2_controlB_mH90mSusy2000"] = (TH1D*)h_["U_controlB_mH90mSusy2000"]->Clone();
    h_["predV2_controlB_mH90mSusy2000"]->Add(h_["D_controlB_mH90mSusy2000"]);    
    h_["predV1_controlB_mH90mSusy2000"] = (TH1D*)h_["predV2_controlB_mH90mSusy2000"]->Clone();
    h_["predV1_controlB_mH90mSusy2000"]->Multiply(h_["S_inverted_mH90mSusy2000"]);
    h_["predV1_controlB_mH90mSusy2000"]->Divide(h_["predV2_inverted_mH90mSusy2000"]);

    h_["predV2_inverted_mH90mSusy2000"]->SetBinContent(1, 2 * h_["predV2_inverted_mH90mSusy2000"]->GetBinContent(1));
    h_["predV2_inverted_mH90mSusy2000"]->SetBinError(1, 2 * h_["predV2_inverted_mH90mSusy2000"]->GetBinError(1));
    h_["predV2_tag_mH90mSusy2000"]->SetBinContent(1, 2 * h_["predV2_tag_mH90mSusy2000"]->GetBinContent(1));
    h_["predV2_tag_mH90mSusy2000"]->SetBinError(1, 2 * h_["predV2_tag_mH90mSusy2000"]->GetBinError(1));
    h_["predV2_controlA_mH90mSusy2000"]->SetBinContent(1, 2 * h_["predV2_controlA_mH90mSusy2000"]->GetBinContent(1));
    h_["predV2_controlA_mH90mSusy2000"]->SetBinError(1, 2 * h_["predV2_controlA_mH90mSusy2000"]->GetBinError(1));
    h_["predV2_controlB_mH90mSusy2000"]->SetBinContent(1, 2 * h_["predV2_controlB_mH90mSusy2000"]->GetBinContent(1));
    h_["predV2_controlB_mH90mSusy2000"]->SetBinError(1, 2 * h_["predV2_controlB_mH90mSusy2000"]->GetBinError(1));

    return h_;
}