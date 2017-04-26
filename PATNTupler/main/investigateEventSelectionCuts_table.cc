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
    double weightedNumberEvents_preCuts;
    double weightedNumberEvents_preSel;
    std::vector<double> weightedNumberEvents_AK8DBT;
    std::vector<double> weightedNumberEvents_AK8PT;
    std::vector<double> weightedNumberEvents_ht;
    std::vector<double> weightedNumberEvents_AK4PT;

private:
    void CountEventsAfterCuts(std::vector<std::vector<std::string>>, std::vector<int>, std::vector<std::vector<int>>, std::vector<std::vector<int>>);
    std::vector<struct dataInput> dataInputVec;
    double luminosity;
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
    std::string baseDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/"; // for the input ROOT files
    std::string outputDirectory = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/output_InvestigateEventSelectionCuts/DEMOTABLE/"; // where we are going to save the plots
    double integratedLuminosity = 50.0; // the integrated luminosity, in fb^-1 that we scale our plots to


    // TWO: input histo label info (first element is the choice for a table, second element is the choice for table, third element is choice for another table, etc.)
    std::vector<std::vector<std::string>> cut2_ak8Dbt = { {"Tight","Med"}, {"Tight","Med"}, {"Tight","Med"} };
    std::vector<int> cut3_ak8Pt = {300, 300, 300};
    std::vector<std::vector<int>>cut4_ht = { {1500,2500}, {2500,3500}, {3500,99000} }; // these are HT bins, not just cuts
    std::vector<std::vector<int>> cut5_ak4Pt = { {300,300}, {300,300}, {300,300} };
    

    // THREE: set the dataInput's to be used (this is a separate object in case you have multiple inputs to a single histogram with different cross sections)
    // SIGNAL
    // ----------------------------------
    struct dataInput signal_mH30_mSusy800;
    signal_mH30_mSusy800.rootFileName = baseDir + "mH30p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH30_mSusy800.crossSection = 4.947 * 0.57 * 0.57; // in pb
    signal_mH30_mSusy800.lheHtCut = {0};

    struct dataInput signal_mH50_mSusy800;
    signal_mH50_mSusy800.rootFileName = baseDir + "mH50p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH50_mSusy800.crossSection = 4.947 * 0.57 * 0.57; // in pb
    signal_mH50_mSusy800.lheHtCut = {0};

    struct dataInput signal_mH70_mSusy800;
    signal_mH70_mSusy800.rootFileName = baseDir + "mH70p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH70_mSusy800.crossSection = 4.947 * 0.57 * 0.57; // in pb
    signal_mH70_mSusy800.lheHtCut = {0};

    struct dataInput signal_mH90_mSusy800;
    signal_mH90_mSusy800.rootFileName = baseDir + "mH90p0_mSusy800p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH90_mSusy800.crossSection = 4.947 * 0.57 * 0.57; // in pb
    signal_mH90_mSusy800.lheHtCut = {0};
    // ----------------------------------
    struct dataInput signal_mH30_mSusy1200;
    signal_mH30_mSusy1200.rootFileName = baseDir + "mH30p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH30_mSusy1200.crossSection = 0.375 * 0.57 * 0.57; // in pb
    signal_mH30_mSusy1200.lheHtCut = {0};

    struct dataInput signal_mH50_mSusy1200;
    signal_mH50_mSusy1200.rootFileName = baseDir + "mH50p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH50_mSusy1200.crossSection = 0.375 * 0.57 * 0.57; // in pb
    signal_mH50_mSusy1200.lheHtCut = {0};

    struct dataInput signal_mH70_mSusy1200;
    signal_mH70_mSusy1200.rootFileName = baseDir + "mH70p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH70_mSusy1200.crossSection = 0.375 * 0.57 * 0.57; // in pb
    signal_mH70_mSusy1200.lheHtCut = {0};

    struct dataInput signal_mH90_mSusy1200;
    signal_mH90_mSusy1200.rootFileName = baseDir + "mH90p0_mSusy1200p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH90_mSusy1200.crossSection = 0.375 * 0.57 * 0.57; // in pb
    signal_mH90_mSusy1200.lheHtCut = {0};
    // ----------------------------------
    struct dataInput signal_mH30_mSusy1600;
    signal_mH30_mSusy1600.rootFileName = baseDir + "mH30p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH30_mSusy1600.crossSection = 0.044 * 0.57 * 0.57; // in pb
    signal_mH30_mSusy1600.lheHtCut = {0};

    struct dataInput signal_mH50_mSusy1600;
    signal_mH50_mSusy1600.rootFileName = baseDir + "mH50p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH50_mSusy1600.crossSection = 0.044 * 0.57 * 0.57; // in pb
    signal_mH50_mSusy1600.lheHtCut = {0};

    struct dataInput signal_mH70_mSusy1600;
    signal_mH70_mSusy1600.rootFileName = baseDir + "mH70p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH70_mSusy1600.crossSection = 0.044 * 0.57 * 0.57; // in pb
    signal_mH70_mSusy1600.lheHtCut = {0};

    struct dataInput signal_mH90_mSusy1600;
    signal_mH90_mSusy1600.rootFileName = baseDir + "mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH90_mSusy1600.crossSection = 0.044 * 0.57 * 0.57; // in pb
    signal_mH90_mSusy1600.lheHtCut = {0};
    // ----------------------------------
    struct dataInput signal_mH30_mSusy2000;
    signal_mH30_mSusy2000.rootFileName = baseDir + "mH30p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH30_mSusy2000.crossSection = 0.006 * 0.57 * 0.57; // in pb
    signal_mH30_mSusy2000.lheHtCut = {0};

    struct dataInput signal_mH50_mSusy2000;
    signal_mH50_mSusy2000.rootFileName = baseDir + "mH50p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH50_mSusy2000.crossSection = 0.006 * 0.57 * 0.57; // in pb
    signal_mH50_mSusy2000.lheHtCut = {0};

    struct dataInput signal_mH70_mSusy2000;
    signal_mH70_mSusy2000.rootFileName = baseDir + "mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH70_mSusy2000.crossSection = 0.006 * 0.57 * 0.57; // in pb
    signal_mH70_mSusy2000.lheHtCut = {0};

    struct dataInput signal_mH90_mSusy2000;
    signal_mH90_mSusy2000.rootFileName = baseDir + "mH90p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root";
    signal_mH90_mSusy2000.crossSection = 0.006 * 0.57 * 0.57; // in pb
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
    class Histograms hTtbar(integratedLuminosity, "ttbar+jets", {TTJets_inclusive, TTJets_ht600to800, TTJets_ht800to1200,TTJets_ht1200to2500,TTJets_ht2500toInf}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hZ(integratedLuminosity, "Z+jets", {ZJetsToQQ_ht600toInf}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hW(integratedLuminosity, "W+jets", {WJetsToQQ_ht600toInf}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hZZ(integratedLuminosity, "ZZ+jets", {ZZTo4Q, ZZTo2L2Q, ZZTo2Q2Nu}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
    class Histograms hWW(integratedLuminosity, "WW+jets", {WWTo4Q, WWTo1L1Nu2Q}, kBlack, cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);


    // FIVE: group the Histograms we are going to use for the table
    // std::vector<class Histograms> histogramsVec = {hSignal_mH30_mSusy800,hSignal_mH50_mSusy800,hSignal_mH70_mSusy800,hSignal_mH90_mSusy800,hTtbar,hZ,hW};
    // std::vector<class Histograms> histogramsVec = {hSignal_mH30_mSusy1200,hSignal_mH50_mSusy1200,hSignal_mH70_mSusy1200,hSignal_mH90_mSusy1200,hTtbar,hZ,hW};
    // std::vector<class Histograms> histogramsVec = {hSignal_mH30_mSusy1600,hSignal_mH50_mSusy1600,hSignal_mH70_mSusy1600,hSignal_mH90_mSusy1600,hTtbar,hZ,hW};
    // std::vector<class Histograms> histogramsVec = {hSignal_mH30_mSusy2000,hSignal_mH50_mSusy2000,hSignal_mH70_mSusy2000,hSignal_mH90_mSusy2000,hTtbar,hZ,hW};
    std::vector<class Histograms> histogramsVec = {hSignal_mH70_mSusy800,hSignal_mH70_mSusy1200,hSignal_mH70_mSusy1600,hSignal_mH70_mSusy2000,hTtbar,hZ,hW,hZZ,hWW};
         

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

    if (cut2_ak8Dbt.size() != cut3_ak8Pt.size() || cut3_ak8Pt.size() != cut4_ht.size() || cut4_ht.size() != cut5_ak4Pt.size()){
        std::cout << "stepXlabels not all same size, exiting..." << std::endl;
        return 1;
    }

    std::string dirExistCommand = "test -e " + outputDirectory;
    std::string makeDirCommand = "mkdir -p " + outputDirectory + "/tmpCompare/";
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    else {
        std::cout << "desired output directory, " << outputDirectory << " already exists\nExiting..." << std::endl;
        return 1;
    }
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/main/investigateEventSelectionCuts_table.cc %s",outputDirectory.c_str()));

    for (size_t iTab = 0; iTab < cut3_ak8Pt.size(); ++iTab){

        // Create the table
        std::ofstream table;
        table.open(Form("%s/cutCount_%.0finvFb__AK8DBT_%s_%s__AK8PT_%d__HT_%dto%d__AK4PT_%d_%d.csv",outputDirectory.c_str(),integratedLuminosity,cut2_ak8Dbt[iTab][0].c_str(),cut2_ak8Dbt[iTab][1].c_str(),cut3_ak8Pt[iTab],cut4_ht[iTab][0],cut4_ht[iTab][1],cut5_ak4Pt[iTab][0],cut5_ak4Pt[iTab][1]));
        table << "Luminosity = " << integratedLuminosity << "fb^-1," << "\n";
        table << "col1: Number of Events\n";
        table << "col2: Fraction from last stage\n";
        table << "bottom: Total fraction\n";
        table << ",";
        for (size_t i = 0; i < histogramsVec.size(); ++i) table << histogramsVec[i].legend.c_str() << ",,,";
        std::vector<double> holdValueVec(histogramsVec.size(), 0.0);

        table << "\nPreCuts:,";
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            table << histogramsVec[i].weightedNumberEvents_preCuts << ",,,";
            holdValueVec[i] = histogramsVec[i].weightedNumberEvents_preCuts;
        }
        
        table << "\n2 FatJets (pre-selection):,";
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            table << histogramsVec[i].weightedNumberEvents_preSel << ",";
            if (histogramsVec[i].weightedNumberEvents_preSel < 0.00000001) table << "0,,";
            else table << histogramsVec[i].weightedNumberEvents_preSel / holdValueVec[i] << ",,";
            holdValueVec[i] = histogramsVec[i].weightedNumberEvents_preSel;
        }

        table << "\nAK8DBT " << cut2_ak8Dbt[iTab][0].c_str() << cut2_ak8Dbt[iTab][1].c_str() << ":,";
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            table << histogramsVec[i].weightedNumberEvents_AK8DBT[iTab] << ",";
            if (histogramsVec[i].weightedNumberEvents_AK8DBT[iTab] < 0.00000001) table << "0,,";
            else table << histogramsVec[i].weightedNumberEvents_AK8DBT[iTab] / holdValueVec[i] << ",,";
            holdValueVec[i] = histogramsVec[i].weightedNumberEvents_AK8DBT[iTab];
        }

        table << "\nAK8PT " << cut3_ak8Pt[iTab] << "(GeV):,";
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            table << histogramsVec[i].weightedNumberEvents_AK8PT[iTab] << ",";
            if (histogramsVec[i].weightedNumberEvents_AK8PT[iTab] < 0.00000001) table << "0,,";
            else table << histogramsVec[i].weightedNumberEvents_AK8PT[iTab] / holdValueVec[i] << ",,";
            holdValueVec[i] = histogramsVec[i].weightedNumberEvents_AK8PT[iTab];
        }
        
        table << "\nHT " << cut4_ht[iTab][0] << "-" << cut4_ht[iTab][1] << "(GeV):,";
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            table << histogramsVec[i].weightedNumberEvents_ht[iTab] << ",";
            if (histogramsVec[i].weightedNumberEvents_ht[iTab] < 0.00000001) table << "0,,";
            else table << histogramsVec[i].weightedNumberEvents_ht[iTab] / holdValueVec[i] << ",,";
            holdValueVec[i] = histogramsVec[i].weightedNumberEvents_ht[iTab];
        }

        table << "\nAK4PT " << cut5_ak4Pt[iTab][0] << "&" << cut5_ak4Pt[iTab][1] << "(GeV):,";
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            table << histogramsVec[i].weightedNumberEvents_AK4PT[iTab] << ",";
            if (histogramsVec[i].weightedNumberEvents_AK4PT[iTab] < 0.00000001) table << "0,,";
            else table << histogramsVec[i].weightedNumberEvents_AK4PT[iTab] / holdValueVec[i] << ",,";
            holdValueVec[i] = histogramsVec[i].weightedNumberEvents_AK4PT[iTab];
        }        

        table << "\n,,";
        for (size_t i = 0; i < histogramsVec.size(); ++i) table << holdValueVec[i] / histogramsVec[i].weightedNumberEvents_preCuts << ",,,";

        table.close();
    } // closes loop through different table configurations
    return 0;
} // closes function 'PlottingCombinedInvestigateSelection'



//--------constructor---------//
Histograms::Histograms(double luminosityD, std::string legendD, std::vector<struct dataInput> dataInputVecD, int colourD, std::vector<std::vector<std::string>> cut2_ak8Dbt, std::vector<int> cut3_ak8Pt, std::vector<std::vector<int>> cut4_ht, std::vector<std::vector<int>> cut5_ak4Pt)
{
    if (cut2_ak8Dbt.size() != cut3_ak8Pt.size() || cut3_ak8Pt.size() != cut4_ht.size() || cut4_ht.size() != cut5_ak4Pt.size()){
        std::cout << "stepXlabels not all same size, exiting..." << std::endl;
        return;
    }

    luminosity = luminosityD;
    dataInputVec = dataInputVecD;
    legend = legendD;
    weightedNumberEvents_preCuts = 0.0;
    weightedNumberEvents_preSel = 0.0;
    std::vector<double> dummyVec(cut2_ak8Dbt.size(), 0.0);
    weightedNumberEvents_AK8DBT = dummyVec;
    weightedNumberEvents_AK8PT = dummyVec;
    weightedNumberEvents_ht = dummyVec;
    weightedNumberEvents_AK4PT = dummyVec;
    CountEventsAfterCuts(cut2_ak8Dbt, cut3_ak8Pt, cut4_ht, cut5_ak4Pt);
} // closes the Histograms constructor



//-----------private----------//
void Histograms::CountEventsAfterCuts(std::vector<std::vector<std::string>> cut2_ak8Dbt, std::vector<int> cut3_ak8Pt, std::vector<std::vector<int>> cut4_ht, std::vector<std::vector<int>> cut5_ak4Pt)
{
    for (dataInput& dataInputElement : dataInputVec){ // loop through all the dataInput's provided

        TFile * f = TFile::Open(dataInputElement.rootFileName.c_str());
        
        TTree * evT = (TTree*)f->Get("eventCountTree");
        Int_t evTEntries = (Int_t)evT->GetEntries();
        
        UInt_t nEvtsRunOverForEntry;
        evT->SetBranchAddress("nEvtsRunOver",&nEvtsRunOverForEntry);
        UInt_t nEvtsRunOverTotal = 0;
        
        UInt_t nEvtsPassPerEntry;
        evT->SetBranchAddress("nEvtsPass",&nEvtsPassPerEntry);
        UInt_t nEvtsPassTotal = 0;
        
        for (Int_t ievT=0; ievT<evTEntries; ++ievT){
            evT->GetEntry(ievT);
            nEvtsRunOverTotal += nEvtsRunOverForEntry;
            nEvtsPassTotal += nEvtsPassPerEntry;
        }
        
        double eventWeighting = 1000.0 * dataInputElement.crossSection * luminosity / nEvtsRunOverTotal;
        // std::cout << nEvtsRunOverTotal << std::endl;
        // std::cout << eventWeighting << std::endl;
        // std::cout << std::endl;

        weightedNumberEvents_preCuts += nEvtsRunOverTotal * eventWeighting;
        weightedNumberEvents_preSel += nEvtsPassTotal * eventWeighting;

        TTree * T = (TTree*)f->Get("doubleBFatJetPairTree");  

        std::vector<std::string> labelsStringUsedSoFar; // so we don't repeat ourselves
        for (size_t iTab = 0; iTab < cut2_ak8Dbt.size(); ++iTab){

            TH1F h2 = TH1F("h2", "", 1, -100.0, 99000.0);
            std::string drawString2B;                             
            if (dataInputElement.lheHtCut[0] == 1) drawString2B = Form("(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && lheHT>=%d && lheHT<%d)", dbtNameToFloat(cut2_ak8Dbt[iTab][0]), dbtNameToFloat(cut2_ak8Dbt[iTab][1]), -100, -100, -100, 99000, -100, -100, dataInputElement.lheHtCut[1], dataInputElement.lheHtCut[2]);
            else drawString2B = Form("(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d)", dbtNameToFloat(cut2_ak8Dbt[iTab][0]), dbtNameToFloat(cut2_ak8Dbt[iTab][1]), -100, -100, -100, 99000, -100, -100); 
            std::cout << "Counting for: " << dataInputElement.rootFileName << std::endl;
            std::cout << "Cut applied: " << drawString2B << std::endl;
            std::cout << std::endl;
            T->Draw("ht>>h2", drawString2B.c_str(), "");
            weightedNumberEvents_AK8DBT[iTab] += eventWeighting * h2.GetEntries();
            // std::cout << eventWeighting << std::endl;
            // std::cout << h2.GetEntries() << std::endl;
            // std::cout << weightedNumberEvents_AK8DBT[iTab] << std::endl;
            // std::cout << std::endl;

            TH1F h3 = TH1F("h3", "", 1, -100.0, 99000.0);
            std::string drawString3B;
            if (dataInputElement.lheHtCut[0] == 1) drawString3B = Form("(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && lheHT>=%d && lheHT<%d)", dbtNameToFloat(cut2_ak8Dbt[iTab][0]), dbtNameToFloat(cut2_ak8Dbt[iTab][1]), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], -100, 99000, -100, -100, dataInputElement.lheHtCut[1], dataInputElement.lheHtCut[2]);
            else drawString3B = Form("(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d)", dbtNameToFloat(cut2_ak8Dbt[iTab][0]), dbtNameToFloat(cut2_ak8Dbt[iTab][1]), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], -100, 99000, -100, -100); 
            std::cout << "Counting for: " << dataInputElement.rootFileName << std::endl;
            std::cout << "Cut applied: " << drawString3B << std::endl;
            std::cout << std::endl;
            T->Draw("ht>>h3", drawString3B.c_str(), "");
            weightedNumberEvents_AK8PT[iTab] += eventWeighting * h3.GetEntries();

            TH1F h4 = TH1F("h4", "", 1, -100.0, 99000.0);
            std::string drawString4B;
            if (dataInputElement.lheHtCut[0] == 1) drawString4B = Form("(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && lheHT>=%d && lheHT<%d)", dbtNameToFloat(cut2_ak8Dbt[iTab][0]), dbtNameToFloat(cut2_ak8Dbt[iTab][1]), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut4_ht[iTab][0], cut4_ht[iTab][1], -100, -100, dataInputElement.lheHtCut[1], dataInputElement.lheHtCut[2]);
            else drawString4B = Form("(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d)", dbtNameToFloat(cut2_ak8Dbt[iTab][0]), dbtNameToFloat(cut2_ak8Dbt[iTab][1]), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut4_ht[iTab][0], cut4_ht[iTab][1], -100, -100); 
            std::cout << "Counting for: " << dataInputElement.rootFileName << std::endl;
            std::cout << "Cut applied: " << drawString4B << std::endl;
            std::cout << std::endl;
            T->Draw("ht>>h4", drawString4B.c_str(), "");
            weightedNumberEvents_ht[iTab] += eventWeighting * h4.GetEntries();

            TH1F h5 = TH1F("h5", "", 1, -100.0, 99000.0);
            std::string drawString5B;
            if (dataInputElement.lheHtCut[0] == 1) drawString5B = Form("(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d && lheHT>=%d && lheHT<%d)", dbtNameToFloat(cut2_ak8Dbt[iTab][0]), dbtNameToFloat(cut2_ak8Dbt[iTab][1]), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut4_ht[iTab][0], cut4_ht[iTab][1], cut5_ak4Pt[iTab][0], cut5_ak4Pt[iTab][1], dataInputElement.lheHtCut[1], dataInputElement.lheHtCut[2]);
            else drawString5B = Form("(fatJetA_doubleBtagDiscrim>%f && fatJetB_doubleBtagDiscrim>%f && fatJetA_p4.Pt()>%d && fatJetB_p4.Pt()>%d && ht>=%d && ht<%d && slimJetA_p4.Pt()>%d && slimJetB_p4.Pt()>%d)", dbtNameToFloat(cut2_ak8Dbt[iTab][0]), dbtNameToFloat(cut2_ak8Dbt[iTab][1]), cut3_ak8Pt[iTab], cut3_ak8Pt[iTab], cut4_ht[iTab][0], cut4_ht[iTab][1], cut5_ak4Pt[iTab][0], cut5_ak4Pt[iTab][1]); 
            std::cout << "Counting for: " << dataInputElement.rootFileName << std::endl;
            std::cout << "Cut applied: " << drawString5B << std::endl;
            std::cout << std::endl;
            T->Draw("ht>>h5", drawString5B.c_str(), "");
            weightedNumberEvents_AK4PT[iTab] += eventWeighting * h5.GetEntries();

        } // closes loop through table configurations
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