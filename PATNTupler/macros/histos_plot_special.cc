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
    std::string outputDir = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/plots_2019_05_20/TTCR/2016_80X/threeBinz/Sig_Bkg_Compare_MGLO_HTStitched600_Tag_UnD/"; // where we are going to save the output plots (should include the samples name, and any important features)
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
    //hS->Add(h_["S_anti_P5_1800sq_R0p384"], -1);

    TH1D * hUnD = (TH1D*)h_["UnD_anti_data"]->Clone();
    hUnD->Add(h_["UnD_anti_TTJets"], -1);
    hUnD->Add(h_["UnD_anti_ZJets"], -1);
    hUnD->Add(h_["UnD_anti_WJets"], -1);
    //hUnD->Add(h_["UnD_anti_P5_1800sq_R0p384"], -1);

    TH1D * hF_withMC = (TH1D*)hS->Clone();
    hF_withMC->Divide(hUnD);
    hF_withMC->GetYaxis()->SetTitle("F_{i}");

    TH1D * h_ddQCD = (TH1D*)h_["UnD_tag_data"]->Clone();
    h_ddQCD->Add(h_["UnD_tag_TTJets"], -1);
    h_ddQCD->Add(h_["UnD_tag_WJets"], -1);
    h_ddQCD->Add(h_["UnD_tag_ZJets"], -1);
    //h_ddQCD->Add(h_["UnD_tag_P5_1800sq_R0p384"], -1);
    h_ddQCD->Multiply(hF_withMC);
    TH1D * h_err = new TH1D("h_err", "", 3, 0, 3);
    // 15-30 percent error on QCD estimation
    h_err->SetBinContent(1,1.0);
    h_err->SetBinContent(2,1.0);
    h_err->SetBinContent(3,1.0);
/*
    h_err->SetBinContent(4,1.0);
    h_err->SetBinContent(5,1.0);
    h_err->SetBinContent(6,1.0);
    h_err->SetBinContent(7,1.0);
    h_err->SetBinContent(8,1.0);
    h_err->SetBinContent(9,1.0);
    h_err->SetBinContent(10,1.0);
    h_err->SetBinContent(11,1.0);
    h_err->SetBinContent(12,1.0);
    h_err->SetBinContent(13,1.0);
    h_err->SetBinContent(14,1.0);
    h_err->SetBinContent(15,1.0);
    h_err->SetBinContent(16,1.0);
    h_err->SetBinContent(17,1.0);
    h_err->SetBinContent(18,1.0);
*/

    h_err->SetBinError(1,0.15);
    h_err->SetBinError(2,0.15);
    h_err->SetBinError(3,0.15);
/*
    h_err->SetBinError(4,0.15);
    h_err->SetBinError(5,0.15);
    h_err->SetBinError(6,0.15);
    h_err->SetBinError(7,0.15);
    h_err->SetBinError(8,0.15);
    h_err->SetBinError(9,0.15);
    h_err->SetBinError(10,0.15);
    h_err->SetBinError(11,0.15);
    h_err->SetBinError(12,0.15);
    h_err->SetBinError(13,0.15);
    h_err->SetBinError(14,0.15);
    h_err->SetBinError(15,0.15);
    h_err->SetBinError(16,0.15);
    h_err->SetBinError(17,0.15);
    h_err->SetBinError(18,0.15);
*/
    h_ddQCD->Multiply(h_err);

    TH1D * h_ddTT = (TH1D*)h_["S_tag_data"]->Clone();
    h_ddTT->Add(h_ddQCD, -1);
    h_ddTT->Add(h_["S_tag_ZJets"], -1);
    h_ddTT->Add(h_["S_tag_WJets"], -1);


    // Alternative tt prediction method
    TH1D * h_altTT = (TH1D*)h_["UnD_tag_data"]->Clone();
    h_altTT->Add(h_["UnD_tag_QCD"], -1);
    h_altTT->Add(h_["UnD_tag_ZJets"], -1);
    h_altTT->Add(h_["UnD_tag_WJets"], -1);
    h_altTT->Divide(h_["UnD_tag_TTJets"]);
    h_altTT->Multiply(h_["S_tag_TTJets"]);



    // THREE: make plot aesthetics and saving
    // std::vector<TH1D*> indiHistoVec = {h_qcdCorr};
    //std::vector<TH1D*> indiHistoVec = {h_ddTT, h_["S_tag_TTJets"]};
    //std::vector<TH1D*> indiHistoVec = {h_ddTT, h_["S_tag_TTJets_Powheg"]};
    //std::vector<TH1D*> indiHistoVec = {h_ddTT, h_["S_tag_TTJets_Powheg_NoReWeighting"]};
    //std::vector<TH1D*> indiHistoVec = {h_ddTT, h_["S_tag_TTJets_MG_HTStitched600"]};
    //std::vector<TH1D*> indiHistoVec = {h_ddTT, h_["S_tag_TTJets_MG_HTStitched600_NoReWeighting"]};
    //std::vector<TH1D*> indiHistoVec = {h_["S_tag_TTJets_MG_HTStitched600"], h_["S_tag_TTJets_MG_HTStitched600_NoReWeighting"]};
    //std::vector<TH1D*> indiHistoVec = {h_["S_tag_TTJets_MG_HTStitched600"], h_["S_tag_TTJets_Powheg"]};
    //std::vector<TH1D*> indiHistoVec = {h_ddTT, h_altTT, h_["S_tag_TTJets"]};
    std::vector<TH1D*> indiHistoVec = {h_["S_tag_P5_1800sq_R0p99"], h_["S_tag_P5_1800sq_R0p384"]};
    //std::vector<TH1D*> indiHistoVec = {h_["S_anti_P5_1800sq_R0p99"], h_["S_anti_P5_1800sq_R0p384"]};
    //std::vector<TH1D*> indiHistoVec = {h_["UnD_tag_P5_1800sq_R0p99"], h_["UnD_tag_P5_1800sq_R0p384"]};
    //std::vector<TH1D*> indiHistoVec = {h_["UnD_anti_P5_1800sq_R0p99"], h_["UnD_anti_P5_1800sq_R0p384"]};
    //std::vector<TH1D*> indiHistoVec = {h_["S_tag_P5_1800sq_R0p99"], h_["S_tag_P5_1800sq_R0p384"], h_["S_tag_P7_1900sq_R0p99"], h_["S_tag_P7_1900sq_R0p384"]}; 
    //std::vector<TH1D*> indiHistoVec = {h_["S_anti_P5_1800sq_R0p99"], h_["S_anti_P5_1800sq_R0p384"], h_["S_anti_P7_1900sq_R0p99"], h_["S_anti_P7_1900sq_R0p384"]};
    //std::vector<TH1D*> indiHistoVec = {h_["UnD_tag_P5_1800sq_R0p99"], h_["UnD_tag_P5_1800sq_R0p384"], h_["UnD_tag_P7_1900sq_R0p99"], h_["UnD_tag_P7_1900sq_R0p384"]};
    //std::vector<TH1D*> indiHistoVec = {h_["UnD_anti_P5_1800sq_R0p99"], h_["UnD_anti_P5_1800sq_R0p384"], h_["UnD_anti_P7_1900sq_R0p99"], h_["UnD_anti_P7_1900sq_R0p384"]};
    //std::vector<TH1D*> indiHistoVec = {hF_withMC, h_qcdF};
    //std::vector<TH1D*> indiHistoVec = {h_["S_tag_QCD"], h_qcdEst};
    //std::vector<TH1D*> indiHistoVec = {h_["S_anti_QCD"], hS};
    //std::vector<TH1D*> indiHistoVec = {h_["S_anti_WJets"], h_["S_anti_ZJets"], h_["S_anti_TTJets"], h_["S_anti_QCD"]};
    //std::vector<TH1D*> indiHistoVec = {h_["UnD_tag_WJets"], h_["UnD_tag_ZJets"], h_["UnD_tag_TTJets"], h_["UnD_tag_QCD"]};
    //std::vector<TH1D*> indiHistoVec = {h_["UnD_anti_WJets"], h_["UnD_anti_ZJets"], h_["UnD_anti_TTJets"], h_["UnD_anti_QCD"]};
    //std::vector<TH1D*> indiHistoVec = {h_["S_tag_WJets"], h_["S_tag_ZJets"], h_["S_tag_TTJets"], h_["S_tag_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h_["S_anti_WJets"], h_["S_anti_ZJets"], h_["S_anti_TTJets"], h_["S_anti_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h_["UnD_tag_WJets"], h_["UnD_tag_ZJets"], h_["UnD_tag_TTJets_Powheg_NoReWeighting"], h_["UnD_tag_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h_["UnD_anti_WJets"], h_["UnD_anti_ZJets"], h_["UnD_anti_TTJets_Powheg"], h_["UnD_anti_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h_["UnD_tag_WJets"], h_["UnD_tag_ZJets"], h_["UnD_tag_TTJets_MG_HTStitched600"], h_["UnD_tag_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h_["UnD_anti_WJets"], h_["UnD_anti_ZJets"], h_["UnD_anti_TTJets_MG_HTStitched600_NoReWeighting"], h_["UnD_anti_QCD"]};
    std::vector<TH1D*> stackHistoVec = {h_["S_tag_WJets"], h_["S_tag_ZJets"], h_["S_tag_TTJets_MG_HTStitched600"], h_["S_tag_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h_["S_anti_WJets"], h_["S_anti_ZJets"], h_["S_anti_TTJets_MG_HTStitched600_NoReWeighting"], h_["S_anti_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h_["S_tag_WJets"], h_["S_tag_ZJets"], h_["S_tag_TTJets_Powheg_NoReWeighting"], h_["S_tag_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h_["S_anti_WJets"], h_["S_anti_ZJets"], h_["S_anti_TTJets_Powheg"], h_["S_anti_QCD"]};

    //Plotter plot = Plotter(indiHistoVec);
    //Plotter plot = Plotter({}, stackHistoVec);
    Plotter plot = Plotter(indiHistoVec, stackHistoVec);

    //std::vector<std::string> legendNames = {"MadGraph LO", "Powheg NLO"};
    //std::vector<std::string> legendNames = {"With Top Quark pT Reweighting", "Without Reweighting"};
    //std::vector<std::string> legendNames = {"S_tag", "S_tag prediction"};
    //std::vector<std::string> legendNames = {"S_anti", "S_anti prediction"};
    //std::vector<std::string> legendNames = {"data driven", "MC"};
    //std::vector<std::string> legendNames = {"data driven", "alternative DD", "MC"};
    //std::vector<std::string> legendNames = {"data", "WJets", "ZJets", "TTJets", "QCD"};
    //std::vector<std::string> legendNames = {"WJets", "ZJets", "TTJets", "QCD"};
    //std::vector<std::string> legendNames = {"P5_1800sq_R0p99", "P5_1800sq_R0p384", "P7_1900sq_R0p99", "P7_1900sq_R0p384", "WJets", "ZJets", "TTJets", "QCD"};
    std::vector<std::string> legendNames = {"P5_1800sq_R0p99", "P5_1800sq_R0p384", "WJets", "ZJets", "TTJets", "QCD"};

    plot.AddLegend(legendNames, 0.70, 0.88, 0.68, 0.86, 0.03);
    // plot.AddLegend(legendNames, 0.70, 0.88, 0.63, 0.86, 0.045);
    
    // plot.AddLatex(luminosity);
    plot.AddLatex(luminosity, "#it{Work In Progress}");

    //plot.AddRatioBox(0.10, 1.90, "ratio", true);
    //plot.AddRatioBox(0.10, 1.90, "with / without", true);
    //plot.AddRatioBox(0.10, 1.90, "data / MC", true);
    //plot.AddRatioBox(0.10, 1.90, "true / pred", true);
    //plot.AddRatioBox(0.10, 1.90, "MC / Pred", true);

    plot.SetErrors();

    std::vector<std::string> stringVec = {""};
    std::string plotName = "linear";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);

    std::string plotName2 = "logY";
    plot.SetLogY();
    // plot.SetYValueMin(0.15); // REMEMBER THIS PARAM! (only for log)
    plot.SetYValueMin(0.50); // REMEMBER THIS PARAM! (only for log)
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName2.c_str()), stringVec);
    return 0;
}





void GetHistograms(std::map<std::string,TH1D*>& h_)
{
    // ************************
    // *** histos locations ***
    
    std::string preamble = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/histos_MHT_2019_01_08/MassCutsV06ThreeBins_FINAL/run2016/";  
    // std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2018_08_03/MassCutsSpecialV06/run2017_v2/";  
    
    std::string postamble = "MassCutsSpecialV06_ak8pt300_ht1500x_ak4pt-1n-1_lumi36.root";
    // std::string postamble = "MassCutsSpecialV06_ak8pt300_ht1500x_ak4pt-1n-1_lumi41.root";
    //std::string postamble = "MassCutsSpecialV06_ak8pt300_ht1500x2500x3500x_ak4pt-1n-1_lumi36.root";

    // ************************
    // ************************

    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("TTJets_Powheg_NoReWeighting");
    histoNameVec.push_back("TTJets_Powheg");
    histoNameVec.push_back("TTJets_MG_HTStitched600");
    histoNameVec.push_back("TTJets_MG_HTStitched600_NoReWeighting");
    histoNameVec.push_back("TTJets");
    histoNameVec.push_back("data");
    histoNameVec.push_back("ZJets");
    histoNameVec.push_back("WJets");
    histoNameVec.push_back("QCD");
    histoNameVec.push_back("P5_1800sq_R0p99");
    histoNameVec.push_back("P5_1800sq_R0p384");
    histoNameVec.push_back("P7_1900sq_R0p99");
    histoNameVec.push_back("P7_1900sq_R0p384");

    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        std::string histoToUse = histoNameVec[iH];
	//std::cout << histoToUse << std::endl;

        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoToUse.c_str(), postamble.c_str()));
        // explanation of terminology
        // 1. S, U, D --> refers to mass space. UnD is the sum U+D.
        // 2. tag, anti --> refers to 2*DBT space (using 'tag' for LooseMaxLooseMax)
        // 3. sample name on the end

        h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtLooseMaxAndLooseMax_NOSYS");
        h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtLooseMaxAndLooseMax_NOSYS");
        h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtLooseMaxAndLooseMax_NOSYS");

	// Inverted DBT Score Triangle
        //h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtDiagDownLoose_NOSYS");
        //h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtDiagDownLoose_NOSYS");
        //h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtDiagDownLoose_NOSYS");

        h_[Form("S_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("U_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("D_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose_NOSYS");

        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("D_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add(h_[Form("U_tag_%s", histoToUse.c_str())]);

        h_[Form("UnD_anti_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_anti_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_anti_%s", histoToUse.c_str())]->Add(h_[Form("D_anti_%s", histoToUse.c_str())]);

        if (histoToUse == "TTJets" || histoToUse == "TTJets_Powheg" || histoToUse == "TTJets_Powheg_NoReWeighting" || histoToUse == "TTJets_MG_HTStitched600" || histoToUse == "TTJets_MG_HTStitched600_NoReWeighting"){


            h_["tt_S_up"] = (TH1D*)f->Get("S_dbtLooseMaxAndLooseMax_dbtTagUp");

            h_["tt_S_down"] = (TH1D*)f->Get("S_dbtLooseMaxAndLooseMax_dbtTagDown");

            h_["tt_UnD_up"] = (TH1D*)f->Get("U_dbtLooseMaxAndLooseMax_dbtTagUp");
            h_["tt_UnD_up"]->Add((TH1D*)f->Get("D_dbtLooseMaxAndLooseMax_dbtTagUp"));

            h_["tt_UnD_down"] = (TH1D*)f->Get("U_dbtLooseMaxAndLooseMax_dbtTagDown");
            h_["tt_UnD_down"]->Add((TH1D*)f->Get("D_dbtLooseMaxAndLooseMax_dbtTagDown"));

/*
            // Inverted DBT Score Triangle
            h_["tt_S_up"] = (TH1D*)f->Get("S_dbtDiagDownLoose_dbtTagUp");

            h_["tt_S_down"] = (TH1D*)f->Get("S_dbtDiagDownLoose_dbtTagDown");

            h_["tt_UnD_up"] = (TH1D*)f->Get("U_dbtDiagDownLoose_dbtTagUp");
            h_["tt_UnD_up"]->Add((TH1D*)f->Get("D_dbtDiagDownLoose_dbtTagUp"));

            h_["tt_UnD_down"] = (TH1D*)f->Get("U_dbtDiagDownLoose_dbtTagDown");
            h_["tt_UnD_down"]->Add((TH1D*)f->Get("D_dbtDiagDownLoose_dbtTagDown"));
*/
            TH1D * h_errS = new TH1D("h_errS", "", 3, 0, 3);
            h_errS->SetBinContent(1,1.0);
            h_errS->SetBinContent(2,1.0);
            h_errS->SetBinContent(3,1.0);
/*
            h_errS->SetBinContent(4,1.0);
            h_errS->SetBinContent(5,1.0);
            h_errS->SetBinContent(6,1.0);
            h_errS->SetBinContent(7,1.0);
            h_errS->SetBinContent(8,1.0);
            h_errS->SetBinContent(9,1.0);  
            h_errS->SetBinContent(10,1.0); 
            h_errS->SetBinContent(11,1.0); 
            h_errS->SetBinContent(12,1.0); 
            h_errS->SetBinContent(13,1.0);
            h_errS->SetBinContent(14,1.0);
            h_errS->SetBinContent(15,1.0);  
            h_errS->SetBinContent(16,1.0); 
            h_errS->SetBinContent(17,1.0); 
            h_errS->SetBinContent(18,1.0); 
*/
            TH1D * h_errUnD = new TH1D("h_errUnD", "", 3, 0, 3);
            h_errUnD->SetBinContent(1,1.0);
            h_errUnD->SetBinContent(2,1.0);
            h_errUnD->SetBinContent(3,1.0);
/*
            h_errUnD->SetBinContent(4,1.0);
            h_errUnD->SetBinContent(5,1.0);
            h_errUnD->SetBinContent(6,1.0);
            h_errUnD->SetBinContent(7,1.0);
            h_errUnD->SetBinContent(8,1.0);
            h_errUnD->SetBinContent(9,1.0);
            h_errUnD->SetBinContent(10,1.0); 
            h_errUnD->SetBinContent(11,1.0); 
            h_errUnD->SetBinContent(12,1.0);
            h_errUnD->SetBinContent(13,1.0);
            h_errUnD->SetBinContent(14,1.0);
            h_errUnD->SetBinContent(15,1.0);
            h_errUnD->SetBinContent(16,1.0); 
            h_errUnD->SetBinContent(17,1.0); 
            h_errUnD->SetBinContent(18,1.0);
*/

            // LOOP THRU BIN ENTRIES
            for (int i = 1; i < 4; ++i){

                double tt_S_dbtErr = 0.5 * (h_["tt_S_up"]->GetBinContent(i) - h_["tt_S_down"]->GetBinContent(i)) / h_[Form("S_tag_%s", histoToUse.c_str())]->GetBinContent(i);
                h_errS->SetBinError(i, tt_S_dbtErr);
                double tt_UnD_dbtErr = 0.5 * (h_["tt_UnD_up"]->GetBinContent(i) - h_["tt_UnD_down"]->GetBinContent(i)) / h_[Form("UnD_tag_%s", histoToUse.c_str())]->GetBinContent(i);
                h_errUnD->SetBinError(i, tt_UnD_dbtErr);               
            }

	    h_[Form("S_tag_%s", histoToUse.c_str())]->Multiply(h_errS);
	    h_[Form("UnD_tag_%s", histoToUse.c_str())]->Multiply(h_errUnD);


        } // closes 'if' histosToUse == TTJets of whatever type

    } // closes loop through histoNameVec
}
