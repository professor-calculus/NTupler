#ifndef NTJET_HH
#define NTJET_HH

#include "../interface/JetStruct.hh"

namespace ran{
  class NtJet{
  public:
    NtJet() : jet(0){} ///< Default constructor
    NtJet(const ran::JetStruct& aJet) : jet(&aJet){} ///< Constructor
    double et() const {return jet->et;} ///<Particle et
    double eta() const {return jet->eta;}///<Particle eta
    double jetProbabilityBJetTags() const {return jet->jetProbabilityBJetTags;}///< jetProbabilityBJetTags b tag discriminator
    double jetBProbabilityBJetTags() const {return jet->jetBProbabilityBJetTags;}///< jetBProbabilityBJetTags b tag discriminator
    double trackCountingHighEffBJetTags() const {return jet->trackCountingHighEffBJetTags;}///< trackCountingHighEffBJetTags b tag discriminator
    double trackCountingHighPurBJetTags() const {return jet->trackCountingHighPurBJetTags;}///< trackCountingHighPurBJetTags b tag discriminator
    double simpleSecondaryVertexBJetTags() const {return jet->simpleSecondaryVertexBJetTags;}///< simpleSecondaryVertexBJetTags b tag discriminator
    double combinedSecondaryVertexBJetTags() const {return jet->combinedSecondaryVertexBJetTags;}///< combinedSecondaryVertexBJetTags b tag discriminator
    double combinedSecondaryVertexMVABJetTags() const {return jet->combinedSecondaryVertexMVABJetTags;}///< combinedSecondaryVertexMVABJetTags b tag discriminator
    double impactParameterMVABJetTags() const {return jet->impactParameterMVABJetTags;}///< impactParameterMVABJetTags b tag discriminator
    double softElectronBJetTags() const {return jet->softElectronBJetTags;}///< softElectronBJetTags b tag discriminator
    double softMuonBJetTags() const {return jet->softMuonBJetTags;}///< softMuonBJetTags b tag discriminator
    double softMuonNoIPBJetTags() const {return jet->softMuonNoIPBJetTags;}///< softMuonNoIPBJetTags b tag discriminator
    int partonFlavour() const {return jet->partonFlavour;}///<  MC parton flavour (sensible values for MC only!) 

  private:
    const ran::JetStruct* jet;

  };
}

#endif
