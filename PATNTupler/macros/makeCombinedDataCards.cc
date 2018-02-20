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
    CommonSystematic(const std::string&, const std::string&, const std::vector<std::string>&);    
    std::string GetSystematicName() const;
    std::vector<std::string> GetSystematicProcesses() const;
    std::string GetSystematicValue(const std::string&, const unsigned int&, std::map<std::string,TH1D*>&);
private:
    std::string systematicName;
    std::string systematicValue;
    std::string systematicHistoTag;
    std::vector<std::string> systematicProcesses; 
};

std::vector<double> GetEventWeightVec_S(const std::string&, std::map<std::string,TH1D*>&, const unsigned int&);
std::vector<double> GetEventWeightVec_UnD(const std::string&, std::map<std::string,TH1D*>&, const unsigned int&);
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
    const std::string outputDirGeneral = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/plots_2018_01_08/combined/testing1001/"; // where we are going to save the output cards (should include the samples name, and any important features)
  

    // TWO: physics info - to match the histograms that you use
    const unsigned int numberOfBins = 30;
    const unsigned int numberOfHtDivisions = 3;


    // THREE: Samples To Use (different project for each signal sample)
    const std::string dataSample = "data";
    std::vector<std::string> signalVec = {"mH70_mSusy2400"}; // the different signal samples you wish to use
    const std::vector<std::string> mcbkVec = {"TTJets", "ZJets", "WJets"}; // the mc background samples
    const std::string qcdName = "QCD"; // this is just a label as QCD contribution is driven during the fit


    // FOUR: data card layout info
    const unsigned int firstColSize = 35;
    const unsigned int otherColSize = 20;


    // FIVE: common systematics (statistical error systematics automatically taken care of)
    // NB1 - make the sure the sample names match to the above
    // NB2 - "SIGNAL" refers all signal samples and std::string qcdName is used for qcd
    std::vector<CommonSystematic> CommonSystematicVec;
    CommonSystematicVec.push_back( CommonSystematic("luminosity lnN", 1.025, {"SIGNAL", "TTJets", "ZJets", "WJets"}) );
    CommonSystematicVec.push_back( CommonSystematic("XS_Signal lnN", 1.050, {"SIGNAL"}) );
    CommonSystematicVec.push_back( CommonSystematic("XS_TTJets lnN", 1.5, {"TTJets"}) );
    CommonSystematicVec.push_back( CommonSystematic("XS_ZJets lnN", 1.4, {"ZJets"}) );
    CommonSystematicVec.push_back( CommonSystematic("XS_WJets lnN", 1.3, {"WJets"}) );
    CommonSystematicVec.push_back( CommonSystematic("jecUnc lnN", "jecUnc", {"SIGNAL", "TTJets", "ZJets", "WJets"}) );
    CommonSystematicVec.push_back( CommonSystematic("dbtLoose lnN", "dbtLoose", {"SIGNAL", "TTJets"}) );


    // SIX: are we blinded ? if true, uses Ai * data_obs_UnD as a dummy for data_obs_S
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
    std::vector<std::vector<double>> signalWeightVec_S;
    std::vector<std::vector<double>> signalWeightVec_UnD;

    std::vector<std::vector<double>> mcbkWeightVec_S;
    std::vector<std::vector<double>> mcbkWeightVec_UnD;
    std::vector<std::vector<int>>  mcbkStatErrorLogicVec;

    for (auto signal : signalVec){
        std::vector<double> eventWeightVec_S = GetEventWeightVec_S(signal, hOriginal_, numberOfBins);
        signalWeightVec_S.push_back(eventWeightVec_S);
        std::vector<double> eventWeightVec_UnD = GetEventWeightVec_UnD(signal, hOriginal_, numberOfBins);
        signalWeightVec_UnD.push_back(eventWeightVec_UnD);
    }

    for (auto mcbk : mcbkVec){
        std::vector<double> eventWeightVec_S = GetEventWeightVec_S(mcbk, hOriginal_, numberOfBins);
        mcbkWeightVec_S.push_back(eventWeightVec_S);
        std::vector<double> eventWeightVec_UnD = GetEventWeightVec_UnD(mcbk, hOriginal_, numberOfBins);
        mcbkWeightVec_UnD.push_back(eventWeightVec_UnD);
        mcbkStatErrorLogicVec.push_back( GetStatErrorLogic(mcbk, hOriginal_, numberOfBins, numberOfHtDivisions) );
    }

    // loop through the different signal sample references
    for (size_t iSig = 0; iSig < signalVec.size(); ++iSig){
        const std::string signal = signalVec[iSig];

        const std::string outputDir = outputDirGeneral + "/" + signal + "/";
        const std::string dirExistCommand = "test -e " + outputDir;
        const std::string makeDirCommand = "mkdir -p " + outputDir;
        if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
        std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/makeCombinedDataCards.cc %s/%s__makeCombinedDataCards.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

        std::cout << "SIGNAL SAMPLE = " << signal << ": " << outputDir << std::endl;

        for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

            unsigned int data_obs_S = hOriginal_[Form("S_tag_%s_NOSYS", dataSample.c_str())]->GetBinContent(iBin);
            if (areWeBlinded) data_obs_S = ceil( QcdSidebandCorr::GetCorr(iBin) * hOriginal_[Form("UnD_tag_%s_NOSYS", dataSample.c_str())]->GetBinContent(iBin) ); // use to get a non zero and roughly realistic value whilst we are blinded
            const unsigned int data_obs_UnD = hOriginal_[Form("UnD_tag_%s_NOSYS", dataSample.c_str())]->GetBinContent(iBin);
            const double rate_signal_S = hOriginal_[Form("S_tag_%s_NOSYS", signal.c_str())]->GetBinContent(iBin);
            const double rate_signal_UnD = hOriginal_[Form("UnD_tag_%s_NOSYS", signal.c_str())]->GetBinContent(iBin);;
            const std::string data_obs_S_str = std::to_string(data_obs_S);
            const std::string data_obs_UnD_str = std::to_string(data_obs_UnD);
            const std::string rate_signal_S_str = std::to_string(rate_signal_S);
            const std::string rate_signal_UnD_str = std::to_string(rate_signal_UnD);
            std::vector<double> rate_mcbkVec_S;
            std::vector<double> rate_mcbkVec_UnD;
            std::vector<std::string> rate_mcbkVec_S_str;
            std::vector<std::string> rate_mcbkVec_UnD_str;
            for (auto mcbk : mcbkVec){
                double rate_S = hOriginal_[Form("S_tag_%s_NOSYS", mcbk.c_str())]->GetBinContent(iBin);               
                double rate_UnD = hOriginal_[Form("UnD_tag_%s_NOSYS", mcbk.c_str())]->GetBinContent(iBin);
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
            WriteBlock(data_obs_S_str, otherColSize, dataCard);
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

                    std::string histoPreamble = "";
                    if (c == 0) histoPreamble = "S_tag_";
                    else histoPreamble = "UnD_tag_";
                    const std::vector<std::string> systematicProcesses = CommonSystematic.GetSystematicProcesses();

                    // signal
                    if (std::find(systematicProcesses.begin(), systematicProcesses.end(), "SIGNAL") != systematicProcesses.end()) {
                        const std::string fullHistogramName = histoPreamble + signal;
                        const std::string systematicValue = CommonSystematic.GetSystematicValue(fullHistogramName.c_str(), iBin, hOriginal_);
                        WriteBlock(systematicValue, otherColSize, dataCard);
                    } else {
                        WriteBlock("-", otherColSize, dataCard);
                    }

                    // monte carlo background
                    for (auto mcbk : mcbkVec){
                        if (std::find(systematicProcesses.begin(), systematicProcesses.end(), mcbk) != systematicProcesses.end()) {
                            const std::string fullHistogramName = histoPreamble + mcbk;
                            const std::string systematicValue = CommonSystematic.GetSystematicValue(fullHistogramName.c_str(), iBin, hOriginal_);
                            WriteBlock(systematicValue, otherColSize, dataCard);
                        } else {
                            WriteBlock("-", otherColSize, dataCard);
                        }                   
                    }

                    // QCD
                    if (std::find(systematicProcesses.begin(), systematicProcesses.end(), qcdName) != systematicProcesses.end()) {
                        const std::string fullHistogramName = histoPreamble + qcdName;
                        const std::string systematicValue = CommonSystematic.GetSystematicValue(fullHistogramName.c_str(), iBin, hOriginal_);
                        WriteBlock(systematicValue, otherColSize, dataCard);
                    } else {
                        WriteBlock("-", otherColSize, dataCard);
                    }
                } // closes the two counts
                dataCard << "\n";
            }

            dataCard << "\n# unique systematics\n";
            if (rate_signal_S > 0){
                const unsigned int iVec = iBin - 1;
                double signalWeight_S = signalWeightVec_S[iSig][iVec];
                const int rawCount = round(rate_signal_S / signalWeight_S);
                const std::string statSysName = "ch" + std::to_string(iBin) + "_SIG_S_stats gmN " + std::to_string(rawCount);
                const std::string signalWeightStr = std::to_string(signalWeight_S);
                WriteBlock(statSysName, firstColSize, dataCard);
                WriteBlock(signalWeightStr, otherColSize, dataCard);
                for (unsigned int c = 0; c < 2 * mcbkVec.size() + 3; ++c) WriteBlock("-", otherColSize, dataCard);
                dataCard << "\n";
            }

            for (size_t iMC = 0; iMC < mcbkVec.size(); ++iMC){
                const unsigned int iVec = iBin - 1;
                if (mcbkStatErrorLogicVec[iMC][iVec] == 1){
                    
                    double mcbkWeight_S = mcbkWeightVec_S[iMC][iVec];
                    
                    if (mcbkWeight_S == 0){
                        std::vector<double> nonZeroWeightVec;
                        for (auto weight : mcbkWeightVec_S[iMC])
                            if (weight > 0) nonZeroWeightVec.push_back(weight);
                        for (auto weight : mcbkWeightVec_UnD[iMC])
                            if (weight > 0) nonZeroWeightVec.push_back(weight);
                        for (auto nonZeroWeight : nonZeroWeightVec)
                            mcbkWeight_S += nonZeroWeight / nonZeroWeightVec.size();
                    }

                    const int rawCount = round(rate_mcbkVec_S[iMC] / mcbkWeight_S);
                    const std::string statSysName = "ch" + std::to_string(iBin) + "_" + mcbkVec[iMC] + "_S_stats gmN " + std::to_string(rawCount);
                    const std::string mcbkWeightStr = std::to_string(mcbkWeight_S);
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

            if (rate_signal_UnD > 0){
                const unsigned int iVec = iBin - 1;
                double signalWeight_UnD = signalWeightVec_UnD[iSig][iVec];
                const int rawCount = round(rate_signal_UnD / signalWeight_UnD);
                const std::string statSysName = "ch" + std::to_string(iBin) + "_SIG_UnD_stats gmN " + std::to_string(rawCount);
                const std::string signalWeightStr = std::to_string(signalWeight_UnD);
                WriteBlock(statSysName, firstColSize, dataCard);
                for (unsigned int c = 0; c < mcbkVec.size() + 2; ++c) WriteBlock("-", otherColSize, dataCard);
                WriteBlock(signalWeightStr, otherColSize, dataCard);
                for (unsigned int c = 0; c < mcbkVec.size() + 1; ++c) WriteBlock("-", otherColSize, dataCard);
                dataCard << "\n";
            }

            for (size_t iMC = 0; iMC < mcbkVec.size(); ++iMC){
                const unsigned int iVec = iBin - 1;
                if (mcbkStatErrorLogicVec[iMC][iVec] == 1){
                    
                    double mcbkWeight_UnD = mcbkWeightVec_UnD[iMC][iVec];
                    
                    if (mcbkWeight_UnD == 0){
                        std::vector<double> nonZeroWeightVec;
                        for (auto weight : mcbkWeightVec_S[iMC])
                            if (weight > 0) nonZeroWeightVec.push_back(weight);
                        for (auto weight : mcbkWeightVec_UnD[iMC])
                            if (weight > 0) nonZeroWeightVec.push_back(weight);
                        for (auto nonZeroWeight : nonZeroWeightVec)
                            mcbkWeight_UnD += nonZeroWeight / nonZeroWeightVec.size();
                    }
                    
                    const int rawCount = round(rate_mcbkVec_UnD[iMC] / mcbkWeight_UnD);
                    const std::string statSysName = "ch" + std::to_string(iBin) + "_" + mcbkVec[iMC] + "_UnD_stats gmN " + std::to_string(rawCount);
                    const std::string mcbkWeightStr = std::to_string(mcbkWeight_UnD);
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
            double corrRatioError = QcdSidebandCorr::GetCorrErr(iBin);
            WriteBlock(Form("ch%d_R", iBin), otherColSize, dataCard);
            dataCard << "param " << std::to_string(corrRatio) << " " << std::to_string(corrRatioError) << "\n";
            WriteBlock(Form("ch%d_alpha", iBin), otherColSize, dataCard);
            dataCard << "rateParam mass_UnD " << qcdName << " ";
            double qcdInitialEstimate = data_obs_UnD;
            for (auto rate_mcbk_UnD : rate_mcbkVec_UnD) qcdInitialEstimate = qcdInitialEstimate - rate_mcbk_UnD;
            if (qcdInitialEstimate < 0){
                qcdInitialEstimate = 0.0;
                std::cout << "WARNING: qcd estimate of zero!" << std::endl;
            }
            double qcdUpperLimit = data_obs_UnD + 3.0 * sqrt(data_obs_UnD + 1);
            dataCard << std::to_string(qcdInitialEstimate) << " " << "[0," << std::to_string(qcdUpperLimit) << "]\n";
            WriteBlock(Form("ch%d_beta", iBin), otherColSize, dataCard);
            dataCard << "rateParam mass_S " << qcdName << " (@0*@1) ch" << iBin << "_R,ch" << iBin << "_alpha\n";

            dataCard.close();

        } // closes loop through the histogram bins

    // combine the data cards into one
    std::system( Form("source %scomboCommand.sh", outputDir.c_str()) );
    std::system( Form("text2workspace.py %sallbins.txt", outputDir.c_str()) );
    std::cout << std::endl;
    } // closes loop through the different signal samples
    return 0; 
}


void GetHistograms(std::map<std::string,TH1D*>& h_)
{
    // histos locations
    std::string preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2018_01_11_CMSSW_8_0_29_dbtV4/TESTING_SYS/";
    
    std::string postamble = "MassCutsV05_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi36.root";
    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("data"); // comment out when working on MC
    // histoNameVec.push_back("QCD");
    histoNameVec.push_back("TTJets");
    histoNameVec.push_back("ZJets");
    histoNameVec.push_back("WJets");
    // histoNameVec.push_back("mH30_mSusy800");
    // histoNameVec.push_back("mH50_mSusy800");
    // histoNameVec.push_back("mH70_mSusy800");
    // histoNameVec.push_back("mH90_mSusy800");
    // histoNameVec.push_back("mH125_mSusy800");
    // histoNameVec.push_back("mH30_mSusy1200");
    // histoNameVec.push_back("mH50_mSusy1200");
    // histoNameVec.push_back("mH70_mSusy1200");
    // histoNameVec.push_back("mH90_mSusy1200");
    // histoNameVec.push_back("mH125_mSusy1200");
    // histoNameVec.push_back("mH30_mSusy1600");
    // histoNameVec.push_back("mH50_mSusy1600");
    // histoNameVec.push_back("mH70_mSusy1600");
    // histoNameVec.push_back("mH90_mSusy1600");
    // histoNameVec.push_back("mH125_mSusy1600");
    // histoNameVec.push_back("mH30_mSusy2000");
    // histoNameVec.push_back("mH50_mSusy2000");
    // histoNameVec.push_back("mH70_mSusy2000");
    // histoNameVec.push_back("mH90_mSusy2000");
    // histoNameVec.push_back("mH125_mSusy2000");
    // histoNameVec.push_back("mH30_mSusy2200");
    // histoNameVec.push_back("mH50_mSusy2200");
    // histoNameVec.push_back("mH70_mSusy2200");
    // histoNameVec.push_back("mH90_mSusy2200");
    // histoNameVec.push_back("mH125_mSusy2200");
    // histoNameVec.push_back("mH30_mSusy2400");
    // histoNameVec.push_back("mH50_mSusy2400");
    histoNameVec.push_back("mH70_mSusy2400");
    // histoNameVec.push_back("mH90_mSusy2400");
    // histoNameVec.push_back("mH125_mSusy2400");
    // histoNameVec.push_back("mH30_mSusy2600");
    // histoNameVec.push_back("mH50_mSusy2600");
    // histoNameVec.push_back("mH70_mSusy2600");
    // histoNameVec.push_back("mH90_mSusy2600");
    // histoNameVec.push_back("mH125_mSusy2600");

    std::vector<std::string> nonTrivialSysVec;
    nonTrivialSysVec.push_back("NOSYS");
    nonTrivialSysVec.push_back("dbtLooseDown");
    nonTrivialSysVec.push_back("dbtLooseUp");
    nonTrivialSysVec.push_back("jecUncUp");
    nonTrivialSysVec.push_back("jecUncDown");

    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        std::string histoToUse = histoNameVec[iH];
        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoToUse.c_str(), postamble.c_str()));

        for (auto nonTrivialSys : nonTrivialSysVec){

            // explanation of terminology
            // 1. S, U, D --> refers to mass space. pred is the prediction of S. UnD is the sum U+D.
            // 2. tag, anti, control --> refers to 2*DBT space
            // 3. sample name on the end
            if ( (TH1D*)f->Get(Form("S_dbtDiagUpLoose_%s", nonTrivialSys.c_str())) == NULL ) continue;
            h_[Form("S_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())] = (TH1D*)f->Get(Form("S_dbtDiagUpLoose_%s", nonTrivialSys.c_str()));
            h_[Form("UnD_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())] = (TH1D*)f->Get(Form("U_dbtDiagUpLoose_%s", nonTrivialSys.c_str()));
            h_[Form("UnD_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())]->Add((TH1D*)f->Get(Form("D_dbtDiagUpLoose_%s", nonTrivialSys.c_str())));

        } // closes loop through nonTrivialSysVec
    } // closes loop through histoNameVec

} // closes function GetHistograms


CommonSystematic::CommonSystematic(const std::string& systematicNameDummy, const double& systematicValueNumber, const std::vector<std::string>& systematicProcessesDummy) :
systematicName(systematicNameDummy),
systematicValue(std::to_string(systematicValueNumber)),
systematicHistoTag(""),
systematicProcesses(systematicProcessesDummy)
{};

CommonSystematic::CommonSystematic(const std::string& systematicNameDummy, const std::string& systematicHistoTagDummy, const std::vector<std::string>& systematicProcessesDummy) :
systematicName(systematicNameDummy),
systematicValue(""),
systematicHistoTag(systematicHistoTagDummy),
systematicProcesses(systematicProcessesDummy)
{};

std::string CommonSystematic::GetSystematicName() const {return systematicName;}
std::vector<std::string> CommonSystematic::GetSystematicProcesses() const {return systematicProcesses;}

std::string CommonSystematic::GetSystematicValue(const std::string& fullHistogramName, const unsigned int& iBin, std::map<std::string,TH1D*>& hOriginal_) 
{
    if (systematicHistoTag.empty() == true) return systematicValue;
    
    else{
        double count_nominal = hOriginal_[Form("%s_NOSYS", fullHistogramName.c_str())]->GetBinContent(iBin); 
        if (count_nominal == 0) return "-";

        double count_sysUp = hOriginal_[Form("%s_%sUp", fullHistogramName.c_str(), systematicHistoTag.c_str())]->GetBinContent(iBin);
        double count_sysDown = hOriginal_[Form("%s_%sDown", fullHistogramName.c_str(), systematicHistoTag.c_str())]->GetBinContent(iBin);

        double k_down = count_sysDown / count_nominal;
        double k_up = count_sysUp / count_nominal;
        if (k_down == 0) k_down = 0.01;
        if (k_up == 0) k_up = 0.01;

        std::string systenaticValueSpec = std::to_string(k_down) + "/" + std::to_string(k_up);
        return systenaticValueSpec;
    }
}


std::vector<double> GetEventWeightVec_S(const std::string& histogramName, std::map<std::string,TH1D*>& hOriginal_, const unsigned int& numberOfBins){

    std::vector<double> eventWeightVec;

    for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

        double binContent_S = hOriginal_[Form("S_tag_%s_NOSYS", histogramName.c_str())]->GetBinContent(iBin);
        double binError_S = hOriginal_[Form("S_tag_%s_NOSYS", histogramName.c_str())]->GetBinError(iBin);
        if (binContent_S != 0) eventWeightVec.push_back(binError_S * binError_S / binContent_S);
        else eventWeightVec.push_back(0.0);
    }
    return eventWeightVec;
}


std::vector<double> GetEventWeightVec_UnD(const std::string& histogramName, std::map<std::string,TH1D*>& hOriginal_, const unsigned int& numberOfBins){

    std::vector<double> eventWeightVec;

    for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

        double binContent_S = hOriginal_[Form("UnD_tag_%s_NOSYS", histogramName.c_str())]->GetBinContent(iBin);
        double binError_S = hOriginal_[Form("UnD_tag_%s_NOSYS", histogramName.c_str())]->GetBinError(iBin);
        if (binContent_S != 0) eventWeightVec.push_back(binError_S * binError_S / binContent_S);
        else eventWeightVec.push_back(0.0);
    }
    return eventWeightVec;
}


std::vector<int> GetStatErrorLogic(const std::string& histogramName, std::map<std::string,TH1D*>& hOriginal_, const unsigned int& numberOfBins, const unsigned int& numberOfHtDivisions){
/*
returns a vector of integers - one for each sample
there is an entry corresponding to each search region bin
returns '1' if there is an entry in S_tag or UnD_tag for a given ht region 
returns '0' if not
use this object to see whether we should give a bin with zero entries an error
*/
    std::vector<int> StatErrorLogic(numberOfBins,0);
    const unsigned int numberOfBinsPerHtDivision = numberOfBins / numberOfHtDivisions;

    for (unsigned int iVec = 0; iVec < numberOfBins; ++iVec){
    
        const unsigned int iBin = iVec + 1;
        const unsigned int htDivisionIndex = floor(iVec / numberOfBinsPerHtDivision);

        double binContent_S = hOriginal_[Form("S_tag_%s_NOSYS", histogramName.c_str())]->GetBinContent(iBin);
        double binContent_UnD = hOriginal_[Form("UnD_tag_%s_NOSYS", histogramName.c_str())]->GetBinContent(iBin);

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