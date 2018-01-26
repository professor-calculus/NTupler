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

// MAKES PLOTS USING HISTOGRAMS

void GetHistograms(std::map<std::string,TH1D*>&);
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info & luminosity
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_01_08/oneDimensionRepresentation/DATA/control/IDBTCv23_minus0p4/newPredictionMethod/"; // where we are going to save the output plots (should include the samples name, and any important features)
    double luminosity = 35.867; // NB this is just a label for the plot. It should match the lumi of the histograms!
    
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_plot.cc %s/%s__histos_plot.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));



    std::map<std::string, TH1D*> h_;
    GetHistograms(h_);
    // explanation of terminology
    // 1. S, U, D --> refers to mass space. pred is the prediction of S. UnD is the sum U+D.
    // 2. tag, anti, control --> refers to 2*DBT space
    // 3. sample name on the end


    // FOR A_{i} PLOTS
    // h_["S_anti_data"]->Divide(h_["UnD_anti_data"]);
    // h_["S_anti_data"]->GetYaxis()->SetTitle("A_{i}");
    // h_["S_anti_data"]->GetYaxis()->SetRangeUser(0,2.2);
    // h_["S_anti_QCD"]->Divide(h_["UnD_anti_QCD"]);
    // h_["S_anti_QCD"]->GetYaxis()->SetTitle("A_{i}");
    // h_["S_anti_QCD"]->GetYaxis()->SetRangeUser(0,2.2);
    // TH1D * h = new TH1D("h", "", 30, 0, 30);
    // for (unsigned int i = 1; i < 31; ++i){
    //     h->SetBinContent(i, QcdSidebandCorr::GetCorr(i));
    //     h->SetBinError(i, 0.10);
    //     std::cout << Form("%.10f", h_["S_anti_data"]->GetBinError(i) ) << std::endl;
    // }


    // TWO: make plot aesthetics and saving
    // std::vector<TH1D*> indiHistoVec = {h_["S_anti_QCD"], h};
    std::vector<TH1D*> indiHistoVec = {h_["S_control_data"], h_["predNew_control_data"]};
    // std::vector<TH1D*> indiHistoVec = {h_["S_tag_mH70_mSusy2000"], h_["UnD_tag_mH70_mSusy2000"]};
    // std::vector<TH1D*> stackHistoVec = {h_["UnD_control_WJets"], h_["UnD_control_ZJets"], h_["UnD_control_TTJets"], h_["UnD_control_QCD"]};

    Plotter plot = Plotter(indiHistoVec);
    // Plotter plot = Plotter({}, stackHistoVec);
    // Plotter plot = Plotter(indiHistoVec, stackHistoVec);

    // std::vector<std::string> legendNames = {"QCD MC", "calculation"};
    // std::vector<std::string> legendNames = {"S_anti", "UnD_anti"};
    std::vector<std::string> legendNames = {"control", "control prediction"};
    // std::vector<std::string> legendNames = {"mH70_mSusy1200", "mH70_mSusy1600", "mH70_mSusy2000", "WJets", "ZJets", "TTJets", "QCD"};
    // std::vector<std::string> legendNames = {"WJets", "ZJets", "TTJets", "QCD"};

    // plot.AddLegend(legendNames, 0.70, 0.88, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.18, 0.38, 0.58, 0.81, 0.028);
    // plot.AddLegend(legendNames, 0.18, 0.38, 0.18, 0.30, 0.040);
    plot.AddLegend(legendNames, 0.68, 0.88, 0.58, 0.81, 0.040);
    // plot.AddLegend(legendNames, 0.18, 0.38, 0.58, 0.81, 0.040);
    // plot.AddLegend(legendNames, 0.75, 0.88, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.67, 0.88, 0.61, 0.80, 0.04); // with ratio box
    // plot.AddLegend2Cols(3, legendNames, 0.70, 0.88, 0.64, 0.83, 0.028);
    // plot.AddLegend2Cols(4, legendNames, 0.70, 0.88, 0.64, 0.83, 0.028);
    
    // plot.AddLatex();
    // plot.AddLatex(luminosity);
    plot.AddLatex(luminosity, "#it{Data} 2016");
    // plot.AddRatioBox(0.1, 1.9, "ratio", true);
    // plot.AddRatioBox("ratio", true);
    // plot.AddRatioBox("true / pred", true);
    plot.AddRatioBox(0.1,2.2, "true / pred", true);
    // plot.AddRatioBox(0.1,2.2, "S / UnD", true);
    // plot.AddRatioBox("S / UnD", true);
    plot.SetErrors();
    // plot.SetErrors("only_stack");
    // plot.SetErrors("only_indi");

    std::vector<std::string> stringVec = {"HT1500-2500", "HT2500-3500", "HT3500+"};
    std::string plotName = "linear";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);
    
    plotName = "log";
    plot.SetLogY();
    // plot.SetYValueMin(0.15); // REMEMBER THIS PARAM! (only for log)
    plot.SetYValueMin(0.80); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(1.10); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(3.10); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(12.10); // REMEMBER THIS PARAM! (only for log)
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);



    return 0;
}





void GetHistograms(std::map<std::string,TH1D*>& h_)
{
    // histos locations
    std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2018_01_11_CMSSW_8_0_29_dbtV4/MassCutsV05/";
    
    std::string postamble = "MassCutsV05_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi36.root";
    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("data");
    histoNameVec.push_back("QCD");
    histoNameVec.push_back("TTJets");
    histoNameVec.push_back("ZJets");
    histoNameVec.push_back("WJets");
    // histoNameVec.push_back("mH30_mSusy800");
    // histoNameVec.push_back("mH50_mSusy800");
    // histoNameVec.push_back("mH70_mSusy800");
    // histoNameVec.push_back("mH90_mSusy800");
    // histoNameVec.push_back("mH30_mSusy1200");
    // histoNameVec.push_back("mH50_mSusy1200");
    // histoNameVec.push_back("mH70_mSusy1200");
    // histoNameVec.push_back("mH90_mSusy1200");
    // histoNameVec.push_back("mH30_mSusy1600");
    // histoNameVec.push_back("mH50_mSusy1600");
    // histoNameVec.push_back("mH70_mSusy1600");
    // histoNameVec.push_back("mH90_mSusy1600");
    // histoNameVec.push_back("mH30_mSusy2000");
    // histoNameVec.push_back("mH50_mSusy2000");
    // histoNameVec.push_back("mH70_mSusy2000");
    // histoNameVec.push_back("mH90_mSusy2000");

    std::string postamble_noAk4 = "MassCutsV05_ak8pt300_ht1500x2500x3500x_ak4pt-1n-1_lumi36.root";
    std::vector<std::string> histoNameVec_noAk4;
    histoNameVec_noAk4.push_back("data_NOAK4");
    histoNameVec_noAk4.push_back("QCD_NOAK4");
    histoNameVec_noAk4.push_back("TTJets_NOAK4");
    histoNameVec_noAk4.push_back("ZJets_NOAK4");
    histoNameVec_noAk4.push_back("WJets_NOAK4");
    // histoNameVec_noAk4.push_back("mH70_mSusy800_NOAK4");
    // histoNameVec_noAk4.push_back("mH70_mSusy1200_NOAK4");
    // histoNameVec_noAk4.push_back("mH70_mSusy1600_NOAK4");
    // histoNameVec_noAk4.push_back("mH70_mSusy2000_NOAK4");

    for (size_t iH = 0; iH < (histoNameVec.size() + histoNameVec_noAk4.size()); ++iH){

        std::string postambleToUse;
        std::string histoToUse;
        if (iH < histoNameVec.size()){
            postambleToUse = postamble;
            histoToUse = histoNameVec[iH];
        }
        else {
            postambleToUse = postamble_noAk4;
            histoToUse = histoNameVec_noAk4[iH - histoNameVec.size()];
        }

        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoToUse.c_str(), postambleToUse.c_str()));
        // explanation of terminology
        // 1. S, U, D --> refers to mass space. pred is the prediction of S. UnD is the sum U+D.
        // 2. tag, anti, control --> refers to 2*DBT space
        // 3. sample name on the end

        if (histoToUse != "data" && histoToUse != "data_NOAK4") h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtDiagUpLoose");
        h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtDiagUpLoose");
        h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtDiagUpLoose");

        h_[Form("S_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose");
        h_[Form("U_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose");
        h_[Form("D_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose");

        h_[Form("S_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtLooseMed2AndOffIDBTCv23");
        h_[Form("U_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtLooseMed2AndOffIDBTCv23");
        h_[Form("D_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtLooseMed2AndOffIDBTCv23");
        h_[Form("S_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("S_dbtOffIDBTCv23AndLooseMed2"));
        h_[Form("U_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("U_dbtOffIDBTCv23AndLooseMed2"));
        h_[Form("D_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("D_dbtOffIDBTCv23AndLooseMed2"));

        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add(h_[Form("D_tag_%s", histoToUse.c_str())]);

        h_[Form("UnD_anti_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_anti_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_anti_%s", histoToUse.c_str())]->Add(h_[Form("D_anti_%s", histoToUse.c_str())]);

        h_[Form("UnD_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_control_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_control_%s", histoToUse.c_str())]->Add(h_[Form("D_control_%s", histoToUse.c_str())]);
   
        // NEW METHOD OF PREDICTION
        h_[Form("predNew_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin);
            double UnDValue = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinError(iBin);
            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinError(iBin, predError);
        }

        h_[Form("predNew_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_control_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin);
            double UnDValue = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinError(iBin);
            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinError(iBin, predError);
        }

        // OLD METHOD OF PREDICTION
        h_[Form("predOld_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("predOld_tag_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("predOld_tag_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);

        h_[Form("predOld_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        h_[Form("predOld_control_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("predOld_control_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);


    } // closes loop through histoNameVec
}