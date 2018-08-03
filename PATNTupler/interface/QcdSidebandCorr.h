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
			corrVec.push_back(1.5643122677);
			corrVec.push_back(0.8191290409);
			corrVec.push_back(0.8173270651);
			corrVec.push_back(0.9315403423);
			corrVec.push_back(1.0348660940);
			corrVec.push_back(1.1204220981);
			corrVec.push_back(1.1502669718);
			corrVec.push_back(1.2842535787);
			corrVec.push_back(1.0676605505);
			corrVec.push_back(1.1622073579);

			// ht1500to2500 - calculation (data)
			// corrVec.push_back(1.55990157594);
			// corrVec.push_back(0.804516719925);
			// corrVec.push_back(0.911643102591);
			// corrVec.push_back(0.970208983103);
			// corrVec.push_back(1.00368715753);
			// corrVec.push_back(1.02741318035);
			// corrVec.push_back(1.05039765545);
			// corrVec.push_back(1.07935948312);
			// corrVec.push_back(1.12216985282);
			// corrVec.push_back(1.19267399065);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.26323270455);
			corrVec.push_back(0.621985429413);
			corrVec.push_back(0.80296800186);
			corrVec.push_back(0.914306254778);
			corrVec.push_back(0.973196867169);
			corrVec.push_back(1.00481759311);
			corrVec.push_back(1.02464438558);
			corrVec.push_back(1.04091905893);
			corrVec.push_back(1.05850716439);
			corrVec.push_back(1.08129800885);

			// ht3500toInf - calculation (mc)
			corrVec.push_back(1.53236672999);
			corrVec.push_back(0.660849709747);
			corrVec.push_back(0.739304126494);
			corrVec.push_back(0.811096563425);
			corrVec.push_back(0.869115645745);
			corrVec.push_back(0.913344760852);
			corrVec.push_back(0.946930174938);
			corrVec.push_back(0.973551363022);
			corrVec.push_back(0.996480775465);
			corrVec.push_back(1.01854059246);

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
			corrVec.push_back(1.7114716107);
			corrVec.push_back(0.7954788453);
			corrVec.push_back(0.8448851365);
			corrVec.push_back(0.8616922362);
			corrVec.push_back(1.0563165906);
			corrVec.push_back(1.1045824095);
			corrVec.push_back(1.1701077199);
			corrVec.push_back(1.1388733965);
			corrVec.push_back(1.1362378976);
			corrVec.push_back(1.0981651376);

			// ht1500to2500 - calculation (data)
			// corrVec.push_back(1.58066471071);
			// corrVec.push_back(0.815505500068);
			// corrVec.push_back(0.917632916305);
			// corrVec.push_back(0.974664394329);
			// corrVec.push_back(1.00840819801);
			// corrVec.push_back(1.03345207715);
			// corrVec.push_back(1.05884605862);
			// corrVec.push_back(1.09200827412);
			// corrVec.push_back(1.14262581336);
			// corrVec.push_back(1.22941825926);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.36821057384);
			corrVec.push_back(0.708853882796);
			corrVec.push_back(0.860372992135);
			corrVec.push_back(0.929753811738);
			corrVec.push_back(0.960672017891);
			corrVec.push_back(0.975847144105);
			corrVec.push_back(0.984935716667);
			corrVec.push_back(0.99212178519);
			corrVec.push_back(0.999451808894);
			corrVec.push_back(1.00818269906);

			// ht3500toInf - calculation (mc)
			corrVec.push_back(1.61683549002);
			corrVec.push_back(0.709731171249);
			corrVec.push_back(0.773904007144);
			corrVec.push_back(0.818038291477);
			corrVec.push_back(0.84640923239);
			corrVec.push_back(0.865604508024);
			corrVec.push_back(0.879268556577);
			corrVec.push_back(0.889414037611);
			corrVec.push_back(0.897209356967);
			corrVec.push_back(0.903368705215);

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
			corrErrVec.push_back(0.0386163339);
			corrErrVec.push_back(0.0199525313);
			corrErrVec.push_back(0.0223332610);
			corrErrVec.push_back(0.0270779291);
			corrErrVec.push_back(0.0326202361);
			corrErrVec.push_back(0.0384020336);
			corrErrVec.push_back(0.0434354346);
			corrErrVec.push_back(0.0547681564);
			corrErrVec.push_back(0.0503150579);
			corrErrVec.push_back(0.0648246064);

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
			corrErrVec.push_back(0.0327941954);
			corrErrVec.push_back(0.0147700952);
			corrErrVec.push_back(0.0174943038);
			corrErrVec.push_back(0.0193105583);
			corrErrVec.push_back(0.0257142791);
			corrErrVec.push_back(0.0293101562);
			corrErrVec.push_back(0.0337594976);
			corrErrVec.push_back(0.0368587089);
			corrErrVec.push_back(0.0409708804);
			corrErrVec.push_back(0.0459769768);

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