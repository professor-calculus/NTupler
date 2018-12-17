#ifndef PHOTONSTRUCT_HH
#define PHOTONSTRUCT_HH

#include <TObject.h>
#include "Math/LorentzVector.h"
#include "TLorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > XYZTLorentzVectorD;
typedef XYZTLorentzVectorD XYZTLorentzVector;

/// RAL Analysis Ntuples library namespace
namespace ran{

  class PhotonStruct: public TObject{
  public:
    PhotonStruct(); ///< default constructor
    virtual ~PhotonStruct(){} ///< virtual destructor
    float et;///< photon et
    float pt;///< photon pt
    float eta;///< photon eta
    float phi;//photon phi

    bool gsfTrack_available;///< Is GSF track available
    float scEta;///< super cluster eta
    float scEnergy;///< super cluster energy
    float e5x5;///< e5x5
    float e1x5;///< e1x5 
    float hadronicOverEm;///< H/E
    float pfIso_chHadrIso;//For photon ID criteria
    float pfIso_neuHadrIso;// " "
    float pfIso_photonIso;// " "
    bool passElectronVeto;
    float sigmaIetaIeta;///< sigmaIetaIeta
    float full5x5_sigmaIetaIeta;///<full5x5_sigmaIetaIeta
    bool passConversionVeto;///< passConversionVeto
    int inner_missing_hits;///<number of inner track missing hits
    bool isEB;///< GSF is EB
    bool isEE;///< GSF is EE 

    ClassDef(PhotonStruct,1)//PhotonStruct class definition
  };

}


#endif