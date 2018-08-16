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
			corrVec.push_back(1.6117647059);
			corrVec.push_back(0.7819068438);
			corrVec.push_back(0.8270412643);
			corrVec.push_back(0.9380234506);
			corrVec.push_back(1.0082107424);
			corrVec.push_back(1.1371107266);
			corrVec.push_back(1.1930379747);
			corrVec.push_back(1.1702970297);
			corrVec.push_back(1.0292028414);
			corrVec.push_back(1.1413911060);

			// ht1500to2500 - calculation (data)
			// corrVec.push_back(1.56105538676);
			// corrVec.push_back(0.792299026394);
			// corrVec.push_back(0.889936752445);
			// corrVec.push_back(0.947097864249);
			// corrVec.push_back(0.989311312588);
			// corrVec.push_back(1.02784719536);
			// corrVec.push_back(1.06845446476);
			// corrVec.push_back(1.11428452007);
			// corrVec.push_back(1.16515340653);
			// corrVec.push_back(1.21160736675);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.42166748165);
			corrVec.push_back(0.669549128221);
			corrVec.push_back(0.792822431347);
			corrVec.push_back(0.878879303559);
			corrVec.push_back(0.937638694061);
			corrVec.push_back(0.98046015031);
			corrVec.push_back(1.01337834624);
			corrVec.push_back(1.03750844502);
			corrVec.push_back(1.04875007842);
			corrVec.push_back(1.03653473091);

			// ht3500toInf - calculation (data - DONT USE OTHER THAN FOR TESTING)
			// corrVec.push_back(1.48651067592);
			// corrVec.push_back(0.635513771772);
			// corrVec.push_back(0.727779037684);
			// corrVec.push_back(0.817995095171);
			// corrVec.push_back(0.892466574728);
			// corrVec.push_back(0.954343628094);
			// corrVec.push_back(1.00894930602);
			// corrVec.push_back(1.06081800203);
			// corrVec.push_back(1.11236822796);
			// corrVec.push_back(1.16134135229);

			// ht3500toInf - calculation (mc)
			corrVec.push_back(1.54454642992);
			corrVec.push_back(0.626045049413);
			corrVec.push_back(0.699470534868);
			corrVec.push_back(0.790415782096);
			corrVec.push_back(0.868625490307);
			corrVec.push_back(0.931039189643);
			corrVec.push_back(0.97896497494);
			corrVec.push_back(1.01224337229);
			corrVec.push_back(1.02620541694);
			corrVec.push_back(1.00999287037);

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
			corrVec.push_back(1.6940559441);
			corrVec.push_back(0.7933126202);
			corrVec.push_back(0.8462135922);
			corrVec.push_back(0.8496479673);
			corrVec.push_back(1.0245398773);
			corrVec.push_back(1.1229689808);
			corrVec.push_back(1.1445940078);
			corrVec.push_back(1.1751662971);
			corrVec.push_back(1.1205626256);
			corrVec.push_back(1.1140035907);

			// ht1500to2500 - calculation (data)
			// corrVec.push_back(1.56773033051);
			// corrVec.push_back(0.798787444199);
			// corrVec.push_back(0.896064074759);
			// corrVec.push_back(0.953870377466);
			// corrVec.push_back(0.996625124185);
			// corrVec.push_back(1.03593158979);
			// corrVec.push_back(1.07805514982);
			// corrVec.push_back(1.12700691212);
			// corrVec.push_back(1.184241969);
			// corrVec.push_back(1.24318923937);

			// ht2500to3500 - calculation (data)
			corrVec.push_back(1.41435606799);
			corrVec.push_back(0.683767852841);
			corrVec.push_back(0.811049770514);
			corrVec.push_back(0.893125933632);
			corrVec.push_back(0.947513233695);
			corrVec.push_back(0.986921125125);
			corrVec.push_back(1.01737060935);
			corrVec.push_back(1.03985622243);
			corrVec.push_back(1.05040877646);
			corrVec.push_back(1.03897771658);

			// ht3500toInf - calculation (data)
			corrVec.push_back(1.57941107132);
			corrVec.push_back(0.695522760024);
			corrVec.push_back(0.77401166431);
			corrVec.push_back(0.850143377131);
			corrVec.push_back(0.9113400316);
			corrVec.push_back(0.960810806759);
			corrVec.push_back(1.0011396109);
			corrVec.push_back(1.03118830076);
			corrVec.push_back(1.04352959767);
			corrVec.push_back(1.02178803851);

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
			corrErrVec.push_back(0.0339370552);
			corrErrVec.push_back(0.0153063287);
			corrErrVec.push_back(0.0182115080);
			corrErrVec.push_back(0.0225280484);
			corrErrVec.push_back(0.0263187886);
			corrErrVec.push_back(0.0324205946);
			corrErrVec.push_back(0.0371476242);
			corrErrVec.push_back(0.0409450422);
			corrErrVec.push_back(0.0405998951);
			corrErrVec.push_back(0.0527916751);

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
			corrErrVec.push_back(0.0337613765);
			corrErrVec.push_back(0.0145080472);
			corrErrVec.push_back(0.0174171606);
			corrErrVec.push_back(0.0188925121);
			corrErrVec.push_back(0.0246163780);
			corrErrVec.push_back(0.0296709447);
			corrErrVec.push_back(0.0326476036);
			corrErrVec.push_back(0.0376424281);
			corrErrVec.push_back(0.0398945632);
			corrErrVec.push_back(0.0459783689);

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