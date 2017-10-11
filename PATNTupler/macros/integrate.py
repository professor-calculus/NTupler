from scipy import integrate

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/integrate.py

# STILL HAVEN'T GOT THE FIRST SIGNAL REGION AS IT IS A DIFFERENT SHAPE!!!

# notes on script:
# it is very much geometry specific and not completely general for any MassRegion object
# assumes indendence between the 2 fatJet mass distributions
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################

# FUNCTIONS #

# The normalised 1d softdropmass functionality for 'tag' AK8Jets
def f_tag_1d(x):
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

# The normalised 1d softdropmass functionality for 'anti' AK8Jets CURRENTLY DEMO
def f_anti_1d(x):
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


# MASS REGION CUTS #

# MassCutsV04
S1_Node1 = 31.5;
S1_Node2 = 16.5;
SMAX_Node1 = 168.5;
SMAX_Node2 = 115.04;
SN_Nodes = [40.7, 50.9, 62.1, 74.3, 87.5, 101.7, 116.9, 133.1, 150.3];

# NEW ATTEMPTS
# S1_Node1 = 31.5;
# S1_Node2 = 16.5;
# SMAX_Node1 = 168.5;
# SMAX_Node2 = 115.04;
# SN_Nodes = [40.7, 50.9, 62.1, 74.3, 87.5, 101.7, 116.9, 133.1, 150.3];

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

# The normalised 2d softdropmass functionality for 'tag' AK8Jets
def f_tag_2d(x,y):
	return f_tag_1d(x) * f_tag_1d(y)

# The normalised 2d softdropmass functionality for 'anti' AK8Jets
def f_anti_2d(x,y):
	return f_anti_1d(x) * f_anti_1d(y)

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

	integral_U_tag = 0
	integral_U_anti = 0
	
	if (i==0):

		gradient_Spec = (yValue(three_x_points_vec[1][0],lineInfo_negHigh) - S1_Node1) / (three_x_points_vec[1][0] - S1_Node2)
		lineInfo_Spec = [S1_Node2, S1_Node1, gradient_Spec]
		def bounds_y_uSpec(x):
			return [yValue(x,lineInfo_Spec), yValue(x,lineInfo_negHigh)]

		integral_U_tag += integrate.nquad(f_tag_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_tag += integrate.nquad(f_tag_2d, [bounds_y_u3, bounds_x_u3])[0]
		
		integral_U_anti += integrate.nquad(f_anti_2d, [bounds_y_uSpec, bounds_x_u2])[0]
		integral_U_anti += integrate.nquad(f_anti_2d, [bounds_y_u3, bounds_x_u3])[0]

	else:
		integral_U_tag += integrate.nquad(f_tag_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_tag += integrate.nquad(f_tag_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_tag += integrate.nquad(f_tag_2d, [bounds_y_u3, bounds_x_u3])[0]

		integral_U_anti = integrate.nquad(f_anti_2d, [bounds_y_u1, bounds_x_u1])[0]
		integral_U_anti += integrate.nquad(f_anti_2d, [bounds_y_u2, bounds_x_u2])[0]
		integral_U_anti += integrate.nquad(f_anti_2d, [bounds_y_u3, bounds_x_u3])[0]

	integral_S_tag = integrate.nquad(f_tag_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_tag += integrate.nquad(f_tag_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_tag += integrate.nquad(f_tag_2d, [bounds_y_s3, bounds_x_s3])[0]

	integral_S_anti = integrate.nquad(f_anti_2d, [bounds_y_s1, bounds_x_s1])[0]
	integral_S_anti += integrate.nquad(f_anti_2d, [bounds_y_s2, bounds_x_s2])[0]
	integral_S_anti += integrate.nquad(f_anti_2d, [bounds_y_s3, bounds_x_s3])[0]

	correctionFactor = (integral_S_tag / integral_S_anti) * (integral_U_anti / integral_U_tag)
	print "correction factor C_" + str(i) + " = " + str(correctionFactor)
