#ifndef PLOTENTRY_HH
#define PLOTENTRY_HH

class PlotEntry{

public:
    // constructor
    PlotEntry(const std::string&, PlotEntry, PlotEntry); // DIVIDING MODE, divides two existing PlotEntry objects histograms, name of entry (eg for legends), numerator PlotEntry, denominator PlotEntry
    PlotEntry(const std::string&, const TH1D&, const std::string&); // name of entry (eg for legends), histogram template, name of variable we wish to plot (this is for data)
	PlotEntry(const std::string&, const TH1D&, const std::string&, const double&); // name of entry (eg for legends), histogram template, name of variable we wish to plot, luminosity (this is for MC)
	
	void AddInput(const std::string&, const std::string&); // flatTree address, selection cut (this is for data)
	void AddInput(const std::string&, const std::string&, const double&, const bool& = true); // flatTree address, selection cut, sample cross-section (this is for MC), bool to prevent adding to numberOfEventsBeforeCuts
	std::string GetPlotEntryName() const;
	TH1D* GetHistogram() const;
	double GetNumberOfEventsBeforeCuts() const;
	double GetNumberOfEventsAfterCuts() const;
	// double GetLuminosity() const;
	std::vector<double> GetStatErrorSquaredVector() const;
	void NormalisePlot();
private:
	std::string plotEntryName;
	double luminosity;
	std::string variableToPlot;
	TH1D hNull;
	TH1D* hTotal;
	double numberOfEventsBeforeCuts;
	std::vector<double> statErrorSquared;
};

#endif