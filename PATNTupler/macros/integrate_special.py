import math
from scipy import integrate

# run with
# $ python $CMSSW_BASE/src/NTupler/PATNTupler/macros/integrate.py

# notes on script:
# THIS IS FOR THE SPECIAL TTBAR SHAPE MassCutsSpecialV03
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
# fits valid between 50 and 250 GeV

def f1d_fullCutsData_anti_ht1500toInf(x):
	p0 = -1.01714e-02
	p1 =  2.82119e+00
	p2 = -2.32503e-03
	p3 =  2.41542e+01
	p4 = -3.62807e-02
	p5 = -1.12862e+04
	p6 = -1.44878e-03
	p7 =  3.41173e+05
	p8 = -2.56284e-01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsData_anti_ht1500toInf"] = f1d_fullCutsData_anti_ht1500toInf

def f1d_fullCutsMC_anti_ht1500toInf(x):
	p0 = -1.05506e-02
	p1 =  3.09793e+00
	p2 = -3.11320e-03
	p3 =  2.30161e+00
	p4 = -3.59524e-02
	p5 = -1.22350e+04
	p6 = -2.26181e-03
	p7 =  4.15630e+05
	p8 = -2.47028e-01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))
function_1d["fullCutsMC_anti_ht1500toInf"] = f1d_fullCutsMC_anti_ht1500toInf

def f1d_fullCutsMC_tag_ht1500toInf(x):
	p0 = -1.08150e-02
	p1 =  3.05618e+00
	p2 = -2.76424e-03
	p3 =  1.21268e+01
	p4 = -3.78982e-02
	p5 = -1.24147e+04
	p6 = -1.35383e-03
	p7 =  4.16109e+05
	p8 = -2.56256e-01
	return p0 + p1/(x-p2) + p3/((x-p4)*(x-p4)) + p5/((x-p6)*(x-p6)*(x-p6)) + p7/((x-p8)*(x-p8)*(x-p8)*(x-p8))	
function_1d["fullCutsMC_tag_ht1500toInf"] = f1d_fullCutsMC_tag_ht1500toInf

####################
####################
####################
# MASS REGION CUTS #
S1_Node1 = 195.0;
S1_Node2 = 100.0;
SMAX_Node1 = 201.0;
SMAX_Node2 = 170.0;
SN_Nodes = [197.0, 199.0];

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

	# define the integration bounds for x in the three U integral segments (first two bins)
	bounds_x_u1 = [three_x_points_vec[i][0], three_x_points_vec[i+1][0]]
	bounds_x_u2 = [three_x_points_vec[i+1][0], three_x_points_vec[i][1]]
	bounds_x_u3 = [three_x_points_vec[i][1], three_x_points_vec[i+1][1]]

	# define the integration bounds for y in the three U integral segments (first two bins)
	def bounds_y_u1(x):
		return [yValue(x,lineInfo_negLow), yValue(x,lineInfo_upperBand)]
	def bounds_y_u2(x):
		return [yValue(x,lineInfo_negLow), yValue(x,lineInfo_negHigh)]
	def bounds_y_u3(x):
		return [yValue(x,lineInfo_upperSignal), yValue(x,lineInfo_negHigh)]

	# define the integration bounds for x in the three U integral segments (last bin)
	bounds_x_u1_last = [three_x_points_vec[i][0], three_x_points_vec[i][1]]
	bounds_x_u2_last = [three_x_points_vec[i][1], three_x_points_vec[i+1][0]]
	bounds_x_u3_last = [three_x_points_vec[i+1][0], three_x_points_vec[i+1][1]]

	# define the integration bounds for y in the three U integral segments (last bin)
	def bounds_y_u2_last(x):
		return [yValue(x,lineInfo_upperSignal), yValue(x,lineInfo_upperBand)]

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

	if (i==1):
		for iK in range (0, len(keys)):
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u1, bounds_x_u1])[0]
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u2, bounds_x_u2])[0]
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u3, bounds_x_u3])[0]

	if (i==2):
		for iK in range (0, len(keys)):
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u1, bounds_x_u1_last])[0]
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u2_last, bounds_x_u2_last])[0]
			integral_U[ keys[iK] ] += integrate.nquad(function_2d[keys[iK]], [bounds_y_u3, bounds_x_u3_last])[0]

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
	ratio__antiS_over_antiUnD__fullCuts_anti_ht1500toInf = integral_S["fullCutsMC_anti_ht1500toInf"] / (2 * integral_U["fullCutsMC_anti_ht1500toInf"])
	print "data A_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500toInf)
	ratio__antiS_over_antiUnD__fullCuts_anti_ht1500toInf = integral_S["fullCutsData_anti_ht1500toInf"] / (2 * integral_U["fullCutsData_anti_ht1500toInf"])
	print "mc A_" + str(iMR) + " = " + str(ratio__antiS_over_antiUnD__fullCuts_anti_ht1500toInf)
	correctionFactor_ht1500toInf = (integral_S["fullCutsMC_tag_ht1500toInf"] / integral_S["fullCutsMC_anti_ht1500toInf"]) * (integral_U["fullCutsMC_anti_ht1500toInf"] / integral_U["fullCutsMC_tag_ht1500toInf"])
	print "correction factor C_" + str(iMR) + " = " + str(correctionFactor_ht1500toInf)
	print
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################
	###############################################################