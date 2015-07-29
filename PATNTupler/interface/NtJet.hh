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
    float jetProbabilityBJetTags() const {return jet->jetProbabilityBJetTags;}///< jetProbabilityBJetTags b tag discriminator
    float jetBProbabilityBJetTags() const {return jet->jetBProbabilityBJetTags;}///< jetBProbabilityBJetTags b tag discriminator
    float trackCountingHighEffBJetTags() const {return jet->trackCountingHighEffBJetTags;}///< trackCountingHighEffBJetTags b tag discriminator
    float trackCountingHighPurBJetTags() const {return jet->trackCountingHighPurBJetTags;}///< trackCountingHighPurBJetTags b tag discriminator
    float simpleSecondaryVertexHighEffBJetTags() const {return jet->simpleSecondaryVertexHighEffBJetTags;}///< simpleSecondaryVertexHighEffBJetTags b tag discriminator
    float simpleSecondaryVertexHighPurBJetTags() const {return jet->simpleSecondaryVertexHighPurBJetTags;}///< simpleSecondaryVertexHighPurBJetTags b tag discriminator
    float combinedInclusiveSecondaryVertexBJetTags() const {return jet->combinedInclusiveSecondaryVertexBJetTags;}///< combinedInclusiveSecondaryVertexBJetTags b tag discriminator
    float jecFactor_unCorrected() const {return jet->jecFactor_unCorrected;}///< factor applied to jet pt to get uncorrect jet pt
    float userFloat_pileupJetId_fullDiscriminant() const {return jet->userFloat_pileupJetId_fullDiscriminant;}///<  userFloat pileup jet ID

    int partonFlavour() const {return jet->partonFlavour;}///<  MC parton flavour (sensible values for MC only!) 

  private:
    const ran::JetStruct* jet;

  };
}

#endif
