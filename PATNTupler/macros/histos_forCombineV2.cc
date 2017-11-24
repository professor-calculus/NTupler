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


void GetHistograms(std::map<std::string,TH1D*>&); // NEED TO CHANGE THE FILE PATH IN THIS FUNCTION WHEN USING NEW HISTOGRAMS
void doQcdHistograms(const std::string& htBin, const unsigned int& numberOfBins, const std::vector<double>& data_binContentUnD, const std::vector<double>& data_binErrorUnD, const std::vector<std::vector<double>>& mcVec_binContentUnD, const std::vector<std::vector<double>>& mcVec_binErrorUnD);

class CombineHistogramSet{

public:
    // constructor    
    CombineHistogramSet(const std::string&, const unsigned int&, std::map<std::string,TH1D*>&, const unsigned int&, const bool& = false);
    
    std::vector<double> Get_UnD_binContentVec();
    std::vector<double> Get_UnD_binErrorVec();

private:
    void WriteHistograms();
    bool isThisData;
    bool eventWeightsOkay;
    std::map<std::string, TH1D*> hErr_;
    TH1D * h;
    TH1D * h_data;
    std::vector<double> UnD_binContentVec;
    std::vector<double> UnD_binErrorVec;
};



int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    // ONE: save info
    const std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/testingHistogramsForCombineToolNEW001/"; // where we are going to save the output plots (should include the samples name, and any important features)
  

    // TWO: labels for the original ht binning of the histograms and number of bins in histo
    const std::vector<std::string> ht_bins = {"ht1500to2500", "ht2500to3500", "ht3500toInf"};
    const size_t numberOfBins_original = 30;


    // THREE: Samples
    const std::string dataSample = "QCD"; // use dummy data until we can unblind true data
    const std::string signalSample = "mH70_mSusy2000";
    const std::vector<std::string> monteCarloBackgrounds = {"TTJets", "ZJets"};


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_forCombine.cc %s/%s__histos_forCombine.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    std::map<std::string, TH1D*> hOriginal_;
    GetHistograms(hOriginal_);
    const size_t numberOfBins_new = numberOfBins_original / ht_bins.size();


    // loop through the different ht bins
    for (size_t iHt = 0; iHt < ht_bins.size(); ++iHt){ 

        std::cout << "HTBIN = " << ht_bins[iHt] << std::endl;
        const std::string outputFileName = outputDir + "/combineTH1D" + ht_bins[iHt] + ".root";
        TFile * outputFile = new TFile(outputFileName.c_str(), "RECREATE");
        size_t firstBin_original = iHt * numberOfBins_new;

        CombineHistogramSet data = CombineHistogramSet(dataSample, numberOfBins_new, hOriginal_, firstBin_original, true);
        CombineHistogramSet signal = CombineHistogramSet(signalSample, numberOfBins_new, hOriginal_, firstBin_original);
        std::vector<CombineHistogramSet> mcVec;
        std::vector<std::vector<double>> mcVec_binContentUnD;
        std::vector<std::vector<double>> mcVec_binErrorUnD;
        for (size_t iMC = 0; iMC < monteCarloBackgrounds.size(); ++iMC){
            mcVec.push_back( CombineHistogramSet(monteCarloBackgrounds[iMC], numberOfBins_new, hOriginal_, firstBin_original) );
            mcVec_binContentUnD.push_back(mcVec[iMC].Get_UnD_binContentVec());
            mcVec_binErrorUnD.push_back(mcVec[iMC].Get_UnD_binErrorVec());
        }

        doQcdHistograms(ht_bins[iHt], numberOfBins_new, data.Get_UnD_binContentVec(), data.Get_UnD_binErrorVec(), mcVec_binContentUnD, mcVec_binErrorUnD);

        outputFile->Close();
        std::cout << "Created the ROOT file: " << outputFileName << std::endl;
        std::cout << std::endl;
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




CombineHistogramSet::CombineHistogramSet(const std::string& processName, const unsigned int& numberOfBins, std::map<std::string,TH1D*>& hOriginal_, const unsigned int& firstBin_original, const bool& isThisDataDummy) :
isThisData(isThisDataDummy),
eventWeightsOkay(true)
{

    if (isThisData){
        std::cout << "creating histograms for process: " << processName << " (this is our data sample)" << std::endl;
/*
SPECIAL NOTE:
for the true dataset the count of DATA_{U+D}^{tag} and DATA_{S}^{tag} should be an integer and the event weight should be unity
whilst we are using a dummy dataset this will not be the case...
>>> to replicate true data we round binContent to the nearest integer AND binError = sqrt(binContent).
>>> we also set event weight to unity.
*/
        h_data = new TH1D("data_obs", ";bin;events / bin", numberOfBins, 0, numberOfBins);
        h = NULL;

        const double eventWeight = 1.0;
        unsigned int numberOfEmptyBins_UnD = 0;

        for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

            double binContent_S = round( hOriginal_[Form("S_tag_%s", processName.c_str())]->GetBinContent(iBin + firstBin_original) );
            h_data->SetBinContent(iBin, binContent_S);
            h_data->SetBinError(iBin, 0);           

            double binContent_UnD = round( hOriginal_[Form("UnD_tag_%s", processName.c_str())]->GetBinContent(iBin + firstBin_original) );
            if (binContent_UnD == 0) numberOfEmptyBins_UnD++;
            UnD_binContentVec.push_back(binContent_UnD);
        }

        for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

            unsigned int iVec = iBin - 1;       
            double binError_UnD = -8000.0;
            if (UnD_binContentVec[iVec] > 0) binError_UnD = eventWeight * sqrt(UnD_binContentVec[iVec]);
            else if (numberOfEmptyBins_UnD < numberOfBins) binError_UnD = eventWeight * sqrt(1.0 / numberOfEmptyBins_UnD);
            else binError_UnD = 0.0;
            UnD_binErrorVec.push_back(binError_UnD);
        }

    } // closes 'if' isThisData == true


    else{
        std::cout << "creating histograms for process: " << processName << std::endl;
        h_data = NULL;
        h = new TH1D(processName.c_str(), ";bin;events / bin", numberOfBins, 0, numberOfBins); 

        unsigned int numberOfEmptyBins_S = 0;
        unsigned int numberOfEmptyBins_UnD = 0;
        std::vector<double> eventWeightVec;

        for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

            // initialize the stat. fluctuation histograms
            hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBin)] = new TH1D(Form("%s_bin%dStatUp", processName.c_str(), iBin), ";bin;events / bin", numberOfBins, 0, numberOfBins);
            hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBin)] = new TH1D(Form("%s_bin%dStatDown", processName.c_str(), iBin), ";bin;events / bin", numberOfBins, 0, numberOfBins);

            //////////////////////////////////////
            // properties of the S_{i}^{tag} count
            double binContent_S = hOriginal_[Form("S_tag_%s", processName.c_str())]->GetBinContent(iBin + firstBin_original);
            h->SetBinContent(iBin, binContent_S);
            h->SetBinError(iBin, 0);              

            double binError_S = hOriginal_[Form("S_tag_%s", processName.c_str())]->GetBinError(iBin + firstBin_original);
            if (binContent_S == 0) numberOfEmptyBins_S++; 
            else eventWeightVec.push_back(binError_S * binError_S / binContent_S);

            ////////////////////////////////////////
            // properties of the U+D_{i}^{tag} count
            double binContent_UnD = hOriginal_[Form("UnD_tag_%s", processName.c_str())]->GetBinContent(iBin + firstBin_original);
            UnD_binContentVec.push_back(binContent_UnD);

            double binError_UnD = hOriginal_[Form("UnD_tag_%s", processName.c_str())]->GetBinError(iBin + firstBin_original);
            if (binContent_UnD == 0) numberOfEmptyBins_UnD++; 
            else eventWeightVec.push_back(binError_UnD * binError_UnD / binContent_UnD);

        } // closes loop through individual bins (for the first time)


        // check that all event weights for a given process are the same
        double eventWeight = -7000.0;
        for (size_t iEW = 0; iEW < eventWeightVec.size(); ++iEW){
            if (iEW == 0) eventWeight = eventWeightVec[0];
            if (iEW != 0 && float(eventWeightVec[iEW]) != float(eventWeightVec[iEW-1]) ){
                std::cout << "WARNING: The event weights are not the same for process^^^!" << std::endl;
                eventWeightsOkay = false;
            }
        }


        for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){
   
            //////////////////////////////////////
            // properties of the S_{i}^{tag} count
            double binContent_S = hOriginal_[Form("S_tag_%s", processName.c_str())]->GetBinContent(iBin + firstBin_original);
            double binError_S = hOriginal_[Form("S_tag_%s", processName.c_str())]->GetBinError(iBin + firstBin_original);

            // fill in the statistical fluctuations for the iBin'th bin of the iBin'th systematic histogram
            if (binContent_S > 0){ // non empty bin
                hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBin)]->SetBinContent(iBin, binContent_S + binError_S);
                hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBin)]->SetBinContent(iBin, binContent_S - binError_S);
                hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBin)]->SetBinError(iBin, 0);
                hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBin)]->SetBinError(iBin, 0);
            }
            else if (binContent_S == 0.0 && numberOfEmptyBins_S < numberOfBins){ // empty bin, but not all bins in histogram are empty
                double binErrorSpecial = eventWeight * sqrt(1.0 / numberOfEmptyBins_S);
                hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBin)]->SetBinContent(iBin, binErrorSpecial);
                hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBin)]->SetBinContent(iBin, 0);
                hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBin)]->SetBinError(iBin, 0);
                hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBin)]->SetBinError(iBin, 0);
            }
            else { // empty bin, and all bins in this histogram are empty
                hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBin)]->SetBinContent(iBin, 0);
                hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBin)]->SetBinContent(iBin, 0);
                hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBin)]->SetBinError(iBin, 0);
                hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBin)]->SetBinError(iBin, 0);                   
            }

            // fill in the (null by definition) statistical fluctuations for the iBin'th bin of the NOT iBin'th systematic histogram
            for (unsigned int iBinLabel = 1; iBinLabel < numberOfBins + 1; ++iBinLabel){
                if (iBinLabel != iBin){
                    hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBinLabel)]->SetBinContent(iBin, binContent_S);
                    hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBinLabel)]->SetBinContent(iBin, binContent_S);
                    hErr_[Form("%s_bin%dStatUp", processName.c_str(), iBinLabel)]->SetBinError(iBin, 0);
                    hErr_[Form("%s_bin%dStatDown", processName.c_str(), iBinLabel)]->SetBinError(iBin, 0);   
                }
            }
            ////////////////////////////////////////
            // properties of the U+D_{i}^{tag} count
            if (eventWeightsOkay){
                unsigned int iVec = iBin - 1;
                double binError_UnD = -8000.0;
                if (UnD_binContentVec[iVec] > 0) binError_UnD = eventWeight * sqrt(UnD_binContentVec[iVec]);
                else if (numberOfEmptyBins_UnD < numberOfBins) binError_UnD = eventWeight * sqrt(1.0 / numberOfEmptyBins_UnD);
                else binError_UnD = 0.0;
                UnD_binErrorVec.push_back(binError_UnD);
            }

        } // closes loop through individual bins (for the second time)
    } // closes 'if' NOT data
    WriteHistograms();
} // closes the constructor

void CombineHistogramSet::WriteHistograms()
{
    if (eventWeightsOkay == false){
        std::cout << "The event weights are not the same... WILL NOT WRITE THE HISTOGRAMS !!!" << std::endl;
        std::cout << std::endl;
        return;
    }

    if (isThisData) h_data->Write();
    else {
        h->Write();
        for (auto & hErrEle : hErr_ ) hErrEle.second->Write();
    }
    return;
}

std::vector<double> CombineHistogramSet::Get_UnD_binContentVec(){return UnD_binContentVec;}
std::vector<double> CombineHistogramSet::Get_UnD_binErrorVec(){return UnD_binErrorVec;}




void doQcdHistograms(const std::string& htBin, const unsigned int& numberOfBins, const std::vector<double>& data_binContentUnD, const std::vector<double>& data_binErrorUnD, const std::vector<std::vector<double>>& mcVec_binContentUnD, const std::vector<std::vector<double>>& mcVec_binErrorUnD){

    std::cout << "Doing ABCD method for QCD" << std::endl;

    // check all the vectors are the right size
    if (data_binContentUnD.size() != numberOfBins){
        std::cout << "NOT DOING QCD: The number of entries in the vector data_binContentUnD does not match the number that should be in the desired histogram" << std::endl;
        return;
    }
    if (data_binErrorUnD.size() != numberOfBins){
        std::cout << "NOT DOING QCD: The number of entries in the vector data_binErrorUnD does not match the number that should be in the desired histogram" << std::endl;
        return;
    }
    if (mcVec_binContentUnD.size() != mcVec_binErrorUnD.size()){
        std::cout << "NOT DOING QCD: The number of MC entries is different between mcVec_binContentUnD and mcVec_binErrorUnD" << std::endl;
        return;
    }
    for (size_t iMC = 0; iMC < mcVec_binContentUnD.size(); ++iMC){
        if (mcVec_binContentUnD[iMC].size() != numberOfBins){
            std::cout << "NOT DOING QCD: The number of entries in the vector mcVec_binContentUnD[" << iMC << "] does not match the number that should be in the desired histogram" << std::endl;
            return;
        }
        if (mcVec_binErrorUnD[iMC].size() != numberOfBins){
            std::cout << "NOT DOING QCD: The number of entries in the vector mcVec_binErrorUnD[" << iMC << "] does not match the number that should be in the desired histogram" << std::endl;
            return;
        }                
    }

    // now do the real thing...
    TH1D * h_QCD = new TH1D("QCD", ";bin;events / bin", numberOfBins, 0, numberOfBins);
    std::map<std::string, TH1D*> hErr_;

    // body = (U+D)_{data}^{tag} - sum_over_mc[ (U+D)_{MC}^{tag} ]
    // deltaBodyUp = upward stat. error
    // deltaBodyDown = downward stat. error
    std::vector<double> bodyVec;
    std::vector<double> deltaBodyDownVec;
    std::vector<double> deltaBodyUpVec;

    for (unsigned int iVec = 0; iVec < numberOfBins; ++iVec){

        double body = data_binContentUnD[iVec];
        double deltaBodyDownSqrd = data_binErrorUnD[iVec] * data_binErrorUnD[iVec];
        double deltaBodyUpSqrd = deltaBodyDownSqrd;

        for (size_t iMC = 0; iMC < mcVec_binContentUnD.size(); ++iMC){
            body = body - mcVec_binContentUnD[iMC][iVec];
            deltaBodyDownSqrd = deltaBodyDownSqrd + mcVec_binErrorUnD[iMC][iVec] * mcVec_binErrorUnD[iMC][iVec];
            if (mcVec_binContentUnD[iMC][iVec] > 0) deltaBodyUpSqrd = deltaBodyUpSqrd + mcVec_binErrorUnD[iMC][iVec] * mcVec_binErrorUnD[iMC][iVec];
        }
        bodyVec.push_back(body);
        deltaBodyDownVec.push_back( sqrt(deltaBodyDownSqrd) );
        deltaBodyUpVec.push_back( sqrt(deltaBodyUpSqrd) );

        unsigned int iBin = iVec + 1;
        hErr_[Form("QCD_bin%dStatUp", iBin)] = new TH1D(Form("QCD_bin%dStatUp", iBin), ";bin;events / bin", numberOfBins, 0, numberOfBins);
        hErr_[Form("QCD_bin%dStatDown", iBin)] = new TH1D(Form("QCD_bin%dStatDown", iBin), ";bin;events / bin", numberOfBins, 0, numberOfBins);

    } // closes loop through the bins (first time)


    for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

        unsigned int iVec = iBin - 1;
        double ratio = QcdSidebandCorr::GetCorr(htBin.c_str(), iBin);

        if (bodyVec[iVec] > 0) h_QCD->SetBinContent(iBin, ratio * bodyVec[iVec]);
        else h_QCD->SetBinContent(iBin, 0);
        h_QCD->SetBinError(iBin, 0);

        if (bodyVec[iVec] + deltaBodyUpVec[iVec] > 0) hErr_[Form("QCD_bin%dStatUp", iBin)]->SetBinContent(iBin, ratio * (bodyVec[iVec] + deltaBodyUpVec[iVec]) );
        else hErr_[Form("QCD_bin%dStatUp", iBin)]->SetBinContent(iBin, 0);
        hErr_[Form("QCD_bin%dStatUp", iBin)]->SetBinError(iBin, 0);

        if (bodyVec[iVec] - deltaBodyDownVec[iVec] > 0) hErr_[Form("QCD_bin%dStatDown", iBin)]->SetBinContent(iBin, ratio * (bodyVec[iVec] - deltaBodyDownVec[iVec]) );
        else hErr_[Form("QCD_bin%dStatDown", iBin)]->SetBinContent(iBin, 0);
        hErr_[Form("QCD_bin%dStatDown", iBin)]->SetBinError(iBin, 0);

        // fill in the (null by definition) statistical fluctuations for the iBin'th bin of the NOT iBin'th systematic histogram
        for (unsigned int iBinLabel = 1; iBinLabel < numberOfBins + 1; ++iBinLabel){
            if (iBinLabel != iBin){
                                
                if (bodyVec[iVec] > 0){
                    hErr_[Form("QCD_bin%dStatUp", iBinLabel)]->SetBinContent(iBin, ratio * bodyVec[iVec]);
                    hErr_[Form("QCD_bin%dStatDown", iBinLabel)]->SetBinContent(iBin, ratio * bodyVec[iVec]);
                }
                else{
                    hErr_[Form("QCD_bin%dStatUp", iBinLabel)]->SetBinContent(iBin, 0);
                    hErr_[Form("QCD_bin%dStatDown", iBinLabel)]->SetBinContent(iBin, 0);
                }
                hErr_[Form("QCD_bin%dStatUp", iBinLabel)]->SetBinError(iBin, 0);
                hErr_[Form("QCD_bin%dStatDown", iBinLabel)]->SetBinError(iBin, 0);   
            }
        }

    } // closes loop through the bins (second time)

    h_QCD->Write();
    for (auto & hErrEle : hErr_ ) hErrEle.second->Write();

} // closes function doQcdHistograms