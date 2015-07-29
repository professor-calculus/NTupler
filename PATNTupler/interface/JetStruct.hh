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
    float jetProbabilityBJetTags;///< jetProbabilityBJetTags b tag discriminator
    float jetBProbabilityBJetTags;///< jetBProbabilityBJetTags b tag discriminator
    float trackCountingHighEffBJetTags;///< trackCountingHighEffBJetTags b tag discriminator
    float trackCountingHighPurBJetTags;///< trackCountingHighPurBJetTags b tag discriminator
    float simpleSecondaryVertexHighEffBJetTags;///< simpleSecondaryVertexHighEffBJetTags b tag discriminator
    float simpleSecondaryVertexHighPurBJetTags;///< combinedSecondaryVertexBJetTags b tag discriminator
    float combinedInclusiveSecondaryVertexBJetTags;///< combinedInclusiveSecondaryVertexBJetTags b tag discriminator
    float jecFactor_unCorrected;///<factor applied to jet pt to get uncorrect jet pt
    float userFloat_pileupJetId_fullDiscriminant;///< userFloat pileup jet ID
    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(JetStruct,1)//Struct class definition
  };

}


#endif
