#ifndef JETSTRUCT_HH
#define JETSTRUCT_HH

#include <TObject.h>

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct JetStruct: public TObject{
    JetStruct(); ///< default constructor
    virtual ~JetStruct(){} ///< destructor
    double et;///< et
    double pt;/// < pt
    double mass;/// < mass
    double eta;///< eta 
    double jetProbabilityBJetTags;///< jetProbabilityBJetTags b tag discriminator
    double jetBProbabilityBJetTags;///< jetBProbabilityBJetTags b tag discriminator
    double trackCountingHighEffBJetTags;///< trackCountingHighEffBJetTags b tag discriminator
    double trackCountingHighPurBJetTags;///< trackCountingHighPurBJetTags b tag discriminator
    double simpleSecondaryVertexHighEffBJetTags;///< simpleSecondaryVertexHighEffBJetTags b tag discriminator
    double simpleSecondaryVertexHighPurBJetTags;///< combinedSecondaryVertexBJetTags b tag discriminator
    double combinedInclusiveSecondaryVertexBJetTags;///< combinedInclusiveSecondaryVertexBJetTags b tag discriminator
    double jecFactor_unCorrected;///<factor applied to jet pt to get uncorrect jet pt
    double userFloat_pileupJetId_fullDiscriminant;///< userFloat pileup jet ID
    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(JetStruct,1)//Struct class definition
  };

}


#endif
