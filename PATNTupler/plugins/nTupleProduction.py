# important to keep up-to-date:
# 1. Global Tag
# 2. dataset (for local running only)
# 3. HLT trigger path
# 4. LHE calculation properties (currently not using)
# 5. puppiCorr.root file

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
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v8', '') #

process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring(
        'root://dcap.pp.rl.ac.uk:1094/pnfs/pp.rl.ac.uk/data/cms/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root'
)
                                       
)
useMiniAOD=True

if useMiniAOD==True:
    process.source.fileNames=cms.untracked.vstring('/store/user/atittert/nmssmP1SignalCascadeV01_13TeV2016_1200sq_1210go_130X2/nmssmP1SignalCascadeV01_13TeV2016_processMc04_ed80X_P1_v1_1200sq_1210go_130X2/180528_132105/0000/nmssmSignal_MINIAODSIM_1.root')
    # process.source.fileNames=cms.untracked.vstring('file:/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/exampleMiniAOD/nmssmSignal_MINIAODSIM_80X.root')
    # process.source.fileNames=cms.untracked.vstring('file:/opt/ppd/scratch/xap79297/Analysis_boostedNmssmHiggs/exampleMiniAOD/TT_MINIAODSIM_80X.root')

#setup the VID with HEEP 7.0
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate
if useMiniAOD:
    switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)
else:
    switchOnVIDElectronIdProducer(process, DataFormat.AOD)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff', 'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff']
#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

# update slimmedJetsAK8 to include version4 of the double-b tagger (and redo JECs)
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
updateJetCollection(
   process,
   labelName = 'NewJEC',
   jetSource = cms.InputTag('slimmedJets'),
   jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
   btagDiscriminators = [
      'pfDeepCSVJetTags:probudsg',
      'pfDeepCSVJetTags:probb',
      'pfDeepCSVJetTags:probc',
      'pfDeepCSVJetTags:probbb',
      'pfDeepCSVJetTags:probcc',
      'pfDeepCMVAJetTags:probudsg',
      'pfDeepCMVAJetTags:probb',
      'pfDeepCMVAJetTags:probc',
      'pfDeepCMVAJetTags:probbb',
      'pfDeepCMVAJetTags:probcc',
   ],
)
updateJetCollection(
   process,
   labelName = 'AK8wDBTV4AndNewJEC',
   jetSource = cms.InputTag('slimmedJetsAK8'),
   pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
   svSource = cms.InputTag('slimmedSecondaryVertices'),
   rParam = 0.8,
   jetCorrections = ('AK8PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
   btagDiscriminators = [
      'pfBoostedDoubleSecondaryVertexAK8BJetTags',
      'pfDeepCSVJetTags:probudsg',
      'pfDeepCSVJetTags:probb',
      'pfDeepCSVJetTags:probc',
      'pfDeepCSVJetTags:probbb',
      'pfDeepCSVJetTags:probcc',
      'pfDeepCMVAJetTags:probudsg',
      'pfDeepCMVAJetTags:probb',
      'pfDeepCMVAJetTags:probc',
      'pfDeepCMVAJetTags:probbb',
      'pfDeepCMVAJetTags:probcc',
   ],
)

# this is our example analysis module reading the results
process.demo = cms.EDAnalyzer("RALMiniAnalyzer",
                                       isThis2016 = cms.bool(True),
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
                                       photons = cms.InputTag("slimmedPhotons"),
                                       tracks = cms.InputTag("packedPFCandidates"),
                                       electrons = cms.InputTag("slimmedElectrons"),
                                       jets = cms.InputTag("updatedPatJetsNewJEC"),
                                       fatjets = cms.InputTag("updatedPatJetsTransientCorrectedAK8wDBTV4AndNewJEC"),
                                       genjets = cms.InputTag("slimmedGenJets"),
                                       genjetsAK8 = cms.InputTag("slimmedGenJetsAK8"),
                                       genParticles = cms.InputTag("prunedGenParticles"),
                                       mets = cms.InputTag("slimmedMETs"),       
                                       bits = cms.InputTag("TriggerResults","","HLT"),
                                       prescales = cms.InputTag("patTrigger"),
                                       objects = cms.InputTag("selectedPatTrigger"),
                                       selectedTriggerPaths = cms.vstring("HLT_PFHT900_v", "HLT_AK8PFJet450_v"),#matches triggers that *contain* the stated names, so finish with _v to make sure your trigger name isn't a subset of others
                                       elesAOD=cms.InputTag("gedGsfElectrons"),
                                       elesMiniAOD=cms.InputTag("slimmedElectrons"),
                                       trkIsolMap=cms.InputTag("heepIDVarValueMaps","eleTrkPtIso"),
                                       vid=cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose"),
                                       rho=cms.InputTag("fixedGridRhoAll"),
                                       rhoPhoton=cms.InputTag("fixedGridRhoFastjetAll")
                                       )

process.p = cms.Path(
    process.egmGsfElectronIDSequence*
    process.demo) #our analysing example module, replace with your module
