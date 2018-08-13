#ifndef DOUBLEBTAGSF_H
#define DOUBLEBTAGSF_H

#include "DoubleBTagWPs.h"

// Enter the fatJet's info and get the DBT SF info back
// info from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco

// TODO: need to fill in tt2017 scale factors (currently set to unity)

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

	double fX_signal = 0.87;
	double fY_signal = 0.13;
	double fX_ttbar = 0.68;
	double fY_ttbar = 0.32;

	// ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//

	double getLooseScaleFactor_signal_2016(const double& fatJet_pt)
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

	double getLooseScaleFactorUp_signal_2016(const double& fatJet_pt)
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

	double getLooseScaleFactorDown_signal_2016(const double& fatJet_pt)
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

	double getMed1ScaleFactor_signal_2016(const double& fatJet_pt)
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

	double getMed1ScaleFactorUp_signal_2016(const double& fatJet_pt)
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

	double getMed1ScaleFactorDown_signal_2016(const double& fatJet_pt)
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

	double getMed2ScaleFactor_signal_2016(const double& fatJet_pt)
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

	double getMed2ScaleFactorUp_signal_2016(const double& fatJet_pt)
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

	double getMed2ScaleFactorDown_signal_2016(const double& fatJet_pt)
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

	double getLooseScaleFactor_ttbar_2016(const double& fatJet_pt)
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

	double getLooseScaleFactorUp_ttbar_2016(const double& fatJet_pt)
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

	double getLooseScaleFactorDown_ttbar_2016(const double& fatJet_pt)
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

	double getMed1ScaleFactor_ttbar_2016(const double& fatJet_pt)
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

	double getMed1ScaleFactorUp_ttbar_2016(const double& fatJet_pt)
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

	double getMed1ScaleFactorDown_ttbar_2016(const double& fatJet_pt)
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

	double getMed2ScaleFactor_ttbar_2016(const double& fatJet_pt)
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

	double getMed2ScaleFactorUp_ttbar_2016(const double& fatJet_pt)
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

	double getMed2ScaleFactorDown_ttbar_2016(const double& fatJet_pt)
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

	double getLooseScaleFactor_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.96;
		else if (fatJet_pt < 350.0)
			return 0.96;
		else if (fatJet_pt < 840.0)
			return 0.95;
		else
			return 0.95;
	}

	double getLooseScaleFactorUp_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.96 + 2 * 0.03);
		else if (fatJet_pt < 350.0)
			return (0.96 + 0.03);
		else if (fatJet_pt < 840.0)
			return (0.95 + 0.06);
		else
			return (0.95 + 2 * 0.06);
	}

	double getLooseScaleFactorDown_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.96 - 2 * 0.03);
		else if (fatJet_pt < 350.0)
			return (0.96 - 0.03);
		else if (fatJet_pt < 840.0)
			return (0.95 - 0.04);
		else
			return (0.95 - 2 * 0.04);
	}

	// ----------------------------------------------------------//

	double getMed1ScaleFactor_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.93;
		else if (fatJet_pt < 350.0)
			return 0.93;
		else if (fatJet_pt < 840.0)
			return 0.90;
		else
			return 0.90;
	}

	double getMed1ScaleFactorUp_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.93 + 2 * 0.04);
		else if (fatJet_pt < 350.0)
			return (0.93 + 0.04);
		else if (fatJet_pt < 840.0)
			return (0.90 + 0.08);
		else
			return (0.90 + 2 * 0.08);
	}

	double getMed1ScaleFactorDown_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.93 - 2 * 0.03);
		else if (fatJet_pt < 350.0)
			return (0.93 - 0.03);
		else if (fatJet_pt < 840.0)
			return (0.90 - 0.04);
		else
			return (0.90 - 2 * 0.04);
	}

	// ----------------------------------------------------------//

	double getMed2ScaleFactor_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.85;
		else if (fatJet_pt < 350.0)
			return 0.85;
		else if (fatJet_pt < 840.0)
			return 0.80;
		else
			return 0.80;
	}

	double getMed2ScaleFactorUp_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.85 + 2 * 0.04);
		else if (fatJet_pt < 350.0)
			return (0.85 + 0.04);
		else if (fatJet_pt < 840.0)
			return (0.80 + 0.07);
		else
			return (0.80 + 2 * 0.07);
	}

	double getMed2ScaleFactorDown_signal_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.85 - 2 * 0.04);
		else if (fatJet_pt < 350.0)
			return (0.85 - 0.04);
		else if (fatJet_pt < 840.0)
			return (0.80 - 0.04);
		else
			return (0.80 - 2 * 0.04);
	}

	// ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//

	double getLooseScaleFactor_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.939;
		else if (fatJet_pt < 350.0)
			return 0.939;
		else if (fatJet_pt < 430.0)
			return 1.007;
		else
			return 0.996;
	}

	double getLooseScaleFactorUp_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.939 + 2 * 0.026);
		else if (fatJet_pt < 350.0)
			return (0.939 + 0.026);
		else if (fatJet_pt < 430.0)
			return (1.007 + 0.055);
		else
			return (0.996 + 0.080);
	}

	double getLooseScaleFactorDown_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.939 - 2 * 0.026);
		else if (fatJet_pt < 350.0)
			return (0.939 - 0.026);
		else if (fatJet_pt < 430.0)
			return (1.007 - 0.054);
		else
			return (0.996 - 0.078);
	}

	// ----------------------------------------------------------//

	double getMed1ScaleFactor_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.922;
		else if (fatJet_pt < 350.0)
			return 0.922;
		else if (fatJet_pt < 430.0)
			return 0.967;
		else
			return 0.902;
	}

	double getMed1ScaleFactorUp_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.922 + 2 * 0.027);
		else if (fatJet_pt < 350.0)
			return (0.922 + 0.027);
		else if (fatJet_pt < 430.0)
			return (0.967 + 0.057);
		else
			return (0.902 + 0.083);
	}

	double getMed1ScaleFactorDown_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.922 - 2 * 0.027);
		else if (fatJet_pt < 350.0)
			return (0.922 - 0.027);
		else if (fatJet_pt < 430.0)
			return (0.967 - 0.056);
		else
			return (0.902 - 0.081);
	}

	// ----------------------------------------------------------//

	double getMed2ScaleFactor_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return 0.875;
		else if (fatJet_pt < 350.0)
			return 0.875;
		else if (fatJet_pt < 430.0)
			return 0.939;
		else
			return 0.893;
	}

	double getMed2ScaleFactorUp_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.875 + 2 * 0.030);
		else if (fatJet_pt < 350.0)
			return (0.875 + 0.030);
		else if (fatJet_pt < 430.0)
			return (0.939 + 0.063);
		else
			return (0.893 + 0.091);
	}

	double getMed2ScaleFactorDown_ttbar_2017(const double& fatJet_pt)
	{
		if (fatJet_pt < 250.0)
			return (0.875 - 2 * 0.030);
		else if (fatJet_pt < 350.0)
			return (0.875 - 0.030);
		else if (fatJet_pt < 430.0)
			return (0.939 - 0.063);
		else
			return (0.893 - 0.089);
	}

	// ----------------------------------------------------------//
	// ----------------------------------------------------------//
	// ----------------------------------------------------------//

	double getDbtTagScaleFactor_signal(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt, const unsigned int& yearOfRun)
	{
		if (yearOfRun != 2016 && yearOfRun != 2017){
            std::cout << "You have not provided a valid yearOfRun for the function DoubleBTagSF::getDbtTagScaleFactor_signal()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of -111111.1" << std::endl;
            return -111111.1;
		}

		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1")){
			if (yearOfRun == 2016) return getMed1ScaleFactor_signal_2016(fatJetA_pt) * getMed1ScaleFactor_signal_2016(fatJetB_pt);
			else return getMed1ScaleFactor_signal_2017(fatJetA_pt) * getMed1ScaleFactor_signal_2017(fatJetB_pt);
		}

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

			double sX;
			double sZ;
			if (yearOfRun == 2016){
				sX = getMed2ScaleFactor_signal_2016(fatJetSpecA_pt) * getMed1ScaleFactor_signal_2016(fatJetSpecB_pt);
				sZ = getMed2ScaleFactor_signal_2016(fatJetSpecA_pt) * getLooseScaleFactor_signal_2016(fatJetSpecB_pt); 
			}
 			else{
				sX = getMed2ScaleFactor_signal_2017(fatJetSpecA_pt) * getMed1ScaleFactor_signal_2017(fatJetSpecB_pt);
				sZ = getMed2ScaleFactor_signal_2017(fatJetSpecA_pt) * getLooseScaleFactor_signal_2017(fatJetSpecB_pt); 
			}
			return (sZ - fX_signal * sX) / fY_signal;
		}
	}


	double getDbtTagScaleFactorUp_signal(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt, const unsigned int& yearOfRun)
	{
		if (yearOfRun != 2016 && yearOfRun != 2017){
            std::cout << "You have not provided a valid yearOfRun for the function DoubleBTagSF::getDbtTagScaleFactorUp_signal()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of -222222.1" << std::endl;
            return -222222.1;
		}

		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1")){
			if (yearOfRun == 2016) return getMed1ScaleFactorUp_signal_2016(fatJetA_pt) * getMed1ScaleFactorUp_signal_2016(fatJetB_pt);
			else return getMed1ScaleFactorUp_signal_2017(fatJetA_pt) * getMed1ScaleFactorUp_signal_2017(fatJetB_pt);
		}

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

			double sX;
			double sZ;
			if (yearOfRun == 2016){
				sX = getMed2ScaleFactorUp_signal_2016(fatJetSpecA_pt) * getMed1ScaleFactorUp_signal_2016(fatJetSpecB_pt);
				sZ = getMed2ScaleFactorUp_signal_2016(fatJetSpecA_pt) * getLooseScaleFactorUp_signal_2016(fatJetSpecB_pt); 
			}
 			else{
				sX = getMed2ScaleFactorUp_signal_2017(fatJetSpecA_pt) * getMed1ScaleFactorUp_signal_2017(fatJetSpecB_pt);
				sZ = getMed2ScaleFactorUp_signal_2017(fatJetSpecA_pt) * getLooseScaleFactorUp_signal_2017(fatJetSpecB_pt); 
			}
			return (sZ - fX_signal * sX) / fY_signal;
		}
	}


	double getDbtTagScaleFactorDown_signal(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt, const unsigned int& yearOfRun)
	{
		if (yearOfRun != 2016 && yearOfRun != 2017){
            std::cout << "You have not provided a valid yearOfRun for the function DoubleBTagSF::getDbtTagScaleFactorDown_signal()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of -333333.1" << std::endl;
            return -333333.1;
		}

		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1")){
			if (yearOfRun == 2016) return getMed1ScaleFactorDown_signal_2016(fatJetA_pt) * getMed1ScaleFactorDown_signal_2016(fatJetB_pt);
			else return getMed1ScaleFactorDown_signal_2017(fatJetA_pt) * getMed1ScaleFactorDown_signal_2017(fatJetB_pt);
		}

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

			double sX;
			double sZ;
			if (yearOfRun == 2016){
				sX = getMed2ScaleFactorDown_signal_2016(fatJetSpecA_pt) * getMed1ScaleFactorDown_signal_2016(fatJetSpecB_pt);
				sZ = getMed2ScaleFactorDown_signal_2016(fatJetSpecA_pt) * getLooseScaleFactorDown_signal_2016(fatJetSpecB_pt); 
			}
 			else{
				sX = getMed2ScaleFactorDown_signal_2017(fatJetSpecA_pt) * getMed1ScaleFactorDown_signal_2017(fatJetSpecB_pt);
				sZ = getMed2ScaleFactorDown_signal_2017(fatJetSpecA_pt) * getLooseScaleFactorDown_signal_2017(fatJetSpecB_pt); 
			}
			return (sZ - fX_signal * sX) / fY_signal;
		}
	}


	// ----------------------------------------------------------//


	double getDbtTagScaleFactor_ttbar(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt, const unsigned int& yearOfRun)
	{
		if (yearOfRun != 2016 && yearOfRun != 2017){
            std::cout << "You have not provided a valid yearOfRun for the function DoubleBTagSF::getDbtTagScaleFactor_ttbar()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of -444444.1" << std::endl;
            return -444444.1;
		}

		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1")){
			if (yearOfRun == 2016) return getMed1ScaleFactor_ttbar_2016(fatJetA_pt) * getMed1ScaleFactor_ttbar_2016(fatJetB_pt);
			else return getMed1ScaleFactor_ttbar_2017(fatJetA_pt) * getMed1ScaleFactor_ttbar_2017(fatJetB_pt);
		}

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

			double sX;
			double sZ;
			if (yearOfRun == 2016){
				sX = getMed2ScaleFactor_ttbar_2016(fatJetSpecA_pt) * getMed1ScaleFactor_ttbar_2016(fatJetSpecB_pt);
				sZ = getMed2ScaleFactor_ttbar_2016(fatJetSpecA_pt) * getLooseScaleFactor_ttbar_2016(fatJetSpecB_pt); 
			}
 			else{
				sX = getMed2ScaleFactor_ttbar_2017(fatJetSpecA_pt) * getMed1ScaleFactor_ttbar_2017(fatJetSpecB_pt);
				sZ = getMed2ScaleFactor_ttbar_2017(fatJetSpecA_pt) * getLooseScaleFactor_ttbar_2017(fatJetSpecB_pt); 
			}
			return (sZ - fX_ttbar * sX) / fY_ttbar;
		}
	}


	double getDbtTagScaleFactorUp_ttbar(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt, const unsigned int& yearOfRun)
	{

		if (yearOfRun != 2016 && yearOfRun != 2017){
            std::cout << "You have not provided a valid yearOfRun for the function DoubleBTagSF::getDbtTagScaleFactorUp_ttbar()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of -555555.1" << std::endl;
            return -555555.1;
		}

		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1")){
			if (yearOfRun == 2016) return getMed1ScaleFactorUp_ttbar_2016(fatJetA_pt) * getMed1ScaleFactorUp_ttbar_2016(fatJetB_pt);
			else return getMed1ScaleFactorUp_ttbar_2017(fatJetA_pt) * getMed1ScaleFactorUp_ttbar_2017(fatJetB_pt);
		}

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

			double sX;
			double sZ;
			if (yearOfRun == 2016){
				sX = getMed2ScaleFactorUp_ttbar_2016(fatJetSpecA_pt) * getMed1ScaleFactorUp_ttbar_2016(fatJetSpecB_pt);
				sZ = getMed2ScaleFactorUp_ttbar_2016(fatJetSpecA_pt) * getLooseScaleFactorUp_ttbar_2016(fatJetSpecB_pt); 
			}
 			else{
				sX = getMed2ScaleFactorUp_ttbar_2017(fatJetSpecA_pt) * getMed1ScaleFactorUp_ttbar_2017(fatJetSpecB_pt);
				sZ = getMed2ScaleFactorUp_ttbar_2017(fatJetSpecA_pt) * getLooseScaleFactorUp_ttbar_2017(fatJetSpecB_pt); 
			}
			return (sZ - fX_ttbar * sX) / fY_ttbar;
		}
	}


	double getDbtTagScaleFactorDown_ttbar(const double& fatJetA_pt, const double& fatJetA_dbt, const double& fatJetB_pt, const double& fatJetB_dbt, const unsigned int& yearOfRun)
	{
		if (yearOfRun != 2016 && yearOfRun != 2017){
            std::cout << "You have not provided a valid yearOfRun for the function DoubleBTagSF::getDbtTagScaleFactorDown_ttbar()" << std::endl;
            std::cout << "It should be 2016 or 2017" << std::endl;
            std::cout << "Returning a dummy value of -666666.1" << std::endl;
            return -666666.1;
		}

		// check fatJets are in the TAG dbt region
		if (fatJetA_dbt < (-1.0 * fatJetB_dbt + 1.0 + DoubleBTagWPs::dbtNameToDouble("Loose"))) return 1.0;

		// are fatJets in the Med1 * Med1 square?
		if (fatJetA_dbt > DoubleBTagWPs::dbtNameToDouble("Med1") && fatJetB_dbt > DoubleBTagWPs::dbtNameToDouble("Med1")){
			if (yearOfRun == 2016) return getMed1ScaleFactorDown_ttbar_2016(fatJetA_pt) * getMed1ScaleFactorDown_ttbar_2016(fatJetB_pt);
			else return getMed1ScaleFactorDown_ttbar_2017(fatJetA_pt) * getMed1ScaleFactorDown_ttbar_2017(fatJetB_pt);
		}

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

			double sX;
			double sZ;
			if (yearOfRun == 2016){
				sX = getMed2ScaleFactorDown_ttbar_2016(fatJetSpecA_pt) * getMed1ScaleFactorDown_ttbar_2016(fatJetSpecB_pt);
				sZ = getMed2ScaleFactorDown_ttbar_2016(fatJetSpecA_pt) * getLooseScaleFactorDown_ttbar_2016(fatJetSpecB_pt); 
			}
 			else{
				sX = getMed2ScaleFactorDown_ttbar_2017(fatJetSpecA_pt) * getMed1ScaleFactorDown_ttbar_2017(fatJetSpecB_pt);
				sZ = getMed2ScaleFactorDown_ttbar_2017(fatJetSpecA_pt) * getLooseScaleFactorDown_ttbar_2017(fatJetSpecB_pt); 
			}
			return (sZ - fX_ttbar * sX) / fY_ttbar;
		}
	}


} // closes namespace 'DoubleBTagSF'
#endif