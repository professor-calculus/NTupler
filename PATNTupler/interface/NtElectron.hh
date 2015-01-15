#ifndef NTELECTRON_HH
#define NTELECTRON_HH

//RAL PARTICLE HEADERS
#include "../interface/ElectronStruct.hh"

namespace ran{
  class NtElectron{
  public:
    NtElectron() : electron(0){} ///< Default constructor
    NtElectron(const ran::ElectronStruct& anElectron) : electron(&anElectron){} ///< Constructor
    double pt() const {return electron->pt;} ///<Particle pt
    double eta() const {return electron->eta;}///<Particle eta
 
    bool gsfTrack_available() const {return electron->gsfTrack_available;}///< Is GSF track available
    double scEta() const {return electron->scEta;}///< super cluster eta
    double scEnergy() const {return electron->scEnergy;}///< super cluster energy
    bool ecalDrivenSeed() const {return electron->ecalDrivenSeed;}///< is ecal driven
    double e2x5Max() const {return electron->e2x5Max;}///< e2x5 max
    double e5x5() const {return electron->e5x5;}///< e5x5
    double e1x5() const {return electron->e1x5;}///< e1x5
    double deltaPhiSuperClusterTrackAtVtx() const {return electron->deltaPhiSuperClusterTrackAtVtx;}
    double hadronicOverEm() const {return electron->hadronicOverEm;}///< H/E
    int nrMissHits() const {return electron->nrMissHits;}///< Number of missing hits
    double scSigmaIEtaIEta() const {return electron->scSigmaIEtaIEta;}
    double dr03EcalRecHitSumEt() const {return electron->dr03EcalRecHitSumEt;}
    double dr03HcalDepth1TowerSumEt() const {return electron->dr03HcalDepth1TowerSumEt;}
    double dr03TkSumPt() const {return electron->dr03TkSumPt;} 
    double pfIso_chgHad() const {return electron->pfIso_chgHad;}///<Particle Flow Charged hadron isolation
    double pfIso_neutHad() const {return electron->pfIso_neutHad;}///<Particle Flow neutral hadron isolation
    double pfIso_pht() const {return electron->pfIso_pht;}///<Particle Flow photon isolation

    //HEEP variables
    double heep_et() const {return electron->heep_et;}///< HEEP et
    double heep_gsfEt() const {return electron->heep_gsfEt;}///< HEEP GSF et
    double heep_scEt() const {return electron->heep_scEt;}///< HEEP supercluster et
    double heep_energy() const {return electron->heep_energy;}///< HEEP energy
    double heep_gsfEnergy() const {return electron->heep_gsfEnergy;}///< HEEP gsf energy
    double heep_caloEnergy() const {return electron->heep_caloEnergy;}///< HEEP calo energy
    double heep_ecalEnergyError() const {return electron->heep_ecalEnergyError;}///< HEEP ecal energy error
    double heep_eta() const {return electron->heep_eta;}///< HEEP eta
    double heep_scEta() const {return electron->heep_scEta;}///< HEEP uper cluster eta
    double heep_detEta() const {return electron->heep_detEta;}///< HEEP delta eta
    double heep_detEtaAbs() const {return electron->heep_detEtaAbs;}///< HEEP absolute delta eta
    double heep_phi() const {return electron->heep_phi;}///< HEEP phi
    double heep_scPhi() const {return electron->heep_scPhi;}///< HEEP supercluster phi
    double heep_detPhi() const {return electron->heep_detPhi;}///< HEEP delta phi
    double heep_zVtx() const {return electron->heep_zVtx;}///< HEEP x vertex
    XYZTLorentzVector heep_p4() const {return electron->heep_p4;}///< HEEP four momentum 
    XYZTLorentzVector heep_gsfP4() const {return electron->heep_gsfP4;}///< HEEP gsf four momentum
    int heep_classification() const {return electron->heep_classification;}///< HEEP classification
    bool heep_isEcalDriven() const {return electron->heep_isEcalDriven;}///< HEEP is ecaldrien
    bool heep_isTrackerDriven() const {return electron->heep_isTrackerDriven;}///< HEEP is tracker driven
    bool heep_isEB() const {return electron->heep_isEB;}///< HEEP is EB 
    bool heep_isEE() const {return electron->heep_isEE;}///< HEEP is EE
    int heep_charge() const {return electron->heep_charge;}///< HEEP heep charge
    int heep_trkCharge() const {return electron->heep_trkCharge;}///< HEEP track charge
    double heep_pVtx() const {return electron->heep_pVtx;}///< HEEP
    double heep_pCalo() const {return electron->heep_pCalo;}///< HEEP
    double heep_ptVtx() const {return electron->heep_ptVtx;}///< HEEP
    double heep_ptCalo() const {return electron->heep_ptCalo;}///< HEEP
    double heep_closestCtfTrk_pt() const {return electron->heep_closestCtfTrk_pt;}///< HEEP
    double heep_closestCtfTrk_eta() const {return electron->heep_closestCtfTrk_eta;}///< HEEP
    double heep_closestCtfTrk_phi() const {return electron->heep_closestCtfTrk_phi;}///< HEEP
    double heep_hOverE() const {return electron->heep_hOverE;}///< HEEP
    double heep_dEtaIn() const {return electron->heep_dEtaIn;}///< HEEP
    double heep_dPhiIn() const {return electron->heep_dPhiIn;}///< HEEP
    double heep_dPhiOut() const {return electron->heep_dPhiOut;}///< HEEP
    double heep_epIn() const {return electron->heep_epIn;}///< HEEP
    double heep_epOut() const {return electron->heep_epOut;}///< HEEP
    double heep_fbrem() const {return electron->heep_fbrem;}///< HEEP
    double heep_bremFrac() const {return electron->heep_bremFrac;}///< HEEP
    double heep_invEOverInvP() const {return electron->heep_invEOverInvP;}///< HEEP}
    //double heep_sigmaEtaEta() const {return electron->heep_sigmaEtaEta;}///< HEEP
    double heep_sigmaEtaEtaUnCorr() const {return electron->heep_sigmaEtaEtaUnCorr;}///< HEEP
    double heep_sigmaIEtaIEta() const {return electron->heep_sigmaIEtaIEta;}///< HEEP
    double heep_e1x5() const {return electron->heep_e1x5;}///< HEEP
    double heep_e2x5Max() const {return electron->heep_e2x5Max;}///< HEEP
    double heep_e5x5() const {return electron->heep_e5x5;}///< HEEP
    double heep_e1x5Over5x5() const {return electron->heep_e1x5Over5x5;}///< HEEP
    double heep_e2x5MaxOver5x5() const {return electron->heep_e2x5MaxOver5x5;}///< HEEP
    //double heep_scSigmaEtaEta() const {return electron->heep_scSigmaEtaEta;}///< HEEP
    double heep_scSigmaEtaEtaUnCorr() const {return electron->heep_scSigmaEtaEtaUnCorr;}///< HEEP
    double heep_scSigmaIEtaIEta() const {return electron->heep_scSigmaIEtaIEta;}///< HEEP
    double heep_scE1x5() const {return electron->heep_scE1x5;}///< HEEP
    double heep_scE2x5Max() const {return electron->heep_scE2x5Max;}///< HEEP
    double heep_scE5x5() const {return electron->heep_scE5x5;}///< HEEP
    double heep_scE1x5Over5x5() const {return electron->heep_scE1x5Over5x5;}///< HEEP
    double heep_scE2x5MaxOver5x5() const {return electron->heep_scE2x5MaxOver5x5;}///< HEEP
    double heep_rhoForIsolCorr() const {return electron->heep_rhoForIsolCorr;}///< HEEP
    double heep_applyRhoIsolCorr() const {return electron->heep_applyRhoIsolCorr;}///< HEEP
    double heep_isolEm() const {return electron->heep_isolEm;}///< HEEP
    double heep_isolHad() const {return electron->heep_isolHad;}///< HEEP
    double heep_isolHadDepth1() const {return electron->heep_isolHadDepth1;}///< HEEP
    double heep_isolHadDepth2() const {return electron->heep_isolHadDepth2;}///< HEEP
    double heep_isolPtTrks() const {return electron->heep_isolPtTrks;}///< HEEP
    double heep_isolEmHadDepth1() const {return electron->heep_isolEmHadDepth1;}///< HEEP
    double heep_isolPtTrksRel03() const {return electron->heep_isolPtTrksRel03;}///< HEEP
    double heep_isolEmRel03() const {return electron->heep_isolEmRel03;}///< HEEP
    double heep_isolHadRel03() const {return electron->heep_isolHadRel03;}///< HEEP
    int heep_numMissInnerHits() const {return electron->heep_numMissInnerHits;}///< HEEP
    int heep_cutCode() const {return electron->heep_cutCode;}///< HEEP cutcode. INVALID = 0x40000000
  
  private:
    const ran::ElectronStruct* electron;

  };
}

#endif
