from CRABClient.UserUtilities import config
config = config()

# submit, from plugins directory, with $ crab submit -c crabConfig_nTupleProd_VJ.py
# check $ cmsRun nTupleProduction.py, is setup correctly first (and that RALMiniAnalyzer is compiled)!!!
# EDIT the following:
# uniqueName, config.Data.inputDataset, config.Data.inputDBS, config.Data.unitsPerJob

uniqueName = 'NAMEXYZ_CMSSW8029wDBTV4_fbd67939' # name for this ntuple production (include commit hash so it is unique and can be traced) 

config.General.requestName = uniqueName # name of the crab job project (eg on dashboard)
config.General.workArea = 'crab_projects' # name of directory where crab project info is stored
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction.py'
config.JobType.inputFiles = ['puppiCorr.root', 'L1PrefiringMaps_new.root']

#################################################################################################
# DATASETS --------------------------------------------------------------------------------------

# MORIOND2017_TT_NLO_POWHEG_v1: 77M events, 982 files = 78K events/file (use two units per job)
# config.Data.inputDataset = '/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
# MORIOND2017_TT_NLO_POWHEG_v2: similar credentials to above
# config.Data.inputDataset = '/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

# MORIOND2017_QCD_HT700to1000_v1: 15.6M events, 184 files = 85K events/file (use two units per job)
# config.Data.inputDataset = '/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
# MORIOND2017_QCD_HT700to1000_v2: 29M events, 364 files = 79K events/file (use two units per job)
# config.Data.inputDataset = '/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_QCD_HT1000to1500_v1: 4.7M events, 90 files = 52K events/file (use three units per job)
# config.Data.inputDataset = '/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
# MORIOND2017_QCD_HT1000to1500_v2: 10.3M events, 170 files = 60K events/file (use three units per job)
# config.Data.inputDataset = '/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_QCD_HT1500to2000_v1: 4.0M events, 80 files = 50K events/file (use three units per job)
# config.Data.inputDataset = '/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
# MORIOND2017_QCD_HT1500to2000_v2: 7.8M events, 136 files = 57K events (use three units per job)
# config.Data.inputDataset = '/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_QCD_HT2000toInf_v1: 2.0M events, 36 files = 55K events/file (use three units per job)
# config.Data.inputDataset = '/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
# MORIOND2017_QCD_HT2000toInf_v2: 4.0M events, 86 files = 46K events (use three units per job)
# config.Data.inputDataset = '/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_ZJets_HT600toInf: 1.0M events, 28 files = 37K events/file (use three units per job)
# config.Data.inputDataset = '/ZJetsToQQ_HT600toInf_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

# MORIOND2017_WJets_HT600toInf: 1.0M events, 18 files = 55K events/file (use three units per job)
# config.Data.inputDataset = '/WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

# SIGNAL SAMPLES: 200K events, 100 files = ~2K events per unit (use ten units per job)
config.Data.inputDataset = 'DATASETXYZ' # use the script quickSignalSubmit_crab.py to deploy

# CENTRAL PRODUCTION SIGNAL: (use one unit per job)
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-1200_TuneCUEP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-2000_TuneCUEP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-2600_TuneCUEP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'

#################################################################################################
# config.Data.inputDBS = 'global' # for centrally produced data
config.Data.inputDBS = 'phys03' # for personal simulation
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
#################################################################################################

config.Data.totalUnits = -1 # '-1' does them all
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/taylor/ntuples_MC16_Jan2019' # Wherever you want the ntuples to go on the RAL T2

config.Site.storageSite = 'T2_UK_SGrid_RALPP'