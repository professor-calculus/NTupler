import FWCore.ParameterSet.Config as cms

#Useful function for local running...

process = cms.Process("Demo")

## Loading Geometry modules for creation of transient geometry information used in determining eta and phi values of recHits ...
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'START42_V13::All'

from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['startup']

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
#      fileNames = cms.untracked.vstring('/store/data/Run2011B/Photon/AOD/PromptReco-v1/000/180/252/6291CDCD-0B05-E111-8CDB-003048F117EC.root')
      fileNames = cms.untracked.vstring('dcache:/pnfs/pp.rl.ac.uk/data/cms/store/user/olaiya/PatTuples/Summer12_DR53X-PU_S10_START53_V7A-v2/TTJets/patTuple_61_1_ZD5.root')                      
#                            fileNames = cms.untracked.vstring('file:patTuple.root')
)

#Defining the output file to store the histograms/NTuples in...
process.TFileService = cms.Service("TFileService", fileName=cms.string('dataNTuple-42X_v1g.root'))



from SHarper.HEEPAnalyzer.HEEPSelectionCuts_cfi import *

process.demo = cms.EDAnalyzer('PATNTupler',
                              dyJetsToLL_EventType = cms.untracked.int32(0), #==0=>Don't select events, ==11=>ele, ==13=>muon, ==15=>tau
                              isMC = cms.untracked.bool(False),
                              printOutInfo = cms.untracked.bool(False), 
                              barrelCuts = cms.PSet(heepBarrelCuts),
                              endcapCuts = cms.PSet(heepEndcapCuts),
                              applyRhoCorrToEleIsol = cms.bool(True), 
                              eleIsolEffectiveAreas = cms.PSet (heepEffectiveAreas),
                              eleRhoCorrLabel = cms.InputTag("kt6PFJetsForIsolation","rho"),
                              verticesLabel = cms.InputTag("offlinePrimaryVerticesWithBS")
)
                              


process.p = cms.Path(process.demo)
