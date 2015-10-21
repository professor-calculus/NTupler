from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'MuonEG_nTuple_production'
config.General.workArea = 'crab_projects'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction_Data.py'

config.Data.lumiMask = 'Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON.txt'
config.Data.inputDataset = '/MuonEG/Run2015B-PromptReco-v1/MINIAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.outLFNDirBase = '/store/user/olaiya/NTuples/MuonEG' #Wherever you want the data to go on the RAL T2
config.Data.publication = False
config.Data.publishDataName = 'MuonEG_nTuple_PROD'

config.Site.storageSite = 'T2_UK_SGrid_RALPP'
