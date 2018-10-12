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
    float pfCombinedSecondaryVertexV2BJetTags() const {return jet->pfCombinedSecondaryVertexV2BJetTags;}///< pfCombinedSecondaryVertexV2BJetTags b tag discriminator
    float pfCombinedInclusiveSecondaryVertexV2BJetTags() const {return jet->pfCombinedInclusiveSecondaryVertexV2BJetTags;}//<pfCombinedInclusiveSecondaryVertexV2BJetTags b tag discriminator
    float pfCombinedDeepCSV_probb() const {return jet->pfCombinedDeepCSV_probb;} // For the Deep NN b-tagger
    float pfCombinedDeepCSV_probbb() const {return jet->pfCombinedDeepCSV_probbb;} // For the Deep NN b-tagger
    float pfCombinedDeepCVMA_probb() const {return jet->pfCombinedDeepCVMA_probb;} // For the Deep NN b-tagger
    float pfCombinedDeepCVMA_probbb() const {return jet->pfCombinedDeepCVMA_probbb;} // For the Deep NN b-tagger
    float pfCombinedDeepCSV_problepb() const {return jet->pfCombinedDeepCSV_problepb;} // For the Deep NN b-tagger
    float pfCombinedMVAV2BJetTags() const {return jet->pfCombinedMVAV2BJetTags;}///< pfCombinedMVABJetTags  b tag discriminator
    float jecFactor_unCorrected() const {return jet->jecFactor_unCorrected;}///< factor applied to jet pt to get uncorrect jet pt
    float jecUncertainty() const {return jet->jecUncertainty;}///< the uncertainty in the JEC applied
    float jerUncUp() const {return jet->jerUncUp;}///< the uncertainty in the JER (up)
    float jerUncDown() const {return jet->jerUncDown;}///< the uncertainty in the JER (down)
    float userFloat_pileupJetId_fullDiscriminant() const {return jet->userFloat_pileupJetId_fullDiscriminant;}///<  userFloat pileup jet ID
    float numberOfDaughters() const {return jet->numberOfDaughters;}///< Number of Jet daughters
    int chargedMultiplicity() const {return jet->chargedMultiplicity;}///< charged multiplicity
    int neutralMultiplicity() const {return jet->neutralMultiplicity;}///< neutral multiplicity
    float muonEnergyFraction() const {return jet->muonEnergyFraction;}///< muonHadronEnergyFraction
    float electronEnergyFraction() const {return jet->electronEnergyFraction;}///< electronHadronEnergyFraction
    float neutralHadronEnergyFraction() const {return jet->neutralHadronEnergyFraction;}///< neutralHadronEnergyFraction
    float HFHadronEnergyFraction() const {return jet->HFHadronEnergyFraction;}///< HFHadronEnergyFraction
    float neutralEmEnergyFraction() const {return jet->neutralEmEnergyFraction;}///< neutralEmEnergyFraction
    float chargedEmEnergyFraction() const {return jet->chargedEmEnergyFraction;}///<chargedEmEnergyFraction
    float chargedHadronEnergyFraction() const {return jet->chargedHadronEnergyFraction;}///<chargedHadronEnergyFraction

    int partonFlavour() const {return jet->partonFlavour;}///<  MC parton flavour (sensible values for MC only!) 

  private:
    const ran::JetStruct* jet;

  };
}

#endif
