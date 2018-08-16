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
	p0 =   3.05084e-03
	p1 =   2.83047e-01
	p2 =  -3.75332e-03
	p3 =   1.24422e-01
	p4 =  -5.21997e-03
	p5 =  -1.47489e+01
	p6 =  -3.47828e-03
	p7 =   6.22878e+02
	p8 =  -1.13843e-02
	p9 =   5.72791e-07
	p10 = -2.93801e-04
	p11 = -4.07941e-08
	p12 =  5.84453e-05
	p13 = -1.26076e-10
	p14 =  4.65995e-05
	p15 =  7.00493e-13
	p16 =  3.34440e-04
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["specCutsData_anti_2016"] = f1d_specCutsData_anti_2016

def f1d_specCutsData_tagLoose_2016(x):
	p0 =   1.37948e-03
	p1 =   2.67903e-01
	p2 =  -9.14359e-04
	p3 =   3.13999e+00
	p4 =  -5.56902e-03
	p5 =   4.37331e+00
	p6 =  -4.86290e-03
	p7 =   3.47221e+02
	p8 =  -1.16573e-02
	p9 =  -2.25149e-06
	p10 = -1.76975e-05
	p11 = -2.79263e-08
	p12 =  4.18700e-05
	p13 = -6.40111e-11
	p14 =  5.81997e-05
	p15 =  5.07473e-13
	p16 =  3.13322e-04
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["specCutsData_tagLoose_2016"] = f1d_specCutsData_tagLoose_2016

def f1d_fullCutsData_anti_ht1500to2500_2016(x):
	p0 =   5.95371e-03
	p1 =   2.96106e-01
	p2 =  -1.26807e-02
	p3 =  -3.27525e+00
	p4 =  -3.75803e-02
	p5 =   1.16137e+02
	p6 =   1.82780e-02
	p7 =  -8.23064e+02
	p8 =  -3.62037e-01
	p9 =  -1.11802e-05
	p10 =  1.70626e-03
	p11 = -1.64704e-07
	p12 = -4.88316e-04
	p13 = -3.69676e-10
	p14 = -2.89138e-03
	p15 =  3.44088e-12
	p16 =  3.26336e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht1500to2500_2016"] = f1d_fullCutsData_anti_ht1500to2500_2016

def f1d_fullCutsData_anti_ht2500to3500_2016(x):
	p0 =   5.50665e-03
	p1 =   1.31010e-01
	p2 =  -6.22536e-03
	p3 =  -5.90121e-01
	p4 =   1.30157e-03
	p5 =   2.07592e+02
	p6 =  -6.06060e-03
	p7 =  -1.84130e+03
	p8 =  -1.13305e-01
	p9 =  -6.22750e-06
	p10 =  3.93287e-04
	p11 = -1.33907e-07
	p12 =  8.44666e-05
	p13 = -2.78631e-10
	p14 = -9.71060e-04
	p15 =  2.77817e-12
	p16 =  1.12059e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht2500to3500_2016"] = f1d_fullCutsData_anti_ht2500to3500_2016

def f1d_fullCutsData_anti_ht3500toInf_2016(x):
	p0 =   8.57728e-03
	p1 =  -4.66612e-02
	p2 =   2.82101e-03
	p3 =   1.05038e+01
	p4 =   2.03271e-02
	p5 =   3.12137e+02
	p6 =  -2.08201e-02
	p7 =  -3.96651e+03
	p8 =   5.78929e-03
	p9 =  -2.52613e-08
	p10 =  3.65309e-04
	p11 = -2.10203e-07
	p12 =  1.21197e-03
	p13 = -6.44329e-10
	p14 = -1.84489e-03
	p15 =  4.17148e-12
	p16 =  1.27267e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht3500toInf_2016"] = f1d_fullCutsData_anti_ht3500toInf_2016

#*,. #*,. #*,. #*,.
#*,. 2017 #*,. #*,.
def f1d_specCutsData_anti_2017(x):
	p0 =   3.09206e-03
	p1 =   2.79233e-01
	p2 =  -3.74469e-03
	p3 =   1.10150e-01
	p4 =  -5.19909e-03
	p5 =  -1.64480e+01
	p6 =  -3.68775e-03
	p7 =   5.56535e+02
	p8 =  -1.23244e-02
	p9 =   1.06963e-06
	p10 = -3.05052e-04
	p11 = -3.78832e-08
	p12 =  6.19313e-05
	p13 = -1.20468e-10
	p14 =  7.52692e-05
	p15 =  6.25219e-13
	p16 =  3.60616e-04
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["specCutsData_anti_2017"] = f1d_specCutsData_anti_2017

def f1d_specCutsData_tagLoose_2017(x):
	p0 =   1.23768e-03
	p1 =   2.50044e-01
	p2 =  -5.75169e-04
	p3 =   3.34593e+00
	p4 =  -5.07547e-03
	p5 =   7.57553e+00
	p6 =  -5.02737e-03
	p7 =   2.57217e+02
	p8 =  -1.11587e-02
	p9 =  -1.84225e-06
	p10 = -1.32944e-05
	p11 = -2.40932e-08
	p12 =  3.93253e-05
	p13 = -6.11895e-11
	p14 =  6.20375e-05
	p15 =  3.63280e-13
	p16 =  2.90891e-04
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["specCutsData_tagLoose_2017"] = f1d_specCutsData_tagLoose_2017

def f1d_fullCutsData_anti_ht1500to2500_2017(x):
	p0 =   6.59827e-03
	p1 =   2.68404e-01
	p2 =  -1.27076e-02
	p3 =  -3.34350e+00
	p4 =  -4.16443e-02
	p5 =   1.26707e+02
	p6 =   2.69756e-02
	p7 =  -1.01037e+03
	p8 =  -4.28359e-01
	p9 =  -1.10694e-05
	p10 =  1.87319e-03
	p11 = -1.79389e-07
	p12 = -7.42166e-04
	p13 = -4.20152e-10
	p14 = -3.60868e-03
	p15 =  3.71283e-12
	p16 =  3.41055e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht1500to2500_2017"] = f1d_fullCutsData_anti_ht1500to2500_2017

def f1d_fullCutsData_anti_ht2500to3500_2017(x):
	p0 =   5.54634e-03
	p1 =   1.44415e-01
	p2 =  -6.39219e-03
	p3 =  -8.39276e-01
	p4 =  -5.52986e-03
	p5 =   1.72827e+02
	p6 =  -2.53631e-03
	p7 =  -1.40189e+03
	p8 =  -1.46047e-01
	p9 =  -5.51972e-06
	p10 =  3.37635e-04
	p11 = -1.35696e-07
	p12 =  1.32333e-04
	p13 = -3.02521e-10
	p14 = -1.07443e-03
	p15 =  2.84744e-12
	p16 =  1.32167e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht2500to3500_2017"] = f1d_fullCutsData_anti_ht2500to3500_2017

def f1d_fullCutsData_anti_ht3500toInf_2017(x):
	p0 =   8.20938e-03
	p1 =   1.10167e-01
	p2 =   2.23699e-03
	p3 =   8.57252e+00
	p4 =   1.73371e-02
	p5 =   2.52954e+02
	p6 =  -1.24079e-02
	p7 =  -3.54534e+03
	p8 =   3.85983e-02
	p9 =  -5.52451e-06
	p10 =  7.00606e-04
	p11 = -2.31573e-07
	p12 =  4.93582e-04
	p13 = -5.55736e-10
	p14 = -6.66774e-04
	p15 =  5.19093e-12
	p16 =  1.85734e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8)) + p9*(x-p10) + p11*(x-p12)*(x-p12) + p13*(x-p14)*(x-p14)*(x-p14) + p15*(x-p16)*(x-p16)*(x-p16)*(x-p16)
function_1d["fullCutsData_anti_ht3500toInf_2017"] = f1d_fullCutsData_anti_ht3500toInf_2017

####################
####################
####################
# MASS REGION CUTS #

# MassCutsV09
S1_Node1 = 40.0
S1_Node2 = 17.6
SMAX_Node1 = 170.7
SMAX_Node2 = 113.7
SN_Nodes = [51.9, 65.1, 78.3, 91.5, 104.7, 117.9, 131.1, 144.3, 157.5]

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
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2016 = integral_S["fullCutsData_anti_ht3500toInf_2016"] / (2 * integral_U["fullCutsData_anti_ht3500toInf_2016"])	
	# print "2016 ht1500to2500 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2016)
	# print "2016 ht2500to3500 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2016)
	# print "2016 ht3500toInf F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2016)

	correctionFactor_specCutsDataLoose_2016 = (integral_S["specCutsData_tagLoose_2016"] / integral_S["specCutsData_anti_2016"]) * (integral_U["specCutsData_anti_2016"] / integral_U["specCutsData_tagLoose_2016"])
	# print "2016 correction factor C_" + str(iMR) + " = " + str(correctionFactor_specCutsDataLoose_2016)
	

	#*,. #*,. #*,. #*,.
	#*,. 2017 #*,. #*,.
	ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2017 = integral_S["fullCutsData_anti_ht1500to2500_2017"] / (2 * integral_U["fullCutsData_anti_ht1500to2500_2017"])
	ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2017 = integral_S["fullCutsData_anti_ht2500to3500_2017"] / (2 * integral_U["fullCutsData_anti_ht2500to3500_2017"])
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2017 = integral_S["fullCutsData_anti_ht3500toInf_2017"] / (2 * integral_U["fullCutsData_anti_ht3500toInf_2017"])	
	# print "2017 ht1500to2500 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2017)
	# print "2017 ht2500to3500 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2017)
	# print "2017 ht3500toInf F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2017)

	correctionFactor_specCutsDataLoose_2017 = (integral_S["specCutsData_tagLoose_2017"] / integral_S["specCutsData_anti_2017"]) * (integral_U["specCutsData_anti_2017"] / integral_U["specCutsData_tagLoose_2017"])
	print "2017 correction factor C_" + str(iMR) + " = " + str(correctionFactor_specCutsDataLoose_2017)
	
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################