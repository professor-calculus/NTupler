#ifndef NTJET_HH
#define NTJET_HH

#include "../interface/JetStruct.hh"

namespace ran{
  class NtJet{
  public:
    NtJet() : jet(0){} ///< Default constructor
    NtJet(const ran::JetStruct& aJet) : jet(&aJet){} ///< Constructor
    float pt() const {return jet->pt;} ///<Particle pt
    float et() const {return jet->et;} ///<Particle et
    float eta() const {return jet->eta;}///<Particle eta
    float phi() const {return jet->phi;}///<Particle phi
    float mass() const {return jet->mass;} ///<Particle mass
    float pfJetProbabilityBJetTags() const {return jet->pfJetProbabilityBJetTags;}///< pfJetProbabilityBJetTags b tag discriminator
    float pfJetBProbabilityBJetTags() const {return jet->pfJetBProbabilityBJetTags;}///< pfJetBProbabilityBJetTags b tag discriminator
    float pfTrackCountingHighEffBJetTags() const {return jet->pfTrackCountingHighEffBJetTags;}///< pfTrackCountingHighEffBJetTags b tag discriminator
    float pfTrackCountingHighPurBJetTags() const {return jet->pfTrackCountingHighPurBJetTags;}///< pfTrackCountingHighPurBJetTags b tag discriminator
    float pfSimpleSecondaryVertexHighEffBJetTags() const {return jet->pfSimpleSecondaryVertexHighEffBJetTags;}///< pfSimpleSecondaryVertexHighEffBJetTags b tag discriminator
    float pfSimpleSecondaryVertexHighPurBJetTags() const {return jet->pfSimpleSecondaryVertexHighPurBJetTags;}///< pfSimpleSecondaryVertexHighPurBJetTags b tag discriminator
    float pfCombinedInclusiveSecondaryVertexV2BJetTags() const {return jet->pfCombinedInclusiveSecondaryVertexV2BJetTags;}///< pfCombinedInclusiveSecondaryVertexV2BJetTags b tag discriminator
    float pfCombinedSecondaryVertexSoftLeptonBJetTags() const {return jet->pfCombinedSecondaryVertexSoftLeptonBJetTags;}//< pfCombinedSecondaryVertexSoftLeptonBJetTags b tag discriminator
    float pfCombinedMVABJetTags() const {return jet->pfCombinedMVABJetTags;}///< pfCombinedMVABJetTags  b tag discriminator
    float jecFactor_unCorrected() const {return jet->jecFactor_unCorrected;}///< factor applied to jet pt to get uncorrect jet pt
    float userFloat_pileupJetId_fullDiscriminant() const {return jet->userFloat_pileupJetId_fullDiscriminant;}///<  userFloat pileup jet ID

    int partonFlavour() const {return jet->partonFlavour;}///<  MC parton flavour (sensible values for MC only!) 

  private:
    const ran::JetStruct* jet;

  };
}

#endif
