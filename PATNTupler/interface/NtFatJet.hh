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
    float jetProbabilityBJetTags() const {return jet->jetProbabilityBJetTags;}///< jetProbabilityBJetTags b tag discriminator
    float jetBProbabilityBJetTags() const {return jet->jetBProbabilityBJetTags;}///< jetBProbabilityBJetTags b tag discriminator
    float trackCountingHighEffBJetTags() const {return jet->trackCountingHighEffBJetTags;}///< trackCountingHighEffBJetTags b tag discriminator
    float trackCountingHighPurBJetTags() const {return jet->trackCountingHighPurBJetTags;}///< trackCountingHighPurBJetTags b tag discriminator
    float simpleSecondaryVertexHighEffBJetTags() const {return jet->simpleSecondaryVertexHighEffBJetTags;}///< simpleSecondaryVertexHighEffBJetTags b tag discriminator
    float simpleSecondaryVertexHighPurBJetTags() const {return jet->simpleSecondaryVertexHighPurBJetTags;}///< simpleSecondaryVertexHighPurBJetTags b tag discriminator
    float combinedInclusiveSecondaryVertexBJetTags() const {return jet->combinedInclusiveSecondaryVertexBJetTags;}///< combinedInclusiveSecondaryVertexBJetTags b tag discriminator
    float jecFactor_unCorrected() const {return jet->jecFactor_unCorrected;}///< factor applied to jet pt to get uncorrect jet pt
    float userFloat_ak8PFJets_CHSPrunedLinks() const {return jet->userFloat_ak8PFJets_CHSPrunedLinks;}///< Pruned jets
    float userFloat_ak8PFJets_CHSTrimmedLinks() const {return jet-> userFloat_ak8PFJets_CHSTrimmedLinks;}///< Trimed jets
    float userFloat_ak8PFJets_CHSFilteredLinks() const {return jet->userFloat_ak8PFJets_CHSFilteredLinks;}///< Filtered jets
    float userFloat_cmsTopTag_PFJets_CHSLinksAK8() const {return jet->userFloat_cmsTopTag_PFJets_CHSLinksAK8;}///< CMS Top Tagger

    int partonFlavour() const {return jet->partonFlavour;}///<  MC parton flavour (sensible values for MC only!) 

  private:
    const ran::FatJetStruct* jet;

  };
}

#endif
