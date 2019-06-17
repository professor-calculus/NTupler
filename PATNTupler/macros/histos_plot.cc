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
#include "PlotEntry2D.hh"
#include "Plotter.hh"
#include "MassRegionCuts.hh"
#include "DoubleBTagWPs.h"
#include "TimeStamp.h"
#include "QcdSidebandCorr.h"

// MAKES PLOTS USING HISTOGRAMS

void GetHistograms2016(std::map<std::string,TH1D*>&);
void GetHistograms2017(std::map<std::string,TH1D*>&);
int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////




    // ONE: save info & luminosity
    const std::string outputDir = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/plots_2019_05_20/2016_80X/oneDimensionRepresentation/MassCuts_V10/MountainRange/Many_Backgrounds/"; // where we are going to save the output plots (should include the samples name, and any important features)
    
    const double luminosity = 35.867; // 2016 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
    // const double luminosity = 41.370; // 2017 Plots::: NB this is just a label for the plot. It should match the lumi of the histograms!
  



    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.
    std::string dirExistCommand = "test -e " + outputDir;
    std::string makeDirCommand = "mkdir -p " + outputDir;
    if (std::system(dirExistCommand.c_str()) != 0) std::system(makeDirCommand.c_str());
    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/histos_plot.cc %s/%s__histos_plot.cc", outputDir.c_str(), TimeStamp::GetTimeStamp().c_str()));

    std::map<std::string, TH1D*> h16_;
    std::map<std::string, TH1D*> h17_;
    GetHistograms2016(h16_);
    //GetHistograms2017(h17_);
    // explanation of terminology
    // 1. S, U, D --> refers to mass space. pred(Old)(New) is the prediction of S. UnD is the sum U+D.
    // 2. tag, anti, control --> refers to 2*DBT space
    // 3. sample name on the end
    // 4. OPTIONAL, add systematic name

    // FOR F_{i} PLOTS 2016: DON"T DELETE
    // h16_["S_anti_data"]->Divide(h16_["UnD_anti_data"]);
    // h16_["S_anti_data"]->GetYaxis()->SetTitle("F_{i}");
    // h16_["S_anti_QCD"]->Divide(h16_["UnD_anti_QCD"]);
    // h16_["S_anti_QCD"]->GetYaxis()->SetTitle("F_{i}");
    // TH1D * h16 = new TH1D("h16", "", 30, 0, 30);
    // for (unsigned int i = 1; i < 31; ++i){
        // h16->SetBinContent(i, QcdSidebandCorr::GetCorr(i, 2016));
        // h16->SetBinError(i, QcdSidebandCorr::GetCorrErr(i, 2016));
        // std::cout << Form("%.10f", h16_["S_anti_data"]->GetBinContent(i) ) << std::endl;
        // std::cout << Form("%.10f", h16_["S_anti_data"]->GetBinError(i) ) << std::endl;
    // }
    
    // FOR F_{i} PLOTS 2017: DON"T DELETE
    // h17_["S_anti_data"]->Divide(h17_["UnD_anti_data"]);
    // h17_["S_anti_data"]->GetYaxis()->SetTitle("F_{i}");
    // h17_["S_anti_QCD"]->Divide(h17_["UnD_anti_QCD"]);
    // h17_["S_anti_QCD"]->GetYaxis()->SetTitle("F_{i}");
    // TH1D * h17 = new TH1D("h17", "", 30, 0, 30);
    // for (unsigned int i = 1; i < 31; ++i){
    //     h17->SetBinContent(i, QcdSidebandCorr::GetCorr(i, 2017));
    //     h17->SetBinError(i, QcdSidebandCorr::GetCorrErr(i, 2017));
    //     std::cout << Form("%.10f", h17_["S_anti_data"]->GetBinContent(i) ) << std::endl;
    //     std::cout << Form("%.10f", h17_["S_anti_data"]->GetBinError(i) ) << std::endl;
    // }
    // *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,. *,.





    // TWO: make plot aesthetics and saving
    
    // std::vector<TH1D*> indiHistoVec = {h16_["S_control_data"], h16_["predNew_control_data"]};
    //std::vector<TH1D*> stackHistoVec = {h16_["S_tag_WJets"], h16_["S_tag_TTJets_MG_HTStitched600_noReWeighting"], h16_["S_tag_QCD"]};
    //std::vector<TH1D*> stackHistoVec = {h16_["S_tag_DiBoson"], h16_["S_tag_TTZ"], h16_["S_tag_TTW"], h16_["S_tag_WJetstoLNu"], h16_["S_tag_ZJetstoLL"], h16_["S_tag_WJets"], h16_["S_tag_ZJetstoQQ"], h16_["S_tag_ST"], h16_["S_tag_TTJets_MG_HTStitched600_noReWeighting"], h16_["S_tag_QCD"]};
    std::vector<TH1D*> stackHistoVec = {h16_["S_tag_Ewk"], h16_["S_tag_ST"], h16_["S_tag_TTJets_MG_HTStitched600"], h16_["S_tag_QCD"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_TTJets"], h16_["S_tag_QCD"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_tag_TTJets"], h16_["S_tag_TTJets_MGLO_HTStitched650"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_anti_TTJets"], h16_["S_anti_QCD"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_TTJets_Powheg"], h16_["S_tag_TTJets_Powheg_NoTopReWeighting"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_TTJets_MGLO_HTStitched600"], h16_["S_tag_TTJets_MGLO_HTStitched600_NoTopReWeighting"]}; 

    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_P1_1400sq_R0p99"], h16_["S_tag_P1_1400sq_R0p384"], h16_["S_tag_P1_1800sq_R0p99"], h16_["S_tag_P1_1800sq_R0p384"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["S_anti_P1_1900sq_R0p99"], h16_["S_anti_P1_1900sq_R0p384"], h16_["S_anti_TTJets"], h16_["S_anti_QCD"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["UnD_tag_P1_1900sq_R0p99"], h16_["UnD_tag_P1_1900sq_R0p384"], h16_["UnD_tag_TTJets"], h16_["UnD_tag_QCD"]};
    // std::vector<TH1D*> indiHistoVec = {h16_["UnD_anti_P1_1900sq_R0p99"], h16_["UnD_anti_P1_1900sq_R0p384"], h16_["UnD_anti_TTJets"], h16_["UnD_anti_QCD"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_P1_1800sq_R0p99"], h16_["S_tag_P1_1800sq_R0p384"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_P1_1800sq_R0p99_PUleq23"], h16_["S_tag_P1_1800sq_R0p99_PUgeq24"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_CPS_mH70_2600sq_scaleUp"], h16_["S_tag_CPS_mH70_2600sq"], h16_["S_tag_CPS_mH70_2600sq_scaleDown"]};
    //std::vector<TH1D*> indiHistoVec = {h16_["S_tag_TTJets_scaleUp"], h16_["S_tag_TTJets"], h16_["S_tag_TTJets_scaleDown"]};

    //Plotter plot = Plotter(indiHistoVec);
    Plotter plot = Plotter({}, stackHistoVec);
    //Plotter plot = Plotter(indiHistoVec, stackHistoVec);

    // std::vector<std::string> legendNames = {"control", "control prediction"};
    // std::vector<std::string> legendNames = {"TTJets", "QCD"};
    // std::vector<std::string> legendNames = {"Powheg", "MGLO HTStitched"};

    //std::vector<std::string> legendNames = {"1800GeV Sq 1GeV LSP", "1800GeV Sq 200GeV LSP"};
    //std::vector<std::string> legendNames = {"1400GeV Sq 1GeV LSP", "1400GeV Sq 200GeV LSP", "1800GeV Sq 1GeV LSP", "1800GeV Sq 200GeV LSP", "WJets", "TTJets", "QCD"};
    //std::vector<std::string> legendNames = {"1800GeV Sq 1GeV LSP", "1800GeV Sq 200GeV LSP", "WJets", "TTJets", "QCD"};
    std::vector<std::string> legendNames = {"Electroweak", "Single Top", "TTJets", "QCD"};
    //std::vector<std::string> legendNames = {"DiBoson","TTZ", "TTW", "WJets to LNu", "ZJets to LL", "WJets to QQ", "ZJets to QQ", "Single Top", "TTJets", "QCD"};
    //std::vector<std::string> legendNames = {"TTJets", "TTJets Without Top pT Reweighting"};
    //std::vector<std::string> legendNames = {"nPU <= 23", "nPU >= 24"};
    //std::vector<std::string> legendNames = {"Scale Up", "Nominal", "Scale Down"};

    // plot.AddLegend(legendNames, 0.16, 0.38, 0.64, 0.83, 0.028);
    // plot.AddLegend(legendNames, 0.18, 0.38, 0.18, 0.30, 0.040);
    // plot.AddLegend(legendNames, 0.67, 0.88, 0.64, 0.83, 0.028);
    plot.AddLegend(legendNames, 0.67, 0.88, 0.61, 0.80, 0.025); // with ratio box
    // plot.AddLegend2Cols(4, legendNames, 0.70, 0.84, 0.64, 0.79, 0.028);
    
    plot.AddLatex();
    // plot.AddLatex(luminosity);
    // plot.AddLatex("#it{Work In Progress}");
    //plot.AddLatex(luminosity, "#it{Work In Progress}");
    
    // plot.AddRatioBox("ratio");
    // plot.AddRatioBox("ratio", true);
    //plot.AddRatioBox(0.1, 1.9, "ratio", true);
    // plot.AddRatioBox("true / pred", true);
    // plot.AddRatioBox(0.1, 4.9, "ratio", true);

    //plot.AddRatioBox(0.1,2.4, "true / pred", true);
    //plot.AddRatioBox(0.1,2.4, "ratio", true);
    //plot.AddRatioBox(0.5,1.5, "ratio", true);

    // plot.AddRatioBox(0.84,1.16, "priv / central", true);
    
    plot.SetErrors();
    // plot.SetErrors("only_stack");
    // plot.SetErrors("only_indi");

    std::vector<std::string> mhtVec = {"MHT0-200", "MHT200+"};
    std::vector<std::string> stringVec = {"HT1500-2500", "HT2500-3500", "HT3500+"};
    std::string plotName = "linear";
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec, mhtVec);
    
    plotName = "log";
    plot.SetLogY();
    plot.SetYValueMin(0.01); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(0.50); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(1.10); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(3.10); // REMEMBER THIS PARAM! (only for log)
    // plot.SetYValueMin(12.10); // REMEMBER THIS PARAM! (only for log)
    plot.SaveSpec01(Form("%s/%s.pdf", outputDir.c_str(), plotName.c_str()), stringVec, mhtVec);





    return 0;
}





void GetHistograms2016(std::map<std::string,TH1D*>& h_)
{
    // histos locations
    std::string preamble = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/histos_MHT_2019_01_08/MassCutsV10_FINAL/run2016/";
    
    std::string postamble = "MassCutsV10_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi36.root";
    std::vector<std::string> histoNameVec;
    //histoNameVec.push_back("data");
    histoNameVec.push_back("QCD");
    //histoNameVec.push_back("TTJets");
    //histoNameVec.push_back("TTJets_Powheg");
    //histoNameVec.push_back("TTJets_Powheg_noReWeighting");
    histoNameVec.push_back("TTJets_MG_HTStitched600");
    //histoNameVec.push_back("TTJets_MG_HTStitched600_noReWeighting");
    //histoNameVec.push_back("ZJetstoQQ");
    //histoNameVec.push_back("WJets");
    //histoNameVec.push_back("WJetstoLNu");
    //histoNameVec.push_back("ZJetstoLL");
    //histoNameVec.push_back("TTW");
    //histoNameVec.push_back("TTZ");
    histoNameVec.push_back("ST");
    //histoNameVec.push_back("DiBoson");
    histoNameVec.push_back("Ewk");

    //histoNameVec.push_back("CPS_mH70_1200sq");
    //histoNameVec.push_back("CPS_mH70_2000sq");
    //histoNameVec.push_back("CPS_mH70_2600sq");

    //histoNameVec.push_back("P1_1800sq_R0p99_PUleq23");
    //histoNameVec.push_back("P1_1800sq_R0p99_PUgeq24");

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


    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        const std::string histoToUse = histoNameVec[iH];
        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoToUse.c_str(), postamble.c_str()));
        // explanation of terminology
        // 1. S, U, D --> refers to mass space. pred is the prediction of S. UnD is the sum U+D.
        // 2. tag, anti, control --> refers to 2*DBT space
        // 3. sample name on the end

	//std::cout << histoToUse.c_str() << std::endl;

        if (histoToUse != "data") h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtDiagUpLoose_NOSYS");
        h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtDiagUpLoose_NOSYS");
        h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtDiagUpLoose_NOSYS");

        h_[Form("S_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("U_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("D_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose_NOSYS");

        h_[Form("S_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("U_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("D_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("S_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("S_dbtOffIDBTCv23AndLooseMed2_NOSYS"));
        h_[Form("U_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("U_dbtOffIDBTCv23AndLooseMed2_NOSYS"));
        h_[Form("D_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("D_dbtOffIDBTCv23AndLooseMed2_NOSYS"));

        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add(h_[Form("D_tag_%s", histoToUse.c_str())]);

        h_[Form("UnD_anti_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_anti_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_anti_%s", histoToUse.c_str())]->Add(h_[Form("D_anti_%s", histoToUse.c_str())]);

        h_[Form("UnD_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_control_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_control_%s", histoToUse.c_str())]->Add(h_[Form("D_control_%s", histoToUse.c_str())]);
 
        // NEW METHOD OF PREDICTION
        h_[Form("predNew_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin, 2016);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin, 2016);
            double UnDValue = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinError(iBin);
            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinError(iBin, predError);
        }

        h_[Form("predNew_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_control_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin, 2016);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin, 2016);
            double UnDValue = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinError(iBin);
            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinError(iBin, predError);
        }

        // OLD METHOD OF PREDICTION
        h_[Form("predOld_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("predOld_tag_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("predOld_tag_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);

        h_[Form("predOld_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        h_[Form("predOld_control_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("predOld_control_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);

        // SYSTEMATIC VARIATIONS (for TAG histograms)
        std::vector<std::string> nonTrivialSysVec;
        nonTrivialSysVec.push_back("jecAK4UncUp");
        nonTrivialSysVec.push_back("jecAK4UncDown");
        nonTrivialSysVec.push_back("jerAK4UncUp");
        nonTrivialSysVec.push_back("jerAK4UncDown");
        nonTrivialSysVec.push_back("jecAK8UncUp");
        nonTrivialSysVec.push_back("jecAK8UncDown");
        nonTrivialSysVec.push_back("jerAK8UncUp");
        nonTrivialSysVec.push_back("jerAK8UncDown");
        nonTrivialSysVec.push_back("jmsUncUp");
        nonTrivialSysVec.push_back("jmsUncDown");
        nonTrivialSysVec.push_back("jmrUncUp");
        nonTrivialSysVec.push_back("jmrUncDown");
        nonTrivialSysVec.push_back("dbtTagUp");
        nonTrivialSysVec.push_back("dbtTagDown");
        nonTrivialSysVec.push_back("isrUp");
        nonTrivialSysVec.push_back("isrDown");
        if (histoToUse.at(0) != 'P' && histoToUse != "data"){
		nonTrivialSysVec.push_back("scaleUp");
                nonTrivialSysVec.push_back("scaleDown");
	}

        for (auto nonTrivialSys : nonTrivialSysVec){

            if ( (TH1D*)f->Get(Form("S_dbtDiagUpLoose_%s", nonTrivialSys.c_str())) == NULL ) continue;
            h_[Form("S_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())] = (TH1D*)f->Get(Form("S_dbtDiagUpLoose_%s", nonTrivialSys.c_str()));
            h_[Form("UnD_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())] = (TH1D*)f->Get(Form("U_dbtDiagUpLoose_%s", nonTrivialSys.c_str()));
            h_[Form("UnD_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())]->Add((TH1D*)f->Get(Form("D_dbtDiagUpLoose_%s", nonTrivialSys.c_str())));

        } // closes loop through nonTrivialSysVec

	if (histoToUse.at(0) != 'P' && histoToUse != "data" && histoToUse != "ZJetstoQQ" && histoToUse != "DiBoson" && histoToUse != "ZJetstoLL" && histoToUse != "WJetstoLNu" && histoToUse != "TTZ" && histoToUse != "TTW" && histoToUse != "ST"){
                h_[Form("S_tag_%s_scaleUp", histoToUse.c_str())]->Scale(h_[Form("S_tag_%s", histoToUse.c_str())]->Integral() / h_[Form("S_tag_%s_scaleUp", histoToUse.c_str())]->Integral());
                h_[Form("S_tag_%s_scaleDown", histoToUse.c_str())]->Scale(h_[Form("S_tag_%s", histoToUse.c_str())]->Integral() / h_[Form("S_tag_%s_scaleDown", histoToUse.c_str())]->Integral());
        }

    } // closes loop through histoNameVec
}






void GetHistograms2017(std::map<std::string,TH1D*>& h_)
{
    // histos locations
    std::string preamble = "/opt/ppd/scratch/titterton/Analysis_boostedNmssmHiggs/histos_MHT_2019_01_08/MassCutsV10/run2017/";
    
    std::string postamble = "MassCutsV10_ak8pt300_ht1500x2500x3500x_ak4pt300n-1_lumi42.root";
    std::vector<std::string> histoNameVec;
    histoNameVec.push_back("data");
    histoNameVec.push_back("QCD");
    histoNameVec.push_back("TTJetsALL");
    histoNameVec.push_back("TTJets0L");
    histoNameVec.push_back("TTJets1L");
    histoNameVec.push_back("TTJets2L");
    histoNameVec.push_back("ZJets");
    histoNameVec.push_back("WJets");

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

    for (size_t iH = 0; iH < histoNameVec.size(); ++iH){

        const std::string histoToUse = histoNameVec[iH];
        TFile * f = new TFile(Form("%s/%s/%s", preamble.c_str(), histoToUse.c_str(), postamble.c_str()));
        // explanation of terminology
        // 1. S, U, D --> refers to mass space. pred is the prediction of S. UnD is the sum U+D.
        // 2. tag, anti, control --> refers to 2*DBT space
        // 3. sample name on the end

        if (histoToUse != "data") h_[Form("S_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtDiagUpLoose_NOSYS");
        h_[Form("U_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtDiagUpLoose_NOSYS");
        h_[Form("D_tag_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtDiagUpLoose_NOSYS");

        h_[Form("S_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("U_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtOffLooseAndOffLoose_NOSYS");
        h_[Form("D_anti_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtOffLooseAndOffLoose_NOSYS");

        h_[Form("S_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("S_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("U_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("U_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("D_control_%s", histoToUse.c_str())] = (TH1D*)f->Get("D_dbtLooseMed2AndOffIDBTCv23_NOSYS");
        h_[Form("S_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("S_dbtOffIDBTCv23AndLooseMed2_NOSYS"));
        h_[Form("U_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("U_dbtOffIDBTCv23AndLooseMed2_NOSYS"));
        h_[Form("D_control_%s", histoToUse.c_str())]->Add((TH1D*)f->Get("D_dbtOffIDBTCv23AndLooseMed2_NOSYS"));

        h_[Form("UnD_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_tag_%s", histoToUse.c_str())]->Add(h_[Form("D_tag_%s", histoToUse.c_str())]);

        h_[Form("UnD_anti_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_anti_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_anti_%s", histoToUse.c_str())]->Add(h_[Form("D_anti_%s", histoToUse.c_str())]);

        h_[Form("UnD_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("U_control_%s", histoToUse.c_str())]->Clone();
        h_[Form("UnD_control_%s", histoToUse.c_str())]->Add(h_[Form("D_control_%s", histoToUse.c_str())]);
   
        // NEW METHOD OF PREDICTION
        h_[Form("predNew_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin, 2017);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin, 2017);
            double UnDValue = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_tag_%s", histoToUse.c_str())]->GetBinError(iBin);
            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_tag_%s", histoToUse.c_str())]->SetBinError(iBin, predError);
        }

        h_[Form("predNew_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        for (int iBin = 1; iBin < h_[Form("predNew_control_%s", histoToUse.c_str())]->GetNbinsX() + 1; ++iBin){
            double corrValue = QcdSidebandCorr::GetCorr(iBin, 2017);
            double corrError = QcdSidebandCorr::GetCorrErr(iBin, 2017);
            double UnDValue = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinContent(iBin);
            double UnDError = h_[Form("predNew_control_%s", histoToUse.c_str())]->GetBinError(iBin);
            double predValue = corrValue * UnDValue;
            double predError = 0.0;
            if (UnDValue != 0) predError = predValue * sqrt( (corrError/corrValue)*(corrError/corrValue) + (UnDError/UnDValue)*(UnDError/UnDValue) );
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinContent(iBin, predValue);
            h_[Form("predNew_control_%s", histoToUse.c_str())]->SetBinError(iBin, predError);
        }

        // OLD METHOD OF PREDICTION
        h_[Form("predOld_tag_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_tag_%s", histoToUse.c_str())]->Clone();
        h_[Form("predOld_tag_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("predOld_tag_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);

        h_[Form("predOld_control_%s", histoToUse.c_str())] = (TH1D*)h_[Form("UnD_control_%s", histoToUse.c_str())]->Clone();
        h_[Form("predOld_control_%s", histoToUse.c_str())]->Multiply(h_[Form("S_anti_%s", histoToUse.c_str())]);
        h_[Form("predOld_control_%s", histoToUse.c_str())]->Divide(h_[Form("UnD_anti_%s", histoToUse.c_str())]);

        // SYSTEMATIC VARIATIONS (for TAG histograms)
        std::vector<std::string> nonTrivialSysVec;
        nonTrivialSysVec.push_back("jecAK4UncUp");
        nonTrivialSysVec.push_back("jecAK4UncDown");
        nonTrivialSysVec.push_back("jerAK4UncUp");
        nonTrivialSysVec.push_back("jerAK4UncDown");
        nonTrivialSysVec.push_back("jecAK8UncUp");
        nonTrivialSysVec.push_back("jecAK8UncDown");
        nonTrivialSysVec.push_back("jerAK8UncUp");
        nonTrivialSysVec.push_back("jerAK8UncDown");
        nonTrivialSysVec.push_back("jmsUncUp");
        nonTrivialSysVec.push_back("jmsUncDown");
        nonTrivialSysVec.push_back("jmrUncUp");
        nonTrivialSysVec.push_back("jmrUncDown");
        nonTrivialSysVec.push_back("dbtTagUp");
        nonTrivialSysVec.push_back("dbtTagDown");
        nonTrivialSysVec.push_back("isrUp");
        nonTrivialSysVec.push_back("isrDown");

        for (auto nonTrivialSys : nonTrivialSysVec){

            if ( (TH1D*)f->Get(Form("S_dbtDiagUpLoose_%s", nonTrivialSys.c_str())) == NULL ) continue;
            h_[Form("S_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())] = (TH1D*)f->Get(Form("S_dbtDiagUpLoose_%s", nonTrivialSys.c_str()));
            h_[Form("UnD_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())] = (TH1D*)f->Get(Form("U_dbtDiagUpLoose_%s", nonTrivialSys.c_str()));
            h_[Form("UnD_tag_%s_%s", histoToUse.c_str(), nonTrivialSys.c_str())]->Add((TH1D*)f->Get(Form("D_dbtDiagUpLoose_%s", nonTrivialSys.c_str())));

        } // closes loop through nonTrivialSysVec

    } // closes loop through histoNameVec
}
