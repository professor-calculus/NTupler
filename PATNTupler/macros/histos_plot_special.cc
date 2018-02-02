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
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_01_08/testing_TTJets_MC/1d_represent/withAK4_LooseLoose/TTJetsCompareV2/"; // where we are going to save the output plots (should include the samples name, and any important features)
    double luminosity = 35.867; // NB this is just a label for the plot. It should match the lumi of the histograms!
    
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_plot.cc %s/%s__histos_plot.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));


    std::map<std::string, TH1D*> h_;
    GetHistograms(h_);
    // explanation of terminology
    // 1. S, U, D --> refers to mass space. UnD is the sum U+D.
    // 2. tag, anti --> refers to 2*DBT space
    // 3. sample name on the end


    // HISTO MANIPULATION
    TH1D * hS = (TH1D*)h_["S_anti_data"]->Clone();
    hS->Add(h_["S_anti_TTJets"], -1);
    hS->Add(h_["S_anti_ZJets"], -1);
    hS->Add(h_["S_anti_WJets"], -1);

    TH1D * hUnD = (TH1D*)h_["UnD_anti_data"]->Clone();
    hUnD->Add(h_["UnD_anti_TTJets"], -1);
    hUnD->Add(h_["UnD_anti_ZJets"], -1);
    hUnD->Add(h_["UnD_anti_WJets"], -1);

    TH1D * hA_withMC = (TH1D*)hS->Clone();
    hA_withMC->Divide(hUnD);
    hA_withMC->GetYaxis()->SetTitle("A_{i}");

    // TH1D * hA_noMC = (TH1D*)h_["S_anti_data"]->Clone();
    // hA_noMC->Divide(h_["UnD_anti_data"]);
    // hA_noMC->GetYaxis()->SetTitle("A_{i}");

    TH1D * h_QCD = (TH1D*)h_["UnD_tag_data"]->Clone();
    h_QCD->Add(h_["UnD_tag_TTJets"], -1);
    h_QCD->Add(h_["UnD_tag_WJets"], -1);
    h_QCD->Add(h_["UnD_tag_ZJets"], -1);
    h_QCD->Multiply(hA_withMC);

    TH1D * h_TTDD = (TH1D*)h_["S_tag_data"]->Clone();
    h_TTDD->Add(h_QCD, -1);
    h_TTDD->Add(h_["S_tag_ZJets"], -1);
    h_TTDD->Add(h_["S_tag_WJets"], -1);


    // TWO: make plot aesthetics and saving
    std::vector<TH1D*> indiHistoVec = {h_TTDD, h_["S_tag_TTJets"]};

    Plotter plot = Plotter(indiHistoVec);

    std::vector<std::string> legendNames = {"data driven", "MC"};

    plot.AddLegend(legendNames, 0.68, 0.88, 0.70, 0.86, 0.045);
    
    // plot.AddLatex(luminosity);
    plot.AddLatex(luminosity, "#it{Data} 2016");
    
    plot.AddRatioBox(0.40, 1.60, "data / MC", true);
    
    plot.SetErrors();

    std::vector<std::string> stringVec = {""};
    std::string plotName = "linear";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec);
   
    
    return 0;
}





void GetHistograms(std::map<std::string,TH1D*>& h_)
{
    // histos locations
    std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2018_01_11_CMSSW_8_0_29_dbtV4/MassCutsSpecialV06/withAK4_LooseLoose/";  
    std::string postamble = "MassCutsSpecialV06_ak8pt300_ht1500x_ak4pt-1n-1_lumi36.root";

    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("data");
    histoNameVec.push_back("TTJets");
    histoNameVec.push_back("ZJets");
    histoNameVec.push_back("WJets");

    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        std::string histoToUse = histoNameVec[iH];

        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoToUse.c_str(), postamble.c_str()));
        // explanation of terminology
        // 1. S, U, D --> refers to mass space. UnD is the sum U+D.
        // 2. tag, anti --> refers to 2*DBT space (using 'tag' for LooseMaxLooseMax)
        // 3. sample name on the end
        h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtLooseMaxAndLooseMax");
        h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtLooseMaxAndLooseMax");
        h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtLooseMaxAndLooseMax");

        h_[Form("S_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose");
        h_[Form("U_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose");
        h_[Form("D_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose");

        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add(h_[Form("D_tag_%s", histoToUse.c_str())]);

        h_[Form("UnD_anti_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_anti_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_anti_%s", histoToUse.c_str())]->Add(h_[Form("D_anti_%s", histoToUse.c_str())]);

    } // closes loop through histoNameVec
}