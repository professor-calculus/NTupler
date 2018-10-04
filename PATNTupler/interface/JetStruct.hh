#ifndef JETSTRUCT_HH
#define JETSTRUCT_HH

#include <TObject.h>

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct JetStruct: public TObject{
    JetStruct(); ///< default constructor
    virtual ~JetStruct(){} ///< destructor
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
    float pfCombinedSecondaryVertexV2BJetTags;///< pfCombinedSecondaryVertexV2BJetTags b tag discriminator
    float pfCombinedInclusiveSecondaryVertexV2BJetTags;///< pfCombinedInclusiveSecondaryVertexV2BJetTags b tag discriminator
    float pfCombinedMVAV2BJetTags;///<  pfCombinedMVABJetTags b tag discriminator
    float pfCombinedDeepCSV_probb; // Deep NN b-tagger
    float pfCombinedDeepCSV_probbb; // Deep NN b-tagger
    float pfCombinedDeepCSV_problepb; // Deep NN b-tagger
    float jecFactor_unCorrected;///<factor applied to jet pt to get uncorrect jet pt
    float jecUncertainty;///<the uncertainty in the JEC applied
    float jerUncUp;///<the uncertainty in the JER (up)
    float jerUncDown;///<the uncertainty in the JER (down)
    float userFloat_pileupJetId_fullDiscriminant;///< userFloat pileup jet ID
    float numberOfDaughters;///< Number of Jet daughters
    int chargedMultiplicity;///< charged multiplicity
    int neutralMultiplicity;///< neutral multiplicity
    float muonEnergyFraction;///< muon energy fraction
    float electronEnergyFraction;///< electron energy fraction
    float neutralHadronEnergyFraction;///< neutralHadronEnergyFraction
    float HFHadronEnergyFraction;///< HFHadronEnergyFraction
    float neutralEmEnergyFraction;///< neutralEmEnergyFraction
    float chargedEmEnergyFraction;///<chargedEmEnergyFraction
    float chargedHadronEnergyFraction;///<chargedHadronEnergyFraction
    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(JetStruct,2)//Struct class definition
  };

}


#endif
