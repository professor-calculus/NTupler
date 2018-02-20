#ifndef PLOTENTRY2D_HH
#define PLOTENTRY2D_HH

class PlotEntry2D{

public:
    // constructor
    PlotEntry2D(const std::string&, const TH2D&, const std::string&); // name of entry (eg for legends), histogram template, variableY:variableX that we wish to plot. (this is for data)
	PlotEntry2D(const std::string&, const TH2D&, const std::string&, const double&); // name of entry (eg for legends), histogram template, variableY:variableX that we wish to plot, luminosity. (this is for MC)
	
	void AddInput(const std::string&, const std::string&); // flatTree address, selection cut (this is for data)
	void AddInput(const std::string&, const std::string&, const double&, const std::string& = ""); // flatTree address, selection cut, sample cross-section (this is for MC), string of the SF weights
	std::string GetPlotEntryName() const;
	TH2D* GetHistogram() const;
	double GetNumberOfEventsBeforeCuts() const;
	double GetNumberOfEventsAfterCuts() const;
	double GetNumberOfEventsAfterCutsStatError() const;
	void NormalisePlot();
private:
	std::string plotEntryName;
	double luminosity;
	std::string variablesToPlot;
	TH2D hNull;
	TH2D* hTotal;
	double numberOfEventsBeforeCuts;
	double numberOfEventsAfterCuts_StatErrorSquared;
};

#endif