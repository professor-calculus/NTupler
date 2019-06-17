#ifndef QCDSIDEBANDCORR_H
#define QCDSIDEBANDCORR_H

// a method to get the QCD sideband correction factor
// corr_{i} = S_{i}^{anti} / U+D_{i}^{anti}

namespace QcdSidebandCorr{

	double GetCorr(const unsigned int& binIndex, const unsigned int& yearOfRun){

		// #*,. #*,. #*,. #*,.
		// #*,. 2016 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		if (yearOfRun == 2016){

			std::vector<double> corrVec;
			
			// ht1500to2500 - normal method
			//corrVec.push_back(1.6064209275);
			//corrVec.push_back(0.7789782091);
			//corrVec.push_back(0.8267942584);
			//corrVec.push_back(0.9253012048);
			//corrVec.push_back(0.9914814815);
			//corrVec.push_back(1.1304957905);
			//corrVec.push_back(1.1731970471);
			//corrVec.push_back(1.1436619718);
			//corrVec.push_back(1.0693493151);
			//corrVec.push_back(1.1156626506);

			// MHT < 200
			// ht1500to2500 - calculation (data)
			corrVec.push_back(1.84907595049);
			corrVec.push_back(0.945526377036);
			corrVec.push_back(0.987699763291);
			corrVec.push_back(1.02582043623);
			corrVec.push_back(1.06562890796);
			corrVec.push_back(1.11024323865);
			corrVec.push_back(1.15958785532);
			corrVec.push_back(1.20505895315);
			corrVec.push_back(1.21496370615);
			corrVec.push_back(1.11762437396);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.82037428762);
			corrVec.push_back(0.931254447234);
			corrVec.push_back(0.966389962047);
			corrVec.push_back(0.976410227103);
			corrVec.push_back(0.973735853407);
			corrVec.push_back(0.964938452518);
			corrVec.push_back(0.955672093232);
			corrVec.push_back(0.953018962627);
			corrVec.push_back(0.967454159366);
			corrVec.push_back(1.01739316583);

			// ht3500toInf - calculation (mc)
			corrVec.push_back(1.62553761844);
			corrVec.push_back(0.794338776641);
			corrVec.push_back(0.863959817491);
			corrVec.push_back(0.919093530095);
			corrVec.push_back(0.961310528178);
			corrVec.push_back(0.990654436176);
			corrVec.push_back(1.00376549778);
			corrVec.push_back(0.99293787768);
			corrVec.push_back(0.947551245113);
			corrVec.push_back(0.860924159953);

			// MHT > 200
                        // ht1500to2500 - calculation (data)
                        corrVec.push_back(1.90964574988);
                        corrVec.push_back(0.968688587782);
                        corrVec.push_back(0.998298804197);
                        corrVec.push_back(1.02804038044);
                        corrVec.push_back(1.0627331223);
                        corrVec.push_back(1.10506418153);
                        corrVec.push_back(1.15542912331);
                        corrVec.push_back(1.20681356533);
                        corrVec.push_back(1.22916231807);
                        corrVec.push_back(1.14519002209);

                        // ht2500to3500 - calculation (data)
                        corrVec.push_back(1.40283865752);
                        corrVec.push_back(0.761693206177);
                        corrVec.push_back(0.961069912837);
                        corrVec.push_back(1.12294785625);
                        corrVec.push_back(1.24581842476);
                        corrVec.push_back(1.33847745653);
                        corrVec.push_back(1.36847660502);
                        corrVec.push_back(1.21374183211);
                        corrVec.push_back(0.806809499626);
                        corrVec.push_back(0.419189890082);

                        // ht3500toInf - calculation (mc)
                        corrVec.push_back(1.68206689165);
                        corrVec.push_back(0.832908223575);
                        corrVec.push_back(0.907813542036);
                        corrVec.push_back(0.966111912793);
                        corrVec.push_back(1.00724787057);
                        corrVec.push_back(1.0226077373);
                        corrVec.push_back(0.991808377729);
                        corrVec.push_back(0.890633652545);
                        corrVec.push_back(0.723532810558);
                        corrVec.push_back(0.544236393055);


			if (binIndex > 0 && binIndex < corrVec.size() + 1 ) return corrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2016 sideband correction factor" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2016


		// #*,. #*,. #*,. #*,.
		// #*,. 2017 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		else if (yearOfRun == 2017){
			
			std::vector<double> corrVec;

			// ht1500to2500 - normal method
			//corrVec.push_back(1.7026466009);
			//corrVec.push_back(0.7909370200);
			//corrVec.push_back(0.8413654618);
			//corrVec.push_back(0.8451202263);
			//corrVec.push_back(1.0144796380);
			//corrVec.push_back(1.0905271199);
			//corrVec.push_back(1.1422745809);
			//corrVec.push_back(1.1844380403);
			//corrVec.push_back(1.0975778547);
			//corrVec.push_back(1.0845724907);

			// MHT < 200
			// ht1500to2500 - calculation (data)
			corrVec.push_back(1.85838042696);
			corrVec.push_back(0.95397292008);
			corrVec.push_back(0.995979165345);
			corrVec.push_back(1.03481866298);
			corrVec.push_back(1.07654584563);
			corrVec.push_back(1.1249790978);
			corrVec.push_back(1.18129186417);
			corrVec.push_back(1.23841210337);
			corrVec.push_back(1.26332461545);
			corrVec.push_back(1.16970608796);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.74770249038);
			corrVec.push_back(0.892730942915);
			corrVec.push_back(0.951015653348);
			corrVec.push_back(0.993605387557);
			corrVec.push_back(1.02634862166);
			corrVec.push_back(1.04963640571);
			corrVec.push_back(1.05779049067);
			corrVec.push_back(1.03757959758);
			corrVec.push_back(0.970194383711);
			corrVec.push_back(0.845135808837);

			// ht3500toInf - calculation (mc)
			corrVec.push_back(1.47280147426);
			corrVec.push_back(0.712654067795);
			corrVec.push_back(0.820925108035);
			corrVec.push_back(0.924387320877);
			corrVec.push_back(1.01427057977);
			corrVec.push_back(1.08934136409);
			corrVec.push_back(1.14741864111);
			corrVec.push_back(1.17550032466);
			corrVec.push_back(1.1397375992);
			corrVec.push_back(0.995117315807);

			// MHT > 200
                        // ht1500to2500 - calculation (data)
                        corrVec.push_back(1.81608340849);
                        corrVec.push_back(0.948521743031);
                        corrVec.push_back(1.01083245614);
                        corrVec.push_back(1.06376087405);
                        corrVec.push_back(1.11847855738);
                        corrVec.push_back(1.18254636841);
                        corrVec.push_back(1.25919713286);
                        corrVec.push_back(1.33587828434);
                        corrVec.push_back(1.34369797014);
                        corrVec.push_back(1.12068327091);

                        // ht2500to3500 - calculation (data)
                        corrVec.push_back(1.66190919794);
                        corrVec.push_back(0.829649211625);
                        corrVec.push_back(0.895704734337);
                        corrVec.push_back(0.94653834391);
                        corrVec.push_back(0.989080815796);
                        corrVec.push_back(1.02851005567);
                        corrVec.push_back(1.06898294997);
                        corrVec.push_back(1.11424871845);
                        corrVec.push_back(1.16800437392);
                        corrVec.push_back(1.23361312652);

                        // ht3500toInf - calculation (mc)
                        corrVec.push_back(1.6870168763);
                        corrVec.push_back(0.769079044761);
                        corrVec.push_back(0.794828216044);
                        corrVec.push_back(0.817079465951);
                        corrVec.push_back(0.838438118168);
                        corrVec.push_back(0.861296637188);
                        corrVec.push_back(0.888091604263);
                        corrVec.push_back(0.921863334725);
                        corrVec.push_back(0.967276704781);
                        corrVec.push_back(1.03285242496);


			if (binIndex > 0 && binIndex < corrVec.size() + 1 ) return corrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2017 sideband correction factor" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2017

		else{
			std::cout << "You have not provided QcdSidebandCorr::GetCorr with a valid yearOfRun, must be 2016 or 2017" << std::endl;
			std::cout << "You are being given a dummy value of -2635426354.0" << std::endl;
			return -2635426354.0;
		}

	} // closes function GetCorr




	double GetCorrErr(const unsigned int& binIndex, const unsigned int& yearOfRun){

		// #*,. #*,. #*,. #*,.
		// #*,. 2016 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		if (yearOfRun == 2016){

			std::vector<double> corrErrVec;

			// ht1500to2500 - normal method
			//corrErrVec.push_back(0.0352796250);
			//corrErrVec.push_back(0.0159298571);
			//corrErrVec.push_back(0.0190088308);
			//corrErrVec.push_back(0.0231644431);
			//corrErrVec.push_back(0.0270426071);
			//corrErrVec.push_back(0.0335637876);
			//corrErrVec.push_back(0.0380500609);
			//corrErrVec.push_back(0.0415511282);
			//corrErrVec.push_back(0.0435266537);
			//corrErrVec.push_back(0.0533274246);

			// ht1500to2500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht2500to3500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht3500toInf - calculation (mc)
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);

                        // TEMPORARY: Set correction factor errors to 0.1 for now for MHT>200 bins
                        for(int i=0; i<30; i++) corrErrVec.push_back(0.30);

			if (binIndex > 0 && binIndex < corrErrVec.size() + 1 ) return corrErrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2016 sideband correction factor error" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2016


		// #*,. #*,. #*,. #*,.
		// #*,. 2017 #*,. #*,.
		// #*,. #*,. #*,. #*,.
		if (yearOfRun == 2017){

			std::vector<double> corrErrVec;

			// ht1500to2500 - normal method
			corrErrVec.push_back(0.0345542152);
			corrErrVec.push_back(0.0147509712);
			corrErrVec.push_back(0.0176379321);
			corrErrVec.push_back(0.0191728313);
			corrErrVec.push_back(0.0248291208);
			corrErrVec.push_back(0.0295094637);
			corrErrVec.push_back(0.0332982690);
			corrErrVec.push_back(0.0386168015);
			corrErrVec.push_back(0.0399156117);
			corrErrVec.push_back(0.0458386330);

			// ht1500to2500 - calculation (data)
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);
			// corrErrVec.push_back(0.15);

			// ht2500to3500 - calculation (data)
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);
			corrErrVec.push_back(0.15);

			// ht3500toInf - calculation (mc)
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);
			corrErrVec.push_back(0.30);

                        // TEMPORARY: Set correction factor errors to 0.1 for now for MHT>200 bins
                        for(int i=0; i<30; i++) corrErrVec.push_back(0.30);

			if (binIndex > 0 && binIndex < corrErrVec.size() + 1 ) return corrErrVec[binIndex - 1];
			else{
				std::cout << "WARNING: you haven't provided a valid index for the QCD 2017 sideband correction factor error" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				return -123456789.0;
			}
		} // closes 'if' yearOfRun==2016

		else{
			std::cout << "You have not provided QcdSidebandCorr::GetCorrErr with a valid yearOfRun, must be 2016 or 2017" << std::endl;
			std::cout << "You are being given a dummy value of -2635426354.0" << std::endl;
			return -2635426354.0;
		}

	} // closes function GetCorrErr


} // closes namespace 'QcdSidebandCorr'
#endif
