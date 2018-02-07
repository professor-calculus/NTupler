#ifndef PLOTTER_HH
#define PLOTTER_HH

#include "../interface/PlotEntry.hh"
#include "../interface/PlotEntry2D.hh"
#include "../interface/MassRegionCuts.hh"

class Plotter{

public:
	// constructor    
	Plotter(std::vector<PlotEntry>);
	Plotter(std::vector<PlotEntry>, std::vector<PlotEntry>);
	Plotter(std::vector<PlotEntry2D>);
	Plotter(std::vector<TH1D*>);
	Plotter(std::vector<TH1D*>, std::vector<TH1D*>);
	Plotter(std::vector<TGraphAsymmErrors*>, const bool&); // the vector order goes: observed, expected, 1sigma, 2sigma

	void AddRatioBox(const std::string& = "ratio", const bool& = false);
	void AddRatioBox(const double&, const double&, const std::string& = "ratio", const bool& = false);
	void AddLegend(TLegend*);
	void AddLegend(const double&, const double&, const double&, const double&, const double& = 0.04);
	void AddLegend(const std::vector<std::string>&, const double&, const double&, const double&, const double&, const double& = 0.04);
	void AddLegend2Cols(const unsigned int&, const double&, const double&, const double&, const double&, const double& = 0.04);
	void AddLegend2Cols(const unsigned int&, const std::vector<std::string>&, const double&, const double&, const double&, const double&, const double& = 0.04);
	void AddLatex(const double&, const std::string& = "#it{Simulation}");
	void AddLatex(const std::string& = "#it{Simulation}");
	TStyle * GetTStyle(); // get it, to edit it
	void SetLogY();
	void SetLogZ();
	void SetErrors(const std::string& = "ALL_ERRORS");
	void SetYValueMin(const double&);
	void Save(const std::string&);
	void Save2D(const std::string&);
	void Save2D(const std::string&, const MassRegionCuts&);
	void SaveSpec01(const std::string& saveName, const std::vector<std::string>);
	void SaveBrazil(const std::string& saveName, const double&, const double&);

private:
	std::vector<PlotEntry> histoIndi;
	std::vector<PlotEntry> histoStack;
	std::vector<PlotEntry2D> histos2D;
	std::vector<TH1D*> th1Indi;
	std::vector<TH1D*> th1Stack;
	std::vector<TGraphAsymmErrors*> graphVec;
	bool addRatioBox;
	bool addRatioBoxUnityLine;
	std::string addRatioBoxInfo;
	std::string ratioBoxYAxisTitle;
	std::vector<double> ratioBoxYAxisMinMax;
	TLegend * leg;
	TLegend * leg2Cols;
	bool addLatex;
	std::string lumiLabel;
	std::string lhsStringAfterCMS;
	bool useLogY;
	bool useLogZ;
	bool plotWithErrorsIndi;
	bool plotWithErrorsStack;
	bool setYValueMin;
	double yValueMin;
	bool useObservedPlot;
	void DrawLatex(const unsigned int& = 1);	
	int SetColor_mellow(int, int);
	int SetColor_stark(const int&);
	TStyle * tdrStyle;
	TStyle * TDRStyle();
};

#endif