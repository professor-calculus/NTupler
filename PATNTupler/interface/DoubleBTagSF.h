#ifndef DOUBLEBTAGSF_H
#define DOUBLEBTAGSF_H

#include "DoubleBTagWPs.h"

// Enter the fatJet's info and get the DBT SF info back
// info from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco

// TODO: need to put in REAL fractions fX_* fY_*

namespace DoubleBTagSF{

	// DBT Tag area = DIAG_UP{Loose,1.0 <-> 1.0,Loose}
	
	// if within the Med1 * Med1 square that is simple
	// if not, need to do claire's ides


	// :::claire's idea:::

	// DBT area X = dbtA > 0.8 (Med2) && dbtB > 0.6 (Med1)
	// DBT area Y = dbtA > 0.8 (Med2) && 0.3 (Loose) < dbtB < 0.6 (Med1)
	// DBT area Z = X + Y

	// use the following fractional distributions (for a given sample)
	// fX = X / (X + Y)
	// fY = Y / (X + Y)
	// note: derived for ht1500+, fatJet*2 pt > 300, 1 isoAk4 pt > 300

	// for scale factors sX, sY, sZ
	// sZ = fX * sX + fY * sY
	// then the scale factor for region Y is
	// sY = (sZ - fX * sX) / fY

	double fX_signal = 0.8;
	double fY_signal = 0.2;
	double fX_ttbar = 0.8;
	double fY_ttbar = 0.2;

	// ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//

	double getLooseScaleFactor_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.96;
		else if (fatJet_pt < 350.0)
			return 0.96;
		else if (fatJet_pt < 430.0)
			return 1.00;
		else if (fatJet_pt < 840.0)
			return 1.01;
		else
			return 1.01;
	}

	double getLooseScaleFactorUp_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.96 + 2 * 0.03);
		else if (fatJet_pt < 350.0)
			return (0.96 + 0.03);
		else if (fatJet_pt < 430.0)
			return (1.00 + 0.04);
		else if (fatJet_pt < 840.0)
			return (1.01 + 0.02);
		else
			return (1.01 + 2 * 0.02);
	}

	double getLooseScaleFactorDown_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.96 - 2 * 0.02);
		else if (fatJet_pt < 350.0)
			return (0.96 - 0.02);
		else if (fatJet_pt < 430.0)
			return (1.00 - 0.03);
		else if (fatJet_pt < 840.0)
			return (1.01 - 0.04);
		else
			return (1.01 - 2 * 0.04);
	}

	// ----------------------------------------------------------//

	double getMed1ScaleFactor_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.93;
		else if (fatJet_pt < 350.0)
			return 0.93;
		else if (fatJet_pt < 430.0)
			return 1.01;
		else if (fatJet_pt < 840.0)
			return 0.99;
		else
			return 0.99;
	}

	double getMed1ScaleFactorUp_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.93 + 2 * 0.03);
		else if (fatJet_pt < 350.0)
			return (0.93 + 0.03);
		else if (fatJet_pt < 430.0)
			return (1.01 + 0.03);
		else if (fatJet_pt < 840.0)
			return (0.99 + 0.02);
		else
			return (0.99 + 2 * 0.02);
	}

	double getMed1ScaleFactorDown_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.93 - 2 * 0.02);
		else if (fatJet_pt < 350.0)
			return (0.93 - 0.02);
		else if (fatJet_pt < 430.0)
			return (1.01 - 0.03);
		else if (fatJet_pt < 840.0)
			return (0.99 - 0.04);
		else
			return (0.99 - 2 * 0.04);
	}

	// ----------------------------------------------------------//

	double getMed2ScaleFactor_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.92;
		else if (fatJet_pt < 350.0)
			return 0.92;
		else if (fatJet_pt < 430.0)
			return 1.01;
		else if (fatJet_pt < 840.0)
			return 0.92;
		else
			return 0.92;
	}

	double getMed2ScaleFactorUp_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.92 + 2 * 0.03);
		else if (fatJet_pt < 350.0)
			return (0.92 + 0.03);
		else if (fatJet_pt < 430.0)
			return (1.01 + 0.03);
		else if (fatJet_pt < 840.0)
			return (0.92 + 0.03);
		else
			return (0.92 + 2 * 0.03);
	}

	double getMed2ScaleFactorDown_signal(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.92 - 2 * 0.03);
		else if (fatJet_pt < 350.0)
			return (0.92 - 0.03);
		else if (fatJet_pt < 430.0)
			return (1.01 - 0.04);
		else if (fatJet_pt < 840.0)
			return (0.92 - 0.05);
		else
			return (0.92 - 2 * 0.05);
	}

	// ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//

	double getLooseScaleFactor_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 1.044;
		else if (fatJet_pt < 350.0)
			return 1.044;
		else if (fatJet_pt < 430.0)
			return 1.074;
		else if (fatJet_pt < 700.0)
			return 1.119;
		else
			return 1.119;
	}

	double getLooseScaleFactorUp_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (1.044 + 2 * 0.028);
		else if (fatJet_pt < 350.0)
			return (1.044 + 0.028);
		else if (fatJet_pt < 430.0)
			return (1.074 + 0.052);
		else if (fatJet_pt < 700.0)
			return (1.119 + 0.079);
		else
			return (1.119 + 2 * 0.079);
	}

	double getLooseScaleFactorDown_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (1.044 - 2 * 0.028);
		else if (fatJet_pt < 350.0)
			return (1.044 - 0.028);
		else if (fatJet_pt < 430.0)
			return (1.074 - 0.052);
		else if (fatJet_pt < 700.0)
			return (1.119 - 0.079);
		else
			return (1.119 - 2 * 0.079);
	}

	// ----------------------------------------------------------//

	double getMed1ScaleFactor_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 1.029;
		else if (fatJet_pt < 350.0)
			return 1.029;
		else if (fatJet_pt < 700.0)
			return 1.156;
		else
			return 1.156;
	}

	double getMed1ScaleFactorUp_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (1.029 + 2 * 0.034);
		else if (fatJet_pt < 350.0)
			return (1.029 + 0.034);
		else if (fatJet_pt < 700.0)
			return (1.156 + 0.064);
		else
			return (1.156 + 2 * 0.064);
	}

	double getMed1ScaleFactorDown_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (1.029 - 2 * 0.034);
		else if (fatJet_pt < 350.0)
			return (1.029 - 0.034);
		else if (fatJet_pt < 700.0)
			return (1.156 - 0.064);
		else
			return (1.156 - 2 * 0.064);
	}

	// ----------------------------------------------------------//

	double getMed2ScaleFactor_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 1.050;
		else if (fatJet_pt < 350.0)
			return 1.050;
		else if (fatJet_pt < 700.0)
			return 1.086;
		else
			return 1.086;
	}

	double getMed2ScaleFactorUp_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (1.050 + 2 * 0.044);
		else if (fatJet_pt < 350.0)
			return (1.050 + 0.044);
		else if (fatJet_pt < 700.0)
			return (1.086 + 0.078);
		else
			return (1.086 + 2 * 0.078);
	}

	double getMed2ScaleFactorDown_ttbar(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (1.050 - 2 * 0.044);
		else if (fatJet_pt < 350.0)
			return (1.050 - 0.044);
		else if (fatJet_pt < 700.0)
			return (1.086 - 0.078);
		else
			return (1.086 - 2 * 0.078);
	}

	// ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//


	double getDbtTagScaleFactor_signal(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt)
	{
		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1"))
			return getMed1ScaleFactor_signal(fatJetA_pt) * getMed1ScaleFactor_signal(fatJetB_pt);

		// otherwise, need to do claires trick
		else{

			double fatJetSpecA_pt, fatJetSpecB_pt;
			if (fatJetA_dbt > fatJetB_dbt){
				fatJetSpecA_pt = fatJetA_pt;
				fatJetSpecB_pt = fatJetB_pt;
			}
			else{
				fatJetSpecA_pt = fatJetB_pt;
				fatJetSpecB_pt = fatJetA_pt;
			}

			double sX = getMed2ScaleFactor_signal(fatJetSpecA_pt) * getMed1ScaleFactor_signal(fatJetSpecB_pt);
			double sZ = getMed2ScaleFactor_signal(fatJetSpecA_pt) * getLooseScaleFactor_signal(fatJetSpecB_pt); 
			return (sZ - fX_signal * sX) / fY_signal;
		}
	}

	double getDbtTagScaleFactorUp_signal(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt)
	{
		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1"))
			return getMed1ScaleFactorUp_signal(fatJetA_pt) * getMed1ScaleFactorUp_signal(fatJetB_pt);

		// otherwise, need to do claires trick
		else{

			double fatJetSpecA_pt, fatJetSpecB_pt;
			if (fatJetA_dbt > fatJetB_dbt){
				fatJetSpecA_pt = fatJetA_pt;
				fatJetSpecB_pt = fatJetB_pt;
			}
			else{
				fatJetSpecA_pt = fatJetB_pt;
				fatJetSpecB_pt = fatJetA_pt;
			}

			double sX = getMed2ScaleFactorUp_signal(fatJetSpecA_pt) * getMed1ScaleFactorUp_signal(fatJetSpecB_pt);
			double sZ = getMed2ScaleFactorUp_signal(fatJetSpecA_pt) * getLooseScaleFactorUp_signal(fatJetSpecB_pt); 
			return (sZ - fX_signal * sX) / fY_signal;
		}
	}

	double getDbtTagScaleFactorDown_signal(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt)
	{
		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1"))
			return getMed1ScaleFactorDown_signal(fatJetA_pt) * getMed1ScaleFactorDown_signal(fatJetB_pt);

		// otherwise, need to do claires trick
		else{

			double fatJetSpecA_pt, fatJetSpecB_pt;
			if (fatJetA_dbt > fatJetB_dbt){
				fatJetSpecA_pt = fatJetA_pt;
				fatJetSpecB_pt = fatJetB_pt;
			}
			else{
				fatJetSpecA_pt = fatJetB_pt;
				fatJetSpecB_pt = fatJetA_pt;
			}

			double sX = getMed2ScaleFactorDown_signal(fatJetSpecA_pt) * getMed1ScaleFactorDown_signal(fatJetSpecB_pt);
			double sZ = getMed2ScaleFactorDown_signal(fatJetSpecA_pt) * getLooseScaleFactorDown_signal(fatJetSpecB_pt); 
			return (sZ - fX_signal * sX) / fY_signal;
		}
	}

	// ----------------------------------------------------------//

	double getDbtTagScaleFactor_ttbar(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt)
	{
		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1"))
			return getMed1ScaleFactor_ttbar(fatJetA_pt) * getMed1ScaleFactor_ttbar(fatJetB_pt);

		// otherwise, need to do claires trick
		else{

			double fatJetSpecA_pt, fatJetSpecB_pt;
			if (fatJetA_dbt > fatJetB_dbt){
				fatJetSpecA_pt = fatJetA_pt;
				fatJetSpecB_pt = fatJetB_pt;
			}
			else{
				fatJetSpecA_pt = fatJetB_pt;
				fatJetSpecB_pt = fatJetA_pt;
			}

			double sX = getMed2ScaleFactor_ttbar(fatJetSpecA_pt) * getMed1ScaleFactor_ttbar(fatJetSpecB_pt);
			double sZ = getMed2ScaleFactor_ttbar(fatJetSpecA_pt) * getLooseScaleFactor_ttbar(fatJetSpecB_pt); 
			return (sZ - fX_ttbar * sX) / fY_ttbar;
		}
	}

	double getDbtTagScaleFactorUp_ttbar(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt)
	{
		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1"))
			return getMed1ScaleFactorUp_ttbar(fatJetA_pt) * getMed1ScaleFactorUp_ttbar(fatJetB_pt);

		// otherwise, need to do claires trick
		else{

			double fatJetSpecA_pt, fatJetSpecB_pt;
			if (fatJetA_dbt > fatJetB_dbt){
				fatJetSpecA_pt = fatJetA_pt;
				fatJetSpecB_pt = fatJetB_pt;
			}
			else{
				fatJetSpecA_pt = fatJetB_pt;
				fatJetSpecB_pt = fatJetA_pt;
			}

			double sX = getMed2ScaleFactorUp_ttbar(fatJetSpecA_pt) * getMed1ScaleFactorUp_ttbar(fatJetSpecB_pt);
			double sZ = getMed2ScaleFactorUp_ttbar(fatJetSpecA_pt) * getLooseScaleFactorUp_ttbar(fatJetSpecB_pt); 
			return (sZ - fX_ttbar * sX) / fY_ttbar;
		}
	}

	double getDbtTagScaleFactorDown_ttbar(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt)
	{
		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1"))
			return getMed1ScaleFactorDown_ttbar(fatJetA_pt) * getMed1ScaleFactorDown_ttbar(fatJetB_pt);

		// otherwise, need to do claires trick
		else{

			double fatJetSpecA_pt, fatJetSpecB_pt;
			if (fatJetA_dbt > fatJetB_dbt){
				fatJetSpecA_pt = fatJetA_pt;
				fatJetSpecB_pt = fatJetB_pt;
			}
			else{
				fatJetSpecA_pt = fatJetB_pt;
				fatJetSpecB_pt = fatJetA_pt;
			}

			double sX = getMed2ScaleFactorDown_ttbar(fatJetSpecA_pt) * getMed1ScaleFactorDown_ttbar(fatJetSpecB_pt);
			double sZ = getMed2ScaleFactorDown_ttbar(fatJetSpecA_pt) * getLooseScaleFactorDown_ttbar(fatJetSpecB_pt); 
			return (sZ - fX_ttbar * sX) / fY_ttbar;
		}
	}


} // closes namespace 'DoubleBTagSF'
#endif