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


void checkTtbarHtStitching(){
	
	double integratedLuminosity = 50.0;
	std::string outputDir = "~";

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


	/////////////////////////////////////////////
	// PLOT OF LHE HT
	{
		TH1F  h = TH1F("h", "", 100, 0, 3000);
		TH1F * hINC = new TH1F("hINC", ";recalc lhe H_{T} (GeV); events / bin", 100, 0, 3000);
		TH1F * hLheHT600to800 = new TH1F("hLheHT600to800", ";recalc lhe H_{T} (GeV); events / bin", 100, 0, 3000);
		TH1F * hLheHT800to1200 = new TH1F("hLheHT800to1200", ";recalc lhe H_{T} (GeV); events / bin", 100, 0, 3000);
		TH1F * hLheHT1200to2500 = new TH1F("hLheHT1200to2500", ";recalc lhe H_{T} (GeV); events / bin", 100, 0, 3000);
		TH1F * hLheHT2500toInf = new TH1F("hLheHT2500toInf", ";recalc lhe H_{T} (GeV); events / bin", 100, 0, 3000);
		
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

		hLheHT2500toInf->SetFillColor(kBlack);
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
	    legend1->AddEntry(hLheHT600to800, "ttbar_lheHT600to800", "f");
		legend1->AddEntry(hLheHT800to1200, "ttbar_lheHT800to1200", "f");
		legend1->AddEntry(hLheHT1200to2500, "ttbar_lheHT1200to2500", "f");
		legend1->AddEntry(hLheHT2500toInf, "ttbar_lheHT2500toInf", "f");
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


	/////////////////////////////////////////////
	// PLOT OF HT
	{
		TH1F  h2 = TH1F("h2", "", 100, 0, 4000);
		TH1F * hINCnocut = new TH1F("hINCnocut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
		TH1F * hHT600to800 = new TH1F("hHT600to800", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT800to1200 = new TH1F("hHT800to1200", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT1200to2500 = new TH1F("hHT1200to2500", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT2500toInf = new TH1F("hHT2500toInf", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		
		T_INC->Draw("ht>>h2", Form("%f",weightingINC), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCnocut->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT600to800->Draw("ht>>h2", Form("%f*(lheHT<600)",weightingINC), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT600to800->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT800to1200->Draw("ht>>h2", Form("%f*(lheHT>600)",weightingHT800to1200), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT800to1200->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT1200to2500->Draw("ht>>h2", Form("%f*(lheHT>600)",weightingHT1200to2500), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT1200to2500->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT2500toInf->Draw("ht>>h2", Form("%f*(lheHT>600)",weightingHT2500toInf), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT2500toInf->AddBinContent(iBin, h2.GetBinContent(iBin));

		TCanvas* c2 =new TCanvas("c2","c2"); 

		hINCnocut->SetLineColor(kBlue);
		hINCnocut->Draw();

		hHT600to800->SetFillColor(kRed);
		hHT600to800->SetLineWidth(0);
		hHT600to800->SetMarkerSize(2);

		hHT800to1200->SetFillColor(kGreen+1);
		hHT800to1200->SetLineWidth(0);
		hHT800to1200->SetMarkerSize(2);

		hHT1200to2500->SetFillColor(kMagenta);
		hHT1200to2500->SetLineWidth(0);
		hHT1200to2500->SetMarkerSize(2);

		hHT2500toInf->SetFillColor(kBlack);
		hHT2500toInf->SetLineWidth(0);
		hHT2500toInf->SetMarkerSize(2);

		THStack * hs2 = new THStack("hs2","");
		hs2->Add(hHT2500toInf);
		hs2->Add(hHT1200to2500);
		hs2->Add(hHT800to1200);
		hs2->Add(hHT600to800);
		hs2->Draw("same");
		hINCnocut->Draw("same");
		gPad->RedrawAxis();

		latex->SetTextAlign(11); // align from left
		latex->DrawLatex(0.15,0.92,"#bf{CMS} #it{Simulation} W.I.P");
		latex->SetTextAlign(31); // align from right
		latex->DrawLatex(0.92,0.92,Form("%.1f fb^{-1} (13 TeV)", integratedLuminosity));

	    TLegend * legend2 = new TLegend();
	    legend2->SetTextSize(0.04);
	    legend2->SetX1NDC(0.56);
	    legend2->SetX2NDC(0.87);
	    legend2->SetY1NDC(0.57);
	    legend2->SetY2NDC(0.87);
	    legend2->AddEntry(hINCnocut, "ttbar_INCLUSIVE", "L");
	    legend2->AddEntry(hHT600to800, "ttbar_lheHT600to800", "f");
		legend2->AddEntry(hHT800to1200, "ttbar_lheHT800to1200", "f");
		legend2->AddEntry(hHT1200to2500, "ttbar_lheHT1200to2500", "f");
		legend2->AddEntry(hHT2500toInf, "ttbar_lheHT2500toInf", "f");
		legend2->Draw("same");

		c2->SetLogy();
		c2->SaveAs(Form("%s/ttbar_HT.pdf", outputDir.c_str()));
		c2->Close();
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////


	/////////////////////////////////////////////
	// PLOT OF HT - lhe ht cut 600
	{
		TH1F  h2 = TH1F("h2", "", 100, 0, 4000);
		TH1F * hINCnocut = new TH1F("hINCnocut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
		TH1F * hINCwcut = new TH1F("hINCwcut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
		TH1F * hHT600to800 = new TH1F("hHT600to800", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT800to1200 = new TH1F("hHT800to1200", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT1200to2500 = new TH1F("hHT1200to2500", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT2500toInf = new TH1F("hHT2500toInf", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		
		T_INC->Draw("ht>>h2", Form("%f",weightingINC), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCnocut->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_INC->Draw("ht>>h2", Form("%f*(lheHT<600)",weightingINC), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCwcut->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT600to800->Draw("ht>>h2", Form("%f*(lheHT>600)",weightingHT600to800), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT600to800->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT800to1200->Draw("ht>>h2", Form("%f*(lheHT>600)",weightingHT800to1200), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT800to1200->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT1200to2500->Draw("ht>>h2", Form("%f*(lheHT>600)",weightingHT1200to2500), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT1200to2500->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT2500toInf->Draw("ht>>h2", Form("%f*(lheHT>600)",weightingHT2500toInf), "");
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

		hHT2500toInf->SetFillColor(kBlack);
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
	    legend2->SetTextSize(0.021);
	    legend2->SetX1NDC(0.56);
	    legend2->SetX2NDC(0.87);
	    legend2->SetY1NDC(0.57);
	    legend2->SetY2NDC(0.87);
	    legend2->AddEntry(hINCnocut, "ttbar_INCLUSIVE", "L");
	    legend2->AddEntry(hINCwcut, "ttbar_INCLUSIVE (recalc_lheHT < 600)", "f");
	    legend2->AddEntry(hHT600to800, "ttbar_lheHT600to800 (recalc_lheHT > 600)", "f");
		legend2->AddEntry(hHT800to1200, "ttbar_lheHT800to1200 (recalc_lheHT > 600)", "f");
		legend2->AddEntry(hHT1200to2500, "ttbar_lheHT1200to2500 (recalc_lheHT > 600)", "f");
		legend2->AddEntry(hHT2500toInf, "ttbar_lheHT2500toInf (recalc_lheHT > 600)", "f");
		legend2->Draw("same");

		c2->SetLogy();
		c2->SaveAs(Form("%s/ttbar_HT_recalcLheHtCut600.pdf", outputDir.c_str()));
		c2->Close();
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// PLOT OF HT - lhe ht cut 800
	{
		TH1F  h2 = TH1F("h2", "", 100, 0, 4000);
		TH1F * hINCnocut = new TH1F("hINCnocut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
		TH1F * hINCwcut = new TH1F("hINCwcut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
		TH1F * hHT600to800 = new TH1F("hHT600to800", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT800to1200 = new TH1F("hHT800to1200", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT1200to2500 = new TH1F("hHT1200to2500", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT2500toInf = new TH1F("hHT2500toInf", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		
		T_INC->Draw("ht>>h2", Form("%f",weightingINC), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCnocut->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_INC->Draw("ht>>h2", Form("%f*(lheHT<800)",weightingINC), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCwcut->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT600to800->Draw("ht>>h2", Form("%f*(lheHT>800)",weightingHT600to800), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT600to800->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT800to1200->Draw("ht>>h2", Form("%f*(lheHT>800)",weightingHT800to1200), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT800to1200->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT1200to2500->Draw("ht>>h2", Form("%f*(lheHT>800)",weightingHT1200to2500), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT1200to2500->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT2500toInf->Draw("ht>>h2", Form("%f*(lheHT>800)",weightingHT2500toInf), "");
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

		hHT2500toInf->SetFillColor(kBlack);
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
	    legend2->SetTextSize(0.021);
	    legend2->SetX1NDC(0.56);
	    legend2->SetX2NDC(0.87);
	    legend2->SetY1NDC(0.57);
	    legend2->SetY2NDC(0.87);
	    legend2->AddEntry(hINCnocut, "ttbar_INCLUSIVE", "L");
	    legend2->AddEntry(hINCwcut, "ttbar_INCLUSIVE (recalc_lheHT < 800)", "f");
	    legend2->AddEntry(hHT600to800, "ttbar_lheHT600to800 (recalc_lheHT > 800)", "f");
		legend2->AddEntry(hHT800to1200, "ttbar_lheHT800to1200 (recalc_lheHT > 800)", "f");
		legend2->AddEntry(hHT1200to2500, "ttbar_lheHT1200to2500 (recalc_lheHT > 800)", "f");
		legend2->AddEntry(hHT2500toInf, "ttbar_lheHT2500toInf (recalc_lheHT > 800)", "f");
		legend2->Draw("same");

		c2->SetLogy();
		c2->SaveAs(Form("%s/ttbar_HT_recalcLheHtCut800.pdf", outputDir.c_str()));
		c2->Close();
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////


	/////////////////////////////////////////////
	// PLOT OF HT - lhe ht cut 1000
	{
		TH1F  h2 = TH1F("h2", "", 100, 0, 4000);
		TH1F * hINCnocut = new TH1F("hINCnocut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
		TH1F * hINCwcut = new TH1F("hINCwcut", ";H_{T} (Gev); events / bin", 100, 0, 4000);
		TH1F * hHT600to800 = new TH1F("hHT600to800", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT800to1200 = new TH1F("hHT800to1200", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT1200to2500 = new TH1F("hHT1200to2500", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		TH1F * hHT2500toInf = new TH1F("hHT2500toInf", ";H_{T}(GeV); events / bin", 100, 0, 4000);
		
		T_INC->Draw("ht>>h2", Form("%f",weightingINC), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCnocut->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_INC->Draw("ht>>h2", Form("%f*(lheHT<1000)",weightingINC), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hINCwcut->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT600to800->Draw("ht>>h2", Form("%f*(lheHT>1000)",weightingHT600to800), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT600to800->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT800to1200->Draw("ht>>h2", Form("%f*(lheHT>1000)",weightingHT800to1200), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT800to1200->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT1200to2500->Draw("ht>>h2", Form("%f*(lheHT>1000)",weightingHT1200to2500), "");
		for (int iBin = 0; iBin < h2.GetNbinsX()+2; ++iBin) hHT1200to2500->AddBinContent(iBin, h2.GetBinContent(iBin));
		T_HT2500toInf->Draw("ht>>h2", Form("%f*(lheHT>1000)",weightingHT2500toInf), "");
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

		hHT2500toInf->SetFillColor(kBlack);
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
	    legend2->SetTextSize(0.021);
	    legend2->SetX1NDC(0.56);
	    legend2->SetX2NDC(0.87);
	    legend2->SetY1NDC(0.57);
	    legend2->SetY2NDC(0.87);
	    legend2->AddEntry(hINCnocut, "ttbar_INCLUSIVE", "L");
	    legend2->AddEntry(hINCwcut, "ttbar_INCLUSIVE (recalc_lheHT < 1000)", "f");
	    legend2->AddEntry(hHT600to800, "ttbar_lheHT600to800 (recalc_lheHT > 1000)", "f");
		legend2->AddEntry(hHT800to1200, "ttbar_lheHT800to1200 (recalc_lheHT > 1000)", "f");
		legend2->AddEntry(hHT1200to2500, "ttbar_lheHT1200to2500 (recalc_lheHT > 1000)", "f");
		legend2->AddEntry(hHT2500toInf, "ttbar_lheHT2500toInf (recalc_lheHT > 1000)", "f");
		legend2->Draw("same");

		c2->SetLogy();
		c2->SaveAs(Form("%s/ttbar_HT_recalcLheHtCut1000.pdf", outputDir.c_str()));
		c2->Close();
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////

}