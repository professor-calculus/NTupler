#ifndef PLOTENTRY_HH
#define PLOTENTRY_HH

class PlotEntry{

public:
    // constructor
    PlotEntry(const std::string&, const TH1F&, const std::string&); // name of entry (eg for legends), histogram template, name of variable we wish to plot (this is for data)
	PlotEntry(const std::string&, const TH1F&, const std::string&, const double&); // name of entry (eg for legends), histogram template, name of variable we wish to plot, luminosity (this is for MC)
	
	void AddInput(const std::string&, const std::string&); // flatTree address, selection cut (this is for data)
	void AddInput(const std::string&, const std::string&, const double&, const bool& = true); // flatTree address, selection cut, sample cross-section (this is for MC), bool to prevent adding to numberOfEventsBeforeCuts
	std::string GetPlotEntryName() const;
	TH1F* GetHistogram() const;
	double GetNumberOfEventsBeforeCuts() const;
	double GetNumberOfEventsAfterCuts() const;
	// double GetLuminosity() const;

private:
	std::string plotEntryName;
	double luminosity;
	std::string variableToPlot;
	TH1F hNull;
	TH1F* hTotal;
	double numberOfEventsBeforeCuts;
};

#endif