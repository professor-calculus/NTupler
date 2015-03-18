import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://dcap.pp.rl.ac.uk//pnfs/pp.rl.ac.uk/data/cms/store/mc/Spring14miniaod/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/7CB96282-EA1E-E411-84B7-001E67396A22.root'
#        'file:/opt/ppd/scratch/olaiya/CMS/analysis/CMSSW_7_2_0/src/PhysicsTools/PatAlgos/test/miniAOD/patTuple.root'
#lcg-ls "srm://heplnx204.pp.rl.ac.uk:8443/srm/managerv2?SFN=/pnfs/pp.rl.ac.uk/data/cms/store/mc/Spring14miniaod/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/7CB96282-EA1E-E411-84B7-001E67396A22.root"
    )
)

process.TFileService = cms.Service("TFileService", fileName=cms.string('nTuple3.root'))

process.demo = cms.EDAnalyzer("MiniAnalyzer",
    vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    muons = cms.InputTag("slimmedMuons"),
    electrons = cms.InputTag("slimmedElectrons"),
#    taus = cms.InputTag("slimmedTaus"),
#    photons = cms.InputTag("slimmedPhotons"),
    jets = cms.InputTag("slimmedJets"),
    fatjets = cms.InputTag("slimmedJetsAK8"),
    mets = cms.InputTag("slimmedMETs"),
    eleLabel = cms.InputTag("patElectrons"),
    applyRhoCorrToEleIsol = cms.bool(True), 
    eleIsolEffectiveAreas = cms.PSet (heepEffectiveAreas),
    eleRhoCorrLabel = cms.InputTag("kt6PFJetsForIsolation","rho"),
    verticesLabel = cms.InputTag("offlinePrimaryVerticesWithBS"),
)

process.load("SHarper.HEEPAnalyzer.HEEPAnalyzer_cfi")
from SHarper.HEEPAnalyzer.HEEPAnalyzer_cfi import swapHEEPToMiniAOD
swapHEEPToMiniAOD(process.heepAnalyzer)

process.p = cms.Path(process.demo)
