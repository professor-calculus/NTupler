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
#include "PlottingTools.h"



struct dataInput{

    std::string rootFileName;
    double crossSection; // in pb 
    // lheHtCut is a 3 element vector
    // 1st element: 1 if want to have an lhe ht cut, 0 if you don't want to have the ht cut
    // 2nd element: lhe ht >= this element
    // 3rd element: lhe ht < this element
    std::vector<int> lheHtCut;
};

class Histograms{
public:
    // constructor
    Histograms(double, std::string, std::vector<struct dataInput>, int, std::vector<std::vector<std::string>>, std::vector<int>, std::vector<std::vector<int>>, std::vector<std::vector<int>>);
    
    std::map<std::string, TH1F*> h_;
    std::string legend;
    int colour;

private:
    void CreateHistograms(std::vector<std::vector<std::string>>, std::vector<int>, std::vector<std::vector<int>>, std::vector<std::vector<int>>);
    void FillHistograms(std::vector<std::vector<std::string>>, std::vector<int>, std::vector<std::vector<int>>, std::vector<std::vector<int>>);
    std::vector<struct dataInput> dataInputVec;
    double luminosity;
    double massMin;
    double massMax;
    unsigned int numberOfBins;
};

double dbtNameToFloat(std::string);



int main(){

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //// U S E R * I N P U T S ///////////////////////////////////////////////////////////////////////////


    // ONE: saving info and lumi
    std::string baseDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_05_18/"; // for the input ROOT files
    std::string outputDirectory = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/output_InvestigateEventSelectionCutsV4/mH70/"; // where we are going to save the plots
    double integratedLuminosity = 50.0; // the integrated luminosity, in fb^-1 that we scale our plots to


    // TWO: cut parameter options
    // std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Tight"}, {"Tight","Med"}, {"Med","Med"} }; // "Loose", "Med", or "Tight" (2 elements in sub-vector, 1st for leading)
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Med"} };// "Loose", "Med", or "Tight" (2 elements in sub-vector, 1st for leading)
    // std::vector<int> cut3_ak8Pt = {200, 250, 300, 350, 400};
    std::vector<int> cut3_ak8Pt = {300};
    std::vector<std::vector<int>>cut4_ht = { {1500,2500}, {2500,3500}, {3500,99000} }; // these are HT bins, not just cuts
    // std::vector<std::vector<int>> cut5_ak4Pt = { {100,100}, {200,200}, {300,300}, {400,400}, {500,500} }; // (2 elements in sub-vector, 1st for leading)
    std::vector<std::vector<int>> cut5_ak4Pt = { {250,250} }; // (2 elements in sub-vector, 1st for leading)


    // THREE: set the dataInput's to be used (this is a separate object in case you have multiple inputs to a single histogram with different cross sections)
    // SIGNAL
    // ----------------------------------
    struct dataInput signal_mH30_mSusy800;
    signal_mH30_mSusy800.rootFileName = baseDir + "mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH30_mSusy800.crossSection = 6.466 * 0.85 * 0.85; // in pb
    signal_mH30_mSusy800.lheHtCut = {0};

    struct dataInput signal_mH50_mSusy800;
    signal_mH50_mSusy800.rootFileName = baseDir + "mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH50_mSusy800.crossSection = 6.466 * 0.85 * 0.85; // in pb
    signal_mH50_mSusy800.lheHtCut = {0};

    struct dataInput signal_mH70_mSusy800;
    signal_mH70_mSusy800.rootFileName = baseDir + "mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH70_mSusy800.crossSection = 6.466 * 0.85 * 0.85; // in pb
    signal_mH70_mSusy800.lheHtCut = {0};

    struct dataInput signal_mH90_mSusy800;
    signal_mH90_mSusy800.rootFileName = baseDir + "mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH90_mSusy800.crossSection = 6.466 * 0.85 * 0.85; // in pb
    signal_mH90_mSusy800.lheHtCut = {0};
    // ----------------------------------
    struct dataInput signal_mH30_mSusy1200;
    signal_mH30_mSusy1200.rootFileName = baseDir + "mH30p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH30_mSusy1200.crossSection = 0.494 * 0.85 * 0.85; // in pb
    signal_mH30_mSusy1200.lheHtCut = {0};

    struct dataInput signal_mH50_mSusy1200;
    signal_mH50_mSusy1200.rootFileName = baseDir + "mH50p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH50_mSusy1200.crossSection = 0.494 * 0.85 * 0.85; // in pb
    signal_mH50_mSusy1200.lheHtCut = {0};

    struct dataInput signal_mH70_mSusy1200;
    signal_mH70_mSusy1200.rootFileName = baseDir + "mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH70_mSusy1200.crossSection = 0.494 * 0.85 * 0.85; // in pb
    signal_mH70_mSusy1200.lheHtCut = {0};

    struct dataInput signal_mH90_mSusy1200;
    signal_mH90_mSusy1200.rootFileName = baseDir + "mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH90_mSusy1200.crossSection = 0.494 * 0.85 * 0.85; // in pb
    signal_mH90_mSusy1200.lheHtCut = {0};
    // ----------------------------------
    struct dataInput signal_mH30_mSusy1600;
    signal_mH30_mSusy1600.rootFileName = baseDir + "mH30p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH30_mSusy1600.crossSection = 0.060 * 0.85 * 0.85; // in pb
    signal_mH30_mSusy1600.lheHtCut = {0};

    struct dataInput signal_mH50_mSusy1600;
    signal_mH50_mSusy1600.rootFileName = baseDir + "mH50p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH50_mSusy1600.crossSection = 0.060 * 0.85 * 0.85; // in pb
    signal_mH50_mSusy1600.lheHtCut = {0};

    struct dataInput signal_mH70_mSusy1600;
    signal_mH70_mSusy1600.rootFileName = baseDir + "mH70p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH70_mSusy1600.crossSection = 0.060 * 0.85 * 0.85; // in pb
    signal_mH70_mSusy1600.lheHtCut = {0};

    struct dataInput signal_mH90_mSusy1600;
    signal_mH90_mSusy1600.rootFileName = baseDir + "mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH90_mSusy1600.crossSection = 0.060 * 0.85 * 0.85; // in pb
    signal_mH90_mSusy1600.lheHtCut = {0};
    // ----------------------------------
    struct dataInput signal_mH30_mSusy2000;
    signal_mH30_mSusy2000.rootFileName = baseDir + "mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH30_mSusy2000.crossSection = 0.009 * 0.85 * 0.85; // in pb
    signal_mH30_mSusy2000.lheHtCut = {0};

    struct dataInput signal_mH50_mSusy2000;
    signal_mH50_mSusy2000.rootFileName = baseDir + "mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH50_mSusy2000.crossSection = 0.009 * 0.85 * 0.85; // in pb
    signal_mH50_mSusy2000.lheHtCut = {0};

    struct dataInput signal_mH70_mSusy2000;
    signal_mH70_mSusy2000.rootFileName = baseDir + "mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH70_mSusy2000.crossSection = 0.009 * 0.85 * 0.85; // in pb
    signal_mH70_mSusy2000.lheHtCut = {0};

    struct dataInput signal_mH90_mSusy2000;
    signal_mH90_mSusy2000.rootFileName = baseDir + "mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH90_mSusy2000.crossSection = 0.009 * 0.85 * 0.85; // in pb
    signal_mH90_mSusy2000.lheHtCut = {0};
    // ----------------------------------
    // BACKGROUND
    struct dataInput TTJets_inclusive;
    TTJets_inclusive.rootFileName = baseDir + "TTJets_inclusiveHt/flatTree.root";
    TTJets_inclusive.crossSection = 831.76; // in pb
    TTJets_inclusive.lheHtCut = {1, 0, 700};    

    struct dataInput TTJets_ht600to800;
    TTJets_ht600to800.rootFileName = baseDir + "TTJets_HT600to800/flatTree.root";
    TTJets_ht600to800.crossSection = 2.6665344485; // in pb
    TTJets_ht600to800.lheHtCut = {1, 700, 99000};

    struct dataInput TTJets_ht800to1200;
    TTJets_ht800to1200.rootFileName = baseDir + "TTJets_HT800to1200/flatTree.root";
    TTJets_ht800to1200.crossSection = 1.0980821984; // in pb
    TTJets_ht800to1200.lheHtCut = {1, 700, 99000};

    struct dataInput TTJets_ht1200to2500;
    TTJets_ht1200to2500.rootFileName = baseDir + "TTJets_HT1200to2500/flatTree.root";
    TTJets_ht1200to2500.crossSection = 0.1987479092; // in pb
    TTJets_ht1200to2500.lheHtCut = {1, 700, 99000};

    struct dataInput TTJets_ht2500toInf;
    TTJets_ht2500toInf.rootFileName = baseDir + "TTJets_HT2500toInf/flatTree.root";
    TTJets_ht2500toInf.crossSection = 0.002368412585; // in pb
    TTJets_ht2500toInf.lheHtCut = {1, 700, 99000};

    struct dataInput ZJetsToQQ_ht600toInf;
    ZJetsToQQ_ht600toInf.rootFileName = baseDir + "ZJetsToQQ_HT600toInf/flatTree.root";
    ZJetsToQQ_ht600toInf.crossSection = 5.67; // in pb 
    ZJetsToQQ_ht600toInf.lheHtCut = {0};

    struct dataInput WJetsToQQ_ht600toInf;
    WJetsToQQ_ht600toInf.rootFileName = baseDir + "WJetsToQQ_HT600toInf/flatTree.root";
    WJetsToQQ_ht600toInf.crossSection = 95.14; // in pb 
    WJetsToQQ_ht600toInf.lheHtCut = {0};

    struct dataInput ZZTo4Q;
    ZZTo4Q.rootFileName = baseDir + "ZZTo4Q/flatTree.root";
    ZZTo4Q.crossSection = 6.842; // in pb 
    ZZTo4Q.lheHtCut = {0};

    struct dataInput ZZTo2L2Q;
    ZZTo2L2Q.rootFileName = baseDir + "ZZTo2L2Q/flatTree.root";
    ZZTo2L2Q.crossSection = 3.380; // in pb 
    ZZTo2L2Q.lheHtCut = {0};

    struct dataInput ZZTo2Q2Nu;
    ZZTo2Q2Nu.rootFileName = baseDir + "ZZTo2Q2Nu/flatTree.root";
    ZZTo2Q2Nu.crossSection = 4.072; // in pb 
    ZZTo2Q2Nu.lheHtCut = {0};

    struct dataInput WWTo4Q;
    WWTo4Q.rootFileName = baseDir + "WWTo4Q/flatTree.root";
    WWTo4Q.crossSection = 51.723; // in pb 
    WWTo4Q.lheHtCut = {0};

    struct dataInput WWTo1L1Nu2Q;
    WWTo1L1Nu2Q.rootFileName = baseDir + "WWTo1L1Nu2Q/flatTree.root";
    WWTo1L1Nu2Q.crossSection = 49.997; // in pb 
    WWTo1L1Nu2Q.lheHtCut = {0};


    // FOUR: group the input datasets and create Histograms
    // ---------------------------------- 
    // class Histograms hSignal_mH30_mSusy800(integratedLuminosity, "mH30_mSusy800", {signal_mH30_mSusy800}, kRed, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // class Histograms hSignal_mH50_mSusy800(integratedLuminosity, "mH50_mSusy800", {signal_mH50_mSusy800}, kGreen, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hSignal_mH70_mSusy800(integratedLuminosity, "mH70_mSusy800", {signal_mH70_mSusy800}, kMagenta, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // class Histograms hSignal_mH90_mSusy800(integratedLuminosity, "mH90_mSusy800", {signal_mH90_mSusy800}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // ----------------------------------
    // class Histograms hSignal_mH30_mSusy1200(integratedLuminosity, "mH30_mSusy1200", {signal_mH30_mSusy1200}, kRed, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // class Histograms hSignal_mH50_mSusy1200(integratedLuminosity, "mH50_mSusy1200", {signal_mH50_mSusy1200}, kGreen, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hSignal_mH70_mSusy1200(integratedLuminosity, "mH70_mSusy1200", {signal_mH70_mSusy1200}, kMagenta, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // class Histograms hSignal_mH90_mSusy1200(integratedLuminosity, "mH90_mSusy1200", {signal_mH90_mSusy1200}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // ----------------------------------
    // class Histograms hSignal_mH30_mSusy1600(integratedLuminosity, "mH30_mSusy1600", {signal_mH30_mSusy1600}, kRed, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // class Histograms hSignal_mH50_mSusy1600(integratedLuminosity, "mH50_mSusy1600", {signal_mH50_mSusy1600}, kGreen, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hSignal_mH70_mSusy1600(integratedLuminosity, "mH70_mSusy1600", {signal_mH70_mSusy1600}, kMagenta, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // class Histograms hSignal_mH90_mSusy1600(integratedLuminosity, "mH90_mSusy1600", {signal_mH90_mSusy1600}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // ----------------------------------
    // class Histograms hSignal_mH30_mSusy2000(integratedLuminosity, "mH30_mSusy2000", {signal_mH30_mSusy2000}, kRed, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // class Histograms hSignal_mH50_mSusy2000(integratedLuminosity, "mH50_mSusy2000", {signal_mH50_mSusy2000}, kGreen, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hSignal_mH70_mSusy2000(integratedLuminosity, "mH70_mSusy2000", {signal_mH70_mSusy2000}, kMagenta, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // class Histograms hSignal_mH90_mSusy2000(integratedLuminosity, "mH90_mSusy2000", {signal_mH90_mSusy2000}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    // ----------------------------------
    class Histograms hTtbar(integratedLuminosity, "ttbar+jets", {TTJets_inclusive, TTJets_ht600to800, TTJets_ht800to1200,TTJets_ht1200to2500,TTJets_ht2500toInf}, PlottingTools::SetColor(1,5), cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hZ(integratedLuminosity, "Z+jets", {ZJetsToQQ_ht600toInf}, PlottingTools::SetColor(2,5), cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hW(integratedLuminosity, "W+jets", {WJetsToQQ_ht600toInf}, PlottingTools::SetColor(3,5), cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hZZ(integratedLuminosity, "ZZ+jets", {ZZTo4Q, ZZTo2L2Q, ZZTo2Q2Nu}, PlottingTools::SetColor(4,5), cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hWW(integratedLuminosity, "WW+jets", {WWTo4Q, WWTo1L1Nu2Q}, PlottingTools::SetColor(5,5), cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
      

    // FIVE: group the Histograms into signal and background
    // std::vector<class Histograms> signalHistograms = {hSignal_mH30_mSusy800,hSignal_mH50_mSusy800,hSignal_mH70_mSusy800,hSignal_mH90_mSusy800};
    // std::vector<class Histograms> signalHistograms = {hSignal_mH30_mSusy1200,hSignal_mH50_mSusy1200,hSignal_mH70_mSusy1200,hSignal_mH90_mSusy1200}; 
    // std::vector<class Histograms> signalHistograms = {hSignal_mH30_mSusy1600,hSignal_mH50_mSusy1600,hSignal_mH70_mSusy1600,hSignal_mH90_mSusy1600};
    // std::vector<class Histograms> signalHistograms = {hSignal_mH30_mSusy2000,hSignal_mH50_mSusy2000,hSignal_mH70_mSusy2000,hSignal_mH90_mSusy2000};
    std::vector<class Histograms> signalHistograms = {hSignal_mH70_mSusy800,hSignal_mH70_mSusy1200,hSignal_mH70_mSusy1600,hSignal_mH70_mSusy2000};
    
    std::vector<class Histograms> backgroundHistograms = {hTtbar, hZ, hW, hZZ, hWW}; // the order here is how they will be plotted
    // std::vector<class Histograms> backgroundHistograms = {hZ, hW}; // the order here is how they will be plotted


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //// E N D * O F * I N P U T S ///////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
 
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string dirExistCommand = "test -e " + outputDirectory;
    std::string makeDirCommand = "mkdir -p " + outputDirectory + "/logY/";
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    else {
        std::cout << "desired output directory, " << outputDirectory << " already exists\nExiting..." << std::endl;
        return 1;
    }
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/main/investigateEventSelectionCuts.cc %s",outputDirectory.c_str()));

    // Create the table
    std::ofstream table;
    table.open(Form("%s/aSignificanceTable_%.0finvFb.csv",outputDirectory.c_str(),integratedLuminosity));
    table << "*SIGNIFICANCE TABLE* significance nDifference (nSignal|nBackground)" << "\n";
    // table << "NB: don't use the first mass bin for counting" << "\n";
    table << "Luminosity = " << integratedLuminosity << "fb^-1," << "\n";
    table << "Backgrounds used:," << "\n";
    for (size_t i = 0; i < backgroundHistograms.size(); ++i) table << backgroundHistograms[i].legend.c_str() << "\n";
    table << "*CUTS USED*,";
    for (size_t i = 0; i < signalHistograms.size(); ++i) table << signalHistograms[i].legend.c_str() << ",";
    table << "\n";
    
    // Plot and Save the combined histograms
    TStyle * tdrStyle = PlottingTools::TDRStyle();
    tdrStyle->cd();
    TLatex * latex = new TLatex();
    latex->SetNDC();
    latex->SetTextFont(42);

    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::string labelsString = "ak8Dbt_" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "__";
                    labelsString += "ak8Pt_" + std::to_string(cut3_ak8Pt[iCut3]) + "__";
                    labelsString += "ht_" + std::to_string(cut4_ht[iCut4][0]) + "to" + std::to_string(cut4_ht[iCut4][1]) + "__";
                    labelsString += "ak4Pt_" + std::to_string(cut5_ak4Pt[iCut5][0]) + "n" + std::to_string(cut5_ak4Pt[iCut5][1]);

                    // std::vector<std::string> histoTagVec = {"fatJetA_prunedMass","fatJetA_softDropMass","fatJetB_prunedMass","fatJetB_softDropMass"};
                    std::vector<std::string> histoTagVec = {"fatJetA_softDropMass"}; // just focus on leading DBT AK8 jet to minimize plots
                    for (const std::string& histoTag : histoTagVec){

                        TCanvas * c1 = new TCanvas("c1","c1");

                        // signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetXaxis()->SetTitle("");
                        signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetXaxis()->SetTitleSize(0.06); 
                        signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetXaxis()->SetLabelSize(0.05);
                        // signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetXaxis()->SetRangeUser(0,190);
                        // signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetYaxis()->SetTitle("events");
                        signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetYaxis()->SetTitleSize(0.06);
                        signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetYaxis()->SetLabelSize(0.05);                    

                        THStack * hs1 = new THStack("hs1","");
                        for (size_t i = 0; i < backgroundHistograms.size(); ++i){
                            backgroundHistograms[i].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->SetFillColor(backgroundHistograms[i].colour);
                            backgroundHistograms[i].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->SetLineWidth(0.0);
                            hs1->Add(backgroundHistograms[i].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]);
                        }
                        double max = 0.0;
                        for (size_t i = 0; i < signalHistograms.size(); ++i){
                            if (signalHistograms[i].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetMaximum() > max)
                                max = signalHistograms[i].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->GetMaximum();
                        }
                        if (hs1->GetMaximum() > max) max = hs1->GetMaximum();
                        signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->SetMaximum(1.05*max);
                        signalHistograms[0].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->Draw();                    
                        hs1->Draw("same");
                        for (size_t i = 0; i < signalHistograms.size(); ++i) signalHistograms[i].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())]->Draw("same");

                        // Add latex stamps
                        latex->SetTextAlign(11); // align from left
                        latex->DrawLatex(0.15,0.92,"#bf{CMS} #it{Simulation} W.I.P");
                        latex->SetTextAlign(31); // align from right
                        latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));
                        // Add legend
                        TLegend * legend1 = new TLegend();
                        legend1->SetTextSize(0.04);
                        legend1->SetX1NDC(0.60);
                        legend1->SetX2NDC(0.87);
                        legend1->SetY1NDC(0.57);
                        legend1->SetY2NDC(0.87);
                        for (size_t i = 0; i < backgroundHistograms.size(); ++i) legend1->AddEntry(backgroundHistograms[i].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())], backgroundHistograms[i].legend.c_str(),"f");
                        for (size_t i = 0; i < signalHistograms.size(); ++i) legend1->AddEntry(signalHistograms[i].h_[Form("%s__%s",histoTag.c_str(),labelsString.c_str())], signalHistograms[i].legend.c_str(),"L");
                        legend1->Draw("same");                   
                        gPad->RedrawAxis();
                        c1->SaveAs(Form("%s/%s_%.0finvFb__%s.pdf",outputDirectory.c_str(),histoTag.c_str(),integratedLuminosity,labelsString.c_str()));
                        gPad->SetLogy();
                        c1->SaveAs(Form("%s/logY/%s_%.0finvFb__%s__LOG.pdf",outputDirectory.c_str(),histoTag.c_str(),integratedLuminosity,labelsString.c_str()));
                        c1->Close();

                    } // closes loop through histoTagVec

                    // *******************************
                    // *** THE SIGNIFICANCE TABLE ***
                    table << Form("%s",labelsString.c_str()) << ",";
                    double nBackgroundObserved = 0.0;
                    for (size_t i = 0; i < backgroundHistograms.size(); ++i){
                        // nBackgroundObserved += backgroundHistograms[i].h_[Form("fatJetA_softDropMass__%s",labelsString.c_str())]->GetEntries(); // note that this method does not work due to the way the histograms were filled
                        for (int iBin = 0; iBin < backgroundHistograms[i].h_[Form("fatJetA_softDropMass__%s",labelsString.c_str())]->GetNbinsX()+2; ++iBin)
                            nBackgroundObserved += backgroundHistograms[i].h_[Form("fatJetA_softDropMass__%s",labelsString.c_str())]->GetBinContent(iBin);
                    }

                    for (size_t i = 0; i < signalHistograms.size(); ++i){
                        
                        double nSignalObserved = 0.0;
                        for (int iBin = 0; iBin < signalHistograms[i].h_[Form("fatJetA_softDropMass__%s",labelsString.c_str())]->GetNbinsX()+2; ++iBin)
                            nSignalObserved += signalHistograms[i].h_[Form("fatJetA_softDropMass__%s",labelsString.c_str())]->GetBinContent(iBin);

                        double significance = nSignalObserved / sqrt(nBackgroundObserved);
                        // double eSignificance = significance * sqrt( (1/nSignalObserved) + (0.25/nBackgroundObserved) ); // nb this isn't quite the right calculation of the error
                        double nDifferenceObserved = nSignalObserved - nBackgroundObserved;
                        // table << Form("%.1f (%.1f | %.1f),", significance, nSignalObserved, nBackgroundObserved);
                        table << Form("%.1f & %.1f (%.1f | %.1f),", significance, nDifferenceObserved, nSignalObserved, nBackgroundObserved);
                    } // closes loop through the signalHistograms
                    table << "\n";                    
                    // *******************************
                    // *******************************
                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states

    table.close();

return 0;
} // closes function 'main'



//--------constructor---------//
Histograms::Histograms(double luminosityD, std::string legendD, std::vector<struct dataInput> dataInputVecD, int colourD, std::vector<std::vector<std::string>> cut2_ak8Dbt, std::vector<int> cut3_ak8Pt, std::vector<std::vector<int>> cut4_ht, std::vector<std::vector<int>> cut5_ak4Pt)
{
    luminosity = luminosityD;
    dataInputVec = dataInputVecD;
    legend = legendD;
    colour = colourD;
    
    // Info for 1d mass histos
    massMin = 0.0;
    massMax = 200.0;
    numberOfBins = 50;

    CreateHistograms(cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    FillHistograms(cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
} // closes the Histograms constructor



//-----------private----------//

void Histograms::CreateHistograms(std::vector<std::vector<std::string>> cut2_ak8Dbt, std::vector<int> cut3_ak8Pt, std::vector<std::vector<int>> cut4_ht, std::vector<std::vector<int>> cut5_ak4Pt)
{
    double MassPerBin = (massMax - massMin) / numberOfBins; // aim for this value to be an integer

    for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
        for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
            for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){

                    std::string labelsString = "ak8Dbt_" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "__";
                    labelsString += "ak8Pt_" + std::to_string(cut3_ak8Pt[iCut3]) + "__";
                    labelsString += "ht_" + std::to_string(cut4_ht[iCut4][0]) + "to" + std::to_string(cut4_ht[iCut4][1]) + "__";
                    labelsString += "ak4Pt_" + std::to_string(cut5_ak4Pt[iCut5][0]) + "n" + std::to_string(cut5_ak4Pt[iCut5][1]);
                    // std::cout << labelsString << std::endl;

                    // histo names should match branches in the tree (important in the FillHistograms stage)
                    h_[Form("fatJetA_prunedMass__%s",labelsString.c_str())] =                
                    new TH1F(Form("fatJetA_prunedMass__%s",labelsString.c_str()), Form(";LeadingBDiscFatJet_Pruned_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
                    h_[Form("fatJetA_prunedMass__%s",labelsString.c_str())]->SetLineWidth(2);
                    h_[Form("fatJetA_prunedMass__%s",labelsString.c_str())]->SetLineColor(colour);

                    h_[Form("fatJetA_softDropMass__%s",labelsString.c_str())] =                
                    new TH1F(Form("fatJetA_softDropMass__%s",labelsString.c_str()), Form(";LeadingBDiscFatJet_SoftDrop_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
                    h_[Form("fatJetA_softDropMass__%s",labelsString.c_str())]->SetLineWidth(2);
                    h_[Form("fatJetA_softDropMass__%s",labelsString.c_str())]->SetLineColor(colour);

                    h_[Form("fatJetB_prunedMass__%s",labelsString.c_str())] =                
                    new TH1F(Form("fatJetB_prunedMass__%s",labelsString.c_str()), Form(";SecondaryBDiscFatJet_Pruned_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
                    h_[Form("fatJetB_prunedMass__%s",labelsString.c_str())]->SetLineWidth(2);
                    h_[Form("fatJetB_prunedMass__%s",labelsString.c_str())]->SetLineColor(colour);                     

                    h_[Form("fatJetB_softDropMass__%s",labelsString.c_str())] =                
                    new TH1F(Form("fatJetB_softDropMass__%s",labelsString.c_str()), Form(";SecondaryBDiscFatJet_SoftDrop_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
                    h_[Form("fatJetB_softDropMass__%s",labelsString.c_str())]->SetLineWidth(2);
                    h_[Form("fatJetB_softDropMass__%s",labelsString.c_str())]->SetLineColor(colour);     

                } // closes loop through cut 5 states
            } // closes loop through cut 4 states
        } // closes loop through cut 3 states
    } // closes loop through cut 2 states
    return;
} // closes the function 'CreateHistograms'



void Histograms::FillHistograms(std::vector<std::vector<std::string>> cut2_ak8Dbt, std::vector<int> cut3_ak8Pt, std::vector<std::vector<int>> cut4_ht, std::vector<std::vector<int>> cut5_ak4Pt)
{
    for (dataInput& dataInputElement : dataInputVec){ // loop through all the dataInput's provided

        TFile * f = TFile::Open(dataInputElement.rootFileName.c_str());
        
        TTree * evT = (TTree*)f->Get("eventCountTree");
        Int_t evTEntries = (Int_t)evT->GetEntries();
        UInt_t nEvtsRunOverForEntry;
        evT->SetBranchAddress("nEvtsRunOver",&nEvtsRunOverForEntry);
        UInt_t nEvtsRunOverTotal = 0;
        for (Int_t ievT=0; ievT<evTEntries; ++ievT){
            evT->GetEntry(ievT);
            nEvtsRunOverTotal += nEvtsRunOverForEntry;
        }
        
        double eventWeighting = 1000.0 * dataInputElement.crossSection * luminosity / nEvtsRunOverTotal;
        // std::cout << nEvtsRunOverTotal << std::endl;
        // std::cout << eventWeighting << std::endl;
        // std::cout << std::endl;

        TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");    

        for (size_t iCut2 = 0; iCut2 < cut2_ak8Dbt.size(); ++iCut2){
            for (size_t iCut3 = 0; iCut3 < cut3_ak8Pt.size(); ++iCut3){
                for (size_t iCut4 = 0; iCut4 < cut4_ht.size(); ++iCut4){        
                    for (size_t iCut5 = 0; iCut5 < cut5_ak4Pt.size(); ++iCut5){
                            
                        std::string labelsString = "ak8Dbt_" + cut2_ak8Dbt[iCut2][0] + cut2_ak8Dbt[iCut2][1] + "__";
                        labelsString += "ak8Pt_" + std::to_string(cut3_ak8Pt[iCut3]) + "__";
                        labelsString += "ht_" + std::to_string(cut4_ht[iCut4][0]) + "to" + std::to_string(cut4_ht[iCut4][1]) + "__";
                        labelsString += "ak4Pt_" + std::to_string(cut5_ak4Pt[iCut5][0]) + "n" + std::to_string(cut5_ak4Pt[iCut5][1]);

                        // std::vector<std::string> histoTagVec = {"fatJetA_prunedMass","fatJetA_softDropMass","fatJetB_prunedMass","fatJetB_softDropMass"};
                        std::vector<std::string> histoTagVec = {"fatJetA_softDropMass"};
                        for (const std::string& histoTag : histoTagVec){

                            TH1F h = TH1F("h", "", numberOfBins, massMin, massMax);
                            
                            std::string drawStringA = Form("%s>>h", histoTag.c_str());
                            std::string drawStringB;                             
                            if (dataInputElement.lheHtCut[0] == 1) drawStringB = Form("%f*(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && lheHT>=%d && lheHT<%d)", eventWeighting, dbtNameToFloat(cut2_ak8Dbt[iCut2][0]), dbtNameToFloat(cut2_ak8Dbt[iCut2][1]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1], dataInputElement.lheHtCut[1], dataInputElement.lheHtCut[2]);
                            else drawStringB = Form("%f*(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d)", eventWeighting, dbtNameToFloat(cut2_ak8Dbt[iCut2][0]), dbtNameToFloat(cut2_ak8Dbt[iCut2][1]), cut3_ak8Pt[iCut3], cut3_ak8Pt[iCut3], cut4_ht[iCut4][0], cut4_ht[iCut4][1], cut5_ak4Pt[iCut5][0], cut5_ak4Pt[iCut5][1]); 
                            // std::cout << drawStringA << std::endl;
                            // std::cout << drawStringB << std::endl;
                            std::cout << "Filling for: " << dataInputElement.rootFileName << std::endl;
                            std::cout << "Cut applied: " << drawStringB << std::endl;
                            std::cout << std::endl;

                            T->Draw(drawStringA.c_str(), drawStringB.c_str(), "");
                            for (int iBin = 0; iBin < h.GetNbinsX()+2; ++iBin) h_[Form("%s__%s", histoTag.c_str(), labelsString.c_str())]->AddBinContent(iBin, h.GetBinContent(iBin));

                        } // closes loop through histoTagVec

                    } // closes loop through cut 5 states
                } // closes loop through cut 4 states
            } // closes loop through cut 3 states
        } // closes loop through cut 2 states

    } // closes loop through the dataInput's provided
    return;
} // closes the function 'FillHistograms'




double dbtNameToFloat(std::string dtbString){

    double dbtFloat;
    if (dtbString == "Off") dbtFloat = -100.0;
    else if (dtbString == "Loose") dbtFloat = 0.3;
    else if (dtbString == "Med") dbtFloat = 0.6;
    else if (dtbString == "Tight") dbtFloat = 0.9;
    else{
        std::cout << "Have not specified all DBT strings correctly\nExiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    return dbtFloat;
}