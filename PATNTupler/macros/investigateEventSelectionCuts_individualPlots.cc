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
#include "../interface/PlottingTools.h"



/*
Executing:
$ root -q -b -l $CMSSW_BASE/src/NTupler/PATNTupler/macros/investigateEventSelectionCuts_individualPlots.cc
*/



double getEventWeighting(TFile * f, double crossSection, double integratedLuminosity);
void investigateEventSelectionCuts_individualPlots(){
	
	double integratedLuminosity = 50.0;
	std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/output_InvestigateEventSelectionCutsV2/individualPlots/";

	TFile * f_ttbar = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/TTJets_inclusiveHt/flatTree.root");
	TFile * f_mH30mSusy1600 = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/mH30p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root");
	TFile * f_mH50mSusy1600 = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/mH50p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root");
	TFile * f_mH70mSusy1600 = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/mH70p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root");
	TFile * f_mH90mSusy1600 = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/mH90p0_mSusy1600p0_ratio0p99_splitting0p1/flatTree.root");
	TFile * f_mH70mSusy2000 = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/mH70p0_mSusy2000p0_ratio0p99_splitting0p1/flatTree.root");

	TTree * T_ttbar = (TTree*)f_ttbar->Get("doubleBFatJetPairTree");
	TTree * T_mH30mSusy1600 = (TTree*)f_mH30mSusy1600->Get("doubleBFatJetPairTree");
	TTree * T_mH50mSusy1600 = (TTree*)f_mH50mSusy1600->Get("doubleBFatJetPairTree");
	TTree * T_mH70mSusy1600 = (TTree*)f_mH70mSusy1600->Get("doubleBFatJetPairTree");
	TTree * T_mH90mSusy1600 = (TTree*)f_mH90mSusy1600->Get("doubleBFatJetPairTree");
	TTree * T_mH70mSusy2000 = (TTree*)f_mH70mSusy2000->Get("doubleBFatJetPairTree");

	double weighting_ttbar = getEventWeighting(f_ttbar, 831.76, integratedLuminosity);
	double weighting_signal1600 = getEventWeighting(f_mH70mSusy1600, 0.060, integratedLuminosity);
	double weighting_signal2000 = getEventWeighting(f_mH70mSusy2000, 0.009, integratedLuminosity);

    TStyle * tdrStyle = PlottingTools::TDRStyle();
    tdrStyle->cd();
	TLatex * latex = new TLatex();
    latex->SetNDC();
    latex->SetTextFont(42);

    std::system(Form("cp $CMSSW_BASE/src/NTupler/PATNTupler/macros/investigateEventSelectionCuts_individualPlots.cc %s",outputDir.c_str()));


	/////////////////////////////////////////////
	// PLOT OF TTBAR MASS
	{
		TH1F  h = TH1F("h", "", 50, -8, 200);
		TH1F * h_ttbarSoftDrop = new TH1F("h_ttbarSoftDrop", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 200);
		TH1F * h_ttbarPruned = new TH1F("h_ttbarPruned", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 200);

		T_ttbar->Draw("fatJetA_softDropMass>>h", Form("%f",weighting_ttbar), "");
		for (int iBin = 0; iBin < h.GetNbinsX()+2; ++iBin) h_ttbarSoftDrop->AddBinContent(iBin, h.GetBinContent(iBin));
		T_ttbar->Draw("fatJetA_prunedMass>>h", Form("%f",weighting_ttbar), "");
		for (int iBin = 0; iBin < h.GetNbinsX()+2; ++iBin) h_ttbarPruned->AddBinContent(iBin, h.GetBinContent(iBin));

		TCanvas* c1 =new TCanvas("c1","c1"); 

		h_ttbarSoftDrop->SetLineColor(kBlue);
		h_ttbarSoftDrop->SetLineWidth(2);
		h_ttbarSoftDrop->Draw();

		h_ttbarPruned->SetLineColor(kRed);
		h_ttbarPruned->SetLineWidth(2);
		// h_ttbarPruned->SetLineStyle(7);
		h_ttbarPruned->Draw("same");

		latex->SetTextAlign(11); // align from left
		latex->DrawLatex(0.25,0.92,"#bf{CMS} #it{Simulation} W.I.P");
		latex->SetTextAlign(31); // align from right
		latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));

	    TLegend * legend1 = new TLegend();
	    legend1->SetTextSize(0.05);
	    legend1->SetX1NDC(0.56);
	    legend1->SetX2NDC(0.87);
	    legend1->SetY1NDC(0.67);
	    legend1->SetY2NDC(0.87);
	    legend1->AddEntry(h_ttbarSoftDrop, "SoftDrop", "L");
	    legend1->AddEntry(h_ttbarPruned, "Pruned", "L");
		legend1->Draw("same");

		c1->SaveAs(Form("%s/ttbar_massRecoType.pdf", outputDir.c_str()));
		c1->Close();
	}
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// PLOT SIGNAL MASS
	{
		TH1F  h2 = TH1F("h2", "", 50, -8, 150);
		TH1F * h_mH30mSusy1600_softDrop = new TH1F("h_mH30mSusy1600_softDrop", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 150);
		TH1F * h_mH50mSusy1600_softDrop = new TH1F("h_mH50mSusy1600_softDrop", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 150);
		TH1F * h_mH70mSusy1600_softDrop = new TH1F("h_mH70mSusy1600_softDrop", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 150);
		TH1F * h_mH90mSusy1600_softDrop = new TH1F("h_mH90mSusy1600_softDrop", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 150);
		TH1F * h_mH30mSusy1600_pruned = new TH1F("h_mH30mSusy1600_pruned", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 150);
		TH1F * h_mH50mSusy1600_pruned = new TH1F("h_mH50mSusy1600_pruned", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 150);
		TH1F * h_mH70mSusy1600_pruned = new TH1F("h_mH70mSusy1600_pruned", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 150);
		TH1F * h_mH90mSusy1600_pruned = new TH1F("h_mH90mSusy1600_pruned", ";LeadingBDiscFatJet_Mass (GeV); events / bin", 50, -8, 150);

		T_mH30mSusy1600->Draw("fatJetA_softDropMass>>h2", Form("%f",weighting_signal1600), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) h_mH30mSusy1600_softDrop->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_mH50mSusy1600->Draw("fatJetA_softDropMass>>h2", Form("%f",weighting_signal1600), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) h_mH50mSusy1600_softDrop->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_mH70mSusy1600->Draw("fatJetA_softDropMass>>h2", Form("%f",weighting_signal1600), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) h_mH70mSusy1600_softDrop->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_mH90mSusy1600->Draw("fatJetA_softDropMass>>h2", Form("%f",weighting_signal1600), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) h_mH90mSusy1600_softDrop->AddBinContent(iBin, h2.GetBinContent(iBin));

		T_mH30mSusy1600->Draw("fatJetA_prunedMass>>h2", Form("%f",weighting_signal1600), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) h_mH30mSusy1600_pruned->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_mH50mSusy1600->Draw("fatJetA_prunedMass>>h2", Form("%f",weighting_signal1600), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) h_mH50mSusy1600_pruned->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_mH70mSusy1600->Draw("fatJetA_prunedMass>>h2", Form("%f",weighting_signal1600), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) h_mH70mSusy1600_pruned->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_mH90mSusy1600->Draw("fatJetA_prunedMass>>h2", Form("%f",weighting_signal1600), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) h_mH90mSusy1600_pruned->AddBinContent(iBin, h2.GetBinContent(iBin));

		TCanvas* c2 =new TCanvas("c2","c2"); 

		h_mH30mSusy1600_softDrop->SetLineColor(kRed);
		h_mH50mSusy1600_softDrop->SetLineColor(kGreen);
		h_mH70mSusy1600_softDrop->SetLineColor(kMagenta);
		h_mH90mSusy1600_softDrop->SetLineColor(kBlack);
		h_mH30mSusy1600_pruned->SetLineColor(kRed);
		h_mH50mSusy1600_pruned->SetLineColor(kGreen);
		h_mH70mSusy1600_pruned->SetLineColor(kMagenta);
		h_mH90mSusy1600_pruned->SetLineColor(kBlack);

		h_mH30mSusy1600_softDrop->SetLineWidth(2);
		h_mH50mSusy1600_softDrop->SetLineWidth(2);
		h_mH70mSusy1600_softDrop->SetLineWidth(2);
		h_mH90mSusy1600_softDrop->SetLineWidth(2);
		h_mH30mSusy1600_pruned->SetLineWidth(3);
		h_mH50mSusy1600_pruned->SetLineWidth(3);
		h_mH70mSusy1600_pruned->SetLineWidth(3);
		h_mH90mSusy1600_pruned->SetLineWidth(3);

		h_mH30mSusy1600_pruned->SetLineStyle(2);
		h_mH50mSusy1600_pruned->SetLineStyle(2);
		h_mH70mSusy1600_pruned->SetLineStyle(2);
		h_mH90mSusy1600_pruned->SetLineStyle(2);

		h_mH30mSusy1600_pruned->Draw();
		h_mH50mSusy1600_pruned->Draw("same");
		h_mH70mSusy1600_pruned->Draw("same");
		h_mH90mSusy1600_pruned->Draw("same");
		h_mH30mSusy1600_softDrop->Draw("same");
		h_mH50mSusy1600_softDrop->Draw("same");
		h_mH70mSusy1600_softDrop->Draw("same");
		h_mH90mSusy1600_softDrop->Draw("same");

		latex->SetTextAlign(11); // align from left
		latex->DrawLatex(0.15,0.92,"#bf{CMS} #it{Simulation} W.I.P");
		latex->SetTextAlign(31); // align from right
		latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));

	    TLegend * legend1 = new TLegend();
	    legend1->SetTextSize(0.035);
	    legend1->SetX1NDC(0.53);
	    legend1->SetX2NDC(0.89);
	    legend1->SetY1NDC(0.61);
	    legend1->SetY2NDC(0.88);
	    legend1->AddEntry(h_mH30mSusy1600_pruned, "m_{H}30_m_{susy}1600_Pruned", "L");
	    legend1->AddEntry(h_mH30mSusy1600_softDrop, "m_{H}30_m_{susy}1600_SoftDrop", "L");
	    legend1->AddEntry(h_mH50mSusy1600_pruned, "m_{H}50_m_{susy}1600_Pruned", "L");
	    legend1->AddEntry(h_mH50mSusy1600_softDrop, "m_{H}50_m_{susy}1600_SoftDrop", "L");
	    legend1->AddEntry(h_mH70mSusy1600_pruned, "m_{H}70_m_{susy}1600_Pruned", "L");
	    legend1->AddEntry(h_mH70mSusy1600_softDrop, "m_{H}70_m_{susy}1600_SoftDrop", "L");
	    legend1->AddEntry(h_mH90mSusy1600_pruned, "m_{H}90_m_{susy}1600_Pruned", "L");
	    legend1->AddEntry(h_mH90mSusy1600_softDrop, "m_{H}90_m_{susy}1600_SoftDrop", "L");
		legend1->Draw("same");

		c2->SaveAs(Form("%s/signal1600_massRecoType.pdf", outputDir.c_str()));
		c2->Close();
	}
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// PLOT SIGNAL jet pt's
	{
		double defaultParam_GetPadRightMargin = tdrStyle->GetPadRightMargin();
		double defaultParam_GetPadLeftMargin = tdrStyle->GetPadLeftMargin();
		tdrStyle->SetPadRightMargin(0.15);
		tdrStyle->SetPadLeftMargin(0.15);

		TH2F * h_mH70mSusy2000_AK8vsAK8 = new TH2F("h_mH70mSusy2000_AK8vsAK8", ";LeadingBDiscFatJet p_{T} (GeV); SecondaryBDiscFatJet p_{T} (GeV)", 300, 0, 3000, 300, 0, 3000);
		T_mH70mSusy2000->Draw("fatJetA_p4.Pt():fatJetB_p4.Pt()>>h_mH70mSusy2000_AK8vsAK8", Form("%f",weighting_signal2000), "colz");
		TCanvas* c3 =new TCanvas("c3","c3"); 
		h_mH70mSusy2000_AK8vsAK8->Draw("colz");
		latex->SetTextAlign(11); // align from left
		latex->DrawLatex(0.15,0.92,"#bf{CMS} #it{Simulation} W.I.P");
		latex->SetTextAlign(31); // align from right
		latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));
		// gPad->SetLogz();
		c3->SaveAs(Form("%s/signal2000_AK8vsAK8.pdf", outputDir.c_str()));
		c3->Close();

		TH2F * h_mH70mSusy2000_AK4vsAK4 = new TH2F("h_mH70mSusy2000_AK4vsAK4", ";SecondaryPtAK4Jet p_{T} (GeV);LeadingPtAK4Jet p_{T} (GeV)", 300, 0, 2500, 300, 0, 2500);
		T_mH70mSusy2000->Draw("slimJetA_p4.Pt():slimJetB_p4.Pt()>>h_mH70mSusy2000_AK4vsAK4", Form("%f",weighting_signal2000), "colz");
		TCanvas* c4 =new TCanvas("c4","c4"); 
		h_mH70mSusy2000_AK4vsAK4->Draw("colz");
		latex->SetTextAlign(11); // align from left
		latex->DrawLatex(0.15,0.92,"#bf{CMS} #it{Simulation} W.I.P");
		latex->SetTextAlign(31); // align from right
		latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));
		gPad->SetLogz();
		c4->SaveAs(Form("%s/signal2000_AK4vsAK4.pdf", outputDir.c_str()));
		c4->Close();

		TH2F * h_mH70mSusy2000_AK8vsAK4 = new TH2F("h_mH70mSusy2000_AK8vsAK4", ";SecondaryPtAK4Jet p_{T} (GeV);SecondaryPtFatJet p_{T} (GeV)", 300, 0, 2500, 300, 0, 2500);
		T_mH70mSusy2000->Draw("fatJetA_p4.Pt():slimJetB_p4.Pt()>>h_mH70mSusy2000_AK8vsAK4", Form("%f*(fatJetA_p4.Pt() < fatJetB_p4.Pt())",weighting_signal2000), "colz");
		T_mH70mSusy2000->Draw("fatJetB_p4.Pt():slimJetB_p4.Pt()>>h_mH70mSusy2000_AK8vsAK4", Form("%f*(fatJetB_p4.Pt() < fatJetA_p4.Pt())",weighting_signal2000), "colz");
		TCanvas* c5 =new TCanvas("c5","c5"); 
		h_mH70mSusy2000_AK8vsAK4->Draw("colz");
		latex->SetTextAlign(11); // align from left
		latex->DrawLatex(0.15,0.92,"#bf{CMS} #it{Simulation} W.I.P");
		latex->SetTextAlign(31); // align from right
		latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));
		// gPad->SetLogz();
		c5->SaveAs(Form("%s/signal2000_AK8vsAK4.pdf", outputDir.c_str()));
		c5->Close();

		tdrStyle->SetPadRightMargin(defaultParam_GetPadRightMargin);
		tdrStyle->SetPadRightMargin(defaultParam_GetPadLeftMargin);
	}
	/////////////////////////////////////////////


} // closes function 'investigateEventSelectionCuts_individualPlots'


double getEventWeighting(TFile * f, double crossSection, double integratedLuminosity)
{
	TTree * evT = (TTree*)f->Get("eventCountTree");
    Int_t evTEntries = (Int_t)evT->GetEntries();
    UInt_t nEvtsRunOverForEntry;
    evT->SetBranchAddress("nEvtsRunOver", &nEvtsRunOverForEntry);
    UInt_t nEvtsRunOverTotal = 0;
    for (Int_t ievT=0; ievT<evTEntries; ++ievT){
        evT->GetEntry(ievT);
        nEvtsRunOverTotal += nEvtsRunOverForEntry;
    }
    return 1000.0 * crossSection * integratedLuminosity / nEvtsRunOverTotal;
}