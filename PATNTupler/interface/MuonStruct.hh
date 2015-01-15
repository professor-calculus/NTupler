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
    double pt ; ///< pt
    double eta ; ///< eta 
    ROOT::Math::XYZTVector p4 ; ///<
    int charge ; ///<
    bool isGlobalMuon ; ///<
    bool isTrackerMuon ; ///<
    bool isStandAloneMuon ; ///<
    int numMatchedMuonStns ; ///<
    double dB; ///<
    bool isPFIsolationValid;///<Is particle flow isolation valid
    double pfIsoR03_sumChgHadPt; ///<sum-pt of charged Hadro
    double pfIsoR03_chgPartPt;///<sum-pt of charged Particles(inludes e/mu) 
    double pfIsoR03_sumNeutHadPt;///<sum pt of neutral hadrons 
    double pfIsoR03_sumPhtEt;///<sum pt of PF photons 
    bool isIsolationValid; ///<Is isolation valid    
    double isolR03_sumPt ; ///<
    double isolR03_emEt ; ///<
    double isolR03_hadEt ; ///<

    double dxy; ///< No quality cuts on the vertex
    double dz; ///< No quality cuts on the vertex

    // Kinematic variables from the global track ...
    bool globTrk_exists ; ///<
    double globTrk_pT ; ///<
    double globTrk_eta ; ///<
    double globTrk_phi ; ///<
    int globTrk_charge ; ///<
    int globTrk_numberOfValidMuonHits ; ///<
    double globTrk_normalisedChi2 ; ///<

    // ... and from the inner track ...
    bool inTrk_exists ; ///<
    double inTrk_pT ; ///<
    double inTrk_eta ; ///<
    double inTrk_phi ; ///<
    int inTrk_charge ; ///<
    int inTrk_numValidPixHits ; ///<
    int inTrk_numValidTrkrHits ; ///<
    double inTrk_trackerLayersWithMeasurement; ///<
    double inTrk_dxyVsOrigin ; ///<
    int  trk_trkrLayersWHits ; ///<

   // ... and from the outer track ...
    bool outTrk_exists ; ///<
    double outTrk_pT ; ///<
    double outTrk_eta ; ///<
    double outTrk_phi ; ///<
    int outTrk_charge ; ///<

    // ... and from 'best' track ...
    bool   bestTrk_exists ; ///<
    double bestTrk_dxy_bspot ; ///<
    double bestTrk_dxy_vtx ; ///<
    double bestTrk_dz_vtx ; ///<

    double pTune_pt; ///< Muon pt called from muon::tevOptimized. Only required for releases younger than CMSSW_5_3_6_patch1
    double pTune_ptError;///< Muon pt error called from muon::tevOptimized. Only required for releases younger than CMSSW_5_3_6_patch1
    double pTune_dxy;///< Muon dxy. Best muon quantities called from muon::tevOptimized. Only required for releases younger than CMSSW_5_3_6_patch1
    double pTune_dz;///< Muon dz. Best muon quantities called from muon::tevOptimized. Only required for releases younger than CMSSW_5_3_6_patch1


    ClassDef(MuonStruct,1)//MuonStruct class definition
  };

}


#endif
