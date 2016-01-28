#ifndef FATJETSTRUCT_HH
#define FATJETSTRUCT_HH

#include <TObject.h>

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct FatJetStruct: public TObject{
    FatJetStruct(); ///< default constructor
    virtual ~FatJetStruct(){} ///< destructor
    float et;///< et
    float pt;/// < pt
    float mass;/// < mass
    float eta;///< eta 
    float phi;/// < phi
    float pfJetProbabilityBJetTags;///< pfJetProbabilityBJetTags b tag discriminator
    float pfJetBProbabilityBJetTags;///< pfJetBProbabilityBJetTags b tag discriminator
    float pfTrackCountingHighEffBJetTags;///< pfTrackCountingHighEffBJetTags b tag discriminator
    float pfTrackCountingHighPurBJetTags;///< pfTrackCountingHighPurBJetTags b tag discriminator
    float pfSimpleSecondaryVertexHighEffBJetTags;///< pfSimpleSecondaryVertexHighEffBJetTags b tag discriminator
    float pfSimpleSecondaryVertexHighPurBJetTags;///< pfCombinedSecondaryVertexBJetTags b tag discriminator
    float pfCombinedInclusiveSecondaryVertexV2BJetTags;///< pfCombinedInclusiveSecondaryVertexV2BJetTags b tag discriminator
    float pfCombinedSecondaryVertexSoftLeptonBJetTags;///< pfCombinedSecondaryVertexSoftLeptonBJetTags b tag discriminator
    float pfCombinedMVABJetTags;///<  pfCombinedMVABJetTags b tag discriminator    
    float jecFactor_unCorrected;///<factor applied to jet pt to get uncorrect jet pt
    float userFloat_ak8PFJets_CHSPrunedLinks;///< Pruned jets
    float userFloat_ak8PFJets_CHSTrimmedLinks;///< Trimed jets
    float userFloat_ak8PFJets_CHSFilteredLinks;///< Filtered jets
    float userFloat_cmsTopTag_PFJets_CHSLinksAK8;///< CMS Top Tagger
    float numberOfDaughters;///< Number of Jet daughters
    float chargedMultiplicity;///< charged multiplicity
    float neutralHadronEnergyFraction;///< neutralHadronEnergyFraction
    float HFHadronEnergyFraction;///< HFHadronEnergyFraction
    float neutralEmEnergyFraction;///< neutralEmEnergyFraction
    float chargedEmEnergyFraction;///<chargedEmEnergyFraction
    float chargedHadronEnergyFraction;///<chargedHadronEnergyFraction

    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(FatJetStruct,1)//Struct class definition
  };

}


#endif
