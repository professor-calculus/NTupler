#ifndef PLOTENTRY_HH
#define PLOTENTRY_HH

#include <TEfficiency.h>

class PlotEntry{

public:
    // constructor
    PlotEntry(const std::string&, const TH1D&, const std::string&); // name of entry (eg for legends), histogram template, name of variable we wish to plot (this is for data)
	PlotEntry(const std::string&, const TH1D&, const std::string&, const double&); // name of entry (eg for legends), histogram template, name of variable we wish to plot, luminosity (this is for MC)
	
	~PlotEntry();

	void AddInput(const std::string&, const std::string&); // flatTree address, selection cut (this is for data)
	void AddInput(const std::string&, const std::string&, const double&, const std::string& = ""); // flatTree address, selection cut, sample cross-section (this is for MC), string of the SF weights
	void AddInputEfficiency(const std::string&, const std::string&, const std::string&); // flatTree address, common cut (to numerator and denominator), additional cut for the numerator
	std::string GetPlotEntryName() const;
	TH1D* GetHistogram() const;
	TEfficiency* GetTEff() const;
	double GetNumberOfEventsBeforeCuts() const;
	double GetNumberOfEventsAfterCuts() const;
	// double GetLuminosity() const;
	std::vector<double> GetStatErrorSquaredVector() const;
	void NormalisePlot();
	void FitFunction(const std::string&, const double&, const double&, const std::vector<double>&, const int& = 46);
private:
	std::string plotEntryName;
	double luminosity;
	std::string variableToPlot;
	TH1D hNull;
	TH1D* hTotal;
	TEfficiency* hEff;
	double numberOfEventsBeforeCuts;
	std::vector<double> statErrorSquared;
};

#endif