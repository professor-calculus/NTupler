#ifndef TRACKSTRUCT_HH
#define TRACKSTRUCT_HH

#include <TObject.h>
#include "Math/LorentzVector.h"
#include "TLorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > XYZTLorentzVectorD;
typedef XYZTLorentzVectorD XYZTLorentzVector;

/// RAL Analysis Ntuples library namespace
namespace ran{

  class TrackStruct: public TObject{
  public:
    TrackStruct(); ///< default constructor
    virtual ~TrackStruct(){} ///< virtual destructor
    float et;///< track et
    float pt;///< track pt
    float eta;///< track eta
    float phi;//track phi

    bool fromPV; // is from Primary Vertex
    int pdgId;

    ClassDef(TrackStruct,1)//TrackStruct class definition
  };

}


#endif
