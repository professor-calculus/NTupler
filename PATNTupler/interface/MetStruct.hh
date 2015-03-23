#ifndef METSTRUCT_HH
#define METSTRUCT_HH

#include <TObject.h>

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct MetStruct: public TObject{
    MetStruct(); ///< default constructor
    virtual ~MetStruct(){} ///< destructor
    float pt;/// < pt
    float eta;/// < eta
    float phi;/// < phi
    float sumEt;///< sumET     
    float genMet_pt;
    bool isCaloMET;///< True if the MET was made from a reco::CaloMET
    bool isPFMET;///< True if the met was made from reco::pfMET
    bool isRecoMET;///<True if met was made from reco::pfMET
    float shiftedPt_JetEnUp;
    float shiftedPt_JetEnDown;
    ClassDef(MetStruct,1)//Struct class definition
  };

}


#endif
