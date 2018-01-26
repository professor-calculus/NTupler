#ifndef QCDSIDEBANDCORR_H
#define QCDSIDEBANDCORR_H

// a method to get the QCD sideband correction factor
// corr_{i} = S_{i}^{anti} / U+D_{i}^{anti}

namespace QcdSidebandCorr{


	double GetCorr(const unsigned int& binIndex){

		std::vector<double> corrVec;

		// ht1500to2500 - normal method
		corrVec.push_back(1.6645285439);
		corrVec.push_back(0.8743148003);
		corrVec.push_back(0.8934723481);
		corrVec.push_back(0.8319770225);
		corrVec.push_back(0.8696296296);
		corrVec.push_back(0.9246206559);
		corrVec.push_back(1.0578193833);
		corrVec.push_back(1.1185819071);
		corrVec.push_back(1.2488584475);
		corrVec.push_back(1.1579846285);

		// ht2500to3500 - calculation
		corrVec.push_back(1.55514522072);
		corrVec.push_back(0.714415425744);
		corrVec.push_back(0.794138432615);
		corrVec.push_back(0.869773121413);
		corrVec.push_back(0.928009321778);
		corrVec.push_back(0.968194116605);
		corrVec.push_back(0.995502359265);
		corrVec.push_back(1.01589601563);
		corrVec.push_back(1.03472134577);
		corrVec.push_back(1.05736017431);

		// ht3500toInf - calculation
		corrVec.push_back(1.76016501196);
		corrVec.push_back(0.814215526066);
		corrVec.push_back(0.837869954443);
		corrVec.push_back(0.857639244302);
		corrVec.push_back(0.872958566091);
		corrVec.push_back(0.884852369897);
		corrVec.push_back(0.894246365298);
		corrVec.push_back(0.901816607555);
		corrVec.push_back(0.908039235252);
		corrVec.push_back(0.913250205158);

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
		corrErrVec.push_back(0.0533375121);
		corrErrVec.push_back(0.0253305568);
		corrErrVec.push_back(0.0276928267);
		corrErrVec.push_back(0.0270113604);
		corrErrVec.push_back(0.0283356018);
		corrErrVec.push_back(0.0295134639);
		corrErrVec.push_back(0.0346219452);
		corrErrVec.push_back(0.0380596574);
		corrErrVec.push_back(0.0462317130);
		corrErrVec.push_back(0.0461952289);

		// ht2500to3500 - calculation
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);

		// ht3500toInf - calculation
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);
		corrErrVec.push_back(0.1);

		if (binIndex > 0 && binIndex < corrErrVec.size() + 1 ) return corrErrVec[binIndex - 1];
		else{
			std::cout << "WARNING: you haven't provided a valid index for the QCD sideband correction factor error" << std::endl;
			std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
			return -123456789.0;
		}
	}

} // closes namespace 'QcdSidebandCorr'
#endif