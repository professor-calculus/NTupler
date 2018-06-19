from CRABClient.UserUtilities import config
config = config()

# submit, from plugins directory, with $ crab submit -c crabConfig_nTupleProd_data2017_VJ.py
# check $ cmsRun nTupleProduction_Data2017.py, is setup correctly first (and that RALMiniAnalyzer is compiled)!!!
# EDIT the following:
# uniqueName, config.Data.lumiMask, config.Data.inputDataset, config.Data.unitsPerJob

uniqueName = 'SingleMuon_Run2017F_CMSSW947_b89f068' # name for this ntuple production (include commit hash so it is unique and can be traced) 

config.Data.lumiMask = 'Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt'

config.General.requestName = uniqueName # name of the crab job project (eg on dashboard)
config.General.workArea = 'crab_projects' # name of directory where crab project info is stored
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction_Data2017.py'
config.JobType.inputFiles = ['puppiCorr.root']

#################################################################################################
# DATASETS --------------------------------------------------------------------------------------

# JetHT 2017 - 31Mar2018: (use five units per job) 
# config.Data.inputDataset = '/JetHT/Run2017B-31Mar2018-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2017C-31Mar2018-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2017D-31Mar2018-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2017E-31Mar2018-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2017F-31Mar2018-v1/MINIAOD'

# SingleMuon 2017 - 31Mar2018: (SETUP TO SELECT ALL EVENTS IN SOURCE CODE, use one unit per job)
# config.Data.inputDataset = '/SingleMuon/Run2017B-31Mar2018-v1/MINIAOD'
# config.Data.inputDataset = '/SingleMuon/Run2017C-31Mar2018-v1/MINIAOD'
# config.Data.inputDataset = '/SingleMuon/Run2017D-31Mar2018-v1/MINIAOD'
# config.Data.inputDataset = '/SingleMuon/Run2017E-31Mar2018-v1/MINIAOD'
config.Data.inputDataset = '/SingleMuon/Run2017F-31Mar2018-v1/MINIAOD'

#################################################################################################
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
#################################################################################################

config.Data.inputDBS = 'global' # for centrally produced data
config.Data.totalUnits = -1 # '-1' does them all
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/taylor/ntuples_DATA17_June2018' # Wherever you want the ntuples to go on the RAL T2

config.Site.storageSite = 'T2_UK_SGrid_RALPP'