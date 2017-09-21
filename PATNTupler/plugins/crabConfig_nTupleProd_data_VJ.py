from CRABClient.UserUtilities import config
config = config()

# submit, from plugins directory, with $ crab submit -c crabConfig_nTupleProd_data_VJ.py
# check $ cmsRun nTupleProduction_Data.py, is setup correctly first (and that RALMiniAnalyzer is compiled)!!!
# EDIT the following:
# uniqueName, config.Data.lumiMask, config.Data.inputDataset, config.Data.unitsPerJob

uniqueName = 'TESTING_JetHt_Run2016H-03Feb2017_ver3-v1_CMSSW8029wDBTV4_7b9754e' # name for this ntuple production (include commit hash so it is unique and can be traced) 
config.Data.lumiMask = 'Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt'

config.General.requestName = uniqueName # name of the crab job project (eg on dashboard)
config.General.workArea = 'crab_projects' # name of directory where crab project info is stored
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction_Data.py'

#################################################################################################
# DATASETS --------------------------------------------------------------------------------------

# JetHT 2016 - 03Feb2017: ~ 100k events/file (use two units per job) 
# config.Data.inputDataset = '/JetHT/Run2016B-03Feb2017_ver1-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2016B-03Feb2017_ver2-v2/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2016C-03Feb2017-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2016D-03Feb2017-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2016E-03Feb2017-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2016F-03Feb2017-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2016G-03Feb2017-v1/MINIAOD'
# config.Data.inputDataset = '/JetHT/Run2016H-03Feb2017_ver2-v1/MINIAOD'
config.Data.inputDataset = '/JetHT/Run2016H-03Feb2017_ver3-v1/MINIAOD'

#################################################################################################
config.Data.unitsPerJob = 1
#################################################################################################

config.Data.inputDBS = 'global' # for centrally produced data
config.Data.totalUnits = -1 # '-1' does them all
config.Data.splitting = 'FileBased'
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/taylor/ntuples_DATA' # Wherever you want the ntuples to go on the RAL T2

config.Site.storageSite = 'T2_UK_SGrid_RALPP'