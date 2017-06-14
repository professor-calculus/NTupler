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


//RAL PARTICLE HEADERS
#include "../interface/PlottingTools.h"



/*
Executing:
$ root -q -b -l $CMSSW_BASE/src/NTupler/PATNTupler/macros/checkTtbarHtStitching.cc
*/


double getEventWeighting(TFile * f, double crossSection, double integratedLuminosity);
void htCutPlot(double lheHTCut, double integratedLuminosity, std::string outputDir, TTree * T_INC, TTree * T_HT600to800, TTree * T_HT800to1200, TTree * T_HT1200to2500, TTree * T_HT2500toInf, double weightingINC, double weightingHT600to800, double weightingHT800to1200, double weightingHT1200to2500, double weightingHT2500toInf);


void checkTtbarHtStitching(){
	
	double integratedLuminosity = 50.0;
	std::string outputDir = "/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/ttbarMcStitching/";

	TFile * fINC = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/TTJets_inclusiveHt/flatTree.root");
	TFile * fHT600to800 = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/TTJets_HT600to800/flatTree.root");
	TFile * fHT800to1200 = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/TTJets_HT800to1200/flatTree.root");
	TFile * fHT1200to2500 = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/TTJets_HT1200to2500/flatTree.root");
	TFile * fHT2500toInf = TFile::Open("/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/flatTrees_2017_04_22/TTJets_HT2500toInf/flatTree.root");

	TTree * T_INC = (TTree*)fINC->Get("doubleBFatJetPairTree");
	TTree * T_HT600to800 = (TTree*)fHT600to800->Get("doubleBFatJetPairTree");
	TTree * T_HT800to1200 = (TTree*)fHT800to1200->Get("doubleBFatJetPairTree");
	TTree * T_HT1200to2500 = (TTree*)fHT1200to2500->Get("doubleBFatJetPairTree");
	TTree * T_HT2500toInf = (TTree*)fHT2500toInf->Get("doubleBFatJetPairTree");

	double weightingINC = getEventWeighting(fINC, 831.76, integratedLuminosity);
	double weightingHT600to800 = getEventWeighting(fHT600to800, 2.6665344485, integratedLuminosity);
	double weightingHT800to1200 = getEventWeighting(fHT800to1200, 1.0980821984, integratedLuminosity);
	double weightingHT1200to2500 = getEventWeighting(fHT1200to2500, 0.1987479092, integratedLuminosity);
	double weightingHT2500toInf = getEventWeighting(fHT2500toInf, 0.002368412585, integratedLuminosity);

    TStyle * tdrStyle = PlottingTools::TDRStyle();
    tdrStyle->cd();
	TLatex * latex = new TLatex();
    latex->SetNDC();
    latex->SetTextFont(42);

	htCutPlot(-1.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(0.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(50.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(100.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(150.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(200.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(250.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(300.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(350.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(400.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(450.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(500.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(550.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(600.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(650.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(700.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(750.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	htCutPlot(800.0, integratedLuminosity, outputDir, T_INC, T_HT600to800, T_HT800to1200, T_HT1200to2500, T_HT2500toInf, weightingINC, weightingHT600to800, weightingHT800to1200, weightingHT1200to2500, weightingHT2500toInf);
	/////////////////////////////////////////////
	// PLOT OF LHE HT
	{
		TH1F  h = TH1F("h", "", 100, 0, 3000);
		TH1F * hINC = new TH1F("hINC", ";LHE H_{T} (GeV); events / bin", 100, 0, 3000);
		TH1F * hLheHT600to800 = new TH1F("hLheHT600to800", ";LHE H_{T} (GeV); events / bin", 100, 0, 3000);
		TH1F * hLheHT800to1200 = new TH1F("hLheHT800to1200", ";LHE H_{T} (GeV); events / bin", 100, 0, 3000);
		TH1F * hLheHT1200to2500 = new TH1F("hLheHT1200to2500", ";LHE H_{T} (GeV); events / bin", 100, 0, 3000);
		TH1F * hLheHT2500toInf = new TH1F("hLheHT2500toInf", ";LHE H_{T} (GeV); events / bin", 100, 0, 3000);
		
		T_INC->Draw("lheHT>>h", Form("%f",weightingINC), "");
		for (int iBin = 0; iBin < h.GetNbinsX()+2; ++iBin) hINC->AddBinContent(iBin, h.GetBinContent(iBin));
		T_HT600to800->Draw("lheHT>>h", Form("%f",weightingHT600to800), "");
		for (int iBin = 0; iBin < h.GetNbinsX()+2; ++iBin) hLheHT600to800->AddBinContent(iBin, h.GetBinContent(iBin));
		T_HT800to1200->Draw("lheHT>>h", Form("%f",weightingHT800to1200), "");
		for (int iBin = 0; iBin < h.GetNbinsX()+2; ++iBin) hLheHT800to1200->AddBinContent(iBin, h.GetBinContent(iBin));
		T_HT1200to2500->Draw("lheHT>>h", Form("%f",weightingHT1200to2500), "");
		for (int iBin = 0; iBin < h.GetNbinsX()+2; ++iBin) hLheHT1200to2500->AddBinContent(iBin, h.GetBinContent(iBin));
		T_HT2500toInf->Draw("lheHT>>h", Form("%f",weightingHT2500toInf), "");
		for (int iBin = 0; iBin < h.GetNbinsX()+2; ++iBin) hLheHT2500toInf->AddBinContent(iBin, h.GetBinContent(iBin));
		
		TCanvas* c1 =new TCanvas("c1","c1"); 

		hINC->SetLineColor(kBlue);
		hINC->Draw();

		hLheHT600to800->SetFillColor(kRed);
		hLheHT600to800->SetLineWidth(0);
		hLheHT600to800->SetMarkerSize(2);

		hLheHT800to1200->SetFillColor(kGreen+1);
		hLheHT800to1200->SetLineWidth(0);
		hLheHT800to1200->SetMarkerSize(2);

		hLheHT1200to2500->SetFillColor(kMagenta);
		hLheHT1200to2500->SetLineWidth(0);
		hLheHT1200to2500->SetMarkerSize(2);

		hLheHT2500toInf->SetFillColor(kYellow);
		hLheHT2500toInf->SetLineWidth(0);
		hLheHT2500toInf->SetMarkerSize(2);

		THStack * hs1 = new THStack("hs1","");
		hs1->Add(hLheHT2500toInf);
		hs1->Add(hLheHT1200to2500);
		hs1->Add(hLheHT800to1200);
		hs1->Add(hLheHT600to800);
		hs1->Draw("same");
		hINC->Draw("same");
		gPad->RedrawAxis();

		latex->SetTextAlign(11); // align from left
		latex->DrawLatex(0.15,0.92,"#bf{CMS} #it{Simulation} W.I.P");
		latex->SetTextAlign(31); // align from right
		latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));

	    TLegend * legend1 = new TLegend();
	    legend1->SetTextSize(0.04);
	    legend1->SetX1NDC(0.56);
	    legend1->SetX2NDC(0.87);
	    legend1->SetY1NDC(0.57);
	    legend1->SetY2NDC(0.87);
	    legend1->AddEntry(hINC, "ttbar_INCLUSIVE", "L");
	    legend1->AddEntry(hLheHT600to800, "ttbar_HT600to800", "f");
		legend1->AddEntry(hLheHT800to1200, "ttbar_HT800to1200", "f");
		legend1->AddEntry(hLheHT1200to2500, "ttbar_HT1200to2500", "f");
		legend1->AddEntry(hLheHT2500toInf, "ttbar_HT2500toInf", "f");
		legend1->Draw("same");

		// c1->Update();
		// TLine *line = new TLine(600,0,600,1e10);
		// line->SetLineStyle(2);
		// line->SetLineWidth(3);
		// line->Draw();

		c1->SetLogy();
		c1->SaveAs(Form("%s/ttbar_lheHT.pdf", outputDir.c_str()));
		c1->Close();
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////
}


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


void htCutPlot(double lheHTCut, double integratedLuminosity, std::string outputDir, TTree * T_INC, TTree * T_HT600to800, TTree * T_HT800to1200, TTree * T_HT1200to2500, TTree * T_HT2500toInf, double weightingINC, double weightingHT600to800, double weightingHT800to1200, double weightingHT1200to2500, double weightingHT2500toInf)
{
	TLatex * latex = new TLatex();
    latex->SetNDC();
    latex->SetTextFont(42);

	TH1F  h2 = TH1F("h2", "", 100, 0, 4000);
	TH1F * hINCnocut = new TH1F("hINCnocut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
	TH1F * hINCwcut = new TH1F("hINCwcut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
	TH1F * hHT600to800 = new TH1F("hHT600to800", ";H_{T}(GeV); events / bin", 100, 0, 4000);
	TH1F * hHT800to1200 = new TH1F("hHT800to1200", ";H_{T}(GeV); events / bin", 100, 0, 4000);
	TH1F * hHT1200to2500 = new TH1F("hHT1200to2500", ";H_{T}(GeV); events / bin", 100, 0, 4000);
	TH1F * hHT2500toInf = new TH1F("hHT2500toInf", ";H_{T}(GeV); events / bin", 100, 0, 4000);
	
	T_INC->Draw("ht>>h2", Form("%f",weightingINC), "");
	for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCnocut->AddBinContent(iBin, h2.GetBinContent(iBin));
	T_INC->Draw("ht>>h2", Form("%f*(lheHT<%f)",weightingINC,lheHTCut), "");
	for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCwcut->AddBinContent(iBin, h2.GetBinContent(iBin));
	T_HT600to800->Draw("ht>>h2", Form("%f*(lheHT>=%f)",weightingHT600to800,lheHTCut), "");
	for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT600to800->AddBinContent(iBin, h2.GetBinContent(iBin));
	T_HT800to1200->Draw("ht>>h2", Form("%f*(lheHT>=%f)",weightingHT800to1200,lheHTCut), "");
	for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT800to1200->AddBinContent(iBin, h2.GetBinContent(iBin));
	T_HT1200to2500->Draw("ht>>h2", Form("%f*(lheHT>=%f)",weightingHT1200to2500,lheHTCut), "");
	for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT1200to2500->AddBinContent(iBin, h2.GetBinContent(iBin));
	T_HT2500toInf->Draw("ht>>h2", Form("%f*(lheHT>=%f)",weightingHT2500toInf,lheHTCut), "");
	for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT2500toInf->AddBinContent(iBin, h2.GetBinContent(iBin));

	TCanvas* c2 =new TCanvas("c2","c2"); 

	hINCnocut->SetLineColor(kBlue);
	hINCnocut->Draw();

	hINCwcut->SetFillColor(kCyan);
	hINCwcut->SetLineWidth(0);
	hINCwcut->SetMarkerSize(2);

	hHT600to800->SetFillColor(kRed);
	hHT600to800->SetLineWidth(0);
	hHT600to800->SetMarkerSize(2);

	hHT800to1200->SetFillColor(kGreen+1);
	hHT800to1200->SetLineWidth(0);
	hHT800to1200->SetMarkerSize(2);

	hHT1200to2500->SetFillColor(kMagenta);
	hHT1200to2500->SetLineWidth(0);
	hHT1200to2500->SetMarkerSize(2);

	hHT2500toInf->SetFillColor(kYellow);
	hHT2500toInf->SetLineWidth(0);
	hHT2500toInf->SetMarkerSize(2);

	THStack * hs2 = new THStack("hs2","");
	hs2->Add(hHT2500toInf);
	hs2->Add(hHT1200to2500);
	hs2->Add(hHT800to1200);
	hs2->Add(hHT600to800);
	hs2->Add(hINCwcut);
	hs2->Draw("same");
	hINCnocut->Draw("same");
	gPad->RedrawAxis();

	latex->SetTextAlign(11); // align from left
	latex->DrawLatex(0.15,0.92,"#bf{CMS} #it{Simulation} W.I.P");
	latex->SetTextAlign(31); // align from right
	latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));

    TLegend * legend2 = new TLegend();
    legend2->SetTextSize(0.025);
    legend2->SetX1NDC(0.56);
    legend2->SetX2NDC(0.87);
    legend2->SetY1NDC(0.57);
    legend2->SetY2NDC(0.87);
    legend2->AddEntry(hINCnocut, "ttbar_INCLUSIVE", "L");
    legend2->AddEntry(hINCwcut, Form("ttbar_INCLUSIVE (lheHT < %.0f)",lheHTCut), "f");
    legend2->AddEntry(hHT600to800, Form("ttbar_HT600to800 (lheHT > %.0f)",lheHTCut), "f");
	legend2->AddEntry(hHT800to1200, Form("ttbar_HT800to1200 (lheHT > %.0f)",lheHTCut), "f");
	legend2->AddEntry(hHT1200to2500, Form("ttbar_HT1200to2500 (lheHT > %.0f)",lheHTCut), "f");
	legend2->AddEntry(hHT2500toInf, Form("ttbar_HT2500toInf (lheHT > %.0f)",lheHTCut), "f");
	legend2->Draw("same");

	c2->SetLogy();
	c2->SaveAs(Form("%s/ttbar_HT_LheHtCut%.0f.pdf", outputDir.c_str(), lheHTCut));
	c2->Close();
	delete hINCnocut;
	delete hINCwcut;
	delete hHT600to800;
	delete hHT800to1200;
	delete hHT1200to2500;
	delete hHT2500toInf;
}