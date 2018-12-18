#ifndef NTPHOTON_HH
#define NTPHOTON_HH

//RAL PARTICLE HEADERS
#include "../interface/PhotonStruct.hh"

namespace ran{
  class NtPhoton{
  public:
    NtPhoton() : photon(0){} ///< Default constructor
    NtPhoton(const ran::PhotonStruct& anPhoton) : photon(&anPhoton){} ///< Constructor
    float pt() const {return photon->pt;} ///<Particle pt
    float et() const {return photon->et;} ///<Particle et
    float eta() const {return photon->eta;}///<Particle eta
    float phi() const {return photon->phi;}///<Particle phi

    bool gsfTrack_available() const {return photon->gsfTrack_available;}///< Is GSF track available
    float scEta() const {return photon->scEta;}///< super cluster eta
    float scEnergy() const {return photon->scEnergy;}///< super cluster energy
    float e5x5() const {return photon->e5x5;}///< e5x5
    float e1x5() const {return photon->e1x5;}///< e1x5
    float hadronicOverEm() const {return photon->hadronicOverEm;}///< H/E 
    float pfIso_chHadrIso() const {return photon->pfIso_chHadrIso;}
    float pfIso_neuHadrIso() const {return photon->pfIso_neuHadrIso;}
    float pfIso_photonIso() const {return photon->pfIso_photonIso;}
    float sigmaIetaIeta() const {return photon->sigmaIetaIeta;}///< sigmaIetaIeta
    float full5x5_sigmaIetaIeta() const {return photon->full5x5_sigmaIetaIeta;}///<full5x5_sigmaIetaIeta
    bool passConversionVeto() const {return photon->passConversionVeto;}///< passConversionVeto 
    bool passElectronVeto() const {return photon->passElectronVeto;}
    int inner_missing_hits() const {return photon->inner_missing_hits;}///<number of inner track missing hits
    bool isEB() const {return photon->isEB;}///< GSF is EB
    bool isEE() const {return photon->isEE;}///< GSF is EE
    
  private:
    const ran::PhotonStruct* photon;

  };
}

#endif