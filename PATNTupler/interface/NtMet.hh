#ifndef NTMET_HH
#define NTMET_HH

//RAL PARTICLE HEADERS
#include "../interface/MetStruct.hh"

/// RAL Analysis Ntuples library namespace
namespace ran{

 class NtMet{
 public:
    NtMet() : met(0) {} ///< default constructor
    NtMet(const ran::MetStruct& aMet) : met(&aMet){} ///< Constructor
    virtual ~NtMet(){} ///< destructor
    float pt() const {return met->pt ;} ///< pt
    float eta() const {return met->eta ;} ///< eta
    float phi() const {return met->phi ;} ///< phi
    float sumEt() const {return met->sumEt ;} ///< sumET     
    float genMet_pt() const {return met->genMet_pt ;} ///<
    bool isCaloMET() const {return met->isCaloMET ;} ///< True if the MET was made from a reco::CaloMET
    bool isPFMET() const {return met->isPFMET ;} ///< True if the met was made from reco::pfMET
    bool isRecoMET() const {return met->isRecoMET ;} ///<True if met was made from reco::pfMET
    float shiftedPt_JetEnUp() const {return met->shiftedPt_JetEnUp ;} ///<
    float shiftedPt_JetEnDown() const {return met->shiftedPt_JetEnDown ;} ///<

  private:
    const ran::MetStruct* met;

  };

}


#endif
