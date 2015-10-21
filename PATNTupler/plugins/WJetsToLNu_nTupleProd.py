from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'wjets_nTuple_production'
config.General.workArea = 'crab_projects'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction.py'

config.Data.inputDataset = '/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.outLFNDirBase = '/store/user/olaiya/NTuples/WJetsToLNu' #Wherever you want the ntuples to go on the RAL T2
config.Data.publication = False
config.Data.publishDataName = 'WJets_nTuple_PROD'

config.Site.storageSite = 'T2_UK_SGrid_RALPP'
