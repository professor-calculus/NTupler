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
    limit = cms.untracked.int32(1500)
)
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')

process.TFileService = cms.Service("TFileService", fileName=cms.string('nTuple.root'))

#setup global tag
from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_miniAODv2_v1', '') #


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )
process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring(
        'root://dcap.pp.rl.ac.uk:1094/pnfs/pp.rl.ac.uk/data/cms/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root'
)
                                       
)
useMiniAOD=True

if useMiniAOD==True:
    # process.source.fileNames=cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root',)
    process.source.fileNames=cms.untracked.vstring('/store/user/taylor/nmssmSignalCascadeV05_13TeV_mH90p0_mSusy2000p0_ratio0p99_splitting0p1/nmssmSignalCascadeV05_13TeV_processMc04_ed8021v1_mH90p0_mSusy2000p0_ratio0p99_splitting0p1/170321_112712/0000/nmssmSignal_MINIAODSIM_1.root',)

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
                                       isThisMC = cms.bool(True),
                                       #mcWeight = cms.double(MCWEIGHT_INSERTEDHERE),
                                       containsLHE = cms.bool(True),
                                       # containsLHE = cms.bool(False), # use for personal MC samples that do not have the correct lhe info format
                                       lhe = cms.InputTag("externalLHEProducer"),
                                       ignoreTopInLheHtCalculation = cms.bool(True),
                                       heepId = cms.InputTag("heepId"),
                                       vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                       muons = cms.InputTag("slimmedMuons"),
                                       electrons = cms.InputTag("slimmedElectrons"),
                                       jets = cms.InputTag("slimmedJets"),
                                       fatjets = cms.InputTag("slimmedJetsAK8"),
                                       mets = cms.InputTag("slimmedMETs"),       
                                       bits = cms.InputTag("TriggerResults","","HLT"),
                                       prescales = cms.InputTag("patTrigger"),
                                       objects = cms.InputTag("selectedPatTrigger"),
                                       selectedTriggerPaths = cms.vstring("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v","HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v","HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v","HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v2","HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v2"),#matches triggers that *contain* the stated names, so finish with _v to make sure your trigger name isn't a subset of others
                                       elesAOD=cms.InputTag("gedGsfElectrons"),
                                       elesMiniAOD=cms.InputTag("slimmedElectrons"),
                                       trkIsolMap=cms.InputTag("heepIDVarValueMaps","eleTrkPtIso"),
                                       vid=cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV70")
                                       )

process.p = cms.Path(
    process.egmGsfElectronIDSequence* 
    process.demo) #our analysing example module, replace with your module

