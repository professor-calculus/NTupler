#ifndef QCDSIDEBANDCORR_H
#define QCDSIDEBANDCORR_H

// a method to get the QCD sideband correction factor
// corr_{i} = S_{i}^{anti} / U+D_{i}^{anti}

namespace QcdSidebandCorr{


	double GetCorr(const std::string& htLabel, const unsigned int& binIndex){

		std::vector<double> ht1500to2500;
		ht1500to2500.push_back(1.60833704142);
		ht1500to2500.push_back(0.80221720466);
		ht1500to2500.push_back(0.886553353762);
		ht1500to2500.push_back(0.950280919979);
		ht1500to2500.push_back(0.991033391582);
		ht1500to2500.push_back(1.01697345868);
		ht1500to2500.push_back(1.03757626984);
		ht1500to2500.push_back(1.06198171606);
		ht1500to2500.push_back(1.1023962518);
		ht1500to2500.push_back(1.18555402258);

		std::vector<double> ht2500to3500;
		ht2500to3500.push_back(1.59651535195);
		ht2500to3500.push_back(0.759337455383);
		ht2500to3500.push_back(0.829678000509);
		ht2500to3500.push_back(0.890113310079);
		ht2500to3500.push_back(0.936008629276);
		ht2500to3500.push_back(0.968961931082);
		ht2500to3500.push_back(0.993120469095);
		ht2500to3500.push_back(1.01295812109);
		ht2500to3500.push_back(1.0329034113);
		ht2500to3500.push_back(1.05822523313);

		std::vector<double> ht3500toInf;
		ht3500toInf.push_back(1.73517081892);
		ht3500toInf.push_back(0.815197125747);
		ht3500toInf.push_back(0.845843649954);
		ht3500toInf.push_back(0.867316833384);
		ht3500toInf.push_back(0.882689955392);
		ht3500toInf.push_back(0.894054962976);
		ht3500toInf.push_back(0.90272317832);
		ht3500toInf.push_back(0.909524451809);
		ht3500toInf.push_back(0.914998217392);
		ht3500toInf.push_back(0.919505434215);

		if (htLabel == "ht1500to2500"){
			if (binIndex > 0 && binIndex < ht1500to2500.size() + 1){
				return ht1500to2500[binIndex - 1];
			}
			else{
				std::cout << "WARNING: you haven't provided a valid index for the ht1500to2500 correction" << std::endl;
				std::cout << "It should be between 1 and " << ht1500to2500.size() << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				std::cout << std::endl;
				return -123456789.0;
			}
		}
		else if (htLabel == "ht2500to3500"){
			if (binIndex > 0 && binIndex < ht2500to3500.size() + 1){
				return ht2500to3500[binIndex - 1];
			}
			else{
				std::cout << "WARNING: you haven't provided a valid index for the ht2500to3500 correction" << std::endl;
				std::cout << "It should be between 1 and " << ht2500to3500.size() << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				std::cout << std::endl;
				return -123456789.0;
			}
		}
		else if (htLabel == "ht3500toInf"){
			if (binIndex > 0 && binIndex < ht3500toInf.size() + 1){
				return ht3500toInf[binIndex - 1];
			}
			else{
				std::cout << "WARNING: you haven't provided a valid index for the ht3500toInf correction" << std::endl;
				std::cout << "It should be between 1 and " << ht3500toInf.size() << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				std::cout << std::endl;
				return -123456789.0;
			}
		}
		else{
			std::cout << "WARNING: you haven't provided a valid htLabel for your desired correction" << std::endl;
				std::cout << "You are being given a dummy value of -123456789.0" << std::endl;
				std::cout << std::endl;
				return -123456789.0;
		}
	}

} // closes namespace 'QcdSidebandCorr'
#endif