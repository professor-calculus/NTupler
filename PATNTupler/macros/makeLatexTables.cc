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


// MAKE LATEX TABLES


void GetHistograms(std::map<std::string,TH1D*>&, const unsigned int&); // NEED TO CHANGE THE FILE PATH IN THIS FUNCTION WHEN USING NEW HISTOGRAMS

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

void WriteBlock(const std::string&, const unsigned int&, std::ofstream&, const bool = false);


int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    // ONE: save info (signal specific directories beneath this)
    const std::string outputDir = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/combinedDataCards_2019_05_20/TESTING_LATEX/";
  


    // TWO: physics info - to match the histograms that you use
    const unsigned int numberOfBins = 60;
    const unsigned int numberOfHtDivisions = 3;



    // THREE: Samples To Use (different project for each signal sample)
    const std::string dataSample = "data";
    const std::string signal = "P1_1800sq_R0p99";
                                       
    std::map<unsigned int, std::vector<std::string>> mcbkVec;
    mcbkVec[2016] = {"TTJets", "ST", "Ewk"}; // the 2016 MC background samples
    mcbkVec[2017] = {"TTJets", "ST", "Ewk"}; // the 2017 MC background samples
    const std::string qcdName = "QCD"; // this is just a label as QCD contribution is driven during the fit



    // FOUR: data card layout info
    const unsigned int colSize = 25;



    // FIVE: common systematics (statistical error systematics automatically taken care of)
    std::map<unsigned int, std::vector<CommonSystematic>> CommonSystematicVec;
    // NB1 - make the sure the sample names match to the above (for the given year)
    // NB2 - "SIGNAL" refers all signal samples
    // *** 2016 ***
    CommonSystematicVec[2016].push_back( CommonSystematic("signalPdfBOTH lnN", 1.1, {"SIGNAL"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("isrReweightBOTH lnN", "isr", {"SIGNAL"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("luminosity2016 lnN", 1.025, {"SIGNAL", "TTJets", "ST", "Ewk"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("XS_TTJets2016 lnN", 2.0, {"TTJets"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("XS_ST2016 lnN", 1.5, {"ST"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("XS_Ewk2016 lnN", 1.5, {"Ewk"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("jecAKXUncBOTH lnN", "jecAKXUnc", {"SIGNAL", "TTJets", "ST", "Ewk"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("jerAKXUnc2016 lnN", "jerAKXUnc", {"SIGNAL", "TTJets", "ST", "Ewk"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("jmsUncBOTH lnN", "jmsUnc", {"SIGNAL", "TTJets", "ST", "Ewk"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("jmrUncBOTH lnN", "jmrUnc", {"SIGNAL", "TTJets", "ST", "Ewk"}) ); // correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("SigDbtTag2016 lnN", "dbtTag", {"SIGNAL"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("TtDbtTag2016 lnN", "dbtTag", {"TTJets"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("prefire2016 lnN", "prefire", {"SIGNAL", "TTJets", "ST", "Ewk"}) );
    CommonSystematicVec[2016].push_back( CommonSystematic("scaleUncSigBoth lnN", 1.25, {"SIGNAL"}) ); // Correlated to 2017
    CommonSystematicVec[2016].push_back( CommonSystematic("scaleUncBoth lnN", "scale", {"TTJets", "ST", "Ewk"}) ); // Correlated to 2017
    // *** 2017 ***
    CommonSystematicVec[2017].push_back( CommonSystematic("signalPdfBOTH lnN", 1.1, {"SIGNAL"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("isrReweightBOTH lnN", "isr", {"SIGNAL"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("luminosity2017 lnN", 1.023, {"SIGNAL", "TTJets", "ST", "Ewk"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("XS_TTJets2017 lnN", 2.0, {"TTJets"}) );
    //CommonSystematicVec[2017].push_back( CommonSystematic("XS_TTJets1L2017 lnN", 3.0, {"TTJets1L"}) );
    //CommonSystematicVec[2017].push_back( CommonSystematic("XS_TTJets2L2017 lnN", 3.0, {"TTJets2L"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("XS_ST2017 lnN", 1.5, {"ST"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("XS_Ewk2017 lnN", 1.5, {"Ewk"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("jecAKXUncBOTH lnN", "jecAKXUnc", {"SIGNAL", "TTJets", "ST", "Ewk"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("jerAKXUnc2017 lnN", "jerAKXUnc", {"SIGNAL", "TTJets", "ST", "Ewk"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("jmsUncBOTH lnN", "jmsUnc", {"SIGNAL", "TTJets", "ST", "Ewk"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("jmrUncBOTH lnN", "jmrUnc", {"SIGNAL", "TTJets", "ST", "Ewk"}) ); // correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("SigDbtTag2017 lnN", "dbtTag", {"SIGNAL"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("TtDbtTag2017 lnN", "dbtTag", {"TTJets"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("prefire2017 lnN", "prefire", {"SIGNAL", "TTJets", "ST", "Ewk"}) );
    CommonSystematicVec[2017].push_back( CommonSystematic("scaleUncSigBoth lnN", 1.25, {"SIGNAL"}) ); // Correlated to 2016
    CommonSystematicVec[2017].push_back( CommonSystematic("scaleUncBoth lnN", "scale", {"TTJets", "ST", "Ewk"}) ); // Correlated to 2016


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

    std::ofstream dataCard;
    dataCard.open( Form("%stableInfo.tex", outputDir.c_str() ) );

    // loop through 2016 and 2017
    std::vector<unsigned int> yearOfRunVec = {2016};
    //std::vector<unsigned int> yearOfRunVec = {2016, 2017};
    for (auto yearOfRun : yearOfRunVec){

        std::map<std::string, TH1D*> hOriginal_;
        GetHistograms(hOriginal_, yearOfRun);

        if (yearOfRun == 2016){
            const std::string dirExistCommand = "test -e " + outputDir;
            const std::string makeDirCommand = "mkdir -p " + outputDir;
            if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
            std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/makeLatexTables.cc %s/%s__makeLatexTables.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));                
        }

        // loop through each search region bin
        for (unsigned int iBin = 1; iBin < numberOfBins + 1; ++iBin){

            unsigned int data_obs_S = hOriginal_[Form("S_tag_%s_NOSYS", dataSample.c_str())]->GetBinContent(iBin);
            const unsigned int data_obs_UnD = hOriginal_[Form("UnD_tag_%s_NOSYS", dataSample.c_str())]->GetBinContent(iBin);
            const double rate_signal_S = hOriginal_[Form("S_tag_%s_NOSYS", signal.c_str())]->GetBinContent(iBin);
            const double rate_signal_UnD = hOriginal_[Form("UnD_tag_%s_NOSYS", signal.c_str())]->GetBinContent(iBin);;
            std::vector<double> rate_mcbkVec_S;
            std::vector<double> rate_mcbkVec_UnD;
            for (auto mcbk : mcbkVec[yearOfRun]){
                double rate_S = hOriginal_[Form("S_tag_%s_NOSYS", mcbk.c_str())]->GetBinContent(iBin);               
                double rate_UnD = hOriginal_[Form("UnD_tag_%s_NOSYS", mcbk.c_str())]->GetBinContent(iBin);
                rate_mcbkVec_S.push_back(rate_S);
                rate_mcbkVec_UnD.push_back(rate_UnD);
            }

            // write the data card
            unsigned int binLabel = iBin;
            if (yearOfRun == 2017) binLabel += numberOfBins;

            // TABLE INTRO
            dataCard << "\n";
            dataCard << "\n";
            dataCard << "\n";
            dataCard << "\\clearpage \n";

            dataCard << "\\begin{table}[hbtp] \n";
            dataCard << "\\centering \n";

            dataCard << "\\topcaption{ \n";

            dataCard << "The event yield information for search region ";
            dataCard << iBin;
            dataCard << ", using ";
            dataCard << yearOfRun;
            dataCard << " data and MC.\n";
            dataCard << "For both the signal and sideband mass regions,\n";
            dataCard << "the observed data yield and the MC yields are provided.\n";
            dataCard << "For each systematic,\n";
            dataCard << "the factors by which the MC yields change under 1$\\sigma$ variation is also given.\n";
            dataCard << "The signal sample corresponds to the signal model\n";
            dataCard << "with parameters $M_{\text{LSP}} = 1$~GeV and $M_{\\textrm{SUSY}} = 1800$~GeV.\n";
            dataCard << "At the bottom of the table, the $F_{i}$ factor,\n";
            dataCard << "which is used for the QCD estimation, is provided.\n";
            dataCard << "All numbers are given to three decimal places.\n";

            dataCard << "} \n";

            dataCard << "\\makebox[\\textwidth]{ \n";
            dataCard << "\\begin{tabular}{|c||";
            for (unsigned int count = 0; count < rate_mcbkVec_S.size() + 1; count++)
                dataCard << "c|";
            dataCard << "} \n";
            dataCard << "\\hline \n";

            // TABLE S MASS REGION
            dataCard << "Mass type & \n";
            dataCard << "\\multicolumn{";
            dataCard << rate_mcbkVec_S.size() + 1;
            dataCard << "}{|c|}{$S$ mass region} \\\\ \n";

            dataCard << "Observed yield & \n";
            dataCard << "\\multicolumn{";
            dataCard << rate_mcbkVec_S.size() + 1;
            dataCard << "}{|c|}{";
            if (areWeBlinded == true) dataCard << "-";
            else dataCard << data_obs_S;
            dataCard << "} \\\\ \n";

            dataCard << "\\hline \n";

            WriteBlock("MC process", colSize, dataCard);
            WriteBlock("& signal", colSize, dataCard);
            for (auto mcbk : mcbkVec[yearOfRun]) WriteBlock(Form("& %s", mcbk.c_str()), colSize, dataCard);
            dataCard << "\\\\ \n";

            WriteBlock("Yield", colSize, dataCard);
            WriteBlock(Form("& %.3f", rate_signal_S), colSize, dataCard);
            for (auto rate_mcbk_S : rate_mcbkVec_S) WriteBlock(Form("& %.3f", rate_mcbk_S), colSize, dataCard);
            dataCard << "\\\\ \n";
            dataCard << "\\hline \n";

            WriteBlock("\\underline{Systematic}", colSize, dataCard);
            for (unsigned int count = 0; count < rate_mcbkVec_S.size() + 1; count++)
                WriteBlock("&", colSize, dataCard);
            dataCard << " \\\\ \n";

            for (auto CommonSystematic : CommonSystematicVec[yearOfRun]){
                WriteBlock(CommonSystematic.GetSystematicName(), colSize, dataCard);

                std::string histoPreamble = "S_tag_";
                const std::vector<std::string> systematicProcesses = CommonSystematic.GetSystematicProcesses();

                // signal
                if (std::find(systematicProcesses.begin(), systematicProcesses.end(), "SIGNAL") != systematicProcesses.end()) {
                    const std::string fullHistogramName = histoPreamble + signal;
                    const std::string systematicValue = CommonSystematic.GetSystematicValue(fullHistogramName.c_str(), iBin, hOriginal_);
                    WriteBlock(Form("& %s", systematicValue.c_str()), colSize, dataCard);
                } else {
                    WriteBlock("& -", colSize, dataCard);
                }

                // monte carlo background
                for (auto mcbk : mcbkVec[yearOfRun]){
                    if (std::find(systematicProcesses.begin(), systematicProcesses.end(), mcbk) != systematicProcesses.end()) {
                        const std::string fullHistogramName = histoPreamble + mcbk;
			//std::cout << fullHistogramName << "\t" << CommonSystematic.GetSystematicName() << "\t" << iBin << std::endl;
                        const std::string systematicValue = CommonSystematic.GetSystematicValue(fullHistogramName.c_str(), iBin, hOriginal_);
                        WriteBlock(Form("& %s", systematicValue.c_str()), colSize, dataCard);
                    } else {
                        WriteBlock("& -", colSize, dataCard);
                    }                   
                }

                dataCard << " \\\\ \n";
            }


            // TABLE U+D MASS REGION
            dataCard << "\\hline \n";
            dataCard << "\\hline \n";

            dataCard << "Mass type & \n";
            dataCard << "\\multicolumn{";
            dataCard << rate_mcbkVec_UnD.size() + 1;
            dataCard << "}{|c|}{$U+D$ mass region} \\\\ \n";

            dataCard << "Observed yield & \n";
            dataCard << "\\multicolumn{";
            dataCard << rate_mcbkVec_UnD.size() + 1;
            dataCard << "}{|c|}{";
            dataCard << data_obs_UnD;
            dataCard << "} \\\\ \n";

            dataCard << "\\hline \n";

            WriteBlock("MC process", colSize, dataCard);
            WriteBlock("& signal", colSize, dataCard);
            for (auto mcbk : mcbkVec[yearOfRun]) WriteBlock(Form("& %s", mcbk.c_str()), colSize, dataCard);
            dataCard << "\\\\ \n";

            WriteBlock("Yield", colSize, dataCard);
            WriteBlock(Form("& %.3f", rate_signal_UnD), colSize, dataCard);
            for (auto rate_mcbk_UnD : rate_mcbkVec_UnD) WriteBlock(Form("& %.3f", rate_mcbk_UnD), colSize, dataCard);
            dataCard << "\\\\ \n";
            dataCard << "\\hline \n";

            WriteBlock("\\underline{Systematic}", colSize, dataCard);
            for (unsigned int count = 0; count < rate_mcbkVec_UnD.size() + 1; count++)
                WriteBlock("&", colSize, dataCard);
            dataCard << " \\\\ \n";

            for (auto CommonSystematic : CommonSystematicVec[yearOfRun]){
                WriteBlock(CommonSystematic.GetSystematicName(), colSize, dataCard);

                std::string histoPreamble = "UnD_tag_";
                const std::vector<std::string> systematicProcesses = CommonSystematic.GetSystematicProcesses();

                // signal
                if (std::find(systematicProcesses.begin(), systematicProcesses.end(), "SIGNAL") != systematicProcesses.end()) {
                    const std::string fullHistogramName = histoPreamble + signal;
                    const std::string systematicValue = CommonSystematic.GetSystematicValue(fullHistogramName.c_str(), iBin, hOriginal_);
                    WriteBlock(Form("& %s", systematicValue.c_str()), colSize, dataCard);
                } else {
                    WriteBlock("& -", colSize, dataCard);
                }

                // monte carlo background
                for (auto mcbk : mcbkVec[yearOfRun]){
                    if (std::find(systematicProcesses.begin(), systematicProcesses.end(), mcbk) != systematicProcesses.end()) {
                        const std::string fullHistogramName = histoPreamble + mcbk;
                        const std::string systematicValue = CommonSystematic.GetSystematicValue(fullHistogramName.c_str(), iBin, hOriginal_);
                        WriteBlock(Form("& %s", systematicValue.c_str()), colSize, dataCard);
                    } else {
                        WriteBlock("& -", colSize, dataCard);
                    }                   
                }

                dataCard << " \\\\ \n";
            }


            // TABLE F FACTOR
            dataCard << "\\hline \n";
            dataCard << "\\hline \n";

            double corrRatio = QcdSidebandCorr::GetCorr(iBin, yearOfRun);
            double corrRatioError = QcdSidebandCorr::GetCorrErr(iBin, yearOfRun);           
            dataCard << "$F_{i}$ factor & \n";
            dataCard << "\\multicolumn{";
            dataCard << rate_mcbkVec_UnD.size() + 1;
            dataCard << "}{|c|}{";
            dataCard << Form("%.3f", corrRatio);
            dataCard << " $\\pm$ ";
            dataCard << Form("%.3f", corrRatioError);
            dataCard << "} \\\\ \n";
            dataCard << "\\hline \n";


            // TABLE OUTRO
            dataCard << "\\end{tabular}} \n";
            dataCard << "\\label{table:yieldTable_bin";
            dataCard << binLabel;
            dataCard << "} \n";
            dataCard << "\\end{table} \n";


        } // closes loop through the search region bins
        
        std::cout << std::endl;

    } // closes loop through yearOfRunVec elements

    dataCard.close();

    return 0; 
}


void GetHistograms(std::map<std::string,TH1D*>& h_, const unsigned int& yearOfRun)
{
    // histos locations
    std::string preamble = "";
    std::string postamble = "";

    if (yearOfRun == 2016){
        preamble = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/histos_MHT_2019_01_08/MassCutsV10_dPhiMHT_FINAL/run2016/";
        postamble = "MassCutsV10_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi36.root";
    }
    else if (yearOfRun == 2017){
        preamble = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/histos_MHT_2019_01_08/MassCutsV10_dPhiMHT_FINAL/run2017/";
        postamble = "MassCutsV10_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi41.root";
    }
    else{
        std::cout << "You have not given GetHistograms a valid year" << std::endl;
        return;
    }

    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("data");
    histoNameVec.push_back("TTJets");
    //histoNameVec.push_back("TTJets0L");
    //histoNameVec.push_back("TTJets1L");
    //histoNameVec.push_back("TTJets2L");
    histoNameVec.push_back("ST");
    histoNameVec.push_back("Ewk");

    histoNameVec.push_back("P1_1200sq_R0p384");
    histoNameVec.push_back("P1_1200sq_R0p555");
    histoNameVec.push_back("P1_1200sq_R0p99");
    histoNameVec.push_back("P1_1400sq_R0p384");
    histoNameVec.push_back("P1_1400sq_R0p555");
    histoNameVec.push_back("P1_1400sq_R0p99");
    histoNameVec.push_back("P1_1800sq_R0p384");
    histoNameVec.push_back("P1_1800sq_R0p555");
    histoNameVec.push_back("P1_1800sq_R0p99");
    histoNameVec.push_back("P1_2200sq_R0p384");
    histoNameVec.push_back("P1_2200sq_R0p555");
    histoNameVec.push_back("P1_2200sq_R0p99");
    histoNameVec.push_back("P1_2600sq_R0p384");
    histoNameVec.push_back("P1_2600sq_R0p555");
    histoNameVec.push_back("P1_2600sq_R0p99");
    histoNameVec.push_back("P3_1300sq_R0p384");
    histoNameVec.push_back("P3_1300sq_R0p555");
    histoNameVec.push_back("P3_1300sq_R0p99");
    histoNameVec.push_back("P3_1500sq_R0p384");
    histoNameVec.push_back("P3_1500sq_R0p555");
    histoNameVec.push_back("P3_1500sq_R0p99");
    histoNameVec.push_back("P3_1900sq_R0p384");
    histoNameVec.push_back("P3_1900sq_R0p555");
    histoNameVec.push_back("P3_1900sq_R0p99");
    histoNameVec.push_back("P3_2300sq_R0p384");
    histoNameVec.push_back("P3_2300sq_R0p555");
    histoNameVec.push_back("P3_2300sq_R0p99");
    histoNameVec.push_back("P3_2700sq_R0p384");
    histoNameVec.push_back("P3_2700sq_R0p555");
    histoNameVec.push_back("P3_2700sq_R0p99");
    histoNameVec.push_back("P5_1200sq_R0p384");
    histoNameVec.push_back("P5_1200sq_R0p555");
    histoNameVec.push_back("P5_1200sq_R0p99");
    histoNameVec.push_back("P5_1400sq_R0p384");
    histoNameVec.push_back("P5_1400sq_R0p555");
    histoNameVec.push_back("P5_1400sq_R0p99");
    histoNameVec.push_back("P5_1800sq_R0p384");
    histoNameVec.push_back("P5_1800sq_R0p555");
    histoNameVec.push_back("P5_1800sq_R0p99");
    histoNameVec.push_back("P5_2200sq_R0p384");
    histoNameVec.push_back("P5_2200sq_R0p555");
    histoNameVec.push_back("P5_2200sq_R0p99");
    histoNameVec.push_back("P5_2600sq_R0p384");
    histoNameVec.push_back("P5_2600sq_R0p555");
    histoNameVec.push_back("P5_2600sq_R0p99");
    histoNameVec.push_back("P6_1200sq_R0p384");
    histoNameVec.push_back("P6_1200sq_R0p555");
    histoNameVec.push_back("P6_1200sq_R0p99");
    histoNameVec.push_back("P6_1400sq_R0p384");
    histoNameVec.push_back("P6_1400sq_R0p555");
    histoNameVec.push_back("P6_1400sq_R0p99");
    histoNameVec.push_back("P6_1800sq_R0p384");
    histoNameVec.push_back("P6_1800sq_R0p555");
    histoNameVec.push_back("P6_1800sq_R0p99");
    histoNameVec.push_back("P6_2200sq_R0p384");
    histoNameVec.push_back("P6_2200sq_R0p555");
    histoNameVec.push_back("P6_2200sq_R0p99");
    histoNameVec.push_back("P6_2600sq_R0p384");
    histoNameVec.push_back("P6_2600sq_R0p555");
    histoNameVec.push_back("P6_2600sq_R0p99");
    histoNameVec.push_back("P7_1300sq_R0p384");
    histoNameVec.push_back("P7_1300sq_R0p555");
    histoNameVec.push_back("P7_1300sq_R0p99");
    histoNameVec.push_back("P7_1500sq_R0p384");
    histoNameVec.push_back("P7_1500sq_R0p555");
    histoNameVec.push_back("P7_1500sq_R0p99");
    histoNameVec.push_back("P7_1900sq_R0p384");
    histoNameVec.push_back("P7_1900sq_R0p555");
    histoNameVec.push_back("P7_1900sq_R0p99");
    histoNameVec.push_back("P7_2300sq_R0p384");
    histoNameVec.push_back("P7_2300sq_R0p555");
    histoNameVec.push_back("P7_2300sq_R0p99");
    histoNameVec.push_back("P7_2700sq_R0p384");
    histoNameVec.push_back("P7_2700sq_R0p555");
    histoNameVec.push_back("P7_2700sq_R0p99");
    histoNameVec.push_back("P8_1300sq_R0p384");
    histoNameVec.push_back("P8_1300sq_R0p555");
    histoNameVec.push_back("P8_1300sq_R0p99");
    histoNameVec.push_back("P8_1500sq_R0p384");
    histoNameVec.push_back("P8_1500sq_R0p555");
    histoNameVec.push_back("P8_1500sq_R0p99");
    histoNameVec.push_back("P8_1900sq_R0p384");
    histoNameVec.push_back("P8_1900sq_R0p555");
    histoNameVec.push_back("P8_1900sq_R0p99");
    histoNameVec.push_back("P8_2300sq_R0p384");
    histoNameVec.push_back("P8_2300sq_R0p555");
    histoNameVec.push_back("P8_2300sq_R0p99");
    histoNameVec.push_back("P8_2700sq_R0p384");
    histoNameVec.push_back("P8_2700sq_R0p555");
    histoNameVec.push_back("P8_2700sq_R0p99");

    std::vector<std::string> nonTrivialSysVec;
    nonTrivialSysVec.push_back("NOSYS");
    //nonTrivialSysVec.push_back("jecAK4UncUp");
    //nonTrivialSysVec.push_back("jecAK4UncDown");
    //nonTrivialSysVec.push_back("jerAK4UncUp");
    //nonTrivialSysVec.push_back("jerAK4UncDown");
    //nonTrivialSysVec.push_back("jecAK8UncUp");
    //nonTrivialSysVec.push_back("jecAK8UncDown");
    //nonTrivialSysVec.push_back("jerAK8UncUp");
    //nonTrivialSysVec.push_back("jerAK8UncDown");
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
    nonTrivialSysVec.push_back("prefireUp");
    nonTrivialSysVec.push_back("prefireDown");
    nonTrivialSysVec.push_back("scaleUp");
    nonTrivialSysVec.push_back("scaleDown");

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
systematicValue(Form("%.3f", systematicValueNumber)),
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

	//std::cout << fullHistogramName.c_str() << systematicHistoTag.c_str() << "Up" << std::endl;

        double count_sysUp = hOriginal_[Form("%s_%sUp", fullHistogramName.c_str(), systematicHistoTag.c_str())]->GetBinContent(iBin);
        double count_sysDown = hOriginal_[Form("%s_%sDown", fullHistogramName.c_str(), systematicHistoTag.c_str())]->GetBinContent(iBin);

        double k_down = count_sysDown / count_nominal;
        double k_up = count_sysUp / count_nominal;
        if (k_down == 0) k_down = 0.001;
        if (k_up == 0) k_up = 0.001;

        std::string systenaticValueSpec = Form("%.3f/%.3f", k_down, k_up);
        return systenaticValueSpec;
    }
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
