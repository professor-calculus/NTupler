from scipy import integrate


# STILL HAVEN'T GOT THE FIRST SIGNAL REGION AS IT IS A DIFFERENT SHAPE!!!
#assumes independence...

#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################
#################################################################################################


# FUNCTIONS #

# The normalised 1d softdropmass functionality for 'tag' AK8Jets CURRENTLY DEMO
def f_tag_1d(x):
	return 1/(x*x)

# The normalised 1d softdropmass functionality for 'anti' AK8Jets CURRENTLY DEMO
def f_anti_1d(x):
	return 1/(x**2.1)


# MassRegionCuts
S1_Node1 = 31.5;
S1_Node2 = 16.5;
SMAX_Node1 = 168.5;
SMAX_Node2 = 115.04;
SN_Nodes = [40.7, 50.9, 62.1, 74.3, 87.5, 101.7, 116.9, 133.1, 150.3];


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

for SN_Node in SN_Nodes:
	x2 = SN_Node
	x1 = yValue(SN_Node, lineInfo_lowerSignal)
	x0 = x1 - 0.5 * (x2 - x1)
	three_x_points_vec.append([x0, x1, x2])

three_x_points_vec.append([upperBand_x2, SMAX_Node2, SMAX_Node1])

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
	
	integral_U_tag = integrate.nquad(f_tag_2d, [bounds_y_u1, bounds_x_u1])[0]
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
	print correctionFactor
