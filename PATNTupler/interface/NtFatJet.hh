#ifndef NTFATJET_HH
#define NTFATJET_HH

#include "../interface/FatJetStruct.hh"

namespace ran{
  class NtFatJet{
  public:
    NtFatJet() : jet(0){} ///< Default constructor
    NtFatJet(const ran::FatJetStruct& aJet) : jet(&aJet){} ///< Constructor
    float pt() const {return jet->pt;} ///<Particle pt
    float et() const {return jet->et;} ///<Particle et
    float eta() const {return jet->eta;}///<Particle eta
    float phi() const {return jet->phi;}///<Particle phi
    float mass() const {return jet->mass;} ///<Particle mass
    float pfJetProbabilityBJetTags() const {return jet->pfJetProbabilityBJetTags;}///< jetProbabilityBJetTags b tag discriminator
    float pfJetBProbabilityBJetTags() const {return jet->pfJetBProbabilityBJetTags;}///< jetBProbabilityBJetTags b tag discriminator
    float pfTrackCountingHighEffBJetTags() const {return jet->pfTrackCountingHighEffBJetTags;}///< trackCountingHighEffBJetTags b tag discriminator
    float pfTrackCountingHighPurBJetTags() const {return jet->pfTrackCountingHighPurBJetTags;}///< trackCountingHighPurBJetTags b tag discriminator
    float pfSimpleSecondaryVertexHighEffBJetTags() const {return jet->pfSimpleSecondaryVertexHighEffBJetTags;}///< simpleSecondaryVertexHighEffBJetTags b tag discriminator
    float pfSimpleSecondaryVertexHighPurBJetTags() const {return jet->pfSimpleSecondaryVertexHighPurBJetTags;}///< simpleSecondaryVertexHighPurBJetTags b tag discriminator
    float pfCombinedInclusiveSecondaryVertexV2BJetTags() const {return jet->pfCombinedInclusiveSecondaryVertexV2BJetTags;}///< combinedInclusiveSecondaryVertexBJetTags b tag discriminator
    float pfCombinedSecondaryVertexSoftLeptonBJetTags() const {return jet->pfCombinedSecondaryVertexSoftLeptonBJetTags;}//< pfCombinedSecondaryVertexSoftLeptonBJetTags b tag discriminator
    float pfCombinedMVABJetTags() const {return jet->pfCombinedMVABJetTags;}///< pfCombinedMVABJetTags  b tag discriminator
    float jecFactor_unCorrected() const {return jet->jecFactor_unCorrected;}///< factor applied to jet pt to get uncorrect jet pt
    float userFloat_ak8PFJets_CHSPrunedLinks() const {return jet->userFloat_ak8PFJets_CHSPrunedLinks;}///< Pruned jets
    float userFloat_ak8PFJets_CHSTrimmedLinks() const {return jet-> userFloat_ak8PFJets_CHSTrimmedLinks;}///< Trimed jets
    float userFloat_ak8PFJets_CHSFilteredLinks() const {return jet->userFloat_ak8PFJets_CHSFilteredLinks;}///< Filtered jets
    float userFloat_cmsTopTag_PFJets_CHSLinksAK8() const {return jet->userFloat_cmsTopTag_PFJets_CHSLinksAK8;}///< CMS Top Tagger
    float numberOfDaughters() const {return jet->numberOfDaughters;}///< Number of Jet daughters
    float chargedMultiplicity() const {return jet->chargedMultiplicity;}///< charged multiplicity
    float neutralHadronEnergyFraction() const {return jet->neutralHadronEnergyFraction;}///< neutralHadronEnergyFraction
    float HFHadronEnergyFraction() const {return jet->HFHadronEnergyFraction;}///< HFHadronEnergyFraction
    float neutralEmEnergyFraction() const {return jet->neutralEmEnergyFraction;}///< neutralEmEnergyFraction
    float chargedEmEnergyFraction() const {return jet->chargedEmEnergyFraction;}///<chargedEmEnergyFraction
    float chargedHadronEnergyFraction() const {return jet->chargedHadronEnergyFraction;}///<chargedHadronEnergyFraction

    int partonFlavour() const {return jet->partonFlavour;}///<  MC parton flavour (sensible values for MC only!) 

  private:
    const ran::FatJetStruct* jet;

  };
}

#endif
