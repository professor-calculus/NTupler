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

# FUNCTIONS #
# all these functions are 'normalised', by which we do not mean that the area under the function is unity...
# ...instead we mean that all the functions come from normalised histograms with the same binning (40 bins between 0 and 200 GeV)

# 'specCuts' means that the cuts were: preSel + 2*fatJet300 + ht1500to2500 + other fatJet DBT < Loose WP (qcd dominates the data here). NOTE: The dbt info is only for one of the jets.
# 'fullCuts' means that the cuts were: preSel + 2*fatJet300 + 2*ak4pt250 + GIVEN_DBT_REGION + GIVEN_HT_BIN (combo of data and MC depending on stats)
# 'fullCutsNOAK4' means that the cuts were: preSel + 2*fatJet300 + GIVEN_DBT_REGION + GIVEN_HT_BIN (combo of data and MC depending on stats)

def f_specCuts_tag_1d(x):
	p0 = -1.89332e-03
	p1 =  1.54318e+00
	p2 = -2.24772e+01
	p3 = -1.36370e+00
	p4 =  5.45750e+00
	p5 =  3.43718e+02
	p6 = -3.94144e+00
	p7 =  4.06954e+03
	p8 = -2.21983e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_specCuts_anti_1d(x):
	p0 =  4.72117e-04
	p1 =  1.48693e+00
	p2 = -2.72819e+01
	p3 = -1.76823e+01
	p4 = -1.59642e+01
	p5 =  1.89666e+02
	p6 = -4.55585e+00
	p7 =  4.74347e+03
	p8 = -8.54973e+00
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_specCuts_control_1d_LooseToMed2(x):
	p0 =  1.00005e-03
	p1 =  7.76647e-01
	p2 = -3.75699e-02
	p3 =  1.88846e+00
	p4 =  3.57878e-01
	p5 = -2.31772e+01
	p6 =  2.23972e-02
	p7 =  8.24659e+02
	p8 = -5.59663e-02
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_specCuts_control_1d_OffToIDBTCv21(x):
	p0 =  3.91070e-03
	p1 =  1.73050e+00
	p2 = -3.54452e+01
	p3 = -2.66813e+01
	p4 = -1.71158e+01
	p5 =  1.89739e+02
	p6 = -6.18362e+00
	p7 =  5.11784e+03
	p8 = -1.19781e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCuts_anti_ht1500to2500_1d(x):
	p0 = -2.19691e-02
	p1 =  6.91121e+00
	p2 = -1.58076e+01
	p3 = -4.49258e+02
	p4 = -2.03684e+01
	p5 =  1.07287e+04
	p6 = -2.16686e+01
	p7 = -9.12852e+04
	p8 = -2.04944e+02
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCuts_anti_ht2500to3500_1d(x):
	p0 = -2.21198e-02
	p1 =  1.10770e+01
	p2 = -1.87420e+02
	p3 = -2.09798e+02
	p4 = -8.95149e+01
	p5 =  4.81998e+02
	p6 = -8.73972e+00
	p7 =  3.51400e+04
	p8 = -2.63162e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCuts_anti_ht3500toInf_1d(x):
	p0 =  1.17054e-03
	p1 =  6.71256e-01
	p2 =  1.44548e-02
	p3 =  5.79320e+00
	p4 = -1.59636e-02
	p5 = -1.28330e+00
	p6 = -2.84894e-02
	p7 = -1.96499e+02
	p8 =  2.63134e-03
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCutsNOAK4_anti_ht1500to2500_1d(x):
	p0 =  1.06028e-04
	p1 =  1.62841e+00
	p2 = -3.94453e+01
	p3 = -1.47137e+01
	p4 = -2.77212e+01
	p5 =  1.32674e+02
	p6 = -3.61154e+00
	p7 =  5.20373e+03
	p8 = -9.58425e+00
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

def f_fullCutsNOAK4_anti_ht2500to3500_1d(x):
	p0 = -1.58203e-02
	p1 =  1.19155e+01
	p2 = -3.78098e+02
	p3 = -1.17622e+02
	p4 = -3.94118e+02
	p5 = -2.91941e+02
	p6 = -4.50485e+01
	p7 =  4.81695e+04
	p8 = -1.22904e+01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))

# MASS REGION CUTS #

# MassCutsV04
S1_Node1 = 31.5;
S1_Node2 = 16.5;
SMAX_Node1 = 168.5;
SMAX_Node2 = 115.04;
SN_Nodes = [40.7, 50.9, 62.1, 74.3, 87.5, 101.7, 116.9, 133.1, 150.3];

# NEW ATTEMPTS
# S1_Node1 = 35.0;
# S1_Node2 = 20.0;
# SMAX_Node1 = 168.5;
# SMAX_Node2 = 115.04;
# SN_Nodes = [45.7, 50.9, 62.1, 74.3, 87.5, 101.7, 116.9, 133.1, 150.3];

#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
# lineInfo object explainer:
# lineInfo[0] is x0
# lineInfo[1] is y0
# lineInfo[2] is m
def yValue(x, lineInfo):
	if (len(lineInfo) != 3):
		return "lineInfo object does not have three params as required"
	else:
		return lineInfo[2] * (x - lineInfo[0]) + lineInfo[1]

def f_specCuts_tag_2d(x,y):
	return f_specCuts_tag_1d(x) * f_specCuts_tag_1d(y)

def f_specCuts_anti_2d(x,y):
	return f_specCuts_anti_1d(x) * f_specCuts_anti_1d(y)

def f_specCuts_control_2d_orientation1(x,y):
	return f_specCuts_control_1d_LooseToMed2(x) * f_specCuts_control_1d_OffToIDBTCv21(y)

def f_specCuts_control_2d_orientation2(x,y):
	return f_specCuts_control_1d_OffToIDBTCv21(x) * f_specCuts_control_1d_LooseToMed2(y)

def f_fullCuts_anti_ht1500to2500_2d(x,y):
	return f_fullCuts_anti_ht1500to2500_1d(x) * f_fullCuts_anti_ht1500to2500_1d(y)

def f_fullCuts_anti_ht2500to3500_2d(x,y):
	return f_fullCuts_anti_ht2500to3500_1d(x) * f_fullCuts_anti_ht2500to3500_1d(y)

def f_fullCuts_anti_ht3500toInf_2d(x,y):
	return f_fullCuts_anti_ht3500toInf_1d(x) * f_fullCuts_anti_ht3500toInf_1d(y)

def f_fullCutsNOAK4_anti_ht1500to2500_2d(x,y):
	return f_fullCutsNOAK4_anti_ht1500to2500_1d(x) * f_fullCutsNOAK4_anti_ht1500to2500_1d(y)

def f_fullCutsNOAK4_anti_ht2500to3500_2d(x,y):
	return f_fullCutsNOAK4_anti_ht2500to3500_1d(x) * f_fullCutsNOAK4_anti_ht2500to3500_1d(y)

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

	lineInfo_negLow = [three_x_points_vec[i][2], three_x_points_vec[i][1], -1]
	lineInfo_negHigh = [three_x_points_vec[i+1][2], three_x_points_vec[i+1][1], -1]

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

	integral_U_specCuts_tag = 0
	integral_U_specCuts_anti = 0
	integral_U_fullCuts_anti_ht1500to2500 = 0
	integral_U_fullCuts_anti_ht2500to3500 = 0
	integral_U_fullCuts_anti_ht3500toInf = 0
	integral_U_fullCutsNOAK4_anti_ht1500to2500 = 0
	integral_U_fullCutsNOAK4_anti_ht2500to3500 = 0
	integral_U_specCuts_control = 0

	if (i==0):

		gradient_Spec = (yValue(three_x_points_vec[1][0],lineInfo_negHigh) - S1_Node1) / (three_x_points_vec[1][0] - S1_Node2)
		lineInfo_Spec = [S1_Node2, S1_Node1, gradient_Spec]
		def bounds_y_uSpec(x):
			return [yValue(x,lineInfo_Spec), yValue(x,lineInfo_negHigh)]

		integral_U_specCuts_tag += integrate.nquad(f_specCuts_tag_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_specCuts_tag += integrate.nquad(f_specCuts_tag_2d, [bounds_y_u3, bounds_x_u3])[0]
		
		integral_U_specCuts_anti += integrate.nquad(f_specCuts_anti_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_specCuts_anti += integrate.nquad(f_specCuts_anti_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCuts_anti_ht1500to2500 += integrate.nquad(f_fullCuts_anti_ht1500to2500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCuts_anti_ht1500to2500 += integrate.nquad(f_fullCuts_anti_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCuts_anti_ht2500to3500 += integrate.nquad(f_fullCuts_anti_ht2500to3500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCuts_anti_ht2500to3500 += integrate.nquad(f_fullCuts_anti_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCuts_anti_ht3500toInf += integrate.nquad(f_fullCuts_anti_ht3500toInf_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCuts_anti_ht3500toInf += integrate.nquad(f_fullCuts_anti_ht3500toInf_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsNOAK4_anti_ht1500to2500 += integrate.nquad(f_fullCutsNOAK4_anti_ht1500to2500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsNOAK4_anti_ht1500to2500 += integrate.nquad(f_fullCutsNOAK4_anti_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsNOAK4_anti_ht2500to3500 += integrate.nquad(f_fullCutsNOAK4_anti_ht2500to3500_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_fullCutsNOAK4_anti_ht2500to3500 += integrate.nquad(f_fullCutsNOAK4_anti_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]

		# U_control needs to do it with the function reflected
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation1, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation2, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation1, [bounds_y_u3, bounds_x_u3])[0]
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation2, [bounds_y_u3, bounds_x_u3])[0]


	else:
		integral_U_specCuts_tag += integrate.nquad(f_specCuts_tag_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_specCuts_tag += integrate.nquad(f_specCuts_tag_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_specCuts_tag += integrate.nquad(f_specCuts_tag_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_specCuts_anti += integrate.nquad(f_specCuts_anti_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_specCuts_anti += integrate.nquad(f_specCuts_anti_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_specCuts_anti += integrate.nquad(f_specCuts_anti_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCuts_anti_ht1500to2500 += integrate.nquad(f_fullCuts_anti_ht1500to2500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCuts_anti_ht1500to2500 += integrate.nquad(f_fullCuts_anti_ht1500to2500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCuts_anti_ht1500to2500 += integrate.nquad(f_fullCuts_anti_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCuts_anti_ht2500to3500 += integrate.nquad(f_fullCuts_anti_ht2500to3500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCuts_anti_ht2500to3500 += integrate.nquad(f_fullCuts_anti_ht2500to3500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCuts_anti_ht2500to3500 += integrate.nquad(f_fullCuts_anti_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCuts_anti_ht3500toInf += integrate.nquad(f_fullCuts_anti_ht3500toInf_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCuts_anti_ht3500toInf += integrate.nquad(f_fullCuts_anti_ht3500toInf_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCuts_anti_ht3500toInf += integrate.nquad(f_fullCuts_anti_ht3500toInf_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsNOAK4_anti_ht1500to2500 += integrate.nquad(f_fullCutsNOAK4_anti_ht1500to2500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsNOAK4_anti_ht1500to2500 += integrate.nquad(f_fullCutsNOAK4_anti_ht1500to2500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsNOAK4_anti_ht1500to2500 += integrate.nquad(f_fullCutsNOAK4_anti_ht1500to2500_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_fullCutsNOAK4_anti_ht2500to3500 += integrate.nquad(f_fullCutsNOAK4_anti_ht2500to3500_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_fullCutsNOAK4_anti_ht2500to3500 += integrate.nquad(f_fullCutsNOAK4_anti_ht2500to3500_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_fullCutsNOAK4_anti_ht2500to3500 += integrate.nquad(f_fullCutsNOAK4_anti_ht2500to3500_2d, [bounds_y_u3, bounds_x_u3])[0]

		# U_control needs to do it with the function reflected
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation1, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation2, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation1, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation2, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation1, [bounds_y_u3, bounds_x_u3])[0]
		integral_U_specCuts_control += 0.5 * integrate.nquad(f_specCuts_control_2d_orientation2, [bounds_y_u3, bounds_x_u3])[0]

	integral_S_specCuts_tag = integrate.nquad(f_specCuts_tag_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_specCuts_tag += integrate.nquad(f_specCuts_tag_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_specCuts_tag += integrate.nquad(f_specCuts_tag_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_specCuts_anti = integrate.nquad(f_specCuts_anti_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_specCuts_anti += integrate.nquad(f_specCuts_anti_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_specCuts_anti += integrate.nquad(f_specCuts_anti_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCuts_anti_ht1500to2500 = integrate.nquad(f_fullCuts_anti_ht1500to2500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCuts_anti_ht1500to2500 += integrate.nquad(f_fullCuts_anti_ht1500to2500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCuts_anti_ht1500to2500 += integrate.nquad(f_fullCuts_anti_ht1500to2500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCuts_anti_ht2500to3500 = integrate.nquad(f_fullCuts_anti_ht2500to3500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCuts_anti_ht2500to3500 += integrate.nquad(f_fullCuts_anti_ht2500to3500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCuts_anti_ht2500to3500 += integrate.nquad(f_fullCuts_anti_ht2500to3500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCuts_anti_ht3500toInf = integrate.nquad(f_fullCuts_anti_ht3500toInf_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCuts_anti_ht3500toInf += integrate.nquad(f_fullCuts_anti_ht3500toInf_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCuts_anti_ht3500toInf += integrate.nquad(f_fullCuts_anti_ht3500toInf_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCutsNOAK4_anti_ht1500to2500 = integrate.nquad(f_fullCutsNOAK4_anti_ht1500to2500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsNOAK4_anti_ht1500to2500 += integrate.nquad(f_fullCutsNOAK4_anti_ht1500to2500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsNOAK4_anti_ht1500to2500 += integrate.nquad(f_fullCutsNOAK4_anti_ht1500to2500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_fullCutsNOAK4_anti_ht2500to3500 = integrate.nquad(f_fullCutsNOAK4_anti_ht2500to3500_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_fullCutsNOAK4_anti_ht2500to3500 += integrate.nquad(f_fullCutsNOAK4_anti_ht2500to3500_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_fullCutsNOAK4_anti_ht2500to3500 += integrate.nquad(f_fullCutsNOAK4_anti_ht2500to3500_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_specCuts_control = integrate.nquad(f_specCuts_control_2d_orientation1, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_specCuts_control += integrate.nquad(f_specCuts_control_2d_orientation1, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_specCuts_control += integrate.nquad(f_specCuts_control_2d_orientation1, [bounds_y_s3, bounds_x_s3])[0]

	###############################################################
	###############################################################
	###############################################################
	###############################################################
	##### #### ### ## # CALCULATIONS AND OUTPUT # ## ### #### #####
	# Do calculations with the total counts in each 2d mass segment

	ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500 = integral_S_fullCuts_anti_ht1500to2500 / (2 * integral_U_fullCuts_anti_ht1500to2500)
	ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500 = integral_S_fullCuts_anti_ht2500to3500 / (2 * integral_U_fullCuts_anti_ht2500to3500)
	ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf = integral_S_fullCuts_anti_ht3500toInf / (2 * integral_U_fullCuts_anti_ht3500toInf)
	ratio__antiS_over_antiUnD__fullCutsNOAK4_anti_ht1500to2500 = integral_S_fullCutsNOAK4_anti_ht1500to2500 / (2 * integral_U_fullCutsNOAK4_anti_ht1500to2500)
	ratio__antiS_over_antiUnD__fullCutsNOAK4_anti_ht2500to3500 = integral_S_fullCutsNOAK4_anti_ht2500to3500 / (2 * integral_U_fullCutsNOAK4_anti_ht2500to3500)
	print ratio__antiS_over_antiUnD__fullCuts_anti_ht1500to2500
	print ratio__antiS_over_antiUnD__fullCuts_anti_ht2500to3500
	print ratio__antiS_over_antiUnD__fullCuts_anti_ht3500toInf
	# print ratio__antiS_over_antiUnD__fullCutsNOAK4_anti_ht1500to2500
	# print ratio__antiS_over_antiUnD__fullCutsNOAK4_anti_ht2500to3500
	print

	# BELOW IS OLD STUFF
	# correctionFactorTag = (integral_S_specCuts_tag / integral_S_specCuts_anti) * (integral_U_specCuts_anti / integral_U_specCuts_tag)
	# correctionFactorControl = (integral_S_specCuts_control / integral_S_specCuts_anti) * (integral_U_specCuts_anti / integral_U_specCuts_control)
	# ratio_antiS_over_antiUnD = integral_S_specCuts_anti / (2 * integral_U_specCuts_anti)
	# print "correction factor tag C_" + str(i) + " = " + str(correctionFactorTag)
	# print "correction factor control C_" + str(i) + " = " + str(correctionFactorControl)
	# print ratio_antiS_over_antiUnD
	
	# print integral_S_specCuts_tag
	# print integral_S_specCuts_anti
	# print integral_S_specCuts_control

	###############################################################
	###############################################################
	###############################################################
	###############################################################