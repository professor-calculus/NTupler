#ifndef PLOTTER_HH
#define PLOTTER_HH

class Plotter{

public:
	// constructor    
	Plotter(std::vector<PlotEntry>);
	Plotter(std::vector<PlotEntry>, std::vector<PlotEntry>);
	
	void AddLegend(TLegend*);
	void AddLegend(const double&, const double&, const double&, const double&, const double& = 0.04);
	void AddLatex(const double&, const std::string& = "#it{Simulation} W.I.P");
	void AddLatex(const std::string& = "#it{Simulation} W.I.P");
	TStyle * GetTStyle(); // get it, to edit it
	void Save(const std::string&);

private:
	std::vector<PlotEntry> histoIndi;
	std::vector<PlotEntry> histoStack;	
	TLegend * leg;
	bool addLatex;
	std::string lumiLabel;
	std::string lhsStringAfterCMS;
	void DrawLatex();	
	int SetColor_mellow(const int&, const int&);
	int SetColor_stark(const int&);
	TStyle * tdrStyle;
	TStyle * TDRStyle();
};

#endif