from CRABClient.UserUtilities import config
config = config()

# submit, from plugins directory, with $ crab submit -c crabConfig_nTupleProd_VJ.py
# EDIT the following
# uniqueName, config.Data.inputDataset, config.Data.unitsPerJob, (config.Data.outLFNDirBase)

uniqueName = 'SAMPLE_CMSSWXYZ_COMMITHASH' # name for this ntuple production (include commit hash so it is unique and can be traced) 
config.General.requestName = uniqueName # name of the crab job project (eg on dashboard)
config.General.workArea = 'crab_projects' # name of directory where crab project info is stored
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction.py'

#################################################################################################
# DATASETS --------------------------------------------------------------------------------------

# MORIOND2017_TT_NLO_POWHEG_v1: 77M events, 982 files = 78K events/file (use two units per job)
config.Data.inputDataset = '/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
# MORIOND2017_TT_NLO_POWHEG_v2: similar credentials to above
# config.Data.inputDataset = '/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
#################################################################################################

config.Data.unitsPerJob = 2
config.Data.totalUnits = -1 # '-1' does them all
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/taylor/ntuples_MC' # Wherever you want the ntuples to go on the RAL T2
# config.Data.outLFNDirBase = '/store/user/taylor/ntuples_DATA' # Wherever you want the ntuples to go on the RAL T2

config.Site.storageSite = 'T2_UK_SGrid_RALPP'