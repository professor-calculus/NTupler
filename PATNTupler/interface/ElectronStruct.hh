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
    int nrMissHits;///< Number of missing hits
    double scSigmaIEtaIEta;///< 
    double dr03EcalRecHitSumEt;///< 
    double dr03HcalDepth1TowerSumEt;///< 
    double dr03TkSumPt;///< 
    double pfIso_chgHad;///<Particle Flow Charged hadron isolation
    double pfIso_neutHad;///<Particle Flow neutral hadron isolation
    double pfIso_pht;///<Particle Flow photon isolation

    double heep_et;///<HEEP et
    double heep_gsfEt;///<HEEP gsf electron et
    double heep_scEt;///<HEEP super cluster et
    double heep_energy;///<HEEP energy
    double heep_gsfEnergy ;///<HEEP gsf energy
    double heep_caloEnergy;///<HEEP calo energy
    double heep_ecalEnergyError;///<HEEP ecal energy error
    double heep_eta;///<HEEP eta
    double heep_scEta;///<HEEP supercluster eta
    double heep_detEta;///<HEEP delta eta
    double heep_detEtaAbs;///<HEEP absolute delta eta
    double heep_phi;///<HEEP phi
    double heep_scPhi;///<HEEP super cluster phi
    double heep_detPhi;///<HEEP delta phi
    double heep_zVtx;///<HEEP z vertex
    XYZTLorentzVector heep_p4;///<HEEP four momentum
    XYZTLorentzVector heep_gsfP4;///<HEEP gsf four momentum

    int heep_classification;///< HEEP classification
    bool heep_isEcalDriven;///< HEEP is ecal driven
    bool heep_isTrackerDriven;///< HEEP is tracker driven
    bool heep_isEB;///< HEEP is EB
    bool heep_isEE;///< HEEP is EE


    int heep_charge; ///<HEEP charge
    int heep_trkCharge;///<HEEP track charge
    double heep_pVtx;///<HEEP p Vertex
    double heep_pCalo;
    double heep_ptVtx;
    double heep_ptCalo;


    double heep_closestCtfTrk_pt;
    double heep_closestCtfTrk_eta;
    double heep_closestCtfTrk_phi;

    double heep_hOverE;
    double heep_dEtaIn;
    double heep_dPhiIn;
    double heep_dPhiOut;
    double heep_epIn;
    double heep_epOut;
    double heep_fbrem;
    double heep_bremFrac;
    double heep_invEOverInvP;
    //double heep_sigmaEtaEta;
    double heep_sigmaEtaEtaUnCorr;
    double heep_sigmaIEtaIEta;
    double heep_e1x5;
    double heep_e2x5Max;
    double heep_e5x5;
    double heep_e1x5Over5x5;
    double heep_e2x5MaxOver5x5;
 
    //double heep_scSigmaEtaEta;
    double heep_scSigmaEtaEtaUnCorr;
    double heep_scSigmaIEtaIEta;
    double heep_scE1x5;
    double heep_scE2x5Max;
    double heep_scE5x5;
    double heep_scE1x5Over5x5;
    double heep_scE2x5MaxOver5x5;

    double heep_rhoForIsolCorr;
    bool heep_applyRhoIsolCorr;

    double heep_isolEm;
    double heep_isolHad;
    double heep_isolHadDepth1;
    double heep_isolHadDepth2;
    double heep_isolPtTrks;
    double heep_isolEmHadDepth1;
  
    double heep_isolPtTrksRel03;
    double heep_isolEmRel03;
    double heep_isolHadRel03;

    int heep_numMissInnerHits;

    int heep_cutCode;///< HEEP cutcode. INVALID = 0x40000000

    ClassDef(ElectronStruct,1)//ElectronStruct class definition
  };

}


#endif
