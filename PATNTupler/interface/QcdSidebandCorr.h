#ifndef QCDSIDEBANDCORR_H
#define QCDSIDEBANDCORR_H

// a method to get the QCD sideband correction factor
// corr_{i} = S_{i}^{anti} / U+D_{i}^{anti}

namespace QcdSidebandCorr{


	double GetCorr(const unsigned int& binIndex){

		std::vector<double> corrVec;

		// ht1500to2500
		corrVec.push_back(1.60833704142);
		corrVec.push_back(0.80221720466);
		corrVec.push_back(0.886553353762);
		corrVec.push_back(0.950280919979);
		corrVec.push_back(0.991033391582);
		corrVec.push_back(1.01697345868);
		corrVec.push_back(1.03757626984);
		corrVec.push_back(1.06198171606);
		corrVec.push_back(1.1023962518);
		corrVec.push_back(1.18555402258);

		// ht2500to3500
		corrVec.push_back(1.59651535195);
		corrVec.push_back(0.759337455383);
		corrVec.push_back(0.829678000509);
		corrVec.push_back(0.890113310079);
		corrVec.push_back(0.936008629276);
		corrVec.push_back(0.968961931082);
		corrVec.push_back(0.993120469095);
		corrVec.push_back(1.01295812109);
		corrVec.push_back(1.0329034113);
		corrVec.push_back(1.05822523313);

		// ht3500toInf
		corrVec.push_back(1.73517081892);
		corrVec.push_back(0.815197125747);
		corrVec.push_back(0.845843649954);
		corrVec.push_back(0.867316833384);
		corrVec.push_back(0.882689955392);
		corrVec.push_back(0.894054962976);
		corrVec.push_back(0.90272317832);
		corrVec.push_back(0.909524451809);
		corrVec.push_back(0.914998217392);
		corrVec.push_back(0.919505434215);

		if (binIndex > 0 && binIndex < corrVec.size() + 1 ) return corrVec[binIndex - 1];
		else{
			std::cout << "WARNING: you haven't provided a valid index for the QCD sideband correction factor" << std::endl;
			std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
			return -123456789.0;
		}
	}

} // closes namespace 'QcdSidebandCorr'
#endif