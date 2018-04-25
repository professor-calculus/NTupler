#ifndef QCDSIDEBANDCORR_H
#define QCDSIDEBANDCORR_H

// a method to get the QCD sideband correction factor
// corr_{i} = S_{i}^{anti} / U+D_{i}^{anti}

namespace QcdSidebandCorr{


	double GetCorr(const unsigned int& binIndex){

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

		// ht1500to2500 - calculation
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

		// ht2500to3500 - calculation
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

		// ht3500toInf - calculation
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
			std::cout << "WARNING: you haven't provided a valid index for the QCD sideband correction factor" << std::endl;
			std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
			return -123456789.0;
		}
	}

	double GetCorrErr(const unsigned int& binIndex){

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

		// ht1500to2500 - calculation
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

		// ht2500to3500 - calculation
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

		// ht3500toInf - calculation
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
			std::cout << "WARNING: you haven't provided a valid index for the QCD sideband correction factor error" << std::endl;
			std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
			return -123456789.0;
		}
	}

} // closes namespace 'QcdSidebandCorr'
#endif