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

//RAL PARTICLE HEADERS
#include "../interface/Plotting.h"



/*
Executing:
$ root -q -b -l $CMSSW_BASE/src/NTupler/PATNTupler/macros/PlottingCombinedInvestigateSelection_vTable.cc
*/



struct dataInput{

    std::string rootFileName;
    double crossSection; // in pb 
};



class Histograms{
public:
    // constructor
    Histograms(double,std::string,std::vector<struct dataInput>,int,std::vector<std::string>,std::vector<std::string>,std::vector<std::string>,std::vector<std::string>,std::vector<std::string>);
    
    std::map<std::string, TH1F*> h_;
    std::string legend;
    int colour;

private:
    void CreateHistograms(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>);
    void FillHistograms(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>);
    std::vector<struct dataInput> dataInputVec;
    double luminosity;
};



void PlottingCombinedInvestigateSelection_vTable(){


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //// U S E R * I N P U T S ///////////////////////////////////////////////////////////////////////////



    // ONE: saving info and lumi
    std::string baseDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/output_InvestigateEventSelection/configuration_007/";
    // std::string outputDirectory = baseDir + "withLooseDBT/table_mSu800"; // where we are going to save the tables
    std::string outputDirectory = baseDir + "table_mH70";
    double integratedLuminosity = 100.0; // the integrated luminosity, in fb^-1 that we normalise our plots to



    // TWO: input histo label info (first element is always off, second element is the choice for table, third element is choice for another table, etc.)
    // -----------------------------------------------------------
    // Set Labels for all the histograms -------------------------> MUST MATCH (or be a subset of) THAT OF THE HISTOS WE ARE GOING TO WORK ON
    
    // std::vector<std::string> step1labels = {"STEP1off", "STEP1on_twoFat", "STEP1on_twoFat", "STEP1on_twoFat", "STEP1on_twoFat"};
    // std::vector<std::string> step2labels = {"STEP2off", "STEP2on_looseLooseDBT", "STEP2on_medMedDBT", "STEP2on_tightMedDBT", "STEP2on_tightTightDBT"};
    // std::vector<std::string> step3labels = {"STEP3off", "STEP3on_twoFat300", "STEP3on_twoFat300", "STEP3on_twoFat300", "STEP3on_twoFat300"};
    // std::vector<std::string> step4labels = {"STEP4off", "STEP4on_HT1500", "STEP4on_HT1500", "STEP4on_HT1500", "STEP4on_HT1500"};
    // std::vector<std::string> step5labels = {"STEP5off", "STEP5on_isoAK4Lead300Sec300", "STEP5on_isoAK4Lead300Sec300", "STEP5on_isoAK4Lead300Sec300", "STEP5on_isoAK4Lead300Sec300"};

    // std::vector<std::string> step1labels = {"STEP1off", "STEP1on_twoFat", "STEP1on_twoFat", "STEP1on_twoFat", "STEP1on_twoFat", "STEP1on_twoFat"};
    // std::vector<std::string> step2labels = {"STEP2off", "STEP2on_tightMedDBT", "STEP2on_tightMedDBT", "STEP2on_tightMedDBT", "STEP2on_tightMedDBT", "STEP2on_tightMedDBT"};
    // std::vector<std::string> step3labels = {"STEP3off", "STEP3on_twoFat300", "STEP3on_twoFat300", "STEP3on_twoFat300", "STEP3on_twoFat300", "STEP3on_twoFat300"};
    // std::vector<std::string> step4labels = {"STEP4off", "STEP4on_HT1500", "STEP4on_HT1500", "STEP4on_HT1500", "STEP4on_HT1500", "STEP4on_HT1500"};
    // std::vector<std::string> step5labels = {"STEP5off", "STEP5on_isoAK4Lead200Sec200", "STEP5on_isoAK4Lead250Sec250", "STEP5on_isoAK4Lead300Sec300", "STEP5on_isoAK4Lead350Sec350", "STEP5on_isoAK4Lead400Sec400"};

    std::vector<std::string> step1labels = {"STEP1off", "STEP1on_twoFat", "STEP1on_twoFat", "STEP1on_twoFat", "STEP1on_twoFat"};
    std::vector<std::string> step2labels = {"STEP2off", "STEP2on_tightMedDBT", "STEP2on_tightTightDBT", "STEP2on_tightMedDBT", "STEP2on_tightTightDBT"};
    std::vector<std::string> step3labels = {"STEP3off", "STEP3on_twoFat300", "STEP3on_twoFat300", "STEP3on_twoFat300", "STEP3on_twoFat300"};
    std::vector<std::string> step4labels = {"STEP4off", "STEP4on_HT1500", "STEP4on_HT1500", "STEP4on_HT2500", "STEP4on_HT2500"};
    std::vector<std::string> step5labels = {"STEP5off", "STEP5on_isoAK4Lead300Sec300", "STEP5on_isoAK4Lead300Sec300", "STEP5on_isoAK4Lead300Sec300", "STEP5on_isoAK4Lead300Sec300"};

    // -----------------------------------------------------------



    // THREE: set the dataInput's to be used (this is a separate object in case you have multiple inputs to a single histogram with different cross sections)
    // SIGNAL
    // ----------------------------------
    struct dataInput signal_mH30_mSusy800;
    signal_mH30_mSusy800.rootFileName = baseDir + "mH30p0_mSusy800p0_ratio0p99_splitting0p1/histos.root";
    signal_mH30_mSusy800.crossSection = 4.947 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH50_mSusy800;
    signal_mH50_mSusy800.rootFileName = baseDir + "mH50p0_mSusy800p0_ratio0p99_splitting0p1/histos.root";
    signal_mH50_mSusy800.crossSection = 4.947 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH70_mSusy800;
    signal_mH70_mSusy800.rootFileName = baseDir + "mH70p0_mSusy800p0_ratio0p99_splitting0p1/histos.root";
    signal_mH70_mSusy800.crossSection = 4.947 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH90_mSusy800;
    signal_mH90_mSusy800.rootFileName = baseDir + "mH90p0_mSusy800p0_ratio0p99_splitting0p1/histos.root";
    signal_mH90_mSusy800.crossSection = 4.947 * 0.57 * 0.57; // in pb
    // ----------------------------------
    struct dataInput signal_mH30_mSusy1200;
    signal_mH30_mSusy1200.rootFileName = baseDir + "mH30p0_mSusy1200p0_ratio0p99_splitting0p1/histos.root";
    signal_mH30_mSusy1200.crossSection = 0.375 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH50_mSusy1200;
    signal_mH50_mSusy1200.rootFileName = baseDir + "mH50p0_mSusy1200p0_ratio0p99_splitting0p1/histos.root";
    signal_mH50_mSusy1200.crossSection = 0.375 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH70_mSusy1200;
    signal_mH70_mSusy1200.rootFileName = baseDir + "mH70p0_mSusy1200p0_ratio0p99_splitting0p1/histos.root";
    signal_mH70_mSusy1200.crossSection = 0.375 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH90_mSusy1200;
    signal_mH90_mSusy1200.rootFileName = baseDir + "mH90p0_mSusy1200p0_ratio0p99_splitting0p1/histos.root";
    signal_mH90_mSusy1200.crossSection = 0.375 * 0.57 * 0.57; // in pb
    // ----------------------------------
    struct dataInput signal_mH30_mSusy1600;
    signal_mH30_mSusy1600.rootFileName = baseDir + "mH30p0_mSusy1600p0_ratio0p99_splitting0p1/histos.root";
    signal_mH30_mSusy1600.crossSection = 0.044 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH50_mSusy1600;
    signal_mH50_mSusy1600.rootFileName = baseDir + "mH50p0_mSusy1600p0_ratio0p99_splitting0p1/histos.root";
    signal_mH50_mSusy1600.crossSection = 0.044 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH70_mSusy1600;
    signal_mH70_mSusy1600.rootFileName = baseDir + "mH70p0_mSusy1600p0_ratio0p99_splitting0p1/histos.root";
    signal_mH70_mSusy1600.crossSection = 0.044 * 0.57 * 0.57; // in pb

    struct dataInput signal_mH90_mSusy1600;
    signal_mH90_mSusy1600.rootFileName = baseDir + "mH90p0_mSusy1600p0_ratio0p99_splitting0p1/histos.root";
    signal_mH90_mSusy1600.crossSection = 0.044 * 0.57 * 0.57; // in pb
    // ----------------------------------
    // BACKGROUND
    struct dataInput TTJets_ht800to1200;
    TTJets_ht800to1200.rootFileName = baseDir + "TTJets_ht800to1200/histos.root";
    TTJets_ht800to1200.crossSection = 1.0980821984; // in pb

    struct dataInput TTJets_ht1200to2500;
    TTJets_ht1200to2500.rootFileName = baseDir + "TTJets_ht1200to2500/histos.root";
    TTJets_ht1200to2500.crossSection = 0.1987479092; // in pb

    struct dataInput TTJets_ht2500toInf;
    TTJets_ht2500toInf.rootFileName = baseDir + "TTJets_ht2500toInf/histos.root";
    TTJets_ht2500toInf.crossSection = 0.002368412585; // in pb

    struct dataInput ZJetsToQQ_ht600toInf;
    ZJetsToQQ_ht600toInf.rootFileName = baseDir + "ZJetsToQQ_ht600toInf/histos.root";
    ZJetsToQQ_ht600toInf.crossSection = 5.67; // in pb 

    struct dataInput WJetsToQQ_ht600toInf;
    WJetsToQQ_ht600toInf.rootFileName = baseDir + "WJetsToQQ_ht600toInf/histos.root";
    WJetsToQQ_ht600toInf.crossSection = 95.14; // in pb 



    // FOUR: group the input datasets and create Histograms
    std::vector<struct dataInput> tmp;
    // ----------------------------------
    tmp = {signal_mH30_mSusy800};    
    class Histograms hSignal_mH30_mSusy800(integratedLuminosity, "mH30_mSusy800", tmp, kRed, step1labels, step2labels, step3labels, step4labels, step5labels);

    tmp = {signal_mH50_mSusy800}; 
    class Histograms hSignal_mH50_mSusy800(integratedLuminosity, "mH50_mSusy800", tmp, kGreen, step1labels, step2labels, step3labels, step4labels, step5labels);
    
    tmp = {signal_mH70_mSusy800};
    class Histograms hSignal_mH70_mSusy800(integratedLuminosity, "mH70_mSusy800", tmp, kBlue, step1labels, step2labels, step3labels, step4labels, step5labels);
    
    tmp = {signal_mH90_mSusy800};
    class Histograms hSignal_mH90_mSusy800(integratedLuminosity, "mH90_mSusy800", tmp, kBlack, step1labels, step2labels, step3labels, step4labels, step5labels);
    // ----------------------------------
    tmp = {signal_mH30_mSusy1200};    
    class Histograms hSignal_mH30_mSusy1200(integratedLuminosity, "mH30_mSusy1200", tmp, kRed, step1labels, step2labels, step3labels, step4labels, step5labels);

    tmp = {signal_mH50_mSusy1200}; 
    class Histograms hSignal_mH50_mSusy1200(integratedLuminosity, "mH50_mSusy1200", tmp, kGreen, step1labels, step2labels, step3labels, step4labels, step5labels);
    
    tmp = {signal_mH70_mSusy1200};
    class Histograms hSignal_mH70_mSusy1200(integratedLuminosity, "mH70_mSusy1200", tmp, kBlue, step1labels, step2labels, step3labels, step4labels, step5labels);
    
    tmp = {signal_mH90_mSusy1200};
    class Histograms hSignal_mH90_mSusy1200(integratedLuminosity, "mH90_mSusy1200", tmp, kBlack, step1labels, step2labels, step3labels, step4labels, step5labels);
    // ----------------------------------
    tmp = {signal_mH30_mSusy1600};    
    class Histograms hSignal_mH30_mSusy1600(integratedLuminosity, "mH30_mSusy1600", tmp, kRed, step1labels, step2labels, step3labels, step4labels, step5labels);

    tmp = {signal_mH50_mSusy1600}; 
    class Histograms hSignal_mH50_mSusy1600(integratedLuminosity, "mH50_mSusy1600", tmp, kGreen, step1labels, step2labels, step3labels, step4labels, step5labels);
    
    tmp = {signal_mH70_mSusy1600};
    class Histograms hSignal_mH70_mSusy1600(integratedLuminosity, "mH70_mSusy1600", tmp, kBlue, step1labels, step2labels, step3labels, step4labels, step5labels);
    
    tmp = {signal_mH90_mSusy1600};
    class Histograms hSignal_mH90_mSusy1600(integratedLuminosity, "mH90_mSusy1600", tmp, kBlack, step1labels, step2labels, step3labels, step4labels, step5labels);
    // ----------------------------------
    tmp = {TTJets_ht800to1200,TTJets_ht1200to2500,TTJets_ht2500toInf};
    class Histograms hTtbar(integratedLuminosity, "ttbar+jets", tmp, Plotting::SetColor(1,4), step1labels, step2labels, step3labels, step4labels, step5labels);

    tmp = {ZJetsToQQ_ht600toInf};
    class Histograms hZ(integratedLuminosity, "Z+jets", tmp, Plotting::SetColor(3,4), step1labels, step2labels, step3labels, step4labels, step5labels);
 
    tmp = {WJetsToQQ_ht600toInf};
    class Histograms hW(integratedLuminosity, "W+jets", tmp, Plotting::SetColor(4,4), step1labels, step2labels, step3labels, step4labels, step5labels);



    // FIVE: group the Histograms we are going to use for the table
    // std::vector<class Histograms> histogramsVec = {hSignal_mH30_mSusy800,hSignal_mH50_mSusy800,hSignal_mH70_mSusy800,hSignal_mH90_mSusy800,hTtbar,hZ,hW};
    // std::vector<class Histograms> histogramsVec = {hSignal_mH30_mSusy1200,hSignal_mH50_mSusy1200,hSignal_mH70_mSusy1200,hSignal_mH90_mSusy1200,hTtbar,hZ,hW};
    // std::vector<class Histograms> histogramsVec = {hSignal_mH30_mSusy1600,hSignal_mH50_mSusy1600,hSignal_mH70_mSusy1600,hSignal_mH90_mSusy1600,hTtbar,hZ,hW};
    std::vector<class Histograms> histogramsVec = {hSignal_mH70_mSusy800,hSignal_mH70_mSusy1200,hSignal_mH70_mSusy1600,hTtbar,hZ};



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

    if (step1labels.size() != step2labels.size() || step2labels.size() != step3labels.size() || step3labels.size() != step4labels.size() || step4labels.size() != step5labels.size()){
        std::cout << "stepXlabels not all same size, exiting..." << std::endl;
        return;
    }

    if (step1labels.size() < 2){
        std::cout << "stepXlabels size < 2, exiting..." << std::endl;
        return;
    }

    std::string dirExistCommand = "test -e " + outputDirectory;
    std::string makeDirCommand = "mkdir -p " + outputDirectory + "/tmpCompare/";
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    else {
        std::cout << "desired output directory, " << outputDirectory << " already exists\nExiting..." << std::endl;
        return;
    }
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/PlottingCombinedInvestigateSelection_vTable.cc %s",outputDirectory.c_str()));

    for (size_t iTab = 1; iTab < step1labels.size(); ++iTab){

        // Create the table
        std::ofstream table;
        table.open(Form("%s/cutCount_%.0finvFb__%s__%s__%s__%s__%s.csv",outputDirectory.c_str(),integratedLuminosity,step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[iTab].c_str(),step5labels[iTab].c_str()));
        table << "Luminosity = " << integratedLuminosity << "fb^-1," << "\n";
        table << "col1: Number of Events\n";
        table << "col2: Fraction from last stage\n";
        table << "bottom: Total fraction\n";
        table << ",";
        for (size_t i = 0; i < histogramsVec.size(); ++i) table << histogramsVec[i].legend.c_str() << ",,,";
        
        std::vector<double> holdValueVec(histogramsVec.size(), 0.0);
        table << "\nPreCuts:,";
        std::string labelsString = Form("%s__%s__%s__%s__%s",step1labels[0].c_str(),step2labels[0].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            double numEvents = histogramsVec[i].h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->GetEntries(); 
            table << numEvents << ",,,";
            holdValueVec[i] = numEvents;
        }
        table << "\n" << step1labels[iTab] << ",";
        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[0].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            double numEvents = histogramsVec[i].h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->GetEntries();
            table << numEvents << ",";
            if (numEvents < 0.00000001) table << "0,,";
            else table << numEvents / holdValueVec[i] << ",,";
            holdValueVec[i] = numEvents;
        }

        table << "\n" << step2labels[iTab] << ",";
        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            double numEvents = histogramsVec[i].h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->GetEntries();
            table << numEvents << ",";
            if (numEvents < 0.00000001) table << "0,,";
            else table << numEvents / holdValueVec[i] << ",,";
            holdValueVec[i] = numEvents;
        }

        table << "\n" << step3labels[iTab] << ",";
        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            double numEvents = histogramsVec[i].h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->GetEntries();
            table << numEvents << ",";
            if (numEvents < 0.00000001) table << "0,,";
            else table << numEvents / holdValueVec[i] << ",,";
            holdValueVec[i] = numEvents;
        }

        table << "\n" << step4labels[iTab] << ",";
        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[iTab].c_str(),step5labels[0].c_str());
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            double numEvents = histogramsVec[i].h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->GetEntries();
            table << numEvents << ",";
            if (numEvents < 0.00000001) table << "0,,";
            else table << numEvents / holdValueVec[i] << ",,";
            holdValueVec[i] = numEvents;
        }

        table << "\n" << step5labels[iTab] << ",";
        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[iTab].c_str(),step5labels[iTab].c_str());
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            double numEvents = histogramsVec[i].h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->GetEntries();
            table << numEvents << ",";
            if (numEvents < 0.00000001) table << "0,,";
            else table << numEvents / holdValueVec[i] << ",,";
            holdValueVec[i] = numEvents;
        }

        table << "\n,,";
        labelsString = Form("%s__%s__%s__%s__%s",step1labels[0].c_str(),step2labels[0].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        for (size_t i = 0; i < histogramsVec.size(); ++i){
            double numEvents = histogramsVec[i].h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->GetEntries();
            table << holdValueVec[i] / numEvents << ",,,";
        }

        table.close();
    } // closes loop through different table configurations


    std::string copyFilesToTmpCmd = "cp " + outputDirectory + "/*1500.csv " + outputDirectory + "/tmpCompare/";
    // std::system(copyFilesToTmpCmd.c_str());

return;
} // closes function 'PlottingCombinedInvestigateSelection'



//--------constructor---------//
Histograms::Histograms(double luminosityD, std::string legendD, std::vector<struct dataInput> dataInputVecD, int colourD, std::vector<std::string> step1labels, std::vector<std::string> step2labels, std::vector<std::string> step3labels, std::vector<std::string> step4labels, std::vector<std::string> step5labels)
{
    if (step1labels.size() != step2labels.size() || step2labels.size() != step3labels.size() || step3labels.size() != step4labels.size() || step4labels.size() != step5labels.size()){
        std::cout << "stepXlabels not all same size" << std::endl;
        return;
    }

    if (step1labels.size() < 2){
        std::cout << "stepXlabels size < 2" << std::endl;
        return;
    }
    luminosity = luminosityD;
    dataInputVec = dataInputVecD;
    legend = legendD;
    colour = colourD;
    CreateHistograms(step1labels, step2labels, step3labels, step4labels, step5labels);
    FillHistograms(step1labels, step2labels, step3labels, step4labels, step5labels);
} // closes the Histograms constructor



//-----------private----------//

void Histograms::CreateHistograms(std::vector<std::string> step1labels, std::vector<std::string> step2labels, std::vector<std::string> step3labels, std::vector<std::string> step4labels, std::vector<std::string> step5labels)
{
    // NB: the binning in this function must match that in InvestigateEventSelection.cc
    double massMin = 0.0;
    double massMax = 200.0;
    unsigned int numberOfBins = 50;
    double MassPerBin = (massMax - massMin) / numberOfBins; // aim for this value to be an integer


    std::vector<std::string> labelsStringUsedSoFar; // to check we haven't already constructed the histogram
    for (size_t iTab = 1; iTab < step1labels.size(); ++iTab){

        std::string labelsString = Form("%s__%s__%s__%s__%s",step1labels[0].c_str(),step2labels[0].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
            h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())] = new TH1F(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()), Form(";LeadingFatJet_Pruned_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
            labelsStringUsedSoFar.push_back(labelsString);
        }

        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[0].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
            h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())] = new TH1F(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()), Form(";LeadingFatJet_Pruned_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
            labelsStringUsedSoFar.push_back(labelsString);
        }

        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
            h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())] = new TH1F(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()), Form(";LeadingFatJet_Pruned_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
            labelsStringUsedSoFar.push_back(labelsString);
        }

        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
        if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
            h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())] = new TH1F(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()), Form(";LeadingFatJet_Pruned_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
            labelsStringUsedSoFar.push_back(labelsString);
        }

        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[iTab].c_str(),step5labels[0].c_str());
        if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
            h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())] = new TH1F(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()), Form(";LeadingFatJet_Pruned_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
            labelsStringUsedSoFar.push_back(labelsString);
        }

        labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[iTab].c_str(),step5labels[iTab].c_str());
        if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
            h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())] = new TH1F(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()), Form(";LeadingFatJet_Pruned_Mass (GeV);Events / %.1f GeV", MassPerBin), numberOfBins, massMin, massMax);
            labelsStringUsedSoFar.push_back(labelsString);
        }        
    } // closes loop through table configurations
    return;
} // closes the function 'CreateHistograms'



void Histograms::FillHistograms(std::vector<std::string> step1labels, std::vector<std::string> step2labels, std::vector<std::string> step3labels, std::vector<std::string> step4labels, std::vector<std::string> step5labels)
{
    for (dataInput& dataInputElement : dataInputVec){ // loop through all the dataInput's provided

        TFile * f = TFile::Open(dataInputElement.rootFileName.c_str());
        // IMPORTANT: must use a histogram that will always be filled
        TH1F * h_ForInitialNumberEntries = (TH1F*)f->Get(Form("ht__%s__%s__%s__%s__%s",step1labels[0].c_str(),step2labels[0].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str()));
        double intialNumberEntries = h_ForInitialNumberEntries->GetEntries();
        double eventWeighting = 1000.0 * dataInputElement.crossSection * luminosity / intialNumberEntries;
        h_ForInitialNumberEntries->Delete();

        std::vector<std::string> labelsStringUsedSoFar; // so we don't refill the same histogram
        for (size_t iTab = 1; iTab < step1labels.size(); ++iTab){

            std::string labelsString = Form("%s__%s__%s__%s__%s",step1labels[0].c_str(),step2labels[0].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
            if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
                TH1F * h = (TH1F*)f->Get(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()));            
                h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->Add(h,eventWeighting);
                h->Delete();                  
                labelsStringUsedSoFar.push_back(labelsString);
            }

            labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[0].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
            if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
                TH1F * h = (TH1F*)f->Get(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()));            
                h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->Add(h,eventWeighting);
                h->Delete();                  
                labelsStringUsedSoFar.push_back(labelsString);
            }

            labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[0].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
            if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
                TH1F * h = (TH1F*)f->Get(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()));            
                h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->Add(h,eventWeighting);
                h->Delete();                  
                labelsStringUsedSoFar.push_back(labelsString);
            }

            labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[0].c_str(),step5labels[0].c_str());
            if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
                TH1F * h = (TH1F*)f->Get(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()));            
                h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->Add(h,eventWeighting);
                h->Delete();                  
                labelsStringUsedSoFar.push_back(labelsString);
            }

            labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[iTab].c_str(),step5labels[0].c_str());
            if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
                TH1F * h = (TH1F*)f->Get(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()));            
                h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->Add(h,eventWeighting);
                h->Delete();                  
                labelsStringUsedSoFar.push_back(labelsString);
            }

            labelsString = Form("%s__%s__%s__%s__%s",step1labels[iTab].c_str(),step2labels[iTab].c_str(),step3labels[iTab].c_str(),step4labels[iTab].c_str(),step5labels[iTab].c_str());
            if ( (std::find(labelsStringUsedSoFar.begin(), labelsStringUsedSoFar.end(), labelsString.c_str()) == labelsStringUsedSoFar.end()) ){
                TH1F * h = (TH1F*)f->Get(Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str()));            
                h_[Form("leadingBDiscFatJetMassPruned__%s",labelsString.c_str())]->Add(h,eventWeighting);
                h->Delete();                  
                labelsStringUsedSoFar.push_back(labelsString);
            }        
        } // closes loop through table configurations

    } // closes loop through the dataInput's provided
    return;
} // closes the function 'FillHistograms'