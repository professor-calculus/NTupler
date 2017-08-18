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

void GetHistograms(std::map<std::string,TH1D*>&);
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info & luminosity
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2017_08_14/qcdEstimationContamination/control_UplusD/w_mH70_mSusyALL/"; // where we are going to save the output plots (should include the samples name, and any important features)
	double luminosity = 50.0; // NB this is just a label for the plot. It should match the lumi of the histograms!
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_plot.cc %s/%s__histos_plot.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));



    std::map<std::string, TH1D*> h_;
    GetHistograms(h_);
    // explanation of terms after h
    // 1. S, U, D --> refers to mass space. predV1, predV2 correspond to predictions of S. UnD is the sum U+D.
    // 2. tag, inverted, controlA, controlB --> refers to 2*DBT space
    // 3. sample name on the end



    // TWO: make plot aesthetics and saving
    std::vector<TH1D*> indiHistoVec = {h_["UnD_control_mH90_mSusy1200"], h_["UnD_control_mH90_mSusy1600"], h_["UnD_control_mH90_mSusy2000"]};
    std::vector<TH1D*> stackHistoVec = {h_["UnD_control_WJets"], h_["UnD_control_ZJets"], h_["UnD_control_TTJets"], h_["UnD_control_QCD"]};

    // Plotter plot = Plotter(indiHistoVec);
    // Plotter plot = Plotter({}, stackHistoVec);
    Plotter plot = Plotter(indiHistoVec, stackHistoVec);

    std::vector<std::string> legendNames = {"mH70_mSusy1200", "mH70_mSusy1600", "mH70_mSusy2000", "WJets", "ZJets", "TTJets", "QCD"};
    // plot.AddLegend(legendNames, 0.70, 0.88, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.67, 0.88, 0.61, 0.80, 0.04); // with ratio box
    plot.AddLegend2Cols(3, legendNames, 0.70, 0.88, 0.64, 0.83, 0.028);
    
    plot.AddLatex(luminosity);
    // plot.AddRatioBox("true / pred");
    // plot.AddRatioBox(0.1,2.3, "true / pred");
    // plot.SetErrors();
    plot.SetErrors("only_stack");
    // plot.SetErrors("only_indi");

    std::vector<std::string> stringVec = {"HT1500-2500", "HT2500-3500", "HT3500+"};
    std::string plotName = "linear";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);
    
    plotName = "log";
    plot.SetLogY();
    plot.SetYValueMin(0.15);
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);



    return 0;
}





void GetHistograms(std::map<std::string,TH1D*>& h_)
{
    // histos locations
    std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_08_11_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV04/";
    std::string postamble = "MassCutsV04_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root";
    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("QCD");
    histoNameVec.push_back("TTJets");
    histoNameVec.push_back("ZJets");
    histoNameVec.push_back("WJets");
    histoNameVec.push_back("mH30_mSusy800");
    histoNameVec.push_back("mH50_mSusy800");
    histoNameVec.push_back("mH70_mSusy800");
    histoNameVec.push_back("mH90_mSusy800");
    histoNameVec.push_back("mH30_mSusy1200");
    histoNameVec.push_back("mH50_mSusy1200");
    histoNameVec.push_back("mH70_mSusy1200");
    histoNameVec.push_back("mH90_mSusy1200");
    histoNameVec.push_back("mH30_mSusy1600");
    histoNameVec.push_back("mH50_mSusy1600");
    histoNameVec.push_back("mH70_mSusy1600");
    histoNameVec.push_back("mH90_mSusy1600");
    histoNameVec.push_back("mH30_mSusy2000");
    histoNameVec.push_back("mH50_mSusy2000");
    histoNameVec.push_back("mH70_mSusy2000");
    histoNameVec.push_back("mH90_mSusy2000");
    histoNameVec.push_back("QCDNOAK4"); // must put at the end
    histoNameVec.push_back("TTJetsNOAK4"); // must put at the end


    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        if (iH >= histoNameVec.size()-2 && histoNameVec[iH] == "QCDNOAK4") postamble = "MassCutsV04_ak8pt300_ht1500x2500x3500x_ak4pt-1n-1_lumi50.root"; // HACK FOR NO AK4 CUT

        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoNameVec[iH].c_str(), postamble.c_str()));
        // explanation of terminology
        // 1. S, U, D --> refers to mass space. predV1, predV2 correspond to predictions of S. UnD is the sum U+D. SoUnD is S over U+D.
        // 2. tag, inverted, controlA, controlB --> refers to 2*DBT space
        // 3. sample name on the end

        h_[Form("S_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("S_dbtMed2MaxAndMed2Max");
        h_[Form("U_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("U_dbtMed2MaxAndMed2Max");
        h_[Form("D_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("D_dbtMed2MaxAndMed2Max");

        h_[Form("S_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose");
        h_[Form("U_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose");
        h_[Form("D_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose");

        h_[Form("S_control_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("S_dbtMed2MaxAndOffIDBTCv1");
        h_[Form("U_control_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("U_dbtMed2MaxAndOffIDBTCv1");
        h_[Form("D_control_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("D_dbtMed2MaxAndOffIDBTCv1");
        h_[Form("S_control_%s", histoNameVec[iH].c_str())]->Add((TH1D*)f->Get("S_dbtOffIDBTCv1AndMed2Max"));
        h_[Form("U_control_%s", histoNameVec[iH].c_str())]->Add((TH1D*)f->Get("U_dbtOffIDBTCv1AndMed2Max"));
        h_[Form("D_control_%s", histoNameVec[iH].c_str())]->Add((TH1D*)f->Get("D_dbtOffIDBTCv1AndMed2Max"));

        h_[Form("UnD_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_tag_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_tag_%s", histoNameVec[iH].c_str())]);
        h_[Form("SoUnD_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("S_tag_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("SoUnD_tag_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("UnD_tag_%s", histoNameVec[iH].c_str())]);
        h_[Form("SoUnD_tag_%s", histoNameVec[iH].c_str())]->GetYaxis()->SetTitle("Ratio");

        h_[Form("UnD_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_inverted_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("UnD_inverted_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_inverted_%s", histoNameVec[iH].c_str())]);
        h_[Form("SoUnD_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("S_inverted_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("SoUnD_inverted_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("UnD_inverted_%s", histoNameVec[iH].c_str())]);
        h_[Form("SoUnD_inverted_%s", histoNameVec[iH].c_str())]->GetYaxis()->SetTitle("Ratio");

        h_[Form("UnD_control_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_control_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("UnD_control_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_control_%s", histoNameVec[iH].c_str())]);
        h_[Form("SoUnD_control_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("S_control_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("SoUnD_control_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("UnD_control_%s", histoNameVec[iH].c_str())]);
        h_[Form("SoUnD_control_%s", histoNameVec[iH].c_str())]->GetYaxis()->SetTitle("Ratio");

        h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_inverted_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_inverted_%s", histoNameVec[iH].c_str())]);

        h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_tag_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_tag_%s", histoNameVec[iH].c_str())]);    
        h_[Form("predV1_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("predV1_tag_%s", histoNameVec[iH].c_str())]->Multiply(h_[Form("S_inverted_%s", histoNameVec[iH].c_str())]);
        h_[Form("predV1_tag_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]);

        h_[Form("predV2_control_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_control_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_control_%s", histoNameVec[iH].c_str())]);    
        h_[Form("predV1_control_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->Clone();
        h_[Form("predV1_control_%s", histoNameVec[iH].c_str())]->Multiply(h_[Form("S_inverted_%s", histoNameVec[iH].c_str())]);
        h_[Form("predV1_control_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]);

        h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->SetBinContent(1, 2 * h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->GetBinContent(1));
        h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->SetBinError(1, 2 * h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->GetBinError(1));
        h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->SetBinContent(1, 2 * h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->GetBinContent(1));
        h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->SetBinError(1, 2 * h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->GetBinError(1));
        h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->SetBinContent(1, 2 * h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->GetBinContent(1));
        h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->SetBinError(1, 2 * h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->GetBinError(1));

        h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->SetBinContent(11, 2 * h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->GetBinContent(11));
        h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->SetBinError(11, 2 * h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->GetBinError(11));
        h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->SetBinContent(11, 2 * h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->GetBinContent(11));
        h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->SetBinError(11, 2 * h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->GetBinError(11));
        h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->SetBinContent(11, 2 * h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->GetBinContent(11));
        h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->SetBinError(11, 2 * h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->GetBinError(11));

        h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->SetBinContent(21, 2 * h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->GetBinContent(21));
        h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->SetBinError(21, 2 * h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->GetBinError(21));
        h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->SetBinContent(21, 2 * h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->GetBinContent(21));
        h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->SetBinError(21, 2 * h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->GetBinError(21));
        h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->SetBinContent(21, 2 * h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->GetBinContent(21));
        h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->SetBinError(21, 2 * h_[Form("predV2_control_%s", histoNameVec[iH].c_str())]->GetBinError(21));

    } // closes loop through histoNameVec
}





// void GetHistograms(std::map<std::string,TH1D*>& h_)
// {
//     // histos locations
//     std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_08_11_CMSSW_8_0_2X_dbtV4_newFatJetLabelling/MassCutsV04/";
//     std::string postamble = "MassCutsV04_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi50.root";
//     std::vector<std::string> histoNameVec;
//     histoNameVec.push_back("QCD");
//     histoNameVec.push_back("TTJets");
//     histoNameVec.push_back("ZJets");
//     histoNameVec.push_back("WJets");
//     histoNameVec.push_back("mH30_mSusy800");
//     histoNameVec.push_back("mH50_mSusy800");
//     histoNameVec.push_back("mH70_mSusy800");
//     histoNameVec.push_back("mH90_mSusy800");
//     histoNameVec.push_back("mH30_mSusy1200");
//     histoNameVec.push_back("mH50_mSusy1200");
//     histoNameVec.push_back("mH70_mSusy1200");
//     histoNameVec.push_back("mH90_mSusy1200");
//     histoNameVec.push_back("mH30_mSusy1600");
//     histoNameVec.push_back("mH50_mSusy1600");
//     histoNameVec.push_back("mH70_mSusy1600");
//     histoNameVec.push_back("mH90_mSusy1600");
//     histoNameVec.push_back("mH30_mSusy2000");
//     histoNameVec.push_back("mH50_mSusy2000");
//     histoNameVec.push_back("mH70_mSusy2000");
//     histoNameVec.push_back("mH90_mSusy2000");
//     histoNameVec.push_back("QCDNOAK4"); // must put at the end
//     histoNameVec.push_back("TTJetsNOAK4"); // must put at the end


//     for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

//         if (iH >= histoNameVec.size()-2 && histoNameVec[iH] == "QCDNOAK4") postamble = "MassCutsV04_ak8pt300_ht1500x2500x3500x_ak4pt-1n-1_lumi50.root"; // HACK FOR NO AK4 CUT

//         TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoNameVec[iH].c_str(), postamble.c_str()));
//         // explanation of terminology
//         // 1. S, U, D --> refers to mass space. predV1, predV2 correspond to predictions of S. UnD is the sum U+D
//         // 2. tag, inverted, controlA, controlB --> refers to 2*DBT space
//         // 3. sample name on the end

//         h_[Form("S_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("S_dbtMed2MaxAndMed2Max");
//         h_[Form("U_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("U_dbtMed2MaxAndMed2Max");
//         h_[Form("D_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("D_dbtMed2MaxAndMed2Max");

//         h_[Form("S_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose");
//         h_[Form("U_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose");
//         h_[Form("D_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose");

//         h_[Form("S_controlA_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("S_dbtMed2MaxAndOffIDBTCv1");
//         h_[Form("U_controlA_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("U_dbtMed2MaxAndOffIDBTCv1");
//         h_[Form("D_controlA_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("D_dbtMed2MaxAndOffIDBTCv1");

//         h_[Form("S_controlB_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("S_dbtOffIDBTCv1AndMed2Max");
//         h_[Form("U_controlB_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("U_dbtOffIDBTCv1AndMed2Max");
//         h_[Form("D_controlB_%s", histoNameVec[iH].c_str())] = (TH1D*)f->Get("D_dbtOffIDBTCv1AndMed2Max");

//         h_[Form("UnD_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_tag_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("UnD_tag_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_tag_%s", histoNameVec[iH].c_str())]);
//         h_[Form("SoUnD_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("S_tag_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("SoUnD_tag_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("UnD_tag_%s", histoNameVec[iH].c_str())]);
//         h_[Form("SoUnD_tag_%s", histoNameVec[iH].c_str())]->GetYaxis()->SetTitle("Ratio");

//         h_[Form("UnD_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_inverted_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("UnD_inverted_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_inverted_%s", histoNameVec[iH].c_str())]);
//         h_[Form("SoUnD_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("S_inverted_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("SoUnD_inverted_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("UnD_inverted_%s", histoNameVec[iH].c_str())]);
//         h_[Form("SoUnD_inverted_%s", histoNameVec[iH].c_str())]->GetYaxis()->SetTitle("Ratio");

//         h_[Form("UnD_controlA_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_controlA_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("UnD_controlA_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_controlA_%s", histoNameVec[iH].c_str())]);
//         h_[Form("SoUnD_controlA_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("S_controlA_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("SoUnD_controlA_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("UnD_controlA_%s", histoNameVec[iH].c_str())]);
//         h_[Form("SoUnD_controlA_%s", histoNameVec[iH].c_str())]->GetYaxis()->SetTitle("Ratio");

//         h_[Form("UnD_controlB_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_controlB_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("UnD_controlB_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_controlB_%s", histoNameVec[iH].c_str())]);
//         h_[Form("SoUnD_controlB_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("S_controlB_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("SoUnD_controlB_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("UnD_controlB_%s", histoNameVec[iH].c_str())]);
//         h_[Form("SoUnD_controlB_%s", histoNameVec[iH].c_str())]->GetYaxis()->SetTitle("Ratio");

//         h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_inverted_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_inverted_%s", histoNameVec[iH].c_str())]);

//         h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_tag_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_tag_%s", histoNameVec[iH].c_str())]);    
//         h_[Form("predV1_tag_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("predV1_tag_%s", histoNameVec[iH].c_str())]->Multiply(h_[Form("S_inverted_%s", histoNameVec[iH].c_str())]);
//         h_[Form("predV1_tag_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]);

//         h_[Form("predV2_controlA_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_controlA_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("predV2_controlA_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_controlA_%s", histoNameVec[iH].c_str())]);    
//         h_[Form("predV1_controlA_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("predV2_controlA_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("predV1_controlA_%s", histoNameVec[iH].c_str())]->Multiply(h_[Form("S_inverted_%s", histoNameVec[iH].c_str())]);
//         h_[Form("predV1_controlA_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]);

//         h_[Form("predV2_controlB_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("U_controlB_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("predV2_controlB_%s", histoNameVec[iH].c_str())]->Add(h_[Form("D_controlB_%s", histoNameVec[iH].c_str())]);    
//         h_[Form("predV1_controlB_%s", histoNameVec[iH].c_str())] = (TH1D*)h_[Form("predV2_controlB_%s", histoNameVec[iH].c_str())]->Clone();
//         h_[Form("predV1_controlB_%s", histoNameVec[iH].c_str())]->Multiply(h_[Form("S_inverted_%s", histoNameVec[iH].c_str())]);
//         h_[Form("predV1_controlB_%s", histoNameVec[iH].c_str())]->Divide(h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]);

//         h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->SetBinContent(1, 2 * h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->GetBinContent(1));
//         h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->SetBinError(1, 2 * h_[Form("predV2_inverted_%s", histoNameVec[iH].c_str())]->GetBinError(1));
//         h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->SetBinContent(1, 2 * h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->GetBinContent(1));
//         h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->SetBinError(1, 2 * h_[Form("predV2_tag_%s", histoNameVec[iH].c_str())]->GetBinError(1));
//         h_[Form("predV2_controlA_%s", histoNameVec[iH].c_str())]->SetBinContent(1, 2 * h_[Form("predV2_controlA_%s", histoNameVec[iH].c_str())]->GetBinContent(1));
//         h_[Form("predV2_controlA_%s", histoNameVec[iH].c_str())]->SetBinError(1, 2 * h_[Form("predV2_controlA_%s", histoNameVec[iH].c_str())]->GetBinError(1));
//         h_[Form("predV2_controlB_%s", histoNameVec[iH].c_str())]->SetBinContent(1, 2 * h_[Form("predV2_controlB_%s", histoNameVec[iH].c_str())]->GetBinContent(1));
//         h_[Form("predV2_controlB_%s", histoNameVec[iH].c_str())]->SetBinError(1, 2 * h_[Form("predV2_controlB_%s", histoNameVec[iH].c_str())]->GetBinError(1));

//     } // closes loop through histoNameVec
// }