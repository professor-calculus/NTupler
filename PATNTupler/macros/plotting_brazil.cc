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
#include <TTree.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TFile.h>

//RAL PARTICLE HEADERS
#include "Plotter.hh"
#include "TimeStamp.h"


// MAKES BRAZIL PLOTS

int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // ONE: input directory (where the combined root files are)
    const std::string inputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2018_10_24/2016_and_2017_wrong2017MassSystematics/TESTING_allSys/";

    // TWO: plot output directory
    const std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_08_03/brazilplots/mSusy2800/";

    // THREE: higgs and SUSY masses (one of which should have a single entry)
    const std::vector<int> higgsMasses = {30, 35, 40, 50, 70, 90, 125};
    const std::vector<int> susyMasses = {2800};

    // FOUR: luminosity label
    const double luminosity = 77.24;

    // FIVE: plot observed line ?
    // const bool plotObserved = true;
    const bool plotObserved = false;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////   

    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/plotting_brazil.cc %s/%s__plotting_brazil.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    // find out whether mH or mSusy is varied
    int fixedMass = -1;
    std::vector<int> variedMass;
    bool varyHiggsMass;
    std::string xAxisTitle = "";
    if (higgsMasses.size() == 1 && susyMasses.size() > 1){
        fixedMass = higgsMasses[0];
        variedMass = susyMasses;
        varyHiggsMass = false;
        xAxisTitle = "M_{SUSY} (GeV)";
    }
    else if (higgsMasses.size() > 1 && susyMasses.size() == 1){
        fixedMass = susyMasses[0];
        variedMass = higgsMasses;
        varyHiggsMass = true;
        xAxisTitle = "M_{H} (GeV)";   
    }
    else {
        std::cout << "You have not provided a correct configuration of higgsMasses and susyMasses... Exiting..." << std::endl;
        return 1;
    }

    const unsigned int nEntries = variedMass.size();
    
    std::vector<double> x_vec;
    std::vector<double> y_vec;
    std::vector<double> yObs_vec;
    std::vector<double> yErrUp1Sig_vec;
    std::vector<double> yErrDown1Sig_vec;
    std::vector<double> yErrUp2Sig_vec;
    std::vector<double> yErrDown2Sig_vec;
    std::vector<double> null_vec(nEntries, 0.0);

    double maxLimitValue = -1.0;
    double minLimitValue = 9999.99;
    for (auto massPoint : variedMass){

        std::string filePath = inputDir + "/higgsCombineTest.AsymptoticLimits.mH";
        if (varyHiggsMass) filePath += std::to_string(massPoint) + ".mSusy" + std::to_string(fixedMass) + ".root";
        else filePath += std::to_string(fixedMass) + ".mSusy" + std::to_string(massPoint) + ".root";

        TFile * f = new TFile(filePath.c_str());
        TTree * T = (TTree*)f->Get("limit");
        double limitValue;
        T->SetBranchAddress("limit", &limitValue);

        x_vec.push_back(massPoint);

        T->GetEntry(2);
        y_vec.push_back(limitValue);
        double medianLimit = limitValue;

        T->GetEntry(0);
        yErrDown2Sig_vec.push_back(medianLimit - limitValue);
        if (limitValue < minLimitValue) minLimitValue = limitValue;

        T->GetEntry(1);
        yErrDown1Sig_vec.push_back(medianLimit - limitValue);

        T->GetEntry(3);
        yErrUp1Sig_vec.push_back(limitValue - medianLimit);

        T->GetEntry(4);
        yErrUp2Sig_vec.push_back(limitValue - medianLimit);
        if (limitValue > maxLimitValue) maxLimitValue = limitValue;

        T->GetEntry(5);
        yObs_vec.push_back(limitValue);
        if (plotObserved && limitValue > maxLimitValue) maxLimitValue = limitValue;
        if (plotObserved && limitValue < minLimitValue) minLimitValue = limitValue;
    }     

    TGraphAsymmErrors * g_obs = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(yObs_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]));
    g_obs->GetXaxis()->SetTitle(xAxisTitle.c_str());
    // g_obs->GetYaxis()->SetTitle("95% upper CL of r");
    g_obs->GetYaxis()->SetTitle("95% upper CL of #sigma / #sigma_{theory}");
    TGraphAsymmErrors * g_exp = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]), &(null_vec[0]));
    TGraphAsymmErrors * g_expErr1Sig = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(yErrDown1Sig_vec[0]), &(yErrUp1Sig_vec[0]));
    TGraphAsymmErrors * g_expErr2Sig = new TGraphAsymmErrors(nEntries, &(x_vec[0]), &(y_vec[0]), &(null_vec[0]), &(null_vec[0]), &(yErrDown2Sig_vec[0]), &(yErrUp2Sig_vec[0]));

    // the vector order goes: observed, expected, 1sigma, 2sigma
    Plotter brazilPlot = Plotter({g_obs, g_exp, g_expErr1Sig, g_expErr2Sig}, plotObserved);
    // brazilPlot.AddLegend(0.20, 0.45, 0.63, 0.86);
    brazilPlot.AddLegend(0.60, 0.85, 0.63, 0.86);
    brazilPlot.AddLatex(luminosity, "#it{Preliminary}");
    brazilPlot.SaveBrazil(Form("%s/linear_fixedMass%d.pdf", outputDir.c_str(), fixedMass), 0.0, 1.05 * maxLimitValue);
    brazilPlot.SetLogY();
    brazilPlot.SaveBrazil(Form("%s/log_fixedMass%d.pdf", outputDir.c_str(), fixedMass), 0.85 * minLimitValue, 1.15 * maxLimitValue);

    return 0;
}