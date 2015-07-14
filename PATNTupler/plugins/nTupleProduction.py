import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'root://dcap.pp.rl.ac.uk//pnfs/pp.rl.ac.uk/data/cms/store/mc/Spring14miniaod/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/7CB96282-EA1E-E411-84B7-001E67396A22.root'
        #'file:/opt/ppd/scratch/olaiya/CMS/analysis/CMSSW_7_2_0/src/PhysicsTools/PatAlgos/test/miniAOD/patTuple.root'
#lcg-ls "srm://heplnx204.pp.rl.ac.uk:8443/srm/managerv2?SFN=/pnfs/pp.rl.ac.uk/data/cms/store/mc/Spring14miniaod/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/7CB96282-EA1E-E411-84B7-001E67396A22.root"
        '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/6A0A8868-4B27-E511-B3F8-02163E011BD1.root',
        '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/7E66D014-D628-E511-9D1E-02163E012603.root',
        '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/84B7599F-4E27-E511-9DEC-02163E014509.root',
        '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/C47A9CF9-6227-E511-908E-02163E014509.root',
        '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/E8FEB382-3228-E511-8B4D-02163E0133E3.root'
    )
)

process.TFileService = cms.Service("TFileService", fileName=cms.string('nTuple.root'))

from SHarper.HEEPAnalyzer.HEEPSelectionCuts_cfi import *

process.demo = cms.EDAnalyzer("RALMiniAnalyzer",
    vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    muons = cms.InputTag("slimmedMuons"),
    electrons = cms.InputTag("slimmedElectrons"),
    jets = cms.InputTag("slimmedJets"),
    fatjets = cms.InputTag("slimmedJetsAK8"),
    mets = cms.InputTag("slimmedMETs"),
    eleLabel = cms.InputTag("patElectrons"),
    barrelCuts = cms.PSet(heepBarrelCuts),
    endcapCuts = cms.PSet(heepEndcapCuts),
    applyRhoCorrToEleIsol = cms.bool(True), 
    eleIsolEffectiveAreas = cms.PSet (heepEffectiveAreas),
    eleRhoCorrLabel = cms.InputTag("fixedGridRhoAll","rho"),
    verticesLabel = cms.InputTag("offlinePrimaryVerticesWithBS"),
    bits = cms.InputTag("TriggerResults","","HLT"),
    prescales = cms.InputTag("patTrigger"),
    objects = cms.InputTag("selectedPatTrigger"),
    selectedTriggerPath = cms.string("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW")
)

process.load("SHarper.HEEPAnalyzer.HEEPAnalyzer_cfi")
from SHarper.HEEPAnalyzer.HEEPAnalyzer_cfi import swapHEEPToMiniAOD
swapHEEPToMiniAOD(process.heepAnalyzer)

process.p = cms.Path(process.demo)
