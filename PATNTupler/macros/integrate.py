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
# ...instead we mean that all the functions come from normalised histograms with the same binning (40 bins between 0 and 200 GeV)
# 'fullCutsData' means that the cuts were: preSel + 2*fatJet300 + 1*ak4pt300 + GIVEN_DBT_REGION + GIVEN_HT_BIN, preformed on Data
# 'fullCutsMC' means that the cuts were: preSel + 2*fatJet300 + 1*ak4pt300 + GIVEN_DBT_REGION + GIVEN_HT_BIN, preformed on MC
# 'specCutsData' means that the cuts were: preSel + 2*fatJet300 + ht1500to2500 + GIVEN_DBT_REGION + fatJetB_DBT < 0.3, preformed on Data
# fits valid between 15 and 200 GeV, softDropMassPUPPI is the mass used

#*,. #*,. #*,. #*,.
#*,. 2016 #*,. #*,.
def f1d_specCutsData_anti_2016(x):
	p0 = -3.63654e-03
	p1 =  3.11147e+00
	p2 = -1.04436e+02
	p3 = -4.45789e+01
	p4 = -1.22714e+02
	p5 =  2.07740e+02
	p6 = -3.38277e+00
	p7 =  1.12255e+04
	p8 = -2.29916e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["specCutsData_anti_2016"] = f1d_specCutsData_anti_2016

def f1d_specCutsData_tagLoose_2016(x):
	p0 = -3.40747e-03
	p1 =  2.05074e+00
	p2 = -6.51921e+01
	p3 = -6.77139e-01
	p4 =  3.94987e+00
	p5 =  6.39078e+02
	p6 = -7.21613e+00
	p7 =  1.15320e+04
	p8 = -6.03416e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["specCutsData_tagLoose_2016"] = f1d_specCutsData_tagLoose_2016

def f1d_fullCutsData_anti_ht1500to2500_2016(x):
	p0 = -1.88044e-02
	p1 =  6.14545e+00
	p2 = -5.97872e+01
	p3 = -1.62308e+02
	p4 = -3.59127e+01
	p5 =  9.89394e+02
	p6 = -1.44970e+01
	p7 =  5.04702e+04
	p8 = -2.50092e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsData_anti_ht1500to2500_2016"] = f1d_fullCutsData_anti_ht1500to2500_2016

def f1d_fullCutsData_anti_ht2500to3500_2016(x):
	p0 = -1.39902e-02
	p1 =  5.47407e+00
	p2 = -8.36004e+01
	p3 = -1.24633e+02
	p4 = -4.35109e+01
	p5 =  1.15529e+02
	p6 = -1.01950e+01
	p7 =  4.47839e+04
	p8 = -1.28801e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsData_anti_ht2500to3500_2016"] = f1d_fullCutsData_anti_ht2500to3500_2016

def f1d_fullCutsMC_anti_ht3500toInf_2016(x):
	p0 = -1.15041e-02
	p1 =  5.53766e+00
	p2 = -1.53310e+02
	p3 = -6.72066e+01
	p4 = -2.94055e+01
	p5 =  3.58214e+03
	p6 = -1.79872e+01
	p7 = -3.06273e+01
	p8 = -1.67188e+02
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsMC_anti_ht3500toInf_2016"] = f1d_fullCutsMC_anti_ht3500toInf_2016

#*,. #*,. #*,. #*,.
#*,. 2017 #*,. #*,.
def f1d_specCutsData_anti_2017(x):
	p0 =  1.76857e-04
	p1 =  1.53081e+00
	p2 = -2.18087e+01
	p3 = -2.41448e+01
	p4 = -1.42758e+01
	p5 =  1.63712e+02
	p6 = -6.06677e+00
	p7 =  9.16816e+03
	p8 = -9.24887e+00
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["specCutsData_anti_2017"] = f1d_specCutsData_anti_2017

def f1d_specCutsData_tagLoose_2017(x):
	p0 = -3.72663e-03
	p1 =  1.58074e+00
	p2 = -4.86866e+01
	p3 =  1.65299e+00
	p4 =  1.96014e+00
	p5 =  1.98330e+02
	p6 = -7.13663e+00
	p7 =  1.19689e+04
	p8 = -9.67216e+00
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["specCutsData_tagLoose_2017"] = f1d_specCutsData_tagLoose_2017

def f1d_fullCutsData_anti_ht1500to2500_2017(x):
	p0 = -2.08381e-02
	p1 =  6.78657e+00
	p2 = -6.22994e+01
	p3 = -1.87939e+02
	p4 = -3.78471e+01
	p5 =  1.21443e+03
	p6 = -1.56722e+01
	p7 =  5.84547e+04
	p8 = -2.84820e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsData_anti_ht1500to2500_2017"] = f1d_fullCutsData_anti_ht1500to2500_2017

def f1d_fullCutsData_anti_ht2500to3500_2017(x):
	p0 = -7.29825e-03
	p1 =  2.87928e+00
	p2 = -4.15626e+01
	p3 = -4.77635e+01
	p4 = -2.35633e+01
	p5 =  1.19455e+02
	p6 = -7.26044e+00
	p7 =  2.11689e+04
	p8 = -9.87851e+00
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsData_anti_ht2500to3500_2017"] = f1d_fullCutsData_anti_ht2500to3500_2017

def f1d_fullCutsData_anti_ht3500toInf_2017(x):
	p0 = -1.85987e-03
	p1 =  9.51859e-01
	p2 = -2.02771e-02
	p3 =  6.20688e+00
	p4 = -1.13021e-02
	p5 = -8.42290e+01
	p6 =  3.71441e-02
	p7 =  3.37361e+02
	p8 = -7.01489e-01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsData_anti_ht3500toInf_2017"] = f1d_fullCutsData_anti_ht3500toInf_2017

def f1d_fullCutsMC_anti_ht3500toInf_2017(x):
	p0 =  2.78357e-04
	p1 =  6.79774e-01
	p2 = -4.83833e-01
	p3 =  4.96317e+00
	p4 =  1.58596e+00
	p5 =  1.20290e+02
	p6 =  2.43597e-01
	p7 = -3.61653e+02
	p8 =  4.81016e+00
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsMC_anti_ht3500toInf_2017"] = f1d_fullCutsMC_anti_ht3500toInf_2017

####################
####################
####################
# MASS REGION CUTS #

# MassCutsV08
S1_Node1 = 40.0;
S1_Node2 = 17.6;
SMAX_Node1 = 172.0;
SMAX_Node2 = 114.7;
SN_Nodes = [53.2, 66.4, 79.6, 92.8, 106.0, 119.2, 132.4, 145.6, 158.8]

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
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2016MC = integral_S["fullCutsMC_anti_ht3500toInf_2016"] / (2 * integral_U["fullCutsMC_anti_ht3500toInf_2016"])	
	# print "2016 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2016)
	# print "2016 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2016)
	# print "2016 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2016MC)

	correctionFactor_specCutsDataLoose_2016 = (integral_S["specCutsData_tagLoose_2016"] / integral_S["specCutsData_anti_2016"]) * (integral_U["specCutsData_anti_2016"] / integral_U["specCutsData_tagLoose_2016"])
	# print "2016 correction factor C_" + str(iMR) + " = " + str(correctionFactor_specCutsDataLoose_2016)
	

	#*,. #*,. #*,. #*,.
	#*,. 2017 #*,. #*,.
	ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2017 = integral_S["fullCutsData_anti_ht1500to2500_2017"] / (2 * integral_U["fullCutsData_anti_ht1500to2500_2017"])
	ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2017 = integral_S["fullCutsData_anti_ht2500to3500_2017"] / (2 * integral_U["fullCutsData_anti_ht2500to3500_2017"])
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2017 = integral_S["fullCutsData_anti_ht3500toInf_2017"] / (2 * integral_U["fullCutsData_anti_ht3500toInf_2017"])	
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2017MC = integral_S["fullCutsMC_anti_ht3500toInf_2017"] / (2 * integral_U["fullCutsMC_anti_ht3500toInf_2017"])	
	# print "2017 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500_2017)
	# print "2017 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500_2017)
	# print "2017 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2017)
	# print "2017 F_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf_2017MC)

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