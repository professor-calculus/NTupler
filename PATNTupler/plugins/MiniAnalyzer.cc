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
      edm::EDGetTokenT<pat::TauCollection> tauToken_;
      edm::EDGetTokenT<pat::PhotonCollection> photonToken_;
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
    tauToken_(consumes<pat::TauCollection>(iConfig.getParameter<edm::InputTag>("taus"))),
    photonToken_(consumes<pat::PhotonCollection>(iConfig.getParameter<edm::InputTag>("photons"))),
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
    const reco::Vertex &PV = vertices->front();

    edm::Handle<pat::MuonCollection> muons;
    iEvent.getByToken(muonToken_, muons);
    for (const pat::Muon &mu : *muons) {
        if (mu.pt() < 5 || !mu.isLooseMuon()) continue;
        printf("muon with pt %4.1f, dz(PV) %+5.3f, POG loose id %d, tight id %d\n",
                mu.pt(), mu.muonBestTrack()->dz(PV.position()), mu.isLooseMuon(), mu.isTightMuon(PV));
    }

    edm::Handle<pat::ElectronCollection> electrons;
    iEvent.getByToken(electronToken_, electrons);
    for (const pat::Electron &el : *electrons) {
        if (el.pt() < 5) continue;
        printf("elec with pt %4.1f, supercluster eta %+5.3f, sigmaIetaIeta %.3f (%.3f with full5x5 shower shapes), pass conv veto %d\n",
                    el.pt(), el.superCluster()->eta(), el.sigmaIetaIeta(), el.full5x5_sigmaIetaIeta(), el.passConversionVeto());
    }

    edm::Handle<pat::PhotonCollection> photons;
    iEvent.getByToken(photonToken_, photons);
    for (const pat::Photon &pho : *photons) {
        if (pho.pt() < 20 or pho.chargedHadronIso()/pho.pt() > 0.3) continue;
        printf("phot with pt %4.1f, supercluster eta %+5.3f, sigmaIetaIeta %.3f\n",
                    pho.pt(), pho.superCluster()->eta(), pho.sigmaIetaIeta());
    }
    edm::Handle<pat::TauCollection> taus;
    iEvent.getByToken(tauToken_, taus);
    for (const pat::Tau &tau : *taus) {
        if (tau.pt() < 20) continue;
        printf("tau  with pt %4.1f, dxy signif %.1f, ID(byMediumCombinedIsolationDeltaBetaCorr3Hits) %.1f, lead candidate pt %.1f, pdgId %d \n",
                    tau.pt(), tau.dxy_Sig(), tau.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits"), tau.leadCand()->pt(), tau.leadCand()->pdgId());
    }


    edm::Handle<pat::JetCollection> jets;
    iEvent.getByToken(jetToken_, jets);
    int ijet = 0;
    for (const pat::Jet &j : *jets) {
        if (j.pt() < 20) continue;
        printf("jet  with pt %5.1f (raw pt %5.1f), eta %+4.2f, btag CSV %.3f, CISV %.3f, pileup mva disc %+.2f\n",
            j.pt(), j.pt()*j.jecFactor("Uncorrected"), j.eta(), std::max(0.f,j.bDiscriminator("combinedSecondaryVertexBJetTags")), std::max(0.f,j.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags")), j.userFloat("pileupJetId:fullDiscriminant"));
        if ((++ijet) == 1) { // for the first jet, let's print the leading constituents
            std::vector<reco::CandidatePtr> daus(j.daughterPtrVector());
            std::sort(daus.begin(), daus.end(), [](const reco::CandidatePtr &p1, const reco::CandidatePtr &p2) { return p1->pt() > p2->pt(); }); // the joys of C++11
            for (unsigned int i2 = 0, n = daus.size(); i2 < n && i2 <= 3; ++i2) {
                const pat::PackedCandidate &cand = dynamic_cast<const pat::PackedCandidate &>(*daus[i2]);
                printf("         constituent %3d: pt %6.2f, dz(pv) %+.3f, pdgId %+3d\n", i2,cand.pt(),cand.dz(PV.position()),cand.pdgId());
            }
        }
    }


    edm::Handle<pat::JetCollection> fatjets;
    iEvent.getByToken(fatjetToken_, fatjets);
    for (const pat::Jet &j : *fatjets) {
        printf("AK8j with pt %5.1f (raw pt %5.1f), eta %+4.2f, mass %5.1f ungroomed, %5.1f pruned, %5.1f trimmed, %5.1f filtered. CMS TopTagger %.1f\n",
            j.pt(), j.pt()*j.jecFactor("Uncorrected"), j.eta(), j.mass(), j.userFloat("ak8PFJetsCHSPrunedLinks"), j.userFloat("ak8PFJetsCHSTrimmedLinks"), j.userFloat("ak8PFJetsCHSFilteredLinks"), j.userFloat("cmsTopTagPFJetsCHSLinksAK8"));
    }

    edm::Handle<pat::METCollection> mets;
    iEvent.getByToken(metToken_, mets);
    const pat::MET &met = mets->front();
    printf("MET: pt %5.1f, phi %+4.2f, sumEt (%.1f). genMET %.1f. MET with JES up/down: %.1f/%.1f\n",
        met.pt(), met.phi(), met.sumEt(),
        met.genMET()->pt(),
        met.shiftedPt(pat::MET::JetEnUp), met.shiftedPt(pat::MET::JetEnDown));

    printf("\n");

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

void MiniAnalyzer::ReadInElectrons(const edm::Event& edmEvent)
{

}
//Read in jet vars
void MiniAnalyzer::ReadInJets(const edm::Event& edmEvent)
{

}

//------------ method for reading in the values of the standard GSF electron variables ---------------
//----------------------------------------------------------------------------------------------------
//-------- Method for reading in the muon information, and dumping it into tsw::Event class ----------
void MiniAnalyzer::ReadInMuons(const edm::Event& edmEvent){

}
