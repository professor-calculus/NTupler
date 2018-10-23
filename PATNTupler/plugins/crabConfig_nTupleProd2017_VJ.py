from CRABClient.UserUtilities import config
config = config()

# submit, from plugins directory, with $ crab submit -c crabConfig_nTupleProd2017_VJ.py
# check $ cmsRun nTupleProduction2017.py, is setup correctly first (and that RALMiniAnalyzer is compiled)!!!
# EDIT the following:
# uniqueName, config.Data.inputDataset, config.Data.inputDBS, config.Data.unitsPerJob

uniqueName = 'WJetsToQQ_HT800_CMSSW947_2ec37e_v3' # name for this ntuple production (include commit hash so it is unique and can be traced) 

config.General.requestName = uniqueName # name of the crab job project (eg on dashboard)
config.General.workArea = 'crab_projects' # name of directory where crab project info is stored
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction2017.py'
config.JobType.inputFiles = ['puppiCorr.root']

#################################################################################################
# DATASETS --------------------------------------------------------------------------------------

# TT to SemiLeptonic: 41M+111M(PS) events: (use three units per job)
# config.Data.inputDataset = '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'
# config.Data.inputDataset = '/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'

# TT to Hadronic: 42M+131M(PS) events: (use three units per job)
# config.Data.inputDataset = '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
# config.Data.inputDataset = '/TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'

# TT to 2L2Nu: 9M+67M(PS) events: (use three units per job)
# config.Data.inputDataset = '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
# config.Data.inputDataset = '/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'

# Z to Jets HT800+: 9.8M events: (use three units per job) 
# config.Data.inputDataset = '/ZJetsToQQ_HT-800toInf_qc19_4j_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
# config.Data.allowNonValidInputDataset = True # in order to use the non-complete dataset

# W to Jets HT800+: 9.4M events: (use four units per job)
config.Data.inputDataset = '/WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
config.Data.allowNonValidInputDataset = True # in order to use the non-complete dataset

# QCD HT1000-1500: 16.6M events: (use three units per job)
# config.Data.inputDataset = '/QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'

# QCD HT1500-2000: 11.3M events: (use three units per job)
# config.Data.inputDataset = '/QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_old_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'

# QCD HT2000+: 5.4M events: (use three units per job)
# config.Data.inputDataset = '/QCD_HT2000toInf_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_old_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM'

# SIGNAL SAMPLES: 200K events & 150 files (use 30 units per job)
# config.Data.inputDataset = 'DATASETXYZ' # use the script quickSignalSubmit_crab.py to deploy

# CENTRAL PRODUCTION SIGNAL: (use one unit per job)
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-1200_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-2000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-2600_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'

#################################################################################################
config.Data.inputDBS = 'global' # for centrally produced data
# config.Data.inputDBS = 'phys03' # for personal simulation
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 3
#################################################################################################

config.Data.totalUnits = -1 # '-1' does them all
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/taylor/ntuples_MC17_June2018' # Wherever you want the ntuples to go on the RAL T2

config.Site.storageSite = 'T2_UK_SGrid_RALPP'