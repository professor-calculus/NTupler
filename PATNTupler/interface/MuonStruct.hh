#ifndef MUONSTRUCT_HH
#define MUONSTRUCT_HH

#include <TObject.h>
#include "Math/GenVector/LorentzVector.h"
#include "Math/Vector4Dfwd.h"

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct MuonStruct: public TObject{
    MuonStruct(); ///< default constructor
    virtual ~MuonStruct(){} ///<virtual distructor
    float pt ; ///< pt
    float eta ; ///< eta 
    ROOT::Math::XYZTVector p4 ; ///<
    int charge ; ///<
    bool isGlobalMuon ; ///<
    bool isTrackerMuon ; ///<
    bool isStandAloneMuon ; ///<
    int numMatchedMuonStns ; ///<
    float dB; ///<
    bool isPFIsolationValid;///<Is particle flow isolation valid
    float pfIsoR03_sumChgHadPt; ///<sum-pt of charged Hadro
    float pfIsoR03_chgPartPt;///<sum-pt of charged Particles(inludes e/mu) 
    float pfIsoR03_sumNeutHadPt;///<sum pt of neutral hadrons 
    float pfIsoR03_sumPhtEt;///<sum pt of PF photons 
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
    int globTrk_numberOfValidMuonHits ; ///<
    float globTrk_normalisedChi2 ; ///<

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
    int  trk_trkrLayersWHits ; ///<

   // ... and from the outer track ...
    bool outTrk_exists ; ///<
    float outTrk_pT ; ///<
    float outTrk_eta ; ///<
    float outTrk_phi ; ///<
    int outTrk_charge ; ///<

    // ... and from 'best' track ...
    bool   bestTrk_exists ; ///<
    float bestTrk_dxy_bspot ; ///<
    float bestTrk_dxy_vtx ; ///<
    float bestTrk_dz_vtx ; ///<

    ClassDef(MuonStruct,1)//MuonStruct class definition
  };

}


#endif
