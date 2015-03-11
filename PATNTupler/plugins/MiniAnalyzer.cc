// -*- C++ -*-
//
// Package:    Test/MiniAnalyzer
// Class:      MiniAnalyzer
// 
/**\class MiniAnalyzer MiniAnalyzer.cc Test/MiniAnalyzer/plugins/MiniAnalyzer.cc

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

class MiniAnalyzer : public edm::EDAnalyzer {
   public:
      explicit MiniAnalyzer(const edm::ParameterSet&);
      ~MiniAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

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


      // ----------member data ---------------------------

      edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
      edm::EDGetTokenT<pat::MuonCollection> muonToken_;
      edm::EDGetTokenT<pat::ElectronCollection> electronToken_;
      //edm::EDGetTokenT<pat::TauCollection> tauToken_;
      // edm::EDGetTokenT<pat::PhotonCollection> photonToken_;
      edm::EDGetTokenT<pat::JetCollection> jetToken_;
      edm::EDGetTokenT<pat::JetCollection> fatjetToken_;
      edm::EDGetTokenT<pat::METCollection> metToken_;

      //Ntuple Tree
      edm::Service<TFileService> fHistos;
      TTree* EventDataTree;

      //Variables whose values will be stored as branches...
      //ran::Event* event_;
      ran::EventInfo evtInfo{};
      std::vector<ran::ElectronStruct>* electronCollection;
      std::vector<ran::MuonStruct>* muonCollection;
      std::vector<ran::JetStruct>* jetCollection;
      bool vBool_;
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
MiniAnalyzer::MiniAnalyzer(const edm::ParameterSet& iConfig):
   //now do what ever initialization is needed
    vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
    muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
    electronToken_(consumes<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
    //tauToken_(consumes<pat::TauCollection>(iConfig.getParameter<edm::InputTag>("taus"))),
    //photonToken_(consumes<pat::PhotonCollection>(iConfig.getParameter<edm::InputTag>("photons"))),
    jetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
    fatjetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("fatjets"))),
    metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets")))
{
    EventDataTree = fHistos->make<TTree>("EventDataTree", "Event data tree");

    //Setting up the links between variables and branches...
    //event_ = 0;	
    //EventDataTree->Branch("event","ran::Event", &event_, 64000, 1); // This line was taken from Jim's tupiliser
    EventDataTree->Branch("evtInfo","ran::EventInfo",&evtInfo);
    EventDataTree->Branch("electronCollection","std::vector<ran::ElectronStruct>", &electronCollection, 64000, 1); 
    EventDataTree->Branch("muonCollection","std::vector<ran::MuonStruct>", &muonCollection, 64000, 1); 
    EventDataTree->Branch("jetCollection","std::vector<ran::JetStruct>", &jetCollection, 64000, 1);

}


MiniAnalyzer::~MiniAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MiniAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle<reco::VertexCollection> vertices;
    iEvent.getByToken(vtxToken_, vertices);
    if (vertices->empty()) return; // skip the event if no PV found
    //const reco::Vertex &PV = vertices->front();

    edm::Handle<pat::MuonCollection> muons;
    iEvent.getByToken(muonToken_, muons);
    for (const pat::Muon &mu : *muons) {
      //float del = mu.p4();
      int cdel;
      cdel =mu.charge();
      /*bool bdel = mu.isGlobalMuon();
      bdel = mu.isTrackerMuon();
      bdel = mu.isStandAloneMuon(); */
      cdel = mu.numberOfMatchedStations();
      //del = muon.dB();
      cdel = cdel +2;
      /*if (bdel){
	printf("\n");
	}*/

        if (mu.pt() < 5 || !mu.isLooseMuon()) continue;
	// printf("muon with pt %4.1f, dz(PV) %+5.3f, POG loose id %d, tight id %d\n",
        //        mu.pt(), mu.muonBestTrack()->dz(PV.position()), mu.isLooseMuon(), mu.isTightMuon(PV));
    }

    edm::Handle<pat::ElectronCollection> electrons;
    iEvent.getByToken(electronToken_, electrons);
    for (const pat::Electron &el : *electrons) {
        if (el.pt() < 5) continue;
        //printf("elec with pt %4.1f, supercluster eta %+5.3f, sigmaIetaIeta %.3f (%.3f with full5x5 shower shapes), pass conv veto %d\n", el.pt(), el.superCluster()->eta(), el.sigmaIetaIeta(), el.full5x5_sigmaIetaIeta(), el.passConversionVeto());
    }

    /* edm::Handle<pat::PhotonCollection> photons;
    iEvent.getByToken(photonToken_, photons);
    for (const pat::Photon &pho : *photons) {
        if (pho.pt() < 20 or pho.chargedHadronIso()/pho.pt() > 0.3) continue;
        //printf("phot with pt %4.1f, supercluster eta %+5.3f, sigmaIetaIeta %.3f\n",
        //            pho.pt(), pho.superCluster()->eta(), pho.sigmaIetaIeta());
    }
    edm::Handle<pat::TauCollection> taus;
    iEvent.getByToken(tauToken_, taus);
    for (const pat::Tau &tau : *taus) {
        if (tau.pt() < 20) continue;
        //printf("tau  with pt %4.1f, dxy signif %.1f, ID(byMediumCombinedIsolationDeltaBetaCorr3Hits) %.1f, lead candidate pt %.1f, pdgId %d \n",
        //            tau.pt(), tau.dxy_Sig(), tau.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits"), tau.leadCand()->pt(), tau.leadCand()->pdgId());
    }
    */

    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(jetToken_, jets);
    /*int ijet = 0;
    for (const pat::Jet &j : *jets) {
        if (j.pt() < 20) continue;
        //printf("jet  with pt %5.1f (raw pt %5.1f), eta %+4.2f, btag CSV %.3f, CISV %.3f, pileup mva disc %+.2f\n",
        //    j.pt(), j.pt()*j.jecFactor("Uncorrected"), j.eta(), std::max(0.f,j.bDiscriminator("combinedSecondaryVertexBJetTags")), std::max(0.f,j.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags")), j.userFloat("pileupJetId:fullDiscriminant"));
        if ((++ijet) == 1) { // for the first jet, let's print the leading constituents
            std::vector<reco::CandidatePtr> daus(j.daughterPtrVector());
            std::sort(daus.begin(), daus.end(), [](const reco::CandidatePtr &p1, const reco::CandidatePtr &p2) { return p1->pt() > p2->pt(); }); // the joys of C++11
            for (unsigned int i2 = 0, n = daus.size(); i2 < n && i2 <= 3; ++i2) {
	      //const pat::PackedCandidate &cand = dynamic_cast<const pat::PackedCandidate &>(*daus[i2]);
                //printf("         constituent %3d: pt %6.2f, dz(pv) %+.3f, pdgId %+3d\n", i2,cand.pt(),cand.dz(PV.position()),cand.pdgId());
            }
	    }
	    }*/


    edm::Handle<pat::JetCollection> fatjets;
    iEvent.getByToken(fatjetToken_, fatjets);
    //for (const pat::Jet &j : *fatjets) {
      //printf("AK8j with pt %5.1f (raw pt %5.1f), eta %+4.2f, mass %5.1f ungroomed, %5.1f pruned, %5.1f trimmed, %5.1f filtered. CMS TopTagger %.1f\n",
      //      j.pt(), j.pt()*j.jecFactor("Uncorrected"), j.eta(), j.mass(), j.userFloat("ak8PFJetsCHSPrunedLinks"), j.userFloat("ak8PFJetsCHSTrimmedLinks"), j.userFloat("ak8PFJetsCHSFilteredLinks"), j.userFloat("cmsTopTagPFJetsCHSLinksAK8"));
    //}

    edm::Handle<pat::METCollection> mets;
    iEvent.getByToken(metToken_, mets);
    //const pat::MET &met = mets->front();
    //printf("MET: pt %5.1f, phi %+4.2f, sumEt (%.1f). genMET %.1f. MET with JES up/down: %.1f/%.1f\n",
    //    met.pt(), met.phi(), met.sumEt(),
    //    met.genMET()->pt(),
    //    met.shiftedPt(pat::MET::JetEnUp), met.shiftedPt(pat::MET::JetEnDown));

    //printf("\n");

    electronCollection = new std::vector<ran::ElectronStruct>();
    muonCollection = new std::vector<ran::MuonStruct>();
    jetCollection = new std::vector<ran::JetStruct>();

    //Clearing contents/setting default values of variables that should get new values in each event...
    ResetEventByEventVariables();

    //Reading in the event information...
    ReadInEvtInfo(vBool_, iEvent);     

    //Reading in the kin variables for the standard and special reco'n GSF electrons... 

    ReadInMuons(iEvent);
      
    //Read in Electrons
    ReadInElectrons(iEvent);

    //Read in Jets
    ReadInJets(iEvent);

    //Fill Ntuple
    EventDataTree->Fill();	

    //delete event_;
    delete electronCollection;
    delete muonCollection;
    delete jetCollection;

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
MiniAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MiniAnalyzer::endJob() 
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
MiniAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
MiniAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MiniAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MiniAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MiniAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MiniAnalyzer);

//------------ method for clearing contents/setting default values of variables that should get new values in each event -------------
void MiniAnalyzer::ResetEventByEventVariables(){
	
	//Resetting the values of the event information variables...
  	evtInfo.runNum = 0;
	evtInfo.evtNum = 0;
	evtInfo.lumiSec = 0;
	
  	
	
}



//----------------------------------------------------------------------------------------------------
//------------ method for reading in the event information (run no., lumi sec etc...)  ---------------
void MiniAnalyzer::ReadInEvtInfo(bool beVerbose, const edm::Event& edmEventObject){

      evtInfo.runNum = edmEventObject.id().run();
      evtInfo.lumiSec = edmEventObject.id().luminosityBlock();
      evtInfo.evtNum = edmEventObject.id().event(); 
}

void MiniAnalyzer::ReadInElectrons(const edm::Event& iEvent)
{
    edm::Handle<pat::ElectronCollection> electrons;
    iEvent.getByToken(electronToken_, electrons);
    for (const pat::Electron &iEle : *electrons) {
      electronCollection->push_back(ran::ElectronStruct{});
      ran::ElectronStruct &ithElec = electronCollection->back();
	//UNESSCESSARY ADDITONAL COPY. SHOULD FILL DIRECTLY INTO THE VECTOR RATHER THAN CREATING AN OBJECT, FILLING IT AND THEN COPYING IT IN THE VECTOR
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

    }

}
//Read in jet vars
void MiniAnalyzer::ReadInJets(const edm::Event& iEvent)
{
    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(jetToken_, jets);
    for (const pat::Jet &iJet: *jets) {
      jetCollection->push_back(ran::JetStruct{});

      ran::JetStruct &ithJet = jetCollection->back();
      ithJet.pt = iJet.pt();
      ithJet.et = iJet.et();
      ithJet.eta = iJet.eta();
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

//------------ method for reading in the values of the standard GSF electron variables ---------------
//----------------------------------------------------------------------------------------------------
//-------- Method for reading in the muon information, and dumping it into tsw::Event class ----------
void MiniAnalyzer::ReadInMuons(const edm::Event& iEvent){

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
