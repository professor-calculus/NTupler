#ifndef RHOEACORRECTIONS_H
#define RHOEACORRECTIONS_H

// Enter the fatJet's info and get the DBT SF info back
// info from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco

namespace RhoEACorr{

	// ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//

	double getRhoEA_CH_2016(const double& electron_eta)
	{
		if (fabs(electron_eta) < 1.0)
			return 0.0360;
		else if (fabs(electron_eta) < 1.479)
			return 0.0377;
		else if (fabs(electron_eta) < 2.0)
			return 0.0306;
		else if (fabs(electron_eta) < 2.2)
			return 0.0283;
        else if (fabs(electron_eta) < 2.3)
			return 0.0254;
        else if (fabs(electron_eta) < 2.4)
			return 0.0217;
		else
			return 0.0167;
	}

	double getRhoEA_NH_2016(const double& electron_eta)
	{
		if (fabs(electron_eta) < 1.0)
			return 0.0597;
		else if (fabs(electron_eta) < 1.479)
			return 0.0807;
		else if (fabs(electron_eta) < 2.0)
			return 0.0629;
		else if (fabs(electron_eta) < 2.2)
			return 0.0197;
        else if (fabs(electron_eta) < 2.3)
			return 0.0184;
        else if (fabs(electron_eta) < 2.4)
			return 0.0284;
		else
			return 0.0591;
	}

	double getRhoEA_PH_2016(const double& electron_eta)
	{
		if (fabs(electron_eta) < 1.0)
			return 0.1210;
		else if (fabs(electron_eta) < 1.479)
			return 0.1107;
		else if (fabs(electron_eta) < 2.0)
			return 0.0699;
		else if (fabs(electron_eta) < 2.2)
			return 0.1056;
        else if (fabs(electron_eta) < 2.3)
			return 0.1457;
        else if (fabs(electron_eta) < 2.4)
			return 0.1719;
		else
			return 0.1998;
	}

	// ----------------------------------------------------------//

	double getRhoEA_CH_2017(const double& electron_eta)
	{
		if (fabs(electron_eta) < 1.0)
			return 0.0360;
		else if (fabs(electron_eta) < 1.479)
			return 0.0377;
		else if (fabs(electron_eta) < 2.0)
			return 0.0306;
		else if (fabs(electron_eta) < 2.2)
			return 0.0283;
        else if (fabs(electron_eta) < 2.3)
			return 0.0254;
        else if (fabs(electron_eta) < 2.4)
			return 0.0217;
		else
			return 0.0167;
	}

	double getRhoEA_NH_2017(const double& electron_eta)
	{
		if (fabs(electron_eta) < 1.0)
			return 0.0597;
		else if (fabs(electron_eta) < 1.479)
			return 0.0807;
		else if (fabs(electron_eta) < 2.0)
			return 0.0629;
		else if (fabs(electron_eta) < 2.2)
			return 0.0197;
        else if (fabs(electron_eta) < 2.3)
			return 0.0184;
        else if (fabs(electron_eta) < 2.4)
			return 0.0284;
		else
			return 0.0591;
	}

	double getRhoEA_PH_2017(const double& electron_eta)
	{
		if (fabs(electron_eta) < 1.0)
			return 0.1210;
		else if (fabs(electron_eta) < 1.479)
			return 0.1107;
		else if (fabs(electron_eta) < 2.0)
			return 0.0699;
		else if (fabs(electron_eta) < 2.2)
			return 0.1056;
        else if (fabs(electron_eta) < 2.3)
			return 0.1457;
        else if (fabs(electron_eta) < 2.4)
			return 0.1719;
		else
			return 0.1998;
	}

	
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//

    double getEA_CH(const double& eta, const int& year)
    {
        if (year == 2016) return getRhoEA_CH_2016(eta);
        else if (year == 2017) return getRhoEA_CH_2017(eta);
        else return -1;
    }

    double getEA_NH(const double& eta, const int& year)
    {
        if (year == 2016) return getRhoEA_NH_2016(eta);
        else if (year == 2017) return getRhoEA_NH_2017(eta);
        else return -1;
    }

    double getEA_PH(const double& eta, const int& year)
    {
        if (year == 2016) return getRhoEA_PH_2016(eta);
        else if (year == 2017) return getRhoEA_PH_2017(eta);
        else return -1;
    }

} // closes namespace 'RhoEACorr'
#endif
