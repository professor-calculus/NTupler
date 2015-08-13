from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'RAL_nTuple_production'
config.General.workArea = 'crab_projects'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction.py'

config.Data.inputDataset = '/ZPrimePSIToEEMuMu_M-4000_13TeV_pythia8/Spring14miniaod-141029_PU40bx50_PLS170_V6AN2-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.outLFNDirBase = '/store/user/olaiya/CRABTest' #Wherever you want the ntuples to go on the RAL T2
config.Data.publication = False
config.Data.publishDataName = 'RAL_nTuple_PROD'

config.Site.storageSite = 'T2_UK_SGrid_RALPP'
