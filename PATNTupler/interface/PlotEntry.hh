


class PlotEntry{

public:
    // constructor - should overload: give it a luminosity and TH1F || TH2F. default would be luminosity=0, also string variable to use for plot eg fatJetA_softDropMass
    // overload for 
	PlotEntry();

	TH1F GetHistogram() const; // get the final histogram ... tricky if don't know if 1d or 2d (think about this!!!)
	TH2F GetHistogram2() const; // ^^

	void AddEntry(); // give it: flatTree address, cross-section, cut string: it will add it to the hTOTAL and do event weighting and counting!

private:
	double luminosity;
	double lumiEventWeighting; // data would NOT want this
	TH1F hTOTAL; // set from the initial histogram in constructor
	TH2F h2TOTAL;

	double numberOfEventsBeforeCuts; // add entry function should top these up
	double numberOfEventsAfterPreSel; // add entry functoin should top these up

};


// vector of these objects ... STACK (backgornd)
// vector of these objects ... SEP

// pass these objects to the plotter