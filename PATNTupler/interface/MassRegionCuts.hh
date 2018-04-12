#ifndef MASSREGIONCUTS_HH
#define MASSREGIONCUTS_HH

class MassRegionCuts{

public:
    // constructor
    MassRegionCuts(const std::string&, const double&, const double&, const double&, const double&, const std::vector<double>&, const std::string& = "fatJetA_softDropMassPuppi", const std::string& = "fatJetB_softDropMassPuppi", const double& = 0.50);

    std::string GetName() const;
    double Get_S1_Node1() const;
    double Get_S1_Node2() const;
    double Get_SMAX_Node1() const;
    double Get_SMAX_Node2() const;
    double Get_sideBandScaleFactor() const;
    std::vector<double> Get_SN_Nodes() const;
    std::vector<std::string> Get_S_Cuts() const;
    std::vector<std::string> Get_U_Cuts() const;
    std::vector<std::string> Get_D_Cuts() const;
    std::vector<std::string> GetAllCuts() const;

private:
	std::string MassRegionCutName; // name to identify the set of mass cuts
    double sideBandScaleFactor; // sideband size as a fraction of signal region, default is 0.50
    std::string fatJetA_mass; // name for the type of mass to use for fatJetA
    std::string fatJetB_mass; // name for the type of mass to use for fatJetB
    double S1_Node1; // first signal region start point node (1of2). the coords are symmetric, but this is the larger one.
    double S1_Node2; // first signal region start point node (2of2). the coords are symmetric, but this is the smaller one.
    double SMAX_Node1;// final signal region end point node (1of2). the coords are symmetric, but this is the larger one.
    double SMAX_Node2; // final signal region end point node (2of2). the coords are symmetric, but this is the smaller one.
    std::vector<double> SN_Nodes; // x values for segments of signal region lower line (number of segments = SN_Nodes.size()+1)
	std::vector<std::string> S_Cuts;
	std::vector<std::string> U_Cuts;
	std::vector<std::string> D_Cuts;
	double yValue(const double&, const double&, const double&, const double&) const;
	void make_cuts();
};

#endif