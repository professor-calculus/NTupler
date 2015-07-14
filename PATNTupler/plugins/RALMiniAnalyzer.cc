// -*- C++ -*-
//
// Package:    Test/RALMiniAnalyzer
// Class:      RALMiniAnalyzer
// 
/**\class RALMiniAnalyzer RALMiniAnalyzer.cc Test/RALMiniAnalyzer/plugins/RALMiniAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Emmanuel Olatunji Olaiya
//         Created:  Wed, 07 Jan 2015 15:05:40 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

//...in order to use the heep::Ele class ...
#include "SHarper/HEEPAnalyzer/interface/HEEPEle.h"
#include "SHarper/HEEPAnalyzer/interface/HEEPEleSelector.h"


//...for histograms creation
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//ROOT HEADERS
#include "TTree.h"

//NTuple object headers
#include "NTupler/PATNTupler/interface/EventInfo.hh"
#include "NTupler/PATNTupler/interface/Particles.hh"


//
// class declaration
//

class RALMiniAnalyzer : public edm::EDAnalyzer {
   public:
      explicit RALMiniAnalyzer(const edm::ParameterSet&);
      ~RALMiniAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      //Did event pass specified trigger
      bool passedTrigger(const edm::Event&);

      ///For clearing contents/setting default values of variables that should get new values in each event...
      void ResetEventByEventVariables();

      ///For reading the event information into the member variables...
      void ReadInEvtInfo(bool, const edm::Event&);

      ///For reading in the electron information, and dumping it into ran::Event class ...
      void ReadInElectrons(const edm::Event&);
  
      ///For reading in the muon information, and dumping it into ran::Event class ...
      void ReadInMuons(const edm::Event&);

      ///For reading in the Jet information, and dumping it into ran::Event class ...
      void ReadInJets(const edm::Event&);

      ///For reading in the Fat Jet information, and dumping it into ran::Event class ...
      void ReadInFatJets(const edm::Event&);

      ///For reading in the Met information, and dumping it into ran::Event class ...
      void ReadInMets(const edm::Event&);

      

      // ----------member data ---------------------------

      edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
      edm::EDGetTokenT<pat::MuonCollection> muonToken_;
      edm::EDGetTokenT<pat::ElectronCollection> electronToken_;
      edm::EDGetTokenT<pat::JetCollection> jetToken_;
      edm::EDGetTokenT<pat::JetCollection> fatjetToken_;
      edm::EDGetTokenT<pat::METCollection> metToken_;
      edm::InputTag eleRhoCorrLabel_;      
      bool applyRhoCorrToEleIsol_;
      edm::InputTag verticesLabel_;
      edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
      edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
      edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
      std::string targetTriggerPath_;

      //Ntuple Tree
      edm::Service<TFileService> fHistos;
      TTree* EventDataTree;

      //Variables whose values will be stored as branches...
      //ran::Event* event_;
      ran::EventInfo evtInfo{};
      std::vector<ran::ElectronStruct>* electronCollection;
      std::vector<ran::MuonStruct>* muonCollection;
      std::vector<ran::JetStruct>* jetCollection;
      std::vector<ran::FatJetStruct>* fatjetCollection;
      std::vector<ran::MetStruct>* metCollection;
      bool vBool_;
           
      heep::EleSelector cuts_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
RALMiniAnalyzer::RALMiniAnalyzer(const edm::ParameterSet& iConfig):
   //now do what ever initialization is needed
    vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
    muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
    electronToken_(consumes<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
    jetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
    fatjetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("fatjets"))),
    metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets"))),
    eleRhoCorrLabel_(iConfig.getParameter<edm::InputTag>("eleRhoCorrLabel")),
    applyRhoCorrToEleIsol_(iConfig.getParameter<bool>("applyRhoCorrToEleIsol")),
    verticesLabel_(iConfig.getParameter<edm::InputTag>("verticesLabel")),
    triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
    triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"))),
    triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"))),
    targetTriggerPath_(iConfig.getParameter<std::string>("selectedTriggerPath"))
    //cuts_(iConfig)
{
    EventDataTree = fHistos->make<TTree>("EventDataTree", "Event data tree");

    //Setting up the links between variables and branches...
    //event_ = 0;	
    //EventDataTree->Branch("event","ran::Event", &event_, 64000, 1); // This line was taken from Jim's tupiliser
    EventDataTree->Branch("evtInfo","ran::EventInfo",&evtInfo);
    EventDataTree->Branch("electronCollection","std::vector<ran::ElectronStruct>", &electronCollection, 64000, 1); 
    EventDataTree->Branch("muonCollection","std::vector<ran::MuonStruct>", &muonCollection, 64000, 1); 
    EventDataTree->Branch("jetCollection","std::vector<ran::JetStruct>", &jetCollection, 64000, 1);
    EventDataTree->Branch("fatjetCollection","std::vector<ran::FatJetStruct>", &fatjetCollection, 64000, 1);
    EventDataTree->Branch("metCollection","std::vector<ran::MetStruct>", &metCollection, 64000, 1);
}


RALMiniAnalyzer::~RALMiniAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
RALMiniAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   //Does event pass any of our specified triggers    
   bool triggerOfInterest = passedTrigger(iEvent);

   if (triggerOfInterest){

     electronCollection = new std::vector<ran::ElectronStruct>();
     muonCollection = new std::vector<ran::MuonStruct>();
     jetCollection = new std::vector<ran::JetStruct>();
     fatjetCollection = new std::vector<ran::FatJetStruct>();
     metCollection = new std::vector<ran::MetStruct>();

     //Clearing contents/setting default values of variables that should get new values in each event...
     ResetEventByEventVariables();

     //Reading in the event information...
     ReadInEvtInfo(vBool_, iEvent);     

     //Read in muons
     ReadInMuons(iEvent);
      
     //Read in Electrons
     ReadInElectrons(iEvent);

     //Read in Jets
     ReadInJets(iEvent);

     //Read in Jets
     ReadInFatJets(iEvent);

     //Read in Met
     //ReadInMets(iEvent); //See a problem reading in met. Need to fix it!

     //Fill Ntuple
     EventDataTree->Fill();	

     //delete event_;
     delete electronCollection;
     delete muonCollection;
     delete fatjetCollection;
     delete jetCollection;
     delete metCollection;

   }//Passed Trigger? 

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
RALMiniAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RALMiniAnalyzer::endJob() 
{

  //Write out Ntuple
  	std::cout << "   done." << std::endl;
	
	
	fHistos->cd();
	EventDataTree->Write();

	std::cout << std::endl;
}

// ------------ method called when starting to processes a run  ------------
/*
void 
RALMiniAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
RALMiniAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
RALMiniAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
RALMiniAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RALMiniAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RALMiniAnalyzer);

//------------ method for clearing contents/setting default values of variables that should get new values in each event -------------
void RALMiniAnalyzer::ResetEventByEventVariables(){
	
	//Resetting the values of the event information variables...
  	evtInfo.runNum = 0;
	evtInfo.evtNum = 0;
	evtInfo.lumiSec = 0;
	
  	
	
}

//-------------------------------------------------------------------------------------------------
//------------method for selecting events that pass specified triggers-----------------------------

bool RALMiniAnalyzer::passedTrigger(const edm::Event& iEvent){

  edm::Handle<edm::TriggerResults> triggerBits;
  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;

  iEvent.getByToken(triggerBits_, triggerBits);
  iEvent.getByToken(triggerObjects_, triggerObjects);
  iEvent.getByToken(triggerPrescales_, triggerPrescales);

  const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);

  //Look for a particular Trigger pass
  //Should pass the path via config as a vector of strings

  bool trigPass(false);
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
    std::string triggerPath =  names.triggerName(i);
    if (triggerBits->accept(i) && (triggerPath.compare(0,targetTriggerPath_.size(),targetTriggerPath_) == 0)){
      //string compare method compare(pos of first char to be compared, size of string to compare, string to compare )
      trigPass =  true;
    }
  }

  return trigPass;

}

//----------------------------------------------------------------------------------------------------
//------------ method for reading in the event information (run no., lumi sec etc...)  ---------------
void RALMiniAnalyzer::ReadInEvtInfo(bool beVerbose, const edm::Event& edmEventObject){

      evtInfo.runNum = edmEventObject.id().run();
      evtInfo.lumiSec = edmEventObject.id().luminosityBlock();
      evtInfo.evtNum = edmEventObject.id().event(); 
}

void RALMiniAnalyzer::ReadInElectrons(const edm::Event& iEvent)
{

  /* edm::Handle<double> rhoHandle;
  iEvent.getByLabel(eleRhoCorrLabel_,rhoHandle);
  double rho = applyRhoCorrToEleIsol_ ? *rhoHandle : 0;

  edm::Handle<reco::VertexCollection> verticesHandle;
  iEvent.getByLabel(verticesLabel_,verticesHandle);
  math::XYZPoint pvPos(0,0,0);
  if(!verticesHandle->empty()) pvPos = verticesHandle->front().position();*/

  edm::Handle<pat::ElectronCollection> electrons;
  iEvent.getByToken(electronToken_, electrons);
  for (const pat::Electron &iEle : *electrons) {
    electronCollection->push_back(ran::ElectronStruct{});
    ran::ElectronStruct &ithElec = electronCollection->back();
     
    ithElec.pt = iEle.pt();
    ithElec.eta = iEle.eta();

    ithElec.gsfTrack_available = iEle.gsfTrack().isAvailable();
    ithElec.scEta = iEle.superCluster()->eta();
    ithElec.scEnergy = iEle.superCluster()->energy(); 
    ithElec.ecalDrivenSeed = iEle.ecalDrivenSeed(); 
    ithElec.sigmaIetaIeta = iEle.sigmaIetaIeta();
    ithElec.full5x5_sigmaIetaIeta = iEle.full5x5_sigmaIetaIeta();
    ithElec.passConversionVeto = iEle.passConversionVeto();
    ithElec.e2x5Max = iEle.e2x5Max();
    ithElec.e5x5 = iEle.e5x5();
    ithElec.e1x5 = iEle.e1x5();
    ithElec.deltaPhiSuperClusterTrackAtVtx = iEle.deltaPhiSuperClusterTrackAtVtx();
    ithElec.hadronicOverEm = iEle.hadronicOverEm();
    //ithElec.nrMissHits = iEle.gsfTrack().trackerExpectedHitsInner().numberOfHits();
    ithElec.scSigmaIEtaIEta = iEle.scSigmaIEtaIEta();
    ithElec.dr03EcalRecHitSumEt = iEle.dr03EcalRecHitSumEt();
    ithElec.dr03HcalDepth1TowerSumEt = iEle.dr03HcalDepth1TowerSumEt();
    ithElec.dr03TkSumPt = iEle.dr03TkSumPt();
    //ithElec.pfIso_chgHad = iEle.pfIsolationVariables().chargedHadronIso;
    //ithElec.pfIso_neutHad = iEle.pfIsolationVariables().neutralHadronIso;
    //ithElec.pfIso_pht = iEle.pfIsolationVariables().photonIso;
    //electronCollection->push_back(ithElec);

    heep::Ele heepEle(iEle);//instantiate HEEP variable
    //HEEP variables
    ithElec.heep_isPatEle = heepEle.isPatEle();
    ithElec.heep_et = heepEle.et();
    ithElec.heep_gsfEt =    heepEle.gsfEt();
    ithElec.heep_scEt   =   heepEle.scEt();
    ithElec.heep_energy  =  heepEle.energy();
    ithElec.heep_gsfEnergy = heepEle.gsfEnergy();
    ithElec.heep_caloEnergy= heepEle.caloEnergy();
    ithElec.heep_ecalEnergyError = heepEle.gsfEle().ecalEnergyError();
    ithElec.heep_eta     =  heepEle.eta();
    ithElec.heep_scEta =      heepEle.scEta();
    ithElec.heep_detEta =     heepEle.detEta();
    ithElec.heep_detEtaAbs =  heepEle.detEtaAbs();
    ithElec.heep_phi =        heepEle.phi();
    ithElec.heep_scPhi =      heepEle.scPhi();
    ithElec.heep_detPhi =     heepEle.detPhi();
    ithElec.heep_zVtx =       heepEle.zVtx();
    ithElec.heep_p4 =         heepEle.p4();
    ithElec.heep_gsfP4 =      heepEle.gsfP4();

    //Variables storing the heep::Ele method values - Classification...
    ithElec.heep_classification =  heepEle.classification();
    ithElec.heep_isEcalDriven =    heepEle.isEcalDriven();
    ithElec.heep_isTrackerDriven = heepEle.isTrackerDriven();
    ithElec.heep_isEB =            heepEle.isEB();
    ithElec.heep_isEE =            heepEle.isEE();

    //Variables storing the heep::Ele method values - track methods ...
    ithElec.heep_charge =    heepEle.charge();
    ithElec.heep_trkCharge = heepEle.trkCharge();
    ithElec.heep_pVtx =      heepEle.pVtx();
    ithElec.heep_pCalo =     heepEle.pCalo();
    ithElec.heep_ptVtx =     heepEle.ptVtx();
    ithElec.heep_ptCalo =    heepEle.ptCalo();    

    //Variables storing the heep::Ele method values ...
    ithElec.heep_hOverE =      heepEle.hOverE();
    ithElec.heep_dEtaIn =      heepEle.dEtaIn();
    ithElec.heep_dPhiIn =      heepEle.dPhiIn();
    ithElec.heep_dPhiOut =     heepEle.dPhiOut();
    ithElec.heep_epIn =        heepEle.epIn();
    ithElec.heep_epOut =       heepEle.epOut();
    ithElec.heep_fbrem =       heepEle.fbrem();
    ithElec.heep_bremFrac =    heepEle.bremFrac();
    ithElec.heep_invEOverInvP = heepEle.invEOverInvP();

    //Variables storing the heep::Ele method values - shower shape variables ...
    ithElec.heep_sigmaEtaEtaFull5x5 =      heepEle.sigmaEtaEtaFull5x5();
    ithElec.heep_sigmaEtaEtaUnCorrFull5x5 = heepEle.sigmaEtaEtaUnCorrFull5x5();
    ithElec.heep_sigmaIEtaIEtaFull5x5 =    heepEle.sigmaIEtaIEtaFull5x5();
    ithElec.heep_e1x5Full5x5 =             heepEle.e1x5Full5x5();
    ithElec.heep_e2x5MaxFull5x5 =          heepEle.e2x5MaxFull5x5();
    ithElec.heep_e5x5Full5x5 =             heepEle.e5x5Full5x5();
    ithElec.heep_e1x5Over5x5Full5x5 =      heepEle.e1x5Over5x5Full5x5();
    ithElec.heep_e2x5MaxOver5x5Full5x5 =   heepEle.e2x5MaxOver5x5Full5x5();
    

    //isolation, we use cone of 0.3
    //first our rho correction funcs
    ithElec.heep_rhoForIsolCorr = heepEle.rhoForIsolCorr();
    ithElec.heep_applyRhoIsolCorr = heepEle.applyRhoIsolCorr();

    //Variables storing the heep::Ele method values - isolation variables ...
    ithElec.heep_isolEm =         heepEle.isolEm();
    ithElec.heep_isolHad =        heepEle.isolHad();
    ithElec.heep_isolHadDepth1 =  heepEle.isolHadDepth1();
    ithElec.heep_isolHadDepth2 =  heepEle.isolHadDepth2();
    ithElec.heep_isolPtTrks =     heepEle.isolPtTrks();
    ithElec.heep_isolEmHadDepth1 = heepEle.isolEmHadDepth1();

    ithElec.heep_isolPtTrksRel03 = heepEle.isolPtTrksRel03();
    ithElec.heep_isolEmRel03 = heepEle.isolEmRel03();
    ithElec.heep_isolHadRel03 = heepEle.isolHadRel03();

    ithElec.heep_dxy = heepEle.dxy();

    ithElec.heep_numMissInnerHits = heepEle.nrMissHits();

    //ithElec.heep_cutCode = cuts_.getCutCode(rho,pvPos,iEle);
    ithElec.heep_cutCode = cuts_.getCutCode(iEle);


  }
}
//Read in jet vars
void RALMiniAnalyzer::ReadInJets(const edm::Event& iEvent)
{
    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(jetToken_, jets);
    for (const pat::Jet &iJet: *jets) {
      jetCollection->push_back(ran::JetStruct{});

      ran::JetStruct &ithJet = jetCollection->back();
      ithJet.pt = iJet.pt();
      ithJet.et = iJet.et();
      ithJet.eta = iJet.eta();
      ithJet.phi = iJet.phi();
      ithJet.mass = iJet.mass();

      ithJet.jecFactor_unCorrected = iJet.jecFactor("Uncorrected");
      ithJet.userFloat_pileupJetId_fullDiscriminant = iJet.userFloat("pileupJetId:fullDiscriminant");

      //Assign the btag discriminators
      ithJet.jetProbabilityBJetTags =  iJet.bDiscriminator("jetProbabilityBJetTags");
      ithJet.jetBProbabilityBJetTags =  iJet.bDiscriminator("jetBProbabilityBJetTags");
      ithJet.trackCountingHighEffBJetTags =  iJet.bDiscriminator("trackCountingHighEffBJetTags");   
      ithJet.trackCountingHighPurBJetTags =  iJet.bDiscriminator("trackCountingHighPurBJetTags");
      ithJet.simpleSecondaryVertexHighEffBJetTags =  iJet.bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
      ithJet.simpleSecondaryVertexHighPurBJetTags =  iJet.bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
      ithJet.combinedInclusiveSecondaryVertexBJetTags =  iJet.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags");


      ithJet.partonFlavour = iJet.partonFlavour();

    }

}

//Read in fat jet vars
void RALMiniAnalyzer::ReadInFatJets(const edm::Event& iEvent)
{
    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(fatjetToken_, jets);
    for (const pat::Jet &iJet: *jets) {
      fatjetCollection->push_back(ran::FatJetStruct{});

      ran::FatJetStruct &ithJet = fatjetCollection->back();
      ithJet.pt = iJet.pt();
      ithJet.et = iJet.et();
      ithJet.eta = iJet.eta();
      ithJet.phi = iJet.phi();
      ithJet.mass = iJet.mass();

      ithJet.jecFactor_unCorrected = iJet.jecFactor("Uncorrected");
      ithJet.userFloat_ak8PFJets_CHSPrunedLinks = iJet.userFloat("ak8PFJetsCHSPrunedLinks");
      ithJet.userFloat_ak8PFJets_CHSTrimmedLinks = iJet.userFloat("ak8PFJetsCHSTrimmedLinks");
      ithJet.userFloat_ak8PFJets_CHSFilteredLinks = iJet.userFloat("ak8PFJetsCHSFilteredLinks");
      ithJet.userFloat_cmsTopTag_PFJets_CHSLinksAK8 = iJet.userFloat("cmsTopTagPFJetsCHSLinksAK8");

      //Assign the btag discriminators
      ithJet.jetProbabilityBJetTags =  iJet.bDiscriminator("jetProbabilityBJetTags");
      ithJet.jetBProbabilityBJetTags =  iJet.bDiscriminator("jetBProbabilityBJetTags");
      ithJet.trackCountingHighEffBJetTags =  iJet.bDiscriminator("trackCountingHighEffBJetTags");   
      ithJet.trackCountingHighPurBJetTags =  iJet.bDiscriminator("trackCountingHighPurBJetTags");
      ithJet.simpleSecondaryVertexHighEffBJetTags =  iJet.bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
      ithJet.simpleSecondaryVertexHighPurBJetTags =  iJet.bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
      ithJet.combinedInclusiveSecondaryVertexBJetTags =  iJet.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags");
      ithJet.partonFlavour = iJet.partonFlavour();

    }

}

void RALMiniAnalyzer::ReadInMets(const edm::Event& iEvent)
{
    edm::Handle<pat::METCollection> mets;
    iEvent.getByToken(metToken_, mets);
    for (const pat::MET &iMet: *mets) {
      metCollection->push_back(ran::MetStruct{});

      ran::MetStruct &ithMet = metCollection->back();
      ithMet.pt = iMet.pt();
      ithMet.eta = iMet.eta();
      ithMet.phi = iMet.phi();
      ithMet.sumEt = iMet.sumEt();
      ithMet.genMet_pt = iMet.genMET()->pt();
      //ithMet.metSignificance =  iMet.metSignificance();
      //ithMet.etFractionHadronic = iMet.etFractionHadronic();
      ithMet.isCaloMET = iMet.isCaloMET();
      ithMet.isPFMET = iMet.isPFMET();
      ithMet.isRecoMET = iMet.isRecoMET();
      ithMet.shiftedPt_JetEnUp = iMet.shiftedPt(pat::MET::JetEnUp);
      ithMet.shiftedPt_JetEnDown = iMet.shiftedPt(pat::MET::JetEnDown);   
    }

}

//------------ method for reading in the values of the standard GSF electron variables ---------------
//----------------------------------------------------------------------------------------------------
//-------- Method for reading in the muon information, and dumping it into tsw::Event class ----------
void RALMiniAnalyzer::ReadInMuons(const edm::Event& iEvent){

    edm::Handle<reco::VertexCollection> vertices;
    iEvent.getByToken(vtxToken_, vertices);
    if (vertices->empty()) return; // skip the event if no PV found
    const reco::Vertex &pV = vertices->front();

    edm::Handle<pat::MuonCollection> muons;
    iEvent.getByToken(muonToken_, muons);

    for (const pat::Muon &imuon : *muons) {
      
      muonCollection->push_back(ran::MuonStruct{}); 
      ran::MuonStruct &aMuon = muonCollection->back(); 

      aMuon.pt = imuon.pt();
      aMuon.eta = imuon.eta();
      aMuon.p4                 = imuon.p4();
      aMuon.charge             = imuon.charge();
      aMuon.isGlobalMuon       = imuon.isGlobalMuon();
      aMuon.isTrackerMuon      = imuon.isTrackerMuon();
      aMuon.isStandAloneMuon   = imuon.isStandAloneMuon();
      aMuon.numMatchedMuonStns = imuon.numberOfMatchedStations();
   
      aMuon.dB                 = imuon.dB();

      aMuon.isPFIsolationValid = imuon.isPFIsolationValid();
      aMuon.pfIsoR03_sumChgHadPt  = imuon.pfIsolationR03().sumChargedHadronPt;
      aMuon.pfIsoR03_chgPartPt = imuon.pfIsolationR03().sumChargedParticlePt;
      aMuon.pfIsoR03_sumNeutHadPt =  imuon.pfIsolationR03().sumNeutralHadronEt;
      aMuon.pfIsoR03_sumPhtEt =  imuon.pfIsolationR03().sumPhotonEt;
      aMuon.isIsolationValid   = imuon.isIsolationValid();
      aMuon.isolR03_sumPt      = imuon.isolationR03().sumPt;
      aMuon.isolR03_emEt       = imuon.isolationR03().emEt;
      aMuon.isolR03_hadEt      = imuon.isolationR03().hadEt;

      // Global track information ...
      if(imuon.globalTrack().get()!=0){
	aMuon.globTrk_exists = true;
	aMuon.globTrk_pT            = imuon.globalTrack()->pt();
	aMuon.globTrk_eta           = imuon.globalTrack()->eta();
	aMuon.globTrk_phi           = imuon.globalTrack()->phi();
	aMuon.globTrk_charge        = imuon.globalTrack()->charge();
	aMuon.globTrk_numberOfValidMuonHits = imuon.globalTrack()->hitPattern().numberOfValidMuonHits();
	aMuon.globTrk_normalisedChi2 = imuon.globalTrack()->normalizedChi2();
      } else {
	aMuon.globTrk_exists = false;
	aMuon.globTrk_pT    = -999.9;       
	aMuon.globTrk_eta  = -999.9;        
	aMuon.globTrk_phi  = -999.9;         
	aMuon.globTrk_charge  = -999.9;      
	aMuon.globTrk_numberOfValidMuonHits = -999.9;
	aMuon.globTrk_normalisedChi2 = -999.9;
      }

      // Inner track information ...
      if(imuon.innerTrack().get()!=0){
	aMuon.inTrk_exists = true;
	aMuon.inTrk_pT               = imuon.innerTrack()->pt();
	aMuon.inTrk_eta              = imuon.innerTrack()->eta();
	aMuon.inTrk_phi              = imuon.innerTrack()->phi();
	aMuon.inTrk_charge           = imuon.innerTrack()->charge();
	aMuon.inTrk_numValidPixHits  = imuon.innerTrack()->hitPattern().numberOfValidPixelHits();
	aMuon.inTrk_numValidTrkrHits = imuon.innerTrack()->hitPattern().numberOfValidTrackerHits();
	aMuon.inTrk_trackerLayersWithMeasurement =  imuon.innerTrack()->hitPattern().trackerLayersWithMeasurement();
	aMuon.inTrk_dxyVsOrigin      = imuon.innerTrack()->dxy(); // Really, this should be calculated as innerTrack()->dxy(vertex->position()), but no vertex information is read in at the moment, and MuonRecoPerformance2010 TWiki page => Can calculate this approximately just relative to (0,0,0)
	aMuon.trk_trkrLayersWHits    = imuon.track()->hitPattern().trackerLayersWithMeasurement();
      } else {
	aMuon.inTrk_exists = false;
	aMuon.inTrk_pT               = -999.9;
	aMuon.inTrk_eta              = -999.9;
	aMuon.inTrk_phi              = -999.9;
	aMuon.inTrk_charge           = -999.9;
	aMuon.inTrk_numValidPixHits  = -999.9;
	aMuon.inTrk_numValidTrkrHits = -999.9;
	aMuon.inTrk_trackerLayersWithMeasurement = -999.9; 
	aMuon.inTrk_dxyVsOrigin      = -999.9;
      }

      // Outer track information ...
      if(imuon.outerTrack().get()!=0){
	aMuon.outTrk_exists = true;
	aMuon.outTrk_pT     = imuon.outerTrack()->pt();
	aMuon.outTrk_eta    = imuon.outerTrack()->eta();
	aMuon.outTrk_phi    = imuon.outerTrack()->phi();
	aMuon.outTrk_charge = imuon.outerTrack()->charge();
      } else {
	aMuon.outTrk_exists = false;
	aMuon.outTrk_pT     = -999.9;
	aMuon.outTrk_eta    = -999.9;
	aMuon.outTrk_phi    = -999.9;
	aMuon.outTrk_charge = 999;
      }

          
      if( imuon.muonBestTrack().get()!=0 ){
	aMuon.bestTrk_exists = true;
	aMuon.bestTrk_dxy_bspot   = -999.9; // NEED TO ADD THE CORRECT VERTEX imuon.muonBestTrack()->dxy( h_beamSpot->position() );
	aMuon.bestTrk_dxy_vtx     = -999.9;//NEED TO ADD THE CORRECT VERTEX imuon.muonBestTrack()->dxy( mainPrimaryVertexIt->position() );
	aMuon.bestTrk_dz_vtx      = -999.9;//NEED TO ADD THE CORRECT VERTEX imuon.muonBestTrack()->dz( mainPrimaryVertexIt->position() );
	aMuon.dxy                = imuon.muonBestTrack()->dxy(pV.position());
	aMuon.dz                 = imuon.muonBestTrack()->dz(pV.position());

	//need to remove pt tune members from muon object
      } else {
	aMuon.bestTrk_exists = false;
	aMuon.bestTrk_dxy_bspot   = -999.9;
	aMuon.bestTrk_dxy_vtx     = -999.9;
	aMuon.bestTrk_dz_vtx      = -999.9;
	aMuon.dxy                = -999.9;
	aMuon.dz                 = -999.9;

      }
      //add muon to Muon collection
      muonCollection->push_back(aMuon);
      //unessesary  copy here should fill inside the vector. fill a reference to muonCollection->back() or something
      // or assign to the pointer obtained from muonCollection->back()

    }

}
