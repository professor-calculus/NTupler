import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'root://dcap.pp.rl.ac.uk//pnfs/pp.rl.ac.uk/data/cms/store/mc/Spring14miniaod/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/7CB96282-EA1E-E411-84B7-001E67396A22.root'
        #'file:/opt/ppd/scratch/olaiya/CMS/analysis/CMSSW_7_2_0/src/PhysicsTools/PatAlgos/test/miniAOD/patTuple.root'
#lcg-ls "srm://heplnx204.pp.rl.ac.uk:8443/srm/managerv2?SFN=/pnfs/pp.rl.ac.uk/data/cms/store/mc/Spring14miniaod/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/7CB96282-EA1E-E411-84B7-001E67396A22.root"
        '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/84B7599F-4E27-E511-9DEC-02163E014509.root'
    )
)

#heep event needs geom & b-field
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = cms.string('MCRUN2_74_V9A')

process.TFileService = cms.Service("TFileService", fileName=cms.string('nTuple.root'))

#from SHarper.HEEPAnalyzer.HEEPSelectionCuts_cfi import *

process.demo = cms.EDAnalyzer("RALMiniAnalyzer",
    isThisMC = cms.bool(False),
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
    selectedTriggerPaths = cms.vstring("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v","HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v","HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v")#matches triggers that *contain* the stated names, so finish with _v to make sure your trigger name isn't a subset of others
)

#setting up the producer to make the HEEP ID value map
from SHarper.HEEPAnalyzer.HEEPSelectionCuts_cfi import *
process.heepId = cms.EDProducer("HEEPIdValueMapProducer",
                                eleLabel = cms.InputTag("gedGsfElectrons"),
                                barrelCuts = cms.PSet(heepBarrelCuts),
                                endcapCuts = cms.PSet(heepEndcapCuts),
                                eleIsolEffectiveAreas = cms.PSet(heepEffectiveAreas),
                                eleRhoCorrLabel = cms.InputTag("fixedGridRhoFastjetAll"),
                                applyRhoCorrToEleIsol = cms.bool(True),
                                verticesLabel = cms.InputTag("offlinePrimaryVertices"),
                                writeIdAsInt =cms.bool(True) #true saves the heep ID as an int, false: saves as a float, user request
                                )
   
process.heepId.barrelCuts.minEt=cms.double(35.)
process.heepId.endcapCuts.minEt=cms.double(35.)

useMiniAOD=True
if useMiniAOD==True:
    process.heepId.verticesLabel=cms.InputTag("offlineSlimmedPrimaryVertices")
    process.heepId.eleLabel=cms.InputTag("slimmedElectrons")


#process.load("SHarper.HEEPAnalyzer.HEEPAnalyzer_cfi")
#from SHarper.HEEPAnalyzer.HEEPAnalyzer_cfi import swapHEEPToMiniAOD
#swapHEEPToMiniAOD(process.heepAnalyzer)

process.p = cms.Path(process.heepId*process.demo)
