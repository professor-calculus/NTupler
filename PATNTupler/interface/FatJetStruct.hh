#ifndef FATJETSTRUCT_HH
#define FATJETSTRUCT_HH

#include <TObject.h>

/// RAL Analysis Ntuples library namespace
namespace ran{

  struct FatJetStruct: public TObject{
    FatJetStruct(); ///< default constructor
    virtual ~FatJetStruct(){} ///< destructor
    double et;///< et
    double pt;/// < pt
    double mass;/// < mass
    double eta;///< eta 
    double phi;/// < phi
    double jetProbabilityBJetTags;///< jetProbabilityBJetTags b tag discriminator
    double jetBProbabilityBJetTags;///< jetBProbabilityBJetTags b tag discriminator
    double trackCountingHighEffBJetTags;///< trackCountingHighEffBJetTags b tag discriminator
    double trackCountingHighPurBJetTags;///< trackCountingHighPurBJetTags b tag discriminator
    double simpleSecondaryVertexHighEffBJetTags;///< simpleSecondaryVertexHighEffBJetTags b tag discriminator
    double simpleSecondaryVertexHighPurBJetTags;///< combinedSecondaryVertexBJetTags b tag discriminator
    double combinedInclusiveSecondaryVertexBJetTags;///< combinedInclusiveSecondaryVertexBJetTags b tag discriminator
    double jecFactor_unCorrected;///<factor applied to jet pt to get uncorrect jet pt
    double userFloat_ak8PFJets_CHSPrunedLinks;///< Pruned jets
    double userFloat_ak8PFJets_CHSTrimmedLinks;///< Trimed jets
    double userFloat_ak8PFJets_CHSFilteredLinks;///< Filtered jets
    double userFloat_cmsTopTag_PFJets_CHSLinksAK8;///< CMS Top Tagger

    int partonFlavour;///<  MC parton flavour (sensible values for MC only obviously!) 
    ClassDef(FatJetStruct,1)//Struct class definition
  };

}


#endif
