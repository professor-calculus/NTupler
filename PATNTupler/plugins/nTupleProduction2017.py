# important to keep up-to-date:
# 1. Global Tag
# 2. dataset (for local running only)
# 3. HLT trigger path
# 4. LHE calculation properties (currently not using)
# 5. puppiCorr.root file (currently not necessary for 2017)

import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.register ('useMiniAOD',
                  False,
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.bool,
                  "use miniAOD rather than AOD")
options.parseArguments()
useMiniAOD=options.useMiniAOD

# set up process
process = cms.Process("HEEP")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(10),
    limit = cms.untracked.int32(100)
)
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')

process.TFileService = cms.Service("TFileService", fileName=cms.string('nTuple.root'))

#setup global tag
from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_mc2017_realistic_v12', '') #


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring(
        'root://dcap.pp.rl.ac.uk:1094/pnfs/pp.rl.ac.uk/data/cms/store/mc/RunIIFall17DRPremix/ZJetsToQQ_HT-800toInf_TuneCP5_13TeV-madgraphMLM-pythia8/AODSIM/94X_mc2017_realistic_v10-v1/40000/000A17B8-2D05-E811-87F8-0CC47AD98F74.root'
)
                                       
)
useMiniAOD=True

if useMiniAOD==True:
    # process.source.fileNames=cms.untracked.vstring('/store/user/taylor/nmssmSignalCascadeV05_13TeV2017_mH70p0_mSusy2200p0_ratio0p99_splitting0p1/nmssmSignalCascadeV05_13TeV2017_processMc04_ed94Xv8_mH70p0_mSusy2200p0_ratio0p99_splitting0p1/180509_094757/0000/nmssmSignal_MINIAODSIM_1.root',)
    process.source.fileNames=cms.untracked.vstring('file:/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/exampleMiniAOD/nmssmSignal_MINIAODSIM_94X.root')
    # process.source.fileNames=cms.untracked.vstring('file:/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/exampleMiniAOD/TT_MINIAODSIM_94X.root')

#setup the VID with HEEP 7.0
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate
if useMiniAOD:
    switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)
else:
    switchOnVIDElectronIdProducer(process, DataFormat.AOD)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff']
#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)


#this is our example analysis module reading the results
process.demo = cms.EDAnalyzer("RALMiniAnalyzer",
                                       isThis2016 = cms.bool(False),
                                       isThisMC = cms.bool(True),
                                       #mcWeight = cms.double(MCWEIGHT_INSERTEDHERE),
                                       # containsLHE = cms.bool(True),
                                       containsLHE = cms.bool(False), # use for personal MC samples that do not have the correct lhe info format
                                       lhe = cms.InputTag("externalLHEProducer"),
                                       ignoreTopInLheHtCalculation = cms.bool(False), # if true will ignore top quarks and daughter particles in the ht calculation (also does so for W, Z, H)
                                       heepId = cms.InputTag("heepId"),
                                       vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                       puInfo = cms.InputTag("slimmedAddPileupInfo"),
                                       muons = cms.InputTag("slimmedMuons"),
                                       electrons = cms.InputTag("slimmedElectrons"),
                                       photons = cms.InputTag("slimmedPhotons"),
                                       tracks = cms.InputTag("packedPFCandidates"),
                                       jets = cms.InputTag("slimmedJets"),
                                       fatjets = cms.InputTag("slimmedJetsAK8"),
                                       genjets = cms.InputTag("slimmedGenJets"),
                                       genjetsAK8 = cms.InputTag("slimmedGenJetsAK8"),
                                       genParticles = cms.InputTag("prunedGenParticles"),
                                       mets = cms.InputTag("slimmedMETs"),       
                                       bits = cms.InputTag("TriggerResults","","HLT"),
                                       prescales = cms.InputTag("patTrigger"),
                                       objects = cms.InputTag("selectedPatTrigger"),
                                       selectedTriggerPaths = cms.vstring("HLT_PFHT1050_v", "HLT_AK8PFJet500_v", "HLT_AK8PFJet550_v"),#matches triggers that *contain* the stated names, so finish with _v to make sure your trigger name isn't a subset of others
                                       elesAOD=cms.InputTag("gedGsfElectrons"),
                                       elesMiniAOD=cms.InputTag("slimmedElectrons"),
                                       trkIsolMap=cms.InputTag("heepIDVarValueMaps","eleTrkPtIso"),
                                       vid=cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Fall17-94X-V1-loose"),
                                       rho=cms.InputTag("fixedGridRhoAll"),
                                       rhoPhoton=cms.InputTag("fixedGridRhoFastjetAll")
                                       )

process.p = cms.Path(
    process.egmGsfElectronIDSequence* 
    process.demo) #our analysing example module, replace with your module
