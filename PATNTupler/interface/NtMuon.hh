#ifndef NTMUON_HH
#define NTMUON_HH

//RAL PARTICLE HEADERS
#include "../interface/MuonStruct.hh"

namespace ran{
  class NtMuon{
  public:
    NtMuon() : muon(0){} ///< Default constructor
    NtMuon(const ran::MuonStruct& aMuon) : muon(&aMuon){} ///< Constructor
    double pt() const {return muon->pt;} ///<Particle pt
    double eta() const {return muon->eta;}///<Particle eta
    ROOT::Math::XYZTVector p4() const {return muon->p4;}
    int charge() const {return muon->charge;}
    bool isGlobalMuon() const {return muon->isGlobalMuon;}
    bool isTrackerMuon() const {return muon->isTrackerMuon;}
    bool isStandAloneMuon() const {return muon->isStandAloneMuon;}
    int numMatchedMuonStns() const {return muon->numMatchedMuonStns;}
    
    bool isPFIsolationValid() const {return muon->isPFIsolationValid;}///<Is particle flow isolation valid
    double pfIsoR03_sumChgHadPt() const {return muon->pfIsoR03_sumChgHadPt;} ///<sum-pt of charged Hadro
    double pfIsoR03_chgPartPt() const {return muon->pfIsoR03_chgPartPt;}///<sum-pt of charged Particles(inludes e/mu) 
    double pfIsoR03_sumNeutHadPt() const {return muon->pfIsoR03_sumNeutHadPt;}///<sum pt of neutral hadrons 
    double pfIsoR03_sumPhtEt() const {return muon->pfIsoR03_sumPhtEt;}///<sum pt of PF photons 
    bool isIsolationValid() const {return muon->isIsolationValid;} ///<Is isolation valid 
    double isolR03_sumPt() const {return muon->isolR03_sumPt;}
    double isolR03_emEt() const {return muon->isolR03_emEt;}
    double isolR03_hadEt() const {return muon->isolR03_hadEt;}

    double dxy() const {return muon->dxy;} ///< No quality cuts on the primary vertex
    double dz() const {return muon->dz;} ///< No quality cuts on the primary vertex

  
    // Kinematic variables from the global track ..

    bool globTrk_exists() const {return muon->globTrk_exists;}
    double globTrk_pT() const {return muon->globTrk_pT;}
    double globTrk_eta() const {return muon->globTrk_eta;}
    double globTrk_phi() const {return muon->globTrk_phi;}
    int globTrk_charge() const {return muon->globTrk_charge;}
    int globTrk_numberOfValidMuonHits() const {return muon->globTrk_numberOfValidMuonHits;}
    double globTrk_normalisedChi2() const {return muon->globTrk_normalisedChi2;}

    // ... and from the inner track ...

    bool inTrk_exists() const {return muon->inTrk_exists;}
    double inTrk_pT() const {return muon->inTrk_pT;}
    double inTrk_eta() const {return muon->inTrk_eta;}
    double inTrk_phi() const {return muon->inTrk_phi;}
    int inTrk_charge() const {return muon->inTrk_charge;}
    int inTrk_numValidPixHits() const {return muon->inTrk_numValidPixHits;}
    int inTrk_numValidTrkrHits() const {return muon->inTrk_numValidTrkrHits;}
    double inTrk_dxyVsOrigin() const {return muon->inTrk_dxyVsOrigin;}
    int trk_trkrLayersWHits() const {return muon->trk_trkrLayersWHits;}

    // ... and from the outer track ...

    bool outTrk_exists() const {return muon->outTrk_exists;}
    double outTrk_pT() const {return muon->outTrk_pT;}
    double outTrk_eta() const {return muon->outTrk_eta;}
    double outTrk_phi() const {return muon->outTrk_phi;}
    int outTrk_charge() const {return muon->outTrk_charge;}

    // ... and from 'best' track ...

    bool bestTrk_exists() const {return muon->bestTrk_exists;}
    double bestTrk_dxy_bspot() const {return muon->bestTrk_dxy_bspot;}///<Beamspot vertex used
    double bestTrk_dxy_vtx() const {return muon->bestTrk_dxy_vtx;}///< Quality cuts applied to the primary vertex
    double bestTrk_dz_vtx() const {return muon->bestTrk_dz_vtx;}///< Quality cuts applied to the primary vertex

    double pTune_pt() const {return muon->pTune_pt;}///<Muon pt called from muon::tevOptimized. Only required for releases younger than CMSSW_5_3_6_patch1
    double pTune_ptError() const {return muon->pTune_ptError;}///<  Muon pt error called from muon::tevOptimized. Only required for releases younger than CMSSW_5_3_6_patch1
    double pTune_dxy() const {return muon->pTune_dxy;}///< Muon dxy. Best muon quantities called from muon::tevOptimized. Only required for releases younger than CMSSW_5_3_6_patch1
    double pTune_dz() const {return muon->pTune_dz;}///< Muon dz. Best muon quantities called from muon::tevOptimized. Only required for releases younger than CMSSW_5_3_6_patch1


  private:
    const ran::MuonStruct* muon;

  };
}

#endif
