#ifndef NTJET_HH
#define NTJET_HH

#include "../interface/JetStruct.hh"

namespace ran{
  class NtJet{
  public:
    NtJet() : jet(0){} ///< Default constructor
    NtJet(const ran::JetStruct& aJet) : jet(&aJet){} ///< Constructor
    double pt() const {return jet->pt;} ///<Particle pt
    double et() const {return jet->et;} ///<Particle et
    double eta() const {return jet->eta;}///<Particle eta
    double phi() const {return jet->phi;}///<Particle phi
    double mass() const {return jet->mass;} ///<Particle mass
    double jetProbabilityBJetTags() const {return jet->jetProbabilityBJetTags;}///< jetProbabilityBJetTags b tag discriminator
    double jetBProbabilityBJetTags() const {return jet->jetBProbabilityBJetTags;}///< jetBProbabilityBJetTags b tag discriminator
    double trackCountingHighEffBJetTags() const {return jet->trackCountingHighEffBJetTags;}///< trackCountingHighEffBJetTags b tag discriminator
    double trackCountingHighPurBJetTags() const {return jet->trackCountingHighPurBJetTags;}///< trackCountingHighPurBJetTags b tag discriminator
    double simpleSecondaryVertexHighEffBJetTags() const {return jet->simpleSecondaryVertexHighEffBJetTags;}///< simpleSecondaryVertexHighEffBJetTags b tag discriminator
    double simpleSecondaryVertexHighPurBJetTags() const {return jet->simpleSecondaryVertexHighPurBJetTags;}///< simpleSecondaryVertexHighPurBJetTags b tag discriminator
    double combinedInclusiveSecondaryVertexBJetTags() const {return jet->combinedInclusiveSecondaryVertexBJetTags;}///< combinedInclusiveSecondaryVertexBJetTags b tag discriminator
    double jecFactor_unCorrected() const {return jet->jecFactor_unCorrected;}///< factor applied to jet pt to get uncorrect jet pt
    double userFloat_pileupJetId_fullDiscriminant() const {return jet->userFloat_pileupJetId_fullDiscriminant;}///<  userFloat pileup jet ID

    int partonFlavour() const {return jet->partonFlavour;}///<  MC parton flavour (sensible values for MC only!) 

  private:
    const ran::JetStruct* jet;

  };
}

#endif
