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
    float pfCombinedInclusiveSecondaryVertexV2BJetTags;///< pfCombinedInclusiveSecondaryVertexV2BJetTags b tag discriminator
    float pfCombinedSecondaryVertexSoftLeptonBJetTags;///< pfCombinedSecondaryVertexSoftLeptonBJetTags b tag discriminator
    float pfCombinedMVABJetTags;///<  pfCombinedMVABJetTags b tag discriminator
    float jecFactor_unCorrected;///<factor applied to jet pt to get uncorrect jet pt
    float userFloat_pileupJetId_fullDiscriminant;///< userFloat pileup jet ID
    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(JetStruct,1)//Struct class definition
  };

}


#endif
