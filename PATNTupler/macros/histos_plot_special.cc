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
#include "QcdSidebandCorr.h"

// MAKES PLOTS USING HISTOGRAMS - FOR OUTPUTS OF SPECIAL TTBAR SHAPE

void GetHistograms(std::map<std::string,TH1D*>&);
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    // ONE: save info & luminosity
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_08_03/2016_80X/testingTTJets_v2/threeBinz/dbtNom/theTest_wQcdErr15/"; // where we are going to save the output plots (should include the samples name, and any important features)
    double luminosity = 35.867; // NB this is just a label for the plot. It should match the lumi of the histograms!
    // double luminosity = 41.370; // 2017 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_plot_special.cc %s/%s__histos_plot_special.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));


    std::map<std::string, TH1D*> h_;
    GetHistograms(h_);
    // explanation of terminology
    // 1. S, U, D --> refers to mass space. UnD is the sum U+D.
    // 2. tag, anti --> refers to 2*DBT space
    // 3. sample name on the end



    // TWO: scale TTJets XS
    const double scaleTTJetsXS = 1.0;

    h_["S_anti_TTJets"]->Scale(scaleTTJetsXS);
    h_["S_tag_TTJets"]->Scale(scaleTTJetsXS);
    h_["UnD_anti_TTJets"]->Scale(scaleTTJetsXS);
    h_["UnD_tag_TTJets"]->Scale(scaleTTJetsXS);



    // QCD MC HISTO MANIPULATION
    TH1D * h_qcdF = (TH1D*)h_["S_anti_QCD"]->Clone();
    h_qcdF->Divide(h_["UnD_anti_QCD"]);
    TH1D * h_qcdEst = (TH1D*)h_["UnD_tag_QCD"]->Clone();
    h_qcdEst->Multiply(h_qcdF);
    TH1D * h_qcdCorr = (TH1D*)h_["S_tag_QCD"]->Clone();
    h_qcdCorr->Divide(h_qcdEst);

    // ~DATA HISTO MANIPULATION
    TH1D * hS = (TH1D*)h_["S_anti_data"]->Clone();
    hS->Add(h_["S_anti_TTJets"], -1);
    hS->Add(h_["S_anti_ZJets"], -1);
    hS->Add(h_["S_anti_WJets"], -1);

    TH1D * hUnD = (TH1D*)h_["UnD_anti_data"]->Clone();
    hUnD->Add(h_["UnD_anti_TTJets"], -1);
    hUnD->Add(h_["UnD_anti_ZJets"], -1);
    hUnD->Add(h_["UnD_anti_WJets"], -1);

    TH1D * hF_withMC = (TH1D*)hS->Clone();
    hF_withMC->Divide(hUnD);
    hF_withMC->GetYaxis()->SetTitle("F_{i}");

    TH1D * h_ddQCD = (TH1D*)h_["UnD_tag_data"]->Clone();
    h_ddQCD->Add(h_["UnD_tag_TTJets"], -1);
    h_ddQCD->Add(h_["UnD_tag_WJets"], -1);
    h_ddQCD->Add(h_["UnD_tag_ZJets"], -1);
    h_ddQCD->Multiply(hF_withMC);
    TH1D * h_err = new TH1D("h_err", "", 3, 0, 3);
    // 15 percent error on QCD estimation
    h_err->SetBinContent(1,1.0);
    h_err->SetBinContent(2,1.0);
    h_err->SetBinContent(3,1.0);
    h_err->SetBinError(1,0.15);
    h_err->SetBinError(2,0.15);
    h_err->SetBinError(3,0.15);
    h_ddQCD->Multiply(h_err);

    TH1D * h_ddTT = (TH1D*)h_["S_tag_data"]->Clone();
    h_ddTT->Add(h_ddQCD, -1);
    h_ddTT->Add(h_["S_tag_ZJets"], -1);
    h_ddTT->Add(h_["S_tag_WJets"], -1);



    // THREE: make plot aesthetics and saving
    // std::vector<TH1D*> indiHistoVec = {h_qcdCorr};
    std::vector<TH1D*> indiHistoVec = {h_ddTT, h_["S_tag_TTJets"]};
    // std::vector<TH1D*> indiHistoVec = {hF_withMC, h_qcdF};
    // std::vector<TH1D*> indiHistoVec = {h_["S_tag_QCD"], h_qcdEst};
    // std::vector<TH1D*> stackHistoVec = {h_["S_anti_WJets"], h_["S_anti_ZJets"], h_["S_anti_TTJets"], h_["S_anti_QCD"]};

    Plotter plot = Plotter(indiHistoVec);
    // Plotter plot = Plotter({}, stackHistoVec);
    // Plotter plot = Plotter(indiHistoVec, stackHistoVec);

    // std::vector<std::string> legendNames = {"S_tag", "S_tag prediction"};
    std::vector<std::string> legendNames = {"data driven", "MC"};
    // std::vector<std::string> legendNames = {"data", "WJets", "ZJets", "TTJets", "QCD"};

    plot.AddLegend(legendNames, 0.70, 0.88, 0.68, 0.86, 0.045);
    // plot.AddLegend(legendNames, 0.70, 0.88, 0.63, 0.86, 0.045);
    
    // plot.AddLatex(luminosity);
    plot.AddLatex(luminosity, "#it{Preliminary}");
    
    plot.AddRatioBox(0.10, 1.90, "data / MC", true);
    // plot.AddRatioBox(0.10, 1.90, "true / pred", true);
    
    plot.SetErrors();

    std::vector<std::string> stringVec = {""};
    std::string plotName = "linear";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);
    
    return 0;
}





void GetHistograms(std::map<std::string,TH1D*>& h_)
{
    // ************************
    // *** histos locations ***

    std::string ttbarDbtSysToUse = "NOSYS";
    
    std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2018_08_03/MassCutsSpecialV06/run2016_v2/";  
    // std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2018_08_03/MassCutsSpecialV06/run2017_v2/";  
    
    std::string postamble = "MassCutsSpecialV06_ak8pt300_ht1500x_ak4pt-1n-1_lumi36.root";
    // std::string postamble = "MassCutsSpecialV06_ak8pt300_ht1500x_ak4pt-1n-1_lumi41.root";

    // ************************
    // ************************

    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("TTJets");
    histoNameVec.push_back("data");
    histoNameVec.push_back("ZJets");
    histoNameVec.push_back("WJets");
    histoNameVec.push_back("QCD");

    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        std::string histoToUse = histoNameVec[iH];

        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoToUse.c_str(), postamble.c_str()));
        // explanation of terminology
        // 1. S, U, D --> refers to mass space. UnD is the sum U+D.
        // 2. tag, anti --> refers to 2*DBT space (using 'tag' for LooseMaxLooseMax)
        // 3. sample name on the end

        std::string systematicAppend = "NOSYS";
        if (histoToUse == "TTJets") systematicAppend = ttbarDbtSysToUse;

        h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get( Form("S_dbtLooseMaxAndLooseMax_%s", systematicAppend.c_str() ) );
        h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get( Form("U_dbtLooseMaxAndLooseMax_%s", systematicAppend.c_str() ) );
        h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get( Form("D_dbtLooseMaxAndLooseMax_%s", systematicAppend.c_str() ) );
        
        h_[Form("S_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("U_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("D_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose_NOSYS");

        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add(h_[Form("D_tag_%s", histoToUse.c_str())]);

        h_[Form("UnD_anti_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_anti_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_anti_%s", histoToUse.c_str())]->Add(h_[Form("D_anti_%s", histoToUse.c_str())]);

    } // closes loop through histoNameVec
}