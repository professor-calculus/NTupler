//STL HEADERS
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <map>
#include <fstream>
#include <algorithm> 
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


void GetHistograms(std::map<std::string,TH1D*>&, const unsigned int&); // NEED TO CHANGE THE FILE PATH IN THIS FUNCTION WHEN USING NEW HISTOGRAMS
std::vector<double> GetQcdUnDLowerBoundInHtDivison(TH1D*, const unsigned int&);

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
void WriteBlock(const std::string&, const unsigned int&, std::ofstream&, const bool = false);


int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info (signal specific directories beneath this)
    const std::string outputDirGeneral = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/combinedDataCards_2019_01_01/withGluino/guess_S/";
  


    // TWO: physics info - to match the histograms that you use
    const unsigned int numberOfBins = 30;
    const unsigned int numberOfHtDivisions = 3;



    // THREE: Samples To Use (different project for each signal sample)
    const std::string dataSample = "data";
    std::vector<std::string> signalVec = { // the different signal samples you wish to use
                                            // "mH30_mSusy800",                                   "mH50_mSusy800",  "mH70_mSusy800",  "mH90_mSusy800",  "mH110_mSusy800",  "mH125_mSusy800",
                                            "mH30_mSusy1200", "mH35_mSusy1200", "mH40_mSusy1200", "mH50_mSusy1200", "mH70_mSusy1200", "mH90_mSusy1200", "mH110_mSusy1200", "mH125_mSusy1200",
                                            "mH30_mSusy1600", "mH35_mSusy1600", "mH40_mSusy1600", "mH50_mSusy1600", "mH70_mSusy1600", "mH90_mSusy1600", "mH110_mSusy1600", "mH125_mSusy1600",
                                            "mH30_mSusy2000", "mH35_mSusy2000", "mH40_mSusy2000", "mH50_mSusy2000", "mH70_mSusy2000", "mH90_mSusy2000", "mH110_mSusy2000", "mH125_mSusy2000",
                                            "mH30_mSusy2200", "mH35_mSusy2200", "mH40_mSusy2200", "mH50_mSusy2200", "mH70_mSusy2200", "mH90_mSusy2200", "mH110_mSusy2200", "mH125_mSusy2200",
                                            "mH30_mSusy2400", "mH35_mSusy2400", "mH40_mSusy2400", "mH50_mSusy2400", "mH70_mSusy2400", "mH90_mSusy2400", "mH110_mSusy2400", "mH125_mSusy2400",
                                            "mH30_mSusy2600", "mH35_mSusy2600", "mH40_mSusy2600", "mH50_mSusy2600", "mH70_mSusy2600", "mH90_mSusy2600", "mH110_mSusy2600", "mH125_mSusy2600", 
                                            "mH30_mSusy2800", "mH35_mSusy2800", "mH40_mSusy2800", "mH50_mSusy2800", "mH70_mSusy2800", "mH90_mSusy2800", "mH110_mSusy2800", "mH125_mSusy2800", 
                                        };
    // std::vector<std::string> signalVec = { // the different signal samples you wish to use: SQUARK PRODUCTION ONLY
    //                                         // "mH30_mSquark800",                                       "mH50_mSquark800",  "mH70_mSquark800",  "mH90_mSquark800",  "mH110_mSquark800",  "mH125_mSquark800",
    //                                         "mH30_mSquark1200", "mH35_mSquark1200", "mH40_mSquark1200", "mH50_mSquark1200", "mH70_mSquark1200", "mH90_mSquark1200", "mH110_mSquark1200", "mH125_mSquark1200",
    //                                         "mH30_mSquark1600", "mH35_mSquark1600", "mH40_mSquark1600", "mH50_mSquark1600", "mH70_mSquark1600", "mH90_mSquark1600", "mH110_mSquark1600", "mH125_mSquark1600",
    //                                         "mH30_mSquark2000", "mH35_mSquark2000", "mH40_mSquark2000", "mH50_mSquark2000", "mH70_mSquark2000", "mH90_mSquark2000", "mH110_mSquark2000", "mH125_mSquark2000",
    //                                         "mH30_mSquark2200", "mH35_mSquark2200", "mH40_mSquark2200", "mH50_mSquark2200", "mH70_mSquark2200", "mH90_mSquark2200", "mH110_mSquark2200", "mH125_mSquark2200",
    //                                         "mH30_mSquark2400", "mH35_mSquark2400", "mH40_mSquark2400", "mH50_mSquark2400", "mH70_mSquark2400", "mH90_mSquark2400", "mH110_mSquark2400", "mH125_mSquark2400",
    //                                         "mH30_mSquark2600", "mH35_mSquark2600", "mH40_mSquark2600", "mH50_mSquark2600", "mH70_mSquark2600", "mH90_mSquark2600", "mH110_mSquark2600", "mH125_mSquark2600", 
    //                                         "mH30_mSquark2800", "mH35_mSquark2800", "mH40_mSquark2800", "mH50_mSquark2800", "mH70_mSquark2800", "mH90_mSquark2800", "mH110_mSquark2800", "mH125_mSquark2800", 
    //                                     };                                        
    std::map<unsigned int, std::vector<std::string>> mcbkVec;
    mcbkVec[2016] = {"TTJets", "ZJets", "WJets"}; // the 2016 MC background samples
    mcbkVec[2017] = {"TTJets0L", "TTJets1L", "TTJets2L", "ZJets", "WJets"}; // the 2017 MC background samples
    const std::string qcdName = "QCD"; // this is just a label as QCD contribution is driven during the fit



    // FOUR: data card layout info
    const unsigned int firstColSize = 35;
    const unsigned int otherColSize = 20;



    // FIVE: common systematics (statistical error systematics automatically taken care of)
    std::map<unsigned int, std::vector<CommonSystematic>> CommonSystematicVec;
    // NB1 - make the sure the sample names match to the above (for the given year)
    // NB2 - "SIGNAL" refers all signal samples
    // *** 2016 ***
    // CommonSystematicVec[2016].push_back( CommonSystematic("signalPdfBOTH lnN", 1.1, {"SIGNAL"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("isrReweightBOTH lnN", "isr", {"SIGNAL"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("luminosity2016 lnN", 1.025, {"SIGNAL", "TTJets", "ZJets", "WJets"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("XS_TTJets2016 lnN", 1.5, {"TTJets"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("XS_ZJets2016 lnN", 1.5, {"ZJets"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("XS_WJets2016 lnN", 1.5, {"WJets"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("jecAKXUncBOTH lnN", "jecAKXUnc", {"SIGNAL", "TTJets", "ZJets", "WJets"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("jerAKXUnc2016 lnN", "jerAKXUnc", {"SIGNAL", "TTJets", "ZJets", "WJets"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("jmsUncBOTH lnN", "jmsUnc", {"SIGNAL", "TTJets", "ZJets", "WJets"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("jmrUncBOTH lnN", "jmrUnc", {"SIGNAL", "TTJets", "ZJets", "WJets"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("SigDbtTag2016 lnN", "dbtTag", {"SIGNAL"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("TtDbtTag2016 lnN", "dbtTag", {"TTJets"}) );
    // *** 2017 ***
    // CommonSystematicVec[2017].push_back( CommonSystematic("signalPdfBOTH lnN", 1.1, {"SIGNAL"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("isrReweightBOTH lnN", "isr", {"SIGNAL"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("luminosity2017 lnN", 1.023, {"SIGNAL", "TTJets0L", "TTJets1L", "TTJets2L", "ZJets", "WJets"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("XS_TTJets0L2017 lnN", 1.5, {"TTJets0L"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("XS_TTJets1L2017 lnN", 1.5, {"TTJets1L"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("XS_TTJets2L2017 lnN", 1.5, {"TTJets2L"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("XS_ZJets2017 lnN", 1.5, {"ZJets"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("XS_WJets2017 lnN", 1.5, {"WJets"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("jecAKXUncBOTH lnN", "jecAKXUnc", {"SIGNAL", "TTJets0L", "TTJets1L", "TTJets2L", "ZJets", "WJets"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("jerAKXUnc2017 lnN", "jerAKXUnc", {"SIGNAL", "TTJets0L", "TTJets1L", "TTJets2L", "ZJets", "WJets"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("jmsUncBOTH lnN", "jmsUnc", {"SIGNAL", "TTJets0L", "TTJets1L", "TTJets2L", "ZJets", "WJets"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("jmrUncBOTH lnN", "jmrUnc", {"SIGNAL", "TTJets0L", "TTJets1L", "TTJets2L", "ZJets", "WJets"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("SigDbtTag2017 lnN", "dbtTag", {"SIGNAL"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("TtDbtTag2017 lnN", "dbtTag", {"TTJets0L", "TTJets1L", "TTJets2L"}) );



    // SIX: are we blinded ? if true, uses Fi * data_obs_UnD as a dummy for data_obs_S
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

    // loop through 2016 and 2017
    std::vector<unsigned int> yearOfRunVec = {2016, 2017};
    for (auto yearOfRun : yearOfRunVec){

        std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
        std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
        std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
        std::cout << "We are doing year: " << yearOfRun << std::endl;
        std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
        std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
        std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;

        std::map<std::string, TH1D*> hOriginal_;
        GetHistograms(hOriginal_, yearOfRun);
        std::vector<double> qcdUnDLowerBoundInHtDivison = GetQcdUnDLowerBoundInHtDivison(hOriginal_[Form("UnD_tag_%s_NOSYS", dataSample.c_str())], numberOfHtDivisions);

        // get event weightings
        std::vector<std::vector<double>> signalWeightVec_S;
        std::vector<std::vector<double>> signalWeightVec_UnD;

        std::vector<std::vector<double>> mcbkWeightVec_S;
        std::vector<std::vector<double>> mcbkWeightVec_UnD;

        for (auto signal : signalVec){
            std::vector<double> eventWeightVec_S = GetEventWeightVec_S(signal, hOriginal_, numberOfBins);
            signalWeightVec_S.push_back(eventWeightVec_S);
            std::vector<double> eventWeightVec_UnD = GetEventWeightVec_UnD(signal, hOriginal_, numberOfBins);
            signalWeightVec_UnD.push_back(eventWeightVec_UnD);
        }

        for (auto mcbk : mcbkVec[yearOfRun]){
            std::vector<double> eventWeightVec_S = GetEventWeightVec_S(mcbk, hOriginal_, numberOfBins);
            mcbkWeightVec_S.push_back(eventWeightVec_S);
            std::vector<double> eventWeightVec_UnD = GetEventWeightVec_UnD(mcbk, hOriginal_, numberOfBins);
            mcbkWeightVec_UnD.push_back(eventWeightVec_UnD);
        }

        // loop through the different signal sample references
        for (size_t iSig = 0; iSig < signalVec.size(); ++iSig){
            const std::string signal = signalVec[iSig];
            const std::string outputDir = outputDirGeneral + "/" + signal + "/";

            std::cout << signal << " ::: " << yearOfRun << std::endl;

            if (yearOfRun == 2016){
                const std::string dirExistCommand = "test -e " + outputDir;
                const std::string makeDirCommand = "mkdir -p " + outputDir;
                if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
                std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/makeCombineDataCardsBOTHYEARS.cc %s/%s__makeCombineDataCardsBOTHYEARS.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));
                
                // write the command for combining the cards for this signal sample
                std::ofstream comboCommand;
                comboCommand.open( Form("%scomboCommand.sh", outputDir.c_str()) );
                std::string comboCommandStr = "combineCards.py ";
                for (unsigned int i = 1; i < numberOfBins + 1; ++i) comboCommandStr += outputDir + "bin" + std::to_string(i) + "_2016.txt ";
                for (unsigned int i = 1; i < numberOfBins + 1; ++i) comboCommandStr += outputDir + "bin" + std::to_string(i+numberOfBins) + "_2017.txt ";
                comboCommandStr += "> " + outputDir + "allbins.txt\n";
                comboCommand << comboCommandStr;
                comboCommand.close();
            }

            // loop through each search region bin
            for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

                unsigned int data_obs_S = hOriginal_[Form("S_tag_%s_NOSYS", dataSample.c_str())]->GetBinContent(iBin);
                // WHEN BLINDED: USE TO GET AN ESTIMATE OF TRUE YIELD 
                if (areWeBlinded){
                    int estimate = round( QcdSidebandCorr::GetCorr(iBin, yearOfRun) * hOriginal_[Form("UnD_tag_%s_NOSYS", dataSample.c_str())]->GetBinContent(iBin) );
                    if (yearOfRun==2016 && iBin%2==0) estimate = estimate - 1;
                    if (yearOfRun==2017 && iBin%2!=0) estimate = estimate + 1;
                    if (estimate < 0) estimate = 0;
                    data_obs_S = estimate;
                }
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
                for (auto mcbk : mcbkVec[yearOfRun]){
                    double rate_S = hOriginal_[Form("S_tag_%s_NOSYS", mcbk.c_str())]->GetBinContent(iBin);               
                    double rate_UnD = hOriginal_[Form("UnD_tag_%s_NOSYS", mcbk.c_str())]->GetBinContent(iBin);
                    rate_mcbkVec_S.push_back(rate_S);
                    rate_mcbkVec_UnD.push_back(rate_UnD);
                    rate_mcbkVec_S_str.push_back( std::to_string(rate_S) );
                    rate_mcbkVec_UnD_str.push_back( std::to_string(rate_UnD) );
                }

                // write the data card
                std::ofstream dataCard;
                unsigned int binLabel = iBin;
                if (yearOfRun == 2017) binLabel += numberOfBins;
                dataCard.open( Form("%sbin%d_%d.txt", outputDir.c_str(),binLabel,yearOfRun) );
                dataCard << "imax 2\n";
                dataCard << "jmax " << mcbkVec[yearOfRun].size() + 1 << "\n";
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
                for (unsigned int c = 0; c < mcbkVec[yearOfRun].size() + 2; ++c) WriteBlock("mass_S", otherColSize, dataCard);
                for (unsigned int c = 0; c < mcbkVec[yearOfRun].size() + 2; ++c) WriteBlock("mass_UnD", otherColSize, dataCard);
                dataCard << "\n";
                WriteBlock("process", firstColSize, dataCard);
                WriteBlock(signal, otherColSize, dataCard);
                for (auto mcbk : mcbkVec[yearOfRun]) WriteBlock(mcbk, otherColSize, dataCard);
                WriteBlock(qcdName, otherColSize, dataCard);
                WriteBlock(signal, otherColSize, dataCard);
                for (auto mcbk : mcbkVec[yearOfRun]) WriteBlock(mcbk, otherColSize, dataCard);
                WriteBlock(qcdName, otherColSize, dataCard, true);
                WriteBlock("process", firstColSize, dataCard);
                for (unsigned int i = 0; i < mcbkVec[yearOfRun].size() + 2; ++i) WriteBlock(std::to_string(i), otherColSize, dataCard); 
                for (unsigned int i = 0; i < mcbkVec[yearOfRun].size() + 2; ++i) WriteBlock(std::to_string(i), otherColSize, dataCard);
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
                for (auto CommonSystematic : CommonSystematicVec[yearOfRun]){
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
                        for (auto mcbk : mcbkVec[yearOfRun]){
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
                // if (rate_signal_S > 0){
                //     const unsigned int iVec = iBin - 1;
                //     double signalWeight_S = signalWeightVec_S[iSig][iVec];
                //     const int rawCount = round(rate_signal_S / signalWeight_S);
                //     const std::string statSysName = "ch" + std::to_string(binLabel) + "_SIG_S_stats gmN " + std::to_string(rawCount);
                //     const std::string signalWeightStr = std::to_string(signalWeight_S);
                //     WriteBlock(statSysName, firstColSize, dataCard);
                //     WriteBlock(signalWeightStr, otherColSize, dataCard);
                //     for (unsigned int c = 0; c < 2 * mcbkVec[yearOfRun].size() + 3; ++c) WriteBlock("-", otherColSize, dataCard);
                //     dataCard << "\n";
                // }

                for (size_t iMC = 0; iMC < mcbkVec[yearOfRun].size(); ++iMC){
                    
                    if (rate_mcbkVec_S[iMC] > 0){
                    
                        const unsigned int iVec = iBin - 1;
                        const double mcbkWeight_S = mcbkWeightVec_S[iMC][iVec];
                        
                        const int rawCount = round(rate_mcbkVec_S[iMC] / mcbkWeight_S);
                        const std::string statSysName = "ch" + std::to_string(binLabel) + "_" + mcbkVec[yearOfRun][iMC] + "_S_stats gmN " + std::to_string(rawCount);
                        const std::string mcbkWeightStr = std::to_string(mcbkWeight_S);
                        WriteBlock(statSysName, firstColSize, dataCard);
                        WriteBlock("-", otherColSize, dataCard);

                        for (unsigned int c = 0; c < mcbkVec[yearOfRun].size(); ++c){
                            if (iMC == c) WriteBlock(mcbkWeightStr, otherColSize, dataCard);
                            else WriteBlock("-", otherColSize, dataCard);
                        }
                        for (unsigned int c = 0; c < mcbkVec[yearOfRun].size() + 3; ++c) WriteBlock("-", otherColSize, dataCard);
                        dataCard << "\n";
                    }
                }

                // if (rate_signal_UnD > 0){
                //     const unsigned int iVec = iBin - 1;
                //     double signalWeight_UnD = signalWeightVec_UnD[iSig][iVec];
                //     const int rawCount = round(rate_signal_UnD / signalWeight_UnD);
                //     const std::string statSysName = "ch" + std::to_string(binLabel) + "_SIG_UnD_stats gmN " + std::to_string(rawCount);
                //     const std::string signalWeightStr = std::to_string(signalWeight_UnD);
                //     WriteBlock(statSysName, firstColSize, dataCard);
                //     for (unsigned int c = 0; c < mcbkVec[yearOfRun].size() + 2; ++c) WriteBlock("-", otherColSize, dataCard);
                //     WriteBlock(signalWeightStr, otherColSize, dataCard);
                //     for (unsigned int c = 0; c < mcbkVec[yearOfRun].size() + 1; ++c) WriteBlock("-", otherColSize, dataCard);
                //     dataCard << "\n";
                // }

                for (size_t iMC = 0; iMC < mcbkVec[yearOfRun].size(); ++iMC){
                    
                    if (rate_mcbkVec_UnD[iMC] > 0){

                        const unsigned int iVec = iBin - 1;                 
                        const double mcbkWeight_UnD = mcbkWeightVec_UnD[iMC][iVec];

                        const int rawCount = round(rate_mcbkVec_UnD[iMC] / mcbkWeight_UnD);
                        const std::string statSysName = "ch" + std::to_string(binLabel) + "_" + mcbkVec[yearOfRun][iMC] + "_UnD_stats gmN " + std::to_string(rawCount);
                        const std::string mcbkWeightStr = std::to_string(mcbkWeight_UnD);
                        WriteBlock(statSysName, firstColSize, dataCard);
                        WriteBlock("-", otherColSize, dataCard);

                        for (unsigned int c = 0; c < mcbkVec[yearOfRun].size() + 2; ++c) WriteBlock("-", otherColSize, dataCard);
                        for (unsigned int c = 0; c < mcbkVec[yearOfRun].size(); ++c){
                            if (iMC == c) WriteBlock(mcbkWeightStr, otherColSize, dataCard);
                            else WriteBlock("-", otherColSize, dataCard);
                        }
                        WriteBlock("-", otherColSize, dataCard);
                        dataCard << "\n";
                    }
                }
                
                // unsigned int iHtIndex = floor( (iBin - 1) / (numberOfBins / numberOfHtDivisions) ); 
                // double qcdUnDLowerBound = qcdUnDLowerBoundInHtDivison[iHtIndex]; // COMPLICATED LOWER BOUND
                double qcdUnDLowerBound = 0.25; // SIMPLE LOWER BOUND
                dataCard << "\n# estimate QCD\n";
                double corrRatio = QcdSidebandCorr::GetCorr(iBin, yearOfRun);
                double corrRatioError = QcdSidebandCorr::GetCorrErr(iBin, yearOfRun);
                WriteBlock(Form("ch%d_F", binLabel), otherColSize, dataCard);
                dataCard << "param " << std::to_string(corrRatio) << " " << std::to_string(corrRatioError) << "\n";
                WriteBlock(Form("ch%d_abcdErr", binLabel), otherColSize, dataCard);
                dataCard << "param 1.000000 0.100000\n";
                WriteBlock(Form("ch%d_alpha", binLabel), otherColSize, dataCard);
                dataCard << "rateParam mass_UnD " << qcdName << " ";
                double qcdInitialEstimate = data_obs_UnD;
                for (auto rate_mcbk_UnD : rate_mcbkVec_UnD) qcdInitialEstimate = qcdInitialEstimate - rate_mcbk_UnD;
                if (qcdInitialEstimate < qcdUnDLowerBound){
                    qcdInitialEstimate = qcdUnDLowerBound;
                    std::cout << "WARNING: initial qcd estimate less than the lower bound! (in U+D search region bin: " << iBin << ")" << std::endl;
                }
                double qcdUpperLimit = 2.0 * data_obs_UnD + 30.0;
                dataCard << std::to_string(qcdInitialEstimate) << " " << "[" << std::to_string(qcdUnDLowerBound) << "," << std::to_string(qcdUpperLimit) << "]\n";            
                WriteBlock(Form("ch%d_beta", binLabel), otherColSize, dataCard);
                dataCard << "rateParam mass_S " << qcdName << " (@0*@1*@2) ch" << binLabel << "_F,ch" << binLabel << "_abcdErr,ch" << binLabel << "_alpha\n";

                dataCard.close();

            } // closes loop through the search region bins
            std::cout << std::endl;
        } // closes loop through the different signal samples
    } // closes loop through yearOfRunVec elements

    std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
    std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
    std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
    std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
    std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
    std::cout << "*,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. " << std::endl;
    std::cout << "We are now combining cards...\n" << std::endl;

    // combine the datacards into one for each signal sample
    for (auto signal : signalVec){
        const std::string outputDir = outputDirGeneral + "/" + signal + "/";
        std::cout << "SIGNAL SAMPLE = " << signal << ": " << outputDir << std::endl;
        std::system( Form("source %scomboCommand.sh", outputDir.c_str()) );
        std::system( Form("sed -i \"s:number of nuisance parameters:number of nuisance parameters\\n---------------\\nshapes * * FAKE\\n---------------:\" %sallbins.txt", outputDir.c_str()) );
        // std::system( Form("text2workspace.py %sallbins.txt", outputDir.c_str()) );
        std::cout << std::endl;
    }

    return 0; 
}


void GetHistograms(std::map<std::string,TH1D*>& h_, const unsigned int& yearOfRun)
{
    // histos locations
    std::string preamble = "";
    std::string postamble = "";

    if (yearOfRun == 2016){
        preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2019_01_01/MassCutsV09/run2016/";
        postamble = "MassCutsV09_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi36.root";
    }
    else if (yearOfRun == 2017){
        preamble = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/histos_2019_01_01/MassCutsV09/run2017/";
        postamble = "MassCutsV09_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi42.root";
    }
    else{
        std::cout << "You have not given GetHistograms a valid year" << std::endl;
        return;
    }

    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("data");
    histoNameVec.push_back("TTJets");
    histoNameVec.push_back("TTJets0L");
    histoNameVec.push_back("TTJets1L");
    histoNameVec.push_back("TTJets2L");
    histoNameVec.push_back("ZJets");
    histoNameVec.push_back("WJets");
    
    histoNameVec.push_back("mH30_mSusy800");
    histoNameVec.push_back("mH50_mSusy800");
    histoNameVec.push_back("mH70_mSusy800");
    histoNameVec.push_back("mH90_mSusy800");
    histoNameVec.push_back("mH110_mSusy800");
    histoNameVec.push_back("mH125_mSusy800");
    histoNameVec.push_back("mH30_mSusy1200");
    histoNameVec.push_back("mH35_mSusy1200");
    histoNameVec.push_back("mH40_mSusy1200");
    histoNameVec.push_back("mH50_mSusy1200");
    histoNameVec.push_back("mH70_mSusy1200");
    histoNameVec.push_back("mH90_mSusy1200");
    histoNameVec.push_back("mH110_mSusy1200");
    histoNameVec.push_back("mH125_mSusy1200");
    histoNameVec.push_back("mH30_mSusy1600");
    histoNameVec.push_back("mH35_mSusy1600");
    histoNameVec.push_back("mH40_mSusy1600");
    histoNameVec.push_back("mH50_mSusy1600");
    histoNameVec.push_back("mH70_mSusy1600");
    histoNameVec.push_back("mH90_mSusy1600");
    histoNameVec.push_back("mH110_mSusy1600");
    histoNameVec.push_back("mH125_mSusy1600");
    histoNameVec.push_back("mH30_mSusy2000");
    histoNameVec.push_back("mH35_mSusy2000");
    histoNameVec.push_back("mH40_mSusy2000");
    histoNameVec.push_back("mH50_mSusy2000");
    histoNameVec.push_back("mH70_mSusy2000");
    histoNameVec.push_back("mH90_mSusy2000");
    histoNameVec.push_back("mH110_mSusy2000");
    histoNameVec.push_back("mH125_mSusy2000");
    histoNameVec.push_back("mH30_mSusy2200");
    histoNameVec.push_back("mH35_mSusy2200");
    histoNameVec.push_back("mH40_mSusy2200");
    histoNameVec.push_back("mH50_mSusy2200");
    histoNameVec.push_back("mH70_mSusy2200");
    histoNameVec.push_back("mH90_mSusy2200");
    histoNameVec.push_back("mH110_mSusy2200");
    histoNameVec.push_back("mH125_mSusy2200");
    histoNameVec.push_back("mH30_mSusy2400");
    histoNameVec.push_back("mH35_mSusy2400");
    histoNameVec.push_back("mH40_mSusy2400");
    histoNameVec.push_back("mH50_mSusy2400");
    histoNameVec.push_back("mH70_mSusy2400");
    histoNameVec.push_back("mH90_mSusy2400");
    histoNameVec.push_back("mH110_mSusy2400");
    histoNameVec.push_back("mH125_mSusy2400");
    histoNameVec.push_back("mH30_mSusy2600");
    histoNameVec.push_back("mH35_mSusy2600");
    histoNameVec.push_back("mH40_mSusy2600");
    histoNameVec.push_back("mH50_mSusy2600");
    histoNameVec.push_back("mH70_mSusy2600");
    histoNameVec.push_back("mH90_mSusy2600");
    histoNameVec.push_back("mH110_mSusy2600");
    histoNameVec.push_back("mH125_mSusy2600");
    histoNameVec.push_back("mH30_mSusy2800");
    histoNameVec.push_back("mH35_mSusy2800");
    histoNameVec.push_back("mH40_mSusy2800");
    histoNameVec.push_back("mH50_mSusy2800");
    histoNameVec.push_back("mH70_mSusy2800");
    histoNameVec.push_back("mH90_mSusy2800");
    histoNameVec.push_back("mH110_mSusy2800");
    histoNameVec.push_back("mH125_mSusy2800");

    histoNameVec.push_back("mH30_mSquark800");
    histoNameVec.push_back("mH50_mSquark800");
    histoNameVec.push_back("mH70_mSquark800");
    histoNameVec.push_back("mH90_mSquark800");
    histoNameVec.push_back("mH110_mSquark800");
    histoNameVec.push_back("mH125_mSquark800");
    histoNameVec.push_back("mH30_mSquark1200");
    histoNameVec.push_back("mH35_mSquark1200");
    histoNameVec.push_back("mH40_mSquark1200");
    histoNameVec.push_back("mH50_mSquark1200");
    histoNameVec.push_back("mH70_mSquark1200");
    histoNameVec.push_back("mH90_mSquark1200");
    histoNameVec.push_back("mH110_mSquark1200");
    histoNameVec.push_back("mH125_mSquark1200");
    histoNameVec.push_back("mH30_mSquark1600");
    histoNameVec.push_back("mH35_mSquark1600");
    histoNameVec.push_back("mH40_mSquark1600");
    histoNameVec.push_back("mH50_mSquark1600");
    histoNameVec.push_back("mH70_mSquark1600");
    histoNameVec.push_back("mH90_mSquark1600");
    histoNameVec.push_back("mH110_mSquark1600");
    histoNameVec.push_back("mH125_mSquark1600");
    histoNameVec.push_back("mH30_mSquark2000");
    histoNameVec.push_back("mH35_mSquark2000");
    histoNameVec.push_back("mH40_mSquark2000");
    histoNameVec.push_back("mH50_mSquark2000");
    histoNameVec.push_back("mH70_mSquark2000");
    histoNameVec.push_back("mH90_mSquark2000");
    histoNameVec.push_back("mH110_mSquark2000");
    histoNameVec.push_back("mH125_mSquark2000");
    histoNameVec.push_back("mH30_mSquark2200");
    histoNameVec.push_back("mH35_mSquark2200");
    histoNameVec.push_back("mH40_mSquark2200");
    histoNameVec.push_back("mH50_mSquark2200");
    histoNameVec.push_back("mH70_mSquark2200");
    histoNameVec.push_back("mH90_mSquark2200");
    histoNameVec.push_back("mH110_mSquark2200");
    histoNameVec.push_back("mH125_mSquark2200");
    histoNameVec.push_back("mH30_mSquark2400");
    histoNameVec.push_back("mH35_mSquark2400");
    histoNameVec.push_back("mH40_mSquark2400");
    histoNameVec.push_back("mH50_mSquark2400");
    histoNameVec.push_back("mH70_mSquark2400");
    histoNameVec.push_back("mH90_mSquark2400");
    histoNameVec.push_back("mH110_mSquark2400");
    histoNameVec.push_back("mH125_mSquark2400");
    histoNameVec.push_back("mH30_mSquark2600");
    histoNameVec.push_back("mH35_mSquark2600");
    histoNameVec.push_back("mH40_mSquark2600");
    histoNameVec.push_back("mH50_mSquark2600");
    histoNameVec.push_back("mH70_mSquark2600");
    histoNameVec.push_back("mH90_mSquark2600");
    histoNameVec.push_back("mH110_mSquark2600");
    histoNameVec.push_back("mH125_mSquark2600");
    histoNameVec.push_back("mH30_mSquark2800");
    histoNameVec.push_back("mH35_mSquark2800");
    histoNameVec.push_back("mH40_mSquark2800");
    histoNameVec.push_back("mH50_mSquark2800");
    histoNameVec.push_back("mH70_mSquark2800");
    histoNameVec.push_back("mH90_mSquark2800");
    histoNameVec.push_back("mH110_mSquark2800");
    histoNameVec.push_back("mH125_mSquark2800");

    std::vector<std::string> nonTrivialSysVec;
    nonTrivialSysVec.push_back("NOSYS");
    nonTrivialSysVec.push_back("jecAKXUncUp");
    nonTrivialSysVec.push_back("jecAKXUncDown");
    nonTrivialSysVec.push_back("jerAKXUncUp");
    nonTrivialSysVec.push_back("jerAKXUncDown");
    nonTrivialSysVec.push_back("jmsUncUp");
    nonTrivialSysVec.push_back("jmsUncDown");
    nonTrivialSysVec.push_back("jmrUncUp");
    nonTrivialSysVec.push_back("jmrUncDown");
    nonTrivialSysVec.push_back("dbtTagUp");
    nonTrivialSysVec.push_back("dbtTagDown");
    nonTrivialSysVec.push_back("isrUp");
    nonTrivialSysVec.push_back("isrDown");

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

std::vector<double> GetQcdUnDLowerBoundInHtDivison(TH1D* dataTagUnD, const unsigned int& numberOfHtDivisions)
{
    std::vector<double> qcdUnDEmptyCountInHtDivison;
    std::vector<double> qcdUnDLowerBoundInHtDivison;
    const unsigned int numberOfBins = dataTagUnD->GetNbinsX();
    const unsigned int numberOfBinsPerHtDivison = numberOfBins / numberOfHtDivisions;

    for (unsigned int iHT = 0; iHT < numberOfHtDivisions; iHT++){
        
        unsigned int nEmptyInHtDivision = 0;
        for (unsigned int iBin = 1; iBin <= numberOfBinsPerHtDivison; iBin++){

            const unsigned int iBinCombo = iHT * numberOfBinsPerHtDivison + iBin;
            if (dataTagUnD->GetBinContent(iBinCombo) == 0) nEmptyInHtDivision++;

        } // closes loop through bin indices of a HT division
    
        qcdUnDEmptyCountInHtDivison.push_back(nEmptyInHtDivision);
    } // closes loop through HT divisons

    for (auto count : qcdUnDEmptyCountInHtDivison){
        if (count == 0) qcdUnDLowerBoundInHtDivison.push_back(1.0);
        else qcdUnDLowerBoundInHtDivison.push_back(1.0/count);
    }

    return qcdUnDLowerBoundInHtDivison;
} // closes function GetQcdUnDLowerBoundInHtDivison

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
        if (k_down == 0) k_down = 0.001;
        if (k_up == 0) k_up = 0.001;

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