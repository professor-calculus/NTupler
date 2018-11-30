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
#include <random>

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

#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/src/classes.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetResolutionObject.h"
#include "CondFormats/JetMETObjects/interface/JetResolution.h"

#include "TH1D.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//...in order to use the heep::Ele class ...
#include "SHarper/HEEPAnalyzer/interface/HEEPEle.h"
#include "SHarper/HEEPAnalyzer/interface/HEEPEleSelector.h"
#include "SHarper/HEEPAnalyzer/interface/HEEPEvent.h"

//... for accessing generator information (GenEventInfoProduct & LHE)
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

//...for histograms creation
#include "FWCore/ServiceRegistry/interface/Service.h"

//ROOT HEADERS
#include "TTree.h"
#include "TF1.h"

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

      //For getting the correction factor for the PUPPI softDropMass 
      float getPUPPIweight(const double&, const double& puppieta);

      //Did event pass specified trigger
      bool passedTrigger(const edm::Event&);

      ///For clearing contents/setting default values of variables that should get new values in each event...
      void ResetEventByEventVariables();

      ///For reading the event information into the member variables...
      void ReadInEvtInfo(bool, const edm::Event&);

      ///For reading the LHE information, and dumping it into ran::Event class ...
      void ReadInLheInfo(const edm::Event&);

      ///For reading the Vertex/PU information, and dumping it into ran::Event class ...
      void ReadInVertexInfo(const edm::Event&);

      ///For reading in the electron information, and dumping it into ran::Event class ...
      void ReadInElectrons(const edm::Event&);
  
      ///For reading in the muon information, and dumping it into ran::Event class ...
      void ReadInMuons(const edm::Event&);

      ///For reading in the Jet information, and dumping it into ran::Event class ...
      void ReadInJets(const edm::Event&, JetCorrectionUncertainty*, JME::JetResolution&, JME::JetResolutionScaleFactor&);

      ///For reading in the Fat Jet information, and dumping it into ran::Event class ...
      void ReadInFatJets(const edm::Event&, JetCorrectionUncertainty*, JME::JetResolution&, JME::JetResolutionScaleFactor&);

      ///For reading in the Met information, and dumping it into ran::Event class ...
      void ReadInMets(const edm::Event&);

      ///For reading in the ISR information
      void ReadInIsrInfo(const edm::Event&);

      ///For reading in the Prefire information
      void ReadInPrefireInfo(const edm::Event&);

      // ----------member data ---------------------------

      bool isThis2016_;
      bool isMC_;
      const bool inputContainsLHE_;
      const bool ignoreTopInLheHtCalculation_;
      TF1 * puppisd_corrGEN_;
      TF1 * puppisd_corrRECO_cen_;
      TF1 * puppisd_corrRECO_for_;

      edm::EDGetTokenT<LHEEventProduct> lheToken_;
      edm::EDGetTokenT<std::vector<PileupSummaryInfo>> puInfoToken_;
      edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
      edm::EDGetTokenT<pat::MuonCollection> muonToken_;
      edm::EDGetTokenT<edm::View<reco::GsfElectron> > electronToken_;
  //edm::EDGetTokenT<edm::ValueMap<int> > heepIdToken_;
      edm::EDGetTokenT<pat::JetCollection> jetToken_;
      edm::EDGetTokenT<pat::JetCollection> fatjetToken_;
      edm::EDGetTokenT<reco::GenJetCollection> genjetToken_;
      edm::EDGetTokenT<reco::GenJetCollection> genjetAK8Token_;
      edm::EDGetTokenT<reco::GenParticleCollection> genParticleToken_;
      edm::EDGetTokenT<pat::METCollection> metToken_;
  edm::EDGetTokenT<edm::View<reco::GsfElectron> > eleAODToken_;//delete this
  edm::EDGetTokenT<edm::View<reco::GsfElectron> > eleMiniAODToken_;//delete this
      edm::EDGetTokenT<edm::ValueMap<bool> > vidToken_; //VID is versioned ID, is the standard E/gamma ID producer which we have configured for HEEP
      edm::EDGetTokenT<edm::ValueMap<float> > trkIsolMapToken_;

      edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
      edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
      edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
      std::vector<std::string> targetTriggerPaths_;

      edm::EDGetTokenT<double> prefweightToken_;
      edm::EDGetTokenT<double> prefweightupToken_;
      edm::EDGetTokenT<double> prefweightdownToken_;

      edm::EDGetTokenT<double> rhoToken_;
      
      //Ntuple Tree
      edm::Service<TFileService> fHistos;
      TTree* EventDataTree;
      TTree* TriggerPathsTree;

      //Variables whose values will be stored as branches...
      //ran::Event* event_;
      ran::EventInfo evtInfo{};
      float lheHT_;
      float nTrueInt_;
      int nPU_;
      int nISR_;
      int nGLUINO_;
      float prefweight_;
      float prefweightup_;
      float prefweightdown_;
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
    isThis2016_(iConfig.getParameter<bool>("isThis2016")),
    isMC_(iConfig.getParameter<bool>("isThisMC")),
    inputContainsLHE_(iConfig.getParameter<bool>("containsLHE")),
    ignoreTopInLheHtCalculation_(inputContainsLHE_ ? iConfig.getParameter<bool>("ignoreTopInLheHtCalculation") : false),
    lheToken_(consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("lhe"))),
    puInfoToken_(consumes<std::vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("puInfo"))),
    vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
    muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
    electronToken_(consumes<edm::View<reco::GsfElectron> >(iConfig.getParameter<edm::InputTag>("electrons"))),
    jetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
    fatjetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("fatjets"))),
    genjetToken_(consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genjets"))),
    genjetAK8Token_(consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genjetsAK8"))),
    genParticleToken_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticles"))),
    metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets"))),
    vidToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("vid"))),
    trkIsolMapToken_(consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("trkIsolMap"))),
    //eleRhoCorrLabel_(iConfig.getParameter<edm::InputTag>("eleRhoCorrLabel")),
    //applyRhoCorrToEleIsol_(iConfig.getParameter<bool>("applyRhoCorrToEleIsol")),
    //verticesLabel_(iConfig.getParameter<edm::InputTag>("verticesLabel")),
    triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
    triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"))),
    triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"))),
    targetTriggerPaths_(iConfig.getParameter<std::vector<std::string> >("selectedTriggerPaths")),

    prefweightToken_(consumes<double>(edm::InputTag("prefiringweight:NonPrefiringProb"))),
    prefweightupToken_(consumes<double>(edm::InputTag("prefiringweight:NonPrefiringProbUp"))),
    prefweightdownToken_(consumes<double>(edm::InputTag("prefiringweight:NonPrefiringProbDown"))),

    rhoToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("rho")))
    //cuts_(iConfig)
{
    std::cout << "*,. *,. *,. *,. *,. *,. *,." << std::endl;
    if (isThis2016_ == true && isMC_ == false) std::cout << "Running on 2016 data (80X)" << std::endl; 
    else if (isThis2016_ == true && isMC_ == true) std::cout << "Running on 2016 MC (80X)" << std::endl; 
    else if (isThis2016_ == false && isMC_ == false) std::cout << "Running on 2017 data (94X)" << std::endl;
    else if (isThis2016_ == false && isMC_ == true) std::cout << "Running on 2017 MC (94X)" << std::endl;
    std::cout << "*,. *,. *,. *,. *,. *,. *,." << std::endl;

    EventDataTree = fHistos->make<TTree>("EventDataTree", "Event data tree");
    //EventDataTree = new TTree("EventDataTree","EventDataTree");
    //Setting up the links between variables and branches...
    //event_ = 0;	
    //EventDataTree->Branch("event","ran::Event", &event_, 64000, 1); // This line was taken from Jim's tupiliser
    EventDataTree->Branch("evtInfo","ran::EventInfo",&evtInfo);
    if (inputContainsLHE_) {
      EventDataTree->Branch("lheHT", &lheHT_, "lheHT/F");
    }
    EventDataTree->Branch("nTrueInt", &nTrueInt_, "nTrueInt/F");
    EventDataTree->Branch("nPU", &nPU_, "nPU/I");
    EventDataTree->Branch("nISR", &nISR_, "nISR/I");
    EventDataTree->Branch("nGluino", &nGLUINO_, "nGluino/I");
    EventDataTree->Branch("electronCollection","std::vector<ran::ElectronStruct>", &electronCollection_, 64000, 1); 
    EventDataTree->Branch("muonCollection","std::vector<ran::MuonStruct>", &muonCollection_, 64000, 1); 
    EventDataTree->Branch("jetCollection","std::vector<ran::JetStruct>", &jetCollection_, 64000, 1);
    EventDataTree->Branch("fatjetCollection","std::vector<ran::FatJetStruct>", &fatjetCollection_, 64000, 1);
    EventDataTree->Branch("metCollection","std::vector<ran::MetStruct>", &metCollection_, 64000, 1);
    EventDataTree->Branch("recordedTriggers", &recordedTriggers_);
    //EventDataTree->Branch("recordedTriggers","std::vector<char>", &recordedTriggers_, 64000, 1);
    EventDataTree->Branch("prefweight", &prefweight_, "prefweight/F");
    EventDataTree->Branch("prefweightup", &prefweightup_, "prefweightup/F");
    EventDataTree->Branch("prefweightdown", &prefweightdown_, "prefweightdown/F");

    //Seperate tree to store trigger names
    TriggerPathsTree = fHistos->make<TTree>("TriggerPathsTree", "Trigger Paths tree");
    //TriggerPathsTree = new TTree("TrigTree","EventDataTree");
    TriggerPathsTree->Branch("triggerPaths", &triggerPaths_);


    //Set Trigger paths
    hltTriggers_ = new ran::TriggerPathToIndex(targetTriggerPaths_);

    triggerPaths_ = new std::vector<std::string>();
    triggerPaths_->insert(triggerPaths_->begin(),targetTriggerPaths_.begin(), targetTriggerPaths_.end());

    // Load functions for PUPPI softDrop
    TFile* file = TFile::Open("puppiCorr.root", "READ");
    puppisd_corrGEN_      = (TF1*)file->Get("puppiJECcorr_gen");
    puppisd_corrRECO_cen_ = (TF1*)file->Get("puppiJECcorr_reco_0eta1v3");
    puppisd_corrRECO_for_ = (TF1*)file->Get("puppiJECcorr_reco_1v3eta2v5");
    delete file;
}


RALMiniAnalyzer::~RALMiniAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  delete hltTriggers_;
  delete triggerPaths_;
  delete puppisd_corrGEN_;
  delete puppisd_corrRECO_cen_;
  delete puppisd_corrRECO_for_;
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


   if (triggerOfInterest || isMC_){ // default
   // if (true){ // use this option if running on data and want all events in the dataset (trigger studies only)

     electronCollection_ = new std::vector<ran::ElectronStruct>();
     muonCollection_ = new std::vector<ran::MuonStruct>();
     jetCollection_ = new std::vector<ran::JetStruct>();
     fatjetCollection_ = new std::vector<ran::FatJetStruct>();
     metCollection_ = new std::vector<ran::MetStruct>();
    
    ESHandle<JetCorrectorParametersCollection> JetCorParColl_AK4;
    iSetup.get<JetCorrectionsRecord>().get("AK4PFchs", JetCorParColl_AK4); 
    JetCorrectorParameters const & JetCorPar_AK4 = (*JetCorParColl_AK4)["Uncertainty"];
    JetCorrectionUncertainty *jecUncObj_AK4 = new JetCorrectionUncertainty(JetCorPar_AK4);

    ESHandle<JetCorrectorParametersCollection> JetCorParColl_AK8;
    if (isThis2016_) iSetup.get<JetCorrectionsRecord>().get("AK8PFchs", JetCorParColl_AK8); 
    else iSetup.get<JetCorrectionsRecord>().get("AK8PFPuppi", JetCorParColl_AK8); 
    JetCorrectorParameters const & JetCorPar_AK8 = (*JetCorParColl_AK8)["Uncertainty"];
    JetCorrectionUncertainty *jecUncObj_AK8 = new JetCorrectionUncertainty(JetCorPar_AK8);

    JME::JetResolution resolution_AK4 = JME::JetResolution::get(iSetup, "AK4PFchs_pt");
    JME::JetResolutionScaleFactor resolutionSF_AK4 = JME::JetResolutionScaleFactor::get(iSetup, "AK4PFchs");

    JME::JetResolution resolution_AK8;
    JME::JetResolutionScaleFactor resolutionSF_AK8;
    if (isThis2016_){
      resolution_AK8 = JME::JetResolution::get(iSetup, "AK8PFchs_pt");
      resolutionSF_AK8 = JME::JetResolutionScaleFactor::get(iSetup, "AK8PFchs");
    }
    else{
      resolution_AK8 = JME::JetResolution::get(iSetup, "AK8PFPuppi_pt");
      resolutionSF_AK8 = JME::JetResolutionScaleFactor::get(iSetup, "AK8PFPuppi"); 
    }

     //Clearing contents/setting default values of variables that should get new values in each event...
     ResetEventByEventVariables();

     //Reading in the event information...
     ReadInEvtInfo(vBool_, iEvent);     

     // Read in LHE event product (if present)
     if (inputContainsLHE_)
       ReadInLheInfo(iEvent);

     // Read in PU info
     if (isMC_)
       ReadInVertexInfo(iEvent);

     //Read in muons
     ReadInMuons(iEvent);
            
     //Read in Electrons
     ReadInElectrons(iEvent);

     //Read in Jets
     ReadInJets(iEvent, jecUncObj_AK4, resolution_AK4, resolutionSF_AK4);

     //Read in Jets
     ReadInFatJets(iEvent, jecUncObj_AK8, resolution_AK8, resolutionSF_AK8);

     //Read in Met
     ReadInMets(iEvent);

     //Read in ISR info
     if (isMC_)
       ReadInIsrInfo(iEvent);

     //Read in pre-fire info
     if (isMC_)
       ReadInPrefireInfo(iEvent);

     //Fill Ntuple
     EventDataTree->Fill();	

     //delete event_;
     delete jecUncObj_AK4;
     delete jecUncObj_AK8;

     delete electronCollection_;
     delete muonCollection_;
     delete fatjetCollection_;
     delete jetCollection_;
     delete metCollection_;

     delete recordedTriggers_;

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

	lheHT_ = 0.0;
  nTrueInt_ = 0.0;
  nPU_ = 0;
  nISR_ = 0;
  nGLUINO_ = 0;
}

//------------ For getting the correction factor for the PUPPI softDropMass -------------
float RALMiniAnalyzer::getPUPPIweight(const double& puppipt, const double& puppieta){

  double genCorr  = 1.;
  double recoCorr = 1.;
  double totalWeight = 1.;

  genCorr =  puppisd_corrGEN_->Eval( puppipt );

  if( fabs(puppieta)  <= 1.3 ){
    recoCorr = puppisd_corrRECO_cen_->Eval( puppipt );
  }
  else{
    recoCorr = puppisd_corrRECO_for_->Eval( puppipt );
  }

  totalWeight = genCorr * recoCorr;

  return totalWeight;
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
	//std::cout <<"Trigger prescale: " <<  triggerPrescales->getPrescaleForIndex(i) << "\n";
	trigPass =  true;
      }
 //      else {//pass specified trigger
	// recordedTriggers_->at(hltTriggers_->getTrigIndex(*triggerPath_Iter)) = 0;
 //      }
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


void RALMiniAnalyzer::ReadInLheInfo(const edm::Event& iEvent)
{
  edm::Handle<LHEEventProduct> lheEventHandle;
  iEvent.getByToken(lheToken_, lheEventHandle);

  if( !(lheEventHandle.failedToGet()) ){

    float ht = 0.0;
    const lhef::HEPEUP eventInfo = lheEventHandle->hepeup();

    // Now, loop over the particles in the LHE event ...
    int numParticles = eventInfo.NUP;
    for(int idx=0; idx<numParticles; idx++){
      const int pdgId = abs(eventInfo.IDUP.at(idx));
      const int status = eventInfo.ISTUP.at(idx);
      const std::pair<int, int> parents = eventInfo.MOTHUP.at(idx);
      const int parent1pdgId = parents.first == 0 ? 0 : eventInfo.IDUP.at(parents.first - 1);
      const int parent2pdgId = parents.second == 0 ? 0 : eventInfo.IDUP.at(parents.second - 1);
      const double pT = sqrt(eventInfo.PUP.at(idx)[0]*eventInfo.PUP.at(idx)[0] + eventInfo.PUP.at(idx)[1]*eventInfo.PUP.at(idx)[1]);

      // Calculating HT: Scalar sum of pT of ...
      //   - quarks & gluons from the final state
      //   - excluding particles that come from decay of top or W - at least for certain ttbar samples
      // According to https://hypernews.cern.ch/HyperNews/CMS/get/generators/3367/1.html
      if (status == 1 ) {
        if (!ignoreTopInLheHtCalculation_)
          ht += pT;
        else if (((pdgId > 0 && pdgId < 6) || pdgId == 21) && parent1pdgId != 6 && parent2pdgId != 6 && parent1pdgId != 24 && parent2pdgId != 24
                 && parent1pdgId != 23 && parent2pdgId != 23 && parent1pdgId != 25 && parent2pdgId != 25)
          ht += pT;
      }

    } //END: for idx<numParticles
    lheHT_ = ht;
  }
  else 
    throw std::runtime_error("Could not read LHE information");
}


void RALMiniAnalyzer::ReadInVertexInfo(const edm::Event& iEvent)
{
  edm::Handle<std::vector<PileupSummaryInfo>> puHandle;
  iEvent.getByToken(puInfoToken_, puHandle);

  nTrueInt_ = puHandle->begin()->getTrueNumInteractions();
  nPU_ = puHandle->begin()->getPU_NumInteractions();

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
    // ithElec.inner_missing_hits =  iEle.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
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
void RALMiniAnalyzer::ReadInJets(const edm::Event& iEvent, JetCorrectionUncertainty * jecUncObj, JME::JetResolution& resolution, JME::JetResolutionScaleFactor& resolutionSF)
{
    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(jetToken_, jets);

    edm::Handle<reco::GenJetCollection> genjets;
    iEvent.getByToken(genjetToken_, genjets);

    edm::Handle<double> rho;
    iEvent.getByToken(rhoToken_, rho);

    for (const pat::Jet &iJet: *jets) {
    

      // JER smearing procedure: https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution
      double c_nom(1.0), c_up(1.0), c_down(1.0);

      if (isMC_){

        JME::JetParameters parameters;
        parameters.setJetEta( iJet.eta() );
        parameters.setJetPt( iJet.pt() );
        parameters.setRho( *rho );

        float jer = resolution.getResolution(parameters);
        float jerSF_nom = resolutionSF.getScaleFactor(parameters);
        float jerSF_up = resolutionSF.getScaleFactor(parameters, Variation::UP);
        float jerSF_down = resolutionSF.getScaleFactor(parameters, Variation::DOWN);

        bool haveGenMatch = false;
        double dR_matchedGenJet = 9999.99;
        double pt_matchedGenJet = -9999.99;
        for (const reco::GenJet &iGenJet : *genjets){
          
          double dR = sqrt( deltaR2(iGenJet.eta(), iGenJet.phi(), iJet.eta(), iJet.phi()) );
          double dPT = fabs( iGenJet.pt() - iJet.pt() );
          const double conesize = 0.4;
          if ( dR < conesize/2 && dR < dR_matchedGenJet && dPT < 3*jer*iJet.pt() ){
            haveGenMatch = true;
            dR_matchedGenJet = dR;
            pt_matchedGenJet = iGenJet.pt();
          }
        }
        
        if (haveGenMatch){
        
          c_nom = 1.0 + (jerSF_nom - 1.0) * (iJet.pt() - pt_matchedGenJet) / (iJet.pt());
          c_up = 1.0 + (jerSF_up - 1.0) * (iJet.pt() - pt_matchedGenJet) / (iJet.pt());
          c_down = 1.0 + (jerSF_down - 1.0) * (iJet.pt() - pt_matchedGenJet) / (iJet.pt());
        
        }
        
        else{
        
          std::random_device rd;
          std::mt19937 e2(rd());
          std::normal_distribution<> dist(0, jer);
          double gaussRandom = dist(e2);

          if (jerSF_nom > 1.0) c_nom = 1.0 + gaussRandom * sqrt(jerSF_nom * jerSF_nom - 1);
          if (jerSF_up > 1.0) c_up = 1.0 + gaussRandom * sqrt(jerSF_up * jerSF_up - 1);
          if (jerSF_down > 1.0) c_down = 1.0 + gaussRandom * sqrt(jerSF_down * jerSF_down - 1);
        
        }
        
        if (c_nom < 0) c_nom = 0.0;
        if (c_up < 0) c_up = 0.0;
        if (c_down < 0) c_down = 0.0;

      } // closes 'if' MC


      jetCollection_->push_back(ran::JetStruct{});
      
      ran::JetStruct &ithJet = jetCollection_->back();
      ithJet.pt = iJet.pt() * c_nom;
      ithJet.et = iJet.et() * c_nom;
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
      ithJet.userFloat_pileupJetId_fullDiscriminant = iJet.userFloat("pileupJetId:fullDiscriminant");

      ithJet.jecFactor_unCorrected = iJet.jecFactor("Uncorrected");
      jecUncObj->setJetEta( iJet.eta() );
      jecUncObj->setJetPt( iJet.pt() ); // here you use the CORRECTED jet pt
      ithJet.jecUncertainty = float(jecUncObj->getUncertainty(true));

      if (c_nom != 0){
        ithJet.jerUncUp = c_up / c_nom;
        ithJet.jerUncDown = c_down / c_nom;
      }
      else{
        ithJet.jerUncUp = 1.0;
        ithJet.jerUncDown = 1.0;              
      }

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

    } // closes loop through jets

}

//Read in fat jet vars
void RALMiniAnalyzer::ReadInFatJets(const edm::Event& iEvent, JetCorrectionUncertainty * jecUncObj, JME::JetResolution& resolution, JME::JetResolutionScaleFactor& resolutionSF)
{
    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(fatjetToken_, jets);

    edm::Handle<reco::GenJetCollection> genjets;
    iEvent.getByToken(genjetAK8Token_, genjets);

    edm::Handle<double> rho;
    iEvent.getByToken(rhoToken_, rho);

    for (const pat::Jet &iJet: *jets) {
      
      if (iJet.pt() <= 170.0) continue;

      // JER smearing procedure: https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution
      double c_nom(1.0), c_up(1.0), c_down(1.0);

      if (isMC_){

        JME::JetParameters parameters;
        parameters.setJetEta( iJet.eta() );
        parameters.setJetPt( iJet.pt() );
        parameters.setRho( *rho );

        float jer = resolution.getResolution(parameters);
        float jerSF_nom = resolutionSF.getScaleFactor(parameters);
        float jerSF_up = resolutionSF.getScaleFactor(parameters, Variation::UP);
        float jerSF_down = resolutionSF.getScaleFactor(parameters, Variation::DOWN);

        bool haveGenMatch = false;
        double dR_matchedGenJet = 9999.99;
        double pt_matchedGenJet = -9999.99;
        for (const reco::GenJet &iGenJet : *genjets){
          
          double dR = sqrt( deltaR2(iGenJet.eta(), iGenJet.phi(), iJet.eta(), iJet.phi()) );
          double dPT = fabs( iGenJet.pt() - iJet.pt() );
          const double conesize = 0.8;
          if ( dR < conesize/2 && dR < dR_matchedGenJet && dPT < 3*jer*iJet.pt() ){
            haveGenMatch = true;
            dR_matchedGenJet = dR;
            pt_matchedGenJet = iGenJet.pt();
          }
        }
        
        if (haveGenMatch){
        
          c_nom = 1.0 + (jerSF_nom - 1.0) * (iJet.pt() - pt_matchedGenJet) / (iJet.pt());
          c_up = 1.0 + (jerSF_up - 1.0) * (iJet.pt() - pt_matchedGenJet) / (iJet.pt());
          c_down = 1.0 + (jerSF_down - 1.0) * (iJet.pt() - pt_matchedGenJet) / (iJet.pt());
        
        }
        
        else{
        
          std::random_device rd;
          std::mt19937 e2(rd());
          std::normal_distribution<> dist(0, jer);
          double gaussRandom = dist(e2);

          if (jerSF_nom > 1.0) c_nom = 1.0 + gaussRandom * sqrt(jerSF_nom * jerSF_nom - 1);
          if (jerSF_up > 1.0) c_up = 1.0 + gaussRandom * sqrt(jerSF_up * jerSF_up - 1);
          if (jerSF_down > 1.0) c_down = 1.0 + gaussRandom * sqrt(jerSF_down * jerSF_down - 1);
        
        }
        
        if (c_nom < 0) c_nom = 0.0;
        if (c_up < 0) c_up = 0.0;
        if (c_down < 0) c_down = 0.0;

      } // closes 'if' MC


      fatjetCollection_->push_back(ran::FatJetStruct{});

      ran::FatJetStruct &ithJet = fatjetCollection_->back();
      ithJet.pt = iJet.pt() * c_nom;
      ithJet.et = iJet.et() * c_nom;
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
      jecUncObj->setJetEta( iJet.eta() );
      jecUncObj->setJetPt( iJet.pt() ); // here you use the CORRECTED jet pt
      ithJet.jecUncertainty = float(jecUncObj->getUncertainty(true));


      // 2016 specific fatJets
      if (isThis2016_){
      
        ithJet.NjettinessAK8_tau1 = iJet.userFloat("NjettinessAK8:tau1");    //
        ithJet.NjettinessAK8_tau2 = iJet.userFloat("NjettinessAK8:tau2");    //  Access the n-subjettiness variables
        ithJet.NjettinessAK8_tau3 = iJet.userFloat("NjettinessAK8:tau3");    // 
        
        ithJet.CHSsoftdrop_mass = iJet.userFloat("ak8PFJetsCHSSoftDropMass"); // access to soft drop mass
        ithJet.CHSpruned_mass = iJet.userFloat("ak8PFJetsCHSPrunedMass");     // access to pruned mass
      
        // softDropMass with PUPPI
        double puppi_pt             = iJet.userFloat("ak8PFJetsPuppiValueMap:pt");
        double puppi_eta           = iJet.userFloat("ak8PFJetsPuppiValueMap:eta");
        float puppiCorr = getPUPPIweight( puppi_pt , puppi_eta );

        TLorentzVector puppi_softdrop, puppi_softdrop_subjet;
        auto const & sdSubjetsPuppi = iJet.subjets("SoftDropPuppi");
        for ( auto const & it : sdSubjetsPuppi ) {
          puppi_softdrop_subjet.SetPtEtaPhiM(it->correctedP4(0).pt(), it->correctedP4(0).eta(), it->correctedP4(0).phi(), it->correctedP4(0).mass());
          puppi_softdrop += puppi_softdrop_subjet;
        }

        float puppi_softdrop_masscorr = puppi_softdrop.M() * puppiCorr;
        ithJet.PUPPIsoftdrop_mass = puppi_softdrop_masscorr;
      } // closes 2016 jet masses and nJettiness

      // 2017 specific fatJets
      else{
      
        ithJet.NjettinessAK8_tau1 = iJet.userFloat("NjettinessAK8Puppi:tau1");    //
        ithJet.NjettinessAK8_tau2 = iJet.userFloat("NjettinessAK8Puppi:tau2");    //  Access the n-subjettiness variables
        ithJet.NjettinessAK8_tau3 = iJet.userFloat("NjettinessAK8Puppi:tau3");    // 
        
        ithJet.CHSsoftdrop_mass = iJet.userFloat("ak8PFJetsCHSValueMap:ak8PFJetsCHSSoftDropMass"); // access to soft drop mass
        ithJet.CHSpruned_mass = iJet.userFloat("ak8PFJetsCHSValueMap:ak8PFJetsCHSPrunedMass");     // access to pruned mass
      
        // softDropMass with PUPPI
        double puppi_pt             = iJet.pt();
        double puppi_eta           = iJet.eta();
        float puppiCorr = getPUPPIweight( puppi_pt , puppi_eta );

        TLorentzVector puppi_softdrop, puppi_softdrop_subjet;
        auto const & sdSubjetsPuppi = iJet.subjets("SoftDropPuppi");
        for ( auto const & it : sdSubjetsPuppi ) {
          puppi_softdrop_subjet.SetPtEtaPhiM(it->correctedP4(0).pt(), it->correctedP4(0).eta(), it->correctedP4(0).phi(), it->correctedP4(0).mass());
          puppi_softdrop += puppi_softdrop_subjet;
        }

        float puppi_softdrop_masscorr = puppi_softdrop.M() * puppiCorr;
        ithJet.PUPPIsoftdrop_mass = puppi_softdrop_masscorr;

        // OLD - softdrop mass drawn directly
        // ithJet.PUPPIsoftdrop_mass = iJet.userFloat("ak8PFJetsPuppiSoftDropMass");
        // ithJet.PUPPIsoftdrop_mass = iJet.userFloat("ak8PFJetsPuppiSoftDropMass") * puppiCorr;

      } // closes 2017 jet masses and nJettiness


      ithJet.pfBoostedDoubleSecondaryVertexAK8BJetTags =  iJet.bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags");// Double b-tag
      ithJet.partonFlavour = iJet.partonFlavour();

      if (c_nom != 0){
        ithJet.jerUncUp = c_up / c_nom;
        ithJet.jerUncDown = c_down / c_nom;
      }
      else{
        ithJet.jerUncUp = 1.0;
        ithJet.jerUncDown = 1.0;              
      }



    } // closes loop through fatJets

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

void RALMiniAnalyzer::ReadInIsrInfo(const edm::Event& iEvent)
{
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByToken(genParticleToken_, genParticles);

    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(jetToken_, jets);


    // method 1 - count extra partons in matrix element (DO NOT USE)
    // unsigned int nisr_me = 0;
    
    // for (const reco::GenParticle &iGenParticle: *genParticles){

    //   // we require a hard outgoing quark or gluon
    //   if (iGenParticle.status()!=23 || (abs(iGenParticle.pdgId())>5 && iGenParticle.pdgId()!=21) ) continue;
      
    //   // we require that the mother is from a parton
    //   unsigned int momid = abs(iGenParticle.mother()->pdgId());
    //   if (momid <= 5 || momid == 21){

    //     nisr_me++;
    //     std::cout << "Additional Parton (pt,eta,phi): " << iGenParticle.pt() << " " << iGenParticle.eta() << " " << iGenParticle.phi() << std::endl;
    //   }

    // } // closes loop through genParticles


    // method 2 - https://github.com/manuelfs/babymaker/blob/0136340602ee28caab14e3f6b064d1db81544a0a/bmaker/plugins/bmaker_full.cc#L1268-L1295
    // count the number of jets that do NOT match to the hard partons (more specifically, their daughters) coming from the MC feynman diagrams
    // this is the number of ISR jets
    unsigned int nisr = 0;

    for (const pat::Jet &iJet: *jets){

      // only consider the jets like we do in the analysis
      if (iJet.pt() < 40.0 || fabs(iJet.eta()) > 3.0) continue;

      bool matched = false;
  
      for (const reco::GenParticle &iGenParticle: *genParticles){
        if (matched) break;
    
        // we require hard outgoing quarks
        if (iGenParticle.status()!=23 || abs(iGenParticle.pdgId())>5) continue;
    
        // we require that the mother IS from t,Z,W,h,H,SUSY
        unsigned int momid = abs(iGenParticle.mother()->pdgId());
        if ( !(momid==6 || momid==23 || momid==24 || momid==25 || momid==35 || momid>1e6) ) continue; 
        
        // check against daughter in case of hard initial splitting
        for (size_t idau(0); idau < iGenParticle.numberOfDaughters(); ++idau) {
          float dR = deltaR(iJet.p4(), iGenParticle.daughter(idau)->p4());
          if (dR < 0.3){
            matched = true;
            break;
          }
        } // closes loop through daughter particles
      
      } // closes loop over MC particles
  
      // if there are no matches, it is an ISR jet
      if (matched == false){
        nisr++;
        // std::cout << "ISR Jet (pt,eta,phi):           " << iJet.pt() << " " << iJet.eta() << " " << iJet.phi() << std::endl;
      }

    } // closes loop over jets

    // Event ISR Info
    // std::cout << "Method 1 isr count: " << nisr_me << std::endl;
    // std::cout << "Method 2 isr count: " << nisr << std::endl;
    // std::cout << std::endl;

    nISR_ = nisr;


    // additional info for signal processes - count the number of gluinos in the event
    unsigned int nGluino = 0;
    for (const reco::GenParticle &iGenParticle: *genParticles){

      if (iGenParticle.pdgId()==1000021 && iGenParticle.numberOfDaughters()==2)
        nGluino++;

    } // closes loop through genParticles
    // std::cout << "number of gluinos: " << nGluino << "\n" << std::endl;
    nGLUINO_ = nGluino;
}

void RALMiniAnalyzer::ReadInPrefireInfo(const edm::Event& iEvent)
{
  edm::Handle<double> theprefweight;
  iEvent.getByToken(prefweightToken_, theprefweight ) ;
  prefweight_ =(*theprefweight);

  edm::Handle<double> theprefweightup;
  iEvent.getByToken(prefweightupToken_, theprefweightup ) ;
  prefweightup_ =(*theprefweightup);

  edm::Handle<double> theprefweightdown;
  iEvent.getByToken(prefweightdownToken_, theprefweightdown ) ;
  prefweightdown_ =(*theprefweightdown);
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