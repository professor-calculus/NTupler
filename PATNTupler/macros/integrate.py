import math
from scipy import integrate

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/integrate.py

# notes on script:
# - it is very much geometry specific and not completely general for any MassRegion object!
# - assumes indendence between the 2 fatJet mass distributions (good for QCD)
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################

function_1d = {}

# FUNCTIONS #
# all these functions are 'normalised', by which we do not mean that the area under the function is unity...
# ...instead we mean that all the functions come from normalised histograms (50 or 80 bins between 0 and 200 GeV)
# 'fullCutsData' means that the cuts were: preSel + 2*fatJet300 + 1*ak4pt300 + GIVEN_DBT_REGION + GIVEN_HT_BIN, preformed on Data
# 'fullCutsMC' means that the cuts were: preSel + 2*fatJet300 + 1*ak4pt300 + GIVEN_DBT_REGION + GIVEN_HT_BIN, preformed on MC
# 'specCutsData' means that the cuts were: preSel + 2*fatJet300 + ht1500to2500 + GIVEN_DBT_REGION + fatJetB_DBT < 0.3, preformed on Data
# fits valid between 15 and 200 GeV, softDropMassPUPPI is the mass used

#*,. #*,. #*,. #*,.
#*,. 2016 #*,. #*,.
def f1d_specCutsData_anti_2016(x):
	p0  =  3.73538e-03
	p1  =  2.17615e-01
	p2  = -3.99795e+00
	p3  =  3.21993e-01
	p4  =  3.04118e+00
	p5  =  2.57828e+01
	p6  = -1.76060e+00
	p7  =  1.09478e+03
	p8  = -4.63207e+00
	p9  =  1.32862e-06
	p10 = -4.77966e+02
	p11 = -6.34208e-08
	p12 = -1.04249e+01
	p13 = -1.54668e-10
	p14 = -5.50088e+01
	p15 =  1.54589e-12
	p16 = -6.38564e+00
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["specCutsData_anti_2016"] = f1d_specCutsData_anti_2016

def f1d_specCutsData_tagLoose_2016(x):
	p0  =  1.40060e-03
	p1  =  2.68626e-01
	p2  = -9.84819e-04
	p3  =  3.07646e+00
	p4  = -5.48608e-03
	p5  =  5.01522e+00
	p6  = -4.82310e-03
	p7  =  3.55667e+02
	p8  = -1.14966e-02
	p9  = -2.33848e-06
	p10 = -1.85145e-05
	p11 = -2.86977e-08
	p12 =  4.13376e-05
	p13 = -6.44486e-11
	p14 =  5.43466e-05
	p15 =  5.32593e-13
	p16 =  3.13356e-04
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["specCutsData_tagLoose_2016"] = f1d_specCutsData_tagLoose_2016

def f1d_fullCutsData_anti_ht1500to2500_2016(x): # Updated
	p0  =  5.94606e-03
	p1  =  2.91997e-01
	p2  = -1.27428e-02
	p3  = -3.31572e+00
	p4  = -3.58251e-02
	p5  =  1.21135e+02
	p6  =  1.76960e-02
	p7  = -8.86758e+02
	p8  = -3.59148e-01
	p9  = -1.10648e-05
	p10 =  1.54069e-03
	p11 = -1.61696e-07
	p12 = -5.80594e-04
	p13 = -3.59516e-10
	p14 = -2.79939e-03
	p15 =  3.33622e-12
	p16 =  2.89569e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht1500to2500_2016"] = f1d_fullCutsData_anti_ht1500to2500_2016

def f1d_fullCutsData_anti_ht1500to2500_MHT200_2016(x): # Updated
        p0  =  2.63522e-03
        p1  =  5.55200e-01
        p2  = -6.81119e+00
        p3  = -5.59044e+00
        p4  = -6.98463e+00
        p5  = -3.74312e+01
        p6  = -2.03275e+00
        p7  =  2.87593e+03
        p8  = -3.87913e+00
        p9  = -1.48131e-05
        p10 =  6.70106e+01
        p11 = -9.93423e-08
        p12 = -2.11753e+01
        p13 = -6.69055e-11
        p14 = -1.79863e+01
        p15 =  1.94427e-12
        p16 = -1.07844e+00
        return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht1500to2500_MHT200_2016"] = f1d_fullCutsData_anti_ht1500to2500_MHT200_2016

def f1d_fullCutsData_anti_ht2500to3500_2016(x): # Updated
	p0  =  5.06465e-03
	p1  =  1.25039e+00
	p2  = -9.82123e+01
	p3  = -1.70248e+01
	p4  = -3.61889e+01
	p5  =  1.24085e+02
	p6  =  4.23586e+00
	p7  = -9.83750e+02
	p8  =  4.00462e+00
	p9  = -8.69613e-05
	p10 = -2.21315e+01
	p11 =  4.98772e-08
	p12 = -1.85011e+02
	p13 =  2.57779e-09
	p14 = -1.57901e+01
	p15 = -4.47720e-12
	p16 = -6.17737e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht2500to3500_2016"] = f1d_fullCutsData_anti_ht2500to3500_2016

def f1d_fullCutsData_anti_ht2500to3500_MHT200_2016(x): # Updated
        p0  =  3.28558e-02
        p1  = -2.13599e+00
        p2  = -1.92140e+01
        p3  =  5.20754e+00
        p4  =  4.06201e+00
        p5  =  1.85006e+03
        p6  = -8.81885e+00
        p7  = -1.38930e+04
        p8  = -3.88335e+00
        p9  = -2.82908e-06
        p10 =  1.17998e+03
        p11 = -4.82899e-07
        p12 = -5.78717e+01
        p13 = -5.52015e-10
        p14 = -1.35169e+02
        p15 =  6.15409e-12
        p16 = -5.98556e+01
        return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht2500to3500_MHT200_2016"] = f1d_fullCutsData_anti_ht2500to3500_MHT200_2016

def f1d_fullCutsMC_anti_ht3500toInf_2016(x): # Updated
	p0  =  4.66078e-03
	p1  = -1.63246e-02
	p2  =  4.20427e-03
	p3  =  6.45728e+00
	p4  =  3.76180e-02
	p5  =  2.32718e+02
	p6  = -1.42631e-02
	p7  = -2.98957e+03
	p8  =  1.20076e-01
	p9  =  2.22533e-06
	p10 = -3.25420e-04
	p11 = -1.17381e-07
	p12 =  8.26025e-05
	p13 = -3.72059e-10
	p14 = -6.88261e-04
	p15 =  2.70078e-12
	p16 =  3.02114e-04
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsMC_anti_ht3500toInf_2016"] = f1d_fullCutsMC_anti_ht3500toInf_2016

def f1d_fullCutsMC_anti_ht3500toInf_MHT200_2016(x): # Updated
        p0  =  5.34694e-03
        p1  =  1.29331e-02
        p2  = -1.96692e-04
        p3  =  4.33529e+00
        p4  =  2.77999e-02
        p5  =  2.55245e+02
        p6  = -1.68494e-02
        p7  = -2.98167e+03
        p8  =  3.36715e-02
        p9  = -4.97150e-06
        p10 =  1.13570e-03
        p11 = -1.71377e-07
        p12 =  6.35414e-04
        p13 = -3.79205e-10
        p14 = -8.73406e-05
        p15 =  4.41180e-12
        p16 =  2.67305e-03
        return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsMC_anti_ht3500toInf_MHT200_2016"] = f1d_fullCutsMC_anti_ht3500toInf_MHT200_2016

#*,. #*,. #*,. #*,.
#*,. 2017 #*,. #*,.
def f1d_specCutsData_anti_2017(x):
	p0  =  3.06265e-03
	p1  =  2.83999e-01
	p2  = -3.81039e-03
	p3  =  7.14707e-02
	p4  = -5.39992e-03
	p5  = -1.75432e+01
	p6  = -3.61788e-03
	p7  =  5.76027e+02
	p8  = -1.33423e-02
	p9  =  5.44448e-07
	p10 = -3.01546e-04
	p11 = -3.98188e-08
	p12 =  5.30418e-05
	p13 = -1.18476e-10
	p14 =  6.79786e-05
	p15 =  7.08076e-13
	p16 =  3.53151e-04
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["specCutsData_anti_2017"] = f1d_specCutsData_anti_2017

def f1d_specCutsData_tagLoose_2017(x):
	p0  =  1.24637e-03
	p1  =  2.50924e-01
	p2  = -6.25704e-04
	p3  =  3.31444e+00
	p4  = -5.00434e-03
	p5  =  8.34498e+00
	p6  = -5.06778e-03
	p7  =  2.54845e+02
	p8  = -1.11619e-02
	p9  = -1.96489e-06
	p10 = -1.27259e-05
	p11 = -2.48779e-08
	p12 =  3.68912e-05
	p13 = -6.16472e-11
	p14 =  5.68417e-05
	p15 =  3.84316e-13
	p16 =  2.91480e-04
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["specCutsData_tagLoose_2017"] = f1d_specCutsData_tagLoose_2017

def f1d_fullCutsData_anti_ht1500to2500_2017(x): # Updated
        p0  =  6.45626e-03
        p1  =  2.80226e-01
        p2  = -1.29192e-02
        p3  = -3.45202e+00
        p4  = -4.19533e-02
        p5  =  1.22605e+02
        p6  =  2.64981e-02
        p7  = -9.50456e+02
        p8  = -4.24623e-01
        p9  = -1.13584e-05
        p10 =  1.84039e-03
        p11 = -1.76146e-07
        p12 = -7.73004e-04
        p13 = -4.05060e-10
        p14 = -3.46355e-03
        p15 =  3.64616e-12
        p16 =  3.33564e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht1500to2500_2017"] = f1d_fullCutsData_anti_ht1500to2500_2017

def f1d_fullCutsData_anti_ht1500to2500_MHT200_2017(x): # Updated
        p0  =  8.58845e-03
        p1  =  1.18745e-01
        p2  = -1.78938e-02
        p3  = -4.91210e+00
        p4  = -5.00389e-02
        p5  =  3.20831e+02
        p6  =  2.46823e-02
        p7  = -3.55904e+03
        p8  = -6.47354e-01
        p9  = -1.38412e-05
        p10 =  3.44953e-03
        p11 = -2.27263e-07
        p12 = -9.33092e-04
        p13 = -5.15370e-10
        p14 = -5.40352e-03
        p15 =  4.83533e-12
        p16 =  5.21369e-03
        return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht1500to2500_MHT200_2017"] = f1d_fullCutsData_anti_ht1500to2500_MHT200_2017

def f1d_fullCutsData_anti_ht2500to3500_2017(x): # Updated
	p0  =  5.99791e-03
	p1  =  1.04206e-01
	p2  = -5.05342e-03
	p3  =  2.22787e-02
	p4  = -4.53546e-03
	p5  =  1.81663e+02
	p6  = -3.14337e-03
	p7  = -1.65666e+03
	p8  = -1.38140e-01
	p9  = -4.42930e-06
	p10 =  4.90897e-04
	p11 = -1.55800e-07
	p12 =  3.68801e-04
	p13 = -3.85563e-10
	p14 = -1.25315e-03
	p15 =  3.46594e-12
	p16 =  1.77384e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht2500to3500_2017"] = f1d_fullCutsData_anti_ht2500to3500_2017

def f1d_fullCutsData_anti_ht2500to3500_MHT200_2017(x): #Updated
        p0  =  3.36430e-03
        p1  = -2.02887e-02
        p2  = -4.46035e+00
        p3  =  5.17717e+00
        p4  =  2.11099e+00
        p5  =  1.46114e+02
        p6  = -1.15051e+01
        p7  =  2.38537e+03
        p8  = -1.80601e+01
        p9  =  1.43876e-06
        p10 = -1.61633e+03
        p11 = -6.64370e-08
        p12 = -4.99745e+01
        p13 = -9.78844e-11
        p14 = -8.26229e+01
        p15 =  8.82241e-13
        p16 = -9.05594e+00
        return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht2500to3500_MHT200_2017"] = f1d_fullCutsData_anti_ht2500to3500_MHT200_2017

def f1d_fullCutsMC_anti_ht3500toInf_2017(x): # Updated
	p0  =  7.40966e-03
	p1  = -5.81151e-01
	p2  = -1.07064e+01
	p3  =  1.90118e+01
	p4  = -4.35822e+00
	p5  =  4.55739e+02
	p6  = -3.21793e+00
	p7  = -6.12125e+03
	p8  = -2.09829e+00
	p9  =  1.51350e-05
	p10 = -2.90056e+01
	p11 = -2.26034e-07
	p12 =  3.61442e+01
	p13 = -7.59454e-10
	p14 = -2.53267e+01
	p15 =  5.56001e-12
	p16 =  8.19191e+00
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsMC_anti_ht3500toInf_2017"] = f1d_fullCutsMC_anti_ht3500toInf_2017

def f1d_fullCutsMC_anti_ht3500toInf_MHT200_2017(x): # Updated
        p0  =  2.08498e-04
        p1  =  1.77018e-01
        p2  =  6.12872e-03
        p3  =  1.04797e+01
        p4  =  2.98498e-03
        p5  =  5.23421e+01
        p6  = -1.58656e-02
        p7  = -1.69889e+03
        p8  =  1.89331e-02
        p9  =  2.27290e-06
        p10 =  5.11175e-05
        p11 =  1.07378e-08
        p12 =  1.38289e-04
        p13 = -7.62656e-12
        p14 =  1.94136e-04
        p15 = -4.23220e-13
        p16 =  5.10733e-04
        return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsMC_anti_ht3500toInf_MHT200_2017"] = f1d_fullCutsMC_anti_ht3500toInf_MHT200_2017

####################
####################
####################
# MASS REGION CUTS #

# MassCutsV09
#S1_Node1 = 40.0
#S1_Node2 = 17.6
#SMAX_Node1 = 170.7
#SMAX_Node2 = 113.7
#SN_Nodes = [51.9, 65.1, 78.3, 91.5, 104.7, 117.9, 131.1, 144.3, 157.5]

# Alex Special MassCutsV10
S1_Node1 = 65.1
S1_Node2 = 36.06
SMAX_Node1 = 197.1  
SMAX_Node2 = 133.1
SN_Nodes = [78.3, 91.5, 104.7, 117.9, 131.1, 144.3, 157.5, 170.7, 183.9]

#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
function_2d = {}
integral_U = {}
integral_S = {}

keys = function_1d.keys()
for iK in range (0, len(keys)):
	def f2d(x,y):
		return function_1d[keys[iK]](x) * function_1d[keys[iK]](y)
	function_2d[ keys[iK] ] = f2d

# lineInfo object explainer:
# lineInfo[0] is x0
# lineInfo[1] is y0
# lineInfo[2] is m
def yValue(x, lineInfo):
	if (len(lineInfo) != 3):
		return "lineInfo object does not have three params as required"
	else:
		return lineInfo[2] * (x - lineInfo[0]) + lineInfo[1]

gradientUpperSignalLine = (SMAX_Node1 - S1_Node1) / (SMAX_Node2 - S1_Node2)
gradientLowerSignalLine = 1 / gradientUpperSignalLine
upperBand_x1 = S1_Node2 - 0.5 * (S1_Node1 - S1_Node2)
upperBand_y1 = S1_Node1 + 0.5 * (S1_Node1 - S1_Node2)
upperBand_x2 = SMAX_Node2 - 0.5 * (SMAX_Node1 - SMAX_Node2)
upperBand_y2 = SMAX_Node1 + 0.5 * (SMAX_Node1 - SMAX_Node2)
gradientUpperBand = (upperBand_y2 - upperBand_y1) / (upperBand_x2 - upperBand_x1)

lineInfo_lowerSignal = [S1_Node1, S1_Node2, gradientLowerSignalLine]
lineInfo_upperSignal = [S1_Node2, S1_Node1, gradientUpperSignalLine]
lineInfo_upperBand = [upperBand_x1, upperBand_y1, gradientUpperBand]

# each element of 'three_x_points_vec' corresponds to the first three x values (increasing from zero) on an edge of a mass region (sidebands and signal region combined)
three_x_points_vec = []

three_x_points_vec.append([-99999, S1_Node2, S1_Node1]) # using the dummy value -99999 as the first edge doesn't have a first x value

for SN_Node in SN_Nodes:
	x2 = SN_Node
	x1 = yValue(SN_Node, lineInfo_lowerSignal)
	x0 = x1 - 0.5 * (x2 - x1)
	three_x_points_vec.append([x0, x1, x2])

three_x_points_vec.append([upperBand_x2, SMAX_Node2, SMAX_Node1])
# print three_x_points_vec

for i in range(0, len(three_x_points_vec)-1):

	iMR = i + 1 # index to label the Mass Regions

	lineInfo_negLow = [three_x_points_vec[i][2], three_x_points_vec[i][1], -1]
	lineInfo_negHigh = [three_x_points_vec[i+1][2], three_x_points_vec[i+1][1], -1]
	# print lineInfo_negLow

	# define the integration bounds for x in the three U integral segments
	bounds_x_u1 = [three_x_points_vec[i][0], three_x_points_vec[i+1][0]]
	bounds_x_u2 = [three_x_points_vec[i+1][0], three_x_points_vec[i][1]]
	bounds_x_u3 = [three_x_points_vec[i][1], three_x_points_vec[i+1][1]]

	# define the integration bounds for y in the three U integral segments
	def bounds_y_u1(x):
		return [yValue(x,lineInfo_negLow), yValue(x,lineInfo_upperBand)]
	def bounds_y_u2(x):
		return [yValue(x,lineInfo_negLow), yValue(x,lineInfo_negHigh)]
	def bounds_y_u3(x):
		return [yValue(x,lineInfo_upperSignal), yValue(x,lineInfo_negHigh)]

	# define the integration bounds for x in the three S integral segments
	bounds_x_s1 = [three_x_points_vec[i][1], three_x_points_vec[i+1][1]]
	bounds_x_s2 = [three_x_points_vec[i+1][1], three_x_points_vec[i][2]]
	bounds_x_s3 = [three_x_points_vec[i][2], three_x_points_vec[i+1][2]]

	# define the integration bounds for y in the three S integral segments
	def bounds_y_s1(x):
		return [yValue(x,lineInfo_negLow), yValue(x,lineInfo_upperSignal)]
	def bounds_y_s2(x):
		return [yValue(x,lineInfo_negLow), yValue(x,lineInfo_negHigh)]
	def bounds_y_s3(x):
		return [yValue(x,lineInfo_lowerSignal), yValue(x,lineInfo_negHigh)]

	for iK in range (0, len(keys)):
		integral_U[ keys[iK] ] = 0

	if (i==0):
		gradient_Spec = (yValue(three_x_points_vec[1][0],lineInfo_negHigh) - S1_Node1) / (three_x_points_vec[1][0] - S1_Node2)
		lineInfo_Spec = [S1_Node2, S1_Node1, gradient_Spec]
		def bounds_y_uSpec(x):
			return [yValue(x,lineInfo_Spec), yValue(x,lineInfo_negHigh)]
		for iK in range (0, len(keys)):
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_uSpec, bounds_x_u2])[0]
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u3, bounds_x_u3])[0]

	else:
		for iK in range (0, len(keys)):
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u1, bounds_x_u1])[0]
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u2, bounds_x_u2])[0]
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u3, bounds_x_u3])[0]

	for iK in range (0, len(keys)):
		integral_S[ keys[iK] ] = integrate.nquad(function_2d[keys[iK]], [bounds_y_s1, bounds_x_s1])[0]
		integral_S[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_s2, bounds_x_s2])[0]
		integral_S[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_s3, bounds_x_s3])[0]

	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	##### #### ### ## # CALCULATIONS AND OUTPUT # ## ### #### #####
	# Do calculations with the total counts in each 2d mass segment


	#*,. #*,. #*,. #*,.
	#*,. 2016 #*,. #*,.
	ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2016 = integral_S["fullCutsData_anti_ht1500to2500_2016"] / (2 * integral_U["fullCutsData_anti_ht1500to2500_2016"])
	ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2016 = integral_S["fullCutsData_anti_ht2500to3500_2016"] / (2 * integral_U["fullCutsData_anti_ht2500to3500_2016"])
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2016_MC = integral_S["fullCutsMC_anti_ht3500toInf_2016"] / (2 * integral_U["fullCutsMC_anti_ht3500toInf_2016"])
        ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_MHT200_2016 = integral_S["fullCutsData_anti_ht1500to2500_MHT200_2016"] / (2 * integral_U["fullCutsData_anti_ht1500to2500_MHT200_2016"])
        ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_MHT200_2016 = integral_S["fullCutsData_anti_ht2500to3500_MHT200_2016"] / (2 * integral_U["fullCutsData_anti_ht2500to3500_MHT200_2016"])
        ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_MHT200_2016_MC = integral_S["fullCutsMC_anti_ht3500toInf_MHT200_2016"] / (2 * integral_U["fullCutsMC_anti_ht3500toInf_MHT200_2016"])	
	#print "2016 ht1500to2500 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2016)
	#print "2016 ht2500to3500 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2016)
	#print "2016 MC ht3500toInf F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2016_MC)
        #print "2016 ht1500to2500 mht200 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_MHT200_2016)
        #print "2016 ht2500to3500 mht200 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_MHT200_2016)
        #print "2016 MC ht3500toInf mht200 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_MHT200_2016_MC)

	correctionFactor_specCutsDataLoose_2016 = (integral_S["specCutsData_tagLoose_2016"] / integral_S["specCutsData_anti_2016"]) * (integral_U["specCutsData_anti_2016"] / integral_U["specCutsData_tagLoose_2016"])
	#print "2016 correction factor C_" + str(iMR) + " = " + str(correctionFactor_specCutsDataLoose_2016)
	

	#*,. #*,. #*,. #*,.
	#*,. 2017 #*,. #*,.
	ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2017 = integral_S["fullCutsData_anti_ht1500to2500_2017"] / (2 * integral_U["fullCutsData_anti_ht1500to2500_2017"])
	ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2017 = integral_S["fullCutsData_anti_ht2500to3500_2017"] / (2 * integral_U["fullCutsData_anti_ht2500to3500_2017"])
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2017_MC = integral_S["fullCutsMC_anti_ht3500toInf_2017"] / (2 * integral_U["fullCutsMC_anti_ht3500toInf_2017"])
        ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_MHT200_2017 = integral_S["fullCutsData_anti_ht1500to2500_MHT200_2017"] / (2 * integral_U["fullCutsData_anti_ht1500to2500_MHT200_2017"])
        ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_MHT200_2017 = integral_S["fullCutsData_anti_ht2500to3500_MHT200_2017"] / (2 * integral_U["fullCutsData_anti_ht2500to3500_MHT200_2017"])
        ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_MHT200_2017_MC = integral_S["fullCutsMC_anti_ht3500toInf_MHT200_2017"] / (2 * integral_U["fullCutsMC_anti_ht3500toInf_MHT200_2017"])
	#print "2017 ht1500to2500 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2017
	#print "2017 ht2500to3500 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2017)
	#print "2017 MC ht3500toInf F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2017_MC)
        #print "2017 ht1500to2500 mht200 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_MHT200_2017)
        #print "2017 ht2500to3500 mht200 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_MHT200_2017)
        print "2017 MC ht3500toInf mht200 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_MHT200_2017_MC)

	correctionFactor_specCutsDataLoose_2017 = (integral_S["specCutsData_tagLoose_2017"] / integral_S["specCutsData_anti_2017"]) * (integral_U["specCutsData_anti_2017"] / integral_U["specCutsData_tagLoose_2017"])
	#print "2017 correction factor C_" + str(iMR) + " = " + str(correctionFactor_specCutsDataLoose_2017)
	
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
