#ifndef ELECTRONSTRUCT_HH
#define ELECTRONSTRUCT_HH

#include <TObject.h>
#include "Math/LorentzVector.h"
#include "TLorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > XYZTLorentzVectorD;
typedef XYZTLorentzVectorD XYZTLorentzVector;

/// RAL Analysis Ntuples library namespace
namespace ran{

  class ElectronStruct: public TObject{
  public:
    ElectronStruct(); ///< default constructor
    virtual ~ElectronStruct(){} ///< virtual destructor
    double pt;///< electron pt
    double eta;///< electron eta

    bool gsfTrack_available;///< Is GSF track available
    double scEta;///< super cluster eta
    double scEnergy;///< super cluster energy
    bool ecalDrivenSeed;///< is ecal driven
    double e2x5Max;///< e2x5 max
    double e5x5;///< e5x5
    double e1x5;///< e1x5
    double deltaPhiSuperClusterTrackAtVtx;///< 
    double hadronicOverEm;///< H/E
    //int nrMissHits;///< Number of missing hits
    double scSigmaIEtaIEta;///< 
    double dr03EcalRecHitSumEt;///< 
    double dr03HcalDepth1TowerSumEt;///< 
    double dr03TkSumPt;///< 
    double sigmaIetaIeta;///< sigmaIetaIeta
    double full5x5_sigmaIetaIeta;///<full5x5_sigmaIetaIeta
    bool passConversionVeto;///< passConversionVeto
    /*double pfIso_chgHad;///<Particle Flow Charged hadron isolation
    double pfIso_neutHad;///<Particle Flow neutral hadron isolation
    double pfIso_pht;///<Particle Flow photon isolation  */

    
    bool heep_isPatEle;///<Is this a PAT electron
    float heep_et;///<HEEP et
    float heep_gsfEt;///<HEEP gsf electron et
    float heep_scEt;///<HEEP super cluster et
    float heep_energy;///<HEEP energy
    float heep_gsfEnergy ;///<HEEP gsf energy
    float heep_caloEnergy;///<HEEP calo energy
    float heep_ecalEnergyError;///<HEEP ecal energy error
    float heep_eta;///<HEEP eta
    float heep_scEta;///<HEEP supercluster eta
    float heep_detEta;///<HEEP delta eta
    float heep_detEtaAbs;///<HEEP absolute delta eta
    float heep_phi;///<HEEP phi
    float heep_scPhi;///<HEEP super cluster phi
    float heep_detPhi;///<HEEP delta phi
    float heep_zVtx;///<HEEP z vertex
    XYZTLorentzVector heep_p4;///<HEEP four momentum
    XYZTLorentzVector heep_gsfP4;///<HEEP gsf four momentum

    int heep_classification;///< HEEP classification
    bool heep_isEcalDriven;///< HEEP is ecal driven
    bool heep_isTrackerDriven;///< HEEP is tracker driven
    bool heep_isEB;///< HEEP is EB
    bool heep_isEE;///< HEEP is EE


    int heep_charge; ///<HEEP charge
    int heep_trkCharge;///<HEEP track charge
    float heep_pVtx;///<HEEP p Vertex
    float heep_pCalo;
    float heep_ptVtx;
    float heep_ptCalo;

    /*
    float heep_closestCtfTrk_pt;
    float heep_closestCtfTrk_eta;
    float heep_closestCtfTrk_phi;
    */

    float heep_hOverE;
    float heep_dEtaIn;
    float heep_dPhiIn;
    float heep_dPhiOut;
    float heep_epIn;
    float heep_epOut;
    float heep_fbrem;
    float heep_bremFrac;
    float heep_invEOverInvP;
    float heep_sigmaEtaEtaFull5x5;
    float heep_sigmaEtaEtaUnCorrFull5x5;
    float heep_sigmaIEtaIEtaFull5x5;
    float heep_e1x5Full5x5;
    float heep_e2x5MaxFull5x5;
    float heep_e5x5Full5x5;
    float heep_e1x5Over5x5Full5x5;
    float heep_e2x5MaxOver5x5Full5x5;
    /*
    //float heep_scSigmaEtaEta;
    float heep_scSigmaEtaEtaUnCorr;
    float heep_scSigmaIEtaIEta;
    float heep_scE1x5;
    float heep_scE2x5Max;
    float heep_scE5x5;
    float heep_scE1x5Over5x5;
    float heep_scE2x5MaxOver5x5;*/

    float heep_rhoForIsolCorr;
    bool heep_applyRhoIsolCorr;

    float heep_isolEm;
    float heep_isolHad;
    float heep_isolHadDepth1;
    float heep_isolHadDepth2;
    float heep_isolPtTrks;
    float heep_isolEmHadDepth1;
  
    float heep_isolPtTrksRel03;
    float heep_isolEmRel03;
    float heep_isolHadRel03;

    float heep_dxy;;///<HEEP dxy
    int heep_numMissInnerHits;

    int heep_cutCode;///< HEEP cutcode. INVALID = 0x40000000   

    ClassDef(ElectronStruct,1)//ElectronStruct class definition
  };

}


#endif
