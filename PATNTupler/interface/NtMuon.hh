#ifndef NTMUON_HH
#define NTMUON_HH

//RAL PARTICLE HEADERS
#include "../interface/MuonStruct.hh"

namespace ran{
  class NtMuon{
  public:
    NtMuon() : muon(0){} ///< Default constructor
    NtMuon(const ran::MuonStruct& aMuon) : muon(&aMuon){} ///< Constructor
    float pt() const {return muon->pt;} ///<Particle pt
    float et() const {return muon->et;} ///<Particle et
    float eta() const {return muon->eta;}///<Particle eta
    float phi() const {return muon->phi;}///<Particle phi
    ROOT::Math::XYZTVector p4() const {return muon->p4;}
    int charge() const {return muon->charge;}
    bool isGlobalMuon() const {return muon->isGlobalMuon;}
    bool isTrackerMuon() const {return muon->isTrackerMuon;}
    bool isStandAloneMuon() const {return muon->isStandAloneMuon;}
    bool isHighPtMuon() const {return muon->isHighPtMuon;} ///< is this a high pt muon
    int numMatchedMuonStns() const {return muon->numMatchedMuonStns;}
    
    bool isPFIsolationValid() const {return muon->isPFIsolationValid;}///<Is particle flow isolation valid
    float pfIsoR03_sumChgHadPt() const {return muon->pfIsoR03_sumChgHadPt;} ///<sum-pt of charged Hadro
    float pfIsoR03_chgPartPt() const {return muon->pfIsoR03_chgPartPt;}///<sum-pt of charged Particles(inludes e/mu) 
    float pfIsoR03_sumNeutHadPt() const {return muon->pfIsoR03_sumNeutHadPt;}///<sum pt of neutral hadrons 
    float pfIsoR03_sumPhtEt() const {return muon->pfIsoR03_sumPhtEt;}///<sum pt of PF photons
    float pfIsoR03_sumNeutHadPtHighThr() const {return muon->pfIsoR03_sumNeutHadPtHighThr;} ///< sum pt of neutral hadrons with a higher threshold
    float pfIsoR03_sumPhtEtHighThr() const {return muon-> pfIsoR03_sumPhtEtHighThr;} ///< sum pt of PF photons with a higher threshold
    float pfIsoR03_sumPUPt() const {return muon->pfIsoR03_sumPUPt;} ///< sum pt of charged Particles not from PV (for Pu corrections) 
    float pfIsoR04_sumChgHadPt() const {return muon->pfIsoR03_sumChgHadPt;} ///<sum-pt of charged Hadro
    float pfIsoR04_chgPartPt() const {return muon->pfIsoR03_chgPartPt;}///<sum-pt of charged Particles(inludes e/mu) 
    float pfIsoR04_sumNeutHadPt() const {return muon->pfIsoR03_sumNeutHadPt;}///<sum pt of neutral hadrons 
    float pfIsoR04_sumPhtEt() const {return muon->pfIsoR03_sumPhtEt;}///<sum pt of PF photons
    float pfIsoR04_sumNeutHadPtHighThr() const {return muon->pfIsoR03_sumNeutHadPtHighThr;} ///< sum pt of neutral hadrons with a higher threshold
    float pfIsoR04_sumPhtEtHighThr() const {return muon-> pfIsoR03_sumPhtEtHighThr;} ///< sum pt of PF photons with a higher threshold
    float pfIsoR04_sumPUPt() const {return muon->pfIsoR03_sumPUPt;} ///< sum pt of charged Particles not from PV (for Pu corrections) 
    bool isIsolationValid() const {return muon->isIsolationValid;} ///<Is isolation valid 
    float isolR03_sumPt() const {return muon->isolR03_sumPt;}
    float isolR03_emEt() const {return muon->isolR03_emEt;}
    float isolR03_hadEt() const {return muon->isolR03_hadEt;}

    float dxy() const {return muon->dxy;} ///< No quality cuts on the primary vertex
    float dz() const {return muon->dz;} ///< No quality cuts on the primary vertex

  
    // Kinematic variables from the global track ..

    bool globTrk_exists() const {return muon->globTrk_exists;}
    float globTrk_pT() const {return muon->globTrk_pT;}
    float globTrk_eta() const {return muon->globTrk_eta;}
    float globTrk_phi() const {return muon->globTrk_phi;}
    int globTrk_charge() const {return muon->globTrk_charge;}
    int globTrk_numberOfValidMuonHits() const {return muon->globTrk_numberOfValidMuonHits;}
    int globTrk_numberOfValidPixelHits() const {return muon->globTrk_numberOfValidPixelHits;}
    int globTrk_trackerLayersWithMeasurement() const {return muon->globTrk_trackerLayersWithMeasurement;}
    float globTrk_normalisedChi2() const {return muon->globTrk_normalisedChi2;}
    float globTrk_dxy() const {return muon->globTrk_dxy;}
    float globTrk_dz() const {return muon->globTrk_dz;}

    // ... and from the inner track ...

    bool inTrk_exists() const {return muon->inTrk_exists;}
    float inTrk_pT() const {return muon->inTrk_pT;}
    float inTrk_eta() const {return muon->inTrk_eta;}
    float inTrk_phi() const {return muon->inTrk_phi;}
    int inTrk_charge() const {return muon->inTrk_charge;}
    int inTrk_numValidPixHits() const {return muon->inTrk_numValidPixHits;}
    int inTrk_numValidTrkrHits() const {return muon->inTrk_numValidTrkrHits;}
    float inTrk_dxyVsOrigin() const {return muon->inTrk_dxyVsOrigin;}
    float inTrk_dxy() const {return muon->inTrk_dxy;}
    float inTrk_dz() const {return muon->inTrk_dz;}
    int trk_trkrLayersWHits() const {return muon->trk_trkrLayersWHits;}

    // ... and from the outer track ...

    bool outTrk_exists() const {return muon->outTrk_exists;}
    float outTrk_pT() const {return muon->outTrk_pT;}
    float outTrk_eta() const {return muon->outTrk_eta;}
    float outTrk_phi() const {return muon->outTrk_phi;}
    int outTrk_charge() const {return muon->outTrk_charge;}
    float outTrk_dxy() const {return muon->outTrk_dxy;}
    float outTrk_dz() const {return muon->outTrk_dz;}

    // ... and from 'best' track ...

    bool bestTrk_exists() const {return muon->bestTrk_exists;}
    float bestTrk_dxy_bspot() const {return muon->bestTrk_dxy_bspot;}///<Beamspot vertex used
    float bestTrk_dxy_vtx() const {return muon->bestTrk_dxy_vtx;}///< Quality cuts applied to the primary vertex
    float bestTrk_dz_vtx() const {return muon->bestTrk_dz_vtx;}///< Quality cuts applied to the primary vertex

    //Tune P muons

    bool tuneP_exists() const {return muon->tuneP_exists;}///< tune P exist
    float tuneP_pt() const {return muon->tuneP_pt;}///< pt
    float tuneP_eta() const {return muon->tuneP_eta;}///< eta
    float tuneP_phi() const {return muon->tuneP_phi;}///< phi
    float tuneP_charge() const {return muon->tuneP_charge;}///< charge
    float tuneP_ptError() const {return muon->tuneP_ptError;}///< pt error 
    float tuneP_dxy() const {return muon->tuneP_dxy;}///< dxy
    float tuneP_dz() const {return muon->tuneP_dz;}///< dxy

  private:
    const ran::MuonStruct* muon;

  };
}

#endif
