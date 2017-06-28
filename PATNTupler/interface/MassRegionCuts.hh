#ifndef MASSREGIONCUTS_HH
#define MASSREGIONCUTS_HH

class MassRegionCuts{

public:
    // constructor
    MassRegionCuts(const std::string&, const double&, const double&, const double&, const double&, const std::vector<double>&);

    std::string GetName() const;
    std::string Get_V_Cuts() const;
    std::string Get_H_Cuts() const;
    std::string Get_K_Cuts() const;
    std::string Get_O_Cuts() const;
    std::vector<std::string> Get_S_Cuts() const;
    std::vector<std::string> GetAllCuts() const;
private:
	std::string MassRegionCutName; // name to identify the set of mass cuts
	double VHK_bandSize; // this number defines the Vertical and Horizontal band regions
	double S1_Node; // one of the lower node points for SignalRegion1. the other node is the VHK_bandSize (nb symmetric under x<->y)
	double SMAX_Node1;// final signal region upper node (1of2). the coords are symmetric, but this is the larger one.
	double SMAX_Node2; // final signal region upper node (2of2). the coords are symmetric, but this is the smaller one.
	std::vector<double> SN_Nodes; // x values for segments of signal region lower line (number of segments = SN_Nodes.size()+1)
	std::string V_Cuts;
	std::string H_Cuts;
	std::string K_Cuts;
	std::string O_Cuts;
	std::vector<std::string> S_Cuts;
	double yValue(const double&, const double&, const double&, const double&) const;
	void make_V_Cuts();
	void make_H_Cuts();
	void make_K_Cuts();
	void make_OnS_cuts();
};

#endif