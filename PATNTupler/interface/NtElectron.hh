#ifndef NTELECTRON_HH
#define NTELECTRON_HH

//RAL PARTICLE HEADERS
#include "../interface/ElectronStruct.hh"

namespace ran{
  class NtElectron{
  public:
    NtElectron() : electron(0){} ///< Default constructor
    NtElectron(const ran::ElectronStruct& anElectron) : electron(&anElectron){} ///< Constructor
    float pt() const {return electron->pt;} ///<Particle pt
    float et() const {return electron->pt;} ///<Particle et
    float eta() const {return electron->eta;}///<Particle eta
    float phi() const {return electron->phi;}///<Particle phi

    bool gsfTrack_available() const {return electron->gsfTrack_available;}///< Is GSF track available
    float scEta() const {return electron->scEta;}///< super cluster eta
    float scEnergy() const {return electron->scEnergy;}///< super cluster energy
    bool ecalDrivenSeed() const {return electron->ecalDrivenSeed;}///< is ecal driven
    float e2x5Max() const {return electron->e2x5Max;}///< e2x5 max
    float e5x5() const {return electron->e5x5;}///< e5x5
    float e1x5() const {return electron->e1x5;}///< e1x5
    float deltaPhiSuperClusterTrackAtVtx() const {return electron->deltaPhiSuperClusterTrackAtVtx;}
    float hadronicOverEm() const {return electron->hadronicOverEm;}///< H/E 
    //int nrMissHits() const {return electron->nrMissHits;}///< Number of missing hits
    float scSigmaIEtaIEta() const {return electron->scSigmaIEtaIEta;}
    float dr03EcalRecHitSumEt() const {return electron->dr03EcalRecHitSumEt;}
    float dr03HcalDepth1TowerSumEt() const {return electron->dr03HcalDepth1TowerSumEt;}
    float dr03TkSumPt() const {return electron->dr03TkSumPt;}
    float sigmaIetaIeta() const {return electron->sigmaIetaIeta;}///< sigmaIetaIeta
    float full5x5_sigmaIetaIeta() const {return electron->full5x5_sigmaIetaIeta;}///<full5x5_sigmaIetaIeta
    bool passConversionVeto() const {return electron->passConversionVeto;}///< passConversionVeto 
    int inner_missing_hits() const {return electron->inner_missing_hits;}///<number of inner track missing hits
    bool isEB() const {return electron->isEB;}///< GSF is EB
    bool isEE() const {return electron->isEE;}///< GSF is EE

    //HEEP variables
    bool  heep_isPatEle() const {return electron-> heep_isPatEle;}///<Is this a PAT electron
    float heep_et() const {return electron->heep_et;}///< HEEP et
    float heep_gsfEt() const {return electron->heep_gsfEt;}///< HEEP GSF et
    float heep_scEt() const {return electron->heep_scEt;}///< HEEP supercluster et
    float heep_energy() const {return electron->heep_energy;}///< HEEP energy
    float heep_gsfEnergy() const {return electron->heep_gsfEnergy;}///< HEEP gsf energy
    float heep_caloEnergy() const {return electron->heep_caloEnergy;}///< HEEP calo energy
    float heep_ecalEnergyError() const {return electron->heep_ecalEnergyError;}///< HEEP ecal energy error
    float heep_eta() const {return electron->heep_eta;}///< HEEP eta
    float heep_scEta() const {return electron->heep_scEta;}///< HEEP uper cluster eta
    float heep_detEta() const {return electron->heep_detEta;}///< HEEP delta eta
    float heep_detEtaAbs() const {return electron->heep_detEtaAbs;}///< HEEP absolute delta eta
    float heep_phi() const {return electron->heep_phi;}///< HEEP phi
    float heep_scPhi() const {return electron->heep_scPhi;}///< HEEP supercluster phi
    float heep_detPhi() const {return electron->heep_detPhi;}///< HEEP delta phi
    float heep_zVtx() const {return electron->heep_zVtx;}///< HEEP x vertex
    XYZTLorentzVector heep_p4() const {return electron->heep_p4;}///< HEEP four momentum 
    XYZTLorentzVector heep_gsfP4() const {return electron->heep_gsfP4;}///< HEEP gsf four momentum
    int heep_classification() const {return electron->heep_classification;}///< HEEP classification
    bool heep_isEcalDriven() const {return electron->heep_isEcalDriven;}///< HEEP is ecaldrien
    bool heep_isTrackerDriven() const {return electron->heep_isTrackerDriven;}///< HEEP is tracker driven
    bool heep_isEB() const {return electron->heep_isEB;}///< HEEP is EB 
    bool heep_isEE() const {return electron->heep_isEE;}///< HEEP is EE
    int heep_charge() const {return electron->heep_charge;}///< HEEP heep charge
    int heep_trkCharge() const {return electron->heep_trkCharge;}///< HEEP track charge
    float heep_pVtx() const {return electron->heep_pVtx;}///< HEEP
    float heep_pCalo() const {return electron->heep_pCalo;}///< HEEP
    float heep_ptVtx() const {return electron->heep_ptVtx;}///< HEEP
    float heep_ptCalo() const {return electron->heep_ptCalo;}///< HEEP
    /*float heep_closestCtfTrk_pt() const {return electron->heep_closestCtfTrk_pt;}///< HEEP
    float heep_closestCtfTrk_eta() const {return electron->heep_closestCtfTrk_eta;}///< HEEP
    float heep_closestCtfTrk_phi() const {return electron->heep_closestCtfTrk_phi;}///< HEEP*/
    float heep_hOverE() const {return electron->heep_hOverE;}///< HEEP
    float heep_dEtaIn() const {return electron->heep_dEtaIn;}///< HEEP
    float heep_dPhiIn() const {return electron->heep_dPhiIn;}///< HEEP
    float heep_dPhiOut() const {return electron->heep_dPhiOut;}///< HEEP
    float heep_epIn() const {return electron->heep_epIn;}///< HEEP
    float heep_epOut() const {return electron->heep_epOut;}///< HEEP
    float heep_fbrem() const {return electron->heep_fbrem;}///< HEEP
    float heep_bremFrac() const {return electron->heep_bremFrac;}///< HEEP
    float heep_invEOverInvP() const {return electron->heep_invEOverInvP;}///< HEEP}
    //float heep_sigmaEtaEtaFull5x5() const {return electron->heep_sigmaEtaEtaFull5x5;}///< HEEP
    float heep_sigmaEtaEtaUnCorrFull5x5() const {return electron->heep_sigmaEtaEtaUnCorrFull5x5;}///< HEEP
    float heep_sigmaIEtaIEtaFull5x5() const {return electron->heep_sigmaIEtaIEtaFull5x5;}///< HEEP
    float heep_e1x5Full5x5() const {return electron->heep_e1x5Full5x5;}///< HEEP
    float heep_e2x5MaxFull5x5() const {return electron->heep_e2x5MaxFull5x5;}///< HEEP
    float heep_e5x5Full5x5() const {return electron->heep_e5x5Full5x5;}///< HEEP
    float heep_e1x5Over5x5Full5x5() const {return electron->heep_e1x5Over5x5Full5x5;}///< HEEP
    float heep_e2x5MaxOver5x5Full5x5() const {return electron->heep_e2x5MaxOver5x5Full5x5;}///< HEEP
    //float heep_scSigmaEtaEta() const {return electron->heep_scSigmaEtaEta;}///< HEEP
    /*float heep_scSigmaEtaEtaUnCorr() const {return electron->heep_scSigmaEtaEtaUnCorr;}///< HEEP
    float heep_scSigmaIEtaIEta() const {return electron->heep_scSigmaIEtaIEta;}///< HEEP
    float heep_scE1x5() const {return electron->heep_scE1x5;}///< HEEP
    float heep_scE2x5Max() const {return electron->heep_scE2x5Max;}///< HEEP
    float heep_scE5x5() const {return electron->heep_scE5x5;}///< HEEP
    float heep_scE1x5Over5x5() const {return electron->heep_scE1x5Over5x5;}///< HEEP
    float heep_scE2x5MaxOver5x5() const {return electron->heep_scE2x5MaxOver5x5;}///< HEEP*/
    float heep_rhoForIsolCorr() const {return electron->heep_rhoForIsolCorr;}///< HEEP
    float heep_applyRhoIsolCorr() const {return electron->heep_applyRhoIsolCorr;}///< HEEP
    float heep_isolEm() const {return electron->heep_isolEm;}///< HEEP
    float heep_isolHad() const {return electron->heep_isolHad;}///< HEEP
    float heep_isolHadDepth1() const {return electron->heep_isolHadDepth1;}///< HEEP
    float heep_isolHadDepth2() const {return electron->heep_isolHadDepth2;}///< HEEP
    float heep_isolPtTrks() const {return electron->heep_isolPtTrks;}///< HEEP
    float heep_isolEmHadDepth1() const {return electron->heep_isolEmHadDepth1;}///< HEEP
    float heep_isolPtTrksRel03() const {return electron->heep_isolPtTrksRel03;}///< HEEP
    float heep_isolEmRel03() const {return electron->heep_isolEmRel03;}///< HEEP
    float heep_isolHadRel03() const {return electron->heep_isolHadRel03;}///< HEEP
    int heep_numMissInnerHits() const {return electron->heep_numMissInnerHits;}///< HEEP
    float heep_dxy() const { return electron->heep_dxy;}///<HEEP dxy
    //int heep_cutCode() const {return electron->heep_cutCode;}///< HEEP cutcode. INVALID = 0x40000000
    bool passHEEPID() const { return electron->passHEEPID;}///< Pass HEEP ID
    float HEEPtrkIsol() const { return electron->HEEPtrkIsol;}///< HEEP trk Isolation
    
  private:
    const ran::ElectronStruct* electron;

  };
}

#endif
