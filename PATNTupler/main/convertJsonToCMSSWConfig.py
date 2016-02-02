#!/usr/bin/env python


import sys,ConfigParser,os,string,commands,time,re

# for json support
try: # FUTURE: Python 2.6, prior to 2.6 requires simplejson
    import json
except:
    try:
        import simplejson as json
    except:
        print "Please use lxplus or set an environment (for example crab) with json lib available"
        sys.exit(1)


#######################################################################################
#main starts here#


# reading config file
if len(sys.argv)<2:
    print "Usage: ",sys.argv[0],"<json> <min runnr> (optional)"
    sys.exit(1)



jsonName=sys.argv[1]
minRunnrStr="0"
if len(sys.argv)==3:
    minRunnrStr=sys.argv[2]


if not minRunnrStr.isdigit():
    print "min run nr parameter not understood:"+minRunnrStr
    sys.exit(1)


minRunnr=int(minRunnrStr)

runnrs=[]

jsondict={}
# read json file
jsonfile=file(jsonName,'r')
jsondict = json.load(jsonfile)



for run in jsondict.keys():
    #print "Reading run: "+run
    runStr=str(run)
  #  print run," ",jsondict[run]
    for lumiPair in jsondict[run]:
        print "       \'{0}:{1}-{0}:{2}\',".format(run,lumiPair[0],lumiPair[1])
       
    if int(run)>=minRunnr:
        runnrs.append(run)


runnrs.sort()
