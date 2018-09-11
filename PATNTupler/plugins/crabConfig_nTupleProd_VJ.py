from CRABClient.UserUtilities import config
config = config()

# submit, from plugins directory, with $ crab submit -c crabConfig_nTupleProd_VJ.py
# check $ cmsRun nTupleProduction.py, is setup correctly first (and that RALMiniAnalyzer is compiled)!!!
# EDIT the following:
# uniqueName, config.Data.inputDataset, config.Data.inputDBS, config.Data.unitsPerJob

uniqueName = 'MORIOND2017_SingleTop_tW_Top_CMSSW8029_titterton' # name for this ntuple production (include commit hash so it is unique and can be traced) 

config.General.requestName = uniqueName # name of the crab job project (eg on dashboard)
config.General.workArea = 'crab_projects' # name of directory where crab project info is stored
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'nTupleProduction.py'
config.JobType.inputFiles = ['puppiCorr.root']

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

# MORIOND2017_TTWJetsToLNu_part1: 2M events, 33 files = 55K events/file (three units per job)
# config.Data.inputDataset = '/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM'

# MORIOND2017_TTWJetsToLNu_part2: 3M events, 49 files = 55K events/file (three units per job)
# config.Data.inputDataset = '/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM'

# MORIOND2017_TTWJetsToQQ: 800K events, 15 files, 55K events/file (three units per job)
# config.Data.inputDataset = '/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

# MORIOND2017_TTZToLLNuNu_part1: 2M events, 36 files, 55K events/file (three units per job)
# config.Data.inputDataset = '/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_TTZToLLNuNu_part2: 6M events, 94 files, 60K events/file (three units per job)
# config.Data.inputDataset = '/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM'

# MORIOND2017_TTZToLLNuNu_part3: 6M events, 149 files, 40K events/file (three units/job)
# config.Data.inputDataset = '/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext3-v1/MINIAODSIM'

# MORIOND2017_TTZToQQ: 750K events, 19 files, 40K events/file (three units/job)
# config.Data.inputDataset = '/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

# MORIOND2017_WZ: 3M events, 128 files, 25k events/file (five units/job)
# config.Data.inputDataset = '/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_WW: 7M events, 116 files, 60K events/file (three units per job)
# config.Data.inputDataset = '/WW_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_ZZ: 1M events, 66 files, 18K events/file (five units per job)
# config.Data.inputDataset = '/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_SingleTop_t-channel_AntiTop: 39M events, 406 files, 100k events/file (two units/job)
# config.Data.inputDataset = '/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

# MORIOND2017_SingleTop_t-channel_Top: 67M events, 723 files, 95k events/file (two units per job)
# config.Data.inputDataset = '/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

# MORIOND2017_SingleTop_s-channel: 3M events, 37 files, 90K events/file (two units per job)
# config.Data.inputDataset = '/ST_s-channel_4f_InclusiveDecays_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

# MORIOND2017_SingleTop_tW_AntiTop: 7M events, 144 files, 50k events/file (three units per job)
# config.Data.inputDataset = '/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# MORIOND2017_SingleTop_tW_Top: 7M events, 161 files, 45k events/file (three units/job)
config.Data.inputDataset = '/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'

# SIGNAL SAMPLES: 200K events, 100 files = ~2K events per unit (use ten units per job)
# config.Data.inputDataset = 'DATASETXYZ' # use the script quickSignalSubmit_crab.py to deploy

# CENTRAL PRODUCTION SIGNAL: (use one unit per job)
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-1200_TuneCUEP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-2000_TuneCUEP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'
# config.Data.inputDataset = '/NMSSMCascade_mH-70_mSUSY-2600_TuneCUEP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM'

#################################################################################################
config.Data.inputDBS = 'global' # for centrally produced data
# config.Data.inputDBS = 'phys03' # for personal simulation
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 3
#################################################################################################

config.Data.totalUnits = -1 # '-1' does them all
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/atittert/ntuples_MC_AUG2018_v2' # Wherever you want the ntuples to go on the RAL T2

config.Site.storageSite = 'T2_UK_SGrid_RALPP'
