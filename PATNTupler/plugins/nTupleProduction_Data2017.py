# important to keep up-to-date:
# 1. Global Tag
# 2. dataset (for local running only)
# 3. HLT trigger path
# 4. puppiCorr.root & L1PrefiringMaps_new.root file

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
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_v11', '') #


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring(
        'root://dcap.pp.rl.ac.uk:1094/pnfs/pp.rl.ac.uk/data/cms/store/data/Run2017E/JetHT/AOD/17Nov2017-v1/20000/00066995-78D0-E711-A5A0-A4BF0112BD14.root'
)
                                       
)
useMiniAOD=True

if useMiniAOD==True:
    # process.source.fileNames=cms.untracked.vstring('/store/data/Run2017E/JetHT/MINIAOD/31Mar2018-v1/00000/00A450F8-2738-E811-8EF7-008CFAC93C0C.root',)
    process.source.fileNames=cms.untracked.vstring('/store/data/Run2017F/JetHT/MINIAOD/31Mar2018-v1/80000/FE96BCE3-1737-E811-BF0B-A4BF0112BC1A.root',)
    # process.source.fileNames=cms.untracked.vstring('/store/data/Run2017E/SingleMuon/MINIAOD/31Mar2018-v1/00000/000D53C5-9D39-E811-A39C-0025905B85A0.root',)

#setup the VID with HEEP 7.0
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate
if useMiniAOD:
    switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)
else:
    switchOnVIDElectronIdProducer(process, DataFormat.AOD)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff', 'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V1_cff']
#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)


from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
# update slimmedJets with new JECs
updateJetCollection(
   process,
   labelName = 'NewJEC',
   jetSource = cms.InputTag('slimmedJets'),
   jetCorrections = ('AK4PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None'),
)
# update slimmedJetsAK8 with new JECs
updateJetCollection(
   process,
   labelName = 'AK8NewJEC',
   jetSource = cms.InputTag('slimmedJetsAK8'),
   jetCorrections = ('AK8PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None'),
)

#this is our example analysis module reading the results
process.demo = cms.EDAnalyzer("RALMiniAnalyzer",
                                       isThis2016 = cms.bool(False),
                                       isThisMC = cms.bool(False),
                                       containsLHE = cms.bool(False), # data does not have lhe info
                                       lhe = cms.InputTag("externalLHEProducer"),
                                       # ignoreTopInLheHtCalculation = cms.bool(True), 
                                       heepId = cms.InputTag("heepId"),
                                       vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                       puInfo = cms.InputTag("slimmedAddPileupInfo"),
                                       muons = cms.InputTag("slimmedMuons"),
                                       electrons = cms.InputTag("slimmedElectrons"),
                                       photons = cms.InputTag("slimmedPhotons"),
                                       tracks = cms.InputTag("packedPFCandidates"),
                                       jets = cms.InputTag("updatedPatJetsNewJEC"),
                                       fatjets = cms.InputTag("updatedPatJetsAK8NewJEC"),
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
    process.patJetCorrFactorsNewJEC*
    process.updatedPatJetsNewJEC*
    process.patJetCorrFactorsAK8NewJEC*
    process.updatedPatJetsAK8NewJEC*
    process.demo) #our analysing example module, replace with your module