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
#include "TimeStamp.h"
#include "QcdSidebandCorr.h"


// RE-FORMAT HISTOGRAMS TO USE WITH COMBINED


void GetHistograms(std::map<std::string,TH1D*>&);
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/testingHistogramsForCombineTool1000/"; // where we are going to save the output plots (should include the samples name, and any important features)
  

    // TWO: labels for the original ht binning of the histograms and number of bins in histo
    std::vector<std::string> ht_bins = {"ht1500to2500", "ht2500to3500", "ht3500toInf"};
    size_t numberOfBins_original = 30;


    // THREE: Samples
    std::string dataSample = "QCD"; // use dummy data until we can unblind true data
    std::string signalSample = "mH70_mSusy2000";
    std::vector<std::string> monteCarloBackgrounds = {"TTJets", "ZJets"};


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<std::string> processNameVec;
    processNameVec.push_back(dataSample); // first element is Data, used for S_{data}^tag and ABCD estimation of this
    processNameVec.push_back(signalSample); // second element is the signal sample
    for (size_t iMC = 0; iMC < monteCarloBackgrounds.size(); ++iMC) processNameVec.push_back(monteCarloBackgrounds[iMC]); // the other elements are MC backgrounds

    size_t numberOfBins_new = numberOfBins_original / ht_bins.size();
    
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_forCombine.cc %s/%s__histos_forCombine.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    std::map<std::string, TH1D*> h_;
    GetHistograms(h_);

    // loop through the different ht bins
    for (size_t iHt = 0; iHt < ht_bins.size(); ++iHt){ 

        std::string outputFileName = outputDir + "/combineTH1D" + ht_bins[iHt] + ".root";
        TFile * outputFile = new TFile(outputFileName.c_str(), "RECREATE");

        TH1D * h_data = new TH1D("data_obs", ";bin;events / bin", numberOfBins_new, 0, numberOfBins_new);
        

        // loop through the different processes
        for (size_t iPro = 0; iPro < processNameVec.size(); ++iPro){

            size_t firstBin_original = iHt * numberOfBins_new;
            TH1D * h = new TH1D(processNameVec[iPro].c_str(), ";bin;events / bin", numberOfBins_new, 0, numberOfBins_new);
            std::map<std::string, TH1D*> hErr_;

            // create the statistical fluctation histograms to fill later
            // count number of empty bins for a given process and ht bin
            // calculate the event_weighting (we use it for the pseudo stat. error of empty bins)
            unsigned int numberOfEmptyBins = 0;
            std::vector<double> eventWeightVec;
            for (unsigned int iBin = 1; iBin < numberOfBins_new + 1; ++iBin){

                hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)] = new TH1D(Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin), ";bin;events / bin", numberOfBins_new, 0, numberOfBins_new);
                hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)] = new TH1D(Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin), ";bin;events / bin", numberOfBins_new, 0, numberOfBins_new);

                double binContent = -8881.81; // intialize with crazy value                
                double eventWeight = -8882.82; // intialize with crazy value

                if (iPro == 0){ // DATA - note that this is purely for UnD^tag and not for the full ABCD calculation.
                    /*
                    SPECIAL NOTE:
                    for the true dataset the count of UnD^tag should be an integer and the event weight should be unity
                    whilst we are using a dummy dataset this will not be the case...
                    >>> to replicate true data we round binContent to the nearest integer AND set binError = sqrt(binContent).
                    >>> we also set event weight to unity.
                    */
                    binContent = round( h_[Form("UnD_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original) );
                    eventWeight = 1.0;
                }

                else if (iPro == 1){ // SIGNAL
                    binContent = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original);
                    double binError = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinError(iBin + firstBin_original);
                    if (binContent != 0) eventWeight = binError * binError / binContent;
                }

                else { // BACKGROUND MC SAMPLES ::: NOTSURE-handling negative mc
                    double corrFactor = QcdSidebandCorr::GetCorr(ht_bins[iHt], iBin);
                    double binContent_S = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original);
                    double binError_S = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinError(iBin + firstBin_original);
                    double binContent_UnD = h_[Form("UnD_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original);
                    double binError_UnD = h_[Form("UnD_tag_%s", processNameVec[iPro].c_str())]->GetBinError(iBin + firstBin_original);
                    
                    binContent = binContent_S - corrFactor * binContent_UnD;
                    
                    if (binContent_S != 0){
                        eventWeight = binError_S * binError_S / binContent_S;
                        if (binContent_UnD != 0){ // check you get the same event weight in the sideband
                            double eventWeight2 = binError_UnD * binError_UnD / binContent_UnD;
                            if (float(eventWeight) != float(eventWeight2) ){
                                std::cout << std::endl;
                                std::cout << "The event weights for process:" << processNameVec[iPro] << ", htBin:" << ht_bins[iHt];
                                std::cout << ", are not the same between S and U+D for bin:" <<  iBin << ". Exiting..." << std::endl;
                                std::cout << std::endl;
                                outputFile->Close();
                                delete outputFile;
                                return 1;           
                            }
                        }
                    }
                    else if (binContent_UnD != 0) eventWeight = binError_UnD * binError_UnD / binContent_UnD;                
                } // closes 'else' corresponding to background MC samples

                if (binContent == 0) numberOfEmptyBins++;
                else eventWeightVec.push_back(eventWeight);
                
            } // closes loop through individual bins

            // check that all event weights for a given process are the same
            for (size_t iEW = 0; iEW < eventWeightVec.size(); ++iEW){
                if (iEW != 0 && float(eventWeightVec[iEW]) != float(eventWeightVec[iEW-1]) ){
                    std::cout << std::endl;
                    std::cout << "The event weights for process:" << processNameVec[iPro] << ", htBin:" << ht_bins[iHt];
                    std::cout << ", are not the same... Exiting..." << std::endl;
                    std::cout << std::endl;
                    outputFile->Close();
                    delete outputFile;
                    return 1;
                }
            }
            double processEventWeight = -8883.83; // intialize with crazy value
            if (eventWeightVec.size() > 0) processEventWeight = eventWeightVec[0];



            // FILL THE HISTOGRAMS (with bin erros = 0 for combine)
            for (unsigned int iBin = 1; iBin < numberOfBins_new + 1; ++iBin){

                double binContent = -8884.84; // intialize with crazy value
                double binError = -8885.85; // intialize with crazy value
                double corrFactor = QcdSidebandCorr::GetCorr(ht_bins[iHt], iBin);

                if (iPro == 0){ // DATA ABCD PREDICTION and DATA_{S}^{tag}                
                    /*
                    SPECIAL NOTE:
                    for the true dataset the count of UnD^tag should be an integer and the event weight should be unity
                    whilst we are using a dummy dataset this will not be the case...
                    >>> to replicate true data we round binContent to the nearest integer AND set binError = sqrt(binContent).
                    >>> we also set event weight to unity.
                    */
                    double binContent_UnD = round( h_[Form("UnD_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original) );
                    double binError_UnD = sqrt(binContent_UnD);
                    binContent = corrFactor * binContent_UnD;
                    binError = corrFactor * binError_UnD;

                    // SPECIAL CASE: fill the DATA_{S}^{tag} (to replicate data we round to nearest integer)
                    double binContent_sTagData = round( h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original) );
                    h_data->SetBinContent(iBin, binContent_sTagData);
                    h_data->SetBinError(iBin, 0);
                }

                else if (iPro == 1){ // SIGNAL
                    binContent = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original);
                    binError = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinError(iBin + firstBin_original);
                }

                else { // BACKGROUND MC SAMPLES
                    double binContent_S = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original);
                    double binError_S = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinError(iBin + firstBin_original);
                    double binContent_UnD = h_[Form("UnD_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original);
                    double binError_UnD = h_[Form("UnD_tag_%s", processNameVec[iPro].c_str())]->GetBinError(iBin + firstBin_original);
                    binContent = binContent_S - corrFactor * binContent_UnD;
                    binError = sqrt(binError_S * binError_S + corrFactor * corrFactor * binError_UnD * binError_UnD);
                }

                // fill the iBin'th bin of the histogram
                h->SetBinContent(iBin, binContent);
                h->SetBinError(iBin, 0);


                // fill in the statistical fluctuations for the iBin'th bin of the iBin'th systematic histogram
                if (binContent > 0){
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, binContent+binError);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, binContent-binError);
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                }
                else if (binContent == 0.0 && numberOfEmptyBins < numberOfBins_new){
                    double binErrorSpecial = processEventWeight * sqrt(1.0/numberOfEmptyBins);
                    if (iPro == 0){ // correct for the fact that the DATA event weight comes before applying the corrFactor
                        binErrorSpecial = corrFactor * binErrorSpecial;
                    }
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, binErrorSpecial);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, 0);
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                }

// NOTSURE-handling negative mc
                else {
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, 0);
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);                   
                }

                // fill in the (null by definition) statistical fluctuations for the iBin'th bin of the NOT iBin'th systematic histogram
                for (unsigned int iBinLabel = 1; iBinLabel < numberOfBins_new + 1; ++iBinLabel){
                    if (iBinLabel != iBin){
                        hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBinLabel)]->SetBinContent(iBin, binContent);
                        hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBinLabel)]->SetBinContent(iBin, binContent);
                        hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBinLabel)]->SetBinError(iBin, 0);
                        hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBinLabel)]->SetBinError(iBin, 0);   
                    }
                }

            } // close loop through individual bins

            h->Write();
            for (auto & hErrEle : hErr_ ) hErrEle.second->Write();

        } // closes loop through the different processes
        h_data->Write();

        outputFile->Close();
        std::cout << "Created the ROOT file: " << outputFileName << std::endl;
        delete outputFile;

    } // closes loop through the different ht bins
    return 0;
}





void GetHistograms(std::map<std::string,TH1D*>& h_)
{
    // histos locations
    std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_09_28_CMSSW_8_0_29_dbtV4/MassCutsV04/";
    
    std::string postamble = "MassCutsV04_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi37.root";
    std::vector<std::string> histoNameVec;
    // histoNameVec.push_back("Data_JetHt2016_goldenJson"); // comment out when working on MC
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

    std::string postamble_noAk4 = "MassCutsV04_ak8pt300_ht1500x2500x3500x_ak4pt-1n-1_lumi37.root";
    std::vector<std::string> histoNameVec_noAk4;
    // histoNameVec_noAk4.push_back("Data_JetHt2016_goldenJson_NOAK4"); // comment out when working on MC
    histoNameVec_noAk4.push_back("QCD_NOAK4");
    histoNameVec_noAk4.push_back("TTJets_NOAK4");
    histoNameVec_noAk4.push_back("ZJets_NOAK4");
    histoNameVec_noAk4.push_back("WJets_NOAK4");
    histoNameVec_noAk4.push_back("mH70_mSusy800_NOAK4");
    histoNameVec_noAk4.push_back("mH70_mSusy1200_NOAK4");
    histoNameVec_noAk4.push_back("mH70_mSusy1600_NOAK4");
    histoNameVec_noAk4.push_back("mH70_mSusy2000_NOAK4");

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
        h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtMed2MaxAndMed2Max");
        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtMed2MaxAndMed2Max");
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("D_dbtMed2MaxAndMed2Max"));

    } // closes loop through histoNameVec
}