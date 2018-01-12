#ifndef QCDSIDEBANDCORR_H
#define QCDSIDEBANDCORR_H

// a method to get the QCD sideband correction factor
// corr_{i} = S_{i}^{anti} / U+D_{i}^{anti}

namespace QcdSidebandCorr{


	double GetCorr(const unsigned int& binIndex){

		std::vector<double> corrVec;

		// ht1500to2500
		corrVec.push_back(1.72095039307);
		corrVec.push_back(0.8282414859);
		corrVec.push_back(0.863177034516);
		corrVec.push_back(0.905665580354);
		corrVec.push_back(0.947521934812);
		corrVec.push_back(0.985142191059);
		corrVec.push_back(1.0195367264);
		corrVec.push_back(1.05554429897);
		corrVec.push_back(1.10290856858);
		corrVec.push_back(1.18358319242);

		// ht2500to3500
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

		// ht3500toInf
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

} // closes namespace 'QcdSidebandCorr'
#endif