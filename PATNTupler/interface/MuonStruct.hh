#ifndef MUONSTRUCT_HH
#define MUONSTRUCT_HH

#include <TObject.h>
#include "Math/GenVector/LorentzVector.h"
#include "Math/Vector4Dfwd.h"

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct MuonStruct: public TObject{
    MuonStruct(); ///< default constructor
    virtual ~MuonStruct(){} ///<virtual destructor
    float pt ; ///< pt
    float eta ; ///< eta 
    float phi;///< phi
    ROOT::Math::XYZTVector p4 ; ///<
    int charge ; ///<
    bool isGlobalMuon ; ///<
    bool isTrackerMuon ; ///<
    bool isPFMuon ;
    bool isLooseMuon ;
    bool isTightMuon ;
    bool isStandAloneMuon ; ///<
    bool isHighPtMuon; ///< is this a high pt muon
    int numMatchedMuonStns ; ///<
    float dB; ///<
    bool isPFIsolationValid;///<Is particle flow isolation valid
    float pfIsoR03_sumChgHadPt; ///<sum-pt of charged Hadro
    float pfIsoR03_chgPartPt;///<sum-pt of charged Particles(inludes e/mu) 
    float pfIsoR03_sumNeutHadPt;///<sum pt of neutral hadrons 
    float pfIsoR03_sumPhtEt;///<sum pt of PF photons 
    float pfIsoR03_sumNeutHadPtHighThr; ///< sum pt of neutral hadrons with a higher threshold
    float pfIsoR03_sumPhtEtHighThr;///< sum pt of PF photons with a higher threshold
    float pfIsoR03_sumPUPt;///< sum pt of charged Particles not from PV (for Pu corrections)
    float pfIsoR04_sumChgHadPt; ///<sum-pt of charged Hadro
    float pfIsoR04_chgPartPt;///<sum-pt of charged Particles(inludes e/mu) 
    float pfIsoR04_sumNeutHadPt;///<sum pt of neutral hadrons 
    float pfIsoR04_sumPhtEt;///<sum pt of PF photons 
    float pfIsoR04_sumNeutHadPtHighThr; ///< sum pt of neutral hadrons with a higher threshold
    float pfIsoR04_sumPhtEtHighThr;///< sum pt of PF photons with a higher threshold
    float pfIsoR04_sumPUPt;///< sum pt of charged Particles not from PV (for Pu corrections)
    bool isIsolationValid; ///<Is isolation valid    
    float isolR03_sumPt ; ///<
    float isolR03_emEt ; ///<
    float isolR03_hadEt ; ///<

    float dxy; ///< No quality cuts on the vertex
    float dz; ///< No quality cuts on the vertex

    // Kinematic variables from the global track ...
    bool globTrk_exists ; ///<
    float globTrk_pT ; ///<
    float globTrk_eta ; ///<
    float globTrk_phi ; ///<
    int globTrk_charge ; ///<
    float globTrk_numberOfValidPixelHits;
    int globTrk_numberOfValidMuonHits ; ///<
    float globTrk_trackerLayersWithMeasurement;
    float globTrk_normalisedChi2 ; ///<
    float globTrk_dxy;///<
    float globTrk_dz;///<

    // ... and from the inner track ...
    bool inTrk_exists ; ///<
    float inTrk_pT ; ///<
    float inTrk_eta ; ///<
    float inTrk_phi ; ///<
    int inTrk_charge ; ///<
    int inTrk_numValidPixHits ; ///<
    int inTrk_numValidTrkrHits ; ///<
    float inTrk_trackerLayersWithMeasurement; ///<
    float inTrk_dxyVsOrigin ; ///<
    float inTrk_dxy;///<
    float inTrk_dz;///<
    int  trk_trkrLayersWHits ; ///<

   // ... and from the outer track ...
    bool outTrk_exists ; ///<
    float outTrk_pT ; ///<
    float outTrk_eta ; ///<
    float outTrk_phi ; ///<
    int outTrk_charge ; ///<
    float outTrk_dxy ; ///<
    float outTrk_dz ; ///<

    // ... and from 'best' track ...
    bool   bestTrk_exists ; ///<
    float bestTrk_dxy_bspot ; ///<
    float bestTrk_dxy_vtx ; ///<
    float bestTrk_dz_vtx ; ///<

    //Tune P muons
    bool tuneP_exists; //< Tune P exist
    float tuneP_pt;///< pt
    float tuneP_eta;///< eta
    float tuneP_phi;///< phi
    float tuneP_charge;///< charge
    float tuneP_ptError;///< pt error 
    float tuneP_dxy;///< dxy
    float tuneP_dz;///< dz

    ClassDef(MuonStruct,1)//MuonStruct class definition
  };

}


#endif
