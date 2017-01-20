// -*- C++ -*-
//
// Package:    plugin/RALMiniAnalyzer.cc
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
// TODOs: USE Unique pointers intead of bare pointers


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "SHarper/HEEPAnalyzer/interface/HEEPCutCodes.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


#include "TH1D.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//...in order to use the heep::Ele class ...
#include "SHarper/HEEPAnalyzer/interface/HEEPEle.h"
#include "SHarper/HEEPAnalyzer/interface/HEEPEleSelector.h"
#include "SHarper/HEEPAnalyzer/interface/HEEPEvent.h"

//...for histograms creation
#include "FWCore/ServiceRegistry/interface/Service.h"

//ROOT HEADERS
#include "TTree.h"

//NTuple object headers
#include "NTupler/PATNTupler/interface/EventInfo.hh"
#include "NTupler/PATNTupler/interface/Particles.hh"
#include "NTupler/PATNTupler/interface/TriggerPathToIndex.hh"


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

      bool isMC_;
      edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
      edm::EDGetTokenT<pat::MuonCollection> muonToken_;
      edm::EDGetTokenT<edm::View<reco::GsfElectron> > electronToken_;
  //edm::EDGetTokenT<edm::ValueMap<int> > heepIdToken_;
      edm::EDGetTokenT<pat::JetCollection> jetToken_;
      edm::EDGetTokenT<pat::JetCollection> fatjetToken_;
      edm::EDGetTokenT<pat::METCollection> metToken_;
  edm::EDGetTokenT<edm::View<reco::GsfElectron> > eleAODToken_;//delete this
  edm::EDGetTokenT<edm::View<reco::GsfElectron> > eleMiniAODToken_;//delete this
      edm::EDGetTokenT<edm::ValueMap<bool> > vidToken_; //VID is versioned ID, is the standard E/gamma ID producer which we have configured for HEEP
      edm::EDGetTokenT<edm::ValueMap<float> > trkIsolMapToken_;

      edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
      edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
      edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
      std::vector<std::string> targetTriggerPaths_;

      //Ntuple Tree
      edm::Service<TFileService> fHistos;
      TTree* EventDataTree;
      TTree* TriggerPathsTree;

      //Variables whose values will be stored as branches...
      //ran::Event* event_;
      ran::EventInfo evtInfo{};
      std::vector<ran::ElectronStruct>* electronCollection_;
      std::vector<ran::MuonStruct>* muonCollection_;
      std::vector<ran::JetStruct>* jetCollection_;
      std::vector<ran::FatJetStruct>* fatjetCollection_;
      std::vector<ran::MetStruct>* metCollection_;
      std::vector<string>* triggerPaths_;
      std::vector<unsigned int>* recordedTriggers_;
      bool vBool_;
           
      ran::TriggerPathToIndex* hltTriggers_;

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
    isMC_(iConfig.getParameter<bool>("isThisMC")),
    vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
    muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
    electronToken_(consumes<edm::View<reco::GsfElectron> >(iConfig.getParameter<edm::InputTag>("electrons"))),
    jetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
    fatjetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("fatjets"))),
    metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets"))),
    vidToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("vid"))),
    trkIsolMapToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("trkIsolMap"))),
    //eleRhoCorrLabel_(iConfig.getParameter<edm::InputTag>("eleRhoCorrLabel")),
    //applyRhoCorrToEleIsol_(iConfig.getParameter<bool>("applyRhoCorrToEleIsol")),
    //verticesLabel_(iConfig.getParameter<edm::InputTag>("verticesLabel")),
    triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
    triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"))),
    triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"))),
    targetTriggerPaths_(iConfig.getParameter<std::vector<std::string> >("selectedTriggerPaths"))
    //cuts_(iConfig)
{

    EventDataTree = fHistos->make<TTree>("EventDataTree", "Event data tree");
    //EventDataTree = new TTree("EventDataTree","EventDataTree");
    //Setting up the links between variables and branches...
    //event_ = 0;	
    //EventDataTree->Branch("event","ran::Event", &event_, 64000, 1); // This line was taken from Jim's tupiliser
    EventDataTree->Branch("evtInfo","ran::EventInfo",&evtInfo);
    EventDataTree->Branch("electronCollection","std::vector<ran::ElectronStruct>", &electronCollection_, 64000, 1); 
    EventDataTree->Branch("muonCollection","std::vector<ran::MuonStruct>", &muonCollection_, 64000, 1); 
    EventDataTree->Branch("jetCollection","std::vector<ran::JetStruct>", &jetCollection_, 64000, 1);
    EventDataTree->Branch("fatjetCollection","std::vector<ran::FatJetStruct>", &fatjetCollection_, 64000, 1);
    EventDataTree->Branch("metCollection","std::vector<ran::MetStruct>", &metCollection_, 64000, 1);
    EventDataTree->Branch("recordedTriggers", &recordedTriggers_);
    //EventDataTree->Branch("recordedTriggers","std::vector<char>", &recordedTriggers_, 64000, 1);

    //Seperate tree to store trigger names
    TriggerPathsTree = fHistos->make<TTree>("TriggerPathsTree", "Trigger Paths tree");
    //TriggerPathsTree = new TTree("TrigTree","EventDataTree");
    TriggerPathsTree->Branch("triggerPaths", &triggerPaths_);


    //Set Trigger paths
    hltTriggers_ = new ran::TriggerPathToIndex(targetTriggerPaths_);

    triggerPaths_ = new std::vector<std::string>();
    triggerPaths_->insert(triggerPaths_->begin(),targetTriggerPaths_.begin(), targetTriggerPaths_.end());
}


RALMiniAnalyzer::~RALMiniAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  delete hltTriggers_;
  //should delete triggerPaths_;
}


//
// member functions
//

// ------------ method called for each event  ------------
void
RALMiniAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   //instantiate vector to store trigger pass or fails
   recordedTriggers_ = new std::vector<unsigned int>(hltTriggers_->size(), 0);
   //Does event pass any of our specified triggers 
   bool triggerOfInterest = passedTrigger(iEvent);
   //triggerOfInterest = (isMC_) ? isMC_ : passedTrigger(iEvent);//Need to write trigger info for MC but not select events based on it

   //for (unsigned int i = 0; i < recordedTriggers_->size(); ++i){
   //  std::cout << "Trigger i (int): " << int(recordedTriggers_->at(i)) << " \n";
   //}

   //if (triggerOfInterest || isMC_){
     electronCollection_ = new std::vector<ran::ElectronStruct>();
     muonCollection_ = new std::vector<ran::MuonStruct>();
     jetCollection_ = new std::vector<ran::JetStruct>();
     fatjetCollection_ = new std::vector<ran::FatJetStruct>();
     metCollection_ = new std::vector<ran::MetStruct>();
    
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
     ReadInMets(iEvent);

     //Fill Ntuple
     EventDataTree->Fill();	

     //delete event_;
     delete electronCollection_;
     delete muonCollection_;
     delete fatjetCollection_;
     delete jetCollection_;
     delete metCollection_;

     delete recordedTriggers_;

     //}//Passed Trigger? 

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
  //Only write once at the start of a job
  TriggerPathsTree->Fill();

}

// ------------ method called once each job just after ending the event loop  ------------
void 
RALMiniAnalyzer::endJob() 
{

  //Write out Ntuple
  	std::cout << "   done." << std::endl;
	
	
	fHistos->cd();
	EventDataTree->Write();
	TriggerPathsTree->Write();

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
    //std::cout << "TRIGGER THAT PASSED IS: " << triggerPath << "\n";
    //if (triggerBits->accept(i)) std::cout << "TRIGGER THAT PASSED IS: " << triggerPath << "and Prescale: " << triggerPrescales->getPrescaleForIndex(i) << "\n";
    for (std::vector<std::string>::const_iterator triggerPath_Iter = targetTriggerPaths_.begin(); 
	 triggerPath_Iter != targetTriggerPaths_.end(); ++triggerPath_Iter){// loop over the triggers I have specified via the config
      if (triggerBits->accept(i) && (triggerPath.compare(0,triggerPath_Iter->size(),*triggerPath_Iter) == 0)){
      //string compare method compare(pos of first char to be compared, size of string to compare, string to compare )       
	recordedTriggers_->at(hltTriggers_->getTrigIndex(*triggerPath_Iter)) = triggerPrescales->getPrescaleForIndex(i);//non zero int means passed, zero is failed
	//std::cout <<"FOUND TRIGGER " << triggerPath << "\n";
	//std::cout <<"Trigger prsecale: " <<  triggerPrescales->getPrescaleForIndex(i) << "\n";
	trigPass =  true;
      } else {//pass specified trigger
	recordedTriggers_->at(hltTriggers_->getTrigIndex(*triggerPath_Iter)) = 0;
      }
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

  //edm::Handle<pat::ElectronCollection> electrons;
  //iEvent.getByToken(electronToken_, electrons);
  //edm::Handle<edm::ValueMap<int> > heepId;
  //iEvent.getByToken(heepIdToken_,heepId);

  edm::Handle<edm::View<reco::GsfElectron> > eleHandle;
  edm::Handle<edm::ValueMap<bool> > vid;
  edm::Handle<edm::ValueMap<float> > trkIsolMap;
  
  iEvent.getByToken(electronToken_,eleHandle);
  iEvent.getByToken(vidToken_,vid);
  iEvent.getByToken(trkIsolMapToken_,trkIsolMap);

  //size_t eleNr=0;

  for(size_t eleNr=0;eleNr<eleHandle->size();eleNr++){
    //for (const pat::Electron &iEle : *electrons) {
    electronCollection_->push_back(ran::ElectronStruct{});
    ran::ElectronStruct &ithElec = electronCollection_->back();        

    edm::Ptr<reco::GsfElectron> elePtr(eleHandle,eleNr);
    const reco::GsfElectron& iEle = *elePtr; //Saves me having to replace the iEle's below to elePtr pointers. Just being lazy

    ithElec.et = iEle.et();
    ithElec.pt = iEle.pt();
    ithElec.eta = iEle.eta();
    ithElec.phi = iEle.phi();    

    ithElec.gsfTrack_available = iEle.gsfTrack().isAvailable();
    ithElec.scEta = iEle.superCluster()->eta();
    ithElec.scEnergy = iEle.superCluster()->energy(); 
    ithElec.ecalDrivenSeed = iEle.ecalDrivenSeed(); 
    ithElec.sigmaIetaIeta = iEle.sigmaIetaIeta();
    ithElec.full5x5_sigmaIetaIeta = iEle.full5x5_sigmaIetaIeta();
    //ithElec.passConversionVeto = iEle.passConversionVeto();
    ithElec.e2x5Max = iEle.e2x5Max();
    ithElec.e5x5 = iEle.e5x5();
    ithElec.e1x5 = iEle.e1x5();
    ithElec.deltaPhiSuperClusterTrackAtVtx = iEle.deltaPhiSuperClusterTrackAtVtx();
    ithElec.deltaEtaSuperClusterTrackAtVtx = iEle.deltaEtaSuperClusterTrackAtVtx();
    ithElec.hadronicOverEm = iEle.hadronicOverEm();
    //ithElec.nrMissHits = iEle.gsfTrack().trackerExpectedHitsInner().numberOfHits();
    ithElec.scSigmaIEtaIEta = iEle.scSigmaIEtaIEta();
    ithElec.dr03EcalRecHitSumEt = iEle.dr03EcalRecHitSumEt();
    ithElec.dr03HcalDepth1TowerSumEt = iEle.dr03HcalDepth1TowerSumEt();
    ithElec.dr03TkSumPt = iEle.dr03TkSumPt();
    //ithElec.pfIso_chgHad = iEle.pfIsolationVariables().chargedHadronIso;
    //ithElec.pfIso_neutHad = iEle.pfIsolationVariables().neutralHadronIso;
    //ithElec.pfIso_pht = iEle.pfIsolationVariables().photonIso;
    ithElec.inner_missing_hits =  iEle.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
    ithElec.isEB = iEle.isEB();
    ithElec.isEE = iEle.isEE();

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
    //ithElec.heep_rhoForIsolCorr = heepEle.rhoForIsolCorr();
    //ithElec.heep_applyRhoIsolCorr = heepEle.applyRhoIsolCorr();

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
    ithElec.passHEEPID = (*vid)[elePtr];
    ithElec.HEEPtrkIsol = (*trkIsolMap)[elePtr];
    //edm::Ptr<pat::Electron> elePtr(electrons,eleNr);
    //ithElec.heep_cutCode =  (*heepId)[elePtr];
    //++eleNr;   
  }
}
//Read in jet vars
void RALMiniAnalyzer::ReadInJets(const edm::Event& iEvent)
{
    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(jetToken_, jets);
    for (const pat::Jet &iJet: *jets) {
      jetCollection_->push_back(ran::JetStruct{});

      ran::JetStruct &ithJet = jetCollection_->back();
      ithJet.pt = iJet.pt();
      ithJet.et = iJet.et();
      ithJet.eta = iJet.eta();
      ithJet.phi = iJet.phi();
      ithJet.mass = iJet.mass();
      ithJet.numberOfDaughters = iJet.numberOfDaughters();
      ithJet.chargedMultiplicity = iJet.chargedMultiplicity();
      ithJet.neutralMultiplicity = iJet.neutralMultiplicity();
      ithJet.muonEnergyFraction = iJet.muonEnergyFraction();
      ithJet.electronEnergyFraction = iJet.electronEnergyFraction();
      ithJet.neutralHadronEnergyFraction = iJet.neutralHadronEnergyFraction();
      ithJet.HFHadronEnergyFraction = iJet.HFHadronEnergyFraction();
      ithJet.neutralEmEnergyFraction = iJet.neutralEmEnergyFraction();
      ithJet.chargedEmEnergyFraction = iJet.chargedEmEnergyFraction();
      ithJet.chargedHadronEnergyFraction = iJet.chargedHadronEnergyFraction();

      ithJet.jecFactor_unCorrected = iJet.jecFactor("Uncorrected");
      ithJet.userFloat_pileupJetId_fullDiscriminant = iJet.userFloat("pileupJetId:fullDiscriminant");

      //Assign the btag discriminators
      ///cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_20/src/PhysicsTools/PatAlgos/python/producersLayer1/jetProducer_cfi.py
      ithJet.pfJetProbabilityBJetTags =  iJet.bDiscriminator("pfJetProbabilityBJetTags");//
      ithJet.pfJetBProbabilityBJetTags =  iJet.bDiscriminator("pfJetBProbabilityBJetTags");//
      ithJet.pfTrackCountingHighEffBJetTags =  iJet.bDiscriminator("pfTrackCountingHighEffBJetTags");//   
      ithJet.pfTrackCountingHighPurBJetTags =  iJet.bDiscriminator("pfTrackCountingHighPurBJetTags");//
      ithJet.pfSimpleSecondaryVertexHighEffBJetTags =  iJet.bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags");//
      ithJet.pfSimpleSecondaryVertexHighPurBJetTags =  iJet.bDiscriminator("pfSimpleSecondaryVertexHighPurBJetTags");//
      ithJet.pfCombinedSecondaryVertexV2BJetTags =  iJet.bDiscriminator("pfCombinedSecondaryVertexV2BJetTags");//
      ithJet.pfCombinedInclusiveSecondaryVertexV2BJetTags =  iJet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
      ithJet.pfCombinedMVAV2BJetTags = iJet.bDiscriminator("pfCombinedMVAV2BJetTags");

      ithJet.partonFlavour = iJet.partonFlavour();

    }

}

//Read in fat jet vars
void RALMiniAnalyzer::ReadInFatJets(const edm::Event& iEvent)
{
    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(fatjetToken_, jets);
    for (const pat::Jet &iJet: *jets) {
      fatjetCollection_->push_back(ran::FatJetStruct{});

      ran::FatJetStruct &ithJet = fatjetCollection_->back();
      ithJet.pt = iJet.pt();
      ithJet.et = iJet.et();
      ithJet.eta = iJet.eta();
      ithJet.phi = iJet.phi();
      ithJet.mass = iJet.mass();
      ithJet.numberOfDaughters = iJet.numberOfDaughters();
      ithJet.chargedMultiplicity = iJet.chargedMultiplicity();
      ithJet.neutralMultiplicity = iJet.neutralMultiplicity();
      ithJet.muonEnergyFraction = iJet.muonEnergyFraction();
      ithJet.electronEnergyFraction = iJet.electronEnergyFraction();
      ithJet.neutralHadronEnergyFraction = iJet.neutralHadronEnergyFraction();
      ithJet.HFHadronEnergyFraction = iJet.HFHadronEnergyFraction();
      ithJet.neutralEmEnergyFraction = iJet.neutralEmEnergyFraction();
      ithJet.chargedEmEnergyFraction = iJet.chargedEmEnergyFraction();
      ithJet.chargedHadronEnergyFraction = iJet.chargedHadronEnergyFraction();

      ithJet.jecFactor_unCorrected = iJet.jecFactor("Uncorrected");

      ithJet.NjettinessAK8_tau1 = iJet.userFloat("NjettinessAK8:tau1");    //
      ithJet.NjettinessAK8_tau2 = iJet.userFloat("NjettinessAK8:tau2");    //  Access the n-subjettiness variables
      ithJet.NjettinessAK8_tau3 = iJet.userFloat("NjettinessAK8:tau3");    // 
 
      ithJet.CHSsoftdrop_mass = iJet.userFloat("ak8PFJetsCHSSoftDropMass"); // access to soft drop mass
      ithJet.CHSpruned_mass = iJet.userFloat("ak8PFJetsCHSPrunedMass");     // access to pruned mass

      ithJet.pfBoostedDoubleSecondaryVertexAK8BJetTags =  iJet.bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags");// Double b-tag

      /*ithJet.puppi_pt = iJet.userFloat("ak8PFJetsPuppiValueMap:pt");
      ithJet.puppi_mass = iJet.userFloat("ak8PFJetsPuppiValueMap:mass");
      ithJet.puppi_eta = iJet.userFloat("ak8PFJetsPuppiValueMap:eta");
      ithJet.puppi_phi = iJet.userFloat("ak8PFJetsPuppiValueMap:phi");
      ithJet.puppi_tau1 = iJet.userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau1");
      ithJet.puppi_tau2 = iJet.userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau2");
      ithJet.puppi_tau3 = iJet.userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau3");*/


      

      ithJet.partonFlavour = iJet.partonFlavour();

    }

}

void RALMiniAnalyzer::ReadInMets(const edm::Event& iEvent)
{
    edm::Handle<pat::METCollection> mets;
    iEvent.getByToken(metToken_, mets);
    for (const pat::MET &iMet: *mets) {
      metCollection_->push_back(ran::MetStruct{});

      ran::MetStruct &ithMet = metCollection_->back();
      ithMet.pt = iMet.pt();
      ithMet.eta = iMet.eta();
      ithMet.phi = iMet.phi();
      ithMet.sumEt = iMet.sumEt(); 
     
      //ithMet.metSignificance =  iMet.metSignificance();
      //ithMet.etFractionHadronic = iMet.etFractionHadronic();
      ithMet.isCaloMET = iMet.isCaloMET();
      ithMet.isPFMET = iMet.isPFMET();
      ithMet.isRecoMET = iMet.isRecoMET();
      /*ithMet.shiftedPt_JetEnUp = iMet.shiftedPt(pat::MET::JetEnUp);
       ithMet.shiftedPt_JetEnDown = iMet.shiftedPt(pat::MET::JetEnDown);   */

      ithMet.shiftedPt_JetEnUp = -99999.99;
      ithMet.shiftedPt_JetEnDown = -99999.99;

      //MC components
      if (isMC_){
	ithMet.genMet_pt = iMet.genMET()->pt();
      } else {
	ithMet.genMet_pt = -99999.9;
      }

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

      muonCollection_->push_back(ran::MuonStruct{}); 
      ran::MuonStruct &aMuon = muonCollection_->back(); 

      aMuon.pt = imuon.pt();
      aMuon.eta = imuon.eta();
      aMuon.phi = imuon.phi();
      aMuon.p4                 = imuon.p4();
      aMuon.charge             = imuon.charge();
      aMuon.isGlobalMuon       = imuon.isGlobalMuon();
      aMuon.isTrackerMuon      = imuon.isTrackerMuon();
      aMuon.isStandAloneMuon   = imuon.isStandAloneMuon();
      aMuon.isHighPtMuon       = imuon.isHighPtMuon(pV);
      aMuon.numMatchedMuonStns = imuon.numberOfMatchedStations();
   
      aMuon.dB                 = imuon.dB();

      aMuon.isPFIsolationValid = imuon.isPFIsolationValid();
      aMuon.pfIsoR03_sumChgHadPt  = imuon.pfIsolationR03().sumChargedHadronPt;
      aMuon.pfIsoR03_chgPartPt = imuon.pfIsolationR03().sumChargedParticlePt;
      aMuon.pfIsoR03_sumNeutHadPt =  imuon.pfIsolationR03().sumNeutralHadronEt;
      aMuon.pfIsoR03_sumPhtEt =  imuon.pfIsolationR03().sumPhotonEt;
      aMuon.pfIsoR03_sumNeutHadPtHighThr  =  imuon.pfIsolationR03().sumNeutralHadronEtHighThreshold;
      aMuon.pfIsoR03_sumPhtEtHighThr =  imuon.pfIsolationR03().sumPhotonEtHighThreshold;
      aMuon.pfIsoR03_sumPUPt =  imuon.pfIsolationR03().sumPUPt;
      aMuon.pfIsoR04_sumChgHadPt  = imuon.pfIsolationR04().sumChargedHadronPt;
      aMuon.pfIsoR04_chgPartPt = imuon.pfIsolationR04().sumChargedParticlePt;
      aMuon.pfIsoR04_sumNeutHadPt =  imuon.pfIsolationR04().sumNeutralHadronEt;
      aMuon.pfIsoR04_sumPhtEt =  imuon.pfIsolationR04().sumPhotonEt;
      aMuon.pfIsoR04_sumNeutHadPtHighThr  =  imuon.pfIsolationR04().sumNeutralHadronEtHighThreshold;
      aMuon.pfIsoR04_sumPhtEtHighThr =  imuon.pfIsolationR04().sumPhotonEtHighThreshold;
      aMuon.pfIsoR04_sumPUPt =  imuon.pfIsolationR04().sumPUPt;

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
        aMuon.globTrk_numberOfValidPixelHits = imuon.globalTrack()->hitPattern().numberOfValidPixelHits();
	aMuon.globTrk_numberOfValidMuonHits = imuon.globalTrack()->hitPattern().numberOfValidMuonHits();
	aMuon.globTrk_trackerLayersWithMeasurement = imuon.globalTrack()->hitPattern().trackerLayersWithMeasurement(); 
	aMuon.globTrk_normalisedChi2 = imuon.globalTrack()->normalizedChi2();
	aMuon.globTrk_dxy            = imuon.globalTrack()->dxy(pV.position());
        aMuon.globTrk_dz            = imuon.globalTrack()->dz(pV.position());
      } else {
	aMuon.globTrk_exists = false;
	aMuon.globTrk_pT    = -999.9;       
	aMuon.globTrk_eta  = -999.9;        
	aMuon.globTrk_phi  = -999.9;         
	aMuon.globTrk_charge  = -999.9;      
	aMuon.globTrk_numberOfValidMuonHits = -999.9;
	aMuon.globTrk_normalisedChi2 = -999.9;
        aMuon.globTrk_dxy    = -999.9;       
	aMuon.globTrk_dz  = -999.9;  
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
        aMuon.inTrk_dxy                = imuon.innerTrack()->dxy(pV.position());
        aMuon.inTrk_dz                = imuon.innerTrack()->dz(pV.position());
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
        aMuon.inTrk_dxy = -999.9;
	aMuon.inTrk_dz = -999.9;
      }

      // Outer track information ...
      if(imuon.outerTrack().get()!=0){
	aMuon.outTrk_exists = true;
	aMuon.outTrk_pT     = imuon.outerTrack()->pt();
	aMuon.outTrk_eta    = imuon.outerTrack()->eta();
	aMuon.outTrk_phi    = imuon.outerTrack()->phi();
	aMuon.outTrk_charge = imuon.outerTrack()->charge();
        aMuon.outTrk_dxy                = imuon.outerTrack()->dxy(pV.position());
        aMuon.outTrk_dz                = imuon.outerTrack()->dz(pV.position());
      } else {
	aMuon.outTrk_exists = false;
	aMuon.outTrk_pT     = -999.9;
	aMuon.outTrk_eta    = -999.9;
	aMuon.outTrk_phi    = -999.9;
	aMuon.outTrk_charge = 999;
        aMuon.outTrk_dxy = -999.9;
	aMuon.outTrk_dz = -999.9;
      }

          
      if( imuon.muonBestTrack().get()!=0 ){
	aMuon.bestTrk_exists = true;
	aMuon.bestTrk_dxy_bspot   = -999.9; // NEED TO ADD THE CORRECT VERTEX imuon.muonBestTrack()->dxy( h_beamSpot->position() );
	aMuon.bestTrk_dxy_vtx     = imuon.muonBestTrack()->dxy(pV.position());
	aMuon.bestTrk_dz_vtx      = imuon.muonBestTrack()->dz(pV.position());
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

      //TuneP muons
      if (imuon.tunePMuonBestTrack().get()!=0){
	aMuon.tuneP_exists =true;
	aMuon.tuneP_pt = imuon.tunePMuonBestTrack()->pt();
	aMuon.tuneP_eta = imuon.tunePMuonBestTrack()->eta();
	aMuon.tuneP_phi = imuon.tunePMuonBestTrack()->phi();
	aMuon.tuneP_charge = imuon.tunePMuonBestTrack()->charge();
	aMuon.tuneP_ptError = imuon.tunePMuonBestTrack()->ptError();
	aMuon.tuneP_dxy = imuon.tunePMuonBestTrack()->dxy(pV.position());
        aMuon.tuneP_dz = imuon.tunePMuonBestTrack()->dz(pV.position());
      }else{
	aMuon.tuneP_exists =false;
        aMuon.tuneP_pt = -999.9;
	aMuon.tuneP_eta =-999.9;
	aMuon.tuneP_phi = -999.9;
	aMuon.tuneP_charge = -999.9;
	aMuon.tuneP_ptError = -999.9;
	aMuon.tuneP_dxy = -999.9;
	aMuon.tuneP_dz = -999.9;

      }

    }

}
//define this as a plug-in
DEFINE_FWK_MODULE(RALMiniAnalyzer);
