import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'LISTOFFILESINSERTEDHERE'
    )
)

#heep event needs geom & b-field
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = cms.string('GLOBALTAG_INSERTEDHERE')

process.TFileService = cms.Service("TFileService", fileName=cms.string('OUTPUTFILENAME_INSERTEDHERE'))

process.demo = cms.EDAnalyzer("RALMiniAnalyzer",
    isThisMC = cms.bool(ISTHISMCORNOT_INSERTEDHERE),
    mcWeight = cms.double(MCWEIGHT_INSERTEDHERE),
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
