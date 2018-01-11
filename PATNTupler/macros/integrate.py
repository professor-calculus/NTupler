import math
from scipy import integrate

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/integrate.py

# notes on script:
# - it is very much geometry specific and not completely general for any MassRegion object!
# - assumes indendence between the 2 fatJet mass distributions (good for QCD)
# - a lot of objects are hard coded in, might want to address that in the future
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################

# FUNCTIONS #
# all these functions are 'normalised', by which we do not mean that the area under the function is unity...
# ...instead we mean that all the functions come from normalised histograms with the same binning (40 bins between 0 and 200 GeV)
# 'fullCutsData' means that the cuts were: preSel + 2*fatJet300 + 1*ak4pt300 + GIVEN_DBT_REGION + GIVEN_HT_BIN, preformed on Data
# 'fullCutsMC' means that the cuts were: preSel + 2*fatJet300 + 1*ak4pt300 + GIVEN_DBT_REGION + GIVEN_HT_BIN, preformed on MC
# fits valid between 13 and 200 GeV

def f_fullCutsData_anti_ht1500to2500_1d(x):
	p0 = -4.22179e-02
	p1 =  1.92541e+01
	p2 = -1.62503e+02
	p3 = -7.18550e+02
	p4 = -8.52301e+01
	p5 =  4.72774e+03
	p6 = -2.87152e+01
	p7 =  1.69088e+05
	p8 = -6.33536e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCutsData_anti_ht2500to3500_1d(x):
	p0 = -1.74534e-02
	p1 =  7.86667e+00
	p2 = -1.27512e+02
	p3 = -2.00748e+02
	p4 = -6.79748e+01
	p5 =  3.96031e+02
	p6 = -1.96542e+01
	p7 =  5.44686e+04
	p8 = -1.60781e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCutsMC_anti_ht1500to2500_1d(x):
	p0 = -4.94653e-02
	p1 =  2.49991e+01
	p2 = -1.96038e+02
	p3 = -1.02717e+03
	p4 = -1.01098e+02
	p5 =  7.15767e+03
	p6 = -3.51401e+01
	p7 =  2.49535e+05
	p8 = -1.00538e+02
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCutsMC_anti_ht2500to3500_1d(x):
	p0 = -6.47232e-03
	p1 =  2.97326e+00
	p2 = -4.89351e+01
	p3 = -5.79091e+01
	p4 = -2.67790e+01
	p5 =  3.45249e+02
	p6 = -8.80802e+00
	p7 =  2.13680e+04
	p8 = -1.26207e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCutsMC_anti_ht3500toInf_1d(x):
	p0 =  1.22198e-03
	p1 =  5.97083e-01
	p2 =  4.98529e+00
	p3 =  7.27657e+00
	p4 =  1.05514e+00
	p5 = -3.77587e+01
	p6 =  2.78296e+00
	p7 = -2.48275e+02
	p8 = -2.33471e-01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCutsMC_tag_ht1500to2500_1d(x):
	p0 = -3.98710e-02
	p1 =  1.83449e+01
	p2 = -1.64520e+02
	p3 = -6.67720e+02
	p4 = -8.75637e+01
	p5 =  4.28107e+03
	p6 = -2.71280e+01
	p7 =  1.87972e+05
	p8 = -7.22679e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))	

def f_fullCutsMC_tag_ht2500to3500_1d(x):
	p0 = -7.91664e-03
	p1 =  3.88477e+00
	p2 = -1.11492e+02
	p3 = -4.29402e+01
	p4 = -1.33364e+02
	p5 =  6.47810e+02
	p6 = -9.76258e+00
	p7 =  3.16980e+04
	p8 = -2.55109e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))


####################
####################
####################
# MASS REGION CUTS #

# MassCutsV05
S1_Node1 = 33.0
S1_Node2 = 17.6
SMAX_Node1 = 168.5
SMAX_Node2 = 115.04
SN_Nodes = [42.5, 52.9, 64.2, 76.4, 89.5, 103.5, 118.4, 134.2, 150.9];

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

def f_fullCutsData_anti_ht1500to2500_2d(x,y):
	return f_fullCutsData_anti_ht1500to2500_1d(x) * f_fullCutsData_anti_ht1500to2500_1d(y)

def f_fullCutsData_anti_ht2500to3500_2d(x,y):
	return f_fullCutsData_anti_ht2500to3500_1d(x) * f_fullCutsData_anti_ht2500to3500_1d(y)

def f_fullCutsMC_anti_ht1500to2500_2d(x,y):
	return f_fullCutsMC_anti_ht1500to2500_1d(x) * f_fullCutsMC_anti_ht1500to2500_1d(y)

def f_fullCutsMC_anti_ht2500to3500_2d(x,y):
	return f_fullCutsMC_anti_ht2500to3500_1d(x) * f_fullCutsMC_anti_ht2500to3500_1d(y)

def f_fullCutsMC_anti_ht3500toInf_2d(x,y):
	return f_fullCutsMC_anti_ht3500toInf_1d(x) * f_fullCutsMC_anti_ht3500toInf_1d(y)

def f_fullCutsMC_tag_ht1500to2500_2d(x,y):
	return f_fullCutsMC_tag_ht1500to2500_1d(x) * f_fullCutsMC_tag_ht1500to2500_1d(y)

def f_fullCutsMC_tag_ht2500to3500_2d(x,y):
	return f_fullCutsMC_tag_ht2500to3500_1d(x) * f_fullCutsMC_tag_ht2500to3500_1d(y)


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

	integral_U_fullCutsData_anti_ht1500to2500 = 0
	integral_U_fullCutsData_anti_ht2500to3500 = 0
	integral_U_fullCutsMC_anti_ht1500to2500 = 0
	integral_U_fullCutsMC_anti_ht2500to3500 = 0
	integral_U_fullCutsMC_anti_ht3500toInf = 0
	integral_U_fullCutsMC_tag_ht1500to2500 = 0
	integral_U_fullCutsMC_tag_ht2500to3500 = 0

	if (i==0):

		gradient_Spec = (yValue(three_x_points_vec[1][0],lineInfo_negHigh) - S1_Node1) / (three_x_points_vec[1][0] - S1_Node2)
		lineInfo_Spec = [S1_Node2, S1_Node1, gradient_Spec]
		def bounds_y_uSpec(x):
			return [yValue(x,lineInfo_Spec), yValue(x,lineInfo_negHigh)]

		integral_U_fullCutsData_anti_ht1500to2500 += integrate.nquad(f_fullCutsData_anti_ht1500to2500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsData_anti_ht1500to2500 += integrate.nquad(f_fullCutsData_anti_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsData_anti_ht2500to3500 += integrate.nquad(f_fullCutsData_anti_ht2500to3500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsData_anti_ht2500to3500 += integrate.nquad(f_fullCutsData_anti_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_anti_ht1500to2500 += integrate.nquad(f_fullCutsMC_anti_ht1500to2500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsMC_anti_ht1500to2500 += integrate.nquad(f_fullCutsMC_anti_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_anti_ht2500to3500 += integrate.nquad(f_fullCutsMC_anti_ht2500to3500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsMC_anti_ht2500to3500 += integrate.nquad(f_fullCutsMC_anti_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_anti_ht3500toInf += integrate.nquad(f_fullCutsMC_anti_ht3500toInf_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsMC_anti_ht3500toInf += integrate.nquad(f_fullCutsMC_anti_ht3500toInf_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_tag_ht1500to2500 += integrate.nquad(f_fullCutsMC_tag_ht1500to2500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsMC_tag_ht1500to2500 += integrate.nquad(f_fullCutsMC_tag_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_tag_ht2500to3500 += integrate.nquad(f_fullCutsMC_tag_ht2500to3500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsMC_tag_ht2500to3500 += integrate.nquad(f_fullCutsMC_tag_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]		

	else:
		integral_U_fullCutsData_anti_ht1500to2500 += integrate.nquad(f_fullCutsData_anti_ht1500to2500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsData_anti_ht1500to2500 += integrate.nquad(f_fullCutsData_anti_ht1500to2500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsData_anti_ht1500to2500 += integrate.nquad(f_fullCutsData_anti_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsData_anti_ht2500to3500 += integrate.nquad(f_fullCutsData_anti_ht2500to3500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsData_anti_ht2500to3500 += integrate.nquad(f_fullCutsData_anti_ht2500to3500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsData_anti_ht2500to3500 += integrate.nquad(f_fullCutsData_anti_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_anti_ht1500to2500 += integrate.nquad(f_fullCutsMC_anti_ht1500to2500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsMC_anti_ht1500to2500 += integrate.nquad(f_fullCutsMC_anti_ht1500to2500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsMC_anti_ht1500to2500 += integrate.nquad(f_fullCutsMC_anti_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_anti_ht2500to3500 += integrate.nquad(f_fullCutsMC_anti_ht2500to3500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsMC_anti_ht2500to3500 += integrate.nquad(f_fullCutsMC_anti_ht2500to3500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsMC_anti_ht2500to3500 += integrate.nquad(f_fullCutsMC_anti_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_anti_ht3500toInf += integrate.nquad(f_fullCutsMC_anti_ht3500toInf_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsMC_anti_ht3500toInf += integrate.nquad(f_fullCutsMC_anti_ht3500toInf_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsMC_anti_ht3500toInf += integrate.nquad(f_fullCutsMC_anti_ht3500toInf_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_tag_ht1500to2500 += integrate.nquad(f_fullCutsMC_tag_ht1500to2500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsMC_tag_ht1500to2500 += integrate.nquad(f_fullCutsMC_tag_ht1500to2500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsMC_tag_ht1500to2500 += integrate.nquad(f_fullCutsMC_tag_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsMC_tag_ht2500to3500 += integrate.nquad(f_fullCutsMC_tag_ht2500to3500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsMC_tag_ht2500to3500 += integrate.nquad(f_fullCutsMC_tag_ht2500to3500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsMC_tag_ht2500to3500 += integrate.nquad(f_fullCutsMC_tag_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]


	integral_S_fullCutsData_anti_ht1500to2500 = integrate.nquad(f_fullCutsData_anti_ht1500to2500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsData_anti_ht1500to2500 += integrate.nquad(f_fullCutsData_anti_ht1500to2500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsData_anti_ht1500to2500 += integrate.nquad(f_fullCutsData_anti_ht1500to2500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCutsData_anti_ht2500to3500 = integrate.nquad(f_fullCutsData_anti_ht2500to3500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsData_anti_ht2500to3500 += integrate.nquad(f_fullCutsData_anti_ht2500to3500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsData_anti_ht2500to3500 += integrate.nquad(f_fullCutsData_anti_ht2500to3500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCutsMC_anti_ht1500to2500 = integrate.nquad(f_fullCutsMC_anti_ht1500to2500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsMC_anti_ht1500to2500 += integrate.nquad(f_fullCutsMC_anti_ht1500to2500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsMC_anti_ht1500to2500 += integrate.nquad(f_fullCutsMC_anti_ht1500to2500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCutsMC_anti_ht2500to3500 = integrate.nquad(f_fullCutsMC_anti_ht2500to3500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsMC_anti_ht2500to3500 += integrate.nquad(f_fullCutsMC_anti_ht2500to3500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsMC_anti_ht2500to3500 += integrate.nquad(f_fullCutsMC_anti_ht2500to3500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCutsMC_anti_ht3500toInf = integrate.nquad(f_fullCutsMC_anti_ht3500toInf_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsMC_anti_ht3500toInf += integrate.nquad(f_fullCutsMC_anti_ht3500toInf_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsMC_anti_ht3500toInf += integrate.nquad(f_fullCutsMC_anti_ht3500toInf_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCutsMC_tag_ht1500to2500 = integrate.nquad(f_fullCutsMC_tag_ht1500to2500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsMC_tag_ht1500to2500 += integrate.nquad(f_fullCutsMC_tag_ht1500to2500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsMC_tag_ht1500to2500 += integrate.nquad(f_fullCutsMC_tag_ht1500to2500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCutsMC_tag_ht2500to3500 = integrate.nquad(f_fullCutsMC_tag_ht2500to3500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsMC_tag_ht2500to3500 += integrate.nquad(f_fullCutsMC_tag_ht2500to3500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsMC_tag_ht2500to3500 += integrate.nquad(f_fullCutsMC_tag_ht2500to3500_2d, [bounds_y_s3, bounds_x_s3])[0]

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

	ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500 = integral_S_fullCutsData_anti_ht1500to2500 / (2 * integral_U_fullCutsData_anti_ht1500to2500)
	ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500 = integral_S_fullCutsData_anti_ht2500to3500 / (2 * integral_U_fullCutsData_anti_ht2500to3500)
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf = integral_S_fullCutsMC_anti_ht3500toInf / (2 * integral_U_fullCutsMC_anti_ht3500toInf)	
	# print "A_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500)
	# print "A_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500)
	print "A_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf)

	correctionFactor_ht1500to2500 = (integral_S_fullCutsMC_tag_ht1500to2500 / integral_S_fullCutsMC_anti_ht1500to2500) * (integral_U_fullCutsMC_anti_ht1500to2500 / integral_U_fullCutsMC_tag_ht1500to2500)
	correctionFactor_ht2500to3500 = (integral_S_fullCutsMC_tag_ht2500to3500 / integral_S_fullCutsMC_anti_ht2500to3500) * (integral_U_fullCutsMC_anti_ht2500to3500 / integral_U_fullCutsMC_tag_ht2500to3500)
	# print "correction factor tag C_" + str(iMR) + " = " + str(correctionFactor_ht1500to2500)
	# print "correction factor tag C_" + str(iMR) + " = " + str(correctionFactor_ht2500to3500)
	# print "correction factor tag C_" + str(iMR) + " = " + str(correctionFactor_ht1500to2500) + "    " + str(correctionFactor_ht2500to3500)
	
	# print integral_S_fullCutsMC_tag_ht1500to2500
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################