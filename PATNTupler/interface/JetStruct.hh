#ifndef JETSTRUCT_HH
#define JETSTRUCT_HH

#include <TObject.h>

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct JetStruct: public TObject{
    JetStruct(); ///< default constructor
    virtual ~JetStruct(){} ///< destructor
    double et;///< et
    double eta;///< eta 
    double jetProbabilityBJetTags;///< jetProbabilityBJetTags b tag discriminator
    double jetBProbabilityBJetTags;///< jetBProbabilityBJetTags b tag discriminator
    double trackCountingHighEffBJetTags;///< trackCountingHighEffBJetTags b tag discriminator
    double trackCountingHighPurBJetTags;///< trackCountingHighPurBJetTags b tag discriminator
    double simpleSecondaryVertexBJetTags;///< simpleSecondaryVertexBJetTags b tag discriminator
    double combinedSecondaryVertexBJetTags;///< combinedSecondaryVertexBJetTags b tag discriminator
    double combinedSecondaryVertexMVABJetTags;///< combinedSecondaryVertexMVABJetTags b tag discriminator
    double impactParameterMVABJetTags;///< impactParameterMVABJetTags b tag discriminator
    double softElectronBJetTags;///< softElectronBJetTags b tag discriminator
    double softMuonBJetTags;///< softMuonBJetTags b tag discriminator
    double softMuonNoIPBJetTags;///< softMuonNoIPBJetTags b tag discriminator
    int partonFlavour;///<  MC parton flavour (sensible values for MC only!) 
    ClassDef(JetStruct,1)//Struct class definition
  };

}


#endif
