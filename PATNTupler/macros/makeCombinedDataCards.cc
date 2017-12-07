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


// MAKE DATACARDS TO USE WITH COMBINED


void GetHistograms(std::map<std::string,TH1D*>&); // NEED TO CHANGE THE FILE PATH IN THIS FUNCTION WHEN USING NEW HISTOGRAMS

class CommonSystematic{
public:
    CommonSystematic(const std::string&, const double&, const std::vector<std::string>&);    
    std::string GetSystematicName() const;
    std::string GetSystematicValue() const;
    std::vector<std::string> GetSystematicProcesses() const;
private:
    std::string systematicName;
    std::string systematicValue;
    std::vector<std::string> systematicProcesses;        
};

double GetEventWeight(const std::string&, std::map<std::string,TH1D*>&, const unsigned int&);
std::vector<int> GetStatErrorLogic(const std::string&, std::map<std::string,TH1D*>&, const unsigned int&, const unsigned int&);

void WriteBlock(const std::string&, const unsigned int&, std::ofstream&, const bool = false);


int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    // ONE: save info (signal specific directories beneath this)
    const std::string outputDirGeneral = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2017_12_07/firstPlay/fullCuts/"; // where we are going to save the output cards (should include the samples name, and any important features)
  

    // TWO: physics info
    const unsigned int numberOfBins = 30;
    const unsigned int numberOfHtDivisions = 3;


    // THREE: Samples to Use
    const std::string dataSample = "Data_JetHt2016_goldenJson";
    std::vector<std::string> signalVec = {"mH30_mSusy1600", "mH50_mSusy1600", "mH70_mSusy1600", "mH90_mSusy1600", "mH30_mSusy2000", "mH50_mSusy2000", "mH70_mSusy2000", "mH90_mSusy2000"}; // the different signal samples you wish to use
    // std::vector<std::string> signalVec = {"mH70_mSusy1600", "mH70_mSusy2000"}; // the different signal samples you wish to use
    const std::vector<std::string> mcbkVec = {"TTJets", "ZJets", "WJets"};
    const std::string qcdName = "QCD"; // this is just a label as QCD contribution is driven during the fit


    // FOUR: data card layout info
    const unsigned int firstColSize = 30;
    const unsigned int otherColSize = 20;


    // FIVE: common systematics 
    // nb, make the sure the sample names match to the above
    // ("SIGNAL" refers to the signal sample and qcdName is used for qcd)
    std::vector<CommonSystematic> CommonSystematicVec;
    CommonSystematicVec.push_back( CommonSystematic("luminosity lnN", 1.2, {"SIGNAL", "TTJets_NOAK4", "ZJets_NOAK4", "WJets_NOAK4"}) );
    CommonSystematicVec.push_back( CommonSystematic("XS_Signal lnN", 1.6, {"SIGNAL"}) );
    CommonSystematicVec.push_back( CommonSystematic("XS_TTJets lnN", 1.5, {"TTJets"}) );
    CommonSystematicVec.push_back( CommonSystematic("XS_ZJets lnN", 1.4, {"ZJets"}) );
    CommonSystematicVec.push_back( CommonSystematic("XS_WJets lnN", 1.3, {"WJets"}) );


    // SIX: are we blinded ? use data_obs_UnD as a dummy for data_obs_S
    bool areWeBlinded = true;


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    if (numberOfBins % numberOfHtDivisions != 0){
        std::cout << "number of ht divisions does not divide into the number of bins. Exiting..." << std::endl;
        return 1;
    }

    std::map<std::string, TH1D*> hOriginal_;
    GetHistograms(hOriginal_);
    
    // get event weightings and see whether we want to set stat errors for mc bkgrd in a ht division
    std::vector<double> signalWeightVec;
    std::vector<double> mcbkWeightVec;
    std::vector<std::vector<int>>  mcbkStatErrorLogicVec;

    for (auto signal : signalVec){
        double eventWeight = GetEventWeight(signal, hOriginal_, numberOfBins);
        signalWeightVec.push_back(eventWeight);
    }
    for (auto mcbk : mcbkVec){
        double eventWeight = GetEventWeight(mcbk, hOriginal_, numberOfBins);
        mcbkWeightVec.push_back(eventWeight);
        mcbkStatErrorLogicVec.push_back( GetStatErrorLogic(mcbk, hOriginal_, numberOfBins, numberOfHtDivisions) );
    }

    // loop through the different signal sample references
    for (auto signal : signalVec){

        const std::string outputDir = outputDirGeneral + "/" + signal + "/";
        const std::string dirExistCommand = "test -e " + outputDir;
        const std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/makeCombinedDataCards.cc %s/%s__makeCombinedDataCards.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

        std::cout << "SIGNAL SAMPLE = " << signal << ": " << outputDir << std::endl;

        for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

            unsigned int data_obs_S = hOriginal_[Form("S_tag_%s", dataSample.c_str())]->GetBinContent(iBin);
            if (areWeBlinded) data_obs_S = hOriginal_[Form("UnD_tag_%s", dataSample.c_str())]->GetBinContent(iBin); // to get a non zero and roughly realistic value whilst we are blinded
            const unsigned int data_obs_UnD = hOriginal_[Form("UnD_tag_%s", dataSample.c_str())]->GetBinContent(iBin);
            const double rate_signal_S = hOriginal_[Form("S_tag_%s", signal.c_str())]->GetBinContent(iBin);
            const double rate_signal_UnD = hOriginal_[Form("UnD_tag_%s", signal.c_str())]->GetBinContent(iBin);;
            const std::string data_obs_S_str = std::to_string(data_obs_S);
            const std::string data_obs_UnD_str = std::to_string(data_obs_UnD);
            const std::string rate_signal_S_str = std::to_string(rate_signal_S);
            const std::string rate_signal_UnD_str = std::to_string(rate_signal_UnD);
            std::vector<double> rate_mcbkVec_S;
            std::vector<double> rate_mcbkVec_UnD;
            std::vector<std::string> rate_mcbkVec_S_str;
            std::vector<std::string> rate_mcbkVec_UnD_str;
            for (auto mcbk : mcbkVec){
                double rate_S = hOriginal_[Form("S_tag_%s", mcbk.c_str())]->GetBinContent(iBin);
                double rate_UnD = hOriginal_[Form("UnD_tag_%s", mcbk.c_str())]->GetBinContent(iBin);
                rate_mcbkVec_S.push_back(rate_S);
                rate_mcbkVec_UnD.push_back(rate_UnD);
                rate_mcbkVec_S_str.push_back( std::to_string(rate_S) );
                rate_mcbkVec_UnD_str.push_back( std::to_string(rate_UnD) );
            }

            // write the command for combining the cards
            std::ofstream comboCommand;
            comboCommand.open( Form("%scomboCommand.sh", outputDir.c_str()) );
            std::string comboCommandStr = "combineCards.py ";
            for (unsigned int i = 1; i < numberOfBins + 1; ++i){
                comboCommandStr += outputDir + "bin" + std::to_string(i) + ".txt ";
            }
            comboCommandStr += "> " + outputDir + "allbins.txt\n";
            comboCommand << comboCommandStr;
            comboCommand.close();

            // write the data card
            std::ofstream dataCard;

            dataCard.open( Form("%sbin%d.txt", outputDir.c_str(),iBin) );
            dataCard << "imax 2\n";
            dataCard << "jmax " << mcbkVec.size() + 1 << "\n";
            dataCard << "kmax *\n";
            dataCard << "------------------------------\n";
            WriteBlock("bin", firstColSize, dataCard);
            WriteBlock("mass_S", otherColSize, dataCard);
            WriteBlock("mass_UnD", otherColSize, dataCard, true);
            WriteBlock("observation", firstColSize, dataCard);
            WriteBlock(data_obs_S_str, otherColSize, dataCard); // in function. make it a template object to write...
            WriteBlock(data_obs_UnD_str, otherColSize, dataCard, true);
            dataCard << "------------------------------\n";
            WriteBlock("bin", firstColSize, dataCard);
            for (unsigned int c = 0; c < mcbkVec.size() + 2; ++c) WriteBlock("mass_S", otherColSize, dataCard);
            for (unsigned int c = 0; c < mcbkVec.size() + 2; ++c) WriteBlock("mass_UnD", otherColSize, dataCard);
            dataCard << "\n";
            WriteBlock("process", firstColSize, dataCard);
            WriteBlock(signal, otherColSize, dataCard);
            for (auto mcbk : mcbkVec) WriteBlock(mcbk, otherColSize, dataCard);
            WriteBlock(qcdName, otherColSize, dataCard);
            WriteBlock(signal, otherColSize, dataCard);
            for (auto mcbk : mcbkVec) WriteBlock(mcbk, otherColSize, dataCard);
            WriteBlock(qcdName, otherColSize, dataCard, true);
            WriteBlock("process", firstColSize, dataCard);
            for (unsigned int i = 0; i < mcbkVec.size() + 2; ++i) WriteBlock(std::to_string(i), otherColSize, dataCard); 
            for (unsigned int i = 0; i < mcbkVec.size() + 2; ++i) WriteBlock(std::to_string(i), otherColSize, dataCard);
            dataCard << "\n";
            WriteBlock("rate", firstColSize, dataCard);
            WriteBlock(rate_signal_S_str, otherColSize, dataCard);
            for (auto rate_mcbk_S_str : rate_mcbkVec_S_str) WriteBlock(rate_mcbk_S_str, otherColSize, dataCard);
            WriteBlock("1", otherColSize, dataCard);
            WriteBlock(rate_signal_UnD_str, otherColSize, dataCard);
            for (auto rate_mcbk_UnD_str : rate_mcbkVec_UnD_str) WriteBlock(rate_mcbk_UnD_str, otherColSize, dataCard);
            WriteBlock("1", otherColSize, dataCard);
            dataCard << "\n------------------------------\n";
            
            dataCard << "# common systematics\n";
            for (auto CommonSystematic : CommonSystematicVec){
                WriteBlock(CommonSystematic.GetSystematicName(), firstColSize, dataCard);

                for (int c = 0; c < 2; c++){

                    const std::string systematicValue = CommonSystematic.GetSystematicValue();
                    const std::vector<std::string> systematicProcesses = CommonSystematic.GetSystematicProcesses();

                    // signal
                    if (std::find(systematicProcesses.begin(), systematicProcesses.end(), "SIGNAL") != systematicProcesses.end()) {
                        WriteBlock(systematicValue, otherColSize, dataCard);
                    } else {
                        WriteBlock("-", otherColSize, dataCard);
                    }
                    // monte carlo background
                    for (auto mcbk : mcbkVec){
                        if (std::find(systematicProcesses.begin(), systematicProcesses.end(), mcbk) != systematicProcesses.end()) {
                            WriteBlock(systematicValue, otherColSize, dataCard);
                        } else {
                            WriteBlock("-", otherColSize, dataCard);
                        }                   
                    }
                    // QCD
                    if (std::find(systematicProcesses.begin(), systematicProcesses.end(), qcdName) != systematicProcesses.end()) {
                        WriteBlock(systematicValue, otherColSize, dataCard);
                    } else {
                        WriteBlock("-", otherColSize, dataCard);
                    }
                } // closes the two counts
                dataCard << "\n";
            }

            dataCard << "\n# unique systematics\n";
            for (size_t iMC = 0; iMC < mcbkVec.size(); ++iMC){
                const unsigned int iVec = iBin - 1;
                if (mcbkStatErrorLogicVec[iMC][iVec] == 1){
                    const int rawCount =  round(rate_mcbkVec_S[iMC] / mcbkWeightVec[iMC]);
                    const std::string statSysName = "ch" + std::to_string(iBin) + "_" + mcbkVec[iMC] + "_S_stats gmN " + std::to_string(rawCount);
                    const std::string mcbkWeightStr = std::to_string(mcbkWeightVec[iMC]);
                    WriteBlock(statSysName, firstColSize, dataCard);
                    WriteBlock("-", otherColSize, dataCard);

                    for (unsigned int c = 0; c < mcbkVec.size(); ++c){
                        if (iMC == c) WriteBlock(mcbkWeightStr, otherColSize, dataCard);
                        else WriteBlock("-", otherColSize, dataCard);
                    }
                    for (unsigned int c = 0; c < mcbkVec.size() + 3; ++c) WriteBlock("-", otherColSize, dataCard);
                    dataCard << "\n";
                }
            }
            for (size_t iMC = 0; iMC < mcbkVec.size(); ++iMC){
                const unsigned int iVec = iBin - 1;
                if (mcbkStatErrorLogicVec[iMC][iVec] == 1){
                    const int rawCount =  round(rate_mcbkVec_UnD[iMC] / mcbkWeightVec[iMC]);
                    const std::string statSysName = "ch" + std::to_string(iBin) + "_" + mcbkVec[iMC] + "_UnD_stats gmN " + std::to_string(rawCount);
                    const std::string mcbkWeightStr = std::to_string(mcbkWeightVec[iMC]);
                    WriteBlock(statSysName, firstColSize, dataCard);
                    WriteBlock("-", otherColSize, dataCard);

                    for (unsigned int c = 0; c < mcbkVec.size() + 2; ++c) WriteBlock("-", otherColSize, dataCard);
                    for (unsigned int c = 0; c < mcbkVec.size(); ++c){
                        if (iMC == c) WriteBlock(mcbkWeightStr, otherColSize, dataCard);
                        else WriteBlock("-", otherColSize, dataCard);
                    }
                    WriteBlock("-", otherColSize, dataCard);
                    dataCard << "\n";
                }
            }
            
            dataCard << "\n# estimate QCD\n";
            double corrRatio = QcdSidebandCorr::GetCorr(iBin);
            double corrRatioError = 0.1 * corrRatio;
            WriteBlock(Form("ch%d_R", iBin), otherColSize, dataCard);
            dataCard << "param " << std::to_string(corrRatio) << " " << std::to_string(corrRatioError) << "\n";
            WriteBlock(Form("ch%d_alpha", iBin), otherColSize, dataCard);
            dataCard << "rateParam mass_UnD " << qcdName << " ";
            double qcdInitialEstimate = data_obs_UnD;
            for (auto rate_mcbk_UnD : rate_mcbkVec_UnD) qcdInitialEstimate = qcdInitialEstimate - rate_mcbk_UnD;
            if (qcdInitialEstimate < 0) qcdInitialEstimate = 0.0;
            double qcdUpperLimit = data_obs_UnD + 3.0 * sqrt(data_obs_UnD + 1);
            dataCard << std::to_string(qcdInitialEstimate) << " " << "[0," << std::to_string(qcdUpperLimit) << "]\n";
            WriteBlock(Form("ch%d_beta", iBin), otherColSize, dataCard);
            dataCard << "rateParam mass_S " << qcdName << " (@0*@1) ch" << iBin << "_R,ch" << iBin << "_alpha\n";

            dataCard.close();

        } // closes loop through the histogram bins

    // combine the data cards into one
    std::system( Form("source %scomboCommand.sh", outputDir.c_str()) );

    } // closes loop through the different signal samples
    return 0; 
}


void GetHistograms(std::map<std::string,TH1D*>& h_)
{
    // histos locations
    std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2017_09_28_CMSSW_8_0_29_dbtV4/MassCutsV04/";
    
    std::string postamble = "MassCutsV04_ak8pt300_ht1500x2500x3500x_ak4pt250n250_lumi37.root";
    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("Data_JetHt2016_goldenJson"); // comment out when working on MC
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
    histoNameVec_noAk4.push_back("Data_JetHt2016_goldenJson_NOAK4"); // comment out when working on MC
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
} // closes function GetHistograms


CommonSystematic::CommonSystematic(const std::string& systematicNameDummy, const double& systematicValueNumber, const std::vector<std::string>& systematicProcessesDummy) :
systematicName(systematicNameDummy),
systematicValue(std::to_string(systematicValueNumber)),
systematicProcesses(systematicProcessesDummy)
{};
std::string CommonSystematic::GetSystematicName() const {return systematicName;}
std::string CommonSystematic::GetSystematicValue() const {return systematicValue;}
std::vector<std::string> CommonSystematic::GetSystematicProcesses() const {return systematicProcesses;}


double GetEventWeight(const std::string& histogramName, std::map<std::string,TH1D*>& hOriginal_, const unsigned int& numberOfBins){

    std::vector<double> eventWeightVec;

    for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){
    
        double binContent_S = hOriginal_[Form("S_tag_%s", histogramName.c_str())]->GetBinContent(iBin);
        double binError_S = hOriginal_[Form("S_tag_%s", histogramName.c_str())]->GetBinError(iBin);
        if (binContent_S != 0) eventWeightVec.push_back(binError_S * binError_S / binContent_S);

        double binContent_UnD = hOriginal_[Form("UnD_tag_%s", histogramName.c_str())]->GetBinContent(iBin);
        double binError_UnD = hOriginal_[Form("UnD_tag_%s", histogramName.c_str())]->GetBinError(iBin);
        if (binContent_UnD != 0) eventWeightVec.push_back(binError_UnD * binError_UnD / binContent_UnD);        
    }

    if (eventWeightVec.size() == 0){
        std::cout << "Not a single entry in S or UnD for the process " << histogramName;
        std::cout << "! Returning a dummy event weighting of -1234.0" << std::endl;
        return -1234.0;
    }

    // check that all event weights for a given process are the same
    double eventWeight = -9876.0;
    for (size_t iEW = 0; iEW < eventWeightVec.size(); ++iEW){
        if (iEW == 0) eventWeight = eventWeightVec[0];
        if (iEW != 0 && float(eventWeightVec[iEW]) != float(eventWeightVec[iEW-1]) ){
            std::cout << "WARNING: The event weights are not the same for process " << histogramName;
            std::cout << "! Returning a dummy event weighting of -9876.0" << std::endl;
            return eventWeight;
        }
    }
    return eventWeight;
}


std::vector<int> GetStatErrorLogic(const std::string& histogramName, std::map<std::string,TH1D*>& hOriginal_, const unsigned int& numberOfBins, const unsigned int& numberOfHtDivisions){

    std::vector<int> StatErrorLogic(numberOfBins,0);
    const unsigned int numberOfBinsPerHtDivision = numberOfBins / numberOfHtDivisions;

    for (unsigned int iVec = 0; iVec < numberOfBins; ++iVec){
    
        const unsigned int iBin = iVec + 1;
        const unsigned int htDivisionIndex = floor(iVec / numberOfBinsPerHtDivision);

        double binContent_S = hOriginal_[Form("S_tag_%s", histogramName.c_str())]->GetBinContent(iBin);
        double binContent_UnD = hOriginal_[Form("UnD_tag_%s", histogramName.c_str())]->GetBinContent(iBin);

        if (binContent_S > 0 || binContent_UnD > 0){
            for (unsigned int c = 0; c < numberOfBinsPerHtDivision * (htDivisionIndex + 1); ++c){
                StatErrorLogic[c] = 1;
            }
        }
    } // closes loop through the bins

    return StatErrorLogic;
} // closes the function GetStatErrorLogic


void WriteBlock(const std::string& strToWrite, const unsigned int& numberOfBlockSpaces, std::ofstream& dataCard, const bool appendNewLine){

    dataCard << strToWrite;
    if (strToWrite.size() < numberOfBlockSpaces){
        for (unsigned int i = 0; i < numberOfBlockSpaces - strToWrite.size(); ++i)
            dataCard << " ";
    }
    else{
        std::cout << "FORMAT ERROR: the string " << strToWrite << " is longer than designated block size" << std::endl;
        dataCard << " ";
    }

    if (appendNewLine) dataCard << "\n";
    return;
}