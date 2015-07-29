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
    float jetProbabilityBJetTags;///< jetProbabilityBJetTags b tag discriminator
    float jetBProbabilityBJetTags;///< jetBProbabilityBJetTags b tag discriminator
    float trackCountingHighEffBJetTags;///< trackCountingHighEffBJetTags b tag discriminator
    float trackCountingHighPurBJetTags;///< trackCountingHighPurBJetTags b tag discriminator
    float simpleSecondaryVertexHighEffBJetTags;///< simpleSecondaryVertexHighEffBJetTags b tag discriminator
    float simpleSecondaryVertexHighPurBJetTags;///< combinedSecondaryVertexBJetTags b tag discriminator
    float combinedInclusiveSecondaryVertexBJetTags;///< combinedInclusiveSecondaryVertexBJetTags b tag discriminator
    float jecFactor_unCorrected;///<factor applied to jet pt to get uncorrect jet pt
    float userFloat_ak8PFJets_CHSPrunedLinks;///< Pruned jets
    float userFloat_ak8PFJets_CHSTrimmedLinks;///< Trimed jets
    float userFloat_ak8PFJets_CHSFilteredLinks;///< Filtered jets
    float userFloat_cmsTopTag_PFJets_CHSLinksAK8;///< CMS Top Tagger

    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(FatJetStruct,1)//Struct class definition
  };

}


#endif
