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


// RE-FORMAT HISTOGRAMS TO USE WITH COMBINED


void GetHistograms(std::map<std::string,TH1D*>&);
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    // ONE: save info
    std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/testingHistogramsForCombineTool6/"; // where we are going to save the output plots (should include the samples name, and any important features)
  

    // TWO: what was the ht binning of the original histograms
    std::vector<std::string> ht_bins = {"1500to2500", "2500to3500", "3500toInf"};
    size_t numberOfBins_original = 30;


    // THREE: 
    std::vector<std::string> processNameVec;
    processNameVec.push_back("mH70_mSusy2000");
    processNameVec.push_back("QCD");
    processNameVec.push_back("TTJets");
    processNameVec.push_back("ZJets");


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    size_t numberOfBins_new = numberOfBins_original / ht_bins.size();
    
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_forCombine.cc %s/%s__histos_forCombine.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    // explanation of terminology
    // 1. S, U, D --> refers to mass space. pred is the prediction of S. UnD is the sum U+D.
    // 2. tag, anti, control --> refers to 2*DBT space
    // 3. sample name on the end
    std::map<std::string, TH1D*> h_;
    GetHistograms(h_);


    // loop through the different ht bins
    for (size_t iHt = 0; iHt < ht_bins.size(); ++iHt){ 

        std::string outputFileName = outputDir + "/combineTH1D_ht" + ht_bins[iHt] + ".root";
        TFile * outputFile = new TFile(outputFileName.c_str(), "RECREATE");

        // TH1D * h_data = new TH1D("data_obs", ";bin;events / bin", numberOfBins_new, 0, numberOfBins_new);

        
        // loop through the different processes
        for (size_t iPro = 0; iPro < processNameVec.size(); ++iPro){

            size_t firstBin_original = iHt * numberOfBins_new;
            TH1D * h = new TH1D(processNameVec[iPro].c_str(), ";bin;events / bin", numberOfBins_new, 0, numberOfBins_new);
            std::map<std::string, TH1D*> hErr_;

            unsigned int numberOfEmptyBins = 0;
            std::vector<double> eventWeightVec;
            for (unsigned int iBin = 1; iBin < numberOfBins_new + 1; ++iBin){

                hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)] = new TH1D(Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin), ";bin;events / bin", numberOfBins_new, 0, numberOfBins_new);
                hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)] = new TH1D(Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin), ";bin;events / bin", numberOfBins_new, 0, numberOfBins_new);
                
                double binContent = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original);
                if (binContent == 0) numberOfEmptyBins++;
                else{
                    double binError = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinError(iBin + firstBin_original);
                    double eventWeight = binError * binError / binContent;
                    eventWeightVec.push_back(eventWeight);
                }
            } // closes loop through individual bins

            // calculate the event_weighting* (we use it for the stat. error of empty bins)
            // if entry composed of multiple event_weightings use the average and print a warning message
            double averageEventWeight = 0.0;
            bool writeWarning = false;
            for (size_t iEW = 0; iEW < eventWeightVec.size(); ++iEW){
                if (iEW != 0 && float(eventWeightVec[iEW]) != float(eventWeightVec[iEW-1])) writeWarning = true;
                averageEventWeight += eventWeightVec[iEW];
            }
            if (eventWeightVec.size() > 0) averageEventWeight = averageEventWeight / eventWeightVec.size();
            if (writeWarning == true && numberOfEmptyBins > 0){
                std::cout << std::endl;
                std::cout << "WARNING! Event weight for ";
                std::cout << "SAMPLE: " << processNameVec[iPro];
                std::cout << ", HT bin: " << ht_bins[iHt];
                std::cout << ", is not uniform" << std::endl;
                std::cout << "...and we need this weight for the ~stat fluctation of empty bins" << std::endl;
                std::cout << "The effective weights are: " << std::endl;
                for (size_t iEW = 0; iEW < eventWeightVec.size(); ++iEW) std::cout << eventWeightVec[iEW] << ", ";
                std::cout << std::endl;
                std::cout << "Using the mean of these values as the weight = " << averageEventWeight << std::endl;
                std::cout << std::endl;
            }

            // FILL THE HISTOS
            for (unsigned int iBin = 1; iBin < numberOfBins_new + 1; ++iBin){

                // fill the iBin'th bin of the histogram
                double binContent = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinContent(iBin + firstBin_original);             
                h->SetBinContent(iBin, binContent);
                h->SetBinError(iBin, 0);

                // fill in the statistical fluctuations for the iBin'th bin of the iBin'th systematic histogram
                if (binContent > 0){
                    double binError = h_[Form("S_tag_%s", processNameVec[iPro].c_str())]->GetBinError(iBin + firstBin_original);
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, binContent+binError);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, binContent-binError);
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                }
                else if (binContent == 0.0 && numberOfEmptyBins < numberOfBins_new){
                    double binError = averageEventWeight * sqrt(1.0/numberOfEmptyBins);
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, binError);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, 0);
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                }
                else {
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinContent(iBin, 0);
                    hErr_[Form("%s_bin%dStatUp", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processNameVec[iPro].c_str(), iBin)]->SetBinError(iBin, 0);                   
                }

                // fill in the (null) statistical fluctuations for the iBin'th bin of the NOT iBin'th systematic histogram
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
            // h_data->Add(h);
            for (auto & hErrEle : hErr_ ) hErrEle.second->Write();

        } // closes loop through the different processes

        // h_data->Write();
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

    std::string postamble_noAk4 = "MassCutsV04_ak8pt300_ht1500x2500x3500x_ak4pt-1n-1_lumi37.root"; // comment out when working on MC
    std::vector<std::string> histoNameVec_noAk4;
    // histoNameVec_noAk4.push_back("Data_JetHt2016_goldenJson_NOAK4");
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
        h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtMed2MaxAndMed2Max");
        h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtMed2MaxAndMed2Max");

        h_[Form("S_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose");
        h_[Form("U_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose");
        h_[Form("D_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose");

        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add(h_[Form("D_tag_%s", histoToUse.c_str())]);

        h_[Form("UnD_anti_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_anti_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_anti_%s", histoToUse.c_str())]->Add(h_[Form("D_anti_%s", histoToUse.c_str())]);
   
        h_[Form("pred_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("pred_tag_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("pred_tag_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);

    } // closes loop through histoNameVec
}