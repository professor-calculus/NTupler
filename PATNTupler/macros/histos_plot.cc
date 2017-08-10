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
#include "MacrosOnCondor.h"


int main(int argc, char** argv){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

	double luminosity = 50.0;

    TFile * f_QCD = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_20/MassCutsV03/QCD/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TH1D * h_S_tag_QCD = (TH1D*)f_QCD->Get("S_dbtMed2MaxAndMed2Max");
    // TH1D * h_S_inverted_QCD = (TH1D*)f_QCD->Get("S_dbtOffLooseAndOffLoose");
    // TH1D * h_S_controlA_QCD = (TH1D*)f_QCD->Get("S_dbtMed2MaxAndOffIDBTCv1");
    // TH1D * h_S_controlB_QCD = (TH1D*)f_QCD->Get("S_dbtOffIDBTCv1AndMed2Max");

    TFile * f_TTJets = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_20/MassCutsV03/TTJets/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TH1D * h_S_tag_TTJets = (TH1D*)f_TTJets->Get("S_dbtMed2MaxAndMed2Max");
    // TH1D * h_S_inverted_TTJets = (TH1D*)f_TTJets->Get("S_dbtOffLooseAndOffLoose");
    // TH1D * h_S_controlA_TTJets = (TH1D*)f_TTJets->Get("S_dbtMed2MaxAndOffIDBTCv1");
    // TH1D * h_S_controlB_TTJets = (TH1D*)f_TTJets->Get("S_dbtOffIDBTCv1AndMed2Max");

    TFile * f_ZJets = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_20/MassCutsV03/ZJets/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TH1D * h_S_tag_ZJets = (TH1D*)f_ZJets->Get("S_dbtMed2MaxAndMed2Max");
    // TH1D * h_S_inverted_ZJets = (TH1D*)f_ZJets->Get("S_dbtOffLooseAndOffLoose");
    // TH1D * h_S_controlA_ZJets = (TH1D*)f_ZJets->Get("S_dbtMed2MaxAndOffIDBTCv1");
    // TH1D * h_S_controlB_ZJets = (TH1D*)f_ZJets->Get("S_dbtOffIDBTCv1AndMed2Max");

    TFile * f_WJets = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_20/MassCutsV03/WJets/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TH1D * h_S_tag_WJets = (TH1D*)f_WJets->Get("S_dbtMed2MaxAndMed2Max");
    // TH1D * h_S_inverted_WJets = (TH1D*)f_WJets->Get("S_dbtOffLooseAndOffLoose");
    // TH1D * h_S_controlA_WJets = (TH1D*)f_WJets->Get("S_dbtMed2MaxAndOffIDBTCv1");
    // TH1D * h_S_controlB_WJets = (TH1D*)f_WJets->Get("S_dbtOffIDBTCv1AndMed2Max");

    TFile * f_mH70mSusy1600 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_20/MassCutsV03/mH70_mSusy1600/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TH1D * h_S_tag_mH70mSusy1600 = (TH1D*)f_mH70mSusy1600->Get("S_dbtMed2MaxAndMed2Max");
    // TH1D * h_S_inverted_mH70mSusy1600 = (TH1D*)f_mH70mSusy1600->Get("S_dbtOffLooseAndOffLoose");
    // TH1D * h_S_controlA_mH70mSusy1600 = (TH1D*)f_mH70mSusy1600->Get("S_dbtMed2MaxAndOffIDBTCv1");
    // TH1D * h_S_controlB_mH70mSusy1600 = (TH1D*)f_mH70mSusy1600->Get("S_dbtOffIDBTCv1AndMed2Max");

    TFile * f_mH70mSusy2000 = new TFile("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_20/MassCutsV03/mH70_mSusy2000/MassCutsV03_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root");
    TH1D * h_S_tag_mH70mSusy2000 = (TH1D*)f_mH70mSusy2000->Get("S_dbtMed2MaxAndMed2Max");
    // TH1D * h_S_inverted_mH70mSusy2000 = (TH1D*)f_mH70mSusy2000->Get("S_dbtOffLooseAndOffLoose");
    // TH1D * h_S_controlA_mH70mSusy2000 = (TH1D*)f_mH70mSusy2000->Get("S_dbtMed2MaxAndOffIDBTCv1");
    // TH1D * h_S_controlB_mH70mSusy2000 = (TH1D*)f_mH70mSusy2000->Get("S_dbtOffIDBTCv1AndMed2Max");

    std::vector<TH1D*> indiHistoVec = {h_S_tag_mH70mSusy1600};
    std::vector<TH1D*> stackHistoVec = {h_S_tag_QCD, h_S_tag_TTJets, h_S_tag_ZJets, h_S_tag_WJets};
    std::vector<std::string> stringVec = {"HT1500-2500", "HT2500-3500", "HT3500+"};


    Plotter plot = Plotter(indiHistoVec, stackHistoVec);
    // Plotter plot = Plotter({}, stackHistoVec);
    // Plotter plot = Plotter(indiHistoVec);
    plot.AddLatex(luminosity);
    plot.AddRatioBox(0,5);
    plot.SetLogY();
    plot.SetErrors("only_stack");
    
    std::vector<std::string> legendNames = {"mH70_mSusy1600", "mH70_mSusy2000", "QCD", "TTJets", "ZJets", "WJets"};
    plot.AddLegend(legendNames, 0.68, 0.88, 0.47, 0.81, 0.03);
    // plot.SetErrors("only_stack");
    plot.SaveSpec01("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_07_20/RATIO/test3.pdf", stringVec);


    return 0;
} 